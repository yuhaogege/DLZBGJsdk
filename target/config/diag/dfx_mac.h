/*
 * Copyright (c) CompanyNameMagicTag 2018-2021. All rights reserved.
 * Description: DFX interfaces
 */
#ifndef _DFX_MAC_H
#define _DFX_MAC_H
#include "dfx_mac_value_def.h"
#include "dfx_sys.h"
#include "dfx_sys_sdm.h"
#include "soc_mdm_phy.h"
#include "dfx_mac_ll.h"
#include "dfx_mac_snr.h"
#include "dfx_mac_ndm.h"
#include "dfx_mac_nm.h"
#include "dfx_mac_ntb.h"
#include "dfx_mac_rt.h"
#include "dfx_mac_topo.h"
#include "dfx_mac_component.h"

#ifdef __cplusplus
extern "C" {
#endif
#define MAC_APPOINTED_STATIC_PROXY_TYPE 1
#define MAC_DYNAMIC_CHOSE_PROXY_TYPE 2

typedef td_u16 id_diag_report_cmd;
typedef td_u16 id_diag_event;
typedef td_u16 id_diag_query;
/* Chip ID in the simplified topology */
typedef enum {
    CHIP_VER_IDX_DW11 = 0x01,
    CHIP_VER_IDX_DW21V100 = 0x03,
    CHIP_VER_IDX_DW61V100 = 0x04,
    CHIP_VER_IDX_DW62V100 = 0x05,
    CHIP_VER_IDX_INVALID = 0xff
} chip_ver_idx_enum;
typedef struct _diag_cmd_report_repeat_request_join_stru_ {
    td_u16 tei;
    td_u16 pad;
} diag_cmd_report_repeat_request_join_stru;
/* 4.2 Record the event when the buffer overflows (each module reports the DBK) */
typedef struct _diag_cmd_report_buffer_overflow_stru_ {
    td_u8 module_id;
    td_u8 buffer_id;
    td_u8 over_flow_size;
    td_u8 pad;
} diag_cmd_report_buffer_overflow_stru;
/* 4.5 Headend: TDMA scheduling period allocation (the network management module reports the period to the console) */
typedef struct id_diag_cmd_tdma_entry_head_ {
    td_u8 beacon_slot_count;    /* Beacon timeslot number */
    td_u8 bind_csma_slot_count; /* Number of bound CSMA timeslots */
    td_u8 tdma_slot_count;      /* Number of TDMA timeslots */
    td_u8 beacon_duration;      /* Beacon timeslot duration (unit: 100 microseconds) Note:
                                 * he duration of each beacon timeslot is the same.
                                 */
} id_diag_cmd_tdma_entry_head;
typedef struct id_diag_cmd_beacon_slot_ {
    td_u32 offset;     /* Relative start time of the beacon timeslot,
                          which is the offset of the start time of the beacon period. Unit: 100 microseconds */
    td_u16 tei;        /* TEI corresponding to the timeslot */
    td_u8 beacon_type; /* Beacon type */
    td_u8 event;       /* Identifies an event (association summary). */
} id_diag_cmd_beacon_slot;
typedef struct id_diag_cmd_bind_csma_slot_ {
    td_u32 offset;   /* Relative time when the timeslot starts, that is, the offset of the start time
                        of the beacon period. The unit is 100 microseconds. */
    td_u16 duration; /* Timeslot duration, unit: 100 microseconds */
    td_u16 tei;      /* Destination TEI corresponding to the timeslot */
} id_diag_cmd_bind_csma_slot;
typedef struct id_diag_cmd_csma_slot_ {
    td_u16 offset;   /* Relative time when the timeslot starts, that is,
                        the offset of the start time of the beacon period.  The unit is 100 microseconds. */
    td_u16 duration; /* Timeslot duration, unit: 100 microseconds */
} id_diag_cmd_csma_slot;
typedef struct id_diag_cmd_tdma_slot_ {
    td_u16 offset;   /* Relative time when the timeslot starts */
    td_u16 duration; /* Timeslot duration */
    td_u16 tei;      /* TEI corresponding to the timeslot */
    td_u8 lid;       /* Timeslot connection identifier */
    td_u8 rsvd;      /* Reserved */
} id_diag_cmd_tdma_slot;
/* Area timeslot entry */
typedef struct id_diag_cmd_region_slot_st_ {
    td_u32 starttime;  /* Absolute start time of the regional timeslot, in NTB */
    td_u32 duration;   /* Duration. The unit is 100 microseconds. */
    td_u8 region_id;   /* Network SNID */
    td_u8 region_type; /* Timeslot type (reserved timeslot or common timeslot) */
    td_u16 rsvd;       /* Reserved */
} id_diag_cmd_region_slot_st;
/* Change the SNID entry. */
typedef struct id_diag_cmd_snid_entry_ {
    td_u32 starttime; /* Time when the SNID is changed */
    td_u8 snid;       /* New SNID */
    td_u8 rsvd1;      /* Reserved */
    td_u16 rsvd2;     /* Reserved */
} id_diag_cmd_snid_entry;
/* Change the key entry. */
typedef struct id_diag_cmd_encryption_key_entry_ {
    td_u32 starttime; /* Change the time of the key. */
    td_u8 key_index;  /* Index of the key to be enabled */
    td_u8 svd1;       /* Reserved */
    td_u16 rsvd2;     /* Reserved */
} id_diag_cmd_encryption_key_entry;
/* Specifications of the timeslot allocation table */
#define DFX_ID_DIAG_CMD_MAX_BEACON_SLOT 150     /* maximum number of Beacon timeslots */
#define DFX_ID_DIAG_CMD_MAX_BIND_CSMA_SLOT 50   /* Maximum number of timeslots that can be bound to a CSMA */
#define DFX_ID_DIAG_CMD_MAX_TDMA_SLOT 50        /* maximum number of TDMA timeslots */
#define DFX_ID_DIAG_CMD_MAX_REGION_SLOT_COUNT 7 /* maximum number of timeslots in an area */
/* 5 Statistics */
/* Query the default parameters of the request command. */
typedef struct {
    td_u32 clear_switch;
} diag_cmd_query_req_para;
typedef struct {
    td_u32 proxy_send_pk_cnt;  /* Number of packets sent by the proxy */
    td_u32 cco_receive_pk_cnt; /* Indicates the number of packets received by the CCO. */
} diag_cmd_query_comm_rate;
typedef struct {
    td_bool is_set_afe_power;     /* Whether to set the analog power */
    td_bool is_set_digital_power; /* Whether to set the digital power */
    td_bool is_write_nv;          /* Whether to write the NV item */
    td_u8 afe_pwoer;              /* Analog power */
    td_s8 digital_power[4];       /* Digital power 4 frequency bands digital power */
    td_u32 delay_effective_time;  /* Delay the effective time, in seconds. */
} diag_cmd_set_delay_power_change;
typedef struct {
    td_u32 delayed_restart_time; /* Delay restart time, in seconds */
} diag_cmd_delayed_restart;
typedef struct {
    td_u32 delay_time;
} diag_cmd_delay_set_ind;
typedef struct {
    td_u32 count[10]; /* Count of temp commands are of 10 words */
} diag_cmd_common_temp_command;
typedef struct {
    td_u32 config[8]; /* Configuration parameter is of 8 words */
} diag_cmd_mac_config_sw_stru;
#define DFX_NDM_CMD_MAC_R_NUM 10
typedef struct {
    td_u8 scope; /* 1: network-wide; 2: single point */
    td_u8 pad;
    td_u8 locked_proxy_addr[EXT_PLC_MAC_ADDR_LEN];
    td_u32 code;                /* Used to feed back the setting result. */
} diag_cmd_ndm_ctrl_route_lock; /* route lockout */
typedef struct {
    td_u8 scope;                  /* 1: network-wide; 2: single point */
    td_u8 pad[3];                 /* Reserved 3 bytes */
    td_u32 code;                  /* Used to feed back the setting result. */
} diag_cmd_ndm_ctrl_route_unlock; /* route unlock */
typedef struct {
    td_u8 oda[EXT_PLC_MAC_ADDR_LEN];
    td_u8 next[EXT_PLC_MAC_ADDR_LEN];
} diag_cmd_ndm_ctrl_route_config; /* static route configuration */
/* In the case of one PB block, the maximum packet length that can be carried by the NCR channel */
#define DIAG_CMD_NDM_CTRL_PING_PK_MAX_LEN 388 /* The available size is limited to one PB. \
                                                 Only the SGCC protocol is supported. */
typedef struct {
    td_u32 len; /* Indicates the length of the echo segment. The outer layer must be 4-byte aligned. */
    td_u8 echo[0];
} diag_cmd_ndm_ctrl_ping; /* ping command. This command does not support the command line operation. */
typedef struct {
    td_u32 nuk;
} diag_cmd_ndm_ctrl_search_dev_stru; /* device search */
typedef struct {
    td_u8 echo1;
    td_u8 echo2;
    td_u16 ack;
} diag_cmd_ndm_ctrl_pipe_ack;
typedef struct {
    td_u8 network_status;         /* Network status: network access status and network access status */
    td_u8 multi_network_status;   /* Multi-network status: abnormal status and normal status */
    td_u8 search_meter_status;    /* Table search status: table search and table search end */
    td_u8 read_meter_addr_status; /* Meter reading address status: read success and read failure */
    td_u8 sys_test_status;        /* Whether the tooling mode is used */
    td_u8 pad[3];                 /* Reserved 3 bytes */
    td_u32 set_not_join_network_read_meteradd_fail_led_cnt;
    td_u32 set_read_meter_fail_led_cnt;
    td_u32 set_not_join_network_led_cnt;
    td_u32 set_multi_network_exception_led_cnt;
    td_u32 set_join_network_led_cnt;
    td_u32 led_set_485_led_cnt;
    td_u16 led_not_join_and_read_meter_fail_timer_start_fail_cnt; /* Failed to start the timer because the terminal has
                                                                   * not accessed the network and failed to read the
                                                                   * meter address. */
    td_u16 led_read_meter_fail_timer_start_fail_cnt; /* Failed to start the timer for reading the meter address. */
    td_u8 led_rx_sys_test_timer_start_fail_cnt;      /* Number of times that the STA indicator timer fails to
                                                      * be started in fixture mode
                                                      */
    td_u8 led_set_sta_test_status_cnt;   /* Number of times that the STA indicator is turned on in fixture mode */
    td_u8 led_set_cltii_test_status_cnt; /* In fixture mode, set the number of times that the II indicator is lit. */
    td_u8 led_set_ndm_test_status_cnt;   /* In tooling mode, set the indicator of the meter reader to turn on. */
} diag_cmd_led_status;
/* Inter-layer message START
 * MAC protocol stack         Inter-layer message ID  (0x1000~0x5000)
 * Scope [0x1000~0x5000]
 * 1, Networking service message [0x1000~0x1400) 1024
 * 2, Route [0x1400~0x1800) 1024
 * 3, transmitter [0x1800~0x1C00) 1024
 * 4, segmenter [0x1C00~0x1E00) 512
 * 5, reassembler [0x1E00~0x2000) 512
 * 6, beacon [0x2000~0x2200) 512
 * 7, conv [0x2200~0x2400) 512
 * 8, RX [0x2400~0x2600) 512
 * 9, NTB [0x2600~0x2700) 256
 * 10, SOUNDING [0x2700~0x2800) 256
 * 11, reader [0x2800~0x2900) 256
 * 12, DFX [0X2900~0X2A00) 256
 * 13, FA [0x2A00~0x2B00]
 * 14, Reserved [0x2B00~0x5000)
 * The ID format is as follows: EXT_DMID_PS_XXX_XXX
 * Structure: DMID_PS_XXX_XXX_S
 */
/* 1, Networking service message [0x1000~0x1400) 1024 */
#define ID_DIAG_LAYER_PS_NM_IGNORE 0x1000
#define ID_DIAG_LAYER_PS_NM_FORMING 0x1001
#define ID_DIAG_LAYER_PS_NM_COORDINATE 0x1002
#define ID_DIAG_LAYER_PS_NM_CHANGEPROXY 0x1003
#define EXT_DMID_MAC_STA_REJOIN_TIME_NO_OK 0x1004
#define EXT_DMID_MAC_GET_MAC_BY_TEI 0x1005
#define EXT_DMID_MAC_GET_LEVEL_BY_TEI 0x1006
#define EXT_DMID_MAC_GET_NEXT_HOP 0x1007
#define EXT_DMID_MAC_ADD_ROUTE_ITEM 0x1008
#define EXT_DMID_MAC_ADD_ROUTE_ITEM_BY_PROXY 0x1009
#define EXT_DMID_MAC_STA_SEND_HEART_BEAT 0x100B
typedef struct {
    td_u16 tei;
    td_u16 proxy_tei;
    td_u32 bpc;
} ext_dmid_mac_sta_send_heart_beat;
typedef struct {
    td_u16 tei;
    td_u16 proxy_tei;
    td_u32 bpc;
} ext_dmid_mac_cco_rcv_heart_beat;
typedef struct {
    td_u32 assoc_last_bpc;
    td_u8 req_send_bpc_interval;
    td_u8 rsvd[3]; /* Reserved 3 bytes */
    td_u32 curent_bpc;
} ext_dmid_mac_sta_rejoin_time_no_ok;
typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN];
    td_u16 tei;
} ext_dmid_mac_get_mac_by_tei;
typedef struct {
    td_u8 level;
    td_u8 rsvd;
    td_u16 tei;
} ext_dmid_mac_get_level_by_tei;

