/*
 * Copyright (c) CompanyNameMagicTag 2012-2018. All rights reserved.
 * Description: DBK package ID and corresponding parameter structure definition of the app
 * Author: CompanyName
 * Create: 2012-07-16
 */

#ifndef __DFX_APP_H__
#define __DFX_APP_H__

#include <soc_ft_nv.h>
#include <app_nv.h>
#include <soc_mdm.h>
#include <dfx_sys.h>

/* Command ID Definition */
#define ID_DIAG_CMD_APP_BEGIN 0x5700
#define ID_DIAG_CMD_APP_END 0x58ff

#define ID_DIAG_CMD_CSIM_PACKET 0x5700 /* concentrator simu packet */
#define ID_DIAG_CMD_SIMU_CTRL 0x5710   /* concentrator simu ctrl */

#define ID_DIAG_CMD_UART_MSG_SEND_INF 0x5750
#define ID_DIAG_CMD_UART_MSG_PROC_INF 0x5751
#define ID_DIAG_CMD_PLC_MSG_SEND_INF 0x5752
#define ID_DIAG_CMD_PLC_MSG_PROC_INF 0x5753

#define ID_DIAG_CMD_WHITELIST_CLEAR 0x5800      /* clear white list */
#define ID_DIAG_CMD_WHITELIST_QUERY_NUM 0x5801  /* query white list number */
#define ID_DIAG_CMD_WHITELIST_QUERY_INFO 0x5802 /* query white list information */
#define ID_DIAG_CMD_WHITELIST_ADD 0x5803        /* add node to white list */
#define ID_DIAG_CMD_WHITELIST_DELETE 0x5804     /* delete node from white list */
#define ID_DIAG_CMD_FAST_SEND_DEMO 0x5806       /* fast_send_demo */
#define ID_DIAG_CMD_PLC_TEST 0x5807             /* Concurrent performance test */
#define ID_DIAG_CMD_PLC_TEST_INFO 0x5808        /* Querying Concurrent Performance Test Information */
#define ID_DIAG_CMD_PLC_TEST_STATUS 0x580e

#define ID_DIAG_CMD_TF_CTRL 0x5809         /* Transformer district identification and control */
#define ID_DIAG_CMD_TF_RESULT_QUERY 0x580a /* Transformer District Identification Result Query */
#define ID_DIAG_CMD_PWF_QUERY 0x580b       /* Querying Power Outage Results */
#define ID_DIAG_CMD_PHY_TEST_INFO 0x580c   /* Test mode information query */
#define ID_DIAG_CMD_NI_DFX_INFO 0x580d
typedef struct {
    td_u16 seq;
    td_u16 reserved;
} diag_cmd_whitelist_clear_req;

typedef struct {
    td_u32 ret_code;
    td_u16 seq;
    td_u16 reserved;
} diag_cmd_whitelist_clear_ind;

typedef struct {
    td_u16 seq;
    td_u16 reserved;
} diag_cmd_whitelist_query_num_req;

typedef struct {
    td_u16 seq;
    td_u16 total;
} diag_cmd_whitelist_query_num_ind;

typedef struct {
    td_u16 seq;
    td_u16 start;
    td_u16 num;
    td_u16 reserved;
} diag_cmd_whitelist_query_info_req;

typedef struct {
    td_u16 seq;
    td_u16 total;
    td_u16 start;
    td_u16 num;
    td_u8 data[60]; /* Diag command whitelist query size is of 60 bytes at maximum */
} diag_cmd_whitelist_query_info_ind;

typedef struct {
    td_u16 seq;
    td_u16 num;
    td_u8 data[60]; /* Diag command whitelist add request size is of 60 bytes at maximum */
} diag_cmd_whitelist_add_req;

typedef struct {
    td_u32 ret_code;
    td_u16 seq;
    td_u16 reserved;
} diag_cmd_whitelist_add_ind;

typedef struct {
    td_u16 seq;
    td_u16 num;
    td_u8 data[60]; /* Diag command whitelist delete request size is of 60 bytes at maximum */
} diag_cmd_whitelist_delete_req;

