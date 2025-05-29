/* *
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: DIAG - param sop.
 */

#ifndef DIAG_SYS_CMD_PARAM_SOP_H
#define DIAG_SYS_CMD_PARAM_SOP_H

#include <soc_mdm_types.h>
#include "diag_cmd_id.h"
#include "diag_cmd_param_plugin.h"

/* *
 * Description: DBK connects to the UE device, notifying the device that DBK has started to connect to the UE.
 * ID   : DIAG_CMD_HOST_CONNECT
 * REQ  : DIAG_CMD_HOST_CONNECT_REQ_STRU
 * CNF  : DIAG_CMD_HOST_CONNECT_CNF_STRU
 * IND  : -
 */
#define IMEI_SIZE 16
typedef struct {
    td_u32 rc; /* result code */
    td_char imei[IMEI_SIZE];
    /* [0-5]:MAC ADDRESS. [6-7]:TEI. [8]:1(CCO)/0(STA). [9]:0(sheet reading module). 1:(copy controller) */
    ext_ue_soft_version ue_soft_version;
    ext_ue_product_ver build_version;
} diag_cmd_host_connect_cnf_stru;

/* Log type definition */
#define DIAG_CMD_LOG_CATETORY_LAYER_ID (1 << 12)
#define DIAG_CMD_LOG_CATETORY_DUMP_ID (1 << 11)
#define DIAG_CMD_LOG_CATETORY_MSG_ID (1 << 10)

/* Normal print level definition */
#define DIAG_CMD_LOG_CFG_PRINT_LEVEL_NONE (0x80000000)
#define DIAG_CMD_LOG_CFG_PRINT_LEVEL_ERROR (0x40000000)
#define DIAG_CMD_LOG_CFG_PRINT_LEVEL_WARNING (0x20000000)
#define DIAG_CMD_LOG_CFG_PRINT_LEVEL_INFO (0x10000000)
#define DIAG_CMD_LOG_CFG_PRINT_LEVEL_NORMAL (0x08000000)

#define DIAG_CMD_LOG_CFG_PRINT_LEVEL_1 (0x04000000)
#define DIAG_CMD_LOG_CFG_PRINT_LEVEL_2 (0x02000000)
#define DIAG_CMD_LOG_CFG_PRINT_LEVEL_3 (0x01000000)

/* Event log level definition */
#define DIAG_CMD_LOG_CFG_EVENT_TYPE_INFO 0x00800000
#define DIAG_CMD_LOG_CFG_EVENT_TYPE_AUDIT_FAILURE 0x00400000
#define DIAG_CMD_LOG_CFG_EVENT_TYPE_AUDIT_SUCCESS 0x00200000
#define DIAG_CMD_LOG_CFG_EVENT_TYPE_ERROR 0x00100000
#define DIAG_CMD_LOG_CFG_EVENT_TYPE_WARNING 0x00080000

/* The switch between the layer, air interface and user plane data is defined by the following macro */
#define DIAG_CMD_REQ_LOG_CFG_FILTER_SWT_CLOSE 0x00
#define DIAG_CMD_REQ_LOG_CFG_FILTER_SWT_OPEN 0x01

/* Reporting on inter-layer messages */
#define DIAG_CMD_REQ_IND_OS_MSG_UL 0x80000000
#define DIAG_CMD_REQ_IND_OS_MSG_DL 0x40000000

/* DUMP data output format definition */
#define DIAG_CMD_IND_DUMP_2X_HEX_BYTE 0x00000000
#define DIAG_CMD_IND_DUMP_4X_HEX_WORD 0x01000000
#define DIAG_CMD_IND_DUMP_8X_HEX_DWORD 0x02000000
#define DIAG_CMD_IND_DUMP_2D_DECIMAL_BYTE 0x10000000
#define DIAG_CMD_IND_DUMP_4D_DECIMAL_WORD 0x20000000
#define DIAG_CMD_IND_DUMP_8D_DECIMAL_DWORD 0x40000000

typedef struct {
    td_u32 cmd_id;
} diag_cmd_stop_req_stru;

typedef struct {
    td_u32 rc;
} diag_cmd_stop_cnf_stru;

typedef struct {
    td_u32 module_id;
} diag_cmd_log_init_req_stru;

typedef struct {
    td_u32 rc;
} diag_cmd_log_init_ack_stru;

#define PRODUCT_VER_SIZE 24
#define FILE_VER_SIZE 8
#define NV_VER_SIZE 4
#define DATE_SIZE 12
#define TIME_SIZE 12
typedef struct {
    td_char product_ver[PRODUCT_VER_SIZE];
    td_char file_ver[FILE_VER_SIZE];
    td_char nv_ver[NV_VER_SIZE];
    td_char date[DATE_SIZE];
    td_char time[TIME_SIZE];
} diag_cmd_ver_qry_ack_stru;

#define DIAG_CMD_ALL_MODULE_ID 0xFFFFFFFF

#define DIAG_CMD_LAYER_MOD_SRC 0
#define DIAG_CMD_LAYER_MOD_DST 1

/* MSG Configuration Confirmation (Single Board -> DBK) Structure */
typedef struct {
    td_u16 rc;        /* result */
    td_u16 module_id; /* module ID */
} diag_cmd_msg_cfg_cnf_stru;

/* MSG reset confirmation (single board -> DBK) structure */
typedef struct {
    td_u32 rc;
} diag_cmd_msg_rst_cnf_stru;

/* MSG Configuration Request (DBK->Single Board) Structure */
typedef struct {
    td_u16 module_id;     /* Module ID, when the MSG is SYS or USR, marks the subcategories under the two categories */
    td_u8 is_dest_module; /* Is it the target module */
    td_u8 switch_code;    /* Turns the configuration on or off */
} diag_cmd_msg_cfg_req_stru;

