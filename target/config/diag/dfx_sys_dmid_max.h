/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: dfx_sys dmid drv max.
 * Author: CompanyName
 * Create: 2021-1-27
 */

#ifndef __DFX_SYS_DMID_MAX_H__
#define __DFX_SYS_DMID_MAX_H__

#include "dfx_sys_dmid_max_def.h"
#include "soc_config.h"

/* phase recognition result request */
typedef struct {
    td_u16 tei;
    td_u8 phase; /* recognized phase */
    td_u8 state; /* phase recognition status */

    td_u8 timeout_cnt; /* identify timeouts */
    td_u8 fail_cnt;    /* identify the number of failures (received but not recognized) */
    td_u16 pad;

    td_u32 time_stamp; /* time, unit ms */

    td_u32 pad2[3]; /* pad 3 byte */
} ext_dmid_pi_zeroc_cross_ntb_req;

/* The number of NTBs answered by the phase recognition result */
#define EXT_DMID_PI_ZEROC_CROSS_NTB_RSP_NTB_NUM_MAX 19

/* phase recognition result response */
typedef struct {
    td_u16 tei;
    td_u8 index; /* number */
    td_u8 ntb_num;

    td_u8 phase_cnt[4]; /* identify the statistical value of the result at each phase 4 count */
    td_u32 time_stamp;  /* timestamp */

    td_u32 cco_cur_ntb; /* the latest ntb in the cco side cache */
    td_u8 phase_result;
    td_u8 pad[3]; /* pad 3 byte */

    td_u32 ntb[EXT_DMID_PI_ZEROC_CROSS_NTB_RSP_NTB_NUM_MAX]; /* Received zero crossing NTB */
} ext_dmid_pi_zeroc_cross_ntb_rsp;

typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN];
    td_u16 tei;

    td_u16 cco_sta_ntb_diff_cnt;
    td_u16 pad;

    td_u32 cco_sta_ntb_diff[18]; /* 18 count */
    td_u32 first_sta_ntb;
    td_u32 last_cco_ntb;
} ext_dmid_pi_cco_sta_ntb_diff;

#define EXT_DMID_ZC_AD_DATA_NUM 50
typedef struct {
    td_u16 current_index;
    td_u8 dir_state;
    td_u8 pad;
    td_u32 collect_data_num;

    td_u16 ad_data[EXT_DMID_ZC_AD_DATA_NUM];
} ext_dmid_zc_ad_data;

typedef struct {
    td_u16 tei;       /* tei */
    td_u8 up_ntb_cnt; /* sta matches the total number of cco rising edges */
    td_u8 period_num; /* number of full cycles */

    td_u8 right_period_num;  /* positive half axis is greater than negative half axis period */
    td_u8 up_ntb_index_last; /* the latest rising edge index */
    td_u8 pn_result;         /* phase recognition result */
    td_u8 pad;
} ext_dmid_pn_result;

typedef struct _rt_channel_stat_st3_ {
    td_u32 channel_info; /* channel quality */
    td_u32 stat_cnt;     /* receive number */
} rt_channel_stat_st3;

typedef struct _rt_proxy_comm_st3_ {
    td_u32 comm_rate;
    td_u32 channel_info;
    td_u32 down_comm_rate;
} rt_proxy_comm_st3;

/* Multi-path selection result report */
typedef struct {
    td_u32 bpc;
    td_u16 proxy_path;
    td_u16 up_up_level_path;
    td_u16 up_leval_path;
    td_u16 same_level_path;
} ext_dmid_rt_multi_path_result;

/* Reporting the results of the agency evaluation */
typedef struct {
    td_u32 bpc;
    td_u16 proxy_candidate[5]; /* 5 count */
    td_u16 proxy_cndd_cnt;
} ext_dmid_rt_evaluate_proxy_result;

/* There is a better proxy result report */
typedef struct {
    td_u32 bpc;
    td_u16 proxy_candidate[5]; /* 5 count */
    td_u16 prxoy_cndd_cnt;
} ext_dmid_rt_better_proxy_result;

/* Downstream path change report */
typedef struct {
    td_u16 odtei;
    td_u16 rsvd;
    td_u16 next_hop[4]; /* 4 count */
} ext_dmid_rt_down_path_change;

