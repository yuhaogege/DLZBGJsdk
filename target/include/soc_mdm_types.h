/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: the general structures and macro definitions.
 */
#ifndef __SOC_MDM_TYPES_H__
#define __SOC_MDM_TYPES_H__
#ifndef SOC_TYPES_H
#error "Please include soc_types.h before using soc_mdm_types.h"
#endif
EXT_START_HEADER

#include <soc_mdm_types_rom.h>

#ifndef PRODUCT_CFG_DBK
#include <soc_mdm_msg.h>
#endif

#define ext_struct_offset(s, m) ((td_u32) & (((s *)0)->m)) /* Structure member offset */

#if defined PRODUCT_CFG_USR_REMAP_32K
#define EXT_DYNAMIC_MEM_START_ADDR 0x2038000
#elif defined(PRODUCT_CFG_USR_REMAP_48K)
#define EXT_DYNAMIC_MEM_START_ADDR 0x2034000
#elif defined(PRODUCT_CFG_USR_REMAP_64K)
#define EXT_DYNAMIC_MEM_START_ADDR 0x2030000
#else
#error "g_dynamic_ram_start_addr invalid"
#endif


#define EXT_PERMANENT_MEM_START_ADDR 0x2000000
#define EXT_PERMANENT_MEM_END_ADDR EXT_DYNAMIC_MEM_START_ADDR

#define EXT_DYNAMIC_MEM_PYH_END_ADDR 0x2040000
#define EXT_DYNAMIC_MEM_VIR_END_ADDR 0x2100000

/****************************************************************************** */
/****************************************************************************** */
/* Byte alignment */
/****************************************************************************** */
#define ALIGNTYPE_1BYTE 1
#define ALIGNTYPE_2BYTE 2
#define ALIGNTYPE_4BYTE 4
#define ALIGNTYPE_8BYTE 8
#define ALIGNTYPE_64BYTE 64
#define ALIGNTYPE_32BYTE 32
#define ALIGNTYPE_4K 4096
#define align_pre(val, a) ((val) & (~((a)-1)))
#define align_next(val, a) ((((val) + ((a)-1)) & (~((a)-1))))
#define align_length(val, a) align_next(val, a)
#define EXT_ALL_F_32 0xFFFFFFFF
#define EXT_ALL_F_16 0xFFFF
/****************************************************************************** */
#ifdef UT_SWITCH
#define uapi_timer_define(timer_proc) \
    _STDCALL timer_proc(td_u32 ulId, td_u32 uMsg, td_u32 ulData, td_u32 dw1, td_u32 dw2)
#define ext_task_define(task_proc) __cdecl task_proc(td_void *pArgv)
#define ext_hitimer_define(timer_proc) \
    _STDCALL timer_proc(td_u32 uTimerID, td_u32 uMsg, td_u32 dwUser, td_u32 dw1, td_u32 dw2)
#else
#define uapi_timer_define(timer_proc) timer_proc(__attribute__((unused)) td_u32 ulData)
#define ext_task_define(task_proc) task_proc(td_void *pArgv)
#define ext_hitimer_define(timer_proc) timer_proc(td_u32 ulParam)
#endif

typedef struct {
    td_u32 lo;
    td_u32 hi;
} ext_obj64;
#define EXT_OBJ64_S_INIT_DEFINED \
    {                           \
        0, 0                    \
    }

typedef struct {
    td_u32 major_minor_version; /* Main version. Minor version */
    td_u32 revision_version;    /* Revision version */
    td_u32 build_version;       /* Internal version */
} ext_ue_soft_version;

#define EXT_BUILD_VER_DATE_LEN 10
#define EXT_BUILD_VER_TIME_LEN 8
#define EXT_BUILD_VER_PRODUCT_NAME_LEN_MAX 28
#define EXT_BUILD_VER_PRODUCT_LEN_MAX \
    (EXT_BUILD_VER_PRODUCT_NAME_LEN_MAX + EXT_BUILD_VER_DATE_LEN + EXT_BUILD_VER_TIME_LEN + 6)

