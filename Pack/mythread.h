#ifndef MYTHREAD_H
#define MYTHREAD_H
#include<QMutex>
#include<QMutexLocker>
#include<QThread>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/uio.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <linux/filter.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>



struct MyArp
{
    //arp首部
    unsigned short arp_hrd;    /* format of hardware address */
    unsigned short arp_pro;    /* format of protocol address */
    unsigned char arp_hln;    /* length of hardware address */
    unsigned char arp_pln;    /* length of protocol address */
    unsigned short arp_op;     /* ARP/RARP operation */

    unsigned char arp_sha[6];    /* sender hardware address */
    unsigned long arp_spa;    /* sender protocol address */
    unsigned char arp_tha[6];    /* target hardware address */
    unsigned long arp_tpa;    /* target protocol address */
};
/*
 ************ eth **************
struct ethhdr {
    unsigned char h_dest[ETH_ALEN];
    unsigned char h_source[ETH_ALEN];
    __be16 h_proto;
} __attribute__((packed));


struct ether_header
{
    u_int8_t ether_dhost[ETH_ALEN];      // destination eth addr
    u_int8_t ether_shost[ETH_ALEN];      // source ether addr
    u_int16_t ether_type;                 // packet type ID field
} __attribute__ ((__packed__));


    ************* ARP **************
struct arphdr
{
    unsigned short    ar_hrd;         format of hardware address
    unsigned short    ar_pro;         format of protocol address
    unsigned char    ar_hln;          length of hardware address
    unsigned char    ar_pln;          length of protocol address
    unsigned short    ar_op;          ARP opcode (command)
}

 ************ IP ***************
struct iphdr
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
    unsigned int ihl:4;
    unsigned int version:4;
#elif __BYTE_ORDER == __BIG_ENDIAN
    unsigned int version:4;
    unsigned int ihl:4;
#else
# error "Please fix <bits/endian.h>"
#endif
    u_int8_t tos;
    u_int16_t tot_len;
    u_int16_t id;
    u_int16_t frag_off;
    u_int8_t ttl;
    u_int8_t protocol;
    u_int16_t check;
    u_int32_t saddr;
    u_int32_t daddr;
};

 ************* TCP **************
struct tcphdr
{
    u_int16_t source;
    u_int16_t dest;
    u_int32_t seq;
    u_int32_t ack_seq;
# if __BYTE_ORDER == __LITTLE_ENDIAN
    u_int16_t res1:4;
    u_int16_t doff:4;
    u_int16_t fin:1;
    u_int16_t syn:1;
    u_int16_t rst:1;
    u_int16_t psh:1;
    u_int16_t ack:1;
    u_int16_t urg:1;
    u_int16_t res2:2;
# elif __BYTE_ORDER == __BIG_ENDIAN
    u_int16_t doff:4;
    u_int16_t res1:4;
    u_int16_t res2:2;
    u_int16_t urg:1;
    u_int16_t ack:1;
    u_int16_t psh:1;
    u_int16_t rst:1;
    u_int16_t syn:1;
    u_int16_t fin:1;
# else
#   error "Adjust your <bits/endian.h> defines"
# endif
    u_int16_t window;
    u_int16_t check;
    u_int16_t urg_ptr;
};

 ************** UDP ************
struct udphdr
{
    u_int16_t source;
    u_int16_t dest;
    u_int16_t len;
    u_int16_t check;
};
*/



class myThread : public QThread
{
    Q_OBJECT
public:
    myThread(QString, QString);
    ~myThread();
    void stop();

signals:
    void updateSignal(QString, QString, QString, QString, QString, QString, QString, QString);

private slots:
    void stopSlot();

protected:
    void run();

private:
    volatile bool stopped;
    int socketID;
    QString sIP, dIP;

    struct in_addr addr;
    struct ether_header *peth;
    struct MyArp *parp;
    struct iphdr *pip;
    struct tcphdr *ptcp;
    struct udphdr *pudp;
    char recvBuf[4096];//data
    char *temp;
    char *src_ip;
    unsigned char *eth_mac;
    char src_mac[18], des_mac[18];
    char *des_ip;
    char *protocol;
    char *info;
    int src_port, des_port;

    QMutex mutex;
};

#endif // MYTHREAD_H
