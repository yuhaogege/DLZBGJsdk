/*
 * Copyright (c) CompanyNameMagicTag 2019-2019. All rights reserved.
 * Description: Supporting 1901 protocol ndm channel.
 */

#ifndef _NDM_CHL_P1901_H_
#define _NDM_CHL_P1901_H_

#include <soc_types.h>
#include <soc_mdm_timer.h>
#include <soc_mdm_msg.h>
#include <soc_mdm_mac.h>
#include <soc_mdm_ndm.h>
#include <soc_mdm_timer.h>
#include "mac_buffer.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DETAIL_STAT_STA_FREQ_MODE_NUM 6

/* Number of search frames transmitted on each frequency band */
#define PER_FREQ_SEND_FREQ_NUM 10

/* Number of frequency band search rounds. */
#define SEND_CYCLE_TIMES 2

/* Heartbeat message when the timer expires */
#define HEARTBEAT_TIMER_CB 1

/* Heartbeat messages received from the NMS */
#define RCV_HEARTBEAT_PKT_CB 2

/* Heartbeat packet request type */
#define NDC_HEARTBEAT_REQ_TYPE 0

/* Heartbeat packet response type */
#define NDC_HEARTBEAT_RSP_TYPE 1

/* No response or request is received in six consecutive channel heartbeat periods. */
#define NDC_NOT_RCV_MAX_HEARTBEAT_CNT 6

/* No packet exchange time (10s) */
#define NDC_NO_PKT_SEND_OR_RCV_TIME_MAX 120

/* Interval for sending heartbeat packets, in milliseconds. */
#define NDC_HEARTBEAT_PERIOD_MAX_VALUE 10000
/* High-priority duration, in seconds. */
#define NDC_CHL_PKT_HIGHEST_LID_TIME_MAX_LEN 100
#define NDC_CHL_PKT_HIGHEST_LID_TIME_MIN_LEN 10

#define NDM_RESEARCH_MAX_PERIOD  5000
#define NDM_RESEARCH_MAX_TIMES   5
#define NDM_MAX_DISCONNC_REQ_NUM 5
#define NDM_PHASE_BIT_SIZE       2

#define NDM_MAX_VALID_SEARCH_NET_RECORD_NUM 80
#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
#define MAC_NDC_ID_NDM_ID               0
#define MAC_NDC_ID_FC_MON_INFO          (MAC_NDC_ID_NDM_ID + 1)  /* Port used by the NDM to receive
                                                                 * messages from the FC. */
#define MAC_NDC_ID_BCN_MON_INFO         (MAC_NDC_ID_NDM_ID + 2)  /* Message used by the NDM to
                                                                  * receive information in beacons */
#define MAC_NDC_ID_FREQ_MON_TIMEOUT     (MAC_NDC_ID_NDM_ID + 3)  /* Used by the NDM to receive frequency
                                                                   * band listening timeout messages. */
#define MAC_NDC_ID_SHORT_SCAN_TIMEOUT   (MAC_NDC_ID_NDM_ID + 4)  /* Port used by the NDM to receive short-period
                                                                 * monitoring expiration messages. */
#define MAC_NDC_ID_MDL_SEARCH_END       (MAC_NDC_ID_NDM_ID + 5)  /* Port used by the NDM to receive the
                                                             * message indicating that the network search ends. */
#define MAC_NDC_ID_SEARCH_FINISH        (MAC_NDC_ID_NDM_ID + 6)  /* Port used by the NDM to receive network search
                                                                 * completion messages. */
#define MAC_NDC_ID_REPORT_TIMEOUT       (MAC_NDC_ID_NDM_ID + 7)  /* Used by the NDM to receive the callback message of
                                                     * the timer for reporting the network search result. */
#define MAC_NDC_ID_EXPIRE_CHECK_TIMEOUT (MAC_NDC_ID_NDM_ID + 8)  /* This interface is used by the NDM to
                                                        receive the callback message of the aging detection timer for
                                                                 * the reported network search result. */
#define MAC_NDC_ID_START_SEARCH_NET     (MAC_NDC_ID_NDM_ID + 9)  /* Used by the NDM to start network search. */
#define MAC_NDC_ID_SEND_REQ_TIMEOUT     (MAC_NDC_ID_NDM_ID + 10) /* Used by the NDM to send search frames. */
#define MAC_NDC_ID_WAIT_RSP_TIMEOUT     (MAC_NDC_ID_NDM_ID + 11)

#endif
/* NDC Channel Command Definition */
#define ndc_is_ndc_inner_cmd(id) (((id) >= 0x3800) && ((id) < 0x3900))

#define MAC_NDC_CMD_CONNECT_REQ 0x3800
#define MAC_NDC_CMD_CONNECT_CNF 0x3801