typedef struct {
    td_u16 v_ver_no;   /* V field */
    td_u16 r_ver_no;   /* R field */
    td_u16 c_ver_no;   /* C field */
    td_u16 b_ver_no;   /* B field */
    td_u16 spc_no;     /* SPC field */
    td_u16 custom_ver; /* Version number specified by the customer */
    td_u32 product_no;
    /* Product type number, indicating the hardware platform in different peripheral combinations. */
    td_char ac_build_date[EXT_BUILD_VER_DATE_LEN]; /* Build date, format: Mon dd yyyy */
    td_char ac_build_time[EXT_BUILD_VER_TIME_LEN]; /* Build time, format: hh:mm:ss */
} ext_ue_build_ver_info;                           /* Internal Version */

typedef struct {
    td_u16 version_v;   /* Version number: V field */
    td_u16 version_r;   /* Version number: R field */
    td_u16 version_c;   /* Version number: C field */
    td_u16 version_b;   /* Version number: B field */
    td_u16 version_spc; /* Version number: SPC field */
    td_u16 reserved[3]; /* Reserved 3 */
} ext_ue_product_ver;

/*
 * @ingroup  hct_ver
 *
 * Product version information
 */
typedef struct {
    td_char *product_version; /* < Version number, for example, dw11 V100R001C00B00 */
    td_char *build_date;      /* < Version build date, for example, 2011-08-01 */
    td_char *build_time;      /* < Version build time, for example, 14:30:26 */
} ext_product_info;

/****************************************************************************** */
#define EXT_INVALID_NV_DATA 0xFFFFFFFF
/****************************************************************************** */
/****************************************************************************** */
#define EXT_INVALID_QUEUE_ID 0xFFFFFFFF
#define EXT_SYS_USER_MESSAGE_ID_MAX 0x00FFFFFF

#define EXT_SYS_MSG_SYS_INIT_STATUS (EXT_SYS_USER_MESSAGE_ID_MAX + 1)
#define EXT_SYS_MSG_UPG_STATUS (EXT_SYS_MSG_SYS_INIT_STATUS)

typedef enum {
    EXT_DMS_LCHL_MODE_LOCAL = 0, /* The data source is local. */
    EXT_DMS_LCHL_MODE_PLC,       /* The data source is PLC. */
    EXT_DMS_LCHL_MODE_MAX
} ext_dms_lchl_mode;

typedef struct {
    td_u16 id;                  /* This parameter is valid only for EXT_DMS_LCHL_MODE_PLC. */
    td_u8 mode;                 /* Channel mode. The value is EN_DMS_LCHL_MODE_XX. */
    td_u8 ctrl;                 /* This parameter is valid only for EXT_DMS_LCHL_MODE_PLC. */
    td_u8 discard_old_data : 1; /* TD_TRUE: Old data may be discarded. */
    td_u8 data_timeout : 1;     /* TD_TRUE: RX data timeout occurs. */
    td_u8 rsv0 : 6;
    td_u8 form_serial_number;
    td_u8 dest_mac[EXT_PLC_MAC_ADDR_LEN];
    td_u8 src_mac[EXT_PLC_MAC_ADDR_LEN];
    td_u16 payload_len;
    td_pbyte payload;
} ext_dms_chl_rx;

typedef struct {
    td_u16 idx;
    td_u16 arq_cnt;
    td_u32 err;
} ext_dms_chl_tx_ind;

typedef struct {
    ext_dms_chl_tx_ind chl_tx;
} ext_dms_chl_status_ind;

#define EXT_DMS_PLC_FRM_LEN1 (3 * 512)
#define EXT_DMS_PLC_FRM_LEN2 80

#define ext_mds_get_frame_index(ind) (((ext_dms_chl_tx_ind *)(ind))->idx)
#define ext_mds_get_frame_error(ind) (((ext_dms_chl_tx_ind *)(ind))->err)
#define ext_mds_get_frame_arq_cnd(ind) (((ext_dms_chl_tx_ind *)(ind))->arq_cnt)

typedef td_u32 (*ext_dms_chl_connect_f)(td_u16, td_u8);
typedef td_u32 (*ext_dms_chl_msg_ind_f)(td_u16, td_u32, td_pvoid);
typedef td_u32 (*ext_dms_chl_rx_f)(td_u16, EXT_CONST ext_dms_chl_rx *);
typedef td_u32 (*ext_dms_chl_iremote_packet_f)(td_u32, td_pvoid, td_u16);

/*
 * @ingroup  hct_dms
 *
 * Structure of a channel API instance
 */