/* site offline message reporting */
typedef struct {
    td_u32 bpc;          /* beacon period when the site is offline */
    td_u16 proxy_tei;    /* parent agent tei when the site is offline */
    td_u16 channel_info; /* channel quality with the parent agent when the site is offline */
    td_u32 comm_rate;    /* communication rate with the parent agent when the site is offline */
} ext_dmid_rt_leave_network;

/* Receive FC content report */
typedef struct {
    td_u32 ntb;
    td_u8 freq;
    td_u8 aten;
    td_s8 snr;
    td_u8 pad;

    td_u32 fc[4]; /* 4 count */
} ext_dmid_mac_rcv_fc_ok;

typedef struct {
    td_u32 ntb;
} ext_dmid_mac_rcv_fc_err;

typedef struct {
    td_u16 ostei;
    td_u16 proxy_tei;
    td_u32 bpc;
    td_u16 send_discovery_pk_cnt;
    td_u16 rsvd;
    td_u32 to_proxy_comm_rate;
    td_u32 to_proxy_channel_info;
    td_u32 to_proxy_down_comm_rate;
    td_bool proxy_changeing;
    td_bool calc_proxy_rate_ok;
    td_u16 new_proxy_tei;
} ext_dmid_mac_rt_discover_list_head;

typedef struct {
    td_u16 ostei;
    td_u16 discover_tei;
    td_u16 last_down_stat_cnt_tmp;
    td_u8 comm_rate;
    td_u8 calc_proxy_rate_ok;
    td_u8 to_cco_near;
    td_u8 rsvd[3]; /* pad 3 byte */
} ext_dmid_mac_rt_discover_list_item;

#define DFX_PROXY_SUM 5
typedef struct {
    td_u32 bpc;
    td_u32 ntb;
    td_u32 msdu_size;
} ext_dmid_mac_rcv_msdu_from_app;

/* ************************************************ ***************************** */
/* Structure definition */
/* ************************************************ ***************************** */
/* ************************************************ ***************************** */
/* Network management message structure definition */
typedef struct {
    td_u16 tei;
    td_u16 proxy;
} ext_dmid_mac_sta_join_its_child;

typedef struct {
    td_u8 new_form_serial_number;
    td_u8 old_form_ferial_number;
    td_u16 rsvd;
} ext_dmid_mac_sta_join_form_num_change;

typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN];
    td_u8 send_success_ratio;
    td_u8 mac_rejoin_reason;
    td_u16 tx_resend_count; /* tx */
    td_u16 assoc_req_count;
    td_u16 rcv_beacon_count;
    td_u16 rcv_beacon_count_of_proxy[DFX_PROXY_SUM];
    td_u32 end_sequence;
} ext_dmid_mac_cco_rcv_sta_assoc_req_sta;

typedef struct {
    td_u16 old_proxy;
    td_u16 chose_proxy_tei[5]; /* 5 count */
} ext_dmid_mac_sta_chose_proxy_result;

typedef struct {
    td_u8 osa[EXT_PLC_MAC_ADDR_LEN];
    td_u8 oda[EXT_PLC_MAC_ADDR_LEN];

    td_u16 mmtype;
    td_u16 ostei;

    td_u16 odtei;
    td_u8 freq;
    td_u8 pad1;

    td_u16 ptcl_ver;
    td_u16 pad2;

    td_u32 snid;
    td_u32 bpc;
} ext_dmid_mac_create_network_module_msdu;

typedef struct {
    td_u8 osa[EXT_PLC_MAC_ADDR_LEN];
    td_u8 oda[EXT_PLC_MAC_ADDR_LEN];

    td_u16 mmtype;
    td_u16 ostei;

    td_u16 odtei;
    td_u16 ptcl_ver;

    td_u8 mac_head_ver;
    td_u8 freq;
    td_u16 pad;

    td_u32 snid;

    td_u32 bpc;
} ext_dmid_mac_rcv_network_module_msdu;

typedef struct {
    td_u8 my_level;
    td_u8 my_ability;
    td_u8 my_mac[EXT_PLC_MAC_ADDR_LEN];

    td_u8 my_snid;
    td_bool my_phase_mode;
    td_u8 my_phase;
    td_u8 form_serial_number;

    td_u16 my_tei;
    td_u16 my_proxy_tei;
} ext_dmid_mac_sta_join_ok;

