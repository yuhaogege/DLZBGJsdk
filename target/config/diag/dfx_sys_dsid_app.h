/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: dfx_sys dmid drv max def.
 * Author: CompanyName
 * Create: 2021-1-27
 */

#ifndef __DFX_SYS_DSID_APP_H__
#define __DFX_SYS_DSID_APP_H__

#include "soc_config.h"
#include "def_sys_dsid_app_def.h"

typedef struct {
    td_u32 rx_total_num;                      /* total number of received packets */
    td_u32 rx_time;                           /* last time data received */
    td_u16 rx_id;                             /* last received id */
    td_u8 src_mac[EXT_PLC_MAC_ADDR_LEN];       /* the last received source mac */
    td_u32 rx_len;                            /* last received data length */
    td_u8 rx_buf[MAX_BUF_LEN];                /* last received data content (partial) */
    td_u32 rx_err_num;                        /* receive the number of error packets */
    td_u32 rx_err_time;                       /* last time received error frame data time stamp */
    td_u16 rx_err_id;                         /* the id of the last received error frame */
    td_u8 err_src_mac[EXT_PLC_MAC_ADDR_LEN];   /* the source mac that received the last error frame */
    td_u32 rx_err_cause;                      /* the reason for the last accepted error frame */
    td_u32 rx_err_len;                        /* last received error frame data length */
    td_u8 rx_err_buf[MAX_BUF_LEN];            /* last received error frame data content (partial) */
    td_u32 tx_total_num;                      /* total number of packets sent */
    td_u32 tx_time;                           /* the last time the data was sent */
    td_u16 tx_id;                             /* id sent last time */
    td_u8 dest_mac[EXT_PLC_MAC_ADDR_LEN];      /* the last destination sent by mac */
    td_u32 tx_len;                            /* the length of the last data sent */
    td_u8 tx_buf[MAX_BUF_LEN];                /* the last data content sent (partial) */
    td_u32 tx_fail_num;                       /* number of failed packets sent */
    td_u32 tx_fail_time;                      /* the last time the data was sent */
    td_u16 tx_fail_id;                        /* id sent last time */
    td_u8 fail_dest_mac[EXT_PLC_MAC_ADDR_LEN]; /* the last destination url sent */
    td_u32 tx_err_cause;                      /* the reason for the last accepted error frame */
    td_u32 tx_fail_len;                       /* the length of the last data sent */
    td_u8 tx_fail_buf[MAX_BUF_LEN];           /* the last data content sent (partial) */
} PLC_CHL_INF_S;                              /* PLC channel receiving statistics */

typedef struct {
    uart_chl_inf_s uart_msg_send_inf;
    uart_chl_inf_s uart_msg_proc_inf;
    PLC_CHL_INF_S plc_msg_send_inf;
    PLC_CHL_INF_S plc_msg_proc_inf;
} ext_dsid_app_chl_rx_tx_inf;

typedef struct {
    ext_obj64 obj_cnt;
    td_u32 heart_num;                  /* heartbeat */
    td_u32 last_time[MAX_LAST_NUM];    /* historical heartbeat time */
    td_u32 timeout_num;                /* heartbeat timeouts */
    td_u32 timeout_time[MAX_LAST_NUM]; /* historical heartbeat timeout */
} ext_dsid_app_heartbeat_inf;           /* heartbeat statistics */

typedef struct {
    ext_obj64 obj_cnt;
    td_u8 status[MAX_LAST_NUM];     /* cco status */
    td_u32 last_time[MAX_LAST_NUM]; /* history state change time scale */
    td_u32 map_ready_time;          /* map maintenance completion time stamp */
    td_u32 map_last_time;           /* last time the map maintenance completion time stamp */
} ext_dsid_app_cco_run_status;       /* cco running status */

