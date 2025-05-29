/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: DFX interfaces of route
 */
#ifndef _DFX_MAC_RT_H_
#define _DFX_MAC_RT_H_

typedef struct {
    td_u16 tei;              /* Next-hop TEI address of the path */
    td_s16 snr;              /* Average path SNR */
    td_u8 channel_info;      /* Channel capacity value */
    td_u8 par;               /* Peak-to-average ratio */
    td_u8 up_comm_rate;      /* Uplink communication rate */
    td_u8 down_comm_rate;    /* Downlink communication rate */
    td_u8 send_success_rate; /* Actual data sending success rate */
    td_u8 rsvd[3];           /* Reserved 3 bytes */
} rt_path_node_comm_info_dfx;

typedef struct {
    rt_path_node_comm_info_dfx proxy_path;
    rt_path_node_comm_info_dfx up_up_level_path;
    rt_path_node_comm_info_dfx up_level_path;
    rt_path_node_comm_info_dfx same_level_path;
} rt_path_comm_info_dfx;

/* Simplified discovery list entity */
typedef struct {
    td_u32 tei : 12;
    td_u32 proxy_tei : 12; /* Parent agent TEI */
    td_u32 role : 4;       /* Site role. The value ranges from 0 to 15. */
    td_u32 level : 4;      /* Site level. The value ranges from 0 to 15. */
    /* Can be calculated */
    td_u32 comm_rate : 7;             /* Uplink/downlink communication rate,
                                       * comm_rate = down_comm_rate * up_comm_rate / 100,
                                       * calculated and displayed by the former
                                       */
    td_u32 down_comm_rate : 7;        /* downlink communication success rate */
    td_u32 up_comm_rate : 7;          /* uplink communication rate */
    td_u32 proxy_proxy_comm_rate : 7; /* Communication rate with agents */
    td_u32 level_relation : 2;        /* Hierarchy: 0-3 */
    td_u32 pad1 : 2;
    td_u32 send_fail_cnt;    /* Number of sending failures */
    td_u32 send_success_cnt; /* Number of successful sending times */
    td_u8 channel_info;      /* Channel quality (beacon frames exist) */
    td_u8 par;               /* Peak-to-average ratio (SOF frame) */
    td_s16 snr : 12;         /* Average SNR (SOF frame) */
    td_s16 pad2 : 4;
} diag_cmd_query_discover_list_entry_sm_stru;
typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN]; /* station MAC address */
    td_u16 pad;
    td_u32 tei : 12;
    td_u32 proxy_tei : 12; /* Parent agent TEI */
    td_u32 role : 4;       /* Site role. The value ranges from 0 to 15. */
    td_u32 level : 4;      /* Site level. The value ranges from 0 to 15. */
    /* Can be calculated */
    td_u32 comm_rate : 7;             /* Uplink/downlink communication rate,
                                       * comm_rate = down_comm_rate * up_comm_rate / 100,
                                       * calculated and displayed by the former
                                       */
    td_u32 down_comm_rate : 7;        /* downlink communication success rate */
    td_u32 up_comm_rate : 7;          /* uplink communication rate */
    td_u32 proxy_proxy_comm_rate : 7; /* Communication rate with agents */
    td_u32 level_relation : 2;        /* Hierarchy: 0-3 */
    td_u32 pad1 : 2;
    td_u8 channel_info; /* Channel quality (beacon frames exist) */
    td_u8 par;          /* Peak-to-average ratio (SOF frame) */
    td_s16 snr : 12;    /* Average SNR (SOF frame) */
    td_s16 pad2 : 4;
} diag_cmd_query_discover_list_entry;
/* Simplified entity for reporting the discovery list to the DBK */
typedef struct {
    td_u16 sn;         /* Sequence number reported in each round. For a query command,
                        * the sequence numbers of multiple corresponding reported data packets are the same.
                        * The value varies according to the reporting of different rounds of requests.
                        */
    td_u8 is_last : 1; /* Flag of the last packet reported in the current round: The value of the last packet is 1.
                        * The value of the non-last packet is 0.
                        */
    td_u8 pad : 7;
    /* Number of valid sites contained in the data packet */
    td_u8 num;
    diag_cmd_query_discover_list_entry_sm_stru entry[DFX_DISCOVER_LIST_ENTRY_SM_MAX]; /* discovers site information. */
} diag_cmd_query_discover_list_sm;
typedef struct {
    td_u16 sn;         /* Sequence number reported in each round. For a query command,
                        * the sequence numbers of multiple corresponding reported data packets are the same.
                        * The value varies according to the reporting of different rounds of requests.
                        */
    td_u8 is_last : 1; /* Flag of the last packet reported in the current round: The value of the last packet is 1.
                        * The value of the non-last packet is 0.
                        */
    td_u8 pad : 7;
    td_u8 num;
    diag_cmd_query_discover_list_entry entry[DFX_DISCOVER_LIST_ENTRY_MAX]; /* discovers site information. */
} diag_cmd_query_discover_list;
/* 2.8 Site routing table information */
typedef struct {
    td_u16 odtei;
    td_u16 next_hop0;
    td_u16 next_hop1;
    td_u16 next_hop2;
    td_u16 next_hop3;
    td_u16 rsvd;
} diag_cmd_query_router_table_entry_stru;
typedef struct {
    td_u16 sn;
    td_u16 is_last;
    td_u16 num;
    td_u16 pad;
    diag_cmd_query_router_table_entry_stru entry[DFX_ROUTER_TABLE_ENTRY_MAX];
} diag_cmd_query_router_table;

typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN];      /* station MAC address */
    td_u16 proxy_tei;                    /* Parent agent TEI */
    td_u32 role : 4;                     /* Site role */
    td_u32 level : 4;                    /* Site level */
    td_u32 min_multiproxy_comm_rate : 7; /* Route Min Communication Rate */
    td_u32 proxy_comm_rate : 7;          /* Uplink communication rate with the agent */
    td_u32 proxy_down_comm_rate : 7;     /* Downlink communication rate with the agent */
    td_u32 network_mode : 2;             /* Network mode: 0: PLC mode; 1: RF mode; 2: SC mode */
    td_u32 is_dual_module : 1;           /* Whether the module is a dual-mode module. 0: single-mode module;
                                          * 1: dual-mode module
                                          */
    td_u8 comm_rate;                     /* Uplink and downlink communication rate */
    td_u8 down_comm_rate;                /* Downlink communication success rate */
    td_u8 up_comm_rate;                  /* Uplink communication success rate */
    td_u8 proxy_proxy_comm_rate;         /* Path communication rate */
    /*
     * indicates the number of downlink received Beacon frames in the current period.
     * (Beacon frames may exceed 255 before networking. Therefore,
     * 16-bit Beacon frames are used.)
     */
    td_u16 cur_down_rcv_cnt;
    td_s16 snr;                    /* Average SNR (SOF frame) */
    td_u8 channel_info;            /* Channel quality (beacon frames exist) */
    td_u8 is_exist : 1;            /* Determine whether the site is discovered by the local site based
                                    * on the downlink route window statistics.
                                    */
    td_u8 is_multi_freq_avail : 1; /* Multi-band Band Enable */
    td_u8 multi_freq : 5;          /* Multi-band */
    td_u8 rsvd1 : 1;
    td_u8 proxy_channel_info;         /* Channel capacity of the proxy station */
    td_u8 down_rcv_flag : 1;          /* downlink receive flag, used for fast route change */
    td_u8 up_channel_update_flag : 1; /* Uplink communication rate update flag */
    td_u8 proxy_calc_ok : 1;          /* Whether to calculate the communication rate with the proxy site */
    td_u8 level_relation : 2;         /* Hierarchy */
    td_u8 phase : 2;                  /* Site phase */
    td_u8 calc_flag : 1;              /* Whether to calculate the uplink communication rate of the site */
    td_u8 comm_rate_low_cnt : 2;      /* Number of times that the uplink/downlink communication rate with the proxy is
                                       * continuously lower than the minimum threshold, Only the information about the
                                       * current proxy between the td_u8 comm_rate_low_cnt: 2; and the site is recorded.
                                       */
    td_u8 rsvd3 : 6;
    td_u8 rsvd2;
    td_u8 rssi; /* RF link signal strength */
    td_u8 pad;
    td_u8 down_rcv_cnt[4];              /* number of downlink received packets in the sliding window. The value is 4. */
    td_u8 up_rcv_cnt[4];                /* Number of Uplink Packets Received in Sliding Window, 4 sliding windows */
    td_u8 down_send_cnt[4];             /* Number of downlink sent sliding windows. 4: sliding window */
    td_u32 sum_cur_channel_quality;     /* Sum of the receive channel quality in the current period */
    td_u32 sum_cur_channel_attenuation; /* total receive channel attenuation in the current period */
    td_u8 down_comm_rate_wind[4];       /* saves the sliding window of downlink communication rate 4. */
    td_u8 up_comm_rate_wind[4];         /* save uplink communication rate 4 slide window */
    td_u16 cur_channel_attenuation_cnt; /* current channel attenuation record */
    td_u16 cur_rvc_need_send_pkt_cnt;   /* number of packets that must be transmitted in the current period. */
    td_u32 send_fail_cnt;               /* Number of sending failures */
    td_u32 send_success_cnt;            /* Number of successful sending times */
    td_u32 rcv_beacon_cnt;              /* Number of BEACON frames received from the site */
} diag_cmd_query_choose_dislist;