/* 2, Route [0x1400~0x1800)  1024 */
#define EXT_DMID_RT_LAYER_BASE 0x1400
#define EXT_DMID_RT_PATH_TABLE (EXT_DMID_RT_LAYER_BASE + 0x00)
#define ID_DIAG_LAYER_PS_ROUTE_AGING 0x1401
#define ID_DIAG_LAYER_PS_ROUTE_FEEDBACK 0x1402
#define ID_DIAG_LAYER_PS_ROUTE_DETECT 0x1403
#define ID_DIAG_LAYER_PS_ROUTE_EVALUATE 0x1404
/* Report structure of the path information table */

/* 3 Transmitter [0x1800~0x1C00)  1024
 * Message definition of the sender module
 */
#define EXT_DMID_MAC_TX_RECEIVE_EVENT 0x1800
#define EXT_DMID_MAC_TX_RCTS_PACKET_GENERATE 0x1801
#define EXT_DMID_MAC_TX_BEACON_MPDU_INFO 0x1802
#define EXT_DMID_MAC_TX_MPDU_INFO 0x1803
#define EXT_DMID_MAC_TX_TDMA_PERIODINFO 0x1804
#define EXT_DMID_MAC_TX_TIMER_SET_INFO 0x1805
#define EXT_DMID_MAC_TX_WAIT_FRAME_SET_INFO 0x1806
#define EXT_DMID_MAC_TX_SEND_ENABLE_INFO 0x1807
#define EXT_DMID_MAC_TX_MPDU_SYMBOL_NUM_INFO 0x1808
#define EXT_DMID_MAC_TX_TONEMAP_QUERY_INFO 0x1809
#define EXT_DMID_MAC_TX_MPDU_TMI_INFO 0x1810
#define EXT_DMID_MAC_TX_PHASE_SWITCH_INFO 0x1811
#define EXT_DMID_MAC_TX_GET_MPDU_FL_INFO 0x1812

