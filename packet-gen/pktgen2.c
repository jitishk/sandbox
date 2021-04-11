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
    NUM_FLOWS = 10000,
    MTU = 1500,
};

typedef struct pktgen_ {
    uint32_t tcp_fd;
    uint32_t udp_fd;
    uint32_t raw_fd;
    uint32_t tcpv6_fd;
    uint32_t udpv6_fd;
    uint32_t udpv6_fd_2;
    uint32_t rawv6_fd;
} pktgen_t;

pktgen_t pktgen;

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

    static void
sockaddr_set_v4 (struct sockaddr_in *addr, char *ip_str, uint32_t port)
{
    memset(addr, 0, sizeof(*addr));
    addr->sin_family = AF_INET;
    inet_pton(AF_INET, ip_str, &addr->sin_addr.s_addr);
    addr->sin_port = htons(port);
}

void
send_v6_packet (int fd, struct sockaddr_in6 *src_addr, struct sockaddr_in6 *dst_addr, void *payload, int payload_size) {
    if (sendto(fd, payload, payload_size, 0, (struct sockaddr *) dst_addr, sizeof(*dst_addr)) < 0) {
        perror("sendto failed.");
        exit(1);
    }
}

    static void
sockaddr_set_v6 (struct sockaddr_in6 *addr, char *ip_str, uint32_t port)
{
    memset(addr, 0, sizeof(*addr));
    addr->sin6_family = AF_INET6;
    inet_pton(AF_INET6, ip_str, &addr->sin6_addr.s6_addr);
    addr->sin6_port = htons(port);
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

    if ((pktgen.udpv6_fd_2 = socket(AF_INET6, SOCK_DGRAM, 0)) < 0) {
        pktgen_print("UDP socket creation failed");
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
    }
}

typedef struct pgen_ip_hdr_ {
    char src_ip[40];
    char dst_ip[40];
    uint8_t tos;        // dscp
} pgen_ip_hdr_t;

typedef struct pgen_udp_hdr_ {
    uint16_t src_port;
    uint16_t dst_port;
} pgen_udp_hdr_t;

typedef struct pgen_rtp_hdr_ {
    uint32_t version    :2;
    uint32_t padding    :1;
    uint32_t extension  :1;
    uint32_t cscrc_count:4;
    uint32_t marker     :1;
    uint32_t payload    :7;
    uint32_t seq_no     :16;    // 32 bits
    uint32_t timestamp;
} pgen_rtp_hdr_t;

    void
pktgen_ipv6_flows (int argc, char *argv[])
{
    int num_flows;
    pgen_ip_hdr_t ip_hdr;
    pgen_udp_hdr_t udp_hdr;
    pgen_rtp_hdr_t rtp_hdr;
    memset(&ip_hdr, 0, sizeof(ip_hdr));
    udp_hdr.src_port = 16384;
    udp_hdr.dst_port = 16384;
    rtp_hdr.seq_no = 1;
    rtp_hdr.timestamp = 0xdeadbeef;

    if (argc>=3) {
        strncpy(ip_hdr.src_ip, argv[2], sizeof(ip_hdr.src_ip));
    } else {
        strncpy(ip_hdr.src_ip, "1001:2002:3003:4004:5005:6006:7007:8008", sizeof(ip_hdr.src_ip));
    }
    if (argc>=4) {
        strncpy(ip_hdr.dst_ip, argv[3], sizeof(ip_hdr.dst_ip));
    } else {
        strncpy(ip_hdr.dst_ip, "1001:2002:3003:4004:5005:6006:7007:8009", sizeof(ip_hdr.dst_ip));
    }
    if (argc >=5) {
        num_flows =  atoi(argv[4]);
    } else {
        num_flows = 1;
    }
    struct sockaddr_in6 src_addr;
    sockaddr_set_v6(&src_addr, ip_hdr.src_ip, udp_hdr.src_port);
    if (bind(pktgen.udpv6_fd, (struct sockaddr *) &src_addr, sizeof(src_addr)) < 0) {
        perror("v6 bind");
        exit(1);
    }

    printf("Generating %d IPv6 flows", num_flows);
    while(1) {
        udp_hdr.dst_port = 16384;
        for (int i = 0; i < num_flows; i++) {
            struct sockaddr_in6 dst_addr;
            sockaddr_set_v6(&dst_addr, ip_hdr.dst_ip, udp_hdr.dst_port);
            send_v6_packet(pktgen.udpv6_fd, &src_addr, &dst_addr, &rtp_hdr, sizeof(rtp_hdr));
            udp_hdr.dst_port++;
        }

        uint32_t rtp_seq_no = 0;
        time_t  rtp_tstamp = time(NULL);

        rtp_seq_no++;
        // simulate loss
        // if(rtp_seq_no > 100 && rtp_seq_no < 1000) {
        //     rtp_seq_no++;
        // }
        rtp_hdr.seq_no = htons(rtp_seq_no);
        rtp_tstamp = time(NULL);
        rtp_hdr.timestamp = htonl(rtp_tstamp);
        // rtp_hdr.timestamp = htonl(0xdeadbeef);
        // usleep(10000); //sleep for 10ms
    }
}