typedef struct {
    td_u8 route;           /* cco routing status */
    td_u8 lr_status;       /* cco end rotation status */
    td_u16 meter_num;      /* number of table files */
    td_u32 lr_req_cnt;     /* round copy query number of readings */
    td_u32 lr_ack_cnt;     /* round copy response table count */
    td_u32 lr_plc_tx;      /* round copy plc send times */
    td_u32 lr_plc_rx;      /* round the number of plc receptions */
    td_u32 lr_meter_tx;    /* round the number of times the meter is sent */
    td_u32 lr_meter_rx;    /* the number of times the meter is received */
    td_u32 lr_plc_try_cnt; /* round copy plc resend times */
    td_u32 lr_plc_seq_err; /* round copy plc frame serial number expired number of discards */
    td_u32 lr_plc_len_err; /* round copy plc frame parsing length error count */
    /* rotate plc frame reception expiration times (cco side is not in receiving plc state) */
    td_u32 lr_plc_exceed_cnt;
    td_u32 lr_plc_timeout;     /* round copy plc timeouts */
    td_u32 lr_up_data_cnt;     /* report the number of data read */
    td_u32 lr_up_data_try_cnt; /* resend the number of times the data was read and read */
    /* number of successful rounds (reporting concentrator succeeded and receiving confirmation frame) */
    td_u32 lr_ok_cnt;
    td_u32 lr_fail_cnt; /* number of failed attempts (plc exceeded maximum number of retries) */
} app_lr_inf;

typedef struct {
    td_u32 xr_status; /* with cco status */
                      /*
                       * the number of times the serial port is received
                       * (cco is the receiving table frame; sta is the receiving meter frame)
                       */
    td_u32 xr_uart_rx;
    /* the number of times the serial port is sent (cco is the send read table frame; sta is the send meter frame) */
    td_u32 xr_uart_tx;
    td_u32 xr_plc_tx;           /* send the number of times with the plc */
    td_u32 xr_plc_rx;           /* received with plc */
    td_u32 xr_nak_num;          /* number of denials */
    td_u8 xr_nak[MAX_LAST_NUM]; /* recently max_last_num times denied reason */
    td_u32 xr_plc_try_cnt;      /* the number of resends with the plc */
    td_u32 xr_plc_seq_err;      /* with the copy of the plc frame serial number expired */
    td_u32 xr_plc_len_err;      /* error length of parsing plc frame */
    /* receive the number of expiration times with the plc frame (the cco side is not in the receiving plc state) */
    td_u32 xr_plc_exceed_cnt;
    td_u32 xr_plc_timeout; /* timeout with plc */
    td_u32 xr_ok_cnt;      /* number of successful copies */
    td_u32 xr_fail_cnt;    /* number of failed attempts */
} app_xr_inf;

typedef struct {
    /* concurrent serial port reception times (cco is the receive read table frame; sta is the receive meter frame) */
    td_u32 pr_uart_rx;
    /* number of concurrent serial port transmissions (cco is the send read table frame; sta is the send meter frame) */
    td_u32 pr_uart_tx;
    td_u32 pr_plc_tx;           /* concurrent plc send times */
    td_u32 pr_plc_rx;           /* concurrent plc reception times */
    td_u32 pr_nak_num;          /* concurrent denials */
    td_u8 pr_nak[MAX_LAST_NUM]; /* recently max_last_num times denied */
    td_u32 pr_plc_try_cnt;      /* concurrent plc resend times */
    td_u32 pr_plc_seq_err;      /* concurrent plc frame number expired and discarded */
    td_u32 pr_plc_len_err;      /* concurrent plc frame parsing length error count */
    td_u32 pr_plc_timeout;      /* concurrent plc timeouts */
    td_u32 pr_ok_cnt;           /* number of concurrent successes */
    td_u32 pr_fail_cnt;         /* number of concurrent failures */
} app_pr_inf;

typedef struct {
    td_u32 pool_id;                       /* cache pool id (memory address of the cache pool) */
    td_u32 pool_mode;                     /* cache pool working mode (mrs_rm_normal_mode : mrs_rm_broadcast_mode) */
    td_u32 parallel_num;                  /* current concurrency */
    td_u32 item_num;                      /* current number of table read requests */
    td_u32 join_fail_cnt;                 /* failed to join buffer pool */
    td_u32 join_fail_cause[MAX_LAST_NUM]; /* recently max_last_num times failed to join the buffer pool */
} app_rm_pool_inf;

typedef struct {
    td_u32 plc_seq;
    td_u32 timer_status;
    app_rm_pool_inf pool_inf[MAX_RM_POOL_NUM];
    app_lr_inf lr_inf; /* round copy (fixed copy) reading information */
    app_xr_inf xr_inf; /* with the table information */
    app_pr_inf pr_inf; /* concurrent reading table information */
} ext_dsid_app_rm_inf;  /* reading table related information statistics */