/* 4. Segmenter     [0x1C00~0x1E00)  512
 * 5, reassembler    [0x1E00~0x2000)  512
 * 6, beacon           [0x2000~0x2200)  512
 * 7, conv              [0x2200~0x2400)  512
 */
#define EXT_DMID_MAC_SEND_MSDU_TO_NM 0x2200
#define EXT_DMID_MAC_ROUTE_LEARNIG 0x2201
#define EXT_DMID_MAC_GET_TEI_FROM_TOPO 0x2202
#define EXT_DMID_MAC_GET_TEI_FROM_PATH_TABLE 0x2203
typedef struct {
    td_u32 bpc;
    td_u32 ntb;
    td_u32 msdu_size;
} ext_dmid_mac_send_msdu_to_nm;

#define EXT_DMID_MAC_RX_RCV_FC 0x2400
#define EXT_DMID_MAC_RX_BEACON_STATUS 0x2401
#define EXT_DMID_MAC_RX_SOF_STATUS 0x2402

/* 9, NTB            [0x2600~0x2700)  256
 * 10, SOUNDING      [0x2700~0x2800)  256
 * 11, reader        [0x2800~0x2900)  256
 */
#define EXT_DMID_MAC_DCM_RCV_REMOTE_CMD_INFO 0x2800
#define EXT_DMID_MAC_DCM_PIPE_TRANS_PK_INFO 0x2801
#define EXT_DMID_MAC_DCM_PIPE_REPLY_ACK_INFO 0x2802
#define EXT_DMID_MAC_DCM_PIPE_PK_ARRIVED_DEST_STA_INFO 0x2803
#define EXT_DMID_MAC_DCM_PIPE_RECV_PK_FROM_PLC_INFO 0x2804
#define EXT_DMID_MAC_DCM_SEG_QUEUE_IN_FAIL_INFO 0x2805
#define EXT_DMID_MAC_DCM_TX_SUCCESS_INFO 0x2806
#define EXT_DMID_MAC_DCM_TX_FAIL_INFO 0x2807
#define EXT_DMID_MAC_DCM_TX_TIMEOUT_INFO 0x2808
#define EXT_DMID_MAC_DCM_RSB_TIMEOUT_INFO 0x2809
#define EXT_DMID_MAC_BUFF_DISP_INFO 0x280a
/* Receives remote commands from the DBK through the NCR channel. */
typedef struct {
    td_u8 role;                             /* CCO ,STA */
    td_u8 mac_addr[4][EXT_PLC_MAC_ADDR_LEN]; /* MAC Address table contains 4 MAC addresses at maximum */
    td_u8 ucchannel_type;
    td_u16 control : 15; /* Control word */
    td_u16 du_link : 1;  /* Uplink and downlink flags */
    td_u16 command_id;
    td_u16 trans_id;
    td_u16 payload_size;
    td_u16 pad;
    td_u32 sn;
} ext_dmid_mac_dcm_rcv_remote_cmd_info;
#define DFX_BUFF_DISP_SIZE_MAX 64
typedef struct {
    td_u16 idx;
    td_u16 size;
    td_u32 buff[DFX_BUFF_DISP_SIZE_MAX >> 2]; /* Right shift 2 bits */
} ext_dmid_mac_buff_disp_info;

