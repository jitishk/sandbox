/*
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 */

#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>
// 0c:75:bd:1b:e1:e7
//#define MY_DEST_MAC0    0x0c
//#define MY_DEST_MAC1    0x75
//#define MY_DEST_MAC2    0xbd
//#define MY_DEST_MAC3    0x1b
//#define MY_DEST_MAC4    0xe1
//#define MY_DEST_MAC5    0xe7

//00:00:00:00:1b:05 (netflow-intf)
#define MY_DEST_MAC0    0x00
#define MY_DEST_MAC1    0x00
#define MY_DEST_MAC2    0x00
#define MY_DEST_MAC3    0x00
#define MY_DEST_MAC4    0x1b
#define MY_DEST_MAC5    0x05


#define DEFAULT_IF    "nfmtd-intf"
#define BUF_SIZ        1024

int main(int argc, char *argv[])
{
    int sockfd;
    struct ifreq if_idx;
    struct ifreq if_mac;
    int tx_len = 0;
    char sendbuf[BUF_SIZ];
    struct ether_header *eh = (struct ether_header *) sendbuf;
    struct iphdr *iph = (struct iphdr *) (sendbuf + sizeof(struct ether_header));
    struct sockaddr_ll socket_address;
    char ifName[IFNAMSIZ];
    
    /* Get interface name */
    if (argc > 1)
        strcpy(ifName, argv[1]);
    else
        strcpy(ifName, DEFAULT_IF);

    /* Open RAW socket to send on */
    if ((sockfd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) == -1) {
        perror("socket");
    }

    /* Get the index of the interface to send on */
    memset(&if_idx, 0, sizeof(struct ifreq));
    strncpy(if_idx.ifr_name, ifName, IFNAMSIZ-1);
    if (ioctl(sockfd, SIOCGIFINDEX, &if_idx) < 0)
        perror("SIOCGIFINDEX");
    /* Get the MAC address of the interface to send on */
    memset(&if_mac, 0, sizeof(struct ifreq));
    strncpy(if_mac.ifr_name, ifName, IFNAMSIZ-1);
    if (ioctl(sockfd, SIOCGIFHWADDR, &if_mac) < 0)
        perror("SIOCGIFHWADDR");

    /* Construct the Ethernet header */
    memset(sendbuf, 0, BUF_SIZ);
    /* Ethernet header */
    eh->ether_shost[0] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[0];
    eh->ether_shost[1] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[1];
    eh->ether_shost[2] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[2];
    eh->ether_shost[3] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[3];
    eh->ether_shost[4] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[4];
    eh->ether_shost[5] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[5];
    eh->ether_dhost[0] = MY_DEST_MAC0;
    eh->ether_dhost[1] = MY_DEST_MAC1;
    eh->ether_dhost[2] = MY_DEST_MAC2;
    eh->ether_dhost[3] = MY_DEST_MAC3;
    eh->ether_dhost[4] = MY_DEST_MAC4;
    eh->ether_dhost[5] = MY_DEST_MAC5;
    /* Ethertype field */
    eh->ether_type = htons(ETH_P_IP);
    tx_len += sizeof(struct ether_header);

    /* Packet data */
    sendbuf[tx_len++] = 0xde;
    sendbuf[tx_len++] = 0xad;
    sendbuf[tx_len++] = 0xbe;
    sendbuf[tx_len++] = 0xef;

    /* Index of the network device */
    socket_address.sll_ifindex = if_idx.ifr_ifindex;
    /* Address length*/
    socket_address.sll_halen = ETH_ALEN;
    /* Destination MAC */
    socket_address.sll_addr[0] = MY_DEST_MAC0;
    socket_address.sll_addr[1] = MY_DEST_MAC1;
    socket_address.sll_addr[2] = MY_DEST_MAC2;
    socket_address.sll_addr[3] = MY_DEST_MAC3;
    socket_address.sll_addr[4] = MY_DEST_MAC4;
    socket_address.sll_addr[5] = MY_DEST_MAC5;

    /* Send packet */
    if (sendto(sockfd, sendbuf, tx_len, 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll)) < 0)
        printf("Send failed\n");

    return 0;
}


#if 0
#define _GNU_SOURCE
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/if_packet.h>
#include <net/ethernet.h> /* the L2 protocols */

#define LCINBAND_INTF_NAME "nfmtd-intf"
//#define IFNAMSIZ 16
#define CPU_INTF_NAME LCINBAND_INTF_NAME

uint16_t pkt_buf_16[] = {
0x00a6, 0xcab6, 0x97b9, 0x0000, 0x0000, 0x1b05, 0x0800, 0x45b0,
0x003a, 0x0000, 0x0000, 0x4111, 0x7cc7, 0x7f01, 0x7f19, 0x7f01,
0x7f20, 0x07d1, 0x0000, 0x0026, 0x0000, 0x0000, 0x6100, 0x04d2,
0x5ca4, 0xfa83, 0x3775, 0xe228, 0x5ca4, 0xfa83, 0x3018, 0x5a60,
0x0000, 0x0000, 0x0000, 0x00ed 
};

// uint8_t pkt_buf[] = {
// 0x00, 0xa6, 0xca, 0xb6, 0x97, 0xb9, 0x00, 0x00, 0x00, 0x00, 0x1b, 0x05, 0x08, 0x00, 0x45, 0xb0,
// 0x00, 0x3a, 0x00, 0x00, 0x00, 0x00, 0x41, 0x11, 0x7c, 0xc7, 0x7f, 0x01, 0x7f, 0x19, 0x7f, 0x01,
// 0x7f, 0x20, 0x07, 0xd1, 0x00, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00, 0x00, 0x61, 0x00, 0x04, 0xd2,
// 0x5c, 0xa4, 0xfa, 0x83, 0x37, 0x75, 0xe2, 0x28, 0x5c, 0xa4, 0xfa, 0x83, 0x30, 0x18, 0x5a, 0x60,
// 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xed 
// };
 
