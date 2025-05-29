/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: dfx_sys.
 * Author: CompanyName
 * Create: 2018-10-31
 */
#ifndef __DFX_SYS_H__
#define __DFX_SYS_H__

#include <soc_ft_nv.h>
#include "soc_types.h"
#include "soc_mdm_types.h"
#include "soc_phy_nv.h"

EXT_START_HEADER
#include "dfx_sys_deid.h"
#include "dfx_sys_dsid.h"
#include "dfx_sys_dmid_drv.h"
#include "dfx_sys_dmid_max.h"
#include "dfx_sys_dmid_ndm.h"
#include "dfx_sys_dsid_app.h"

#define NETWORK_NOTIFY_REC_TIMES (5)
#define UPG_CTX_STATUS_CHANGE_REC_TIMES (5)
#define UPG_CCO_REC_SND_QUERY_CNT (3)     /* Record the number of times the CCO sends the query */
#define UPG_CCO_REC_QUERY_EXC_STA_CNT (3) /* CCO record query abnormal number of sites */
#define UPG_STA_REC_REPLY_STATUS_CNT (3)  /* STA records the response status query times */

typedef struct {
    td_u32 formed_sta_cnt;
    td_u32 formed_lts;
} ext_upg_network_formed_stru;

typedef struct {
    td_u32 changed_lts;
    td_u8 upg_ctx_status;
    td_u8 upg_status;
    td_u8 from;
    td_u8 reserved;
} ext_upg_ctx_status_change_stru;

typedef struct {
    td_u16 tei;                     /* tei of the site */
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN]; /* site mac address */
} ext_upg_sta_info;                  /* 8 bytes */

typedef struct {
    td_u32 rcv_file_size;    /* receive upgrade file size reported when the site answers the query */
    td_u32 reply_time_stamp; /* timestamp of the site response status query */
    td_u32 reply_ret;        /* return value of the site response status query */
} ext_upg_sta_reply_query_info;

typedef struct {
    ext_obj64 obj_cnt;

    td_u16 rcv_blk_cnt; /* total number of upgraded data frames received [new] ok */
    td_u16 rcv_blk_lsn; /* the sequence number of the last received data [new] ok */
    td_u32 rcv_blk_lts; /* the timestamp of the last data received [new] ok */

    td_u16 snd_blk_cnt;      /* the total number of valid valid data packets is ok */
    td_u16 snd_blk_lsn;      /* the serial number of the last sent upgrade data ok */
    td_u16 snd_blk_fail_cnt; /* the number of times to return fail when sending data ok */
    td_u16 snd_blk_full_cnt; /* the number of times to return full when sending data ok */
    td_u32 snd_blk_lts;      /* the timestamp of the last successful send upgrade data ok */

    td_u16 snd_status_cnt;                     /* the number of times the status query was sent ok */
    td_u16 snd_status_fail_cnt;                /* send status query failures ok */
    td_u16 snd_status_full_cnt;                /* send status query but the queue is full ok */
    td_u8 snd_status_mac[EXT_PLC_MAC_ADDR_LEN]; /* the mac address of the current sending status query site ok */
    td_u32 snd_status_lts;                     /* the timestamp of the last time the status query was sent ok */
    /* the number of times the status query reply was received, excluding the number of drops ok */
    td_u16 rcv_status_reply_cnt;
    td_u16 rcv_status_reply_discard_cnt; /* receive status query reply discards ok */
    td_u32 rcv_status_lts;               /* last successful timestamp for receiving a site query reply */
    td_u16 pad;
    td_u16 snd_affirm_cnt;      /* the number of times the completion notification was sent ok */
    td_u16 snd_affirm_fail_cnt; /* the number of failures to send completion notifications ok */
    td_u16 snd_affirm_full_cnt; /* send completion notification but the queue is full ok */
    td_u32 snd_affirm_lts;      /* timestamp of the last notification of completion notification ok */

    td_u32 snd_stop_lts;        /* the last timestamp to stop the upgrade is ok */
    td_u32 snd_stop_req_upg_id; /* upg id ok to stop upgrading frames when the last time the download is stopped */
    td_u16 snd_stop_cnt;        /* send the number of times to stop upgrading ok */
    td_u16 snd_stop_fail_cnt;   /* send the number of failed to stop upgrading ok */
    td_u16 snd_stop_full_cnt;   /* send the stop upgrade but the queue is full ok */

    td_u16 snd_start_cnt;                    /* send the number of starts to upgrade ok */
    td_u16 snd_start_fail_cnt;               /* send the number of failed upgrades to start ok */
    td_u16 snd_start_full_cnt;               /* send the start of the upgrade but the queue is full ok */
    td_u32 snd_start_lts;                    /* the last timestamp to start the upgrade is ok */
    td_u16 stat_rcv_reply_start_discard_cnt; /* reply to start the upgrade and discard the number of times ok */
    /* the number of times the current upgrade reply starts to be upgraded, excluding the discarded part ok */
    td_u16 stat_rcv_reply_start_cnt;
    td_u32 stat_rcv_reply_start_lts; /* last successful processing of the received start frame timestamp ok */

    td_u32 upg_finish_ts;   /* whole network upgrade end timestamp */
} ext_dsid_upg_cco_snd_info; /* (0xa502) */