typedef struct {
    td_u8 tdma_state; /* slot state: normal state, densely transmitted beacon, dense association */
    td_u8 bind_tdma_lid;
    td_u16 bind_tdma_len;
    td_u32 current_bpc; /* current bpc */
    td_u32 starttime;   /* beacon period start time */

    td_u32 beacon_period_len; /* beacon period length */
    td_u16 proxy_beacon_cnt;  /* number of proxy beacons */
    td_u16 all_beacon_cnt;    /* number of beacons */

    td_u32 phase_a_cp;  /* phase a csma slot length */
    td_u32 phase_b_cp;  /* phase b csma slot length */
    td_u32 phase_c_cp;  /* phase c csma slot length */
    td_u32 phase_a_bcp; /* phase a bound csma slot length */
    td_u32 phase_b_bcp; /* phase b binds csma slot length */
    td_u32 phase_c_bcp; /* phase c bound csma slot length */

    td_u32 cur_ntb;
} ext_dmid_mac_tdma_alloc_info;

/* set the working frequency band */
typedef struct {
    td_u16 reason; /* set the reason */
    td_u8 freq;    /* set the working frequency band */
    td_u8 pad;
} ext_dmid_mac_set_wrok_freq;

typedef struct {
    td_u32 snid;

    td_u8 freq;
    td_u8 pad;
    td_u16 ptcl_ver; /* protocol version number */
} ext_dmid_mac_set_snid;

typedef struct {
    td_u32 heartbeat_cycle_cnt;   /* heartbeat cycle count */
    td_u32 rcv_heartbeat_cnt;     /* receive heartbeat packets */
    td_u16 offline_sta_num;       /* number of offline sites */
    td_u16 false_offline_sta_num; /* number of fake offline sites */
} ext_dmid_mac_heartbeat_info;

typedef struct {
    td_u32 bpc;
    td_u32 relative_starttime; /* base time relative to time, in ntb */
    td_u32 starttime;          /* relative time unit: 1 microsecond */
    td_u32 duration;           /* duration unit: 1 microsecond */

    td_u16 tei;      /* tei corresponding to the time slot */
    td_u8 slot_type; /* slot type */
    td_u8 phase;     /* slot phase (csma slot exists) */

    td_u8 lid;         /* priority */
    td_u8 slot_source; /* slot source */
    td_u8 pad[2];      /* pad 2 byte */

    td_u32 cur_ntb;
} ext_dmid_mac_slot_info;

typedef struct {
    td_u8 level;
    td_u8 ability;
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN];

    td_u16 tei;
    td_u16 proxy_tei;

    td_u8 atten;
    td_u8 pad1[3]; /* pad 3 byte */

    ext_sys_status_info sta_ver;

    td_u32 cur_time; /* Current time, unit s */
} ext_dmid_mac_cco_some_sta_join_ok;

typedef struct {
    td_u8 fail_reason;
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN];
    td_u8 atten;

    td_u16 pad1;
    td_u8 pad[2]; /* pad 2 byte */

    ext_sys_status_info sta_ver;
} ext_dmid_mac_cco_some_sta_join_fail;

typedef struct {
    td_u16 old_proxy;
    td_u16 chose_proxy_tei[5]; /* 5 count */
} ext_dmid_mac_sta_start_change_proxy;

typedef struct {
    td_bool result;
    td_u8 rsvd[3]; /* pad 3 byte */
} ext_dmid_mac_sta_handle_change_proxy_cnf;

typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN];
    td_bool is_relay_ability;
    td_u8 product_type;

    td_u32 proxy_change_cnt; /* Proxy changes */
    td_u32 leave_cnt;        /* Offline times */
} ext_dmid_mac_cco_delete_sta_info;

typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN];
    td_u8 pad[2]; /* pad 2 byte */
} ext_dmid_mac_cco_report_violence_sta;

typedef struct {
    td_u32 access_nid;

    td_u8 access_freq;
    td_u8 rpt_state;
    td_u8 network_freq;
    td_u8 phy_freq;
} ext_dmid_mac_sta_report_access_network;

typedef struct {
    td_u32 bpc;

    td_u16 proxy_tei[5]; /* 5 count */
    td_u16 pad;
} ext_dmid_mac_sta_report_assoc_proxy;