#define MAC_NDC_CMD_DEVID_REQ 0x3804 /* Concentrator ID request packet */
#define MAC_NDC_CMD_DEVID_RSP 0x3805 /* Concentrator ID response packet */

#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
/* Defines the network search specifications that can be recorded at each NDM frequency band. */
#define MDL_CCO_MAX    (MAX_SNID_PV100 + 1)
#define MDL_STA_MAX    16
#define MDL_RECORD_MAX MDL_STA_MAX /* The maximum value of MDL_CCO_MAX or MDL_STA_MAX must be selected. */

#define MAC_NDC_DISCONNECT 0 /* The NDC is disconnected. */
#define MAC_NDC_CONNECTED  1 /* The NDC is connected successfully. */
#endif
#define is_invalid_req_mode(req_mode) (((req_mode) == EXT_NDM_CONNECT_UNKNOWN_MODE) ||  \
                                       ((req_mode) >= EXT_NDM_CONNECT_INVALID_MODE))

#define NDM_SEARCH_FREQ_MAX_CNT       50
#define NDM_RCV_MIN_FC_CNT            5
#define NDM_PREVENT_INFINITE_LOOP_CNT 6

#define ENTIRETY_FREQ                 0x00
#define LOW_FREQ                      0x01

/* Defines the packet structure of the NDM channel. */
typedef struct {
    td_u16 heartbeat_cnt; /* Count the number of heartbeat frames that are not replied currently.
                                          * If the number exceeds the specified value, the connection is disconnected.
                                          */
    td_u16 no_pkt_time;   /* Time when no packet is exchanged (except heartbeat packets) */

    td_u32 heartbeat_period;
    ext_timer_handle_s heartbeat_timer;
} ndc_heartbeat_timer_ctrl;

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
/* Number of saved connection requests or response SNs. */
#define NDC_MAX_RCV_CONNECT_INFO_SN 2

typedef struct {
    td_u16 tei;
    td_u16 pkt_sn;
} ndc_rcv_req_info;
#endif

typedef struct {
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
    ndc_rcv_req_info ndm_info[NDC_MAX_RCV_CONNECT_INFO_SN];
    td_u16 new_rcv_tei;
#else
    td_u16 rsp_sn;
#endif
    td_u16 heartbeat_sn;
} ndc_connect_info;

typedef struct {
    td_u8 pre_mac[EXT_PLC_MAC_ADDR_LEN];
    td_u16 pre_tei;
} ndc_pre_connect_info;

typedef struct {
    td_u8 pkt_type : 1; /* Heartbeat request 0: REQ; Indicates the heartbeat response. 1: RSP */
    td_u8 pad1 : 7;
    td_u8 ndm_phase_cnt : 2; /* Number of phases for communication between the ndm and CCO */
    td_u8 ndm_phase0 : 2;    /* First-priority phase */
    td_u8 ndm_phase1 : 2;    /* Second-priority phase */
    td_u8 ndm_phase2 : 2;    /* Third-priority phase */
    td_u16 pkt_sn;           /* Packet SN */
} ndm_chl_heartbeat_st;

#define NEED_MORE_SAFE_TIME_10_SECONDS  10
typedef struct {
    td_u8 highest_lid_time_len;
    td_u8 is_use_highest_lid;
    td_u16 start_time_stamp;

    ext_timer_handle_s chl_highest_clock;
} ndm_chl_pkt_lid_info;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
/* Defines the basic information. */
typedef struct {
    td_u8 inuse; /* Whether an entry is in use */
    td_u8 pad;
    td_u8 connect_state; /* Indicates whether the connection is successful. */
    td_u8 product_type;  /* Product type: CCO, STA, relay, and II */

    td_u32 snid; /* Network ID */

    td_u32 rcv_fc_cnt; /* FC receiving times */
    td_u32 rcv_pb_cnt; /* PB Receive Times */

    td_u8 id[EXT_MDM_DEV_ID_LEN];    /* Indicates the device ID of the product. CCO indicates
                                     * the concentrator name, and other values indicate the meter number. */
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN]; /* MAC address of a site. */

    td_u16 tei;    /* Site TEI */
    td_u8 ndc_ver; /* NDC Version */
    td_s8 snr;     /* snr */

    td_u8 sta_type; /* STA products are used to distinguish the I-type meter and electric meter. */
    td_u8 ptcl_ver;
    td_u16 rsvd;

    td_u32 last_rcv_time; /* Record the time when the site is received. */
} mac_ndc_mdl_basic_info;