/* MSG Reset Request (DBK->Single Board) Structure */
typedef struct {
    td_u32 msg_type; /* MSG type, 0: all; 1: SYS; 2: DEV; 3: USR */
} diag_cmd_msg_cfg_rst_req_stru;

typedef struct {
    td_u32 module;   /* source module ID */
    td_u32 dest_mod; /* destination module ID */
    td_u32 no;       /* No. */
    td_u32 id;       /* ID */
    td_s8 dta[0];    /* User Data Cache */
} diag_cmd_log_layer_ind_stru;

/* *
 * CMD ID : -   DIAG_CMD_SET_AT_PHY_CHANNEL
 * REQ    : -     DIAG_CMD_SET_AT_PHY_CHANNEL_REQ_STRU
 * CNF    : -     DIAG_CMD_SET_AT_PHY_CHANNEL_CNF_STRU
 *
 * Write data to NV through this channel
 */
typedef enum {
    PHY_CHANNEL_ETH = 0,
    PHY_CHANNEL_USB = 1,
    PHY_CHANNEL_UART = 2,
    PHY_CHANNEL_UNKNOWN = 0xFFF
} msp_at_phy_channel_enum;

/* REQ */
typedef struct {
    msp_at_phy_channel_enum phy_channel;
} diag_cmd_set_at_phy_channel_req_stru;

/* Reply */
typedef struct {
    msp_at_phy_channel_enum phy_channel;
    td_u32 rc;
} diag_cmd_set_at_phy_channel_cnf_stru;

/* diag connect req pub_a */
#define DIAG_CONN_PUB_DATA_SIZE 128
#define DIAG_CONN_USR_NAME_SIZE 20

#define EXT_DIAG_CONN_ERR_OK 0              /* Connection verification succeeded */
#define EXT_DIAG_CONN_ERR_SYS_CALL 1        /* Connection verification system error */
#define EXT_DIAG_CONN_ERR_PASS_WORD_WRONG 2 /* Connection verification password error */
#define EXT_DIAG_CONN_ERR_ACCOUNT_LOCK 3    /* Connection verification account lock */
#define EXT_DIAG_CONN_ERR_BUSY 6            /* Connection needs to preempt */

typedef struct {
    td_u16 err_no;        /* Error code (as above) */
    td_u16 res_try_cnt;   /* Remaining retries (units) */
    td_u16 res_lock_time; /* Remaining lock duration (unit s) */
    td_u16 pad;           /* Reserved */
} diag_cmd_conn_auth_ind_stru;

typedef struct {
    td_u8 pub_a[DIAG_CONN_PUB_DATA_SIZE];
} diag_cmd_conn_req_pub_a_stru;

/* diag connect rsp pub_b */
#define DIAG_CONN_SALT_SIZE 20
typedef struct {
    td_u8 pub_b[DIAG_CONN_PUB_DATA_SIZE];
    td_u8 salt[DIAG_CONN_SALT_SIZE];
    td_u32 salt_len;
    diag_cmd_conn_auth_ind_stru ret_ind; /* In order to be compatible with the old version, put it at the end */
} diag_cmd_conn_rsp_pub_b_stru;

/* diag connect req m */
#define DIAG_CONN_KEY_HASH_SIZE 32
typedef struct {
    td_u8 req_m1[DIAG_CONN_KEY_HASH_SIZE];
} diag_cmd_conn_req_m_stru;

/* diag connect rsp m */
typedef struct {
    td_u32 ret; /* EXT_ERR_SUCCESS or EXT_ERR_FAILURE. */
    td_u8 rsp_m2[DIAG_CONN_KEY_HASH_SIZE];
    diag_cmd_conn_auth_ind_stru ret_ind; /* In order to be compatible with the old version, put it at the end */
} diag_cmd_conn_rsp_m_stru;

#define DIAG_PWD_MAX_LEN 32
#define DIAG_CONN_VERTIFY_SIZE 128
typedef struct {
    td_u8 old_pwd[DIAG_PWD_MAX_LEN];
    td_u8 new_pwd[DIAG_PWD_MAX_LEN];
    td_u8 new_salt[DIAG_CONN_SALT_SIZE];
    td_u8 old_pwd_len;
    td_u8 new_pwd_len;
    td_u8 new_salt_len;
    td_u8 pad;
} diag_cmd_change_pwd_req_stru;

#define EXT_CHANGE_PWD_ERR_OK 0                  /* password has been updated */
#define EXT_CHANGE_PWD_ERR_SYS_CALL 1            /* System error */
#define EXT_CHANGE_PWD_ERR_PASSWORD_WRONG 2      /* The original password is incorrect */
#define EXT_CHANGE_PWD_ERR_ACCOUNT_LOCK 3        /* Account Lock */
#define EXT_CHANGE_PWD_ERR_PASSWORD_FORMAT_ERR 4 /* The password format is incorrect */

typedef diag_cmd_conn_auth_ind_stru diag_cmd_change_pwd_ind_stru;

typedef struct {
    td_u32 data_size; /* Data length */
    td_u8 data[0];    /* Data content */
} diag_plc_test_cmd_stru;

#define DIAG_HEART_BEAT_DIR_UP 0x1
#define DIAG_HEART_BEAT_DIR_DOWN 0x2
#define DIAG_HEART_BEAT_DEFAULT_RANDOM_DATA 0x5C5C
typedef struct {
    td_u32 dir;         /* direction bit */
    td_u32 random_data; /* random data, requiring the same uplink and downlink */
} diag_cmd_heart_beat_struct;

#endif /* __DIAG_SYS_CMD_PARAM_SOP_H__ */
