/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description:   System Level Diagnostic Interface Definition
 * Author: CompanyName
 * Create: 2018-10-31
 */

#ifndef DFX_SYS_SDM_H
#define DFX_SYS_SDM_H

#include <soc_ft_nv.h>
#include "soc_types.h"
#include "soc_mdm_types.h"
#include "soc_phy_nv.h"

EXT_START_HEADER
/* SDM interface */
#define NM_STAT_SOFT_RESET_INFO_NUM (5) /* Count the maximum number of soft restarts, over the range of cycles */

#define EXT_SDM_FA_EVALUATE_RESULT_FREQ_NUM_MAX 3 /* Maximum number of bands stored in the band evaluation */

typedef struct {
    td_u32 addr;
} ext_sdm_klg_macbuffer;

typedef struct {
    td_u32 snid; /* snid before restart */

    td_u8 soft_reset_reason; /* mac soft restart reason */
    td_u8 freq;              /* get the band before restarting */
    td_u8 pad[2];            /* used to record more detailed measurable, pad 2 byte */
    td_u32 reset_sys_time;   /* mac soft restart event */
} diag_cmd_soft_reset_info_stru;

typedef struct {
    td_u8 index;  /* Soft restart count */
    td_u8 pad[3]; /* pad 3 byte */
    diag_cmd_soft_reset_info_stru soft_reset_info[NM_STAT_SOFT_RESET_INFO_NUM];
} diag_cmd_soft_reset;

typedef struct {
    td_u32 last_fail_reason;
    td_u16 call_total_times;
    td_u16 call_fail_times;
} ext_diag_sync_stat_s;

/* ***************************************************************************** */
typedef struct {
    td_u32 last_fail_reason;
    td_u16 call_total_times;
    td_u16 call_fail_times;
} ext_diag_sync_stat;

/* ***************************************************************************** */
/* System level key information saved to Flash */
/* ***************************************************************************** */
typedef struct {
    td_u32 cnt[10];  /* 10 count */
    td_u32 size[10]; /* 10 count */
} ext_sys_dfx_nm_err_size_log;

typedef struct {
    td_u32 magic;
    td_u32 boot_cnt;
    td_u32 sys_upg_start;
    td_u32 sys_upg_stop; /* number of stops */
    td_u32 nv_init_phase1;
    td_u32 nv_init_phase2;

    td_u8 sys_upg_update;
    td_u8 sys_upg_sta_update; /* dai shuo */
    td_u8 sys_upg_crc_ok;
    td_u8 sys_upg_finish; /* before restart */

    td_u8 sys_upg_ok; /* after reboot */
    td_u8 application_init;
    td_u8 mac_close_led;
    td_u8 wdg_enable;

    td_u8 init_end;
    td_u8 mac_rsb_task_init_ok;
    td_u8 mac_tx_task_init_ok;
    td_u8 mac_cs_task_init_ok;

    td_u8 mac_nm_msg_task_init_ok;
    td_u8 mac_nm_evt_task_init_ok;
    td_u8 mac_dfx_task_init_ok;
    td_u8 temp;
} ext_sdm_klg_init;

typedef enum {
    EXT_RUNSTATUS_10 = 10,  /* 10 Receive PLC */
    EXT_RUNSTATUS_20 = 20,  /* 20 Processing PLC messages */
    EXT_RUNSTATUS_30 = 30,  /* 30 into the queue */
    EXT_RUNSTATUS_40 = 40,  /* 40 processing queue */
    EXT_RUNSTATUS_50 = 50,  /* 50 send serial message */
    EXT_RUNSTATUS_60 = 60,  /* 60 processing serial port to send messages */
    EXT_RUNSTATUS_70 = 70,  /* 70 Received a serial message and sent it */
    EXT_RUNSTATUS_80 = 80,  /* 80 processing serial port to receive messages */
    EXT_RUNSTATUS_90 = 90,  /* 90 send PLC message to the queue */
    EXT_RUNSTATUS_100 = 100 /* 100 response message, send PLC */
} ext_app_runstatus;

