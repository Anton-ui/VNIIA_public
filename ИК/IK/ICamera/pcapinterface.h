#ifndef PCAPINTERFACE_H
#define PCAPINTERFACE_H

#include <QObject>
#include <QMap>
#include <QDebug>
#include <QThread>
#include <string.h>
#include <stdlib.h>
#include <QStringList>

#include <pcap.h>
//#include <pcap/pcap.h>
#include <winsock2.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <iptypes.h>

#include "defines.h"
#include "triplebuf.h"



#define DEBUG


#define WORKING_BUFFER_SIZE 15000
#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

#define IPTOSBUFFERS    12

#define BUF_SIZE_RECIEVE 1328


class interfaceParams : public QObject
{
     Q_OBJECT
public:

    interfaceParams()
    {
        deviceIpAdress.clear();
        deviceIpAdress.append("255.255.255.255");
      //  findAdaptersList();
    }

    ~interfaceParams()
    {
        iAdapterInfo = 0;
        bufAdapter = 0;
        bufAdapter = 0;
        AdaptersListDescriptor.clear();
        AdaptersListIp.clear();
        AdaptersListMac.clear();
    }

    void refreshAdaptersList();
    QStringList getAdaptersListIp();
    QStringList getAdaptersListDescriptor();
    QStringList getAdaptersListMac();

    void findAdaptersList();

protected:
    //WinAPI

    PIP_ADAPTER_INFO pAdapterInfo, pAd;
    ULONG iAdapterInfo, bufAdapter;
    PIP_ADAPTER_ADDRESSES pAdapter;

    QStringList AdaptersListIp;
    QStringList AdaptersListMac;
    QStringList AdaptersListDescriptor;



    QString CharToQstring(u_char *in, u_char len);
    void QstrTochar(QString in, u_char *out, u_char len);

private:
    QString deviceIpAdress;

signals:
    void sendMsg_temp(uchar typeOfMsg, QString Msg);

};


class pcapInterface : public interfaceParams
{
    Q_OBJECT

public:

    pcapInterface()
    {
        ip_mac_dst = new ip_mac_adresses();
        ip_mac_src = new ip_mac_adresses();
    }
    pcapInterface(QString ipAdress)
    {

      ip_mac_dst = new ip_mac_adresses();
      ip_mac_src = new ip_mac_adresses();

      ipAdr.ipAdrStr.clear();
      ipAdr.ipAdrStr.append(ipAdress);

      QstrTochar(ipAdress,&ipAdr.ipAdrChr[0], 4);

      if(filter != NULL)
      {
          free(filter);
          filter = NULL;
      }

      filter = (char*)malloc(10 * sizeof(char));
      strcpy(filter, "udp or arp");
  
    }
    ~pcapInterface()
    {

    }

    static pcapInterface *getInstance();

public slots:
    void initInThread();
    void stop();
    //void SendPacket(QString pct_type, uchar *packet_data);

protected:

    typedef struct ipAdress {
        QString ipAdrStr;
        u_char  ipAdrChr[4];

    }ipAdress;

    typedef struct ip_address
    {
        u_char byte1;
        u_char byte2;
        u_char byte3;
        u_char byte4;

    }ip_address;

    typedef struct mac_address
    {
        u_char byte1;
        u_char byte2;
        u_char byte3;
        u_char byte4;
        u_char byte5;
        u_char byte6;

    }mac_address;

    typedef struct ip_mac_adresses {
        ip_address ip;
        mac_address mac;
    }ip_mac_adresses;

    typedef struct arp_header
    {
        mac_address dest_mac_broadcast; // destination mac address (0xFFFFFFFFFFFF)
        mac_address src_mac0; // source mac address
        u_short     ftype; // Frame type 0x0806
        u_short     htype; // Hardware type 0x0001
        u_short     ptype; // Protocol type 0x0800
        u_char      hlen; // MAC lenth 0x06
        u_char      plen; // IP lenth 0x04
        u_short     arp_cmd; // Arp request(0x0001), arp answer(0x0002)
        mac_address src_mac; // source MAC
        ip_address  src_ip; // source IP
        mac_address dst_mac; // destination MAC
        ip_address  dst_ip; // destination IP
    }arp_header;