/* cco strategy related statistics */
typedef struct {
    ext_obj64 obj_cnt;

    td_u32 bc_entry_lts;  /* broadcast policy entry timestamp ok */
    td_u32 u2b_entry_lts; /* u2b policy enters timestamp ok */
    td_u32 entry_lts;     /* unicast policy entry timestamp ok */

    td_u16 bc_loop;         /* broadcast policy has sent the number of rounds ok */
    td_u16 u2b_proc_cnt;    /* u2b policy has processed the number of agents ok */
    td_u16 proc_cnt;        /* unicast policy has processed the number of sites ok */
    td_u8 finish_timer_cnt; /* upgrade the number of pre-start timers */
    td_u8 drv_upg_mode;     /* upgrade mode returned by the underlying system at power-on */

    td_u32 verify_upg_file_lrc; /* file verification result */
    td_u32 verify_upg_file_lts; /* file check timestamp */
    td_u32 real_start_ret;      /* write flash to start the upgrade result */

    td_u32 check_rcv_block_lrc; /* the last received block check result */
    td_u32 check_rcv_block_lts; /* the last received block timestamp */
    td_u32 cache_rcv_block_lrc; /* the last received block cache result */
    td_u32 cache_rcv_block_lts; /* the most recently received data cache timestamp */
    td_u8 drv_start_ret;        /* call the underlying start to upgrade the frame results */
    td_u8 mac_freq;             /* the current running band of mac */
    td_u8 mem_write_fail_cnt;   /* write flash failures */
    td_u8 mem_read_fail_cnt;    /* read flash failures */
    td_u32 mem_write_ret;       /* last time trv write upgrade cache result */
    td_u32 mem_read_ret;        /* last time trv read upgrade cache result */
    td_u16 mem_write_lts;       /* last time trv write upgrade cache timestamp */
    td_u16 mem_read_lts;        /* last time trv read upgrade cache timestamp */
    td_u32 start_timer_ret;     /* the return value of the timer since the last time */
    td_u32 start_timer_period;  /* the last cycle of the timer */
    td_u8 start_timer_id;       /* the last timer id */
    td_u8 pad1;                 /* pad 1 byte */
    td_u16 max_bitmap_len;      /* max bitmap len */
    td_u32 max_file_len;        /* max file len */
    td_u32 list_ret;
    td_u8 pad[8];           /* pad 8 byte */
} ext_dsid_upg_cco_rcv_info; /* (0xa504) */

/* cco channel and network status change statistics */
typedef struct {
    ext_obj64 obj_cnt;

    td_u16 stat_proc_cnt;   /* the number of current upgrade transfers is ok */
    td_u16 stat_verfiy_cnt; /* the number of current verifications is ok */
    td_u16 stat_succ_cnt;   /* the current number of successes is ok */
    td_u16 stat_fail_cnt;   /* the current number of failures is ok */

    td_u32 network_notify_times;                                            /* number of channel changes ok */
    ext_upg_network_formed_stru network_notify_st[NETWORK_NOTIFY_REC_TIMES]; /* last 5 channel changes ok */
    /* network status change record ok */
    ext_upg_ctx_status_change_stru upg_ctx_change[UPG_CTX_STATUS_CHANGE_REC_TIMES];
} ext_dsid_upg_network_info; /* (0xa505) */