typedef struct {
    ext_dms_chl_connect_f fn_chl_connect; /* < Channel connection callback function */
    ext_dms_chl_rx_f fn_chl_data_rx;      /* < Data receive callback function */
    ext_dms_chl_msg_ind_f fn_chl_msg_rx;  /* < Message notification callback function (reserved) */
} ext_dms_chl_port_obj;

#define EXT_DMS_DEFAULT_DEV_CHL_IF_VALUE \
    {                                   \
        TD_NULL, TD_NULL, TD_NULL       \
    }

#define EXT_DMS_FRM_INTER_INFO1_SIZE (sizeof(td_u8) + sizeof(td_u8)) /* mmt, vt */
#define EXT_DMS_FRM_INTER_INFO2_SIZE (4)                             /* Length of the common control field */

#define EXT_DMS_INTER_INFO_SIZE (EXT_DMS_FRM_INTER_INFO1_SIZE + EXT_DMS_FRM_INTER_INFO2_SIZE)
#define EXT_DMS_FRM_INTER_MAX_PAYLOAD_SIZE (1994)
#define EXT_DMS_FRM_MAX_PAYLOAD_SIZE (EXT_DMS_FRM_INTER_MAX_PAYLOAD_SIZE - EXT_DMS_FRM_INTER_INFO2_SIZE)
#define EXT_DMS_FRM_INTER_MAX_SIZE (EXT_DMS_FRM_INTER_MAX_PAYLOAD_SIZE + EXT_DMS_FRM_INTER_INFO1_SIZE)

typedef struct {
    td_u16 data_size; /* < Data size, that is, the size of the space occupied by ucData (unit: byte) */
    td_u8 inside_info[EXT_DMS_INTER_INFO_SIZE]; /* < Internal use */
    td_u8 data[0];                             /* < Data */
} ext_dms_chl_tx_data;

#define EXT_DMS_CHL_FRAME_HRD_SIZE (sizeof(ext_dms_chl_tx_data))

#define EXT_DMS_CHL_TX_TYPE_NA 0x00
#define EXT_DMS_CHL_TX_TYPE_MR 0x01
#define EXT_DMS_CHL_TX_TYPE_UPG 0x02
#define EXT_DMS_CHL_TX_TYPE_DIAG 0x04
#define EXT_DMS_CHL_TX_TYPE_LB 0x08 /* Local broadcast */
#define EXT_DMS_CHL_TX_TYPE_MAX 0x0F

#define EXT_DMS_CHL_TX_CTRL_NA 0x00     /* Default sending */
#define EXT_DMS_CHL_TX_CTRL_CIPHER 0x01 /* Encryption mode */
#define EXT_DMS_CHL_TX_CTRL_FORCE 0x08  /* Forcible sending, regardless of whether the network or channel is available \
                                        */
#define EXT_DMS_CHL_TX_CTRL_MAX 0x0F

#define EXT_DMS_CHL_TX_ADAPTIVE 0x0
#define EXT_DMS_CHL_TX_TREE_NETWORK 0x1
#define EXT_DMS_CHL_TX_MESH_NETWORK 0x2

typedef struct {
    td_u16 id;  /* Frame ID, determined by the user */
    td_u8 ctrl; /* Control word. For details about the value, see EXT_DMS_CHL_TX_CTRL_XXX. */
    td_u8 type;
    /* For details about the value, see EXT_DMS_CHL_TX_TYPE_XX. Maximum value: EXT_DMS_CHL_TX_TYPE_MAX -- mt */
    td_u8 prio; /* Priority, determined by the user. [0x0 - 0xF] -- vt */
    td_u8 max_resend_count;
    /* Specifies the maximum number of MAC packet sending times.  */
    /* If the value is 0, the default setting is used:
     *  The tree network usage is configured based on NV 0x46:
     *      The maximum number of unicast retransmissions is max_resend_cnt;
     *      The fixed number of broadcast retransmissions is fix_resend_cnt;
     *  The value used by the mesh network is specific:
     *      The maximum number of unicast retransmissions is 5.
     *      The fixed number of broadcast retransmissions is 1.
     * If the value is not 0, the value of this parameter is used.
     */
    td_u8 send_mode;
    /*
     * Sending mode. The value is EXT_MAC_FAST_SEND_BACKOFF, EXT_MAC_FAST_SEND_PRIORITY,
     * EXT_MAC_FAST_SEND_PREEMPT, or EXT_MAC_FAST_SEND_MAX
     */
    td_bool is_stas_commu;
    td_u8 dest_mac[EXT_PLC_MAC_ADDR_LEN];
    /*
     * MAC address of the target module.
     * (1) If the value is all 0s, data is sent to the CCO.
     */
    td_u8 is_set_send_type; /**< This parameter is used with send_type. #TD_TRUE :set; #TD_FALSE :not set */
    td_u8 send_type; /* 0: send based on the route query result, 1: proxy broadcast, 2: local broadcast */
} ext_dms_chl_tx_ctrl;
typedef ext_dms_chl_tx_ctrl ext_dms_chl_tx_addr_s;

