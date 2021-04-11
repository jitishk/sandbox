/*
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 */

#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>

// #define DEST_MAC0    0x0c
// #define DEST_MAC1    0x75
// #define DEST_MAC2    0xbd
// #define DEST_MAC3    0x1b
// #define DEST_MAC4    0xe1
// #define DEST_MAC5    0xe7

#define DEST_MAC0    ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[0]
#define DEST_MAC1    ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[1]
#define DEST_MAC2    ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[2]
#define DEST_MAC3    ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[3]
#define DEST_MAC4    ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[4]
#define DEST_MAC5    ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[5]

#define ETHER_TYPE    0x0800

#define DEFAULT_IF    "nfm-intf"
#define BUF_SIZ        1024

int main(int argc, char *argv[])
{
    char sender[INET6_ADDRSTRLEN];
    int sockfd, ret, i;
    int sockopt;
    ssize_t numbytes;
    struct ifreq ifopts;    /* set promiscuous mode */
    struct ifreq if_ip;    /* get ip addr */
    struct ifreq if_mac;
    struct sockaddr_storage their_addr;
    uint8_t buf[BUF_SIZ];
    char ifName[IFNAMSIZ];
    
    /* Get interface name */
    if (argc > 1)
        strcpy(ifName, argv[1]);
    else
        strcpy(ifName, DEFAULT_IF);

    /* Header structures */
    struct ether_header *eh = (struct ether_header *) buf;
    struct iphdr *iph = (struct iphdr *) (buf + sizeof(struct ether_header));
    struct udphdr *udph = (struct udphdr *) (buf + sizeof(struct iphdr) + sizeof(struct ether_header));

    memset(&if_ip, 0, sizeof(struct ifreq));

    /* Open PF_PACKET socket, listening for EtherType ETHER_TYPE */
    if ((sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETHER_TYPE))) == -1) {
        perror("listener: socket");    
        return -1;
    }

    /* Set interface to promiscuous mode - do we need to do this every time? */
    strncpy(ifopts.ifr_name, ifName, IFNAMSIZ-1);
    ioctl(sockfd, SIOCGIFFLAGS, &ifopts);
    ifopts.ifr_flags |= IFF_PROMISC;
    ioctl(sockfd, SIOCSIFFLAGS, &ifopts);

    /* Get the MAC address of the interface to send on */
    memset(&if_mac, 0, sizeof(struct ifreq));
    strncpy(if_mac.ifr_name, ifName, IFNAMSIZ-1);
    if (ioctl(sockfd, SIOCGIFHWADDR, &if_mac) < 0)
        perror("SIOCGIFHWADDR");

    /* Allow the socket to be reused - incase connection is closed prematurely */
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof sockopt) == -1) {
        perror("setsockopt");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    /* Bind to device */
    if (setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, ifName, IFNAMSIZ-1) == -1)    {
        perror("SO_BINDTODEVICE");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

repeat:    printf("listener: Waiting to recvfrom...\n");
    numbytes = recvfrom(sockfd, buf, BUF_SIZ, 0, NULL, NULL);
    printf("listener: got packet %d bytes\n", numbytes);

    /* Check the packet is for me */
    if (eh->ether_dhost[0] == DEST_MAC0 &&
            eh->ether_dhost[1] == DEST_MAC1 &&
            eh->ether_dhost[2] == DEST_MAC2 &&
            eh->ether_dhost[3] == DEST_MAC3 &&
            eh->ether_dhost[4] == DEST_MAC4 &&
            eh->ether_dhost[5] == DEST_MAC5) {
        printf("Correct destination MAC address\n");
    } else {
        printf("Wrong destination MAC: %x:%x:%x:%x:%x:%x\n",
                        eh->ether_dhost[0],
                        eh->ether_dhost[1],
                        eh->ether_dhost[2],
                        eh->ether_dhost[3],
                        eh->ether_dhost[4],
                        eh->ether_dhost[5]);
        ret = -1;
        goto done;
    }

    /* Get source IP */
    ((struct sockaddr_in *)&their_addr)->sin_addr.s_addr = iph->saddr;
    inet_ntop(AF_INET, &((struct sockaddr_in*)&their_addr)->sin_addr, sender, sizeof sender);

    /* Look up my device IP addr if possible */
    strncpy(if_ip.ifr_name, ifName, IFNAMSIZ-1);
    if (ioctl(sockfd, SIOCGIFADDR, &if_ip) >= 0) { /* if we can't check then don't */
        printf("Source IP: %s\n My IP: %s\n", sender, 
                inet_ntoa(((struct sockaddr_in *)&if_ip.ifr_addr)->sin_addr));
        /* ignore if I sent it */
        if (strcmp(sender, inet_ntoa(((struct sockaddr_in *)&if_ip.ifr_addr)->sin_addr)) == 0)    {
            printf("but I sent it :(\n");
            ret = -1;
            goto done;
        }
    }

    /* UDP payload length */
    ret = ntohs(udph->len) - sizeof(struct udphdr);

    /* Print packet */
    printf("\tData:");
    for (i=0; i<numbytes; i++) printf("%02x:", buf[i]);
    printf("\n");

done:    goto repeat;

    close(sockfd);
    return ret;
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

#define LCINBAND_INTF_NAME "nfm-intf"
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
    0x00, 0xa6, 0xca, 0xb6, 0x97, 0xb9, 0x00, 0x00, 0x00, 0x00, 0x1b, 0x05, 0x08, 0x00, 0x45, 0xb0,
    0x00, 0x3a, 0x00, 0x00, 0x00, 0x00, 0x41, 0x11, 0x7c, 0xc7, 0x7f, 0x01, 0x7f, 0x19, 0x7f, 0x01,
    0x7f, 0x20, 0x07, 0xd1, 0x00, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00, 0x00, 0x61, 0x00, 0x04, 0xd2,
    0x5c, 0xa5, 0x18, 0x82, 0x18, 0x5d, 0x3e, 0x40, 0x5c, 0xa5, 0x18, 0x82, 0x10, 0xff, 0xb6, 0x60,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa8
};



