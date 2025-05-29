/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: DFX interfaces of ndm
 */
#ifndef _DFX_MAC_NDM_H
#define _DFX_MAC_NDM_H

typedef enum _ndm_statics_context {
    NDM_SET = 0,
    NDM_NDM2NM,
    NDM_NM2CVG,
    NDM_CVG2SEG,
    NDM_SEG_IN_QUEUE,
    NDM_SEG_IN_QUEUE_FAIL,
    NDM_TX_ALL,
    NDM_TX_SUCCESS,
    NDM_TX_FAIL,
    NDM_TX_TIMEOUT,
    NDM_TX_RESEND,
    NDM_RX_RCV,
    NDM_RSB2CVG,
    NDM_RSB_TIMEOUT,
    NDM_RSB_FILTER,
    NDM_CVG2NM,
    NDM_NM_FILTER,
    NDM_NM2NDM
} ndm_statics_context_enum;

/* NDM-NDM mode: The DBK queries the NDM transformer district identification status and result. */
typedef enum {
    EXT_MAC_TF_IDENTIFY_CMD_NDM_TX_MODE_START = 0,
    EXT_MAC_TF_IDENTIFY_CMD_NDM_RX_MODE_START,
    EXT_MAC_TF_IDENTIFY_CMD_NDM_STOP,
} ext_mac_ndm2_ndm_tf_set_cmd_req;
typedef ext_mac_ndm2_ndm_tf_set_cmd_req ext_mac_ndm2ndm_tf_query_status_req_e;
typedef enum {
    EXT_MAC_TF_IDENTIFY_DOING,   /* identification in progress */
    EXT_MAC_TF_IDENTIFY_SUCCESS, /* identification success */
    EXT_MAC_TF_IDENTIFY_FAIL,    /* identification failed */
} ext_mac_ndm2_ndm_tf_identify_result_enum;
typedef struct {
    ext_mac_ndm2ndm_tf_query_status_req_e ndm_mode;
    td_u32 tf_ident_time;
    td_u8 status;
    td_u8 tf_result; /* Recognition result. 0: being recognized;
                        1: recognized successfully; 2: failed to be recognized */
    td_u8 serial_num;
    td_u8 phase;
    td_u8 level;         /* Receiver level, 1M, 240K */
    td_u8 stop_reason;   /* The transmitter stops abnormally. 0: normal; 1: power line harmonic stops abnormally. */
    td_u8 pad1[2];       /* Reserved 2 bytes */
    td_u16 wobble_scope; /* Mean value of the disturbance, in us */
    td_u16 noise_scope;  /* Real-time noise average value (unit: us) */
    td_u32 pad[3];       /* Reserved 3 bytes */
} diag_cmd_ndm2_ndm_tf_query_status_ind_stru;

#define NDM_FC_MAX_SIZE 16
#define NDM_PB_MAX_SIZE 520

typedef struct {
    td_u16 sn;
    td_u16 pkt_idx;
    td_u16 total_pkt_cnt;
    td_u16 pb_len;
    td_u16 total_pb_len;
    td_u16 pad;
    td_u32 time_stamp;
    td_u32 payload_type;
    td_u8 fc[NDM_FC_MAX_SIZE];
    td_u8 pb[0];
} diag_cmd_ndm_capture_report;


#define NDM_MODE_NORMAL 0
#define NDM_MODE_CAPTURE_DATA 1

typedef struct {
    td_u32 capture_switch; /* type switch of reporing MPDU message by bit */
    td_u32 mode_switch;    /* change mode */
    td_u32 pad;
} diag_cmd_ndm_capture_switch_req;

typedef struct {
    td_u32 ret;
} diag_cmd_ndm_capture_switch_ind;


typedef struct {
    td_u32 pad;
} diag_cmd_ndm_capture_stop_req;

typedef struct {
    td_u32 ret;
} diag_cmd_ndm_capture_stop_ind;

typedef struct {
    td_u8 freq;
    td_u8 pad1;
    td_u16 ptcl_ver;
    td_u32 snid;
} diag_cmd_set_ndm_capture_net_req;

typedef struct {
    td_u32 ret;
} diag_cmd_set_ndm_capture_net_ind;