typedef struct {
    td_u8 current_freq;
    td_u8 next_freq;
    td_u8 pad[2]; /* pad 2 byte */
} ext_dmid_mac_sta_report_freq_blind_check;

typedef struct {
    td_bool result;
    td_u8 reason;
    td_u16 new_proxy;

    td_u16 req_change_tei;
    td_u8 new_level; /* Agent changes the new level of the site */
    td_u8 pad;

    td_u16 old_proxy;
    td_u16 chose_proxy_tei[5]; /* 5 count */

    td_u32 end_sequence;
} ext_dmid_mac_new_change_proxy_dfx;

typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN];
    td_u16 rsvd;
} ext_dmid_mac_cco_heart_beat_check_warning;

typedef struct {
    td_u8 reason;
    td_u8 rsvd[3]; /* pad 3 byte */
} ext_dmid_mac_sta_leave_network;

typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN];
    td_u16 tei;
    td_u16 proxy_tei;
    td_u16 rsvd;
} ext_dmid_mac_cco_judge_sta_leave_network;

typedef struct {
    td_u8 max_level;
    td_u8 rsvd[3]; /* pad 3 byte */
    td_u16 child_sum;
    td_u16 pco_sum;
    td_u32 cur_bpc;
} ext_dmid_mac_cco_start_network_form;

typedef struct {
    td_u8 level;
    td_u8 rsvd;
    td_u16 child_sum;
    td_u32 cur_bpc;
} ext_dmid_mac_cco_start_some_level_join;

typedef struct {
    td_u8 state;
    td_bool is_timer;
    td_u8 pad1[2]; /* pad 2 byte */
} ext_dmid_mac_spc_identify_state;

typedef struct {
    td_u8 last_level;
    td_u8 rsvd;
    td_u16 last_level_join_child_sum;
    td_u32 cur_bpc;
} ext_dmid_mac_cco_last_level_join_count;

typedef struct {
    td_u8 max_level;
    td_u8 rsvd[3]; /* pad 3 byte */
    td_u16 pco_sum;
    td_u16 child_sum;
    td_u32 cur_bpc;
} ext_dmid_mac_cco_network_form_ok_info;

typedef struct {
    td_u8 conflict_cco_mac[EXT_PLC_MAC_ADDR_LEN];
    td_u16 neighbor_snid; /* SNID bitmap of the neighbor network */

    td_u8 sta_mac[EXT_PLC_MAC_ADDR_LEN];
    td_u16 sta_tei;

    td_u8 conflict_type;
    td_u8 pad;
    td_u16 ptcl_ver;
} ext_dmid_mac_network_conflict_report_info;

typedef struct {
    td_u16 ptcl_ver;
    td_u16 pad1;

    td_u32 free_snid_list[15]; /* 15count */
    td_u32 change_snid;
} ext_dmid_mac_network_conflict_choose_new_snid;

typedef struct {
    td_u32 snid[16]; /* 16 count */

    td_u8 sta_mac[EXT_PLC_MAC_ADDR_LEN];
    td_u16 ptcl_ver;

    td_u16 sta_tei;
    td_u16 pad1;
} ext_dmid_mac_query_neighbor_network_info;

typedef struct {
    td_u16 tei;
    td_u8 count;
    td_u8 fc_crc;

    td_u8 crc[4]; /* 4 count */

    td_u8 fc_mmib;
    td_u8 final_value;
    td_u8 rsvd[2]; /* pad 2 byte */

    td_u8 mmib[4]; /* 4 count */
} ext_dmid_mac_sta_mmib_info;

typedef struct {
    td_u16 tei;
    td_u16 preamble_sat_num;

    td_u8 attenuation;
    td_u8 final_value;
    td_u8 rsvd[2]; /* pad 2 byte */
} ext_dmid_mac_sta_attenuation_info;

typedef struct {
    td_u8 emc_inter_type;
    td_u8 pad[3];            /* pad 3 byte */
    td_u32 emc_nbi_table[8]; /* 8 count */
} ext_dmid_mac_nbi_table_report_info;

typedef struct {
    td_u8 emc_lab_scene;
    td_u8 emc_1vs1_scene;
    td_u8 emc_awgn_scene;
    td_u8 emc_freq_pow_scene;

    td_u8 emc_freq_pow_sum;
    td_u8 pad[3]; /* pad 3 byte */
} ext_dmimd_lab_info;