typedef struct {
    td_u32 magic;
    td_u32 is_net_connected; /* is it networked? */
    td_u32 run_status;       /* current running status */
    td_u32 run_time_stamp;   /* run state refresh time */

    /* plc path */
    td_u32 plc_msg_id;         /* the type, length, and time of the last received message */
    td_u32 plc_msg_len;        /* the length of the last received message */
    td_u32 plc_msg_time_stamp; /* the last received message moment */
    td_u32 plc_total_rx_blk;   /* frame count received by the plc */

    /* uart path */
    td_u32 uart_msg_len;        /* the length of the last received message */
    td_u32 uart_msg_time_stamp; /* the last received message moment */

    /* failed to send to plc */
    td_u32 plc_send_failed_cnt;
    td_u32 plc_send_msg_failed_cnt;
    /* sending a serial port failed */
    td_u32 uart_send_failed_cnt;
    td_u32 uart_send_msg_failed_cnt;

    /* the current number of message queue elements */
    td_u32 msg_queen_cnt;
} ext_sdm_klg_app;

#if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER) || defined(PRODUCT_CFG_DBK)

/* 0xB130-0xB139 meter reading measurable use */
#define EXT_DSID_SDM_MRS_BASE 0xB130
#define EXT_DSID_SDM_MLG_LRPR 0xB130
#define EXT_DSID_SDM_MLG_LOW_RATIO 0xB131
#define EXT_DSID_SDM_MLG_XR 0xB132
#define EXT_DSID_SDM_MLG_DAY_FREEZE 0xB133
#define EXT_DSID_SDM_MRS_END 0xB133

#define EXT_DSID_SDM_FA 0xB135
#define EXT_DSID_SDM_BAT 0xB136

#define EXT_DSID_SDM_KLG_DUMP 0xB150 /* EXT_SDM_KLG_DUMP_S */
#define EXT_DSID_SDM_KLG_SYS 0xB151  /* EXT_SDM_KLG_SYS_S */
#define EXT_DSID_SDM_KLG_INIT 0xB152 /* EXT_SDM_KLG_INIT_S */
#define EXT_DSID_SDM_KLG_APP 0xB153  /* EXT_SDM_KLG_APP_S */
#define EXT_DSID_SDM_KLG_OS 0xB154   /* EXT_SDM_KLG_OS_S */
#define EXT_DSID_SDM_KLG_SYSERR 0xB155
#define EXT_DSID_SDM_KLG_ALL 0xB156
/* ***************************************************************************** */
/* sdm test define */
/* ***************************************************************************** */
#define EXT_DSID_SDM_TEST_BASE 0xB160
#define EXT_DSID_SDM_TEST_SEVT 0xB161
#define EXT_DSID_SDM_TEST_REVT 0xB162
/* ....Reserved */
#define EXT_DSID_SDM_TEST_END 0xB169
/* ***************************************************************************** */
/* ***************************************************************************** */
typedef struct {
    td_u32 ret;
} ext_sdm_erase_ram_ind;

/* magic num */
#define MAGIC_NUM_PERIOD 0x5245504d /* MPER */
#define MAGIC_NUM_EVT 0x54564553    /* SEVT */
#define MAGIC_NUM_MRS 0x53524d4d    /* MMRS */
#define MAGIC_NUM_FA 0x3141464D     /* MFA1 */
#define MAGIC_NUM_BAT 0x5441424D    /* MBAT */
#define MAGIC_NUM_NMLV 0x4E4d4c56   /* NMLV */

typedef struct {
    td_u32 id;
    td_u32 hex[6]; /* 6 count */
    td_u32 decimal_0;
    td_u32 decimal_1;
    td_u32 decimal_2;
    td_u32 decimal_3;
    td_u32 decimal_4;
    td_u32 decimal_5;
} ext_dsid_sdm_test; /* TEST command */

typedef struct {
    td_u32 idx;
    td_u32 magic;
    td_u32 sec : 6;
    td_u32 min : 6;
    td_u32 hour : 5;
    td_u32 day : 5;
    td_u32 month : 4;
    td_u32 year : 6;
    td_u32 macbpc;
} ext_sdm_eng_storage;

