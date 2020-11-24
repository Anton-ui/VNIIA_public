#include "pcapinterface.h"

#define IPTOSBUFFERS 12
// bytes swap
#define HTYPE 0x0100
#define PTYPE 0x0008
#define FTYPE 0x0608
#define ARP_REQUEST 0x0100
#define HLEN_PLEN 0x0406
#define OPERATION 0x0100
#define IP_TYPE 0x0008
#define HEADER_LEN 0x0045
#define IP_LEN 0x2600
#define ID     0x0100
#define FLAG_OFSET 0x0040
#define UDP_PROT 0x1140
#define CHECK_SUM 0xC7E6
#define SRC_PORT 0x24CD
#define DST_PORT 0x23CD
#define UDP_LEN_HEADER 0x1000

pcapInterface::ipAdress pcapInterface::ipAdr;

TripleBuf *pcapDeviceSniffer::trBuffer = NULL;

void interfaceParams::refreshAdaptersList()
{
    findAdaptersList();
}

QStringList interfaceParams::getAdaptersListIp()
{
    return AdaptersListIp;
}

QStringList interfaceParams::getAdaptersListDescriptor()
{
    return AdaptersListDescriptor;
}

QStringList interfaceParams::getAdaptersListMac()
{
    return AdaptersListMac;
}

void interfaceParams::findAdaptersList()
{
    iAdapterInfo = 0;
    bufAdapter = 0;
    bufAdapter = WORKING_BUFFER_SIZE;

    int iErr = GetAdaptersInfo(NULL, &iAdapterInfo);

    if(iErr && (iErr != ERROR_BUFFER_OVERFLOW))
    {
       QString str;
        str.append("Get adapters info failed\n");
        qDebug() << "Get adapters info failed";
        emit sendMsg_temp(LOG, str);

       #ifdef DEBUG
            qDebug()<<"crit", "Get adapters info failed";
       #endif
    }
    else
    {
        QString str;
        str.append("Get adapters info ok\n");
        qDebug() << "Get adapters info ok";
        //emit sendMsg_temp(LOG, str);
    }

    if(!(pAdapterInfo = (PIP_ADAPTER_INFO)GlobalAlloc(GPTR,iAdapterInfo)))
    {
        QString str;
        str.append("pAdapterInfo Memory allocating error\n");
        qDebug() << "pAdapterInfo Memory allocating error";
        emit sendMsg_temp(LOG, str);

        #ifdef DEBUG
        qDebug()<<"crit", "Memory allocating error";
        #endif
    }
    else
    {
        QString str;
        str.append("pAdapterInfo Memory allocating ok\n");
        qDebug() << "pAdapterInfo Memory allocating ok";
       // emit sendMsg_temp(LOG, str);
    }

    pAdapter = (IP_ADAPTER_ADDRESSES*) MALLOC(bufAdapter);
    if(pAdapter == NULL)
    {
        QString str;
        str.append("pAdapter Memory allocating error\n");
        qDebug() << "pAdapter Memory allocating error";
        emit sendMsg_temp(LOG, str);

        #ifdef DEBUG
         qDebug()<<"crit", "Memory allocating error";
         #endif
    }
    else
    {
        QString str;
        str.append("pAdapter Memory allocating ok\n");
        qDebug() << "pAdapter Memory allocating ok";
       // emit sendMsg_temp(LOG, str);
    }

    if(GetAdaptersInfo(pAdapterInfo, &iAdapterInfo))
    {
        QString str;
        str.append("GetAdaptersInfo failed\n");
        qDebug() << "GetAdaptersInfo failed";
        emit sendMsg_temp(LOG, str);
        #ifdef DEBUG
        qDebug()<<"crit", "GetAdaptersInfo failed";
        #endif
    }
    else
    {
        QString str;
        str.append("GetAdaptersInfo ok\n");
        qDebug() << "GetAdaptersInfo ok";
       // emit sendMsg_temp(LOG, str);
    }

    AdaptersListIp.clear();
    AdaptersListDescriptor.clear();
    AdaptersListMac.clear();
    for(pAdapterInfo; pAdapterInfo->Next != NULL; pAdapterInfo = pAdapterInfo->Next )
    {
        AdaptersListDescriptor.append(pAdapterInfo->Description);
        AdaptersListIp.append(pAdapterInfo->IpAddressList.IpAddress.String);
        AdaptersListMac.append( CharToQstring(pAdapterInfo->Address,6));
    }
    QString txt_temp;
    txt_temp.clear();
    txt_temp.append(QString::number(AdaptersListDescriptor.length()));
    //emit sendMsg_temp(LOG, txt_temp);
}