/* Statistics of the data collected by the NMR controller based on the policy */
typedef struct {
    td_u8 follow_ccomac[EXT_PLC_MAC_ADDR_LEN]; /* Configures the MAC address to be traced. */
    td_u8 cur_freq;                           /* Current working frequency band */
    td_u8 snid;
    td_u8 ndm_cur_status;           /* Current status of the NMR controller: network searching and following */
    td_u8 follow_status;            /* Current status: following, following up, following failure */
    td_u8 set_follow_snid;          /* Delivered SN ID. */
    td_u8 set_follow_mode;          /* Issued follow-up mode */
    td_u8 set_follow_cmd_cnt;       /* Number of times the delivers commands */
    td_u8 start_follow_cnt;         /* Number of times that the delivered data collection start is followed */
    td_u8 stop_follow_cnt;          /* Number of times that the delivered stop data collection is followed */
    td_u8 start_follow_success_cnt; /* Number of successful data collection start follow-up times */
    td_u8 stop_follow_success_cnt;  /* Number of successful data collection stop times */
    td_u8 start_follow_fail_by_timer_fail_cnt;    /* Number of delivery start failures */
    td_u8 start_follow_fail_by_invalid_para_cnt;  /* Number of startup failures due to parameter errors */
    td_u8 start_follow_fail_by_cur_is_follow_cnt; /* indicates the number of times that the command is not executed
                                                   * when the command is delivered again because data collection
                                                   * is already performed.
                                                   */
    td_u8 switch_freq_in_full_mode_cnt;           /* Number of frequency band traversal times in one traversal */
    td_u8 notify_switch_freq_cnt;                 /* Number of frequency band switchover notifications
                                                   * on the entire network
                                                   */
    td_u8 continue_follow_fail_cnt;               /* Number of consecutive follow-up failures */
    td_u8 resume_cnt;                             /* Stop data collection and restore the number
                                                    of network search times. */
    td_u8 pad[2];                                 /* Reserved 2 bytes */
    td_u16 rcv_valid_follow_snid_cnt;             /* number of beacons with SNIDs received in the current period.
                                                   * This parameter applies to V100R001.
                                                   */
    td_u32 rcv_pb_cnt;                            /* Indicates the number of received pBs in follow-up mode. */
    td_u16 rcv_valid_cco_mac_cnt;                 /* number of CCO-configured cells received in the current period */
    td_u16 tei;                                   /* Obtain the TEI corresponding to an available idle timeslot. */
    td_u32 starttime;                 /* Start time of the available idle timeslots obtained by the NMR reader */
    td_u32 duration;                  /* Obtain the available idle timeslot length of the NMR controller.
                                       * The unit is microsecond.
                                       */
    td_u32 rcv_valid_slot_cnt;        /* Number of Times the BSC Parses Available Idle Timeslots */
    td_u32 follow_switch_freq_cnt;    /* Number of frequency band switchovers in mode */
    td_u32 follow_timer_callback_cnt; /* Number of times that the periodic detection timer is called back */
    td_u32 start_timer_fail_cnt;      /* Number of times that the timer fails to be started */
} diag_cmd_mac_ndm_follow_stat;
typedef struct {
    td_u8 ccomac[EXT_PLC_MAC_ADDR_LEN]; /* MAC address of the following CCO */
    td_u8 freq;                        /* freq from the network */
    td_u8 snid;
    td_u32 mode; /* Start the follow mode. The fixed follow mode is used,
                and the frequency band switching mode is used. */
    td_u32 cmd;  /* Command for starting or stopping the follow-up operation */
} diag_cmd_mac_ndm_set_follow_req;

typedef struct {
    td_u32 access_mode;
    td_u8 freq;
    td_u8 snid;
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN];
    td_u8 action;
    td_u8 reserved[3]; /* Reserved 3 bytes */
} diag_cmd_ndc_dbk_req;
typedef struct {
    td_bool set_ndm; /* td_true Set the NDM device. */
    td_u8 rsvd;
    td_u8 dev_id[6]; /* dev_id is of 6 bytes */
    diag_cmd_ndc_dbk_req dbk_request;
} diag_cmd_ndc_test;
typedef struct {
    td_u16 ndm_tei;      /* TEI of the NMR controller */
    td_u8 ndm_work_mode; /* Working mode */
    td_u8 pad;           /* Reserved */
} diag_cmd_ndm_state_info;

/* Request parameters for forcibly connecting the NCR */
typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN];
    td_u8 pad[2]; /* Reserved 2 bytes */
} diag_cmd_ndm_connect_req;
/* Interface for setting the attributes of the NMR controller */
typedef struct {
    td_u16 ndm_tei;
    td_u8 work_mode;
    td_u8 set_status;
} diag_cmd_ndm_attr_set;
/* Set the attributes of the transponder. */
typedef struct {
    td_u32 result; /* Set the result. */
} diag_cmd_ndm_attr_set_ack;
#endif