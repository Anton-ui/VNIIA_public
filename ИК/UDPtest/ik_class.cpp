#include "ik_class.h"

IK_class::IK_class(QObject *parent):
    QObject(parent)
{
    IP_Address = "169.254.0.13";
}

void IK_class::InitThread()
{
    pcap_if_t *alldevs;
    pcap_if_t *d;
    int inum;
    int i=0;
    char errbuf[PCAP_ERRBUF_SIZE];

    /* Retrieve the device list on the local machine */
    if (pcap_findalldevs_ex((char*)PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
    {
        fprintf(stderr,"Error in pcap_findalldevs: %s\n", errbuf);
        exit(1);
    }

    /* Seach ethernet adapter */
    for(d=alldevs; d; d=d->next)
    {
        i++;
        pcap_addr_t *dev_addr;
        u_char *devIp;

        for(dev_addr = d->addresses; dev_addr != NULL; dev_addr = dev_addr->next)
        {
            if(dev_addr->addr->sa_family == AF_INET && dev_addr->addr && dev_addr->netmask)
            {
               devIp = (u_char *)&(((struct sockaddr_in *)dev_addr->addr)->sin_addr.s_addr);
               QString IP_QS = CharToQstring_Address(devIp, 4);

               if( !(QString::compare(IP_QS, IP_Address, Qt::CaseInsensitive)))
               {
                   inum = i;
               }
            }
        }
    }

    if(i==0)
    {
        qDebug() << "No interfaces found! Make sure WinPcap is installed.";
        exit(0);
    }


    /* Jump to the selected adapter */
    for(d=alldevs, i=0; i< inum-1 ;d=d->next, i++);

    /* Open the device */
    if ((adhandle= pcap_open(d->name, 65536, PCAP_OPENFLAG_PROMISCUOUS, 1000, NULL, errbuf)) == NULL)
    {
        qDebug() << "Unable to open the adapter." << d->name << "is not supported by WinPcap";
        /* Free the device list */
        pcap_freealldevs(alldevs);
        exit(0);
    }

    /* At this point, we don't need any more the device list. Free it */
    pcap_freealldevs(alldevs);

    /* start the capture */
    qDebug() << "capture started!";
    pcap_loop(adhandle, 0, IK_class::packet_handler, NULL);
}

void IK_class::packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data)
{
    static short cntLine;

    if (header->len == 60)
    {
        qDebug() << "first packet";
        cntLine = 0;
    }
    else if (header->len == 1328)
    {
        qDebug() << cntLine;
        cntLine++;
    }
}

QString IK_class::CharToQstring_Address(u_char *in, u_char len)
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

void IK_class::stop()
{
    pcap_breakloop(adhandle);
}
