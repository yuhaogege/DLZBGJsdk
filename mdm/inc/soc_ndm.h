/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: NDM interface definition, for MDM and DBK only.
 */

/******************************************************************************/
#ifndef __SOC_NDM_H__
#define __SOC_NDM_H__
#ifndef PRODUCT_CFG_DBK
#include <soc_ft_nv.h>
#endif

#if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER) || defined(PRODUCT_CFG_DBK)
#ifdef PRODUCT_CFG_DBK
#include "soc_types.h"
#endif
#include <soc_cmn_nv.h>
#include <soc_config.h>
#include <soc_mdm_upg.h>
/******************************************************************************/
#define EXT_TC_EXECUTE_DOOR 0x6098

typedef struct {
    td_u32 type;
    td_u32 item;
} execute_tc_stru;

/******************************************************************************/
/* Definition of the control package ID */
/* The default range of the operator package is [0x3400, 0x3600), that is, EXT_ND_NDM_START, EXT_ND_NDM_END-1 */
#define EXT_ND_NDM_START EXT_ND_NDM_START_DEFAULT
#define EXT_ND_NDM_END   EXT_ND_NDM_END_DEFAULT

#define EXT_ND_ERR_IND        0x3401
#define EXT_ND_CONNECTED_INFO 0x3402 /* Parameter: , , EXT_ND_CONNECTED_INFO_IND_S */
#define EXT_MAC_FIND_MODULE   0x3407 /* Parameter: , , ,EXT_MAC_FIND_MODULE_IND_S */
#define EXT_ND_DEVICE_INFO    0x3409
#define DBK_QRY_UE_API_VER   0x340A

#define UPG_STATUS       0x3410 /* Query the upgrade status. upg_status_ind */
#define UPG_START        0x3412 /* Parameter: upg_start_req, , upg_start_ind */
#define UPG_STOP         0x3413 /* upg_stop_req */
#define UPG_TRANSMIT_BLK 0x3415 /* Sending the Upgrade File Block: upg_transmit_blk_req,, EXT_UPG_TRANSMIT_BLK_IND_S */

#define ND_MODULE_DETECT        0x3416 /* Querying the upgrade status, , EXT_ND_MODULE_DETECT_IND_S */
#define UPG_STAT_INFO_IND       0x3417 /* UPG_STATUS, , upg_stat_info_ind */
#define UPG_STATUS_REMOTE_QUERY 0x3418 /* remote progress query */
#define UPG_AFFIRM              0x3419 /* This message is sent when the network-wide upgrade is complete and is
                                                               * used to notify each module of restart.
                                                               */
#define UPG_STA_LIST            0x341A /* Send STA List */
#define UPG_EXTEND_INFO         0x341B /* upgrade extended information */
#define UPG_LIST_REPLY          0x341C /* List upgrade response DBK query list */
#define EXT_DBG_MEM_STAT         0x3420 /* Request for reporting memory statistics, , */
#define EXT_DBG_MEM_SUM_STAT_IND 0x3421 /* memory statistics reporting, EXT_DBG_MEM_SUM_STAT_IND_S */
#define EXT_DBG_MEM_MOD_STAT_IND 0x3422 /* memory statistics reporting, EXT_DBG_MEM_MOD_STAT_IND_S */
#define EXT_DBG_STAT_Q           0x3430 /* EXT_DBG_STAT_Q_S,,EXT_DBG_MEM_MOD_STAT_IND_S */
#define EXT_DBG_DUMP_DIAG_ITEMS  0x3431 /* EXT_DBG_DUMP_DIAG_ITEMS_REQ_S,,    : dumpmsg dump flash key message */
#define EXT_DBG_DFX_TASK_TEST    0x3432 /* EXT_DBG_DFX_TASK_TEST_S, , */