const unsigned char intf_nfm_addr[]= {0x00,0x00,0x00,0x00,0x1b,0x05};
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
    int sockfd, retval, i;
    //struct sockaddr_in sa;
    struct sockaddr_ll sa;
    struct mmsghdr msgs[VLEN];
    struct iovec iovecs[VLEN];
    char bufs[VLEN][BUFSIZE];
    //struct timespec timeout;
    struct ifreq ifr;

    sockfd = socket(AF_PACKET, SOCK_RAW,  htons(ETH_P_ALL));
    if (sockfd == -1) {
        perror("socket()");
        exit(EXIT_FAILURE);
    }
    printf("socket: %d\n", sockfd);

    bzero(&sa, sizeof(sa));
    sa.sll_family   = AF_PACKET;
    sa.sll_protocol = htons(ETH_P_ALL);

    // Get Interface info
    strncpy(ifr.ifr_name, CPU_INTF_NAME, IFNAMSIZ);
    if(ioctl(sockfd, SIOCGIFINDEX, &ifr) < 0) {
        return -1;
    }
    sa.sll_ifindex  = ifr.ifr_ifindex;

    if ((retval=bind(sockfd, (struct sockaddr *)&sa, sizeof(sa))) != 0) {
        printf("%d bind failed.\n", retval);
        return -1;
    }

    // strncpy(ifr.ifr_name, CPU_INTF_NAME, IFNAMSIZ);
    // if(ioctl(sockfd, SIOCGIFHWADDR, &ifr) < 0) {
    //     return -1;
    // }
    // memcpy(sa.sll_addr,ether_broadcast_addr, ETHER_ADDR_LEN);
    // memcpy(sa.sll_addr,tap_lc_spoof_addr, ETHER_ADDR_LEN);
    // memcpy(sa.sll_addr,ifr.ifr_hwaddr.sa_data, ETHER_ADDR_LEN);

    // print sockaddr_ll
    // printf("sa: %#x %#x %#x\n", sa.sll_family, sa.sll_protocol, sa.sll_ifindex);
    // printf("\thw_addr: { ");
    // for (int i=0; i<ETHER_ADDR_LEN; i++) {
    //     printf("%02x ", sa.sll_addr[i]); 
    // }
    // printf("}\n");

    // Set message
    memset(msgs, 0, sizeof(msgs));
    for (i = 0; i < VLEN; i++) {
        memset(bufs[i], 0, sizeof(bufs[i]));
        iovecs[i].iov_base         = bufs[i];
        iovecs[i].iov_len          = sizeof(bufs[i]);
        msgs[i].msg_hdr.msg_iov    = &iovecs[i];
        msgs[i].msg_hdr.msg_iovlen = 1;
        // listner memcpy(bufs[i], pkt_buf, sizeof(pkt_buf));
        // msgs[i].msg_hdr.msg_name   = &sa;
        // msgs[i].msg_hdr.msg_namelen= sizeof(sa);
    }
    unsigned int count=0;
    while (1) { 
        retval = recvmmsg(sockfd, msgs, VLEN, 0, NULL);
        if (retval == -1) {
            perror("recvmmsg()");
            exit(EXIT_FAILURE);
        }
        count++;
        if ((count%10000 == 0) || ((bufs[0][65] == 0xfe) || (bufs[0][66] == 0xab))) {
            printf("messages received: %u\r", count);
            for (i = 0; i < VLEN; i++) {
                printf("msg %d:\n", i);
                jdump(bufs[i], sizeof(pkt_buf));
                printf("\n");
            }
        } else {
            memset(bufs[0], 0, sizeof(bufs[0]));
        }
    }

    exit(EXIT_SUCCESS);
}
#endif