#define FL_FOLDER_AND_FILE_NAME_LEN 25
#define FL_MAX_TEI 1016
/* Issue, start, stop, and query the DBK fault location command. */
typedef enum {
    EXT_MAC_NOTIFY_START_FL_CMD = 1,    /* Instructs the to enable the fault locating function. */
    EXT_MAC_NOTIFY_STOP_FL_CMD,         /* Instructs the to stop the fault locating function. */
    EXT_MAC_NOTIFY_QUERY_FL_STATUS_CMD, /* Query the current fault location status. */
} diag_cmd_set_fault_locat_enum;
/* Fault location Current running status */
typedef enum {
    EXT_MAC_FAULT_LOCAT_STOP, /* The positioning is complete. */
    EXT_MAC_FAULT_LOCAT_ON,   /* The fault is being located. */
} diag_cmd_fault_locat_status_enum;
/* Fault location version number */
typedef enum {
    FAULT_LOCAT_ERROR = 0,
    FAULT_LOCAT_VER1,
    FAULT_LOCAT_VER2,
    FAULT_LOCAT_VER3,
} diag_mac_fault_locat_ver_enum;
/* Error code for starting fault location */
typedef enum {
    EXT_MAC_FAULT_LOCAT_ERR_SUCCESS = EXT_ERR_SUCCESS,
    EXT_MAC_FAULT_LOCAT_ERR_DISABLE = 1,      /* The current meter reading controller
                                                cannot be started for fault locating. */
    EXT_MAC_FAULT_LOCAT_ERR_NOT_SUPPORT,      /* Fault locating is not supported in the current NCR mode. */
    EXT_MAC_FAULT_LOCAT_ERR_ALREADY_START,    /* The fault is being located. */
    EXT_MAC_FAULT_LOCAT_ERR_ALREADY_STOP,     /* The positioning has been stopped. */
    EXT_MAC_FAULT_LOCAT_ERR_MAC,              /* The delivered MAC parameter is invalid. */
    EXT_MAC_FAULT_LOCAT_ERR_FREQ,             /* The delivered Freq parameter is invalid. */
    EXT_MAC_FAULT_LOCAT_ERR_SNID,             /* The delivered SNID parameter is invalid. */
    EXT_MAC_FAULT_LOCAT_ERR_FOLDER_NAME,      /* The delivered file name is invalid. */
    EXT_MAC_FAULT_LOCAT_ERR_SD_NO_EXIST,      /* The SD card does not exist. */
    EXT_MAC_FAULT_LOCAT_ERR_SD_NO_MORE_SPACE, /* The remaining space of the SD card is insufficient. */
    EXT_MAC_FAULT_LOCAT_ERR_SD_SAME_NAME,     /* The file name in the SD card is duplicate. */
} diag_mac_fault_locat_err_enum;
/* Unlock response */
typedef struct {
    td_u32 ret; /* If 0 is returned, the operation is successful. */
} diag_cmd_unlock_network_ind;
/* 12, DFX [0X2900-0X2A00) 256 */
/* Inter-layer message */
typedef struct {
    td_u16 tei : 12;
    td_u16 network_mode : 1;
    td_u16 rsvd : 3;
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN];
} diag_cmd_query_dual_module_entry;
typedef struct {
    td_u8 cco_mac[EXT_PLC_MAC_ADDR_LEN]; /* MAC address of the CCO to be added to the network */
    td_u8 mac_type;                     /* MAC address type */
    td_u8 is_dual_module;               /* Whether the module is a dual-mode module */
    td_u8 main_network_snid;   /* Main network code (main network code carried in the payload of the beacon frame) */
    td_u8 rf_network_snid;     /* Main network ID of the RF site */
    td_u8 sub_network_snid;    /* RF sub-network number */
    td_u8 network_mode;        /* Network mode */
    td_u8 network_mode_policy; /* Network mode policy */
    td_u8 pad[3];              /* Reserved 3 bytes */
} diag_cmd_query_sta_info;
typedef struct {
    td_u16 tei;                /* TEI of the site */
    td_u16 proxy_tei;          /* Proxy site TEI */
    td_u16 counnected_sta_num; /* Number of accessible sites */
    td_u16 pad;
    td_u32 connected_graph[32]; /* Connection diagram is of 32 words */
} diag_cmd_query_connected_graph_ind;
typedef struct {
    td_u8 comm_rate_th;             /* Communication rate threshold */
    td_u8 comm_rate_is_enable;      /* communication rate threshold */
    td_s8 snr_th;                   /* Average SNR threshold */
    td_u8 snr_is_enable;            /* Indicates whether to enable the average SNR threshold. */
    td_u8 down_comm_rate_th;        /* Downlink traffic rate threshold */
    td_u8 down_comm_rate_is_enable; /* downlink communication rate threshold */
    td_u8 up_comm_rate_th;          /* Uplink communication rate threshold */
    td_u8 up_comm_rate_is_enable;   /* uplink communication rate threshold */
} diag_cmd_query_connected_graph_req;
/* The STA sends the duty cycle information that can be maintained and measured. */
typedef struct {
    td_u8 duty_start_cnt;
    td_u8 duty_stop_cnt;
    td_u8 start_fail_cnt;
    td_bool is_duty_on;
    td_u32 max_sendlength; /* Maximum transmission duration of the delivered duty cycle (unit: us) */
    td_u8 duty_percent;    /* Delivered duty cycle */
    td_u8 pad[3];          /* Reserved 3 bytes */
    td_u32 remain_time_not_enough_cnt;
    td_u32 sack_duty_cnt;
    td_u32 sound_sack_duty_cnt;
    td_u32 csma_duty_cnt;
    td_u32 fresh_record_cnt;
    td_u32 max_period1_duty_percent; /* Maximum duty cycle in period 1 */
    td_u32 cur_period1_duty_percent; /* Current duty cycle in period 1 */
    td_u32 max_period2_duty_percent; /* Maximum duty cycle in period 2 */
    td_u32 cur_period2_duty_percent; /* Current duty cycle in period 2 */
    td_u32 max_period3_duty_percent; /* Maximum duty cycle in period 3 */
    td_u32 cur_period3_duty_percent; /* Current duty cycle in period 3 */
} diag_cmd_tx_duty_statics_stru;
#define DFX_MAX_IDENTIFY_RESULT_ENTRY_NUM 15
typedef struct {
    td_u32 freq : 7;                    /* Band number */
    td_u32 snid : 24;                   /* Network ID */
    td_u32 is_valid : 1;                /* Whether the data is valid */
    td_u8 pad0[2];                      /* Reserved 2 bytes */
    td_u8 cco_mac[EXT_PLC_MAC_ADDR_LEN]; /* CCO MAC address */
    td_u16 signal_mean;                 /* Mean signal value */
    td_u16 noise_mean;                  /* Average noise value */
    td_u16 soft_val;                    /* Soft value */
    td_u8 seq_type;                     /* Codeword type */
    td_u8 pad;                          /* Reserved */
    td_u32 last_refresh_time;           /* Last refresh time, in seconds */
} diag_cmd_his_result_entry_stru;
typedef struct {
    diag_cmd_his_result_entry_stru max; /* historical maximum disturbance */
    diag_cmd_his_result_entry_stru entry[DFX_MAX_IDENTIFY_RESULT_ENTRY_NUM];
} diag_cmd_his_result_cache;
/* Current recognition result */
typedef struct {
    td_u32 freq : 7;                    /* Band number */
    td_u32 snid : 24;                   /* Network ID */
    td_u32 is_valid : 1;                /* Whether the data is valid */
    td_u8 pad0[2];                      /* Reserved 2 bytes */
    td_u8 cco_mac[EXT_PLC_MAC_ADDR_LEN]; /* CCO_MAC */
    td_u16 signal_mean;                 /* Mean signal value */
    td_u16 noise_mean;                  /* Average noise value */
    td_u16 soft_val;                    /* Soft value */
    td_u8 seq_type;                     /* Codeword type */
    td_u8 pad;                          /* Reserved */
    td_u32 seq_refresh_time;            /* Last refresh time of the sequence (unit: s) */
    td_u32 bcn_refresh_time;            /* Last update time of the beacon, in seconds */
} diag_cmd_cur_result_entry_stru;
typedef struct {
    td_u32 last_seq_time; /* Identification of the last received disturbance (unit: s) */
    diag_cmd_cur_result_entry_stru entry[DFX_MAX_IDENTIFY_RESULT_ENTRY_NUM];
} diag_cmd_cur_result_cache;