/* reserved: 0x3502--0x3509 */
#define EXT_UPG_NV_W               0x3510      /* Parameter: EXT_UPG_NV_W_REQ_S, , */
#define EXT_UPG_NV_R               0x3511      /* Parameter: EXT_UPG_NV_R_REQ_S, , EXT_UPG_NV_IND_S */
#define EXT_UPG_NV_BW              EXT_UPG_NV_W /* Parameter: EXT_UPG_NV_W_REQ_S, , */
#define EXT_UPG_NV_BR              EXT_UPG_NV_R /* Parameter: EXT_UPG_NV_R_REQ_S, , EXT_UPG_NV_IND_S */
#define EXT_UPG_NV_BE_IND          0x3512      /* Parameter: , , EXT_UPG_NV_BE_IND_S */
#define EXT_NDM_BAT_UPG_STATUS_MSG 0x3550      /* Query the upgrade status */
#define EXT_DIAG_DRV_NDM_DEBUG     0x3551      /* Deleted II_NDM_TEST from the official code for debugging. */
#define EXT_DIAG_DRV_BQ_STAT_MSG   0x3552      /* This command is used to query global variables of the BQ module. */
#define EXT_DIAG_DRV_SD_DC_STAT    0x3553
#define EXT_NDM_SD_DC_SET          0x3554 /* "sd_dc_set" */
#define EXT_NDM_SD_DC_QUERY        0x3555 /* "sd_dc_query" */

/* Definition of the control word (15 bits) */
/* Upstream control word */
#define EXT_ND_FRAME_UL_CTRL_STATUS_OK                 0   /* The command request is processed successfully. */
/*
 * The command request is being processed and has
 * been transparently transmitted to the specified trunk.
 */
#define EXT_ND_FRAME_UL_CTRL_STATUS_TRANS              1
#define EXT_ND_FRAME_UL_CTRL_STATUS_FAIL_INVALID_PARAM 100 /* parameter error */

#define EXT_ND_FRAME_STATUS_OK                 EXT_ND_FRAME_UL_CTRL_STATUS_OK
#define EXT_ND_FRAME_STATUS_TRANS              EXT_ND_FRAME_UL_CTRL_STATUS_TRANS
#define EXT_ND_FRAME_STATUS_FAIL_INVALID_PARAM EXT_ND_FRAME_UL_CTRL_STATUS_FAIL_INVALID_PARAM


/*  Downstream control word
 * The MAC determines whether to process the data. If the data is not processed,
 * the MAC transparently transmits the data to other modules.
 * After the data is processed by other modules,
 * the MAC transparently transmits the data to the DBK.
 */
#define EXT_ND_FRAME_DL_CTRL_DEFAULT 0
/*
 * MAC processing is required and does not need to be transparently
 * transmitted to the app. By default, MAC processing is performed.
 */
#define EXT_ND_FRAME_DL_CTRL_MAC          1
#define EXT_ND_FRAME_DL_CTRL_NEED_ACK     2
#define EXT_ND_FRAME_DL_CTRL_NOT_NEED_ACK 3

#define EXT_ND_DL 0
#define EXT_ND_UL 1

#define EXT_UPG_STATUS_QRY_SYS_MODE 0 /* Query the working mode. */
#define EXT_UPG_STATUS_QRY_UPGRADE  1 /* queries the current upgrade status. */

#define UPG_STATUS_OK      EXT_UPG_STATUS_OK      /* upgrade succeeded */
#define UPG_STATUS_END     EXT_UPG_STATUS_END     /* The upgrade is complete. */
#define UPG_STATUS_NORMAL  4                     /* Normal upgrade status */
#define UPG_STATUS_UNKNOWN EXT_UPG_STATUS_UNKNOWN /* Unknown */

#define UPG_STATUS_STOPPING EXT_UPG_STATUS_STOPPING

#define UPG_STATUS_FAIL    EXT_UPG_STATUS_FAIL    /* upgrade failed */
#define UPG_STATUS_PROCESS EXT_UPG_STATUS_PROCESS /* Upgrading... */

#define EXT_UPG_VER_UPDATE_OPTION_MASK (EXT_UPG_VER_UPDATE_OPTION_START)

/* Maximum number of MAC addresses in the MAC address list supported by the NCR channel */
#define EXT_PLC_RELAY_NUM_MAX 3

#define EXT_COL_METER_NUM_MAX 33

#define EXT_PLC_CRASH_SERIAL_RPT_CODE "$PlCCrAsH^"

#define EXT_DEID_OPTION_REFRESH 0xF0

/************************************************************
                             Data Structure Definition
************************************************************/
/*********************DBK Querying the API Version************************************/
/* Command interface version number */
#define EXT_CMD_VER 0

typedef struct _diag_cmd_dbk_qry_ue_api_ver_ind_s_ {
    td_u16 cmd_ver;   /* Command interface version number */
    td_u16 h_api_ver; /* API version */
    td_u16 api_ver;   /* Current API version of the board */
    td_u16 ver;       /* C version */
    td_u32 err_code;  /* Error code */
    td_u32 pad;       /* Reserved bit */
} diag_cmd_dbk_qry_ue_api_ver_ind;

