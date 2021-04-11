void nfm_flow_rtp_spoof();

nfm_flow_rtp_spoof();

static void nfm_flow_ipv4_loss_free_homewood (nfm_homewood_ipv4_full_t *p, 
        nfm_hw_flow_rec_info_t *record) {
    static uint16_t seqNum1 = 0;
    p->ipv4_base.pkt_cnt = 60000;
    p->ipv4_base.byte_cnt_0_to_31 = p->ipv4_base.pkt_cnt*128;
    seqNum1 = (seqNum1 + p->ipv4_base.pkt_cnt) % (65536);
    p->ipv4_analytics.analytics_latency_mode.udf.udf1 = seqNum1;
    buginf_wrap(DBG_NFM_ERROR, "%s: seqNum1:%u pkt_cnt:%u \n", __FUNCTION__, seqNum1, p->ipv4_base.pkt_cnt);
    nfm_netflow_ipv4_update_hash ((void*) record);

    usleep(50000);
    p->ipv4_base.pkt_cnt = 60000;
    p->ipv4_base.byte_cnt_0_to_31 = p->ipv4_base.pkt_cnt*128;
    seqNum1 = (seqNum1 + p->ipv4_base.pkt_cnt) % (65536);
    buginf_wrap(DBG_NFM_ERROR, "%s: seqNum1:%u pkt_cnt:%u \n", __FUNCTION__, seqNum1, p->ipv4_base.pkt_cnt);
    p->ipv4_analytics.analytics_latency_mode.udf.udf1 = seqNum1;
    nfm_netflow_ipv4_update_hash ((void*) record);

    usleep(50000);
    p->ipv4_base.pkt_cnt = 60000;
    p->ipv4_base.byte_cnt_0_to_31 = p->ipv4_base.pkt_cnt*128;
    seqNum1 = (seqNum1 + p->ipv4_base.pkt_cnt) % (65536);
    buginf_wrap(DBG_NFM_ERROR, "%s: seqNum1:%u pkt_cnt:%u \n", __FUNCTION__, seqNum1, p->ipv4_base.pkt_cnt);
    p->ipv4_analytics.analytics_latency_mode.udf.udf1 = seqNum1;
    nfm_netflow_ipv4_update_hash ((void*) record);
}

static void nfm_flow_ipv4_lossy_homewood (nfm_homewood_ipv4_full_t *p, 
        nfm_hw_flow_rec_info_t *record) {
    static uint16_t seqNum = 0;
    p->ipv4_base.pkt_cnt = 20;
    p->ipv4_base.byte_cnt_0_to_31 = p->ipv4_base.pkt_cnt*128;
    seqNum = (seqNum + p->ipv4_base.pkt_cnt) % (65536);
    p->ipv4_analytics.analytics_latency_mode.udf.udf1 = seqNum;
    nfm_netflow_ipv4_update_hash ((void*) record);

    usleep(50000);
    p->ipv4_base.pkt_cnt = 70000;
    uint32_t pkt_loss = 3332;
    p->ipv4_base.byte_cnt_0_to_31 = p->ipv4_base.pkt_cnt*128;
    seqNum = (seqNum + p->ipv4_base.pkt_cnt) % (65536) + pkt_loss;
    p->ipv4_analytics.analytics_latency_mode.udf.udf1 = seqNum;
    nfm_netflow_ipv4_update_hash ((void*) record);

    usleep(50000);
    p->ipv4_base.pkt_cnt = 70000;
    pkt_loss = 0;
    p->ipv4_base.byte_cnt_0_to_31 = p->ipv4_base.pkt_cnt*128;
    seqNum = (seqNum + p->ipv4_base.pkt_cnt) % (65536) + pkt_loss;
    p->ipv4_analytics.analytics_latency_mode.udf.udf1 = seqNum;
    nfm_netflow_ipv4_update_hash ((void*) record);

    usleep(50000);
    p->ipv4_base.pkt_cnt = 70000;
    pkt_loss = 1;
    p->ipv4_base.byte_cnt_0_to_31 = p->ipv4_base.pkt_cnt*128;
    seqNum = (seqNum + p->ipv4_base.pkt_cnt) % (65536) + pkt_loss;
    p->ipv4_analytics.analytics_latency_mode.udf.udf1 = seqNum;
    nfm_netflow_ipv4_update_hash ((void*) record);
}