#define EXT_DIAG_PAYLOAD_MAX_SIZE 512

#define EXT_DIAG_TYPE_A 0      /* R&D commissioning version of the DBK */
#define EXT_DIAG_TYPE_B 1      /* Onsite maintenance version of the DBK */
#define EXT_DIAG_TYPE_LLN 0x0F /* Layer Level N/ A */
#define EXT_DIAG_TYPE_SLN 0xF0 /* System Level N/ A */
#define EXT_DIAG_TYPE_SL0 0xF1 /* System Level 0 */

/* Print level */
#define EXT_DIAG_PRINT_LEVEL_NONE (0x80000000)
#define EXT_DIAG_PRINT_LEVEL_ERROR (0x40000000)
#define EXT_DIAG_PRINT_LEVEL_WARNING (0x20000000)
#define EXT_DIAG_PRINT_LEVEL_INFO (0x10000000)
#define EXT_DIAG_PRINT_LEVEL_NORMAL (0x08000000)
#define EXT_DIAG_PRINT_LEVEL_2 (0x04000000)
#define EXT_DIAG_PRINT_LEVEL_3 (0x02000000)
#define EXT_DIAG_PRINT_LEVEL_4 (0x01000000)

#define ext_diag_mod_id(module_id, type) (((module_id) & 0x00FFFFFF) | (type))
#define ext_diag_id(module_id, type) (((module_id) & 0x00FFFFFF) | (type))

#ifdef EXT_HAVE_DIAG_TXT_ID
#ifndef EXT_DIAG_FILE_ID
#error 'DIAG_FILE_ID is not defined! Please define at the top of your file __FILE__'.
#endif
#define ext_diag_txt(str) (td_pcstr)(EXT_DIAG_FILE_ID + __LINE__)
#else
#define ext_diag_txt(str) str
#endif

typedef struct {
    td_u16 id; /* Specify the message id. */
    td_u16 src_mod_id;
    td_u16 dest_mod_id;
    td_u16 data_size; /* the data size in bytes. */
    td_pvoid data;    /* Pointer to the data buffer. */
} ext_diag_layer_msg;

#define EXT_DIAG_OBJ_ID_MASK 0x07               /* Valid bit width of the user object ID */
#define EXT_DIAG_INSTANCE_ID_MASK 0x0f          /* Valid bit width of the channel mode ID */
#define EXT_DIAG_OBJ_ID_OFFSET_IN_INSTANCE_ID 4 /* Offset of the user object ID in the channel mode ID */
#define ext_diag_get_obj_id_from_instance_id(instance_id) \
    (((instance_id) >> EXT_DIAG_OBJ_ID_OFFSET_IN_INSTANCE_ID) & EXT_DIAG_OBJ_ID_MASK)

#define uapi_diag_reg_cmd_tbl(tbl) (tbl), ext_array_count(tbl)
#define uapi_diag_is_dremote_packet(x) \
    ((((td_u8)(x)) & EXT_DIAG_INSTANCE_ID_MASK) == ((td_u8)EXT_DIAG_CMD_INSTANCE_DREMOTE))
#define ext_diag_is_iremote_packet(x) \
    ((((td_u8)(x)) & EXT_DIAG_INSTANCE_ID_MASK) == ((td_u8)EXT_DIAG_CMD_INSTANCE_IREMOTE))