/******************************************************************************/
/* Query the basic information about the directly connected module: EXT_ND_CONNECT_MODULE_INFO */
/******************************************************************************/
/******************************************************************************/
/* REQ: None */
/* IND: EXT_ND_CONNECTED_INFO_IND_S */
typedef struct {
    td_u8 sys_mode : 7;                           /* Device status */
    td_u8 join : 1;                               /* Check whether the networking is complete. */
    td_u8 dev_type;                               /* Product form */
    td_u8 mac_addr[EXT_PLC_MAC_ADDR_LEN];          /* communication module MAC */
    td_u8 ip[EXT_IPV4_ADDR_LEN];                   /* Ethernet IP */
    td_u32 snid;                                  /* Communication network SNID */
    td_u8 cco_addr[EXT_PLC_MAC_ADDR_LEN];          /* CCO MAC */
    td_u8 pad[2];                                 /* reserved 2 bytes */
    td_char positoin[EXT_MODULE_POSITION_LEN_MAX]; /* location information */

    td_u32 start_time; /* Device startup time */
} ext_nd_connected_info_ind;
#define EXT_ND_CONNECTED_INFO_ACK_S ext_nd_connected_info_ind

/******************************************************************************/
/* EXT_ND_MAC_FIND_MODULE */
/******************************************************************************/
/******************************************************************************/
/* Search for devices. */
/* REQ: None */
/* ACK: */
typedef struct {
    /* EXT_ND_TYPE_DCM:   meter reading controller */
    /* EXT_ND_TYPE_CCO:   headend */
    /* EXT_ND_TYPE_STA:   terminal */
    /* EXT_ND_TYPE_RELAY: trunk */
    td_u8 type : 4;
    td_u8 online : 1;               /* Online or offline */
    td_u8 status : 1;               /* Normal, Abnormal */
    td_u8 phase_mode;               /* Phase mode, multi-phase or single-phase */
    td_u8 phase;                    /* Phase type */
    td_u8 band;                     /* Frequency band */
    td_u16 tei;                     /* Site TEI (online) */
    td_u32 join_time;               /* Time when a user joins the network (online) */
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN]; /* MAC address */
} ext_module_item;

typedef struct {
    td_u16 item_cnt; /* Number of peripheral modules */
    ext_module_item ast_item[0];
} ext_mac_find_module_ind;

#if defined(PRODUCT_CFG_SUPPORT_UPG)
typedef struct {
    td_u32 upg_id;         /* Upgrade ID. The ID must be unique during the upgrade. */
    td_u16 reset_interval; /* Restart waiting time, in seconds */
    td_u16 option;
} ext_upg_affirmative_req;

typedef struct {
    td_u32 rc;     /* Result code. EXT_ERR_SUCCESS indicates success. Other values indicate error codes. */
    td_u32 upg_id; /* Upgrade ID. The ID must be unique during the upgrade. */
} ext_upg_affirmative_ind;

#define UPG_LIST_NUM_PER_PACK  30
#define UPG_LIST_MAX_NUM_OTHER 500
#define UPG_LIST_MAX_NUM_MRS   1000
#define UPG_LIST_LEN_PER_PACK  UPG_LIST_NUM_PER_PACK* EXT_PLC_MAC_ADDR_LEN

typedef struct {
    td_u8 stru_ver;
    td_u8 stru_size;
    td_u16 padding;
    td_u32 upg_id; /* upg ID */
    td_u32 ret;    /* return code */
} upg_sta_list_ind;

typedef struct {
    td_u8 stru_ver;
    td_u8 stru_size;
    td_u8 packet_total;
    td_u8 packet_seq;
    td_u32 upg_id;
    td_u16 sta_total;
    td_u16 sta_current;
    td_u8 sta_list[UPG_LIST_NUM_PER_PACK][EXT_PLC_MAC_ADDR_LEN];
} upg_list_reply_ind;

typedef enum {
    EXT_UPG_EXTEND_INF_VER0 = 0,
} ext_upg_extend_inf_ver;

#define EXT_UPG_EXTEND_INF_VER EXT_UPG_EXTEND_INF_VER0

#define EXT_UPG_EXTEND_SUPPORT_LIST_UPGRADE 0x01 /* support list upg */
#define EXT_UPG_EXTEND_SUPPORT_FUN          EXT_UPG_EXTEND_SUPPORT_LIST_UPGRADE

