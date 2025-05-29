/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: DFX interfaces of components
 */
#ifndef _DFX_MAC_COMPONENT_H
#define _DFX_MAC_COMPONENT_H
/* CCO phase identification statistics */
typedef struct {
    td_u32 identify_start_success_cnt;       /* Number of successful phase identifications */
    td_u32 stop_identify_cnt;                /* Number of times for stopping phase identification */
    td_u16 identify_start_fail_by_doing_cnt; /* Number of identifications triggered during the identification process */
    td_u16 zc_collect_fail_rcv_rsp_cnt;
    td_u16 identify_start_fail_by_not_support_cnt; /* Number of startup failures due to unsupported identification */
    td_u16 rcv_zero_cross_ntb_rsp_err_cnt;         /* Indicates the number of received error packets. */
    td_u32 send_zero_cross_ntb_req_cnt;            /* Number of sent phase-crossing NTB query packets */
    td_u32 rcv_zero_cross_ntb_rsp_cnt;             /* Number of received phase-crossing NTB response packets */
    td_u32 parallel_query_timer_msg_cnt;           /* Number of times that the parallel query timeout
                                                      detection timer is called back */
    td_u32 parallel_query_timeout_cnt;             /* Parallel Query Timeout Times */
    td_u32 query_one_turn_over_cnt;                /* Number of times that a query ends. */
    td_u32 check_over_timer_msg_cnt;      /* Number of times that the timer for confirming phase identification ends */
    td_u32 ntb_diff_smaller_than_1ms_cnt; /* The zero-crossing NTB interval is less than 1 ms. */
    td_u16 discard_zero_cross_rsp_cnt;
    td_u16 get_phase_by_sta_ntb_rsp_err_cnt;   /* The STA's response packet is incorrect during phase calculation. */
    td_u32 phase_success_sta_cnt;              /* Number of successful phase identifications */
    td_u32 phase_fail_cnt;                     /* Number of phase identification failures */
    td_u32 phase_fail_sta_cnt;                 /* Number of phase identification failures */
    td_u16 rcv_zero_cross_ntb_rsp_timeout_cnt; /* The timeout NTB response packet is received. */
    td_u16 get_phase_by_sta_ntb_ref_err_cnt;   /* The NTB parameter on the CCO side is incorrect
                                                  during phase calculation. */
    td_u16 malloc_err_cnt;                     /* Count of dynamic memory application failures */
    td_u16 start_timer_err_cnt;                /* timer start failure count */
    td_u16 send_msg_err_cnt;                   /* Number of messages that fail to be sent */
    td_u16 msdu_to_tx_err_cnt;
    td_u8 pad1[20]; /* Reserved 20 bytes */
} diag_cmd_pi_cco_static;

/* STA phase identification statistics */
typedef struct {
    td_u16 rcv_zero_cross_ntb_req_cnt;  /* Number of received NTB query requests with zero values */
    td_u16 send_zero_cross_ntb_rsp_cnt; /* Number of Sent Zero-Query Responses */
    td_u16 phase_ntb_collect_reinit;    /* Number of times that the phase identification NTB is re-collected */
    td_u16 send_msg_err_cnt;            /* Number of messages that fail to be sent */
    td_u16 start_timer_err_cnt;         /* timer start failure count */
    td_u16 msdu_to_tx_err_cnt;          /* Indicates the number of transmitted msdu errors. */
} diag_cmd_pi_sta_static;


#define DIAG_CMD_PI_CCO_BASE_NTB_NUM_MAX 92 /* Maximum number of NTBs reported by the each time */
/* Phase identification: reporting the reference NTB at the CCO end */
typedef struct {
    td_u8 index; /* Packet number */
    td_u8 pad1;
    td_u16 next_ntb_store_index; /* Next buffer NTB index */
    td_u32 ntb_store_cnt;        /* Number of cached NTBs */
    td_u32 pad[3];               /* Reserved 3 words */
    td_u32 ntb[DIAG_CMD_PI_CCO_BASE_NTB_NUM_MAX];
} diag_cmd_pi_cco_base_ntb;
typedef struct {
    td_u32 start_zc_cnt;
    td_u32 start_zc_fail_cnt;
    td_u32 zc_init_cnt;
    td_u32 zc_positive_cnt;
    td_u32 zc_negative_cnt;
    td_u32 rising_edge_cnt;
    td_u32 failing_edge_cnt;
    td_u32 calc_zc_cnt;
    td_u32 self_ckeck_cnt;
    td_u16 self_check_fail_cnt;
    td_u16 add_job_fail_cnt;
    td_u32 fire_channel;
} diag_cmd_zc_stat;