#define ext_diag_is_local_packet(x) ((((td_u8)(x) & EXT_DIAG_INSTANCE_ID_MASK)) == ((td_u8)EXT_DIAG_CMD_INSTANCE_LOCAL))
#define ext_diag_is_valid_packet_option(x) \
    (uapi_diag_is_dremote_packet(x) || uapi_diag_is_dremote_packet(x) || uapi_diag_is_dremote_packet(x))

#define EXT_SAL_DFX_HOST_CTRL_MR_SIMU_DISABLE 0
#define EXT_SAL_DFX_HOST_CTRL_MR_SIMU_ENABLE 1
#define EXT_SAL_DFX_HOST_CTRL_PLC_SIMU_ENABLE 1

/***************************************************************************** */
/***************************************************************************** */
typedef enum {
    EXT_DFX_MAC_MSG_ROUTER = 0x200,
    EXT_DFX_MAC_MSG_DIAG

    /* Other defined values must be smaller than 0x400. */
} ext_sal_dfx_mac_msg;

/* Channel disconnection notification */
#define EXT_DMS_CHL_DISCONNECT 0
#define EXT_DMS_CHL_CONNECT 1

#define EXT_USERDATA_ID_USR 0
#define EXT_USERDATA_ID_APP 1
#define EXT_USERDATA_ID_CRASH_HRD 3
#define EXT_USERDATA_ID_CRASH_DATA 4
#define EXT_USERDATA_ID_CRASH_DATA1 5
#define EXT_USERDATA_ID_CRASH_DATA2 6
#define EXT_USERDATA_ID_CRASH_DATA3 7
#define EXT_USERDATA_ID_TF_LIST 8
#define EXT_USERDATA_ID_BLACK_LIST 9

typedef struct {
    td_u32 upg_id; /* Upgrade file ID */
    td_u32 option; /* The value is EXT_UPG_REQ_OPTION_NA, EXT_REQ_OPTION_CCO,  */
    /* EXT_UPG_REQ_OPTION_STA, EXT_UPG_REQ_OPTION_BROADCAST, or EXT_UPG_REQ_OPTION_UNICAST. */
} ext_upg_req_option;

typedef struct {
    td_u16 id;
    td_u16 payload_size;
    td_u8 payload[0];
} ext_diag_packet;

#define EXT_ND_SYS_STATUS_NORMAL 0  /* Normal mode */
#define EXT_ND_SYS_STATUS_UPGRADE 1 /* Upgrade mode */
#define EXT_ND_SYS_STATUS_TEST 2    /* Test mode */
#define EXT_ND_SYS_STATUS_DIAG 4    /* Diagnosis mode */
#define EXT_ND_SYS_STATUS_UNKNOWN 8 /* Unknown mode */

/* Station version information */
typedef struct {
    /*
     * Operating mode of the board after startup. This variable is
     * not updated in real time. Normal mode: #EXT_ND_SYS_STATUS_NORMAL;
     * Upgrade mode: #EXT_ND_SYS_STATUS_UPGRADE;
     * Test mode: #EXT_ND_SYS_STATUS_TEST; Diagnosis mode: #EXT_ND_SYS_STATUS_DIAG;
     * Unknown mode: EXT_ND_SYS_STATUS_UNKNOWN;
     */
    td_u8 sys_mode;
    td_u8 boot_ver_idx; /* < Boot version number configured by initialization */
    td_u8 sys_cause;    /* < Soft restart cause */

    td_u8 version_v;         /* < Version number: V */
    td_u8 version_r;         /* < Version number: R */
    td_u8 version_c;         /* < Version number: C */
    td_u8 build_datetime[6]; /* < Build time 6 byte */
    td_u16 version_b;        /* < Version number: B */
    td_u16 version_spc;      /* < Version number: SPC */
    td_u16 chip_version;     /* < Chip version. dw11 V200 chip: NM_CHIP_VER_DW11_V200;
                                dw11 V100 chip: NM_CHIP_VER_DW11_V100; Other chips: NM_CHIP_VER_OTHER */

    /* --- Public part BEGIN --- */
    td_u8 boot_cause;     /* < Normal restart: EXT_ND_SYS_BOOT_CAUSE_NORMAL; Exception restart:
                               EXT_ND_SYS_BOOT_CAUSE_EXP; Watchdog restart: EXT_ND_SYS_BOOT_CAUSE_WD */
    td_u8 boot_ver;       /* < Bootloader version */
    td_u16 sw_ver;        /* < Software version number */
    td_u16 year : 7;      /* < Version date (year) */
    td_u16 month : 4;     /* < Version date (month) */
    td_u16 day : 5;       /* < Version date (date) */
    td_char manu_code[2]; /* < Manufacturer code 2 byte */
    td_char chip_code[2]; /* < Chip code 2 byte */
} ext_sys_status_info;

typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN];
    td_u8 product_type; /* EXT_ND_TYPE_DCM         1 // Debug control module */
    /* EXT_ND_TYPE_CCO      2 // Headend */
    /* EXT_ND_TYPE_STA      3 // Terminal (common station) */
    /* EXT_ND_TYPE_RELAY    4 // Relay */
    td_u8 pad1;

    td_u16 tei : 12;
    td_u16 level : 4;
    td_u16 proxy_tei : 12;
    td_u16 role : 4; /* EXT_ND_TYPE_XX */

    ext_sys_status_info sta_ver;
} ext_mac_network_topo_entry;

typedef struct {
    td_u8 repeter_level; /* Relay level */
    td_pvoid topo;       /* Pointing to the ext_mac_network_topo_entry structure definition */
} ext_mac_config_info;

#define EXT_HNV_FILE_SIGNATURE uapi_make_identifier('H', 'N', 'V', '$')

/****************************************************************************** */
/* Return result for component voting. The default value is EXT_SYS_ENUM_RST_NO_REQ. */
typedef enum {
    EXT_SYS_ENUM_RST_NO_REQ = 0, /* There is no restart request. */
    EXT_SYS_ENUM_RST_DO_PLEASE,  /* The system needs to be restarted immediately. */
    EXT_SYS_ENUM_RST_NOT_PLEASE, /* The system cannot be restarted. */
    EXT_SYS_ENUM_RST_MAX
} ext_sys_rst_component_vote;

/***************************************************************************** */
typedef struct {
    td_u8 src_mac[EXT_PLC_MAC_ADDR_LEN];
    td_u8 dest_mac[EXT_PLC_MAC_ADDR_LEN];

    td_bool is_exist_mac;   /* Whether the MAC address is carried */
    td_u8 max_resend_count; /* Maximum number of times that a packet is sent. If the value is not 0,  */
    /* the value of this parameter applies. If the value is 0, the default configuration is used. */
    td_u8 lid;
    td_u8 send_mode; /* For details, see ext_mdm_fast_send_mode. */

    td_u32 is_set_send_type : 1; /* Whether the user sets the sending type */
    td_u32 send_type : 3;        /* Sending type set by the user */
    td_u32 is_ee_iv : 1;         /* Encrypted or not */
    td_u32 pad : 1;
    td_u32 form_serial_number : 8;
    /* Routing strategy used for packet sending
     * 0: Adaptive, Tree network or Mesh network;
     * 1: Tree network;
     * 2: Mesh network;
     */
    td_u32 use_network_mode : 2;
    td_u32 pad2 : 16;
} dms_mac_frame_ctrl;

#if defined(HAVE_PCLINT_CHECK)
/* For the convenience of use, PCLINT check is not performed on the macro definition */
#define ext_check_default_id(id) (id)
#else
#define ext_check_default_id(id) (((id) == 0) ? __LINE__ : (id))
#endif
#define ext_diag_log_msg_mk_id_e(id) \
    ext_make_u32(((((td_u16)(ext_check_default_id(id))) << 4) + 0), EXT_DIAG_LOG_MSG_FILE_ID)
#define ext_diag_log_msg_mk_id_w(id) \
    ext_make_u32(((((td_u16)(ext_check_default_id(id))) << 4) + 1), EXT_DIAG_LOG_MSG_FILE_ID)
#define ext_diag_log_msg_mk_id_i(id) \
    ext_make_u32(((((td_u16)(ext_check_default_id(id))) << 4) + 2), EXT_DIAG_LOG_MSG_FILE_ID)
#define ext_diag_log_msg_mk_mod_id(id) ext_make_u32(EXT_DIAG_LOG_MSG_MOD_ID, ext_check_default_id(id))
/***************************************************************************** */
/***************************************************************************** */
#define EXT_DIAG_SYS_INIT_RESET 0
#define EXT_DIAG_SYS_INIT_SET 1