/* Maintenance and test information of energy applications */
typedef struct {
    td_u8 is_energy_mode : 1;
    td_u8 is_opening : 1;
    td_u8 is_cco_no_send_bcn : 1;
    td_u8 is_chip_detecting : 1;
    td_u8 is_full : 1;
    td_u8 is_white_list_empty : 1;
    td_u8 is_unlock_rejoin_enable : 1; /* whether to enable the function of unlocking offline users in full load */
    td_u8 pad1 : 1;
    td_u8 sta_rejoin_by_unlock_cnt; /* The STA station is not locked because it is fully loaded.
                                     * Therefore, the number of warm resets is
                                     */
    td_u16 open_cnt;
    td_u16 stop_cnt;
    td_u16 detect_chip_ver;
    td_u16 clear_detect_chip_ver_cnt;
    td_u32 network_form_time;
    td_u32 network_form_max_time;
    td_u32 reject_by_no_register_cnt; /* The CCO rejects the site access request because
                                       * the CCO-based authentication does not take effect in the energy version.
                                       */
} diag_cmd_query_energy_statics_stru;
typedef struct {
    td_u16 send_pb_cnt; /* Number of transmitted PB blocks */
    td_u16 send_fc_cnt; /* Number of transmitted FCs */
    td_u32 send_pb_ntb; /* ntb at the time point when the PB is sent */
    td_u32 send_fc_ntb; /* ntb at the time of fc transmission. */
    td_u16 rcv_pb_cnt;  /* Number of received PB blocks */
    td_u16 rcv_fc_cnt;  /* Indicates the number of received FCs. */
    td_u32 rcv_pb_ntb;  /* ntb at the time when the PB is received */
    td_u32 rcv_fc_ntb;  /* receives fc at the fc moment. */
} diag_cmd_fast_send;
/* Query the statistics of the neighboring node. */
typedef struct {
    td_u8 index;               /* When the number of neighboring nodes is greater than 16,
                                * this parameter is used to display nodes in groups.
                                */
    td_u8 filter_by_level : 1; /* mode 1: Filter by level */
    td_u8 level : 7;
    td_u16 tei;         /* Method 2: Filter by TEI. */
    td_u32 join_snid;   /* Method 3: Select by joining the network. The parameters include join_cco_mac. */
    td_u32 belong_snid; /* Method 4: Select based on the home network. The parameters include belong_cco_mac. */
    td_u8 join_cco_mac[EXT_PLC_MAC_ADDR_LEN];
    td_u8 belong_cco_mac[EXT_PLC_MAC_ADDR_LEN];
} diag_cmd_get_top_neighbor_req;
/* Neighbor node statistics */
typedef struct {
    td_u32 join_snid; /* NID of the network to be added. */
    td_u16 tei;       /* Site TEI */
    td_u8 level;      /* Topology level */
    td_u8 pad;
    td_u16 avg_att; /* Attenuation filtering value */
    td_s16 avg_snr; /* Average SNR value */
    td_u32 fc_cnt;  /* Number of FCs received in this period */
} diag_neighbor_info;
#define DIAG_TOP_NEIGHBOR_QUERY_NUM 16 /* Number of returned neighbor node information query results */
/* Response to the request for querying the statistics of a neighboring node */
typedef struct {
    td_u32 ret; /* Query result. If 0 is returned, the operation is successful. */
    diag_neighbor_info results[DIAG_TOP_NEIGHBOR_QUERY_NUM]; /* neighbor node information */
} diag_cmd_get_top_neighbor_ind;