typedef struct {
    td_u16 collect_num;
    td_u16 channel;
} diag_ad_collect_req;
typedef struct {
    td_u32 ad_data_addr;
} diag_ad_collect_ind;

typedef struct {
    td_u32 phase_total_rcv_cnt[3];      /* Total number of CCO BEACON frames received in phase 3 */
    td_u16 phase_rcv_cnt_last[3];       /* 3: total number of CCO BEACON frames received
                                         * in the previous period before the current phase.
                                         */
    td_u16 phase_rcv_cnt_current[3];    /* 3 Number of CCO BEACON frames received in the current period */
    td_u32 evaluate_phase_cnt[3];       /* Number of times that three phases are evaluated */
    td_u8 phase_channel_last[3];        /* Channel capacity value in the previous period before phase 3 */
    td_u8 phase_channel_current[3];     /* Channel capacity in the current period in phase 3 */
    td_s8 phase_snr[3];                 /* SNR in 3 phases */
    td_u8 phase[3];                     /* Sorting of the currently evaluated 3 phases */
    td_u8 phase_send_cnt[3];            /* Configure the number of 3-phase retransmissions dynamically. */
    td_u8 last_period_collect_finished; /* Whether the previous statistical period ends */
} diag_cmd_query_phase_statics;

#define EXT_MAC_PHASE_IDENTIFY_RESULT_ENTRY_NUM_MAX 46 /* Maximum number of sites reported by       \
                                                       * the phase identification result each time \
                                                       */
#define EXT_PHASE_INDEXTIFY_VER 3                      /* Version of the structure used for reporting the    \
                                                       * phase identification result (3: 4-byte; 0: 8-byte) \
                                                       */
/* Phase identification result of a single site */
typedef struct {
    td_u32 tei : 12;   /* TEI of the local site */
    td_u32 result : 4; /* Transformer district identification result:
                        * PHASE_ALL, PHASE_A, PHASE_B, PHASE_C, and INVALID_PHASE. For details, see soc_mdm.h .
                        */
    td_u32 state : 4;  /* Phase identification status: 0 (unidentified) 1: not supported;
                         2: being identified; 3: identification end */
    td_u32 pad1 : 12;
} diag_cmd_phase_identify_result_entry_stru;
/* Response structure of the phase identification result: */
typedef struct {
    td_u16 sn;        /* Sequence number of the reported identification result of each area. For a query command,
                       * the sequence numbers of the corresponding multiple reported data packets are the same.
                       * The value varies depending on the request reporting cycle.
                       */
    td_u16 entry_num; /* Number of valid sites contained in the data packet */
    td_bool is_last;  /* Flag of the last packet reported in the current round:
                       * The value of the last packet is 1. The value of the non-last packet is 0.
                       */
    td_u8 ver : 2;
    td_u8 pad1 : 6;
    td_u8 pad2[2]; /* Reserved 2 bytes */
    diag_cmd_phase_identify_result_entry_stru entry[EXT_MAC_PHASE_IDENTIFY_RESULT_ENTRY_NUM_MAX];
} diag_cmd_phase_identify_result;