typedef enum {
    SDM_BSP = 0, /* EXT_DSID_SDM_BSP_S */
    SDM_CHL,     /* EXT_DSID_SDM_CHL_S */
    SDM_RX,      /* EXT_DSID_SDM_RX_S */
    SDM_TX,      /* EXT_DSID_SDM_TX_S */
    SDM_BCN,     /* EXT_DSID_SDM_BCN_S */
    SDM_NM,      /* EXT_DSID_SDM_NM_S */
    SDM_RT,      /* EXT_DSID_SDM_RT_S */
    SDM_MR = 7,  /* EXT_DSID_SDM_MR_S */
    SDM_EVT,     /* EXT_SDM_ENG_EVT_S */

    /* Meter reading related data */
    SDM_MRS_LRPR_STATS = 100,
    SDM_MRS_LOW_RATIO,
    SDM_MRS_XR_STATS,
    SDM_MRS_DAY_FREEZE_STATS,
    SDM_MRS_ALL,

    SDM_FA = 120,
    SDM_BAT = 130,
} ext_sdm_cmd_id; /* Size is limited to 0-255 */
/* ***************************************************************************** */
typedef enum {
    EXT_SDM_ENG_EVT_VT_ID_NORMAL = 0,
    EXT_SDM_ENG_EVT_VT_ID_WIFI_OFF,
    EXT_SDM_ENG_EVT_VT_ID_WIFI_ON,
    EXT_SDM_ENG_EVT_VT_ID_SD_DC_START,

    EXT_SDM_ENG_EVT_VT_ID_NAC_ETH = 32,
    EXT_SDM_ENG_EVT_VT_ID_NAC_UART,
    EXT_SDM_ENG_EVT_VT_ID_NAC_UART_RX,
    EXT_SDM_ENG_EVT_VT_ID_NAC_BQ_UPG,
    EXT_SDM_ENG_EVT_VT_ID_NAC_SD_CARD,
    EXT_SDM_ENG_EVT_VT_ID_NAC_PLC_SENSITIVE,
    EXT_SDM_ENG_EVT_VT_ID_NAC_KEY0, /* Number button */
    EXT_SDM_ENG_EVT_VT_ID_NAC_WRITE_BACK_NV,
} ext_sdm_eng_evt_vt_id;

typedef struct {
    ext_sdm_eng_evt_vt_id id;
    td_u32 data[3]; /* Replace the original ulTemperature,ulVoltage,reserve,3 count */
} ext_sdm_eng_evt_vt;