typedef struct {
    td_u32 file_crc;
    td_u32 file_len;
    td_u32 tx_plc_cnt;
    td_u32 rx_plc_cnt;
    td_u32 last_tx_time; /* unit: milliseconds */
    td_u32 last_rx_time; /* unit: milliseconds */
    td_u16 topo_num;
    td_u16 current;
    td_u16 tx_plc_fail_cnt;
    td_u16 rx_plc_err_cnt;
    td_u32 finish_time;
    td_u16 result_suc_cnt;
    td_u16 result_fail_cnt;
    td_u8 status;
    td_u8 type;
    td_u8 query_timer_flag;
    td_u8 reserved;
} ext_dsid_upg_cco_query_info; /* (0xa512) */

typedef struct {
    td_u32 option;              /* upgrade option */
    td_u32 file_crc;            /* upgrade file crc */
    td_u32 file_len;            /* upgrade file length */
    td_u32 rx_info_plc_time[2]; /* the timestamp of the last received packet, in milliseconds, 2 count */
    td_u32 tx_info_plc_time[2]; /* the timestamp of the last two messages sent, in milliseconds, 2count */
    td_u32 tx_plc_cnt;          /* total number of times to answer the query site information message */
    td_u32 rx_plc_cnt;          /* total number of messages received by the query site */
    td_u8 rx_plc_err_cnt;       /* received incorrect plc frame times */
    td_u8 tx_plc_fail_cnt;      /* send plc frame failures */
    td_u8 get_file_info_ret;    /* get the result of the upgrade file information */
    td_u8 mem_write_fail_cnt;   /* write flash failures */
    td_u32 mem_write_ret;       /* last time the upgrade cache flash return value */
} ext_dsid_upg_sta_query_info;   /* (0xa513) */

typedef struct {
    ext_obj64 obj_cnt;

    td_u16 rcv_blk_cnt;         /* total number of upgraded data frames received ok */
    td_u16 rcv_blk_lsn;         /* the sequence number of the last received data ok */
    td_u32 rcv_blk_lts;         /* the timestamp of the last time the data was received ok */
    td_u32 check_rcv_block_lrc; /* last checksum return package return value ok */
    td_u32 check_rcv_block_lts; /* the timestamp of the last verification upgrade package ok */
    td_u32 cache_rcv_block_lrc; /* the last time the cached data was returned. ok */
    td_u32 cache_rcv_block_lts; /* the timestamp of the data received last cached ok */
    td_u32 last_edge_id;        /* the last boundary value reported to the dbk ok */
    td_u32 last_edge_id_lts;    /* timestamp of the last boundary value reported to the dbk ok */
    td_u32 verify_upg_file_lrc; /* upgrade file verification result ok */
    td_u32 verify_upg_file_lts; /* upgrade file check timestamp ok */

    td_u16 rcv_start_cnt;   /* received start frame number ok */
    td_u16 rcv_start_lrc;   /* the last call to the start function returns the value ok */
    td_u32 rcv_start_lts;   /* the timestamp of the last successful call to the start function ok */
    td_u16 rcv_u2b_blk_cnt; /* number of times the u2b packet was received and forwarded */

    td_u16 reply_start_cnt;      /* answer the number of start frames ok */
    td_u32 reply_start_lts;      /* timestamp of the last successful response to the start frame ok */
    td_u16 reply_start_fail_cnt; /* answer the number of failed start frames ok */
    td_u16 reply_start_full_cnt; /* answer start frame queue full count ok */
    td_u32 reply_start_lrc;      /* the return value of the last reply start upgrade frame ok */

    td_u8 error_para_start;   /* receive an error and start upgrading the number of frames */
    td_u8 discard_rcv_start;  /* received the number of times the upgrade frame was not processed */
    td_u8 std_malloc_fail;    /* number of memory failures allocated by the national network mode */
    td_u8 affirm_discard_cnt; /* discard the number of notifications completed on the network */
    td_u8 drv_upg_mode;       /* upgrade mode returned by the underlying system at power-on */
    td_u8 drv_start_ret : 4;  /* call the underlying start interface return value */
    td_u8 drv_extend_ret : 4; /* call the underlying validation extension information interface return value */
    /* do you need to upgrade yourself: 55 need to upgrade yourself / 22 do not need to upgrade yourself */
    td_u8 upgrade_self;
    td_u8 plc_mode;          /* plc protocol mode */
} ext_dsid_upg_sta_start_blk; /* (0xa503) */

