#include <sys/types.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define pktgen_print(vargs...) do { \
    time_t curr_time; \
    struct tm ts; \
    char buffer[600]; \
    int len = 0; \
    time(&curr_time); \
    ts = *localtime((time_t *)&curr_time); \
    len += strftime(buffer, sizeof(buffer), "%H:%M:%S %Z %b %d %Y", &ts); \
    len += snprintf(buffer+len, sizeof(buffer)-len, "[pid:%d]", getpid()); \
    len += snprintf(buffer+len, sizeof(buffer)-len, "[%s:%d]", __FUNCTION__, __LINE__);\
    len += snprintf(buffer+len, sizeof(buffer)-len, vargs); \
    FILE *fp = fopen("/tmp/pktgen.log", "a"); \
    fprintf(fp, "%s\n",buffer); \
    printf("%s\n",buffer); \
    fclose(fp); \
} while(0);

enum {
    RTP_UDP_PORT_LO = 16384,
    RTP_UDP_PORT_HIGH = 32767,
    MTU = 1500,
};

typedef struct pktgen_ {
    uint32_t tcp_fd;
    uint32_t udp_fd;
    uint32_t raw_fd;
    uint32_t tcpv6_fd;
    uint32_t udpv6_fd;
    uint32_t rawv6_fd;
} pktgen_t;

pktgen_t pktgen;

typedef struct rtp_hdr_ {
    uint32_t version    :2;
    uint32_t padding    :1;
    uint32_t extension  :1;
    uint32_t cscrc_count:4;
    uint32_t marker     :1;
    uint32_t payload    :7;
    uint32_t seq_no     :16;    // 32 bits

    uint32_t timestamp;         // 64 bits
    uint32_t ssrc_identifier;   // 96 bits
    uint32_t dummydata[1];      // 128 bits = 16 bytes
} rtp_hdr_t;                    

typedef struct ip_hdr_ {
    uint32_t version    :4;
    uint32_t IHL        :4;
    uint32_t dscp       :6;     // same as TOS
    uint32_t ECN        :2;
    uint32_t total_len  :16;    // 32 bits
    uint32_t  identification:16;
    uint32_t flags      :3;
    uint32_t fragment_offset:13;// 64 bits
    uint32_t ttl        :8;
    uint32_t protocol   :8;
    uint32_t hdr_chksum :16;    // 96 bits
    uint32_t src_ip;            // 128 bits
    uint32_t dst_ip;            // 160 bits = 20 bytes
    uint32_t options[0];
} ip_hdr_t;

typedef struct ipv6_hdr_ {
    uint32_t version    :4;
    uint32_t traffic_class:8;
    uint32_t flow_label :20;    // 32 bits
    uint32_t payload_len:16;
    uint32_t next_hdr   :8;
    uint32_t hop_limit  :8;     // 64 bits
    uint32_t src_ip[4];       // 192 bits 
    uint32_t dst_ip[4];       // 320 bits == 40 bytes
} ipv6_hdr_t;

typedef struct tcp_hdr_ {
    uint32_t src_port   :16;
    uint32_t dst_port   :16;    // 32 bits
    uint32_t seq_no;            // 64 bits
    uint32_t ack_no;            // 96 bits
    uint32_t data_offset:4;
    uint32_t reserved   :3;
    uint32_t ns         :1;
    uint32_t cwr        :1;
    uint32_t ece        :1;
    uint32_t urg        :1;
    uint32_t ack        :1;
    uint32_t psh        :1;
    uint32_t rst        :1;
    uint32_t syn        :1;
    uint32_t fin        :1;
    uint32_t window_size:16;    // 128 bits
    uint32_t checksum   :16;
    uint32_t urg_pointer:16;    // 160 bits = 20 bytes
    uint32_t options[0];
} tcp_hdr_t;