uint8_t pkt_buf[] = {
    0x92, 0x79, 0x6f, 0x80, 0x89, 0x19, 0x00, 0x00, 0x00, 0x00, 0x1b, 0x05, 0x08, 0x00, 0x45, 0xb0,
    0x00, 0x3a, 0x00, 0x00, 0x00, 0x00, 0x41, 0x11, 0x7c, 0xc7, 0x7f, 0x01, 0x7f, 0x19, 0x7f, 0x01,
    0x7f, 0x20, 0x07, 0xd1, 0x00, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00, 0x00, 0x61, 0x00, 0x04, 0xd2,
    0x5c, 0xa5, 0x18, 0x82, 0x18, 0x5d, 0x3e, 0x40, 0x5c, 0xa5, 0x18, 0x82, 0x10, 0xff, 0xb6, 0x60,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa8
};

const unsigned char ether_broadcast_addr[]= {0xff,0xff,0xff,0xff,0xff,0xff};
const unsigned char tap_lc_spoof_addr[]= {0x92,0x79,0x6f,0x80,0x89,0x19};

void jdump (char *msg, size_t msg_len) {
    printf("{");
    for (int i=0; i<msg_len; i++) {
        if ((i%16) == 0) {
            printf("\n");
        }
        printf("0x%02x", (unsigned)(unsigned char)msg[i]);
        if (i<msg_len-1) {
            printf(", ");
        }
    }
    printf("\n}\n\n");
}

int
main(void)
{
#define VLEN 1
#define BUFSIZE 200
#define TIMEOUT 1
    int sockfd, sockfdd, retval, i;
    //struct sockaddr_in sa;
    struct sockaddr_ll sa;
    struct mmsghdr msgs[VLEN];
    struct iovec iovecs[VLEN];
    char bufs[VLEN][BUFSIZE];
    //struct timespec timeout;
    struct ifreq ifr;

    sockfd = socket(AF_PACKET, SOCK_RAW,  htons(ETH_P_IP));
    if (sockfd == -1) {
        perror("socket()");
        exit(EXIT_FAILURE);
    }
    printf("socket: %d\n", sockfd);

    bzero(&sa, sizeof(sa));
    sa.sll_family   = AF_PACKET;
    sa.sll_protocol = htons(ETH_P_IP);

    // Get Interface info
    strncpy(ifr.ifr_name, CPU_INTF_NAME, IFNAMSIZ);
    if(ioctl(sockfd, SIOCGIFINDEX, &ifr) < 0) {
        return -1;
    }
    sa.sll_ifindex  = ifr.ifr_ifindex;

    strncpy(ifr.ifr_name, CPU_INTF_NAME, IFNAMSIZ);
    if(ioctl(sockfd, SIOCGIFHWADDR, &ifr) < 0) {
        return -1;
    }
    //memcpy(sa.sll_addr,ether_broadcast_addr, ETHER_ADDR_LEN);
    memcpy(sa.sll_addr,ifr.ifr_hwaddr.sa_data, ETHER_ADDR_LEN);

    // print sockaddr_ll
    printf("sa: %#x %#x %#x\n", sa.sll_family, sa.sll_protocol, sa.sll_ifindex);
    printf("\thw_addr: { ");
    for (int i=0; i<ETHER_ADDR_LEN; i++) {
        printf("%02x ", sa.sll_addr[i]); 
    }
    printf("}\n");

    // Get dest mac address
    sockfdd = socket(AF_PACKET, SOCK_RAW,  htons(ETH_P_IP));
    strncpy(ifr.ifr_name, "nfm-intf", IFNAMSIZ);
    if(ioctl(sockfdd, SIOCGIFHWADDR, &ifr) < 0) {
        return -1;
    }
    // Set message
    memset(msgs, 0, sizeof(msgs));
    for (i = 0; i < VLEN; i++) {
        memset(bufs[i], 0, sizeof(bufs[i]));
        memcpy(pkt_buf, sa.sll_addr, ETHER_ADDR_LEN);
        memcpy(pkt_buf + ETHER_ADDR_LEN, ifr.ifr_hwaddr.sa_data, ETHER_ADDR_LEN);
        memcpy(bufs[i], pkt_buf, sizeof(bufs[i]));
        bufs[i][65] = 0xfe;
        bufs[i][66] = 0xab;
        iovecs[i].iov_base         = bufs[i];
        iovecs[i].iov_len          = sizeof(bufs[i]);
        msgs[i].msg_hdr.msg_name   = &sa;
        msgs[i].msg_hdr.msg_namelen= sizeof(sa);
        msgs[i].msg_hdr.msg_iov    = &iovecs[i];
        msgs[i].msg_hdr.msg_iovlen = 1;
    }

    printf("messages sent\n");
    for (i = 0; i < VLEN; i++) {
        printf("msg %d:\n", i);
        jdump(bufs[i], sizeof(pkt_buf));
        printf("\n");
    }

    retval = sendmmsg(sockfd, msgs, VLEN, 0);
    if (retval == -1) {
        perror("sendmmsg()");
        exit(EXIT_FAILURE);
    }
    printf("%d messages sent\n", retval);

    exit(EXIT_SUCCESS);
}
#endif