pcapInterface *pcapInterface::getInstance()
{
    static pcapInterface *m_instance = new pcapInterface();

     if (m_instance == 0)
         m_instance = new pcapInterface();
     return m_instance;
}

TripleBuf *pcapDeviceSniffer::getBufferPtr()
{
    return trBuffer;
}

void pcapInterface::initInThread()
{
    alldevs = new pcap_if_t;
    d = new pcap_if_t;

    u_char *devIp;
    char errbuf[PCAP_ERRBUF_SIZE];
    char devNum = 0;
    bool flag_dev_find = false;

   // ip_mac_dst = new ip_mac_adresses();
    //ip_mac_src = new ip_mac_adresses();

    //char *filter = "udp or arp"; // filter for BPF
    struct bpf_program fp; // compiled filter
    bpf_u_int32 maskp;
    bpf_u_int32 netp;

    if(pcap_findalldevs(&alldevs, errbuf) == -1)
    {
        QString str;
        str.append("Error in find all devices\n Errbuf: ").append(errbuf);
        sendMsg(LOG, str);
        pcap_freealldevs(alldevs);

    }

    for(d = alldevs; d!=NULL; d = d->next)
    {
        pcap_addr_t *dev_addr;

        for(dev_addr = d->addresses; dev_addr != NULL; dev_addr = dev_addr->next)
        {
            if(dev_addr->addr->sa_family == AF_INET && dev_addr->addr && dev_addr->netmask)
            {
               devIp = (u_char *)&(((struct sockaddr_in *)dev_addr->addr)->sin_addr.s_addr);
               if( !(QString::compare(CharToQstring(devIp, 4), ipAdr.ipAdrStr, Qt::CaseInsensitive)))
               {
                   flag_dev_find = true;
                   goto found;
               }
            }
        }
        devNum++;
    }
    found: // go to our device

    if(flag_dev_find)
    {
        qDebug()<<"PcapInterface thread: "<< QThread::currentThreadId();


        d = alldevs;
        for(int i = 0; i < devNum; i++)
            d = d->next;

        if ((handle = pcap_open_live(d->name,
                                       BUF_SIZE_RECIEVE, //65536
                                       1,  // 1 неразборчивый режим (без фильтрации на уровне карты)
                                       0, // 1000
                                       errbuf
                                       )) == NULL)
        {
            // вывод ошибки
            pcap_freealldevs(alldevs);
            QString str;
            str.append("Error in pcap open live\n Errbuf: ").append(errbuf);
            sendMsg(LOG, str);
            pcap_freealldevs(alldevs);
        }

        if (pcap_datalink (handle) != DLT_EN10MB) // Check if its Ethernet connection
        {
            // send error
            QString str;
            str.append("Its's not Ethernet connection");
            sendMsg(LOG, str);
            pcap_freealldevs(alldevs);
        }

        if(pcap_lookupnet(d->name, &netp, &maskp, errbuf ) == -1)
        {
            //error
            QString str;
            str.append("Error in pcap lookupnet\n Errbuf: ").append(errbuf);
            sendMsg(LOG, str);
            pcap_freealldevs(alldevs);
        }

        //Compile filter
        if(pcap_compile(handle, &fp, filter, 0, maskp) == -1)
        {
            //error
            QString str;
            str.append("Can't compile filter");
            sendMsg(LOG, str);
            pcap_freealldevs(alldevs);
        }

        //Set Filter
        if(pcap_setfilter(handle, &fp) < 0)
        {
            QString str;
            str.append("Can't set filter");
            sendMsg(LOG, str);
            pcap_freealldevs(alldevs);
        }

        //start capture
        pcap_loop(handle, 0, packet_handler, NULL);
        qDebug() << "Interface capture started;";

    }
    else
    {
        pcap_freealldevs(alldevs);
        sendMsg(MSG_BOX_CRITICAL, "Device not found!");
        //quiet from thread and close it
    }


}