/* Defines the NDC channel control data structure. */
typedef struct {
    td_u8 cur_status;  /* Current NDC status: MAC_NDC_CH_WORK_STATUS_E */
    td_u8 start_freq;  /* Network Search Start Frequency Band */
    td_u8 search_mode; /* Record the network search mode. */
    td_u8 access_mode; /* Record the network access mode. */

    td_u32 cur_snid;                        /* Current network ID */
    /* MAC address of the CCO that can be accessed preferentially in CCO mode */
    td_u8 pri_cco_mac[EXT_PLC_MAC_ADDR_LEN];
    td_u8 connect_mac[EXT_PLC_MAC_ADDR_LEN]; /* MAC address of the connection */

    td_u8 total_freq_search_cnt; /* Record the number of times that all frequency bands are traversed. */
    td_u8 freq_change_cnt;       /* Record the number of frequency band handovers. */
    td_u8 freq_valid_num;        /* Record the number of valid frequency bands for automatic network search. */
    td_u8 freq_mode;             /* Record the frequency band setting mode,
                                    * which can be user-specified or automatic (frequency band number).  */

    td_u16 filter_tei; /* Records the TEI that needs to be filtered. */
    td_u8 is_in_reconnect;
    td_u8 research_cnt; /* Number of search retries. */

    td_u32 filter_snid; /* Record the SNIDs of the received packets to be filtered. */
    td_u32 rcv_fc_cnt;  /* Records the total number of FCs received after switching to the current frequency band. */
    td_u32 rcv_min_fc_cnt;
    td_u32 conn_time_stamp;   /* Time when the connection is refreshed for the last time. */
    td_u32 search_start_time; /* Network Search Start Time */
    td_u32 search_end_time;   /* Network Search End Time */
    td_u32 freq_change_time;  /* Time when the frequency band is changed */
    td_u8 send_req_num;
    td_u8 search_cycle_cnt;
    td_u8 start_send_req_freq;
    td_u8 req_mode;
} mac_ndc_status_ctrl_st;

typedef struct {
    td_u16 ndm_search_freq_max_cnt;
    td_u8 ndm_freq_scan_period_cco;
    td_u8 ndm_freq_scan_period_sta;

    td_u8 ndm_search_net_short_timeout;
    td_u8 ndm_search_result_check_timeout;
    td_u8 ndm_report_search_result_timeout;
    td_u8 ndm_wait_respond_timeout;

    td_u16 ndm_send_cnnc_req_period;
    td_u16 pad;

    ext_timer_handle_s ndm_search_net_long_timer;
    ext_timer_handle_s ndm_search_net_short_timer;
    ext_timer_handle_s ndm_search_result_expire_timer;
    ext_timer_handle_s ndm_report_result_timer;
    ext_timer_handle_s ndm_send_req_timer;
    ext_timer_handle_s ndm_wait_respond_timer;
} mac_ndc_data_info_detail_st;

td_void ndc_handle_msg_main(EXT_CONST EXT_IN ext_sys_queue_msg *msg);
td_void ndc_start_search_net(td_void);
td_void ndc_handle_rcv_bcn_info(td_u8 *bcn, td_u8 bcn_len);
td_void ndc_handle_rcv_fc_info(EXT_CONST ext_mdm_rcv_fc_info *rcv_fc_info);
mac_ndc_mdl_basic_info *ndc_find_record_entry(td_u8 freq, td_u32 snid, td_u8 *mac, td_u8 mac_len);
mac_ndc_mdl_basic_info *ndc_find_entry_to_add_new_info(td_u32 snid, td_u8 *mac,
                                                       td_u8 mac_len, td_s8 chl_info);
td_u32 ndc_chl_init(td_void);
td_u32 ndc_rcv_dbk_cmd(td_u16 cmd_id, td_pvoid cmd_param, td_u16 cmd_param_size, td_u8 option);
td_u32 ndc_handle_dbk_net_search_cmd(td_u32 cmd_id, td_pvoid cmd_param,
                                     td_u32 cmd_param_size, td_u32 option);