typedef struct {
    ext_obj64 obj_cnt;

    td_u32 rcv_status_lts; /* the timestamp of the last time the status query frame was received ok */
    td_u16 rcv_status_cnt; /* the number of times the status query frame was received ok */
    td_u16 rcv_start_frc;  /* the first call to start the return value of the upgrade function */

    td_u32 write_ret;              /* write flash results */
    td_u32 reply_status_lts;       /* timestamp of the last response status query ok */
    td_u16 reply_status_cnt;       /* number of times to reply status query frame ok */
    td_u16 reply_status_fail_cnt;  /* response status query frame failures ok */
    td_u16 reply_status_full_cnt;  /* response status query frame queue full count ok */
    td_u8 reply_status_upg_status; /* board status when the last response status query ok */
    td_u8 reply_status_report_cnt; /* sta actively reports status times */

    td_u32 finish_timer_set_lts; /* last time set the finish or affirm timer timestamp ok */
    td_u32 finish_timer_period;  /* last time set the finish or affirm timer timer period ok */

    td_u32 rcv_affirm_lts; /* timestamp of the last time the completion notification was received ok */
    td_u16 rcv_affirm_cnt; /* the number of times the completion notification was received and processed ok */

    td_u16 rcv_stop_cnt;        /* received the number of stop escalation responses ok */
    td_u16 rcv_stop_denied_cnt; /* refusal to stop the upgrade ok */
    td_u16 mem_write_lts;       /* the last time the flash interface timestamp was written (unit: s) */
    td_u32 rcv_stop_lrc;        /* the last time i received a return value that stopped the upgrade */
    td_u32 rcv_stop_lts;        /* the timestamp of the last stop of the upgrade ok */
    /* receive the upg id of the current upgrade of the module when the upgrade is stopped. */
    td_u32 rcv_stop_upg_id;
    td_u32 rcv_stop_req_upg_id; /* receive the upg id ok requesting to stop the upgrade when the upgrade is stopped */
} ext_dsid_upg_sta_status_stop;  /* (0xa506) */

/* Site response status query information */
typedef struct {
    ext_obj64 obj_cnt;
    /* STA site last 3 response status query information */
    ext_upg_sta_reply_query_info stru_sta_reply_query_detail[UPG_STA_REC_REPLY_STATUS_CNT];
} ext_dsid_upg_sta_reply; /* (0xa508) */

/* ***************************************************************************** */
/* APP消息定义 */
/* ***************************************************************************** */
/* 范围 [0B00, 0F00) */
/* ***************************************************************************** */
/* ***************************************************************************** */
/* 结构定义 */
/* ***************************************************************************** */
#define APP_MAX_BUF_LEN 96 /* Maximum report buffer length */

typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN]; /* the changed mac address */
    td_s16 index;                   /* mac index */
    td_u8 option;                   /* operation type */
} ext_dmid_app_mac_list_change;      /* maclist changes */

typedef struct {
    td_u8 addr[METER_ADDR_LEN];     /* the table address where the change occurred */
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN]; /* the changed mac address */
    td_u8 option;                   /* operation type */
} ext_dmid_app_mac_map_change;       /* macmap changes */

typedef struct {
    td_u16 len;                  /* length of data received */
    td_u8 data[APP_MAX_BUF_LEN]; /* received data content */
} ext_dmid_app_chl_rx_uart;       /* meter reading channel data reception */