typedef struct udp_hdr_ {
    uint32_t src_port   :16;
    uint32_t dst_port   :16;    // 32 bits
    uint32_t length     :16;
    uint32_t checksum   :16;    // 64 bits = 8 bytes
} udp_hdr_t;

typedef struct payload_ {
    uint8_t data[1500];
} payload_t;

void
send_packet (int fd, struct sockaddr_in *src_addr, struct sockaddr_in *dst_addr, void *payload, int payload_size) {
	if (sendto(fd, payload, payload_size, 0, (struct sockaddr *) dst_addr, sizeof(*dst_addr)) < 0) {
		perror("sendto failed.");
		exit(1);
	}
}

void
send_v6_packet (int fd, struct sockaddr_in6 *src_addr, struct sockaddr_in6 *dst_addr, void *payload, int payload_size) {
	if (sendto(fd, payload, payload_size, 0, (struct sockaddr *) dst_addr, sizeof(*dst_addr)) < 0) {
		perror("sendto failed.");
		exit(1);
	}
}

static void
sockaddr_set_v4 (struct sockaddr_in *addr, char *ip_str, uint32_t port)
{
    memset(addr, 0, sizeof(*addr));
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = inet_addr(ip_str);
    addr->sin_port = htons(port);
}

void
pktgen_send_v4_rtp_pkt (ip_hdr_t *ip_hdr, udp_hdr_t *udp_hdr, rtp_hdr_t *rtp_hdr, payload_t *payload)
{
    pktgen_print("src ip/port %s/%d binding", inet_ntoa(*((struct in_addr *)&ip_hdr->src_ip)), udp_hdr->src_port);
    // set socket options
    if (ip_hdr->dscp != 0) {
        uint8_t dscp = ip_hdr->dscp;
        if(setsockopt(pktgen.udp_fd, IPPROTO_IP, IP_TOS, &dscp, sizeof(dscp))) {
            pktgen_print("set TOS %#x failed", ip_hdr->dscp);
            exit(1);
        }
    }

    // set source parameters
    struct sockaddr_in src_addr;
    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.sin_family = AF_INET;
    memcpy(&src_addr.sin_addr, &ip_hdr->src_ip, sizeof(src_addr.sin_addr));
    src_addr.sin_port = htons(udp_hdr->src_port);
    pktgen_print("src ip/port %s/%d binding", inet_ntoa(src_addr.sin_addr), src_addr.sin_port);
    if (bind(pktgen.udp_fd, (struct sockaddr *) &src_addr, sizeof(src_addr)) < 0) {
        pktgen_print("src ip/port %s/%d bind failed", inet_ntoa(*((struct in_addr *)&ip_hdr->src_ip)), src_addr.sin_port);
        exit(1);
    }

    //set dest parameters
    struct sockaddr_in dst_addr;
    memset(&dst_addr, 0, sizeof(dst_addr));
    dst_addr.sin_family = AF_INET;
    memcpy(&dst_addr.sin_addr, &ip_hdr->dst_ip, sizeof(dst_addr.sin_addr));
    dst_addr.sin_port = htons(udp_hdr->dst_port);

    send_packet(pktgen.udp_fd, &src_addr, &dst_addr, &rtp_hdr, sizeof(rtp_hdr));
}

void
pktgen_send_v6_rtp_pkt (ipv6_hdr_t *ipv6_hdr, udp_hdr_t *udp_hdr, rtp_hdr_t *rtp_hdr, payload_t *payload)
{
    char ipv6_addr_str[40];

    // set source parameters
    struct sockaddr_in6 src_addr;
    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.sin6_family = AF_INET6;
    memcpy(&src_addr.sin6_addr, &ipv6_hdr->src_ip, sizeof(src_addr.sin6_addr));
    src_addr.sin6_port = htons(udp_hdr->src_port);
    if (bind(pktgen.udpv6_fd, (struct sockaddr *) &src_addr, sizeof(src_addr)) < 0) {
        pktgen_print("src ip/port %s/%d bind failed",
                inet_ntop(AF_INET6, ipv6_hdr->src_ip, ipv6_addr_str, sizeof(ipv6_addr_str)),
                udp_hdr->src_port);
        exit(1);
    }

    //set dest parameters
    struct sockaddr_in6 dst_addr;
    memset(&dst_addr, 0, sizeof(dst_addr));
    dst_addr.sin6_family = AF_INET6;
    memcpy(&dst_addr.sin6_addr, &ipv6_hdr->src_ip, sizeof(dst_addr.sin6_addr));
    dst_addr.sin6_port = htons(udp_hdr->dst_port);
 
    send_v6_packet(pktgen.udpv6_fd, &src_addr, &dst_addr, &rtp_hdr, sizeof(rtp_hdr));
}


