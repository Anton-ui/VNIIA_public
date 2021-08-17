#ifndef IK_CLASS_H
#define IK_CLASS_H

#include <QDebug>
#include <QObject>
#include <QByteArray>

#include "thread_buffer.h"

#define HAVE_REMOTE
#define IMG_WIDTH 640
#define IMG_HEIGHT 480
#define DATA_IMAGE 1280

#include <pcap.h>

class IK_class: public QObject
{
    Q_OBJECT
public:
    explicit IK_class(QObject *parent = nullptr);
    void StartCapture();
    void stop();


public slots:
    void InitThread();

private:
    static void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);
    QString CharToQstring_Address(u_char *in, u_char len);
    QString IP_Address;
    pcap_t *adhandle;

    typedef struct ip_address
    {
        u_char byte1;
        u_char byte2;
        u_char byte3;
        u_char byte4;

    }ip_address;
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
    typedef struct udp_header
    {
        u_short sport;			// Source port
        u_short dport;			// Destination port
        u_short len;			// Datagram length
        u_short crc;			// Checksum
        u_char datagram[1286];  // поменял с char на u_char
    }udp_header;

};

#endif // IK_CLASS_H