typedef struct {
    ext_obj64 obj_cnt;
    td_u32 change_num;              /* analog channel change times */
    td_u8 status[MAX_LAST_NUM];     /* analog channel status (with historical status) */
    td_u32 last_time[MAX_LAST_NUM]; /* analog channel state change time (history) */
} ext_dsid_app_vm_chl_inf;           /* analog channel related information */

typedef struct {
    ext_obj64 obj_cnt;
    td_u32 status;                    /* sta status */
    td_u32 state_time;                /* sta status change time stamp */
    td_u32 vm_chg_num;                /* virtual table changes */
    td_u8 vm_flg[MAX_LAST_NUM];       /* virtual table flag (with historical status) */
    td_u32 vm_time[MAX_LAST_NUM];     /* virtual table state change time (history) */
    td_u8 report_flg[MAX_LAST_NUM];   /* active reporting status (with historical status) */
    td_u32 report_time[MAX_LAST_NUM]; /* active reporting of status change time (history) */
    td_u8 change_flg[MAX_LAST_NUM];   /* change the status of the report (with historical status) */
    td_u32 change_time[MAX_LAST_NUM]; /* change the status of the report status change (history) */
} ext_dsid_app_sta_status;             /* sta status statistics */

typedef struct {
    td_u32 delay_20ms_cnt;    /* message processing delay exceeds 20ms */
    td_u32 delay_50ms_cnt;    /* message processing delay exceeds 50ms */
    td_u32 delay_100ms_cnt;   /* message processing delay exceeds 100ms */
    td_u32 delay_200ms_cnt;   /* message processing delay exceeds 200ms */
    td_u32 delay_300ms_cnt;   /* message processing delay exceeds 300ms */
    td_u32 delay_500ms_cnt;   /* message processing delay exceeds 500ms */
    td_u32 delay_1000ms_cnt;  /* message processing delay exceeds 1000ms */
    td_u32 delay_2000ms_cnt;  /* message processing delay exceeds 2000ms */
    td_u32 delay_5000ms_cnt;  /* message processing delay exceeds 5000ms */
    td_u32 delay_10000ms_cnt; /* message processing delay exceeds 10000ms */
    td_u32 delay_30000ms_cnt; /* message processing delay exceeds 30000ms */
    td_u32 other_cnt;         /* other times that do not exceed the processing delay threshold */
} app_msg_dly_level_cnt;

typedef struct {
    td_u32 curr_msg_num;                   /* current message queue depth */
    td_u32 max_msg_num;                    /* maximum queue depth */
    td_u32 max_mag_num_time;               /* maximum queue depth occurrence time */
    td_u32 total_tx_cnt;                   /* total number of messages sent */
    td_u32 tx_no_timer_cnt;                /* number of messages sent (without timer messages) */
    td_u32 total_rx_cnt;                   /* total number of messages received */
    td_u32 rx_no_timer_cnt;                /* number of messages received (without timer messages) */
    td_u32 tx_fail_cnt;                    /* number of failed message transmissions */
    td_u32 last_tx_id[MAX_LAST_NUM_2];     /* last message sent id */
    td_u32 last_rx_id[MAX_LAST_NUM_2];     /* id of the last message processing */
    td_u32 last_tx_time;                   /* last time the message was sent */
    td_u32 last_rx_time;                   /* last message processing time */
    td_u32 last_msg_delay[MAX_LAST_NUM_2]; /* historical message processing interval */
    td_u32 max_msg_delay;                  /* maximum message processing interval */
    td_u32 max_delay_id;                   /* id when the maximum message processing interval occurs */
    td_u32 max_delay_time;                 /* the maximum message processing interval time stamp occurs */
    td_u32 max_delay_queue_num;            /* queue depth at maximum message processing interval */
    td_u32 max_msg_proc;                   /* maximum message processing time */
    td_u32 max_msg_proc_id;                /* maximum message processing time id */
    td_u32 max_msg_proc_time;              /* maximum message processing time lapse */
    td_u32 last_fail_id[MAX_LAST_NUM];     /* history message failed to send id */
    td_u32 last_fail_time[MAX_LAST_NUM];   /* history message failed to send */
    app_msg_dly_level_cnt delay_cnt;       /* message processing delay statistics */
} ext_dsid_app_msg_inf;                     /* apply task message queue statistics */