typedef struct {
    td_u16 len;                  /* length of data sent */
    td_u8 data[APP_MAX_BUF_LEN]; /* sent data content */
} ext_dmid_app_chl_tx_uart;       /* meter reading channel data transmission */

typedef struct {
    td_u16 id;                      /* received the id of the plc frame */
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN]; /* source mac */
    td_u16 len;                     /* received the length of the plc frame structure */
    td_u8 data[APP_MAX_BUF_LEN];    /* received the contents of the plc frame structure */
} ext_dmid_app_chl_rx_plc;           /* plc channel data reception */

typedef struct {
    td_u16 id;                      /* send the id of the plc frame */
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN]; /* purpose mac */
    td_u16 len;                     /* send the length of the plc frame */
    td_u8 data[APP_MAX_BUF_LEN];    /* send the contents of the plc frame */
} ext_dmid_app_chl_tx_plc;           /* plc channel data transmission */

typedef struct {
    td_u8 type;           /* channel type */
    td_u8 state;          /* channel status */
} ext_dmid_app_chl_status; /* current channel status notification */

typedef struct {
    td_u16 id;             /* timer id */
    td_u32 value;          /* timer value */
    td_u8 option;          /* timer type */
} ext_dmid_app_timer_start; /* timer starts */

typedef struct {
    td_u16 id;            /* timer id */
} ext_dmid_app_timer_stop; /* timer stops */

typedef struct {
    td_u16 id;             /* timer id */
    td_u32 p;              /* timer callback function pointer */
} ext_dmid_app_timer_activ; /* timer activation */

typedef struct {
    td_u16 len;                  /* data length */
    td_u8 data[APP_MAX_BUF_LEN]; /* data content */
} ext_dmid_app_rx_frame_complete; /* received a full frame */

typedef struct {
    td_u8 ctrl;                   /* control word */
    td_u8 afn;                    /* function code */
    td_u8 fn;                     /* information unit */
} ext_dmid_app_376_2_dec_complete; /* 376.2 frame structure parsing completed */

typedef struct {
    td_u8 protocol;             /* statute type */
    td_u8 ctrl;                 /* control word */
    td_u32 id;                  /* data id */
} ext_dmid_app_645_dec_complete; /* 645 frame structure parsing completed */

typedef struct {
    td_u32 id;                  /* session id */
    td_u32 sn;                  /* session sn */
    td_u8 cnt;                  /* cnt logo */
    td_u8 acd;                  /* acd logo */
} ext_dmid_app_ses_enc_complete; /* the session is organized */

typedef struct {
    td_u32 id[2];                 /* session id ([0] frame [1] local), 2 count */
    td_u32 sn[2];                 /* session sn ([0] frame [1] local), 2 count */
    td_u8 cnt;                    /* cnt logo */
    td_u8 acd;                    /* session invalid reason */
} ext_dmid_app_ses_check_complete; /* session checked */

typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN]; /* corresponding mac address */
    td_u16 len;                     /* plc frame length */
    td_u8 data[APP_MAX_BUF_LEN];    /* plc frame structure content */
} ext_dmid_app_tx_get_addr_plc;      /* send to get the meter address plc frame */

typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN];  /* corresponding mac address */
    td_u8 num;                       /* the corresponding number of table addresses */
    meter_addr meter[MAX_METER_NUM]; /* corresponding list of table addresses */
    td_u16 len;                      /* plc frame length */
    td_u8 data[APP_MAX_BUF_LEN];     /* plc frame structure content */
} ext_dmid_app_rx_get_addr_plc;       /* receive the acquisition of the meter address plc frame */

typedef struct {
    td_u8 addr[METER_ADDR_LEN];     /* table address */
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN]; /* corresponding mac address */
} ext_dmid_app_find_mac_map_ok;      /* find the mac mapping table successfully */

typedef struct {
    td_u8 addr[METER_ADDR_LEN];  /* table address */
    td_u8 result;                /* reason for failure */
} ext_dmid_app_find_mac_map_fail; /* failed to find mac mapping table */