/* Node search IND structure */
typedef struct {
    td_u32 ret;
} diag_cmd_node_find_ind;
/* Node lookup REQ structure */
typedef struct {
    td_u8 mode;                              /* Search mode. For the definition, see NM_NODE_FIND_MODE_XX */
    td_u8 find_duration;                     /* Search duration, in minutes. */
    td_u8 mac_num;                           /* In list mode, query the number of sites. */
    td_u8 lock_duration;                     /* Lock duration, in minutes. */
    td_u32 cycle_send_duration;              /* Periodic sending duration, in seconds */
    td_u8 mac_list[NODE_FIND_MAC_LIST_SIZE]; /* MAC address list to be queried */
} diag_cmd_node_find_req;
/* Application version information */
typedef struct {
    td_u32 ret;
} diag_app_ver_cmd_ind_result;

#define DIAG_RPT_SIGNAL_QULITY_NUM 5
typedef struct {
    td_u16 my_tei;
    td_u16 my_proxy_tei;
    td_u8 snr_to_proxy;
    td_u8 atten_to_proxy;
    td_u16 pad;
} diag_cmd_query_sta_signal_qulity_to_proxy;

typedef struct {
    td_u8 tei_num;
    td_u8 pad;
    td_u16 tei_list[DIAG_RPT_SIGNAL_QULITY_NUM];
} diag_cmd_query_sta_signal_qulity_req;

typedef struct {
    td_u32 sn;
    td_u16 sta_num;
    td_u8  is_last;
    td_u8 pad;
    diag_cmd_query_sta_signal_qulity_to_proxy entry[DIAG_RPT_SIGNAL_QULITY_NUM];
} diag_cmd_report_signal_qulity_of_proxy;

typedef struct {
    td_u8 is_smart_select_en;
    td_u8 trigger_bcn_atten;
    td_u8 is_in_auth;
    td_u8 pad;
    td_u32 trigger_snid;
    td_u16 enble_by_app_cnt;
    td_u16 disable_by_app_cnt;
    td_u32 recent_start_time;
    td_u32 last_end_time;
} dfx_cmd_query_smart_select_info;

typedef struct {
    td_bool is_lock_topo_enable;
    td_bool is_trans_opti_enable;
    td_bool is_exist_old;
    td_bool pad;
    td_u32 rt_cycle_cnt;
    td_u16 rt_cycle_length;
    td_u16 rt_remain_time;
    td_u16 new_sta_join_cycle_left;
    td_u16 app_set_cycle;
} dfx_cmd_cco_rt_opti_info;
#ifdef __cplusplus
}
#endif
#endif /* DFX_H */