typedef struct {
    td_u32 ret_code;
    td_u16 seq;
    td_u16 reserved;
} diag_cmd_whitelist_delete_ind;

typedef struct {
    td_u8 set_enable;
    td_u8 set_plc_mode;
    td_u8 set_send_mode;
    td_u8 set_send_phase;

    td_u32 set_send_cnt;
    td_u32 set_send_period; /* Sending period (ms) */

    td_u8 oda[EXT_PLC_MAC_ADDR_LEN]; /* Indicates the destination MAC address. */
    td_u8 send_broadcast_flag;      /* Sets the broadcast flag. */
    td_u8 resend_cnt;
} diag_cmd_fast_send_demo_req;

typedef struct {
    td_u32 ret_code;
} diag_cmd_fast_send_demo_ind;

#define DIAG_CMD_PLC_TEST_IND_RET_START_SUCCESS 0
#define DIAG_CMD_PLC_TEST_IND_RET_INVALID_PARAMENT 1
#define DIAG_CMD_PLC_TEST_IND_RET_PREPARING 2
#define DIAG_CMD_PLC_TEST_IND_RET_STOP_SUCCESS 3
#define DIAG_CMD_PLC_TEST_IND_RET_BUSY 4
#define DIAG_CMD_PLC_TEST_IND_RET_INITIALIZING 5

#define DIAG_CMD_PLC_TEST_SEARCH_IND_RET_IDLE 0
#define DIAG_CMD_PLC_TEST_SEARCH_IND_RET_BUSY 1

/* The frame sequence number can be added to prevent two packets from being duplicated. */
typedef struct {
    td_u16 frame_length;
    td_u16 total_num; /* total number of sta */
    td_u16 total_time;
    td_u16 timeout;
    td_u64 dialog_start_time;
    td_u8 action; /* 0: stopped; 1: running */
    td_u8 is_last_frame;
    td_u8 parallel_num;
    td_u8 only_cco_count_time;
    td_u8 num; /* number of sta per batch */
    td_u8 send_interval;
    td_u8 refresh_interval;
    td_u8 test_mode;
    td_u8 mac[120]; /* Diag command packet buffer size is of 120 bytes at maximum */
    td_u8 pad[0];
} diag_cmd_plc_test_req, diag_cmd_plc_test_status_ind;

typedef struct {
    td_u32 ret_code; /* 0: success; EXT_ERR_BUSY: being tested */
} diag_cmd_plc_test_ind;

typedef struct {
    td_u32 pad;
} diag_cmd_plc_test_info_req;

typedef struct {
    td_u32 pad;
} diag_cmd_plc_test_status_req;

typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN];
    td_u8 ret[2]; /* Return code is of 2 bytes */
    td_u32 success_count;
    td_u32 total_count;
    td_u32 success_up_time;
    td_u32 success_down_time;
} diag_cmd_plc_test_info;

typedef struct {
    td_u32 num;
    td_u32 count_time;
    td_u8 statue;        /* 0: stopped; 1: running */
    td_u8 search_statue; /* 0: search is stopped; 1: search is running */
    td_u8 is_last_frame;
    td_u8 pad[1];
    diag_cmd_plc_test_info data[10]; /* 10 test info records */
} diag_cmd_plc_test_info_ind;