static void nfm_flow_ipv4_ooo_homewood (nfm_homewood_ipv4_full_t *p, 
        nfm_hw_flow_rec_info_t *record)
{
    static uint16_t seqOOO = 15;
    p->ipv4_base.pkt_cnt = 20;
    p->ipv4_base.byte_cnt_0_to_31 = p->ipv4_base.pkt_cnt*128;
    seqOOO = (seqOOO + p->ipv4_base.pkt_cnt) % (65536);
    p->ipv4_analytics.analytics_latency_mode.udf.udf1 = seqOOO;
    nfm_netflow_ipv4_update_hash ((void*) record);

    usleep(50000);
    p->ipv4_base.pkt_cnt = 20;
    uint32_t pkt_loss = 2;
    uint32_t pkt_gain = 0;
    p->ipv4_base.byte_cnt_0_to_31 = p->ipv4_base.pkt_cnt*128;
    seqOOO = (seqOOO + p->ipv4_base.pkt_cnt) % (65536) + pkt_loss - pkt_gain;
    p->ipv4_analytics.analytics_latency_mode.udf.udf1 = seqOOO;
    nfm_netflow_ipv4_update_hash ((void*) record);

    usleep(50000);
    p->ipv4_base.pkt_cnt = 20;
    pkt_loss = 0;
    pkt_gain = 0;
    p->ipv4_base.byte_cnt_0_to_31 = p->ipv4_base.pkt_cnt*128;
    seqOOO = (seqOOO + p->ipv4_base.pkt_cnt) % (65536) + pkt_loss - pkt_gain;
    p->ipv4_analytics.analytics_latency_mode.udf.udf1 = seqOOO;
    nfm_netflow_ipv4_update_hash ((void*) record);

    usleep(50000);
    p->ipv4_base.pkt_cnt = 20;
    pkt_loss = 0;
    pkt_gain = 2;
    p->ipv4_base.byte_cnt_0_to_31 = p->ipv4_base.pkt_cnt*128;
    seqOOO = (seqOOO + p->ipv4_base.pkt_cnt) % (65536) + pkt_loss - pkt_gain;
    p->ipv4_analytics.analytics_latency_mode.udf.udf1 = seqOOO;
    nfm_netflow_ipv4_update_hash ((void*) record);
}

void nfm_flow_ipv6_lossy_homewood (nfm_homewood_ipv6_full_t *p, 
        nfm_hw_flow_rec_info_t *record)
{
    static uint16_t seqNum = 0;
    p->ipv6_base.pkt_cnt = 20;
    p->ipv6_base.byte_cnt_0_to_31 = p->ipv6_base.pkt_cnt*128;
    seqNum = (seqNum + p->ipv6_base.pkt_cnt) % (65536);
    p->ipv6_analytics.analytics_latency_mode.udf.udf1 = seqNum;
    nfm_netflow_ipv6_update_hash ((void*) record);

    usleep(50000);
    p->ipv6_base.pkt_cnt = 70000;
    uint32_t pkt_loss = 66000;
    p->ipv6_base.byte_cnt_0_to_31 = p->ipv6_base.pkt_cnt*128;
    seqNum = (seqNum + p->ipv6_base.pkt_cnt) % (65536) + pkt_loss;
    p->ipv6_analytics.analytics_latency_mode.udf.udf1 = seqNum;
    nfm_netflow_ipv6_update_hash ((void*) record);

    usleep(50000);
    p->ipv6_base.pkt_cnt = 70000;
    pkt_loss = 0;
    p->ipv6_base.byte_cnt_0_to_31 = p->ipv6_base.pkt_cnt*128;
    seqNum = (seqNum + p->ipv6_base.pkt_cnt) % (65536) + pkt_loss;
    p->ipv6_analytics.analytics_latency_mode.udf.udf1 = seqNum;
    nfm_netflow_ipv6_update_hash ((void*) record);

    usleep(50000);
    p->ipv6_base.pkt_cnt = 70000;
    pkt_loss = 1;
    p->ipv6_base.byte_cnt_0_to_31 = p->ipv6_base.pkt_cnt*128;
    seqNum = (seqNum + p->ipv6_base.pkt_cnt) % (65536) + pkt_loss;
    p->ipv6_analytics.analytics_latency_mode.udf.udf1 = seqNum;
    nfm_netflow_ipv6_update_hash ((void*) record);
}