typedef struct {
    td_u8 nbi_power_mode; /* nbi power reporting mode */
    td_u8 max_power;      /* maximum power value */
    td_u8 min_power;      /* minimum power value */
    td_u8 diff;           /* difference */

    td_u8 nbi_power[32]; /* nbi power statistics table 32count */
} ext_dmid_mac_nbi_power_report_info;

#define EMC_POWER_CONTROL_PK_PERIODIC (1)           /* Periodic */
#define EMC_POWER_CONTROL_RESIEVE_SACK (2)          /* Receive channel information in the Sack frame */
#define EMC_POWER_CONTROL_RCV_FRAME_ATTENUATION (3) /* Channel attenuation value when receiving a frame */
typedef struct {
    td_u8 pk_type;
    td_u8 channel_attenuation;
    td_u16 emc_pream_sat_num;

    td_u8 preamle_fft_num;
    td_u8 emc_pk_pow_ctrl_switch;
    td_u8 emc_pow_ctrl_switch;
    td_u8 emc_pow_ctrl_start;
} ext_dmid_mac_power_control_packet_info;

typedef struct {
    td_u8 power_control_type;
    td_u8 attenuation;
    td_s8 emc_powindex_before_adj;
    td_u8 emc_afe_before_adj;

    td_s8 emc_powindex_after_adj;
    td_u8 emc_afe_after_adj;
    td_u8 pad[2]; /* pad 2 byte */
} ext_dmimd_mac_power_contorl_info;

typedef struct {
    td_u8 white_noise_value;
    td_u8 emc_awgn_conti_num;
    td_u8 emc_nonawgn_conti_num;
    td_u8 pad;
} ext_dmid_mac_white_noise_info;

typedef struct {
    td_u16 nbi_start;
    td_u16 nbi_end;

    td_u32 last_detect_time;
} ext_dmid_mac_freq_avoid_result_info;

typedef struct {
    td_u32 max_hest_value;
    td_u32 min_hest_value;

    td_u32 exce_time;
    td_u16 carrier_num;
    td_u8 hest_continue_detect_cnt;
    td_u8 pad;
} ext_dmid_mac_hest_info;

/* NTB module message structure definition */
typedef struct {
    td_u32 snid : 24;
    td_u32 form_serial_number : 8;

    td_u32 stei : 12;
    td_u32 ltmr_high : 8;
    td_u32 freq : 5;
    td_u32 bt : 2;
    td_u32 level : 4;
    td_u32 is_rcv_pb : 1;

    td_u8 cco_mac[EXT_PLC_MAC_ADDR_LEN];
    td_s8 snr;
    td_u8 pad1;

    td_u32 ltmr_low;
    td_u32 bts;
    td_u32 time_stamp;
} ext_dmid_mac_ntb_sync_param;

typedef struct {
    td_u32 tei : 12;
    td_u32 level : 4;
    td_u32 bt : 2;
    td_u32 avail_freqerr_pos : 4;
    td_u32 freq : 5;
    td_u32 sync_status : 3; /* The synchronization state of the synchronization object */
    td_u32 is_rcv_pb : 1;
    td_u32 pad1 : 1;

    td_u32 snid : 24;
    td_u32 sync_target_idx : 4;
    td_u32 freqerr_excpt_cnt_with_mac : 4;

    td_s8 snr;
    td_u8 freqerr_offset_high;
    td_u8 ltmrn_front_08;
    td_u8 freqerr_excpt_cnt_without_mac : 4;
    td_u8 pad2 : 4;

    td_u8 cco_mac[EXT_PLC_MAC_ADDR_LEN];
    td_s16 freqerr_output;

    td_u32 freqerr_offset_low;
    td_u32 cbtsn_front;
    td_u32 ltmrn_front_32;

    td_u32 last_rcv_timestamp;
    td_u32 ntb;
} ext_dmid_mac_ntb_sync_sta;

typedef struct {
    td_u16 tei : 12;
    td_u16 report_state : 4;
    td_s16 freq_err;

    td_u32 snid : 24;
    td_u32 freq : 8;
} ext_dmid_mac_ntb_neighbor_freqerr;

#endif