void pcapInterface::stop()
{
    pcap_breakloop(handle);
    emit pcapInterface::s_finished();
}

//void pcapInterface::SendPacket(QString pct_type, uchar *packet_data)
//{

//}

char *pcapInterface::iptos(u_long in)
{
    static char output[IPTOSBUFFERS][3*4+3+1];
    static short which;
    u_char *p;

    p = (u_char *)&in;
    which = (which + 1 == IPTOSBUFFERS ? 0 : which + 1);
    _snprintf_s(output[which], sizeof(output[which]), sizeof(output[which]),"%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
    return output[which];

}

void pcapDeviceSniffer::updateIpMacSettings(QString pck_type,
                                            QString SourceMAC, QString DestinationMAC,
                                            QString SourceIP, QString DestIP,
                                            ushort SourcePort, ushort DestinationPort)
{
    if(pck_type == "IP_MAC_DATA")
    {
        QstrTochar(SourceMAC, &ip_mac_src->mac.byte1,6);
        QstrTochar(SourceIP, &ip_mac_src->ip.byte1,4);
        QstrTochar(DestinationMAC, &ip_mac_dst->mac.byte1,6);
        QstrTochar(DestIP, &ip_mac_dst->ip.byte1,4);
    }

}

QString interfaceParams::CharToQstring(u_char *in, u_char len)
{
    QString out;

    if(len == 4)
    {
        for(int i = 0; i < len; i++)
        {
            if(i!=3)
                out.append(QString::number(in[i])).append(".");
            else
                out.append(QString::number(in[i]));
        }
    }
    else if(len == 6)
    {
        for(int i = 0; i < len; i++)
        {
            if(i!=5)
                out.append(QString::number(in[i])).append(":");
            else
                out.append(QString::number(in[i]));
        }
    }
    else
    {
        for(int i = 0; i < len; i++)
        {
            out.append(QString::number(in[i]));
        }
    }
    return out;
}



void interfaceParams::QstrTochar(QString in, u_char *out, u_char len)
{
    uchar b_mult = 1;
    BYTE tempByte = 0;
    for(int i = in.length()-1; i >= 0; i--)
    {
        if( (in.at(i) == '.') || (in.at(i) == ':') )
        {
            out[len-1] = tempByte;
            tempByte = 0;
            b_mult = 1;
            len--;
        }
        else
        {
            tempByte += in.at(i).digitValue()*b_mult;
            b_mult *=10;
        }

        if(i == 0)
        {
            out[len-1] = tempByte;
            tempByte = 0;
            b_mult = 1;
        }

    }
}

void pcapDeviceSniffer::CreatePacket(uchar pck_type,
                                 uchar *UserData,
                                 uint UserDataLen
                                 )
{
    u_char *packet_data;

    if(pck_type == PCKT_TYPE_UDP)
    {
        // DataLength + 42 Bytes of Headers
        packet_data = new unsigned char[UserDataLen + 42];
        //packet_data = new unsigned char[60];
        int Pcktcnt = 0;
        uint PTR;
        memcpy((void*)packet_data, (void*)&ip_mac_dst->mac.byte1,6); // destMAC;
        Pcktcnt +=6;
        memcpy((void*)(packet_data + Pcktcnt), (void*)&ip_mac_src->mac.byte1, 6); // SrcMAC
        Pcktcnt +=6;
        PTR = IP_TYPE;
        memcpy((void*)(packet_data + Pcktcnt), (void*)&PTR, 2);
        Pcktcnt +=2;
        PTR = HEADER_LEN;
        memcpy((void*)(packet_data + Pcktcnt), (void*)&PTR, 2);
        Pcktcnt +=2;
        PTR = IP_LEN;
        memcpy((void*)(packet_data + Pcktcnt), (void*)&PTR, 2);
        Pcktcnt +=2;
        PTR = ID;
        memcpy((void*)(packet_data + Pcktcnt), (void*)&PTR, 2);
        Pcktcnt +=2;
        PTR = FLAG_OFSET;
        memcpy((void*)(packet_data + Pcktcnt), (void*)&PTR, 2);
        Pcktcnt +=2;
        PTR = UDP_PROT;
        memcpy((void*)(packet_data + Pcktcnt), (void*)&PTR, 2);
        Pcktcnt +=2;
        PTR = CHECK_SUM;
        memcpy((void*)(packet_data + Pcktcnt), (void*)&PTR, 2);
        Pcktcnt +=2;

        memcpy((void*)(packet_data + Pcktcnt), (void*)&ip_mac_src->ip.byte1, 4);
        Pcktcnt +=4;
        memcpy((void*)(packet_data + Pcktcnt), (void*)&ip_mac_dst->ip.byte1, 4);
        Pcktcnt +=4;

        PTR = DST_PORT;
        memcpy((void*)(packet_data + Pcktcnt), (void*)&PTR, 2);
        Pcktcnt +=2;

        PTR = SRC_PORT;
        memcpy((void*)(packet_data + Pcktcnt), (void*)&PTR, 2);
        Pcktcnt +=2;

        PTR = UDP_LEN_HEADER;
        memcpy((void*)(packet_data + Pcktcnt), (void*)&PTR, 2);
        Pcktcnt +=2;
        //check Sum
        PTR = 0x0000;
        memcpy((void*)(packet_data + Pcktcnt), (void*)&PTR, 2);
        Pcktcnt +=2;
        //preambula
       /* PTR = 0x0000;
        memcpy((void*)(packet_data + Pcktcnt), (void*)&PTR, 2);
        Pcktcnt +=2;*/
        // message cmd
      /*  PTR = 0x0300;
        memcpy((void*)(packet_data + Pcktcnt), (void*)&PTR, 2);
        Pcktcnt +=2;*/
        //data
        memcpy((void*)(packet_data + Pcktcnt), (void*)UserData, UserDataLen);
        Pcktcnt += UserDataLen;

        qDebug() << handle_dev << packet_data << Pcktcnt;
        pcap_sendpacket(handle_dev, packet_data, Pcktcnt);

    }
    else if(pck_type == PCKT_TYPE_ARP)
    {
        packet_data = new unsigned char[60];
        uint PTR;
        uint Pcktcnt = 0;

        BYTE dstMACff[6];

        for(int d=0;d<6;++d)
        {
            dstMACff[d] = 0xFF;
        }

        memcpy((void*)(packet_data), (void*)&dstMACff, 6); // SrcMAC
        Pcktcnt +=6;
        memcpy((void*)(packet_data + Pcktcnt), (void*)&ip_mac_src->mac.byte1, 6); // SrcMAC
        Pcktcnt +=6;
        PTR = FTYPE;
        memcpy((void*)(packet_data + Pcktcnt), (void*)&PTR, 2); //Frame type (FTYPE) Ethernet II
        Pcktcnt +=2;
        PTR = HTYPE;
        /////////////////////////////////////////////////////////////////////
        memcpy((void*)(packet_data + Pcktcnt), &PTR, 2);
        //Hardware type (HTYPE)
        Pcktcnt +=2;
        PTR = PTYPE;
        memcpy((void*)(packet_data + Pcktcnt), (void*)&PTR, 2); //Ptotocol type (PTYPE)
        Pcktcnt +=2;
        PTR = HLEN_PLEN;
        memcpy((void*)(packet_data + Pcktcnt), (void*)&PTR, 2);//MAClen_IP_len
        Pcktcnt +=2;
        PTR = ARP_REQUEST;
        memcpy((void*)(packet_data + Pcktcnt), (void*)&PTR, 2); //ARP request
        Pcktcnt +=2;
        memcpy((void*)(packet_data + Pcktcnt), (void*)&ip_mac_src->mac.byte1, 6); // SrcMAC
        Pcktcnt +=6;
        memcpy((void*)(packet_data + Pcktcnt), (void*)&ip_mac_src->ip.byte1, 4); // SrcIP
        Pcktcnt +=4;
        memcpy((void*)(packet_data + Pcktcnt), (void*)&ip_mac_dst->mac.byte1, 6 ); // DestMAC
        Pcktcnt +=6;
        memcpy((void*)(packet_data + Pcktcnt), (void*)&ip_mac_dst->ip.byte1, 4); // DestIP
        Pcktcnt +=4;

        for(int s=0;s<(60-42);++s )
        {
            uchar b = 0x00;
            memcpy((void*)(packet_data + Pcktcnt), (void*)&b, 1);
            Pcktcnt +=1;

        }
        qDebug() << handle_dev << packet_data << Pcktcnt;
        pcap_sendpacket(handle_dev, packet_data, Pcktcnt);
    }

}

void pcapInterface::packet_handler(u_char *param, const pcap_pkthdr *header, const u_char *pkt_data)
{
    arp_header *arp;
    ip_header *ih;
    udp_header *uh;
    u_int ip_len;

    static QStringList arpTableIp;
    static QStringList arpTableMac;

    //retireve the position of the ip header
    ih = (ip_header *) (pkt_data + 14); //length of ethernet header
    // retireve the position of the udp header
    ip_len = (ih->ver_ihl & 0xf) * 4;
    uh = (udp_header *) ((u_char*)ih + ip_len);
    // retrive the position of arp header
    arp = (arp_header *) (pkt_data);

    //Time
    static ushort fr_cnt = 0;
    time_t local_tv_sec;
    struct tm *ltime;
    char timestr[16];
    local_tv_sec = header->ts.tv_sec;
    ltime=localtime(&local_tv_sec);
    strftime( timestr, sizeof timestr, "%H:%M:%S", ltime);
//    qDebug()<<fr_cnt<<"pcapInterf time: "<<timestr <<"."<<header->ts.tv_usec<<" len: "<<header->len;
//    fr_cnt++;
    if((arp->src_ip.byte1 == ipAdr.ipAdrChr[0]) &&
       (arp->src_ip.byte2 == ipAdr.ipAdrChr[1]) &&
       (arp->src_ip.byte4 != ipAdr.ipAdrChr[3]) )
    {
        //our module

    }

    if((ih->saddr.byte1 == ipAdr.ipAdrChr[0]) &&
       (ih->saddr.byte2 == ipAdr.ipAdrChr[1]) &&
       (ih->saddr.byte4 != ipAdr.ipAdrChr[3]) )
    {
      //our module
      QString tempIp;
      u_char len;

      tempIp.append(getInstance()->CharToQstring(&ih->saddr.byte1, 4));
      len = arpTableIp.length();
      if(arpTableIp.length() > 0)
      {
          for(int i = 0; i < len; i++)
          {
              int comp = QString::compare(arpTableIp.at(i), tempIp);
              if(comp !=0)
              {
                  arpTableIp.append(getInstance()->CharToQstring(&ih->saddr.byte1, 4));
                  arpTableMac.append(getInstance()->CharToQstring(&arp->src_mac0.byte1, 6));
                  u_char l = arpTableIp.length() -1;
                  getInstance()->sendIpMac(arpTableIp.at(l), arpTableMac.at(l));
              }
          }

      }
      else
      {
          arpTableIp.append(getInstance()->CharToQstring(&ih->saddr.byte1, 4));
          arpTableMac.append(getInstance()->CharToQstring(&arp->src_mac0.byte1, 6));
          u_char l = arpTableIp.length() -1;
          getInstance()->sendIpMac(arpTableIp.at(l), arpTableMac.at(l));
      }

    }

}

pcapDeviceSniffer *pcapDeviceSniffer::getInstance()
{
    static pcapDeviceSniffer *m_instance = new pcapDeviceSniffer();

    if(m_instance == 0)
        m_instance = new pcapDeviceSniffer();
    return m_instance;
}

void pcapDeviceSniffer::packet_handler(u_char *param, const pcap_pkthdr *header, const u_char *pkt_data)
{
    ip_header *ih;
    udp_header *uh;
    u_int ip_len;

    static short cntLine;
    static boolean flag_lost_line;
    static uchar *ptrFrame = (uchar*) malloc (615350);
    ushort cntLine_from_dev;
    static ushort cntFrame_from_dev, cntFrame_console;

    ih = (ip_header *) (pkt_data + 14); //length of ethernet header
    // retireve the position of the udp header
    ip_len = (ih->ver_ihl & 0xf) * 4;
    uh = (udp_header *) ((u_char*)ih + ip_len);

    //Time
    static ushort fr_cnt = 0;
    time_t local_tv_sec;
    struct tm *ltime;
    char timestr[16];
    local_tv_sec = header->ts.tv_sec;
    ltime=localtime(&local_tv_sec);
    strftime( timestr, sizeof timestr, "%H:%M:%S", ltime);



    cntLine_from_dev = uh->datagram[1] | ((uh->datagram[0])<<8); //

    // first packet;
    if((uh->datagram[3] == 0x01)&&(header->len == 60))
    {
        qDebug() << "first packet";
        if(cntLine == IMG_HEIGHT)
        {
            trBuffer->changeInput();
           // getInstance()->s_inputBufferReady();

        }
        cntFrame_from_dev = uh->datagram[5] | ((uh->datagram[4])<<8);
        cntFrame_console = cntFrame_from_dev;
        //qDebug()<<c_i<<": "<<cntFrame_from_dev<< " New Frame: "<< cntLine_from_dev << " cntLine = "<< cntLine;
        flag_lost_line = true;
        cntLine = 0;
        ptrFrame = trBuffer->getPtrForInput();
        fr_cnt++;
        //qDebug()<<fr_cnt <<". timeS: "<<timestr <<"."<<header->ts.tv_usec<<" len: "<<header->len;

    }
    else if( (flag_lost_line)&&(cntFrame_console == cntFrame_from_dev) && (header->len == 1328)&&(uh->datagram[3] == 0x02))
    {
        if (cntLine % 10 == 0)
            qDebug() << "data packet" << cntLine;
        ushort pckt_len = IMG_WIDTH*2;
        memcpy(ptrFrame + 2*cntLine_from_dev, uh->datagram + 4, 2); // Temperature
        memcpy(ptrFrame + 960 + cntLine_from_dev*pckt_len, uh->datagram + 6, pckt_len );//video data
       // qDebug()<<c_i<<": "<<"cntLine: "<<cntLine<<" dev_line: "<< cntLine_from_dev;
       cntLine++;
    }    
    else if(uh->datagram[3] == 0x03)
    {
        //echo
        qDebug() << "echo packet";
        getInstance()->s_EchoMsg(&uh->datagram[0]);
    }
    else if((flag_lost_line)&&(uh->datagram[3] == 0x02)) // Lost packet
    {
        qDebug() << "lost packet";
        QString txt;
        txt.append("Expected line ").append(QString::number(cntLine)).append(" in frame ").append(QString::number(cntFrame_console))
           .append(" Recieve line ").append(QString::number(cntLine_from_dev)).append(" in frame ").append(QString::number(cntFrame_from_dev));
        getInstance()->sendMsg(LOG,txt);
        //qDebug()<<c_i<<": "<<cntFrame_from_dev<<"      Lost Line: "<< cntLine_from_dev << " cntLine = "<< cntLine;
        flag_lost_line = false;

        cntFrame_console = cntFrame_from_dev;
    }
}

void pcapDeviceSniffer::initInThread()
{
    alldevs = new pcap_if_t;
    d = new pcap_if_t;

    u_char *devIp;
    char errbuf[PCAP_ERRBUF_SIZE];
    char devNum = 0;
    bool flag_dev_find = false;
    bool flag_error_pcap = false;

    //char *filter = "udp or arp"; // filter for BPF
    struct bpf_program fp; // compiled filter
    bpf_u_int32 maskp;
    bpf_u_int32 netp;

    if(pcap_findalldevs(&alldevs, errbuf) == -1)
    {
        QString str;
        str.append("Error in find all devices\n Errbuf: ").append(errbuf);
        sendMsg(LOG, str);
        pcap_freealldevs(alldevs);

        qDebug() << "Error in find all devices";
    }

    for(d = alldevs; d!=NULL; d = d->next)
    {
        pcap_addr_t *dev_addr;

        for(dev_addr = d->addresses; dev_addr != NULL; dev_addr = dev_addr->next)
        {
            if(dev_addr->addr->sa_family == AF_INET && dev_addr->addr && dev_addr->netmask)
            {
               devIp = (u_char *)&(((struct sockaddr_in *)dev_addr->addr)->sin_addr.s_addr);
               QString IP_QS = CharToQstring(devIp, 4);
               qDebug() << "dev" << devIp;
               /////////////////////
               ipAdr.ipAdrStr = "169.254.0.13";
               /////////////////////
               qDebug() << IP_QS << "       " << ipAdr.ipAdrStr;
               if( !(QString::compare(IP_QS, ipAdr.ipAdrStr, Qt::CaseInsensitive)))
               {
                   flag_dev_find = true;
                   qDebug() << "found!";
                   goto found;
               }
            }
        }
        devNum++;
    }
    found: // go to our device

    if(flag_dev_find)
    {
        #ifdef DEBUG
        qDebug()<<"PcapDevice thread: "<< QThread::currentThreadId();
        #endif

        d = alldevs;
        for(int i = 0; i < devNum; i++)
            d = d->next;

        if ((handle_dev = pcap_open_live(d->name,
                                       BUF_SIZE_RECIEVE, //65536
                                       1,  // 1 неразборчивый режим (без фильтрации на уровне карты)
                                       0, // 1000
                                       errbuf
                                       )) == NULL)
        {
            // вывод ошибки
            QString str;
            str.append("Error in pcap open live\n Errbuf: ").append(errbuf);
            sendMsg(LOG, str);
            flag_error_pcap = true;
            goto errExit;
        }

        if (pcap_datalink (handle_dev) != DLT_EN10MB) // Check if its Ethernet connection
        {
            // send error
            QString str;
            str.append("Its's not Ethernet connection");
            sendMsg(LOG, str);
            flag_error_pcap = true;
            goto errExit;
        }

        if(pcap_lookupnet(d->name, &netp, &maskp, errbuf ) == -1)
        {
            //error
            QString str;
            str.append("Error in pcap lookupnet\n Errbuf: ").append(errbuf);
            sendMsg(LOG, str);
            flag_error_pcap = true;
            goto errExit;
        }

        //Compile filter
        if(pcap_compile(handle_dev, &fp, filter, 0, maskp) == -1)
        {
            //error
            QString str;
            str.append("Can't compile filter");
            sendMsg(LOG, str);
            flag_error_pcap = true;
            goto errExit;
        }

        //Set Filter
        if(pcap_setfilter(handle_dev, &fp) < 0)
        {
            QString str;
            str.append("Can't set filter");
            sendMsg(LOG, str);
            flag_error_pcap = true;
            goto errExit;
        }       

        errExit:
            if(flag_error_pcap)
            {
                pcap_freealldevs(alldevs);
                stop();

            }
            else
            {
                //start capture
                qDebug() << "capture started!";
                pcap_loop(handle_dev, 0, pcapDeviceSniffer::packet_handler, NULL);
            }



    }
    else
    {
        pcap_freealldevs(alldevs);
        sendMsg(MSG_BOX_CRITICAL, "Device not found!");
        qDebug() << "Device not found!";
        //quiet from thread and close it
    }

}

void pcapDeviceSniffer::stop()
{
    pcap_breakloop(handle_dev);
    emit s_finished_dev();

}