/* The generation types of the transformer district recognition blacklist are as follows: */
typedef enum {
    DIAG_CMD_BLACKLIST_TYPE_USER = 0, /* blacklist imported by the user */
    DIAG_CMD_BLACKLIST_TYPE_AUTO      /* Automatically generated blacklist */
} diag_cmd_tf_identify_blacklist_type;
/* Error type of the transformer district identification blacklist */
typedef enum {
    EXT_MAC_BLACKLIST_ERR_UNKNOWN = 0,              /* The error cause is unknown. */
    EXT_MAC_BLACKLIST_ERR_BIG_NOISE,                /* large noise */
    EXT_MAC_BLACKLIST_ERR_CODE_CRASH,               /* code word collision */
    EXT_MAC_BLACKLIST_ERR_BIG_NOISE_AND_CODE_CRASH, /* Both large noise and code word collision. */
} ext_mac_tf_identify_blacklist_err_type;
/* Transformer district recognition blacklist */
typedef struct _diag_cmd_tf_identify_blacklist_entry_s_ {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN];
    td_u16 scope_avg;
    td_u8 type : 2;      /* Blacklist type. The value is DIAG_CMD_TF_IDENTIFY_BLACKLIST_TYPE_E */
    td_u8 rcv_level : 3; /* RX level */
    td_u8 pad1 : 3;
    td_u8 err_type; /* Error code type. The value is ext_mac_tf_identify_blacklist_err_type */
    td_u8 pad2[2];  /* Reserved 2 bytes */
} diag_cmd_tf_identify_blacklist_entry;

/* The structure for querying the transformer district identification blacklist is as follows: */
typedef struct _diag_cmd_blacklist_req_s_ {
    td_u16 sn;           /* Serial number of the result report. For a query command, the serial numbers
                          * of multiple corresponding report data packets are the same.
                          * The value varies according to the reporting of different rounds of requests.
                          */
    td_u16 pkt_idx;      /* Index of the current packet */
    td_u16 num_total;    /* Total number */
    td_u16 is_last : 1;  /* Flag of the last packet reported in the current round:
                          * The value of the last packet is 1. The value of the non-last packet is 0.
                          */
    td_u16 mac_num : 15; /* Number of valid MAC addresses contained in the data packet */
    td_u8 pad[4];        /* Reserved 4 bytes */
    diag_cmd_tf_identify_blacklist_entry entry[0];
} diag_cmd_blacklist_req;

typedef struct {
    td_u8 identify_state;
    td_u8 invalid_snid_cnt;
    td_u16 atten_filter_cnt;
    td_u16 rcv_notice_cnt;
    td_u8 notice_bcn_pend; /* Check whether the STA is forwarding the notification beacon. */
    td_u8 out_spc_atten;
    td_u16 snid_diff_cnt;
    td_u16 atten_big_diff_cnt;
    td_u16 sta_timer_set_rcv_seq_over_cnt;
    td_u16 sta_timer_set_state_init_cnt;
    td_u16 cco_timer_set_assoc_cnt;
    td_u16 cco_timer_set_init_cnt;
    td_u8 start_para_fail_cnt;
    td_u8 start_fail_cnt;
    td_u16 start_identify_cnt;
    td_u32 rcv_neighbor_gold_code_info;
    td_u16 rcv_neighbor_gold_code_cnt;
    td_u8 rcv_neighbor_atten;
    td_u8 malloc_err_cnt;
    td_u16 stop_identify_cnt;
    td_u16 app_stop_identify_cnt;
    td_u32 rcv_cnt;
    td_u16 rcv_fifo_read_err_cnt;
    td_u16 rcv_fifo_write_err_cnt;
    td_u16 send_notice_cnt;
    td_u16 send_end_cnt;
    td_u32 sync_success_cnt;
    td_u32 fast_power_down_code_success_cnt;
    td_u32 base_code_success_cnt; /* Number of successful basic code word detections */
    td_u32 base_code_false_cnt;   /* Number of basic code word false alarms */
    td_u32 gold_bit_lost_cnt;     /* gold bit loss count */
    td_u32 gold_code_fail_cnt;    /* gold code word loss count */
    td_u32 gold_code_success_cnt; /* Indicates the number of gold code word detection successes. */
    td_u32 gold_info_fail_cnt;    /* gold code word information loss count */
    td_u32 gold_info_success_cnt;
    td_u32 rcv_gold_code_info_cnt;
    td_u32 last_rcv_gold_code_info;
    td_u8 in_date_len_zero_cnt;
    td_u8 write_lock_info_err_cnt;
    td_u16 in_data_len_err_cnt;
    td_u8 read_lock_info_err_cnt;
    td_u8 base_code_time_max_err_cnt;  /* Number of times that the interval of basic codewords
                                        * is greater than the maximum value
                                        */
    td_u16 base_code_time_min_err_cnt; /* Number of times that the interval of basic codewords
                                        * is shorter than the minimum value
                                        */
    td_u32 base_code_time_max;
    td_u32 base_code_time_min;
    td_u16 retransmit_notice_beacn_cnt;         /* Number of notification beacons forwarded by online stations
                                                 * during SPCDMA network identification on the 3.2M_SGSM network
                                                 */
    td_u16 retransmit_notice_beacn_timeout_cnt; /* Number of notification beacon
    timeouts during online station forwarding in 3.2M_SGSM and SPCDMA network identification  */
} diag_cmd_spc_info;

