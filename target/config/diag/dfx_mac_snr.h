/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description:  DFX interfaces of snr
 */
#ifndef DFX_MAC_PART2_H
#define DFX_MAC_PART2_H
#include "soc_config.h"

#define DIAG_SNR_LEVEL_NUM 5 /* Supports the collection of data at five levels. */
#define DIAG_QUERY_NEIGHBOR_NUM 32
/* Query the intermediate calculation result REQ of a network. Only one parameter needs to be hit. */
typedef struct {
    td_u32 snid;
    td_u8 cco_mac[EXT_PLC_MAC_ADDR_LEN];
    td_u16 pad;
} diag_cmd_sta_snr_data_detail_req_stru;
/* Query the intermediate calculation result IND of a network. */
typedef struct {
    td_u32 ret;                         /* If 0 is returned, the operation is successful. */
    td_u32 last_reset_time;             /* Time of the last reset of the counter (unit: s) */
    td_u16 collect_duration;            /* Statistical duration, in seconds */
    td_u8 collect_enable;               /* Whether the statistics are being collected */
    td_u8 index;                        /* Network ID index. For details, see the neighboring network
                                         * information queried by macGetNearNetInfos.
                                         */
    td_u32 snid;                        /* Working SNID */
    td_u32 last_rcv_time;               /* Last time when the network beacon is received */
    td_u8 cco_mac[EXT_PLC_MAC_ADDR_LEN]; /* CCO MAC address */
    td_u16 pad2;
    td_s16 avg_snr[DIAG_SNR_LEVEL_NUM]; /* Average SNR of the best point at each layer (unit: 0.01 dB) */
    td_u16 avg_att[DIAG_SNR_LEVEL_NUM]; /* Average attenuation value of the best point at each layer (unit: 0.01 dB) */
} diag_cmd_sta_snr_data_detail_ind_stru;
typedef struct {
    td_u32 snid : 24;
    td_u32 net_idx : 8;
    td_u32 level : 4;
    td_s32 avg_snr : 14;
    td_u32 avg_att : 14;
    td_u32 fc_cnt;
} diag_snr_net_result_st;
/* Query the recognition result IND of each network in the current period. */
typedef struct {
    td_u32 ret;
    td_u32 last_reset_time;                                      /* Time of the last reset of the counter (unit: s) */
    td_u16 collect_duration;                                     /* Statistical duration, in seconds */
    td_u16 collect_enable : 1;                                   /* Whether the statistics are being collected */
    td_u16 list_node_num : 15;                                   /* Number of neighboring nodes */
    td_u32 snr_save_times : 24;                                  /* Number of history records */
    td_u32 net_num : 8;                                          /* Number of neighboring networks */
    diag_snr_net_result_st net_results[DIAG_QUERY_NEIGHBOR_NUM]; /* network identification
                                                                result in the current period */
} diag_cmd_sta_snr_data_simple_ind;
#define MAX_SNR_CHECK_STAT_NUM 24
#define MAX_SNR_CHECK_STAT_QUERY_NUM 12
#define MAX_SNR_BELONG_CHANGE_RECORD_NUM 5
#define SNR_SHORT_CYCLE_RECORD_NUM 4
#define SNR_LONG_CYCLE_RECORD_NUM 24
#define SNR_NET_BEST_NODE_NUM 5
typedef struct {
    td_u32 snid;
    td_u8 cco_mac[EXT_PLC_MAC_ADDR_LEN];
    td_u16 pad;
} diag_cmd_net_statistics_req;
typedef struct {
    td_u16 tei : 12;
    td_u16 level : 4;
    td_u16 fc_cnt;
    td_s16 avg_snr;
    td_u16 avg_att;
} diag_snr_node_collector_st;
typedef struct {
    td_u32 ret;
    td_u32 snid : 24;   /* Network ID */
    td_u32 net_idx : 8; /* Network index. For details, see the neighboring network information table. */
    diag_snr_node_collector_st node_info[SNR_NET_BEST_NODE_NUM];
} diag_cmd_net_best_node_ind;
typedef struct {
    td_u32 level : 4;
    td_s32 avg_snr : 14;
    td_u32 avg_att : 14;
    td_u32 fc_cnt;
} diag_snr_cycle_info_st;
typedef struct {
    td_u32 ret;
    td_u32 snid : 24;   /* Network ID */
    td_u32 net_idx : 8; /* Network index. For details, see the neighboring network information table. */
    td_u32 short_cycle_idx;
    diag_snr_cycle_info_st short_cycle_records[SNR_SHORT_CYCLE_RECORD_NUM];
} diag_cmd_net_short_cycle_statistics_ind;
typedef struct {
    td_u32 ret;
    td_u32 snid : 24;   /* Network ID */
    td_u32 net_idx : 8; /* Network index. For details, see the neighboring network information table. */
    td_u32 long_cycle_idx;
    diag_snr_cycle_info_st long_cycle_records[SNR_LONG_CYCLE_RECORD_NUM];
} diag_cmd_net_long_cycle_statistics_ind_stru;
/* Rapidly converge the record structure. */
typedef struct {
    td_u32 to_snid;    /* Switch to the network SNID. */
    td_u8 status;      /* Handover status */
    td_u8 my_level;    /* Local network */
    td_u8 to_level;    /* Switch to the SNR layer of the network. */
    td_u8 net_cnt;     /* Number of networks reaching the threshold */
    td_s16 my_avg_snr; /* average SNR of the current network */
    td_s16 to_avg_snr; /* Average SNR of Handovers to the Network */
} diag_fast_change_record_st;
/* Station fast convergence record IND */
typedef struct {
    td_u32 ret;              /* If 0 is returned, the operation is successful. */
    td_u8 fast_change_index; /* Fast convergence record subscript */
    td_u8 pad;
    td_u16 fast_change_success_times; /* Number of successful fast convergence record judgments */
    diag_fast_change_record_st records[MAX_SNR_CHECK_STAT_QUERY_NUM]; /* record */
} diag_cmd_sta_fast_change_stat_ind;
/* Query the fast convergence record REQ. */
typedef struct {
    td_u32 group_idx; /* Group subscript. A total of 24 records can be queried. A maximum of 12
                       * records can be queried each time. The records are divided into two groups.
                       */
} diag_cmd_sta_fast_change_stat_req;
/* size 28 bytes */
typedef struct {
    td_u8 change_reason;
    td_u8 old_net_idx;
    td_u8 new_net_idx;
    td_u8 pad;
    td_u32 old_net_snid;
    td_u32 new_net_snid;
    td_u8 old_net_cco[EXT_PLC_MAC_ADDR_LEN];
    td_u8 new_net_cco[EXT_PLC_MAC_ADDR_LEN];
    td_u32 change_time;
} diag_snr_belong_change_record;
/* Query the site homing change records. */
typedef struct {
    td_u32 ret;
    td_u8 record_index;
    td_u8 pad[3]; /* Reserved 3 bytes */
    diag_snr_belong_change_record records[MAX_SNR_BELONG_CHANGE_RECORD_NUM];
} diag_cmd_get_belong_change_reocrds_ind;
/* Neighbor network information 20B */
typedef struct {
    td_u32 nid : 24;                    /* Working NID */
    td_u32 index : 8;                   /* Subscript in the network statistics information */
    td_u8 cco_mac[EXT_PLC_MAC_ADDR_LEN]; /* CCO MAC address. All 0s and all Fs are invalid. */
    td_u16 phase_att[PHASE_CNT];        /* Attenuation of three-phase received CCO. Unit: 0.01 dB. */
    td_u32 last_rcv_time;               /* Last received time */
} diag_net_info_st;
/* Query the running status of the STA SNR module (IND). */
typedef struct {
    td_u8 identify_net_idx;           /* Index of the home network in the recognition result */
    td_u8 last_change_reason;         /* Reason why the recognition result is changed last time */
    td_u8 neighbor_block_malloc_fail; /* Number of times neighbor table memory blocks fail to be allocated */
    td_u8 belong_stable_times;        /* Number of stable cycles of the ownership information */
    diag_net_info_st join_net_info;   /* Add network information */
    diag_net_info_st belong_net_info; /* home network information */
    td_u8 begin_new_collect_cnt;      /* starts the count of new collection periods. */
    td_u8 neighbor_table_init_fail_cnt;
    td_u8 belong_change_record_idx;
    td_u8 save_history_fail;
    td_u8 read_nv_fail;
    td_u8 copy_nv_fail;
    td_u8 fast_change_index;
    td_u8 fast_change_success_times; /* Number of Successful Fast Convergence Handovers */
    td_s16 preamble_snr;             /* SNR value of the last triggered preamble filtering mechanism */
    td_u16 preamble_attenuation;     /* Attenuation of the last preamble filtering mechanism */
    td_u32 pream_sta_num_th_cnt;     /* Number of times that the preamble filtering mechanism is triggered */
} diag_cmd_sta_snr_ctrl_stat_ind;
/* Query the information about adjacent sites. REQ */
typedef struct {
    td_u32 snid;
    td_u32 tei;
} diag_cmd_near_sta_data_req;
/* Query the information about the adjacent sites. IND */
typedef struct {
    td_u32 ret;     /* 0 indicates success. */
    td_s16 avg_snr; /* Average snr */
    td_u16 avg_att;
    td_u32 level : 8;               /* Level */
    td_u32 fc_cnt : 24;             /* number of FCs received in the current period. */
    diag_net_info_st join_net_info; /* Add network information */
} diag_cmd_near_sta_data_ind;