typedef struct {
    ext_sdm_eng_evt_vt_id id;
    td_u8 flag;
    td_u8 err_code;
    td_u16 reserved;
    td_u32 data[2]; /* pad 2 byte */
} ext_sdm_eng_evt_nac;
typedef struct {
    td_u8 bq_ver_old; /* the version number in the bq file */
    td_u8 bq_ver_new; /* read the version number in the bq chip after the upgrade */
    td_u16 reserved;
    td_u32 ret;             /* EXT_ERR_XXX */
} ext_sdm_eng_evt_nac_bqupg; /* measurable information BQ upgrade private / / default value: 0x0000a1a1 */
typedef struct {
    td_u8 rx_data_num; /* uart check success times */
    td_u8 reserved1;
    td_u16 reserved2;
    td_u32 ret;            /* uart test error code ext_err_xxx */
} ext_sdm_eng_evt_nac_uart; /* flash measurable private structure (serial port) default value: 0x00000003 */
typedef struct {
    ext_sdm_eng_evt_vt_id id;
    td_char rx_dada1[4];      /* 4 count */
    td_char rx_dada2[4];      /* 4 count */
    td_char rx_dada3[4];      /* 4 count */
} ext_sdm_eng_evt_nac_uart_rx; /* default value: 0x63614e23(#Nac) 0x63614e23 0x63614e23 */
typedef struct {
    td_char rx_data[4]; /* 4 count */
    td_u32 ret;
} ext_sdm_eng_evt_nac_sdcard; /* default value: 0x54534554 */
typedef struct {
    td_u32 rx_beacon_num; /* number of beacons received (as the basis for assignment of bisbeacon) */
    td_u8 is_beacon;      /* mac network status td_true can continue testing, td_false wait for the button to exit */
    td_u8 tx_fail_number; /* number of failed transmissions */
    td_u8 tx_number;      /* number of times sent */
    td_u8 reserved;
} ext_sdm_eng_evt_nac_plc; /* default value: 0x0000000n  0x00nn0001 */
typedef struct {
    ext_sdm_eng_evt_vt_id id;
    td_u8 is_key_interrupt;
    td_u8 is_responce_key;
    td_u8 is_press_down;
    td_u8 is_press_up;
    td_u32 data[2];        /* pad 2 byte */
} ext_sdm_eng_evt_nac_key0; /* default value: 0x01000101 */
/* ***************************************************************************** */
/* ***************************************************************************** */
/* storage access id */
typedef enum {
    SDM_KLG_FLASH_B0 = 0, /* 'crash' SDM_KLG flash storage region. */
    SDM_KLG_FLASH_B1,     /* 'period' SDM_KLG flash storage region. */
    SDM_KLG_FLASH_B2,     /* 'initializtion' SDM_KLG flash storage region. */
    SDM_KLG_FLASH_B3,     /* 'reboot by user(NOT rst mgr)' SDM_KLG flash storage region. */
    SDM_KLG_RAM = 8       /* SDM KLG in RAM. */
} ext_sdm_klg_sa_id;

typedef enum {
    SDM_KLG_SYS = 0,
    SDM_KLG_INIT,
    SDM_KLG_APP,
    SDM_KLG_OS,
    SDM_KLG_SYSERR
} ext_sdm_klg_id;

typedef struct {
    ext_sdm_klg_sa_id said;
    ext_sdm_klg_id scid;
    td_u32 enable_sdm_klg; /* The current SDM KLG function is enabled, and the SDM KLG is disabled by default */
    /* after the EXT_DSID_SDM_KLG_DUMP command is received. */
} ext_sdm_klg_dump;

/* SDM task internal use structure: put this only for DBK parsing */
typedef struct {
    td_u32 aul_data[EXT_SYS_MSG_PARAM_NUM_MAX_AULDATA];
} ext_sdm_eng_evt_prv;

EXT_EXTERN td_u32 UAPI_SDM_SaveCycleEvent(ext_sdm_eng_evt_vt *evt, td_bool be_sync);

#if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER) || defined(PRODUCT_CFG_DBK)

/* cmd: EXT_DSID_SDM_DUMP REQ */
typedef struct {
    td_u16 check_sum;
    td_u16 prod_type;
    td_u8 sdm_id;
    td_u8 opt;
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN]; /* target mac address when exporting remotely */
    td_u32 addr;                    /* flash start address of the exported content */
    td_u32 size;                    /* the size of the exported content, all ff means to clear the corresponding area */
} ext_sdm_data_dump_req;

typedef struct {
    td_u16 check_sum;
    td_u16 rc;         /* error code */
    td_u32 addr;       /* the response packet content corresponds to the flash address */
    td_u32 size;       /* should defeat the content corresponding to the flash size */
    td_u32 finish : 1; /* export completed */
    td_u32 reserved : 31;
    td_u8 data[0]; /* data */
} ext_sdm_data_dump_ind;

typedef struct {
    td_u32 timestamp;
} ext_sdm_klg_os;

typedef enum {
    EXT_SDM_TEST_FRME_SWT_NORMAL = 0,
    EXT_SDM_TEST_FRME_SWT_ON = 0x01,
    EXT_SDM_TEST_FRME_SWT_OFF = 0x02
} ext_sdm_test_frme_swt;