typedef struct {
    td_u8 security_assoc_state; /* Secure access status security_assoc_state_en */
    /* Delayed NEK update flag. This flag corresponds to the new_nek space.
     * When this flag is TD_TRUE, new_nek is not TD_NULL. When this flag is TD_FALSE, new_nek is TD_NULL.
     */
    td_u8 delay_update_nek_flag;
    td_u8 negotiation_fail_reason; /* DAK negotiation failure cause */
    td_u8 pad;
    td_u32 client_hello_cnt;         /* Number of sent client_hello packets */
    td_u32 client_key_exchange_cnt;  /* Measurement point of Number of Sent client_key_exchange Messages */
    td_u32 get_nek_req_cnt;          /* send get_nek_req times */
    td_u32 parallel_notify_turn_cnt; /* CCO parallel notification of NEK round update */
    td_u16 next_notify_sta_index;    /* The CCO notifies the NEK of updating the STA index next time. */
    td_u8 pad1[2];                   /* Reserved 2 bytes */
    td_u32 last_set_nek_timestamp;   /* Timestamp of the last NEK setting (unit: s) */
    td_u32 update_nek_time;          /* Network-wide delay for NEK update */
    td_u32 negotiate_timeout_cnt;    /* Number of sites with DAK negotiation timeout */
    td_u32 psk_id_err_cnt;           /* Number of sites whose psk_id verification fails */
    td_u32 psk_value_err_cnt;        /* Number of sites whose psk verification fails */
    td_u32 security_reinit_cnt;      /* Number of times that the protocol security module is reinitialized */
    td_u32 peer_null_err_cnt;        /* Number of STA negotiation packets received when the context
                                      * on the CCO side has been released
                                      */
} diag_cmd_security_info;

typedef struct {
    td_u32 start_active_search_cnt;  /* active search times */
    td_u32 send_search_req_cnt;      /* Number of search frames transmitted by the ciu */
    td_u32 send_search_rsp_cnt;      /* Number of search response frames sent by sta */
    td_u32 send_p2p_pkt_cnt;         /* Number of created and sent P2P packets */
    td_u32 rcv_p2p_pkt_from_plc_cnt; /* Indicates the number of received P2P packets. */
    td_u32 rcv_search_rsp_cnt;       /* Number of search response frames received by the ciu */
    td_u32 rcv_search_req_cnt;       /* sta: number of searched frames. */
    td_u32 rcv_p2p_pkt_from_app;
    td_u32 save_app_p2p_pkt_success_cnt;
    td_u32 save_app_p2p_pkt_fail_cnt;
    td_u32 free_p2p_pkt_cnt;
    td_u32 pad;
} diag_cmd_query_ndm_p2p_statics;