typedef enum {
    DFX_LOCK_ROUTE_TYPE_WHOLE = 0x01,
    DFX_LOCK_ROUTE_TYPE_PART,
} dfx_ndm_lock_route_scope;


#define DIAG_QUERY_NET_INFO_NUM 16
/* Query the statistics of the neighboring network. REQ */
typedef struct {
    td_u32 group_id; /* A total of 32 records can be queried each time. A maximum of 16 records
                      * can be queried each time. The records are divided into two groups. The value is 0 or 1.
                      */
} diag_cmd_near_net_info_req;
/* Query the neighboring network statistics IND */
typedef struct {
    td_u32 ret;                                          /* If 0 is returned, the operation is successful. */
    diag_net_info_st net_infos[DIAG_QUERY_NET_INFO_NUM]; /* neighboring network information */
} diag_cmd_near_net_info_ind;
/* Set the home network of the site. REQ */
typedef struct {
    td_u8 cco_mac[EXT_PLC_MAC_ADDR_LEN]; /* Home CCO MAC address */
    td_u16 pad;
} diag_cmd_set_belong_network_req;
/* Set the home network IND of the site. */
typedef struct {
    td_u32 ret; /* The value 0 indicates that the command is executed successfully.
                 * Other values indicate that the command fails to be executed.
                 */
} diag_cmd_set_belong_network_ind;
/* Clear the home network IND of the site. */
typedef struct {
    td_u32 ret; /* The value 0 indicates that the command is executed successfully.
                 * Other values indicate that the command fails to be executed.
                 */
} diag_cmd_clear_belong_network_ind;
typedef struct {
    td_u32 ret;
} diag_cmd_snr_set_value_cnt_param_ind_st;
#define DIAG_QUERY_VALUE_DETAIL_NUM 120
typedef struct {
    td_u32 ret;
    td_u32 snr_total_cnt;
    td_s16 snr_avg;
    td_s16 ratio_snr_avg;
    td_u32 att_total_cnt;
    td_u16 att_avg;
    td_u16 ratio_att_avg;
    td_u16 detail_cnt[DIAG_QUERY_VALUE_DETAIL_NUM];
} diag_cmd_snr_query_value_cnt_info_ind_st;