typedef struct {
    ext_obj64 obj_cnt;
    td_u32 tx_ok_cnt;                  /* send the number of successes by broadcast */
    td_u32 last_tx_time[MAX_LAST_NUM]; /* historically used broadcast to send successful time stamps */
    td_u32 tx_fail_cnt;                /* number of failed transmissions using broadcast */
    td_u32 fail_tx_time[MAX_LAST_NUM]; /* historically used broadcast transmission failure time scale */
    td_u32 rx_cnt;                     /* total number of received broadcast frames */
    td_u32 last_rx_time[MAX_LAST_NUM]; /* historically received broadcast frame time stamp */
    td_u32 bc_rm_cnt;                  /* number of times to read the table using broadcast */
    td_u32 bc_rm_ok_cnt;               /* number of successful reads using the broadcast method */
    /* receive non-broadcast response times using the broadcast mode readlist */
    td_u32 bc_rm_non_bc_resp_cnt;
    td_u32 bc_rm_fail_cnt; /* number of failed table reads using broadcast (timeout) */
} ext_dsid_app_plc_bc_inf;  /* application layer plc broadcast frame transmission and reception statistics */

typedef struct {
    td_u8 curr_num;                       /* current queue depth */
    td_u8 max_num;                        /* maximum queue depth */
    td_u16 first_id;                      /* leader id */
    td_u32 lock_time;                     /* head lock time (ms) */
    td_u32 last_lock_time;                /* last time the team lock time (ms) */
    td_u32 max_lock_time;                 /* maximum head lock time (ms) */
    td_u32 queue_full_cnt;                /* queue full */
    td_u32 queue_full_time[MAX_LAST_NUM]; /* queue full time */
} ext_dsid_app_rm_queue_inf;               /* application layer reading table queue information statistics */

typedef struct {
    ext_obj64 obj_cnt;
    td_u8 afn;    /* afn of the rejected command */
    td_u8 fn;     /* fn of the rejected command */
    td_u8 type;   /* denying the reason */
} refuse_cmd_inf; /* reject command information */

typedef struct {
    ext_obj64 obj_cnt;
    td_u32 total_cnt;
    refuse_cmd_inf refust_cmd_list[10]; /* the last 10 times denied frame information */
} ext_dsid_app_refuse_cmd_inf;           /* reject command statistics (376.2 denied frames) */

typedef struct {
    ext_obj64 obj_cnt;
    td_u32 last_time[MAX_LAST_NUM];       /* history change moment */
    td_u8 chl_status[MAX_LAST_NUM];       /* history channel status */
    td_u32 change_num;                    /* channel change count */
    td_u32 first_time;                    /* the first successful network connection time */
    td_u32 chl_change_true;               /* true number */
    td_u32 chl_change_false;              /* false number */
    td_u32 last_true_time[MAX_LAST_NUM];  /* last channel changed to available time stamp */
    td_u32 last_false_time[MAX_LAST_NUM]; /* last channel changed to unavailable time stamp */
    td_u32 max_interval;                  /* maximum interval between changes */
    td_u32 min_interval;                  /* minimum interval between changes */
    td_u32 msg_tx_fail_num;               /* channel change message transmission failures */
    td_u32 msg_fail_time[MAX_LAST_NUM];   /* history message failed to send */
    td_u32 msg_fail_cause[MAX_LAST_NUM];  /* reason for failure to send history messages */
} ext_dsid_app_topo_change_inf;            /* topo change statistics */

typedef struct {
    ext_obj64 obj_cnt;
    td_u32 rx_msg_cnt;        /* total number of received messages */
    td_u32 mag_filter_cnt;    /* total number of message filtering */
    td_u32 bc_filter_cnt;     /* total number of broadcast message filtering */
    td_u32 normal_filter_cnt; /* total number of non-broadcast packet filtering */
    td_u16 id[20];            /* the last 20 filtered ids */
} ext_dsid_app_msg_filter_inf; /* packet filtering statistics */

