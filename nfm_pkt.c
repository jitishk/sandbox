#include <stdio.h>
//#include <linux/if_ether.h>
#include <stdint.h>
//#include "ip.h"
//#include <linux/udp.h>
#include <string.h>
//#include <arpa/inet.h>
//#include <netinet/udp.h>
#include <stdbool.h>

#include "netinet/ip.h"
#include "netinet/tcp.h"
#include "netinet/udp.h"
#include <syserr_defs.h>

//#define PACKED __attribute__ ((packed)) 
//#include "nfm.h"
#include "nfm_hw_flow_format.h"
typedef union analytics_latency_mode_ {
        struct latency_mode_false {
            uint32_t seqack__seq_num        : 32;
            uint32_t seqack__ack_num        : 32;
        } latency_mode_false;

        struct latency_mode_true {
            uint32_t latency__max           : 16;
            uint32_t latency__sum_0_to_31   : 32;
            uint32_t latency__sum_32_to_47  : 16;
        } latency_mode_true;
        struct udf {
            uint16_t udf1   : 16;
            uint16_t udf2   : 16;
            uint16_t udf3   : 16;
        } udf;
} PACKED analytics_latency_mode_t;
#include <nfm_homewood_hw_flow_format.h>


typedef struct _nfm_pkt {
    struct ethhdr ehdr;
    struct iphdr  ihdr;
    struct udphdr uhdr;
    void * data[0];
} nfm_pkt_t;


uint16_t pkt_in_hex[] = {
0x003a, 0x9c2c, 0x4667, 0x0000, 0x0000, 0x1b05, 0x0800, 0x45b0,
0x0082, 0x0000, 0x0000, 0x4111, 0x7c7f, 0x7f01, 0x7f19, 0x7f01,
0x7f20, 0x07d1, 0x0000, 0x006e, 0x0000, 0x0000, 0x6100, 0x04d2,
0x5c92, 0x9c52, 0x33fc, 0xe288, 0x5c92, 0x9c52, 0x2c9f, 0x5ce0,
0x0001, 0x0000, 0x0000, 0x0034, 0x0040, 0x17f9, 0xa010, 0x1020,
0xf0a0, 0xc0b0, 0xe204, 0xe20e, 0xf124, 0x1114, 0x10e0, 0x0058,
0x03f8, 0x0004, 0x0080, 0x07e8, 0x0000, 0x0000, 0x0000, 0x0001,
0x0008, 0x0508, 0x0101, 0x0001, 0x0000, 0x0000, 0x0400, 0x0af0,
0xb405, 0xa600, 0x0200, 0x0ff0, 0x0000, 0x0000, 0x0000, 0x7f3e
};

void
nfm_pkt_set_udp (nfm_pkt_t *pkt, uint16_t payloadlen){
    memset(pkt, 0, sizeof(nfm_pkt_t));
//   pkt->ehdr.h_proto = htons(ETH_P_IP);
//   pkt->ihdr.saddr = htonl(0x0a010101);
//   pkt->ihdr.daddr = htonl(0x14010101);
//   pkt->ihdr.protocol = 0x11; //UDP = 0x11 = 17
//   pkt->uhdr.dest = 20000;
//   pkt->uhdr.source = 20000;
    //pkt->uhdr.uh_ulen = htons(sizeof(pkt->uhdr) + payloadlen);
}

void
nfm_pkt_set_tetr (nfm_pkt_t *pkt)
{
}

#define jprint(fmt, ...) { \
    FILE *fp = fopen("/tmp/jpkt.log", "a"); \
    char err_buffer[300]; \
    int slen = snprintf(err_buffer, sizeof(err_buffer), "[%s:%d] ", __FILE__, __LINE__); \
    fprintf(fp, "%s", err_buffer); \
    slen = slen; \
    fclose(fp); \
}

// snprintf(err_buffer+slen, 300-slen, fmt, __VA_ARGS__);
char *
nfm_decode_raw_pkt(uint8_t *inpkt_p, uint32_t total_len, int *nfm_pkt_len);
void
nfm_parse_export_pkt(unsigned char *pkt, int pkt_size);

#define IP_PROTO_UDP  17
#define NFM_HOMEWOOD_CE_FULL_SZ 72

void
nfm_fake_sock_msg ()
{
    unsigned int nfm_len;
    char *nfm_data;

    jprint("send fake sock msg");
    memset(pkt_in_hex, 0, sizeof(pkt_in_hex));
    //pkt_in_hex[6] = ntohs(pkt_in_hex[6]);

    struct ethhdr *ehdr  = (struct ethhdr *)pkt_in_hex;
    ehdr->h_proto = htons(ETH_P_IP);

    struct iphdr *ihdr  = (struct iphdr *)((char *)ehdr + sizeof(struct ethhdr));
    int ip_len = 16;
    ihdr->version = IPVERSION;
    ihdr->protocol = IP_PROTO_UDP;
    ihdr->ihl = ip_len;

    struct udphdr *uhdr = (struct udphdr *)((char *)ihdr + ihdr->ihl*4);
    int udp_len = NFM_HOMEWOOD_CE_FULL_SZ + sizeof(tetr_hdr_t) + sizeof(struct udphdr);
    uhdr->uh_ulen = htons(udp_len);

    int num_flows = 1;
    tetr_hdr_t *thdr = (tetr_hdr_t *)((char *)uhdr + sizeof(struct udphdr));
    thdr->num_flows = SWAP_16(num_flows);
    char *nfm_payload = (char *)thdr + sizeof(tetr_hdr_t);

    //pkt_dump(pkt_in_hex, sizeof(pkt_in_hex));


    nfm_homewood_ipv4_full_t ipv4_full;
    ipv4_full.ipv4_base.sip = 0x01020304;
    ipv4_full.ipv4_base.dip = 0x05060708;
    ipv4_full.ipv4_base.dst_port = 2000;
    ipv4_full.ipv4_base.src_port = 2001;
    ipv4_full.ipv4_base.byte_cnt_0_to_31 = 123456;
    ipv4_full.ipv4_base.pkt_cnt = 5000;
    ipv4_full.ipv4_base.src_tep = 1536; // IFTMC_TAH_PC_VIF_INDEX_BASE
    ipv4_full.ipv4_base.tenant_id = 0x1a006600; //if_index or hw_bd_id
    ipv4_full.ipv4_base.profile_id = 0x0; //if_index or hw_bd_id

    int loop_limit = 28;
    for (int i=0; i<loop_limit; i++) {
        nfm_payload[i*4+0] = ((char *)&ipv4_full)[i*4+3];
        nfm_payload[i*4+1] = ((char *)&ipv4_full)[i*4+2];
        nfm_payload[i*4+2] = ((char *)&ipv4_full)[i*4+1];
        nfm_payload[i*4+3] = ((char *)&ipv4_full)[i*4+0];
        /*NFM_EVENT_LOG_TRACE("Yogesh: PACKET --> %x %x %x %x",
                data[i*4+3],data[i*4+2], data[i*4+1], data[i*4+0]);*/
    }

    for (int i=0; i<18; i++) {
        ((uint32_t *)nfm_payload)[i] = htonl(((uint32_t *)&ipv4_full)[i]);
    }
    nfm_payload[3] = (0x1F & 0x19);

    nfm_data = nfm_decode_raw_pkt((char *)pkt_in_hex, 9*8*2, &nfm_len);
    nfm_parse_export_pkt(nfm_data, nfm_len);
}










