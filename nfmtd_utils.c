// #include <vlan_mgr_defs.h>
// #include <bd_mgr_defs.h>
// #include <syserr_defs.h>
// 
// #include "nfm.h"
// #include "nfm_homewood_hw_flow_format.h"
// #include "nfm_hea_hw_flow_format.h"
// #include "nfm_hw_flow_format.h"
// #include "nfm_netflow_hw.h"
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>

enum {
    PKT_SIZE = 1024,
};

uint8_t G_pkt[PKT_SIZE];

#define EHDR(pkt) ((struct ethhdr *)pkt)
#define IHDR(pkt) ((struct iphdr *)((uint8_t *)pkt + sizeof(struct ethhdr)))

#include <execinfo.h>
#include <sys/types.h>
#include <unistd.h>

#define nfmtd_print_bt(vargs...) do { \
    time_t curr_time; \
    struct tm ts; \
    char buffer[600]; \
    int len = 0; \
    void *array[10]; \
    size_t size = 5; \
    char **strings; \
    size_t i; \
    size = backtrace (array, 10); \
    strings = backtrace_symbols (array, size); \
    time(&curr_time); \
    ts = *localtime((time_t *)&curr_time); \
    len += strftime(buffer, sizeof(buffer), "%H:%M:%S %Z %b %d %Y", &ts); \
    len += snprintf(buffer+len, sizeof(buffer)-len, "[pid:%d]", getpid()); \
    for (i = 0; i < size-1; i++) { \
        len+= snprintf(buffer+len, sizeof(buffer)-len, "{%s}->", strings[i]); \
    } \
    len += snprintf(buffer+len, sizeof(buffer)-len, "[%s:%d]", __FUNCTION__, __LINE__);\
    len += snprintf(buffer+len, sizeof(buffer)-len, vargs); \
    FILE *fp = fopen("/tmp/nfmtd.log", "a"); \
    fprintf(fp, "%s\n",buffer); \
    free (strings); \
    fclose(fp); \
} while(0);

double nfm_stats[10];

#define nfmtd_print(vargs...) do { \
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
    FILE *fp = fopen("/tmp/nfmtd.log", "a"); \
    fprintf(fp, "%s\n",buffer); \
    fclose(fp); \
} while(0);

#define JQUTF_CALL(rc, err_str, goto_label, fn) do { \
    char buf[256]; \
    if ((rc = fn) == EOK) { \
        snprintf(buf, 256, "%s %s", err_str, "success."); \
        jqutf_print(buf, rc, cerr_strerror(rc)); \
    } else { \
        snprintf(buf, 256, "%s %s", err_str, "failed: rc:%#x (%s))"); \
        jqutf_print(buf, rc, cerr_strerror(rc)); \
        goto goto_label; \
    } \
} while(0);

#define nfmtd_tsstart \
	struct timespec jtsstart; \
    clock_gettime(CLOCK_MONOTONIC, &jtsstart);

#define nfmtd_tsstop(id, vargs...) do { \
	struct timespec diff_ts, jtsend; \
    clock_gettime(CLOCK_MONOTONIC, &jtsend); \
	if ((jtsend.tv_nsec-jtsstart.tv_nsec)<0) { \
		diff_ts.tv_sec = jtsend.tv_sec-jtsstart.tv_sec-1; \
		diff_ts.tv_nsec = 1000000000+jtsend.tv_nsec-jtsstart.tv_nsec; \
	} else { \
		diff_ts.tv_sec = jtsend.tv_sec-jtsstart.tv_sec; \
		diff_ts.tv_nsec = jtsend.tv_nsec-jtsstart.tv_nsec; \
	} \
	double udifftime = diff_ts.tv_sec*1000000 + diff_ts.tv_nsec/(double)1000; \
	nfm_stats[id] += udifftime; \
} while(0);

#define nfmtd_printstats do { \
	for(int i=0; i<10; i++) { \
		nfmtd_print("id: %d time: %.4f", i, nfm_stats[i]); \
	} \
} while(0);

void nfmtd_init ()
{
    FILE *fp = fopen("/tmp/nfmtd.log", "w");
    fclose(fp);
    return;
}

void nfmtd_mem_dump_hex (uint64_t id, const char *type, const uint8_t * pkt_in, size_t pkt_size)
{
    nfmtd_tsstart;
    nfmtd_print("ID: %10lld type: %s size:%d\n", id, type, pkt_size);
    nfmtd_print("{");
    for (int i=0; i<pkt_size; i++) {
        nfmtd_print("0x%2x", pkt_in[i]);
        if (i < pkt_size - 1) {
            nfmtd_print(",");
        } 
        if (i%16 == 0) {
            nfmtd_print("\n");
        }
    }
    nfmtd_print("}\n");
    nfmtd_tsstop(1);
    nfmtd_printstats;
}

static void   /* "static" means don't export the symbol... */
myfunc2(void)
{
    nfmtd_print("testing");
}

    void