typedef struct {
    td_u32 ver;
    td_u32 uart_tx_num;
    td_u32 uart_rx_num;
    td_u16 uart_tx_deny_num;
    td_u16 uart_rx_deny_num;
    td_u16 uart_time_out_num;
    td_u16 uart_rx_err_data_num;
    td_u32 plc_tx_num;
    td_u32 plc_rx_num;
    td_u16 plc_rx_exceed_num;
    td_u16 plc_tx_fail_num;
    td_u32 plc_time_out_num;
    td_u32 test_uart_tx_num;
    td_u32 test_uart_rx_num;
    td_u16 test_uart_tx_deny_num;
    td_u16 test_plc_tx_fail_num;
    td_u32 test_plc_tx_num;
    td_u32 test_plc_rx_num;
    td_u32 test_plc_time_out_num;
    td_u16 test_plc_rx_exceed_num;
    td_u16 test_plc_rx_bc_num;
} ext_dsid_app_rm_rx_tx_inf;

typedef struct {
    /* get clock status after power-on 0: not acquired; 1: completed; 2: getting acquired */
    td_u8 curr_get_time_status;
    td_u8 get_time_succ_flag;
    /* whether the clock is successful after power-on 1: successful, */
    /* combined with the status parameter uccurrgettimestatus */
    td_u8 reserved[2];    /* pad 2 byte */
    td_u8 first_clock[6]; /* 6 count */
    td_u8 last_clock[6];  /* 6 count */
    td_u16 get_time_send_cnt;
    td_u16 first_clock_fail_cnt;
} ext_dsid_app_cco_get_time_inf;

typedef struct {
    td_u8 curr_retry;
    td_u8 join_cco_queue_malloc_fail_cnt;
    td_u16 curr_item;
    td_u8 reserved[3]; /* pad 3 byte */
    td_u8 curr_afn;
    td_u16 curr_fn;
    td_u16 join_cco_queue_fail_cnt;
    td_u32 join_cco_queue_cnt;
    td_u32 cco_report_cnt;
    td_u32 cco_rx_ack_cnt;
    td_u32 cco_rx_deny_cnt;
    td_u32 cco_report_time_out_cnt;
} ext_dsid_app_cco_report_queue_inf;

typedef struct {
    td_u32 time;
    td_bool acd_flg;
    td_u8 join_cco_queue_fail_cnt;
    td_u8 join_cco_queue_malloc_fail_cnt;
    td_u8 join_evt_rep_list_malloc_fail_cnt;
    td_u8 reserved;
    td_u8 evt_list_item_num;   /* number of entries in the event list reported by cco */
    td_u16 evt_queue_item_num; /* number of cco queue entries */
    td_u16 cco_rx_compatibility_err_cnt;
    td_u16 cco_report_timeout_cnt;          /* total timeout count (without retransmission) */
    td_u32 cco_report_evt_cnt;              /* report event frame statistics (without retransmission) */
    td_u32 cco_evt_join_queue_time_sec[10]; /* units are seconds 10count */
    td_u32 cco_rx_ack_cnt;
    td_u32 cco_rx_evt_plc_cnt;
    td_u32 cco_rx_evt_plc_time_sec[10]; /* units are seconds 10count */
    td_u32 cco_tx_ack_plc_cnt;
    td_u32 cco_filter_plc_cnt;
    td_u32 cco_evt_rx_proto_err_cnt;
    td_u32 cco_evt_rx_report_disable_cnt;
    td_u32 cco_tx_forbid_plc_cnt;
    td_u32 cco_tx_full_plc_cnt;
    td_u32 cco_tx_allow_plc_cnt;
    td_u32 cco_forbid_rx_evt_cnt;
    td_u32 cco_filter_reported_evt_cnt;
    td_u32 cco_evt_replace_reported_head_cnt;
    td_u32 evt_reported_replace_same_addr_cnt;
    td_u32 evt_reported_aged_cnt;
    td_u32 evt_queue_front;
    td_u32 evt_queue_rear;
} ext_dfx_app_cco_evt_inf;

typedef struct {
    ext_dfx_app_cco_evt_inf dfx_cco_evt_inf[2]; /* pad 2 byte */
} ext_dsid_app_cco_evt_inf;

typedef ext_dsid_app_cco_evt_inf ext_dsid_app_cco_evt_bak_inf_s;