void nfm_flow_rtp_spoof_homewood ()
{
    nfm_hw_flow_rec_info_t record = {0};
    nfm_homewood_ipv4_full_t p;
    nfm_homewood_ipv6_full_t p6;
    tetr_hdr_t t = {0};
    memset(&p, 0, sizeof(nfm_homewood_ipv4_full_t));
    memset(&p6, 0, sizeof(nfm_homewood_ipv6_full_t));

    p.ipv4_base.profile_id = NFP_RTP_PROF_INDEX;
    t.num_flows = 1;
    p6.ipv6_base.profile_id = NFP_RTP_PROF_INDEX;

    record.flow = &p;
    record.t = &t;

    // Get the first interface
    pt_get_next_attrtype attr;
    if_index_t ifindex_walk = 0;
    ushort mask_len = 0;
    uint32_t hw_bd_id = 0;
    // pt_get_next_attr_init(&attr);
    // nfm_intf_t *intf = nfm_pt_get_first_intf(&ifindex_walk, &mask_len, &attr);
    // //im_get_ifindex_name(v4_metrics.if_id, (char*) ifname, sizeof(ifname));
    // eltm_query_ifindex_to_lif(intf->ifindex, &hw_bd_id);

    p.ipv4_base.sip = 0x03010102;
    p.ipv4_base.dip = 0x04010102;
    p.ipv4_base.src_port = 0x1000;
    p.ipv4_base.dst_port = 0x1001;
    //p.ipv4_base.tenant_id = hw_bd_id;
    nfm_flow_ipv4_loss_free_homewood(&p, &record);

    // intf = nfm_pt_get_next_intf(&ifindex_walk, &mask_len, &attr);
    // eltm_query_ifindex_to_lif(intf->ifindex, &hw_bd_id);
    p.ipv4_base.sip = 0x13010102;
    p.ipv4_base.dip = 0x14010102;
    p.ipv4_base.src_port = 0x1001;
    p.ipv4_base.dst_port = 0x1002;
    //p.ipv4_base.tenant_id = hw_bd_id;
    nfm_flow_ipv4_lossy_homewood(&p, &record);
 
    // intf = nfm_pt_get_next_intf(&ifindex_walk, &mask_len, &attr);
    // eltm_query_ifindex_to_lif(intf->ifindex, &hw_bd_id);
    p.ipv4_base.sip = 0x23010102;
    p.ipv4_base.dip = 0x24010102;
    p.ipv4_base.src_port = 0x2001;
    p.ipv4_base.dst_port = 0x2002;
    //p.ipv4_base.tenant_id = hw_bd_id;
    nfm_flow_ipv4_ooo_homewood(&p, &record);

    // intf = nfm_pt_get_next_intf(&ifindex_walk, &mask_len, &attr);
    // eltm_query_ifindex_to_lif(intf->ifindex, &hw_bd_id);
    p6.ipv6_ext.sipv6_3 = 1;
    p6.ipv6_ext.sipv6_2 = 1;
    p6.ipv6_ext.sipv6_1 = 1;
    p6.ipv6_base.sip = 1;
    p6.ipv6_ext.dipv6_3 = 2;
    p6.ipv6_ext.dipv6_2 = 2;
    p6.ipv6_ext.dipv6_1 = 2;
    p6.ipv6_base.dip = 2;
    p6.ipv6_base.src_port = 0x4001;
    p6.ipv6_base.dst_port = 0x4002;
    //p6.ipv6_base.tenant_id = hw_bd_id;
    p6.ipv6_base.profile_id = NFP_RTP_PROF_INDEX;
    record.flow = &p6;
    nfm_flow_ipv6_lossy_homewood(&p6, &record);
}