typedef struct {
    td_u16 tei;
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN]; /* station MAC address */
    td_u16 proxy_tei;               /* Parent agent TEI */
    td_u8 role;                     /* Site role */
    td_u8 level_relation;           /* Hierarchy */
    td_u8 proxy_down_comm_rate;     /* Downlink communication rate with the proxy */
    td_u8 proxy_comm_rate;          /* Uplink communication rate with the proxy */
    td_u8 proxy_proxy_comm_rate;    /* Path communication rate */
    td_u8 comm_rate_low_cnt;        /* Number of consecutive times that the communication
                                       rate with the agent is lower than 30. */
    td_u8 level;                    /* Site level */
    td_u8 phase;                    /* Update routing information. */
    td_u8 proxy_calc_ok;            /* Whether to calculate the communication rate with the proxy site. */
    td_u8 down_rcv_flag;
    td_u8 calc_ok_up; /* Whether to calculate the uplink communication rate of the site */
    td_u8 is_exist;
    td_u8 chl_atenuation;           /* Channel attenuation */
    td_u8 down_comm_rate;           /* Downlink communication success rate. Before network access,
                                     * the value is the number of messages that are successfully sent.
                                     */
    td_u8 up_comm_rate;             /* Uplink communication success rate and number of packets that
                                     * fail to be sent before network access
                                     */
    td_u8 min_multiproxy_comm_rate; /* path is the minimum communication rate. */
    td_u8 cur_down_rcv_cnt;         /* indicates the number of downlink received Beacon frames in the current period.
                                     * (Beacon frames may be greater than 255 before networking.
                                     * Avoid inverting the number of received Beacon frames.) */
    td_s8 snr;                      /* Average SNR */
    td_u8 down_rcv_cnt;             /* Number of downlink received packets in the sliding window */
    td_u8 proxy_channel_info;       /* Proxy channel quality (SNR) */
    td_u8 calc_ok_down;             /* Whether to calculate the downlink communication rate of the site */
    td_u8 pad1;
} ext_dmid_rt_path_table_st;

typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN];
    td_u16 tei;
} ext_dmid_mac_get_tei_from_path_table;

typedef struct {
    td_u16 next_hop;
    td_u16 odtei;
    td_u16 my_tei;
    td_u8 rsvd;
    td_bool result;
} ext_dmid_mac_get_next_hop;
typedef struct {
    td_u16 odtei;
    td_u16 next_hop;
    td_u16 my_tei;
    td_u8 rsvd;
    td_u8 result;
} ext_dmid_mac_add_route_item;
typedef struct {
    td_u16 tei;
    td_u16 tei_proxy;
    td_u16 my_tei;
    td_u8 rsvd;
    td_u8 result;
} ext_dmid_mac_add_route_item_by_proxy;
typedef struct {
    td_u16 ostei;
    td_u16 stei;
} ext_dmid_mac_route_learnig;
#endif