typedef struct {
    td_u32 total_size;
    td_u32 last_inc_size;
    td_u16 inc_malloc_err_count;
    td_u16 inc_malloc_count;
} ext_dfx_app_cco_memory_inc_inf;

typedef struct {
    ext_dfx_app_cco_memory_inc_inf dfx_cco_mem_inc_inf[7]; /* 7count */
} ext_dsid_app_cco_memory_inc_inf;

typedef struct {
    td_u32 mac_report_ts;
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN];
    td_u8 state_change;
    td_u8 offline_reason;
    td_u32 offline_duration;
} ext_dfx_mac_report_entry;

typedef struct {
    td_u8 enable;
    td_u8 detect_status;   /* 0-idle 1-white list added unread table 2-probe meter reading */
    td_u8 join_white_flag; /* 0 - not allowed to join the whitelist 1-allow to join the whitelist */
    /* 0-default 1-optimization completed 2-20 minutes has been in the network 3 - is */
    td_u8 allow_join_white_reason;
    /* allowed state, the external permission to restore after the enable state */
    td_u32 allow_join_white_time;  /* timestamps allowed to be whitelisted */
    td_u32 forbid_join_white_time; /* timestamps that are not allowed to be whitelisted */
    /* cause from permission status to prohibition 0-default 1-plc channel not available */
    td_u8 forbid_join_white_reason;
    /*
     * external de-enable
     * 0-unknown 1 - need to set the site to notify the mac when the table file is notified for 20 minutes.
     * wait for the site to enter the network. when the table file changes from empty to non-empty,
     * the flag will be set.
     * 2 - the file is cleared when the mac address is cleared, and then the table file is added. set this flag to 1
     */
    td_u8 set_wait_topo_flag;
    td_u8 rm_flag; /* 0-not in the probe through the meter 1 - is detecting the copy, has sent the transparent frame */
    td_u8 extern_flag; /* external operation detection meter reading mechanism enabled 0-external no operation */
    /* 1-external operation */
    td_u8 original_enable;   /* original switch before external enable */
    td_u8 curr_turn;         /* current probe metering rounds */
    td_u16 detect_white_num; /* a single number of white names appended in the probe mechanism */
    td_u8 topo_status;
    td_u8 uc97_di_index; /* currently reading the di index of the 97 table */
    td_u8 reserved;
    td_u8 detect_end_reason[5];    /* 5 count */
    td_u32 detect_start_time[5];   /* the time when the last 5 probes started */
    td_u32 detect_end_time[5];     /* the time of the last 5 detection mechanisms */
    td_u32 evaluted_time;          /* topology optimization completion time */
    td_u32 plc_disconnect_time;    /* plc channel becomes unavailable time */
    td_u32 archives_clear_time;    /* when the table file was recently emptied */
    td_u32 archives0_to1_time;     /* time when the table file changes from empty to non-empty */
    td_u32 extern_disable_time;    /* externally disable detection switch time */
    td_u32 extern_resume_time;     /* external recovery enable detection switch time */
    td_u16 black_list_num;         /* number of blacklists */
    td_u16 detect_white_valid_num; /* the number of valid whitelists added to the probe mechanism */
    td_u8 refresh_white_flag;      /* refresh whitelist timer monitor flag */
    td_u8 reserved2[15];           /* pad 15 count */
} ext_dsid_app_cco_detect_rm_inf;

typedef struct {
    ext_obj64 obj_cnt;
    td_u16 total_num;      /* total number of current nodes */
    td_u32 modify_cnt;     /* modified times */
    td_u32 add_cnt;        /* number of additions */
    td_u32 del_cnt;        /* number of deletions */
    td_u32 init_cnt;       /* number of initializations */
} ext_dsid_app_mrec_status; /* current table file status */

typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN];
    td_u8 padding[2];        /* pad 2 byte */
} ext_dsid_app_mac_list_cont; /* MacList information */

/* mac address information */
typedef struct {
    td_u8 mac_addr[EXT_PLC_MAC_ADDR_LEN]; /* mac address, 6 bytes, placed first */
    td_u8 report;                        /* has been reported up */
    td_u8 retries;                       /* number of retries */
} dfx_mrs_mac_info;