td_void ndc_handle_send_req_process(td_void);
td_u32 ndc_create_and_send_cnnc_req(td_u8 cnnc_mode, td_u8 is_need_ack, td_u8 *mac, td_u8 mac_len);
td_void ndc_handle_lost_connect(td_void);
__hot td_void ndc_send_req_cb(td_u32 data);
__hot td_void ndc_wait_respond_timeout_cb(td_u32 data);
td_void ndc_set_ndc_cur_status(td_u8 status);
td_u32 ndc_report_search_status(td_u8 search_status, td_u32 err_code);
td_void ndc_handle_active_cnnc(ext_mac_ndm_search_conn_req *cnnc_info);
__hot td_void ndc_handle_search_net_long_timer(td_u32 data);
__hot td_void ndc_handle_search_net_short_timer(td_u32 data);
td_u32 ndc_start_search(td_void);
__hot td_void ndc_handle_search_result_expire_timeout(td_u32 data);
td_void ndc_search_result_expire_check(td_void);
__hot td_void ndc_handle_report_result_timeout(td_u32 data);
td_void ndc_report_search_result(td_void);
mac_ndc_mdl_basic_info *ndc_find_record_entry_by_freq(td_u8 freq, td_u32 snid, td_u8 *mac, td_u8 mac_len);
mac_ndc_mdl_basic_info *ndc_find_record_entry_by_mac(td_u8 freq, td_u8 *mac, td_u8 mac_len);
td_void ndc_handle_rcv_bcn_info_msg(ext_sys_queue_msg *msg);
td_void ndc_handle_fc_info_msg(EXT_CONST ext_sys_queue_msg *msg);
td_u32 ndc_set_search_freq(td_u8 search_freq);
td_u8 ndc_get_next_search_freq(td_void);
td_void ndc_handle_short_timer_msg(td_void);
td_bool ndc_is_finish_freq_cycle_search(td_void);
td_void ndc_report_all_net_info(td_void);
td_u32 ndc_get_filter_snid(td_void);
td_u16 ndc_get_filter_tei(td_void);
td_bool ndc_is_rcv_bcn_info(td_u8 *bcn_info);
td_bool ndc_is_rcv_fc_info(td_u32 snid, td_u16 stei, td_u8 freq);
td_u8 ndc_get_freq_mode(td_void);
td_u8 ndc_get_access_mode(td_void);
td_u8 ndc_get_ndc_search_status(td_void);
td_u16 ndc_get_continue_search_time(td_void);
ext_mac_ndm_multinetwork_info_st *ndc_write_info_to_report_by_freq(ext_mac_ndm_multinetwork_info_st *dest,
                                                                  td_u8 freq, td_u32 snid, td_u8 access_mode,
                                                                  td_u8 max_record, EXT_INOUT td_u8 *precord_num);
td_u32 ndc_handle_active_search(ext_mac_ndm_search_conn_req *search_ctrl);
td_u32 ndc_set_serach_net_para(td_u8 freq_mode, td_u32 snid, td_u8 access_mode, td_u8 *mac, td_u8 mac_len);
td_void ndc_stop_search_net_timer(td_void);
td_void ndc_stop_connected_state_timer(td_void);
td_u8 *ndc_get_cco_mac(td_void);
td_bool ndc_is_ndc_in_net_search(td_void);
td_void ndc_handle_bcn_info_msg(EXT_CONST ext_sys_queue_msg *msg);
td_u8 ndc_get_valid_result_num(td_u8 access_mode, td_u8 freq_mode, td_u32 filter_snid);
td_void ndc_handle_long_timer_msg(td_void);
mac_ndc_data_info_detail_st *ndc_get_search_timer_ctrl_info(td_void);
td_void ndc_handle_respond_timeout(td_void);
td_void ndc_start_wait_respond_timer(td_void);
td_void ndc_reset_wait_respond_timer(td_void);
td_u32 ndc_handle_monitor_cmd(td_u32 mode_switch, td_u32 capture_switch);
td_u32 ndc_set_monitor_network_info(td_u8 freq_mode, td_u32 snid, td_u16 ptcl);
td_u32 ndc_stop_monitor_plc(td_void);
td_void ndc_enter_monitor_mode(td_void);
#endif
td_u32 ndc_get_new_rcv_req_ndm_tei(td_void);
td_void ndc_create_connect_rsp(td_u8 result, EXT_CONST td_u8 *oda, td_u8 len, td_u16 odtei);
td_void ndc_handle_chl_connect_req_msg(EXT_IN EXT_CONST ext_sys_queue_msg *msg);
td_void ndc_handle_chl_connect_rsp_msg(EXT_CONST EXT_IN ext_sys_queue_msg *msg);
__hot td_void ndc_heartbeat_cb(td_u32 data);
td_void ndc_set_cnnc_mode(td_u8 cnnc_mode);
td_void ndc_create_and_send_heartbeat_pkt(EXT_CONST td_u8 *mac, td_u8 mac_len);
td_u32 ndc_handle_heartbeat_pkt(mac_buffer_st *rcv_mac_buf);
td_u32 ndc_handle_heartbeat_msg(EXT_CONST ext_sys_queue_msg *msg);
ndc_heartbeat_timer_ctrl *ndc_get_heartbeat_ctrl_info(td_void);
td_void ndc_stop_heartbeat_timer(td_void);
td_void ndc_restart_heartbeat_timer(td_void);
td_void ndc_save_heartbeat_pkt_sn(td_u16 sn);
td_bool ndc_is_repeat_heartbeat_pkt(td_u16 sn);
td_void ndc_reset_no_chl_pkt_time(void);
td_bool ndc_is_chl_pkt_use_highest_lid(void);
td_void ndc_handle_chl_lid_clock_timeout(td_void);
td_void ndc_chl_set_highest_lid_time_len(td_u8 time);

#ifdef __cplusplus
}
#endif
#endif