typedef struct {
    td_u32 rx_total_len; /* *< Total number of received bytes */
    td_u32 rx_last_time; /* *< Last Receiving Time */
    td_u32 rx_ok_len;    /* *< Number of correct bytes received (complete frame parsed) */
    td_u32 rx_err_len;   /* *< Number of received error bytes (number of bytes discarded
                          * when the complete frame fails to be parsed)
                          */
    td_u32 rx_ok_num;    /* *< Number of received complete frames */
    td_u32 rx_err_num;   /* *< Indicates the number of received error frames. */
    td_u16 rx_err_cause; /* *< Cause of the last error received */
    td_u16 rx_buf_tail;  /* *< End position of the buffer content */
    td_u8 rx_buf[64];    /* *< Last received content (-rx_buf_tail is the tail of the current coverage) 64 bytes */

    td_u32 tx_total_len; /* *< Total number of sent bytes */
    td_u32 tx_last_time; /* *< Last sending time */
    td_u32 tx_ok_len;    /* *< Number of bytes that are successfully sent */
    td_u32 tx_err_len;   /* *< Number of bytes that fail to be sent (statistics are collected
                          * when a failure message is returned after the DMS interface is invoked).
                          */
    td_u32 tx_total_num; /* *< Total number of sent frames */
    td_u32 tx_ok_num;    /* *< Number of frames that are successfully sent */
    td_u32 tx_err_num;   /* *< Number of frames that fail to be sent (statistics are collected after a failure
                          * message is returned after the DMS interface is called)
                          */
    td_u16 tx_err_cause; /* *< Reason for the last sending failure. */
    td_u16 tx_last_len;  /* *< Length of the data sent in the previous frame */
    td_u8 tx_buf[64];    /* *< Data sent in the previous frame (64 bytes) */
} uart_chl_inf;

typedef struct {
    td_u32 rx_total_num;                    /* *< Total number of received packets */
    td_u32 rx_time;                         /* *< Last received data flag */
    td_u16 rx_id;                           /* *< ID of the last received packet */
    td_u8 src_mac[EXT_PLC_MAC_ADDR_LEN];     /* *< Source MAC address received last time */
    td_u32 rx_len;                          /* *< Length of the data received last time */
    td_u8 rx_buf[64];                       /* *< Data received last time (partially) 64 bytes */
    td_u32 rx_err_num;                      /* *< Indicates the number of received error packets. */
    td_u32 rx_err_time;                     /* *< Last received error frame flag */
    td_u16 rx_err_id;                       /* *< ID of the last received error frame */
    td_u8 err_src_mac[EXT_PLC_MAC_ADDR_LEN]; /* *< Source MAC address of the last received error frame */
    td_u16 rx_err_cause;                    /* *< Cause of the error frame received last time */
    td_u16 rx_err_len;                      /* *< Length of the error frame received last time */
    td_u8 rx_err_buf[64]; /* *< Content (partial) of the error frame data received last time: 64 bytes */

    td_u32 tx_total_num;                     /* *< Total number of sent packets */
    td_u32 tx_time;                          /* *< Last sent data flag */
    td_u16 tx_id;                            /* *< ID sent last time */
    td_u8 dst_mac[EXT_PLC_MAC_ADDR_LEN];      /* *< Destination MAC address sent last time */
    td_u32 tx_len;                           /* *< Length of the data sent last time */
    td_u8 tx_buf[64];                        /* *< Content (part) of the data sent last time: 64 bytes */
    td_u32 tx_fail_num;                      /* *< Number of packets that fail to be sent */
    td_u32 tx_fail_time;                     /* *< Flag of the data that fails to be sent last time. */
    td_u8 fail_dst_mac[EXT_PLC_MAC_ADDR_LEN]; /* *< ID of the last sending failure. */
    td_u16 tx_err_cause;                     /* *< Error cause of the last sending failure. */
    td_u16 tx_fail_len;                      /* *< Length of the data that fails to be sent last time */
    td_u8 tx_fail_buf[64]; /* *< Content (partial) of the data that fails to be sent last time: 64 bytes */
} plc_chl_inf;

/* Transformer district identification control command type */
typedef enum {
    TF_CMD_TYPE_GET_STAT = 0,   /* Querying the Identification Status */
    TF_CMD_TYPE_START_IDENTIFY, /* Start transformer district identification. */
    TF_CMD_TYPE_STOP_IDENTIFY,  /* Stop transformer district identification */
} diag_tf_cmd_type;

/* Transformer district identification control command request parameter. */
typedef struct {
    td_u16 cmd_type; /* Transformer district identification control
                        command type. For the definition, see diag_tf_cmd_type. */
    td_u16 duration; /* Transformer district identification timeout interval, in minutes.
                      * This parameter is valid only when the start command is executed.
                      * The CCO takes a long time to identify the command after receiving the command.
                      */
} diag_cmd_tf_ctrl_req;