typedef struct {
    td_u8 stru_ver;
    td_u8 stru_size;
    td_u16 pad;
} upg_extend_info_req;

typedef struct {
    td_u8 stru_ver;   /* Version number. The version number is 0 */
    td_u8 stru_size;  /* Structure size */
    td_u8 upg_option; /* CCO STA upgrade mode:0: N/A; 1: entire network; 2: list */
    td_u8 fun_bitmap; /* Supported functions and features */
    td_u32 upg_id;    /* Upgrade ID */
    td_u32 pad;
} upg_extend_info_ind;

typedef struct {
    td_u16 req_sn;
    td_u16 idx;
    td_u32 rc;
} ext_upg_blk_id;

typedef struct {
    td_u16 checksum;
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN];
    td_u32 upg_id;
    td_u16 rc;
    td_u16 id_num;
    ext_upg_blk_id aul_id[0];
} ext_upg_transmit_blk_ind;

/* Used by the CCO to query the site upgrade status */
typedef enum {
    EXT_UPG_STATUS_IND_ENTIRE_VER0,
    EXT_UPG_STATUS_IND_ENTIRE_VER1,
} ext_upg_status_ind_entire_ver;

#define EXT_UPG_STATUS_IND_ENTIRE_VER (EXT_UPG_STATUS_IND_ENTIRE_VER1)

#define UPG_FILE_INFO_ID  0xC1
#define UPG_FILE_HEAD_LEN 298
typedef struct {
    td_u32 upg_id;
    td_u16 last_blk_idx;
    td_u16 error_code;
    td_u32 rcv_file_size;
    td_u32 blk_size;
    td_u32 file_size;
    td_u8 upg_status;
    td_u8 pad;
    td_u8 mac_addr[EXT_PLC_MAC_ADDR_LEN];
    td_u32 upg_end_time; /* modified by ds, duration(Sec.) of indicate sta from rcv start frame to rcv all upg data */
} ext_upg_status_ind_entire;

typedef struct {
    ext_upg_status_ind_entire status;

    td_u8 blk_per_pkt; /* Number of blocks contained in an upgrade package. */
    td_u8 pad;
    td_u16 bitmap_cnt;
    td_u8 bitmap[0];
} ext_upg_status_detail_ind;

typedef enum {
    UPG_QRY_PRODUCT_TYPE_METER = 0, /* watt-hour meter */
    UPG_QRY_PRODUCT_TYPE_METERI,    /* I collector */
    UPG_QRY_PRODUCT_TYPE_METERII,   /* II collector */
} ext_upg_query_product_type;

typedef enum {
    UPG_INFO_ID_CODE = 0, /* Vendor ID */
    UPG_INFO_ID_SW_VER,   /* Version information */
    UPG_INFO_ID_BOOT_VER, /* Bootloader version */
    UPG_INFO_ID_FILE_CRC, /* Upgrade file CRC32 */
    UPG_INFO_ID_FILE_LEN, /* Upgrade file length */
    /* UPG_INFO_ID_CHIP,       // Chip code */
    /* UPG_INFO_ID_DATE,       // Version date */
    UPG_INFO_ID_PRODUCT_TYPE,  /* Product type */
} ext_upg_query_info_id;

typedef struct {
    td_u16 stru_ver : 6;  /* protocol version */
    td_u16 stru_size : 6; /* packet header length */
    td_u16 reserved : 4;
    td_u8 reserved2;
    td_u8 info_item_cnt; /* Number of information list elements */
    td_u8 info_item[0];  /* Message ID list */
} ext_upg_sta_info_req;

typedef struct {
    td_u16 stru_ver : 6;  /* protocol version */
    td_u16 stru_size : 6; /* packet header length */
    td_u16 reserved1 : 4;
    td_u8 reserved2;
    td_u8 info_item_cnt; /* Number of information list elements */
    /* Information list: data field list. One or more information fields are placed in sequence (element ID (1 byte) +
     * element data length (1 byte) + element data.(L)) */
    td_u8 info_item[0];
} ext_upg_sta_info_ind;

/* PLC packet format for State Grid interconnection */
typedef struct {
    td_u16 stru_ver : 6;  /* protocol version */
    td_u16 stru_size : 6; /* packet header length */
    td_u16 reserved1 : 4;
    td_u16 reserved2;
    td_u32 upg_id;      /* Upgrade ID */
    td_u16 time_window; /* Upgrade time window */
    td_u16 packet_size; /* Upgrade block size */
    td_u32 file_size;   /* Size of the upgrade file */
    td_u32 crc;         /* File CRC check */
} ext_upg_std_start_req; /* 11 start req */