void nfm_flow_spoof_homewood ()
{
    nfm_hw_flow_rec_info_t record = {0};
    nfm_homewood_ipv4_full_t p;
    nfm_homewood_ipv6_full_t p6;
    tetr_hdr_t t = {0};
    memset(&p, 0, sizeof(nfm_homewood_ipv4_full_t));
    memset(&p6, 0, sizeof(nfm_homewood_ipv6_full_t));

    record.flow = &p;
    record.t = &t;

    // Get the first interface
    pt_get_next_attrtype attr;
    if_index_t ifindex_walk = 0;
    ushort mask_len = 0;
    uint32_t hw_bd_id = 0;
    char *ifname = "Ethernet1/34";
    if_index_t ifindex = 436224512;

    // if_ordinal iod = im_get_if_by_name(ifname);
    // im_get_ifindex_frm_iod(iod, &ifindex);
    //eltm_query_ifindex_to_lif(intf->ifindex, &hw_bd_id);

    // pt_get_next_attr_init(&attr);
    // nfm_intf_t *intf = nfm_pt_get_first_intf(&ifindex_walk, &mask_len, &attr);
    // //im_get_ifindex_name(v4_metrics.if_id, (char*) ifname, sizeof(ifname));
    // eltm_query_ifindex_to_lif(intf->ifindex, &hw_bd_id);

    p.ipv4_base.sip = 0x05050102;
    p.ipv4_base.dip = 0x06060102;
    p.ipv4_base.src_port = 0x3000;
    p.ipv4_base.dst_port = 0x3001;
    p.ipv4_base.tenant_id = ifindex;
    // p.ipv4_base.profile_id = NFP_RTP_PROF_INDEX;
    t.num_flows = 1;
    nfm_flow_ipv4_loss_free_homewood(&p, &record);

    // intf = nfm_pt_get_next_intf(&ifindex_walk, &mask_len, &attr);
    // eltm_query_ifindex_to_lif(intf->ifindex, &hw_bd_id);
    p.ipv4_base.sip = 0x13010102;
    p.ipv4_base.dip = 0x14010102;
    p.ipv4_base.src_port = 0x1001;
    p.ipv4_base.dst_port = 0x1002;
    //p.ipv4_base.tenant_id = hw_bd_id;
    nfm_flow_ipv4_lossy_homewood(&p, &record);
 
    // intf = nfm_pt_get_next_intf(&ifindex_walk, &mask_len, &attr);
    // eltm_query_ifindex_to_lif(intf->ifindex, &hw_bd_id);
    // p.ipv4_base.sip = 0x23010102;
    // p.ipv4_base.dip = 0x24010102;
    // p.ipv4_base.src_port = 0x2001;
    // p.ipv4_base.dst_port = 0x2002;
    // //p.ipv4_base.tenant_id = hw_bd_id;
    // nfm_flow_ipv4_ooo_homewood(&p, &record);

    // intf = nfm_pt_get_next_intf(&ifindex_walk, &mask_len, &attr);
    // eltm_query_ifindex_to_lif(intf->ifindex, &hw_bd_id);
    p6.ipv6_ext.sipv6_3 = 1;
    p6.ipv6_ext.sipv6_2 = 1;
    p6.ipv6_ext.sipv6_1 = 1;
    p6.ipv6_base.sip = 1;
    p6.ipv6_ext.dipv6_3 = 2;
    p6.ipv6_ext.dipv6_2 = 2;
    p6.ipv6_ext.dipv6_1 = 2;
    p6.ipv6_base.dip = 2;
    p6.ipv6_base.src_port = 0x4001;
    p6.ipv6_base.dst_port = 0x4002;
    //p6.ipv6_base.tenant_id = hw_bd_id;
    p6.ipv6_base.profile_id = NFP_RTP_PROF_INDEX;
    record.flow = &p6;
    nfm_flow_ipv6_lossy_homewood(&p6, &record);
}

#include "nfm_hea_hw_flow_format.h"