/* meter information */
typedef struct {
    td_u8 meter_addr[EXT_METER_ADDR_LEN]; /* meter address, 6 bytes */
    td_u8 protocol;                      /* statute type */
    td_u8 device_type;                   /* device type */
} dfx_mrs_meter_info;

typedef struct {
    td_u16 ttl;               /* mapping relationship ttl */
    td_u8 report;             /* report flag */
    td_u8 lock;               /* xxx */
    dfx_mrs_mac_info mac;     /* mac */
    dfx_mrs_meter_info meter; /* meter information */
} ext_dsid_app_mac_map_cont;   /* mac mapping table content */

typedef struct {
    td_bool valid_flg;   /* valid flag td_true : this slave is valid td_false : this slave is invalid */
    td_u8 protocol_type; /* specification type 00: reserved (unknown) 01:97-645 02:07-645 other: unknown */
    td_u16 node_seq;     /* slave node (electric meter) serial number in the table file */
    td_u8 node_addr[EXT_PLC_MAC_ADDR_LEN]; /* slave address (meter address) */
    /*
     * node information save valid flag
     * (td_true: the node information should be saved td_false: the node information should not be saved)
     */
    td_bool save_valid_flg;
    td_u8 check_sum;    /* checksum */
} ext_dsid_app_mrec_inf; /* table file content */

typedef struct {
    ext_obj64 obj_cnt;
    td_u32 sign;
    td_u32 option;          /* upgrade option */
    td_u32 upg_id;          /* upgrade id */
    td_u32 file_len;        /* length of the upgrade file */
    td_u32 blk_num;         /* block 512 bytes is a unit */
    td_u32 upg_time_window; /* upgrade time window */
    td_u32 last_block_id;   /* id of the previous block */
    td_u32 wait_max_upg_time;
} ext_dsid_app_upg_continus_start_nv;

typedef struct {
    ext_obj64 obj_cnt;
    td_u32 sign;
    td_u32 sign_2;
    td_u32 sign_3;
    td_u32 entry_timestamp;
    td_u32 notify_timestamp;
    td_u32 from;
} ext_dsid_app_upg_continus_topo_notify_info;

typedef struct {
    ext_obj64 obj_cnt;
    td_u32 clear_mrs_upg_time; /* the time when the mrs received the last concentrator clear download command */
    /* the time when the mrs started the upgrade last time (that is, receiving the correct first frame download file) */
    td_u32 mrs_upg_start_time;
    /* module upgrade end time, including the end of the upgrade and the end of the upgrade caused by other reasons */
    td_u32 mrs_upg_end_time;
    td_u8 mrs_rx_clear_upg_status; /* the status of the mrs module when it receives the clear download command */
    td_u8 mrs_upg_status;          /* mrs module remote upgrade status */
    /* the wrong identifier in the message received by the mrs module during the download process */
    td_u16 mrs_frame_err_flag;
    /* the upgrade status received by the mrs module is not the 4 values in the secondary development interface. */
    td_u8 mrs_rx_err_upg_status;
    td_u8 mrs_release_mem_fail; /* mrs module releases the flag of the upgrade file memory failure */
    /* the number of times the upgrade command was received while the upgrade was started */
    td_u8 start_upg_rx_stop_upg;
    td_u8 reserved;
    td_u8 mrs_get_upg_momery_fail; /* mrs module gets the flag of upgrade file memory failure */
    td_u8 upg_file_type;           /* upgrade file type */
    td_u8 upg_start_ret;           /* the result of the initial upgrade sent by the mrs module to the upg module */
    td_u8 upg_stop_ret;            /* the result of the stop upgrade sent by the mrs module to the uog module */
    td_u32 mrs_upg_len_err_size;   /* the value of the upgrade file length error during mrs module upgrade download */
    /* the correct number of frames received by the mrs module in the concentrator during the upgrade process */
    td_u16 mrs_rx_upg_frame_num;
    /* number of frames sent by the mrs module to the concentrator during the upgrade process */
    td_u16 mrs_tx_upg_frame_num;
    td_u32 upg_start_time;        /* the start upgrade time sent by the mrs module to the upg module */
    td_u32 upg_stop_time;         /* stop upgrade time sent by the mrs module to the upg module */
    td_u32 mrs_msg_rx_start_time; /* mrs receives the timestamp of the start of the upgrade response frame */
    td_u32 mrs_msg_rx_start_ret;  /* mrs receives the return value of the start upgrade response frame */
    /* total number of correct messages received by the mrs module during remote upgrade */
    td_u16 mrs_msg_rx_total_times;
    /* number of messages sent by the mrs module to the mrs itself during remote upgrade */
    td_u16 mrs_msg_tx_mrs_times;
    /* number of messages sent by the mrs module to the upg module during remote upgrade */
    td_u16 msg_tx_upg_times;
    /* the number of times the mrs module received the upg module during the remote upgrade */
    td_u16 msg_rx_upg_times;
    /* the number of times the object id or upgrade id is not equal to 4 in the message that the mrs module */
    td_u16 msg_rx_upg_id_err_times;
    /* receives the upg module during the remote upgrade. */
    /* number of messages sent by the mrs module to the drv_upg module during remote upgrade */
    td_u8 msg_tx_drv_times;
    /* the number of times the mrs module received the drv_upg module during the remote upgrade */
    td_u8 msg_rx_drv_times;
    td_u16 msg_tx_fail_id; /* failed message id sent by the mrs module during remote upgrade */
    td_u8 msg_tx_fail_num; /* total number of failed messages sent by the mrs module during remote upgrade */
    /* the message id received by the mrs module during the remote upgrade is not the number of */
    td_u8 mrs_msg_rx_id_err_times;
    /* upgrade stops, upgrade status or file check id. */
    td_u32 msg_fail_ret[4]; /* the return value of the last 4 times the message failed to be sent */
} ext_dsid_app_remote_upg_inf;

