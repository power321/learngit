#include "mythread.h"

#include <error.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <QDebug>

using namespace std;

myThread::myThread(QString IP1, QString IP2)
{
    this->sIP = IP1;
    this->dIP = IP2;

    stopped = false;
    socketID = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if(socketID == -1)
        qDebug()<<strerror(errno);

    //Change eth0 work status
    struct ifreq ifr;
    strncpy(ifr.ifr_name, "eth0", strlen("eth0")+1);
    if((ioctl(socketID, SIOCGIFFLAGS, &ifr) == -1))
        this->exit();
    ifr.ifr_flags |= IFF_PROMISC;
    if((ioctl(socketID, SIOCGIFFLAGS, &ifr) == -1))
        this->exit();

}

void myThread::run()
{
    QString str_sIP, str_dIP, str_sMac, str_dMac, str_sPort, str_dPort, str_Protocol, str_info;

    while(1)
    {
        {
            QMutexLocker locker(&mutex);
            if(stopped)
            {
                stopped = false;
                break;
            }
        }
        src_ip = "";
        memset(src_mac, 0, sizeof(src_mac));
        src_port = 0;
        des_ip = "";
        memset(des_mac, 0, sizeof(des_mac));
        des_port = 0;
        protocol = "";
        info = "";

        str_sIP = tr("");
        str_dIP = tr("");
        str_sMac = tr("");
        str_dMac = tr("");
        str_sPort = tr("");
        str_dPort = tr("");
        str_Protocol = tr("");
        str_info = tr("");

        int recvLen = recvfrom(socketID, recvBuf, sizeof(recvBuf), 0, NULL, NULL);
        recvBuf[recvLen] = 0;
        if(recvLen < 0)
            qDebug()<<"recv error!";
        else
        {

            temp = recvBuf;
            peth = (struct ether_header *)temp;
            eth_mac = peth->ether_shost;
            sprintf(src_mac, "%02x:%02x:%02x:%02x:%02x:%02x",*eth_mac,*(eth_mac+1),*(eth_mac+2),*(eth_mac+3),*(eth_mac+4),*(eth_mac+5));


            eth_mac = peth->ether_dhost;
            sprintf(des_mac, "%02x:%02x:%02x:%02x:%02x:%02x",*eth_mac,*(eth_mac+1),*(eth_mac+2),*(eth_mac+3),*(eth_mac+4),*(eth_mac+5));

            temp += sizeof(struct ether_header);//14bytes
            if(ntohs( peth->ether_type ) == 0x0800)//IP
            {
                pip = (struct iphdr *)temp;
                temp += sizeof(struct iphdr);
                switch(pip->protocol)
                {
                case IPPROTO_TCP:
                    des_ip = inet_ntoa(*(struct in_addr*)&(pip->daddr));
                    str_dIP = QString::fromLatin1(des_ip);
                    src_ip = inet_ntoa(*(struct in_addr*)&(pip->saddr));
                    str_sIP = QString::fromLatin1(src_ip);

                    ptcp = (struct tcphdr *)temp;
                    temp += sizeof(struct tcphdr);
                    str_Protocol = tr("TCP");


                    src_port = ntohs(ptcp->source);
                    des_port = ntohs(ptcp->dest);

                    str_sPort = QString::number(src_port);
                    str_dPort = QString::number(des_port);

                    info = temp;
                    str_info = QString::fromUtf8(info);
                    break;

                case IPPROTO_UDP:
                    des_ip = inet_ntoa(*(struct in_addr*)&(pip->daddr));
                    str_dIP = QString::fromLatin1(des_ip);
                    src_ip = inet_ntoa(*(struct in_addr*)&(pip->saddr));
                    str_sIP = QString::fromLatin1(src_ip);

                    pudp = (struct udphdr *)temp;
                    temp += sizeof(struct udphdr);
                    str_Protocol = tr("UDP");
                    src_port = ntohs(pudp->source);
                    des_port = ntohs(pudp->dest);
                    str_sPort = QString::number(src_port);
                    str_dPort = QString::number(des_port);

                    info = temp;
                    str_info = QString::fromUtf8(info);
                    break;

                case IPPROTO_ICMP:
                    temp += 8;//8bytes
                    str_Protocol = tr("ICMP");
                    info = temp;
                    str_info = QString::fromUtf8(info);
                    break;

                case IPPROTO_IGMP:
                    str_Protocol = tr("IGMP");
                    break;

                default:
                    str_Protocol = tr("Unknow Protocol");
                    break;
                }

                str_sMac = QString::fromUtf8(src_mac);
                str_dMac = QString::fromUtf8(des_mac);

                qDebug()<<"sIP"<<this->sIP<<"dIP"<<this->dIP;
                qDebug()<<"src ip"<<str_sIP;
                qDebug()<<"des ip"<<str_dIP;
                qDebug()<<"src mac"<<str_sMac;
                qDebug()<<"des mac"<<str_dMac;
                qDebug()<<"src port"<<str_sPort;
                qDebug()<<"des port"<<str_dPort;
                qDebug()<<"protocol"<<str_Protocol;
                qDebug()<<"info"<<str_info;
                if(this->sIP != "" && str_sIP == this->sIP && this->dIP != "" && str_dIP == this->dIP)
                    emit updateSignal(str_sIP, str_dIP, str_sMac, str_dMac, str_sPort, str_dPort, str_Protocol, str_info);
                else if(this->sIP != "" && str_sIP == this->sIP )
                    emit updateSignal(str_sIP, str_dIP, str_sMac, str_dMac, str_sPort, str_dPort, str_Protocol, str_info);
                else if(this->dIP != "" && str_dIP ==this->dIP)
                    emit updateSignal(str_sIP, str_dIP, str_sMac, str_dMac, str_sPort, str_dPort, str_Protocol, str_info);
                else if(this->sIP == "" && this->dIP == "")
                    emit updateSignal(str_sIP, str_dIP, str_sMac, str_dMac, str_sPort, str_dPort, str_Protocol, str_info);
            }
            /*else if(ntohs( peth->ether_type )== 0x0806)//ARP
            {
                 parp = (struct MyArp *)temp;
                 str_Protocol = tr("ARP");

                 addr.s_addr = parp->arp_spa;
                 src_ip = inet_ntoa(addr);
                 str_sIP = QString::fromUtf8(src_ip);

                 addr.s_addr = parp->arp_tpa;
                 des_ip = inet_ntoa(addr);
                 str_dIP = QString::fromUtf8(des_ip);
             }
             else if(ntohs( peth->ether_type ) == 0x8035)//RARP
                  str_Protocol = tr("RARP");
             else
                str_Protocol = tr("Unknow Protocol");

            */

         }
        usleep(500);
    }
}

void myThread::stopSlot()
{
    QMutexLocker locker(&mutex);
    stopped = true;
}



myThread::~myThread()
{

}