void nfm_flow_ipv4_loss_free_hea (nfm_hea_ipv4_full_t *p, 
        nfm_hw_flow_rec_info_t *record) {
    static uint16_t seqNum1 = 0;
    p->ipv4_base.pkt_cnt = 60000;
    p->ipv4_base.byte_cnt_0_to_31 = p->ipv4_base.pkt_cnt*128;
    seqNum1 = (seqNum1 + p->ipv4_base.pkt_cnt) % (65536);
    p->ipv4_analytics.analytics_latency_mode.udf.udf1 = seqNum1;
    buginf_wrap(DBG_NFM_ERROR, "%s: seqNum1:%u pkt_cnt:%u \n", __FUNCTION__, seqNum1, p->ipv4_base.pkt_cnt);
    nfm_netflow_ipv4_update_hash ((void*) record);

    usleep(50000);
    p->ipv4_base.pkt_cnt = 60000;
    p->ipv4_base.byte_cnt_0_to_31 = p->ipv4_base.pkt_cnt*128;
    seqNum1 = (seqNum1 + p->ipv4_base.pkt_cnt) % (65536);
    buginf_wrap(DBG_NFM_ERROR, "%s: seqNum1:%u pkt_cnt:%u \n", __FUNCTION__, seqNum1, p->ipv4_base.pkt_cnt);
    p->ipv4_analytics.analytics_latency_mode.udf.udf1 = seqNum1;
    nfm_netflow_ipv4_update_hash ((void*) record);

    usleep(50000);
    p->ipv4_base.pkt_cnt = 60000;
    p->ipv4_base.byte_cnt_0_to_31 = p->ipv4_base.pkt_cnt*128;
    seqNum1 = (seqNum1 + p->ipv4_base.pkt_cnt) % (65536);
    buginf_wrap(DBG_NFM_ERROR, "%s: seqNum1:%u pkt_cnt:%u \n", __FUNCTION__, seqNum1, p->ipv4_base.pkt_cnt);
    p->ipv4_analytics.analytics_latency_mode.udf.udf1 = seqNum1;
    nfm_netflow_ipv4_update_hash ((void*) record);
}

void nfm_flow_ipv4_lossy_hea (nfm_hea_ipv4_full_t *p, 
        nfm_hw_flow_rec_info_t *record) {
    static uint16_t seqNum = 0;
    p->ipv4_base.pkt_cnt = 20;
    p->ipv4_base.byte_cnt_0_to_31 = p->ipv4_base.pkt_cnt*128;
    seqNum = (seqNum + p->ipv4_base.pkt_cnt) % (65536);
    p->ipv4_analytics.analytics_latency_mode.udf.udf1 = seqNum;
    nfm_netflow_ipv4_update_hash ((void*) record);

    usleep(50000);
    p->ipv4_base.pkt_cnt = 70000;
    uint32_t pkt_loss = 3332;
    p->ipv4_base.byte_cnt_0_to_31 = p->ipv4_base.pkt_cnt*128;
    seqNum = (seqNum + p->ipv4_base.pkt_cnt) % (65536) + pkt_loss;
    p->ipv4_analytics.analytics_latency_mode.udf.udf1 = seqNum;
    nfm_netflow_ipv4_update_hash ((void*) record);

    usleep(50000);
    p->ipv4_base.pkt_cnt = 70000;
    pkt_loss = 0;
    p->ipv4_base.byte_cnt_0_to_31 = p->ipv4_base.pkt_cnt*128;
    seqNum = (seqNum + p->ipv4_base.pkt_cnt) % (65536) + pkt_loss;
    p->ipv4_analytics.analytics_latency_mode.udf.udf1 = seqNum;
    nfm_netflow_ipv4_update_hash ((void*) record);

    usleep(50000);
    p->ipv4_base.pkt_cnt = 70000;
    pkt_loss = 1;
    p->ipv4_base.byte_cnt_0_to_31 = p->ipv4_base.pkt_cnt*128;
    seqNum = (seqNum + p->ipv4_base.pkt_cnt) % (65536) + pkt_loss;
    p->ipv4_analytics.analytics_latency_mode.udf.udf1 = seqNum;
    nfm_netflow_ipv4_update_hash ((void*) record);
}