/* need packet feature registration */
typedef struct {
    td_u32 key_event;         /* ext_sdm_eng_evt_cause_e, currently using ext_sdm_eng_evt_cause_mac_discard_msdu */
    td_u32 feature_value[2];  /* the value of the message feature field, which can be set to 2 for the offset. */
    td_u16 feature_offset[2]; /* message feature field offset, offset from msdu header; can be set 2 */
    td_u8 feature_size[2];    /* the size of the message feature field can be set to a maximum of 4, 2 count */
    /* the field offset in the message to be saved, the offset from the msdu header; 2 can be set */
    td_u16 save_offset[2];
    /*
     * the size of the field in the message to be saved, the offset can be set to 2;
     * the sum of the two values ??does not exceed 6 (bytes)
     */
    td_u8 save_size[2]; /* the offset can be set to 2; */
} ext_sdm_reg_feature;
/* ************************************************ ***************************** */
/* ************************************************ ***************************** */
#define EXT_LEAVE_PLC_MAC_ADDR_LEN (EXT_PLC_MAC_ADDR_LEN - 2)
typedef struct {
    /* site mac address (in order to save storage space, the mac address only stores the lower 4 bytes) */
    td_u8 mac[EXT_LEAVE_PLC_MAC_ADDR_LEN];
    /* offline start time (13 concentrator for calendar time, 09 concentrator for system runtime) */
    td_u32 leave_start_time;
    /*
     * offline duration, unit: minute
     * (65535 minutes, can represent 45 days, if the site offline time exceeds 45 days, the storage overflows)
     */
    td_u32 leave_time : 16;
    td_u32 leave_reason : 4;    /* offline reason */
    td_u32 sw_reset_reason : 6; /* soft restart reason */
    td_u32 hw_reset_reason : 6; /* hard restart reason */
} ext_sdm_leave_info;

typedef enum {
    SD_DRV_DFX_API_GET_FIRMWARE,
    SD_DRV_DFX_API_CHECK_EXIST,
    SD_DRV_DFX_API_GET_DISK_SIZE,
    SD_DRV_DFX_API_DISK_FORMAT,
    SD_DRV_DFX_API_GET_DIR_FIRST,
    SD_DRV_DFX_API_GET_DIR_NEXT,
    SD_DRV_DFX_API_MAKE_DIR,
    SD_DRV_DFX_API_ENTER_DIR,
    SD_DRV_DFX_API_BACK_ROOT,
    SD_DRV_DFX_API_MAKE_FILE,
    SD_DRV_DFX_API_REMOVE_FILE,
    SD_DRV_DFX_API_FILE_OPEN,
    SD_DRV_DFX_API_FILE_CLOSE,
    SD_DRV_DFX_API_FILE_GET_LEN,
    SD_DRV_DFX_API_FILE_READ,
    SD_DRV_DFX_API_FILE_CLEAR,
    SD_DRV_DFX_API_FILE_APPEND,
    SD_DRV_DFX_API_NOTIFY_CUR_TIME,
    SD_DRV_DFX_API_NOTIFY_GET_DIR_NUM,
    SD_DRV_DFX_API_DISK_OTHER_MAX,

    SD_DRV_DFX_API_REMOVE_DIR,
    SD_DRV_DFX_API_FILE_SEEK,
    SD_DRV_DFX_API_DIRECTORY_RENAME,
} sd_drv_dfx_api;

typedef struct {
    td_u16 idle_max_wait_time;
    td_u16 ready_max_wait_time;
    td_u8 idle_time_out_times;
    td_u8 ready_time_out_times;
    td_u8 reserver[2]; /* pad 2 byte */
} sd_drv_api_dfx;

/* ***************************************************************************** */
/* ***************************************************************************** */
#define DEAD_CHECK