void
pktgen_ipv4_flows (int argc, char *argv[])
{
    int num_flows;
    pgen_ip_hdr_t ip_hdr;
    pgen_udp_hdr_t udp_hdr;
    pgen_rtp_hdr_t rtp_hdr;
    printf("Generating %d IPv4 flows\n", num_flows);
    memset(&ip_hdr, 0, sizeof(ip_hdr));
    if (argc>=3) {
        strncpy(ip_hdr.src_ip, argv[2], sizeof(ip_hdr.src_ip));
    } else {
        strncpy(ip_hdr.src_ip, "2.2.2.2", sizeof(ip_hdr.src_ip));
    }
    if (argc>=4) {
        strncpy(ip_hdr.dst_ip, argv[3], sizeof(ip_hdr.dst_ip));
    } else {
        strncpy(ip_hdr.src_ip, "2.2.2.1", sizeof(ip_hdr.src_ip));
    }
    if (argc >=5) {
        num_flows =  atoi(argv[4]);
    } else {
        num_flows = 1;
    }
    udp_hdr.src_port = 17000;
    udp_hdr.src_port = 4112;
    udp_hdr.dst_port = 17000;
    udp_hdr.dst_port = 319;
    rtp_hdr.seq_no = 1;
    rtp_hdr.timestamp = 0xdeadbeef;

    struct sockaddr_in src_addr;
    sockaddr_set_v4(&src_addr, ip_hdr.src_ip, udp_hdr.src_port);
    if (bind(pktgen.udp_fd, (struct sockaddr *) &src_addr, sizeof(src_addr)) < 0) {
        perror("bind");
        exit(1);
    }

    struct sockaddr_in dst_addr;
    sockaddr_set_v4(&dst_addr, ip_hdr.dst_ip, udp_hdr.dst_port);

    uint32_t rtp_seq_no = 0;
    time_t  rtp_tstamp = time(NULL);

    printf("Generating %d IPv4 flows", num_flows);
    while (1) {
        udp_hdr.dst_port = 17000;
        udp_hdr.dst_port = 319;
        for (int i=0; i<num_flows; i++) {
            send_packet(pktgen.udp_fd, &src_addr, &dst_addr, &rtp_hdr, sizeof(rtp_hdr));
            udp_hdr.dst_port++;
        }
        //usleep(10000); //sleep for 10ms

        rtp_seq_no++;
        // simulate loss
        //if(rtp_seq_no > 100 && rtp_seq_no < 1000) {
        //    rtp_seq_no++;
        //}
        rtp_hdr.seq_no = htons(rtp_seq_no);
        rtp_tstamp = time(NULL);
        rtp_hdr.timestamp = htonl(rtp_tstamp);
        rtp_hdr.timestamp = htonl(0xdeadbeef);
    }
}

int main (int argc, char *argv[])
{
    pktgen_init();

    if ((argc == 2) && (!strcmp(argv[1], "v6"))) {
        pktgen_ipv6_flows(argc, argv);
    }
    if ((argc >= 2) && (!strcmp(argv[1], "v4"))) {
        pktgen_ipv4_flows(argc, argv);
    }
    return 0;
}