typedef struct {
    td_u8 cur_work_freq;
    td_u8 start_search_freq;
    td_u8 state;
    td_u8 connect_mode;
    td_u32 snid;
    td_u8 no_mac_flag;
    td_u8 heartbeat_frame_flag;
    td_u8 my_mac[EXT_PLC_MAC_ADDR_LEN];
    td_u16 link_sta_tei;
    td_u8 link_sta_mac[EXT_PLC_MAC_ADDR_LEN];
} diag_cmd_query_nmd_ciu_detail_info;

typedef struct {
    td_u8 tl_id_cnt;                     /* Count of created handles */
    td_u8 join_send_list_fail_cnt;       /* Number of messages that fail to be added to the sending list. */
    td_u8 join_send_malloc_fail_cnt;     /* Number of messages that fail to be sent to allocate memory */
    td_u8 fragment_send_malloc_fail_cnt; /* segment memory allocation failure count */
    td_u16 curr_send_node_cnt;           /* Count of current sending node */
    td_u16 curr_rcv_node_cnt;            /* Count of current receiving node */
    td_u16 curr_rcv_fragment_cnt;        /* Count of currently received segments */
    td_u16 tl_assemble_malloc_fail_cnt;  /* Number of reassembly memory allocation failures */
    td_u16 tl_send_retry_cnt;            /* retransmission count */
    td_u16 tl_send_total_timeout_cnt;    /* Indicates the count of transmission timeouts. */
    td_u32 app_send_tl_cnt;              /* Number of packets sent by the APP through the transport layer */
    td_u32 tl_send_app_cnt;              /* Number of APP packets sent by the transport layer */
    td_u16 tl_send_fail_cnt;             /* Indicates the number of packets that fail to be sent. */
    td_u8 pad[2];                        /* Reserved 2 bytes */
    td_u32 rcv_app_to_tl_cnt;            /* Number of times that the application invokes
                                           the transport layer during reception */
    td_u32 tl_assemble_app_cnt;          /* Number of APP packets reassembled by the module */
    td_u32 tl_rsp_bitmap_ack_cnt;        /* Count of sending response bitmap. */
    td_u32 tl_rsp_delay_ack_cnt;         /* Indicates the count of delayed responses sent by the module. */
    td_u32 tl_rcv_bitmap_ack_cnt;        /* Number of received acknowledgment bits */
    td_u32 tl_rcv_delay_ack_cnt;         /* Number of delayed responses */
    td_u16 tl_rcv_ack_seq_err_cnt;       /* Indicates the count of received response sequence number errors. */
    td_u16 tl_rcv_id_err_cnt;            /* RX handle error count */
    td_u8 rcv_node_malloc_fail_cnt;      /* Number of memory allocation failures of the receiving node */
    td_u8 rcv_fragment_malloc_fail_cnt;  /* Number of memory allocation failures in receiving segments. */
    td_u16 rcv_repeat_fragment_cnt;      /* Number of duplicate segments received */
    td_u16 tl_cache_mem_size;            /* Current buffer memory at the transport layer */
    td_u16 tl_cache_mem_peek_size;       /* peak memory usage of the current buffer at the transport layer */
} diag_cmd_tl_info;

#define DFX_MESH_RT_ENTRY_NUM 15

typedef struct {
    td_u8 dmac[EXT_PLC_MAC_ADDR_LEN];
    td_u8 next_hop_mac[EXT_PLC_MAC_ADDR_LEN];

    td_s8 snr;
    td_u8 min_comm_level;
    td_u8 hop_cnt : 4;
    td_u8 useless_flag : 2;
    td_u8 direct_flag : 1;
    td_u8 rsvd : 1;
    td_u8 send_fail_cnt;

    td_u16 last_time;
    td_u16 invalid_sequence;

    td_u32 sta_sequence;
} diag_cmd_mesh_rt_entry_info;

typedef struct {
    td_u16 sn;
    td_u16 is_last;
    td_u16 num;
    td_u16 pad;
    diag_cmd_mesh_rt_entry_info mesh_rt[DFX_MESH_RT_ENTRY_NUM];
} diag_cmd_mesh_rt_info;

typedef struct {
    td_u8 query_mac[EXT_PLC_MAC_ADDR_LEN];
} diag_cmd_mesh_query_para;
#endif