typedef struct {
    td_u8 addr[METER_ADDR_LEN];  /* the current meter address of the processing node */
    td_u16 index;                /* the serial number of the current processing node */
    td_u16 len;                  /* frame length */
    td_u8 data[APP_MAX_BUF_LEN]; /* frame content */
} ext_dmid_app_inq_meter;         /* send a read table query frame */

typedef struct {
    td_u8 addr[METER_ADDR_LEN];  /* reply to the table address of the read table */
    td_u8 read_flg;              /* read the reading flag of the reading table */
    td_u16 len;                  /* the length of the message that answers the read list */
    td_u8 data[APP_MAX_BUF_LEN]; /* answer the message content of the read table */
} ext_dmid_app_inq_ans_meter;     /* receive the read table query response frame */

typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN]; /* purpose mac */
    td_u16 len;                     /* read table plc structure length */
    td_u8 data[APP_MAX_BUF_LEN];    /* read table plc structure content */
} ext_dmid_app_tx_rm_plc_cco;        /* send read table plc frame (cco) */

typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN]; /* source mac */
    td_u16 len;                     /* read table plc structure length */
    td_u8 data[APP_MAX_BUF_LEN];    /* read table plc structure content */
} ext_dmid_app_rx_rm_plc_cco;        /* receive the read table plc frame (cco) */

typedef struct {
    td_u16 len;                  /* read table plc structure length */
    td_u8 data[APP_MAX_BUF_LEN]; /* read table plc structure content */
} ext_dmid_app_tx_rm_plc_sta;     /* send read table plc frame (sta) */

typedef struct {
    td_u16 len;                  /* read table plc structure length */
    td_u8 data[APP_MAX_BUF_LEN]; /* read table plc structure content */
} ext_dmid_app_rx_rm_plc_sta;     /* receive the read table plc frame (sta) */

typedef struct {
    td_u8 second;
    td_u8 minute;
    td_u8 hour;
    td_u8 day;
    td_u8 month;
} app_time;

typedef struct {
    app_time start_time;        /* search table start time */
    td_u16 duration;            /* search table duration */
    td_u8 retry;                /* number of resends from the node */
    td_u8 slice;                /* random waiting time slice */
} ext_dmid_app_start_scan_meter; /* start the search table */

typedef struct {
    td_u8 num;                       /* number of slave nodes reported */
    meter_addr meter[MAX_METER_NUM]; /* slave address */
    td_u8 protocol;                  /* statute type */
    td_u16 index;                    /* slave node number */
    td_u8 type;                      /* device type */
} ext_dmid_app_rep_scan_meter;        /* report the search information */

typedef struct {
    td_u8 from;                  /* data source */
    td_u16 len;                  /* data length */
    td_u8 data[APP_MAX_BUF_LEN]; /* data content */
} ext_dmid_app_tx_rm_sta;         /* send meter reading data (sta) */

typedef struct {
    td_u16 len;                  /* data length */
    td_u8 data[APP_MAX_BUF_LEN]; /* data content */
} ext_dmid_app_rx_rm_sta;         /* receive meter reading data (sta) */

typedef struct {
    td_u8 addr[METER_ADDR_LEN];  /* reported table address */
    td_u16 len;                  /* reported frame length */
    td_u8 data[APP_MAX_BUF_LEN]; /* reported frame content */
} ext_dmid_app_rep_rm_cco;        /* report reading data (cco) */

typedef struct {
    td_u16 len;                  /* heartbeat frame length */
    td_u8 data[APP_MAX_BUF_LEN]; /* heartbeat frame content */
} ext_dmid_app_tx_heartbeat;      /* send heartbeat frame */

typedef struct {
    td_u8 num;                       /* current table address number */
    meter_addr meter[MAX_METER_NUM]; /* table address list (maximum 32 elements) */
} ext_dmid_app_upd_maddr;             /* update the list of meter addresses */

typedef struct {
    td_u8 num;                /* number of current queue elements */
    td_u32 id[MAX_QUEUE_NUM]; /* queue id list */
    td_u8 form;               /* data source */
    td_u8 top_state;          /* header status (whether locked) */
} ext_dmid_app_que_status;     /* current queue status */