#define EXT_NV_CFG_DISABLE_U16_SWT 0xFFFE
#define EXT_NV_CFG_DISABLE_U32_SWT 0xFFFFFFFE
/***************************************************************************** */
/***************************************************************************** */
/* Frame format definition of the net diagnostic modem (NDM) */
/***************************************************************************** */
typedef struct {
    td_u32 sn;
    td_u16 trans_id;     /* Transmission ID */
    td_u16 control : 15; /* Control word */
    td_u16 du_link : 1;  /* Uplink or downlink ID */

    td_u16 user_data; /* User-defined data. The current version indicates the DBK packet ID. */
    td_u8 prod_type;
    /*
     * Indicates whether the command is converted by the local command.
     * 0: Remote query (ndm->cco->sta); 1: Local query (ndm->cco/sta)
     */
    td_u8 transformed : 1;
    td_u8 pad : 7; /* Reserved */

    td_u8 mac_addr[EXT_PLC_RELAY_NUM_MAX][EXT_PLC_MAC_ADDR_LEN]; /* Destination address */
    td_u8 ndm_mac_addr[EXT_PLC_MAC_ADDR_LEN];                   /* MAC address used by the NDM. The value is 0. */
} ext_nd_remote_frame_head;

/***************************************************************************** */
/* Frame format definition of the NDM */
/***************************************************************************** */
typedef struct {
    td_u32 sn;
    td_u16 trans_id;     /* Transmission ID */
    td_u16 control : 15; /* Control word */
    td_u16 du_link : 1;  /* Uplink or downlink ID */

    td_u16 user_data; /* User-defined data. The current version indicates the DBK packet ID. */
    td_u8 prod_type;
    td_u8 pad; /* Reserved */

    td_u16 fragment_id;
    td_u16 fragment_count : 9; /* Total number of fragmented packets */
    /* 0: Data does not need to be reported in a packet. 1: Data needs to be reported by in a complete packet */
    td_u16 rsb_pkt_flag : 1;
    /* Indicates whether the command is converted by the local command. 0: Remote query; 1: Local query. */
    td_u16 transformed : 1;
    td_u16 pad2 : 5; /* Reserved. The default value is 0. */

    td_u16 fragment_trans_id;
    td_u16 pad3;

    td_u8 mac_addr[EXT_PLC_RELAY_NUM_MAX][EXT_PLC_MAC_ADDR_LEN]; /* Destination address */
    td_u8 ndm_mac_addr[EXT_PLC_MAC_ADDR_LEN];                   /* MAC address used by the NDM. The value is 0. */
} ext_nd_remote_frame_ind_head;

/***************************************************************************** */
/***************************************************************************** */
/* System status definition */
/***************************************************************************** */
#define EXT_ND_NET_STATUS_FORM_FAIL 0
#define EXT_ND_NET_STATUS_FORM_END 1
#define EXT_ND_NET_STATUS_FORMING 2
#define EXT_ND_NET_STATUS_FORM_CHANGED 3
#define EXT_ND_NET_STATUS_FORM_UNKNOWN 0xFF

#define EXT_ND_SYS_ERR_STATUS_UNKNOWN 0xFF
#define EXT_ND_SYS_ERR_STATUS_OK 0x0
#define EXT_ND_SYS_ERR_STATUS_CRASH_FOUND 0x1

#define EXT_ND_SYS_BOOT_CAUSE_NORMAL 0x0        /* Normal restart */
#define EXT_ND_SYS_BOOT_CAUSE_EXP 0x1           /* Exception restart */
#define EXT_ND_SYS_BOOT_CAUSE_WD 0x2            /* Watchdog restart */
#define EXT_ND_SYS_BOOT_CAUSE_UPG_VERIFY 0x3    /* Upgrade verification restart */
#define EXT_ND_SYS_BOOT_CAUSE_UPG_FAIL 0x4      /* Upgrade failure restart */
#define EXT_ND_SYS_BOOT_CAUSE_UPG_BACK_FAIL 0x5 /* Upgrade rollback failure restart */

#define EXT_ND_SYS_UPG_CAUSE_NORMAL 0x01 /* No exception occurs during the upgrade. */
#define EXT_ND_SYS_UPG_CAUSE_FAIL 0x02   /* An error occurs during the upgrade. */