typedef struct {
    td_u32 sm_valid_frame;
    td_u32 sm_invalid_frame;
    td_u32 map_valid_frame;
    td_u32 map_invalid_frame;
    td_u32 reserved[12]; /* pad 12 * 4 byte */
} ext_dsid_app_plc_frame_stat_inf;

/* eqt statistic */
typedef struct {
    ext_obj64 obj_cnt;
    td_u32 timestamp;
    td_u32 eqt_timer_cnt[4]; /* [0] the number of times the timer is executed stops the timer count , 4count */
    /* after execution [1] reset timer cnt. [2] stop timer counter, 4 count */
    /* [0] max counter of timer [1] execute cnt before reset timer. [2] execute cnt before stop timer */
    /* [3] execute cnt in timer call-back */
    td_u32 eqt_timer_val[4];      /* 4 count */
    td_u32 eqt_eqx_port_reset[4]; /* [0] total number of port changes, [1]-[3] number of uart port changes, 4cout */
    /* send eqmc channel success, failure, insufficient memory, total number of transmissions record */
    td_u32 eqt_eqx_port_reset_val[6]; /* 6 count */
    td_u32 dbk_chl_rx_ok_cnt;         /* number of successfully received dbk messages */
    td_u32 dbk_chl_rx_chksum_err_cnt; /* number of dbk packets with crc check error */
    td_u32 dbk_chl_rx_id_err_cnt;     /* number of unsupported dbk packets */
    td_u32 dbk_chl_sndtask_cnt[2];    /* send dbk packets to the main task success/failure, 2 count */
    td_u32 dbk_chl_sndtask_nomem_err_cnt;
    td_u32 dbk_chl_not_find_req_proc_cnt;
    td_u32 dbk_chl_sndport_err_cnt;  /* number of errors sent to dbk */
    td_u32 snd_ecmsg_cnt[2];         /* send to the master task success and error calculation, 2 count */
    td_u32 eqt_ecmsg_proc_exec_cnt;  /* ec msg total processing entry execution times */
    td_u32 eqt_dbkreq_proc_exec_cnt; /* dbk req total processing entry execution times */
    td_u32 eqt_eqmc_rcv_cnt[4];      /* receive eqmc channel data count [3] total number of receptions, 4 count */
    /* send eqmc channel success, failure, insufficient memory, total number of transmissions */
    td_u32 eqt_eqmc_snd_cnt[4]; /* 4 count */
    td_u32 eqt_eqmc_rcv_len[4]; /* 4 count */
    td_u32 eqt_mc_parse_err_cnt;
} ext_dsid_app_eqt_chl_stat;

#endif