void
pktgen_init ()
{
    memset(&pktgen, 0, sizeof(pktgen));

    if ((pktgen.udp_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        pktgen_print("UDP socket creation failed");
        exit(1);
    }
    if ((pktgen.tcp_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        pktgen_print("TCP socket creation failed");
        exit(1);
    }
    if ((pktgen.raw_fd = socket(AF_INET, SOCK_RAW, 0)) < 0) {
        pktgen_print("raw socket creation failed");
        exit(1);
    }

    if ((pktgen.udpv6_fd = socket(AF_INET6, SOCK_DGRAM, 0)) < 0) {
        pktgen_print("UDP socket creation failed");
        exit(1);
    }
    if ((pktgen.tcpv6_fd = socket(AF_INET6, SOCK_STREAM, 0)) < 0) {
        pktgen_print("TCP socket creation failed");
        exit(1);
    }
    if ((pktgen.rawv6_fd = socket(AF_INET6, SOCK_RAW, 0)) < 0) {
        pktgen_print("raw socket creation failed");
        exit(1);
    }}

int main (int argc, char *argv[]) {

    pktgen_init();

    int fd;
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }


    char src_ip[] = "2.2.2.2";
    uint32_t src_port = 16384;
    struct sockaddr_in src_addr;
    sockaddr_set_v4(&src_addr, src_ip, src_port);
    if (bind(fd, (struct sockaddr *) &src_addr, sizeof(src_addr)) < 0) {
        perror("bind");
        exit(1);
    }

    char dst_ip[] = "2.2.2.1";
    uint32_t dst_port = 16384;
    struct sockaddr_in dst_addr;
    sockaddr_set_v4(&dst_addr, dst_ip, dst_port);

    ip_hdr_t ip_hdr;
    memset(&ip_hdr, 0, sizeof(ip_hdr));
    ip_hdr.src_ip = inet_addr(src_ip);
    ip_hdr.dst_ip = inet_addr(dst_ip);
    udp_hdr_t udp_hdr;
    memset(&udp_hdr, 0, sizeof(udp_hdr));
    udp_hdr.src_port = htons(src_port);
    udp_hdr.dst_port = htons(dst_port);

    uint32_t rtp_seq_no = 0;
    time_t  rtp_tstamp = time(NULL);
    rtp_hdr_t rtp_hdr;
    memset(&rtp_hdr, 0, sizeof(rtp_hdr));
    rtp_hdr.version = 1;
    rtp_hdr.seq_no = htons(rtp_seq_no);
    rtp_hdr.timestamp = htonl(rtp_tstamp);

    while (1) {
        send_packet(fd, &src_addr, &dst_addr, &rtp_hdr, sizeof(rtp_hdr));
        usleep(10000); //sleep for 10ms

        rtp_seq_no++;
        // simulate loss 
        if(rtp_seq_no > 100 && rtp_seq_no < 1000) {
            rtp_seq_no++;
        }
        rtp_hdr.seq_no = htons(rtp_seq_no);
        rtp_tstamp = time(NULL);        
        rtp_hdr.timestamp = htonl(rtp_tstamp);
        //rtp_hdr.timestamp = htonl(0xdeadbeef);
    }
    return 0;
}