void nfm_flow_ipv4_ooo_hea (nfm_hea_ipv4_full_t *p, 
        nfm_hw_flow_rec_info_t *record)
{
    static uint16_t seqOOO = 15;
    p->ipv4_base.pkt_cnt = 20;
    p->ipv4_base.byte_cnt_0_to_31 = p->ipv4_base.pkt_cnt*128;
    seqOOO = (seqOOO + p->ipv4_base.pkt_cnt) % (65536);
    p->ipv4_analytics.analytics_latency_mode.udf.udf1 = seqOOO;
    nfm_netflow_ipv4_update_hash ((void*) record);

    usleep(50000);
    p->ipv4_base.pkt_cnt = 20;
    uint32_t pkt_loss = 2;
    uint32_t pkt_gain = 0;
    p->ipv4_base.byte_cnt_0_to_31 = p->ipv4_base.pkt_cnt*128;
    seqOOO = (seqOOO + p->ipv4_base.pkt_cnt) % (65536) + pkt_loss - pkt_gain;
    p->ipv4_analytics.analytics_latency_mode.udf.udf1 = seqOOO;
    nfm_netflow_ipv4_update_hash ((void*) record);

    usleep(50000);
    p->ipv4_base.pkt_cnt = 20;
    pkt_loss = 0;
    pkt_gain = 0;
    p->ipv4_base.byte_cnt_0_to_31 = p->ipv4_base.pkt_cnt*128;
    seqOOO = (seqOOO + p->ipv4_base.pkt_cnt) % (65536) + pkt_loss - pkt_gain;
    p->ipv4_analytics.analytics_latency_mode.udf.udf1 = seqOOO;
    nfm_netflow_ipv4_update_hash ((void*) record);

    usleep(50000);
    p->ipv4_base.pkt_cnt = 20;
    pkt_loss = 0;
    pkt_gain = 2;
    p->ipv4_base.byte_cnt_0_to_31 = p->ipv4_base.pkt_cnt*128;
    seqOOO = (seqOOO + p->ipv4_base.pkt_cnt) % (65536) + pkt_loss - pkt_gain;
    p->ipv4_analytics.analytics_latency_mode.udf.udf1 = seqOOO;
    nfm_netflow_ipv4_update_hash ((void*) record);
}

void nfm_flow_ipv6_lossy_hea (nfm_hea_ipv6_full_t *p, 
        nfm_hw_flow_rec_info_t *record)
{
    static uint16_t seqNum = 0;
    p->ipv6_base.pkt_cnt = 20;
    p->ipv6_base.byte_cnt_0_to_31 = p->ipv6_base.pkt_cnt*128;
    seqNum = (seqNum + p->ipv6_base.pkt_cnt) % (65536);
    p->ipv6_analytics.analytics_latency_mode.udf.udf1 = seqNum;
    nfm_netflow_ipv6_update_hash ((void*) record);

    usleep(50000);
    p->ipv6_base.pkt_cnt = 70000;
    uint32_t pkt_loss = 66000;
    p->ipv6_base.byte_cnt_0_to_31 = p->ipv6_base.pkt_cnt*128;
    seqNum = (seqNum + p->ipv6_base.pkt_cnt) % (65536) + pkt_loss;
    p->ipv6_analytics.analytics_latency_mode.udf.udf1 = seqNum;
    nfm_netflow_ipv6_update_hash ((void*) record);

    usleep(50000);
    p->ipv6_base.pkt_cnt = 70000;
    pkt_loss = 0;
    p->ipv6_base.byte_cnt_0_to_31 = p->ipv6_base.pkt_cnt*128;
    seqNum = (seqNum + p->ipv6_base.pkt_cnt) % (65536) + pkt_loss;
    p->ipv6_analytics.analytics_latency_mode.udf.udf1 = seqNum;
    nfm_netflow_ipv6_update_hash ((void*) record);

    usleep(50000);
    p->ipv6_base.pkt_cnt = 70000;
    pkt_loss = 1;
    p->ipv6_base.byte_cnt_0_to_31 = p->ipv6_base.pkt_cnt*128;
    seqNum = (seqNum + p->ipv6_base.pkt_cnt) % (65536) + pkt_loss;
    p->ipv6_analytics.analytics_latency_mode.udf.udf1 = seqNum;
    nfm_netflow_ipv6_update_hash ((void*) record);
}