/* Transformer District Identification Switch Control Response Structure */
typedef struct {
    td_u32 ret; /* Indicates the command execution result.
                    The value 0 indicates that the command is executed successfully. */

    td_u16 tf_status; /* Transformer district identification status. For details, see diag_tf_cmd_status. */
    td_u16 pad;

    td_u32 last_duration; /* Remaining time before the identification times out, in seconds. */
} diag_cmd_tf_ctrl_ind;

/* transformer district recognition result query request */
typedef struct {
    td_u32 begin_idx; /* Sequence number of the result to be reported by the CCO. */
} diag_cmd_tf_result_req;

typedef enum {
    TF_RESULT_TYPE_NONE = 0,   /* Unknown recognition result. */
    TF_RESULT_TYPE_BELONG,     /* Identify the transformer district. */
    TF_RESULT_TYPE_NOT_BELONG, /* Identify the transformer district. */
} diag_sta_tf_result;

typedef struct {
    td_u8 sta_mac[EXT_PLC_MAC_ADDR_LEN]; /* MAC address of a site. */
    td_u16 tei;                         /* TEI of a site */
    td_u32 result;                      /* Transformer district recognition result.
                                            For details, see diag_sta_tf_result. */
} diag_cmd_sta_tf_result;

#define DIAG_TF_RESULT_REPORT_NUM 24

/* Response to the query of transformer district identification results */
typedef struct {
    td_u32 ret; /* Query result. If the value is 0, the operation is successful. */

    td_u16 total_num;        /* Total number of sites */
    td_u16 report_begin_idx; /* Report start sequence number. */
    td_u32 report_num;       /* Number of reported valid packets */
    diag_cmd_sta_tf_result results[DIAG_TF_RESULT_REPORT_NUM];
} diag_cmd_tf_result_ind;

/* Querying Power Outage Information */
typedef struct {
    td_u32 rcv_pwf_frame_time; /* *< Indicates the time when the power failure report frame is received. */

    td_u16 pwf_tei_cnt; /* *< Number of TEI in power outage */
    td_u8 pad[2];       /* pad(2) */
} diag_cmd_pwf_result_query;

#define TEST_MODE_NUM 8

typedef struct {
    td_u8 test_mode_flag;
    td_u8 pre_mode;
    td_u8 pad[2]; /* reserved 2 bytes */

    td_u32 enter_test_mode_time;
    td_u32 exit_test_mode_time;
} diag_cmd_phy_test_mode_inf;

/* Query test mode information. */
typedef struct {
    td_u8 mode;
    td_u8 payload_type;
    td_u8 invalid_port;
    td_u8 invalid_test_id;

    td_u32 count;
    td_u32 latest_report_time;
    td_u32 rcv_frame_err_cnt;
    td_u32 rcv_frame_err_time;
    td_u32 parse_err_app_cnt;
    td_u32 check_phy_tans_test_cnt;
    td_u32 check_msdu_test_cnt;

    diag_cmd_phy_test_mode_inf mode_info[TEST_MODE_NUM];
} diag_cmd_test_mode_info_ind;

/* APP name length */
#define EXT_APP_NAME_LEN 64
/* APP version length */
#define EXT_APP_VER_LEN 64
/* App information query */
typedef struct {
    td_char app_name[EXT_APP_NAME_LEN];
    td_char ver[EXT_APP_VER_LEN];
} ext_app_ver_cmd_ind_info;

typedef struct {
    td_u8 cur_state;
    td_u8 cur_send_cycle_cnt;
    td_u16 next_qry_sta_tei;

    td_u32 rcv_sta_rsp_cnt;

    td_u16 same_network_sta_cnt;
    td_u16 different_network_sta_cnt;

    td_u16 repeat_sta_rsp_cnt;
    td_u16 reserved;

    td_u16 add_sta_to_white_list_num;
    td_u16 wait_sta_rsp_timeout_cnt;

    td_u32 start_query_time;
    td_u32 start_add_whitelist_time;
    td_u32 set_whitelist_time;

    td_u32 send_qry_frame_success_cnt;
    td_u32 send_qry_frame_fail_cnt;
} diag_cmd_ni_dfx_info;
#endif /* __DFX_APP_H__ */