/***************************************************************************** */
#define ID_DIAG_CMD_SEND_FRAME (0x5721)
/***************************************************************************** */
#define EXT_ND_TYPE_DCM 1       /* Debug control module */
#define EXT_ND_TYPE_CCO 2       /* Headend */
#define EXT_ND_TYPE_STA 3       /* Terminal (common station) */
#define EXT_ND_TYPE_RELAY 4     /* Relay */
#define EXT_ND_TYPE_CLTII 5     /* II collection */
#define EXT_ND_TYPE_CLTI 6      /* I collection */
#define EXT_ND_TYPE_THREE_STA 7 /* Three-phase meter module */

#define EXT_ND_DEV_TYPE_METER 0x10     /* Electric meter */
#define EXT_ND_DEV_TYPE_METER_COL 0x11 /* Collector */
#define EXT_ND_DEV_TYPE_CCO 0x12       /* CCO */
#define EXT_ND_DEV_TYPE_UNKNOWN 0xFF   /* Unidentified device */
/***************************************************************************** */
/***************************************************************************** */
#define EXT_DIAG_LD_FILE_OPT_NA 0x00    /* Unspecified option */
#define EXT_DIAG_LD_FILE_OPT_START 0x01 /* ext_lou16(ulOption) */
#define EXT_DIAG_LD_FILE_OPT_END 0x02   /* ext_lou16(ulOption) */
#define EXT_DIAG_LD_FILE_OPT_BLK 0x03   /* ext_lou16(ulOption) */
/***************************************************************************** */
/***************************************************************************** */
#define EXT_DIAG_LD_FILE_OPT_TIMEOUT 0xFF
#define EXT_LOAD_HUPG_FILE_DRV_PROC_ID 0
#define EXT_LOAD_HUPG_FILE_SAL_PROC_ID 1
#define EXT_LOAD_FILE_BQ_PROC_ID 2
#define EXT_LOAD_HUPG_FILE_MDM_PROC_ID 3
#define EXT_LOAD_FILE_PROC_ID_MAX 0xFFFE
#define EXT_LOAD_NV_FILE_PROC_ID 0xFFFE /* the Process ID to burn NV File to UE */
#define EXT_LOAD_INVALID_FILE_PROC_ID 0xFFFF
/***************************************************************************** */
/***************************************************************************** */
#define EXT_MDM_LD_FILE_BLOCK_SIZE (1024)
/***************************************************************************** */
typedef enum {
    EXT_CFG_PERFORMANCE_HIGH, /* High-performance device */
    EXT_CFG_PERFORMANCE_LOW,  /* Low-performance device */
    EXT_CFG_PERFORMANCE_MAX,  /* Maximum enumeration value. */
} ext_cfg_performance;        /* Option value of the device performance in the early stage */

/***************************** DIAG Operation Logs ST********************************************** */
/***************************** DIAG Operation Logs END********************************************** */
/* Definition of basic information items in the basic information database */
#define EXT_MODULE_POSITION_LEN_MAX 64
#define EXT_BIDB_ITEM_NUM_MAX 1 /* 1024 */

#ifdef PRODUCT_CFG_NDM_COLLECT_DATA
/**
 * @ingroup  hct_diag
 * @brief User command processing function.
 *
 * @par Description:
 * The DIAG calls the corresponding command processing function based on the command ID in the received data.
 * @attention None.
 *
 * @param  id[IN] Indicates the command ID.
 * @param  cmd_param[IN] Pointer to the command content.
 * @param  cmd_param_size[IN] Size of the command content.
 * @param  option[IN] Indicates the ulOption command option. When reporting a response packet,
 * the user receives the option and sends it to the corresponding content reporting interface.
 *
 * @retval EXT_ERR_NOT_FOUND cannot be returned. If uapi_diag_send_ack_packet is called to
 * return an ACK message during command processing, EXT_ERR_CONSUMED needs to be returned.
 *
 * @par Dependency:
 * @li soc_mdm_diag.h: Header file where the interface declaration is located.
 * @see  None.
 * @since SocMDL_V100R001C00
 */
typedef td_u32 (*uapi_diag_async_cmd_f)(td_u16 id, td_pvoid cmd_param, td_u16 cmd_param_size, td_u32 option);
#endif

EXT_END_HEADER
#endif /* __SOC_MDM_TYPES_H__ */