typedef struct {
    td_u16 stru_ver : 6;  /* protocol version */
    td_u16 stru_size : 6; /* packet header length */
    td_u16 reserved1 : 4;
    td_u8 reserved2;
    td_u8 rc;           /* Result code for starting the upgrade */
    td_u32 upg_id;      /* Upgrade ID */
} ext_upg_std_start_ind; /* 11 start ind */

typedef struct {
    td_u8 id;                      /* Optional ID */
    td_u8 size;                    /* Size of optional content */
    td_u8 data[UPG_FILE_HEAD_LEN]; /* optional content */
} ext_upg_expand_info;

typedef struct {
    td_u16 stru_ver : 6;   /* protocol version */
    td_u16 stru_size : 6;  /* packet header length */
    td_u16 need_ack : 1;   /* Indicates whether the call needs to be answered. */
    td_u16 broad_cast : 1; /* Whether the packet is a broadcast packet */
    td_u16 cmn_reserved : 2;
    td_u16 data_size;   /* Size of the optional area */
    td_u32 upg_id;      /* Upgrade ID */
    td_u16 time_window; /* Upgrade time window */
    td_u16 packet_size; /* Upgrade block size */
    td_u32 file_size;   /* Size of the upgrade file */
    td_u32 crc;         /* File CRC check */
    td_u8 data[0];      /* Extended content */
} ext_upg_std_expand_start_req;

typedef struct {
    td_u16 stru_ver : 6;  /* protocol version */
    td_u16 stru_size : 6; /* packet header length */
    td_u16 reserved1 : 4;
    td_u16 reserved2;
    td_u32 upg_id; /* Upgrade ID */
} ext_upg_std_stop_req;

typedef struct {
    td_u16 stru_ver : 6;  /* protocol version */
    td_u16 stru_size : 6; /* packet header length */
    td_u16 reserved : 4;
    td_u16 packet_size; /* Data block size */
    td_u32 upg_id;      /* Upgrade ID */
    td_u32 packet_num;  /* Data block number, starting from 0 */
    td_u8 packet[0];    /* Data block */
} ext_upg_std_transmit_req;

typedef struct {
    td_u16 stru_ver : 6;  /* protocol version */
    td_u16 stru_size : 6; /* packet header length */
    td_u16 reserved : 4;
    td_u16 packet_num;   /* Number of consecutively queried blocks */
    td_u32 packet_start; /* Start block number, starting from 0 */
    td_u32 upg_id;       /* Upgrade ID */
} ext_upg_std_status_req;

typedef struct {
    td_u16 stru_ver : 6;   /* protocol version */
    td_u16 stru_size : 6;  /* packet header length */
    td_u16 upg_status : 4; /* Upgrade status */
    td_u16 packet_num;     /* Number of valid blocks */
    td_u32 packet_start;   /* Start block number */
    td_u32 upg_id;         /* Upgrade ID */
    td_u8 bitmap[0];
} ext_upg_std_status_ind;

typedef struct {
    td_u16 stru_ver : 6;  /* protocol version */
    td_u16 stru_size : 6; /* packet header length */
    td_u16 reserved : 4;
    td_u16 reset_time;       /* Waiting reset time */
    td_u32 upg_id;           /* Upgrade ID */
    td_u32 try_running_time; /* Trial run time */
} ext_upg_std_affirm_req;

typedef ext_upg_sta_info_req ext_upg_std_sta_info_req_s;

typedef struct {
    td_u16 stru_ver : 6;  /* protocol version */
    td_u16 stru_size : 6; /* packet header length */
    td_u16 reserved1 : 4;
    td_u8 reserved2;
    td_u8 info_item_cnt; /* Number of information list elements */
    td_u32 upg_id;       /* Upgrade ID */
    /* Information list: data field list. One or more information fields are placed in sequence (element ID (1 byte) +
     * element data length (1 byte) + element data.(L)) */
    td_u8 info_item[0];
} ext_upg_std_sta_info_ind;
#endif

typedef struct {
    td_u8 dev_type;
    td_u8 dev_num;
    td_u8 dev_addr[EXT_COL_METER_NUM_MAX][EXT_PLC_MAC_ADDR_LEN];
} ext_nd_module_detect_ind;

#endif /* defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER) || defined(PRODUCT_CFG_DBK) */

#endif /* __SOC_NDM_H__ */