/* ************************************************ ***************************** */
/* system statistic definition */
/* ************************************************ ***************************** */
/* range [0xb000, 0xb200) */
/* ************************************************ ***************************** */
typedef struct {
    td_u16 send_uart_fail_cnt;
    td_u16 ack_ind_malloc_fail_cnt;
    td_u16 msg_malloc_fail_cnt;
    td_u16 msg_send_fail_cnt;
    td_u16 msg_overbig_cnt;
    td_u16 ind_send_fail_cnt;
    td_u16 ind_malloc_fail_cnt;
    td_u8 diag_queue_used_cnt;
    td_u8 diag_queue_total_cnt;
    td_u8 dec_fail_cnt;
    td_u8 enc_fail_cnt;
    td_u16 pkt_size_err_cnt;
    td_u32 local_req_cnt;
    td_u32 remote_req_cnt;
    td_u16 req_cache_overflow_cnt;
    td_u8 conn_excep_cnt;
    td_u8 conn_bu_cnt;
    td_u8 chl_busy_cnt;
    td_u8 req_overbig1_cnt;
    td_u16 rx_remote_req_cnt;
    td_u16 rx_remote_req_invalid_cnt;
    td_u8 cmd_list_total_cnt;  /* < the total number of command lists that support registration */
    td_u8 cmd_list_used_cnt;   /* < the number of registered command lists */
    td_u8 stat_list_total_cnt; /* < the total number of statistical object lists that support registration */
    td_u8 stat_list_used_cnt;  /* < the number of registered statistics object lists */
    td_u8 req_overbig2_cnt;
    td_u8 invalid_dec_id;
    td_u8 heart_beat_timeout_cnt;
    td_u8 rx_start_flag_wrong_cnt;
    td_u8 rx_ver_wrong_cnt;
    td_u8 rx_pkt_data_size_wrong_cnt;
    td_u8 rx_crc16_req_wrong_cnt;
    td_u8 rx_crc16_mux_wrong_cnt;
    td_u8 pad[2]; /* pad 2 byte */
} ext_stat_diag_qry;
/* System message definition */
/* range [0x2000, 0x2100) */
#define EXT_DMID_UPG_BASE (0x2000)
#define EXT_DMID_UPG_CCO_SEND_START (EXT_DMID_UPG_BASE + 0x1)
#define EXT_DMID_UPG_CCO_SEND_STOP (EXT_DMID_UPG_BASE + 0x2)
#define EXT_DMID_UPG_CCO_SEND_DATA (EXT_DMID_UPG_BASE + 0x3)
#define EXT_DMID_UPG_CCO_SEND_STATUS (EXT_DMID_UPG_BASE + 0x4)
#define EXT_DMID_UPG_CCO_SEND_QRY_INFO (EXT_DMID_UPG_BASE + 0x5)
#define EXT_DMID_UPG_CCO_SEND_AFFIRM (EXT_DMID_UPG_BASE + 0x6)

#define EXT_DMID_UPG_CCO_RCV_START_IND (EXT_DMID_UPG_BASE + 0x7)
#define EXT_DMID_UPG_CCO_RCV_STATUS_IND (EXT_DMID_UPG_BASE + 0x8)
#define EXT_DMID_UPG_CCO_RCV_QRY_INFO (EXT_DMID_UPG_BASE + 0x9)

#define EXT_DMID_UPG_CCO_ADD_PROCESS_ITEM (EXT_DMID_UPG_BASE + 0xA)
#define EXT_DMID_UPG_CCO_DEL_PROCESS_ITEM (EXT_DMID_UPG_BASE + 0xB)

#define EXT_DMID_UPG_CHL_STATUS (EXT_DMID_UPG_BASE + 0xC)
#define EXT_DMID_UPG_MAC_CHL_STATUS (EXT_DMID_UPG_BASE + 0xD)

#define EXT_DMID_UPG_REMOVE_ITEM_INFO (EXT_DMID_UPG_BASE + 0xE)