void nfm_flow_rtp_spoof_hea ()
{
    nfm_hw_flow_rec_info_t record = {0};
    nfm_hea_ipv4_full_t p;
    nfm_hea_ipv6_full_t p6;
    tetr_hdr_t t = {0};
    memset(&p, 0, sizeof(nfm_hea_ipv4_full_t));
    memset(&p6, 0, sizeof(nfm_hea_ipv6_full_t));

    record.flow = &p;
    record.t = &t;

    // Get the first interface
    pt_get_next_attrtype attr;
    if_index_t ifindex_walk = 0;
    ushort mask_len = 0;
    uint32_t hw_bd_id = 0;
    // pt_get_next_attr_init(&attr);
    // nfm_intf_t *intf = nfm_pt_get_first_intf(&ifindex_walk, &mask_len, &attr);
    // //im_get_ifindex_name(v4_metrics.if_id, (char*) ifname, sizeof(ifname));
    // eltm_query_ifindex_to_lif(intf->ifindex, &hw_bd_id);

    p.ipv4_base.sip = 0x03010102;
    p.ipv4_base.dip = 0x04010102;
    p.ipv4_base.src_port = 0x1000;
    p.ipv4_base.dst_port = 0x1001;
    //p.ipv4_base.tenant_id = hw_bd_id;
    p.ipv4_base.profile_id = NFP_RTP_PROF_INDEX;
    t.num_flows = 1;
    nfm_flow_ipv4_loss_free_hea(&p, &record);

    // intf = nfm_pt_get_next_intf(&ifindex_walk, &mask_len, &attr);
    // eltm_query_ifindex_to_lif(intf->ifindex, &hw_bd_id);
    p.ipv4_base.sip = 0x13010102;
    p.ipv4_base.dip = 0x14010102;
    p.ipv4_base.src_port = 0x1001;
    p.ipv4_base.dst_port = 0x1002;
    //p.ipv4_base.tenant_id = hw_bd_id;
    nfm_flow_ipv4_lossy_hea(&p, &record);
 
    // intf = nfm_pt_get_next_intf(&ifindex_walk, &mask_len, &attr);
    // eltm_query_ifindex_to_lif(intf->ifindex, &hw_bd_id);
    p.ipv4_base.sip = 0x23010102;
    p.ipv4_base.dip = 0x24010102;
    p.ipv4_base.src_port = 0x2001;
    p.ipv4_base.dst_port = 0x2002;
    //p.ipv4_base.tenant_id = hw_bd_id;
    nfm_flow_ipv4_ooo_hea(&p, &record);

    // intf = nfm_pt_get_next_intf(&ifindex_walk, &mask_len, &attr);
    // eltm_query_ifindex_to_lif(intf->ifindex, &hw_bd_id);
    p6.ipv6_ext.sipv6_3 = 1;
    p6.ipv6_ext.sipv6_2 = 1;
    p6.ipv6_ext.sipv6_1 = 1;
    p6.ipv6_base.sip = 1;
    p6.ipv6_ext.dipv6_3 = 2;
    p6.ipv6_ext.dipv6_2 = 2;
    p6.ipv6_ext.dipv6_1 = 2;
    p6.ipv6_base.dip = 2;
    p6.ipv6_base.src_port = 0x4001;
    p6.ipv6_base.dst_port = 0x4002;
    //p6.ipv6_base.tenant_id = hw_bd_id;
    p6.ipv6_base.profile_id = NFP_RTP_PROF_INDEX;
    record.flow = &p6;
    nfm_flow_ipv6_lossy_hea(&p6, &record);
}