typedef struct {
    td_u8 lock_reason;                  /* Locking reason */
    td_u8 lock_status;                  /* Lock status */
    td_u8 cco_mac[EXT_PLC_MAC_ADDR_LEN]; /* Lock the MAC address or master node address of the CCO on the network. */
    td_u32 lock_begin_time;             /* Lock start time */
    td_u32 lock_duration;               /* Lock duration */
    td_u8 unlock_reason;                /* Unlocking reason */
    td_u8 pad[3];                       /* Reserved 3 bytes */
    td_u32 unlock_time;                 /* Unlock time */
} diag_lock_record;
#define NM_LOCK_RECORD_NUM 8
typedef struct {
    td_u8 lock_status;                       /* Lock status */
    td_u8 lock_reason_index;                 /* Lock reason record subscript */
    td_u8 lock_cco_mac[EXT_PLC_MAC_ADDR_LEN]; /* locks the MAC address of the network. */
    td_u32 lock_begin_time;                  /* Lock start time, in seconds. */
    td_u32 lock_duration;                    /* Lock duration, in seconds. */
    td_u32 reject_time;                      /* Lock the network rejection time. */
    td_u32 last_leave_time;                  /* Time stamp from the time when the network is locked to
                                              * the time when the network is deregistered. The unit is second.
                                              */
    td_u32 leave_time_len;                   /* Offline duration after the network is locked, in seconds. */
    td_u16 lock_cnt;                         /* Number of lock times */
    td_u16 write_nv_cnt;                     /* Number of NV writes */
    td_u16 reject_cnt;                       /* Number of rejected locked networks */
    td_u16 lock_timeout_cnt;                 /* Lock timeout times */
    td_u16 lock_fail_by_not_join_cnt;        /* Failed to lock the network because the network is not added. */
    td_u16 send_msg_fail_cnt;                /* message sending failure (MT) */
    diag_lock_record lock_records[NM_LOCK_RECORD_NUM]; /* lock history */
    td_u8 belong_status;                               /* Home status */
    td_u8 belong_clear_reason;                         /* Clear the homing reason. */
    td_u8 belong_cco_mac[EXT_PLC_MAC_ADDR_LEN];         /* Home CCO address */
} diag_cmd_query_lock_info;

/* Lock network parameters. */
typedef struct {
    td_u8 cco_mac[EXT_PLC_MAC_ADDR_LEN]; /* If this parameter is valid, the network for transferring parameters
                                        is locked. If this parameter is invalid, the current network is locked.  */
    td_u16 pad;
    td_u32 duration; /* Lock duration, in seconds. */
} diag_cmd_lock_network_req;
/* Lock the network and reply */
typedef struct {
    td_u32 ret; /* If 0 is returned, the operation is successful. */
} diag_cmd_lock_network_ind;

#endif