#define EXT_DMID_UPG_VERIFY_RESULT (EXT_DMID_UPG_BASE + 0xF)
#define EXT_DMID_UPG_WRITE_RESULT (EXT_DMID_UPG_BASE + 0x10)

#define EXT_DMID_UPG_TIME_INFO (EXT_DMID_UPG_BASE + 0x11)

typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN];
    td_u16 id;
    td_u32 ret;
} dmid_upg_send_req;

typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN];
    td_u16 rcv_len;
} dmid_upg_rcv_ind;

typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN];
    td_u16 tei;
} dmid_upg_process_item;

typedef struct {
    td_bool avail;
    td_u8 reserved[3]; /* pad 3 byte */
} dmid_upg_chl_status;

typedef struct {
    td_u32 ret;
} dmid_upg_result;

typedef struct {
    td_u32 time;
} dmid_upg_time;

/* System configuration command definition */
/* range [0xA001, 0xB000) */
/* Command addition must be added in order, and for sizes smaller than EXT_DCID_SYS_CMD_FIXED_LEN */
#define EXT_DCID_SAL_DBG_SWT_CFG 0xA001 /* EXT_DCID_SAL_DBG_SWT_CFG_S */
#define EXT_DCID_STR 0xA010
#define EXT_DCID_SYS_NUM 4 /* The number of all commands, you need to adjust the macro definition size after adding */
/* the command, recompile SAL */
#define EXT_DCID_SYS_CMD_FIXED_LEN 64 /* The maximum length of the command parameter is 64 */
/* The sum of the data structure sizes of */
#define EXT_DCID_SYS_BUFFER_LEN (EXT_DCID_SYS_CMD_FIXED_LEN * EXT_DCID_SYS_NUM)
/* all commands */
typedef struct {
    td_u8 cfg[8];   /* [0]: mac->dms; [1]: dms->mac; [2]: dump send 2 tcp port time.8 count */
    td_u32 data[8]; /* [0]: cmd id,8 count */
} ext_dcid_sal_dbg_swt_cfg;
/* range [0xB050, 0xB200) */
/* EXT_MDM_EQT_CHL_PK               0xB000    // EXT_MDM_EQT_CHL_PK_REQ_S  EXT_MDM_EQT_CHL_PK_IND_S */
#define EXT_SYS_RESERVED 0xB001 /* REQ, ACK, IND? */

/* Get configuration command data such as EXT_DCID_UPG_CHL_CFG_S EXT_DCID_SAL_DBG_CFG_S */
/* ulSysCid: configuration command definition, range [0xA001, 0xB000) */
EXT_EAPI td_u16 UAPI_SYS_GetSysCmdCfgLen(td_u32 sys_cid);

/* system command registration */
/* ulSysCid: configuration command definition, range [0xB000, 0xB050) */
/* usCfgLen: Take 0 */
EXT_EAPI td_u32 UAPI_SYS_RegisterSysCmdCfg(td_u32 sys_cid, td_u16 cfg_len);

/* CMN statistic */
/* CMN [0xA150, 0xA250) */
#define EXT_DSID_SYS_EVT_MSG 0xA150
#define EXT_DSID_SYS_PERIOD_MSG 0xA151
#define EXT_DSID_SYS_SDM_MSG 0xA152
#define EXT_DSID_SYS_MRS_MSG 0xA153
#define EXT_DSID_SYS_FA_MSG 0xA154
#define EXT_DSID_SYS_BAT_MSG 0xA155
#define EXT_DSID_SYS_NMLV_MSG 0xA156
#define EXT_DSID_SYS_FPGA_MSG 0xA157
#define EXT_DSID_SYS_SD_MODE_DFX 0xA160
#define EXT_DSID_SYS_DIAG_ASYNC 0xA161

#include <dfx_sys_sdm.h>
/* CTS */
#define EXT_DBG_CTS_TEST 0x500C     /* EXT_DBG_CTS_Q_S, , */
#define EXT_DBG_CTS_MAC_TEST 0x500D /* EXT_DBG_CTS_MAC_TEST */

EXT_END_HEADER
#endif /* __DFX_SYS_H__ */