typedef enum _dfx_pk_size_exception_pos_ {
    NM_HANDLE_ASSOC_REQ_ERR_FUNC = 0,
    NM_HANDLE_ASSOC_CNF_ERR_FUNC,
    NM_CREATE_ASSOC_IND_01_ERR_FUNC,
    NM_CREATE_ASSOC_IND_02_ERR_FUNC,
    NM_CREATE_ASSOC_IND_03_ERR_FUNC,
    NM_CREATE_CHANGE_PROXY_CNF_ERR_FUNC,
    NM_HANDLE_CHANGE_PROXY_CNF_ERR_FUNC,
    NM_CREATE_CHANGE_KEY_IND_ERR_FUNC,
    /* plc_mac_output_NU_ERR_FUNC , */
    DFX_NDM_HANDLE_DBK_CMD_CALLBACK_ERR_FUNC,
    DFX_NDM_PACKET_FRAME_ERR_FUNC,
    DFX_NDM_PACKET_FRAME_COPY_PAYLOAD_LIST_ERR_FUNC,
    DFX_NDM_HANDLE_PACKET_ERR_FUNC,
    DFX_NDM_PIPE_SEND_ACK_BACK_ERR_FUNC,
    DFX_NDM_PIPE_FRAGMENT_OUTPUT_ERR_FUNC,
    DFX_NDM_FILL_REPLY_PAYLOAD_ERR_FUNC,
    /* Ndc_DRemoteSend_ERR_FUNC */
} dfx_pk_size_exception_pos;

#define MAX_PK_EXCEPTION_CNT 15 /* (17-2) */

typedef struct {
    td_u16 type;         /* 0:indication , 1: cnf, 2: req, 3:rsp */
    td_u16 channel_type; /* type of the tracker channel */
    td_u16 cmd_id;       /* command id */
    td_u16 pad;

    td_u32 payload_size; /* message size */
} dfx_ndm_pk_exception_info;
typedef struct {
    td_u16 pk_mmtype; /* message mtype */
    td_u16 ostei;     /* message mmtype */
    td_u16 odtei;     /* message type */
    td_u16 pad;

    td_u32 payload_size; /* message size */
} dfx_nm_pk_exception_info;

typedef union {
    dfx_ndm_pk_exception_info ndm_pk_excpetion_info;
    dfx_nm_pk_exception_info nm_pk_excpetion_info;
} dfx_pk_exception_info;
typedef struct _dfx_mac_check_pk_err_status_ {
    dfx_pk_size_exception_pos err_pos; /* exception function location */
    dfx_pk_exception_info err_pk_info; /* exception message information */
} dfx_mac_check_pk_err_status;

#define FLASH_BLOCK_SIZE 4096
#define SDM_PK_INFO_SIZE sizeof(dfx_mac_check_pk_err_status)
#define MAX_SDM_PK_NUM (FLASH_BLOCK_SIZE / SDM_PK_INFO_SIZE)

typedef struct {
    td_u8 pk_buffer[MAX_SDM_PK_NUM][SDM_PK_INFO_SIZE];
} ext_sdm_mac_err_pk_cache;

/* ***************************************************************************** */
#endif /* PRODUCT_CFG_CORE_TYPE_BOOTLOADER */
/* ***************************************************************************** */
/* ***************************************************************************** */
/* ulSdmId's value can be set as follow */
/* EXT_DSID_SDM_CHL */
/* EXT_DSID_SDM_RX */
/* EXT_DSID_SDM_TX */
/* EXT_DSID_SDM_BCN */
/* EXT_DSID_SDM_NM */
/* EXT_DSID_SDM_MR */
/* EXT_DSID_SDM_KLG_ALL */
/* EXT_DSID_SDM_KLG_SYSERR */
typedef struct {
    td_u32 tx_ok_num;
    td_u32 tx_err_num;
    td_u32 rx_ok_num;
    td_u32 rx_err_num;
} ext_sdm_test_frame_info;

td_u32 UAPI_SDM_SendTestFrame(td_pvoid frame, td_u32 frame_size, td_u8 frame_flag, td_u8 option);
td_u32 UAPI_SDM_GetTestFrameInfo(ext_sdm_test_frame_info *test_frame_info);

#endif /* PRODUCT_CFG_CORE_TYPE_BOOTLOADER */
/* ***************************************************************************** */
/* ***************************************************************************** */
EXT_END_HEADER
#endif /* DFX_SYS_SDM_H */