myfunc(int ncalls)
{
    if (ncalls > 1)
        myfunc(ncalls - 1);
    else
        myfunc2();
}

    int
main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "%s num-calls\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    myfunc(atoi(argv[1]));
    exit(EXIT_SUCCESS);
}


//void func2(void) {
//    nfmtd_print("woring?");
//}
//void func1(void) {
//    printf("in func1");
//    func2();
//}
//void func(void) {
//    func1();
//}
//
//int main(int argc, const char *argv[])
//{
//    func(); 
//    return 0;
//}

//
//int nfmtd_make_flow_update_packet (nfmtd_homewood_ipv4_full_t *p, tetr_hdr_t *t) {
//    memset(pkt, 0, sizeof(pkt));
//
//    // Ethernet header
//    struct ethhdr *ehdr = EHDR(pkt_in);
//    ehdr->h_proto = htons(ETH_P_IP);
//
//    // IP header
//    struct iphdr *ihdr = IHDR(pkt_in);
//    int ip_len = 16;
//    ihdr->version = IPVERSION;
//    ihdr->protocol = IP_PROTO_UDP;
//    ihdr->ihl = ip_len;
//
//    // UDP header
//    struct udphdr *uhdr = (struct udphdr *)((char *)ihdr + ihdr->ihl*4);
//    int udp_len = nfmtd_HOMEWOOD_CE_FULL_SZ + sizeof(tetr_hdr_t) + sizeof(struct udphdr);
//    uhdr->uh_ulen = htons(udp_len);
//
//    // Tetration header
//    int num_flows = 1;
//    int exporter_id = 1;
//    tetr_hdr_t *thdr = (tetr_hdr_t *)((char *)uhdr + sizeof(struct udphdr));
//    exporter_id = exporter_id & 0x00ffffff;
//    thdr->exporter_id = SWAP_24(exporter_id);
//    num_flows = num_flows & 0x0000ffff;
//    thdr->num_flows = SWAP_16(num_flows);
//    thdr->window_start_time_hi = SWAP_32(window_start_time_hi);
//    thdr->window_start_time_lo = SWAP_32(window_start_time_lo);
//    thdr->prev_window_start_time_hi = SWAP_32(prev_window_start_time_hi);
//    thdr->prev_window_start_time_lo = SWAP_32(prev_window_start_time_lo);
//    char *nfmtd_payload = (char *)thdr + sizeof(tetr_hdr_t);
//}
//
//
//void nfmtd_pkt_dump (uint8_t * pkt_in, size_t pkt_size) {
//    int len = 0;
//
//    struct ethhdr *ehdr = EHDR(pkt_in);
//    struct iphdr *ihdr = IHDR(pkt_in);
//    struct udphdr *uhdr = (struct udphdr *)((char *)ihdr + ihdr->ihl*4);
//    nfmtd_print("ETH hexdump\n"); 
//    nfmtd_mem_dump(ehdr, sizeof(struct ethhdr));
//    nfmtd_print("IP hexdump\n"); 
//    nfmtd_mem_dump(ihdr, ihdr->ihl*4);
//    nfmtd_print("UDP hexdump\n"); 
//    nfmtd_mem_dump(ehdr, sizeof(struct udphdr));
//}
//
//int nfmtd_flow_spoof () {
//    nfmtd_hw_flow_rec_info_t record = {0};
//    nfmtd_homewood_ipv4_full_t p = {0};
//    tetr_hdr_t t = {0};
//
//    record.flow = &p;
//    record.t = &t;
//
//    p.ipv4_base.sip = 0x03010102;
//    p.ipv4_base.dip = 0x04010102;
//    p.ipv4_base.src_port = 0x1000;
//    p.ipv4_base.dst_port = 0x1001;
//    p.ipv4_base.pkt_cnt = 0x0fff;
//    p.ipv4_base.byte_cnt_0_to_31 = p.ipv4_base.pkt_cnt*128;
//    p.ipv4_base.profile_id = NFP_RTP_PROF_INDEX;
//    p.ipv4_analytics.analytics_latency_mode.udf.udf1 = 100;
//    t.num_flows = 1;
//
//    nfmtd_netflow_ipv4_update_hash ((void*) &record);
//
//    sleep(1);
//    p.ipv4_base.pkt_cnt = 220;
//    p.ipv4_base.byte_cnt_0_to_31 = p.ipv4_base.pkt_cnt*128;
//    p.ipv4_analytics.analytics_latency_mode.udf.udf1 = 320;
//    nfmtd_netflow_ipv4_update_hash ((void*) &record);
//
//    sleep(1);
//    p.ipv4_base.pkt_cnt = 111;
//    p.ipv4_base.byte_cnt_0_to_31 = p.ipv4_base.pkt_cnt*128;
//    p.ipv4_analytics.analytics_latency_mode.udf.udf1 = 431;
//    nfmtd_netflow_ipv4_update_hash ((void*) &record);
//
//    return 0;
//}
//
//     