void nfm_flow_spoof_hea ()
{
    nfm_hw_flow_rec_info_t record = {0};
    nfm_hea_ipv4_full_t p;
    nfm_hea_ipv6_full_t p6;
    tetr_hdr_t t = {0};
    memset(&p, 0, sizeof(nfm_hea_ipv4_full_t));
    memset(&p6, 0, sizeof(nfm_hea_ipv6_full_t));

    record.flow = &p;
    record.t = &t;

    // Get the first interface
    pt_get_next_attrtype attr;
    if_index_t ifindex_walk = 0;
    ushort mask_len = 0;
    uint32_t hw_bd_id = 0;
    char *ifname = "Ethernet1/34";
    if_index_t ifindex = 436224512;

    // if_ordinal iod = im_get_if_by_name(ifname);
    // im_get_ifindex_frm_iod(iod, &ifindex);
    //eltm_query_ifindex_to_lif(intf->ifindex, &hw_bd_id);

    // pt_get_next_attr_init(&attr);
    // nfm_intf_t *intf = nfm_pt_get_first_intf(&ifindex_walk, &mask_len, &attr);
    // //im_get_ifindex_name(v4_metrics.if_id, (char*) ifname, sizeof(ifname));
    // eltm_query_ifindex_to_lif(intf->ifindex, &hw_bd_id);

    p.ipv4_base.sip = 0x05050102;
    p.ipv4_base.dip = 0x06060102;
    p.ipv4_base.src_port = 0x3000;
    p.ipv4_base.dst_port = 0x3001;
    p.ipv4_base.tenant_id = ifindex;
    // p.ipv4_base.profile_id = NFP_RTP_PROF_INDEX;
    t.num_flows = 1;
    nfm_flow_ipv4_loss_free_hea(&p, &record);

    // // intf = nfm_pt_get_next_intf(&ifindex_walk, &mask_len, &attr);
    // // eltm_query_ifindex_to_lif(intf->ifindex, &hw_bd_id);
    // p.ipv4_base.sip = 0x13010102;
    // p.ipv4_base.dip = 0x14010102;
    // p.ipv4_base.src_port = 0x1001;
    // p.ipv4_base.dst_port = 0x1002;
    // //p.ipv4_base.tenant_id = hw_bd_id;
    // nfm_flow_ipv4_lossy_hea(&p, &record);
 
    // // intf = nfm_pt_get_next_intf(&ifindex_walk, &mask_len, &attr);
    // // eltm_query_ifindex_to_lif(intf->ifindex, &hw_bd_id);
    // // p.ipv4_base.sip = 0x23010102;
    // // p.ipv4_base.dip = 0x24010102;
    // // p.ipv4_base.src_port = 0x2001;
    // // p.ipv4_base.dst_port = 0x2002;
    // // //p.ipv4_base.tenant_id = hw_bd_id;
    // // nfm_flow_ipv4_ooo_hea(&p, &record);

    // // intf = nfm_pt_get_next_intf(&ifindex_walk, &mask_len, &attr);
    // // eltm_query_ifindex_to_lif(intf->ifindex, &hw_bd_id);
    // p6.ipv6_ext.sipv6_3 = 1;
    // p6.ipv6_ext.sipv6_2 = 1;
    // p6.ipv6_ext.sipv6_1 = 1;
    // p6.ipv6_base.sip = 1;
    // p6.ipv6_ext.dipv6_3 = 2;
    // p6.ipv6_ext.dipv6_2 = 2;
    // p6.ipv6_ext.dipv6_1 = 2;
    // p6.ipv6_base.dip = 2;
    // p6.ipv6_base.src_port = 0x4001;
    // p6.ipv6_base.dst_port = 0x4002;
    // //p6.ipv6_base.tenant_id = hw_bd_id;
    // p6.ipv6_base.profile_id = NFP_RTP_PROF_INDEX;
    // record.flow = &p6;
    // nfm_flow_ipv6_lossy_hea(&p6, &record);
}

extern sw_card_id_t nfm_card_id;
void nfm_flow_rtp_spoof()
{
    if (SW_CARD_ID_IS_TOR_WITH_HEAVENLY_ASIC(nfm_card_id)) {
        nfm_flow_rtp_spoof_hea();
    }
    if (SW_CARD_ID_IS_TOR_WITH_HOMEWOOD_ASIC(nfm_card_id)) {
        nfm_flow_rtp_spoof_homewood();
    }
}