    typedef struct ip_header
    {
        u_char ver_ihl;         // version (4 bits0 + Internet header length (4 bits)
        u_char tos;             // type of service
        u_short tlen;           // total length
        u_short identification; // Identification
        u_short flags_fo;       // flags (3 bits) + fragment offset (13 bits)
        u_char ttl;             // time to live;
        u_char proto;           // protocol
        u_short crc;			// Header checksum
        ip_address	saddr;		// Source address
        ip_address	daddr;		// Destination address
        u_int	op_pad;			// Option + Padding
    }ip_header;

    // UDP header
    typedef struct udp_header
    {
        u_short sport;			// Source port
        u_short dport;			// Destination port
        u_short len;			// Datagram length
        u_short crc;			// Checksum
        u_char datagram[1286];  // поменял с char на u_char
    }udp_header;

   /* typedef struct arp_table
    {
        ip_address  ip_src;
        mac_address mac_src;
    }arp_table;*/

    static ipAdress ipAdr;
    pcap_if_t *alldevs;
    pcap_if_t *d;
    pcap_t *handle;
    char *filter = NULL;

    u_int netmask;
    u_int mask;

    u_int PacketDataLen;

    //Functions
    char *iptos(u_long in);
    /*QString CharToQstring(u_char *in, u_char len);
    void QstrTochar(QString in, u_char *out, u_char len);*/



protected:

    ip_mac_adresses *ip_mac_dst;
    ip_mac_adresses *ip_mac_src;

    static void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);

signals:
    void sendMsg(uchar typeOfMsg, QString Msg);
    void sendIpMac(QString ip, QString mac);

    //Thread
    void s_finished();

};

class pcapDeviceSniffer : public pcapInterface
{
     Q_OBJECT

public:

    pcapDeviceSniffer()
    {


    }
    pcapDeviceSniffer(QString ipAdress_adapter, QString macAdress_adapter,
                      QString ipAdress_device,  QString macAdress_device,
                      TripleBuf *trBuf)
    {
        //this->ipAdr_adap.ipAdrStr.clear();
        //ipAdr_adap.ipAdrStr.append(ipAdress_adapter);

        QstrTochar(ipAdress_adapter,&this->ip_mac_src->ip.byte1, 4);
        QstrTochar(ipAdress_device,&this->ip_mac_dst->ip.byte1, 4);
        QstrTochar(macAdress_adapter,&this->ip_mac_src->mac.byte1, 6);
        QstrTochar(macAdress_device,&this->ip_mac_dst->mac.byte1, 6);

        /*ipAdr_dev.ipAdrStr.clear();
        //ipAdr_dev.ipAdrStr.append("host ");
        ipAdr_dev.ipAdrStr.append(ipAdress_device);
        QstrTochar(ipAdress_device,&ipAdr_dev.ipAdrChr[0], 4);*/

        trBuffer = trBuf;

        u_char filter_len;
        filter_len = ipAdress_device.length();
        if(filter != NULL)
        {
            free(filter);
            filter = NULL;
        }
        QString tempStr;
        tempStr.append("host ").append(ipAdress_device);
        filter = (char*)malloc((5+filter_len) * sizeof(char));
        strcpy(filter, tempStr.toStdString().data());


    }
    ~pcapDeviceSniffer()
    {
        handle_dev = NULL;
    }

    static pcapDeviceSniffer *getInstance();
    static TripleBuf *trBuffer;
    TripleBuf *getBufferPtr();




private:

    pcap_t *handle_dev;

    static void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);

public slots:
    void initInThread();
    void stop();

    void updateIpMacSettings(QString pck_type,
                             QString SourceMAC,
                             QString DestinationMAC,
                             QString SourceIP,
                             QString DestIP,
                             ushort SourcePort = 0,
                             ushort DestinationPort = 0
                             );
    void CreatePacket(uchar pck_type,
                       uchar *UserData,
                       uint UserDataLen);

signals:
    void s_finished_dev();
    void s_EchoMsg(u_char *data);
    void s_inputBufferReady();
};


#endif // PCAPINTERFACE_H
