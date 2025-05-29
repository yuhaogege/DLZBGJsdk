/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: NV item settings in factory mode.
 * Author: CompanyName
 * Create: 2018-6-9
 */

#ifndef FT_NV_H
#define FT_NV_H
#include <soc_partition_table.h>

/* ***************************************************************************** */
/* The value range of id NV ID [0x00, 0x19],The user can be added to EXT_NV_FTM_ID_END */
/* ***************************************************************************** */
#define EXT_NV_FTM_ID_START 0x0
#define EXT_NV_FTM_ID_END 0x19

#define EXT_NV_FTM_MANUFACOTR_ID 0x01 /* EXT_NV_FTM_MANUFACOTR_ID_S */
#define EXT_NV_FTM_RESERVE_ID_2 0x02
#define EXT_NV_FTM_PRODUCT_ID 0x03 /* EXT_NV_FTM_PRODUCT_ID_S */
#define EXT_NV_FTM_FLASH_PARTIRION_TABLE_ID (0x04)
#define EXT_NV_FTM_FLASH_PARTIRION_TABLE_B_ID (0x05) /* Partition table for transforming addresses */
#define EXT_NV_FTM_FLASH_PARTIRION_CHANGE_ID (0x06)  /* Transform the address structure */
#define EXT_NV_FTM_UPG_EXTENSION_ID (0x07)
#define EXT_NV_FTM_RESERVE_ID_8 (0x08)
#define EXT_NV_FTM_RESERVE_ID_9 (0x09)
#define EXT_NV_FTM_RESERVE_ID_10 (0x0a)
#define EXT_NV_FTM_RESERVE_ID_11 (0x0b)
#define EXT_NV_FTM_RESERVE_ID_12 (0x0c)
#define EXT_NV_FTM_RESERVE_ID_13 (0x0d)
#define EXT_NV_FTM_RESERVE_ID_14 (0x0e)
#define EXT_NV_FTM_RESERVE_ID_15 (0x0f)
#define EXT_NV_FTM_RESERVE_ID_16 (0x10)
#define EXT_NV_FTM_RESERVE_ID_17 (0x11)

/* The following is changed to a non-factory area, to be removed */
#define EXT_NV_FTM_SOC_INFO_CFG_ID 0x80
#define EXT_NV_FTM_STARTUP_CFG_ID 0x85   /* EXT_NV_FTM_STARTUP_CFG_S */
#define EXT_NV_FTM_UPG_START_CFG_ID 0x86 /* EXT_NV_FTM_UPG_START_CFG_S */
#define EXT_NV_FTM_UPG_CFG_ID 0x87       /* EXT_NV_FTM_UPG_CFG_S */

#define ID_DIAG_CMD_CONFIG_NM_SERIAL_NUMBER (0x88)
#define EXT_NV_FTM_NM_INIT_PARA 0x89 /* EXT_NV_FTM_RESERVED1_S */
#define EXT_NV_FTM_UPG (0x8b)

inline td_bool uapi_is_nv_ftm(td_u16 x)
{
    return (x < EXT_NV_FTM_ID_END);
}

typedef struct {
    td_char id[3];      /* vendor code, 3 len */
    td_char chip_id[5]; /* chip code, default is "11", 5 len */
} ext_nv_ftm_manufacotr_id;

#define EXT_FTM_PRODUCT_TYPE_BASE 0
#define EXT_FTM_PRODUCT_TYPE_CCO_BEGIN 1
#define EXT_FTM_PRODUCT_TYPE_CCO_END 60
#define EXT_FTM_PRODUCT_TYPE_NDM_BEGIN 61
#define EXT_FTM_PRODUCT_TYPE_NDM_END 100
#define EXT_FTM_PRODUCT_TYPE_STA_BEGIN 101
#define EXT_FTM_PRODUCT_TYPE_STA_END 8191

/* The maximum value is 16 bits. */
typedef enum {
    EXT_FTM_PRODUCT_TYPE_UNKOWN = 0,

    /* CCO id - range [1, 60]. */
    EXT_FTM_PRODUCT_TYPE_C01 = EXT_FTM_PRODUCT_TYPE_CCO_BEGIN,
    EXT_FTM_PRODUCT_TYPE_C02,
    EXT_FTM_PRODUCT_TYPE_C03,
    EXT_FTM_PRODUCT_TYPE_C04,
    EXT_FTM_PRODUCT_TYPE_C05,
    EXT_FTM_PRODUCT_TYPE_C06,
    EXT_FTM_PRODUCT_TYPE_C07,
    EXT_FTM_PRODUCT_TYPE_C08,

    EXT_FTM_PRODUCT_TYPE_C60 = EXT_FTM_PRODUCT_TYPE_CCO_END, /* End of CCO ID */

    /* NDM id - range [61, 100]. */
    EXT_FTM_PRODUCT_TYPE_D61 = EXT_FTM_PRODUCT_TYPE_NDM_BEGIN,
    EXT_FTM_PRODUCT_TYPE_D62,
    EXT_FTM_PRODUCT_TYPE_D63,
    EXT_FTM_PRODUCT_TYPE_D64,

    EXT_FTM_PRODUCT_TYPE_D100 = EXT_FTM_PRODUCT_TYPE_NDM_END, /* End of NDM ID */

    /* STA id - range [101, 8191]. */
    EXT_FTM_PRODUCT_TYPE_M101 = EXT_FTM_PRODUCT_TYPE_STA_BEGIN,
    EXT_FTM_PRODUCT_TYPE_M102,
    EXT_FTM_PRODUCT_TYPE_M103,
    EXT_FTM_PRODUCT_TYPE_M104,
    EXT_FTM_PRODUCT_TYPE_M105,
    EXT_FTM_PRODUCT_TYPE_M106,
    EXT_FTM_PRODUCT_TYPE_M107,
    EXT_FTM_PRODUCT_TYPE_M108,

    EXT_FTM_PRODUCT_TYPE_M8191 = EXT_FTM_PRODUCT_TYPE_STA_END, /* End of STA ID */
} ext_ftm_product_type;

typedef struct {
    ext_ftm_product_type product_type;   /* *< product form */
    td_u8 plc_mac[EXT_PLC_MAC_ADDR_LEN]; /* *< communication module mac address */
    td_u8 reserved[6];                  /* pad 6 byte */
} ext_nv_ftm_product_id;

#define EXT_SYS_TEST_MODE_EQM_NONE 0
#define EXT_SYS_TEST_MODE_EQM_STA_M 1
#define EXT_SYS_TEST_MODE_EQM_STA_IIC 2
#define EXT_SYS_TEST_MODE_EQM_NDM 4
#define EXT_SYS_TEST_MODE_EQM_NSTA 8

#define DRV_UPG_CLEAR 0
#define DRV_UPG_NV_ALONE 1
#define DRV_UPG_CODE_NV 2
#define DRV_UPG_BOOT 3

typedef struct {
    td_u8 data[64]; /* 64 数据长度 */
    td_u32 rsv[16]; /* 16 数据长度 */
} ext_nv_ftm_soc_cfg;

typedef struct {
    td_u32 status; /* ext_nd_sys_status_normal ext_nd_sys_status_upgrade ext_nd_sys_status_test ext_nd_sys_status_ */
    td_u32 load_flash_addr; /* boot area address */
    td_u32 last_blk_idx;    /* index number of the last consecutive valid upgrade file block */
    td_u32 rcv_file_size; /* the length of the upgrade file received(refers to the size of consecutive valid blocks) */
    td_u32 upg_id;
    td_u32 upg_window_time;
    td_u32 back_times; /* record the number of rollbacks */
    td_u32 runtimes;   /* the boot is used to record the number of times the os and the application are started. */

    td_u8 upg_target;    /* upgrade nv, or code+nv */
    td_u8 test_mode;     /* ext_sys_test_mode_eqm_sta_m, ext_sys_test_mode_eqm_* */
    td_u8 decompress_nv; /* decompress nv when the flag bit 0x55 is read */
    td_u8 reserved1[1];

    td_u32 reserved2;
} ext_nv_ftm_startup_cfg;

/* upgrade start information, sent from the start command */
typedef struct {
    td_u32 option;          /* upgrade option */
    td_u32 upg_id;          /* upgrade id */
    td_u32 file_len;        /* length of the upgrade file */
    td_u32 blk_num;         /* block 512 bytes is a unit */
    td_u32 upg_time_window; /* upgrade time window */
    td_u32 last_block_id;   /* id of the previous block */
    td_u32 crc;
} ext_nv_ftm_upg_start_cfg;

/* upgrade the transceiver configuration, detailed information reference
<<dw11v100 b017 software upgrade program design. docx>> document description */
typedef struct {
    td_u16 total_number_when_start; /* the number of sites in the entire network when starting the upgrade */
    td_u16 extra_timer_max_count;   /* set the maximum number of timers */
    td_u16 extra_timer_period;      /* the whole network determines the end of the upgrade:
    set the timer duration after networking optimization; */
    td_u16 upg_verify_new_policy;   /* whether to start a new upgrade verification policy,
    the original policy is used by default */
    td_u16 total_number_threshold;  /* start the timer threshold */
    td_u16 rcv_timeout_timer_period;
    td_u16 blk_send_period; /* default 100ms */
    td_u8 reserved2;
    td_u8 upg_type;                 /* when the upgrade type is boot, the value is 0xab */
    td_u8 device_upg_status;        /* device upgrade status, if the cco/sta is restarted during the upgrade process,
    it is easy to identify the device upgrade status after startup. */
    td_u8 transmit_blk_num;         /* default 1 (optional 7, 24) */
    td_u16 mac_tx_arq_period;       /* mac send retry expired configuration, in seconds. */
    td_u16 start_tx_period_num;     /* after the number of transmissions, start reissue */
    td_u16 transmit_blk_resend_num; /* the number of consecutive retries for the block */

    td_u16 broadcast_stop_num;    /* default 30 */
    td_u16 broadcast_stop_period; /* default 100 ms */

    td_u16 sta_dfx_report;          /* the status reported by the sta to the cco itself */
    td_u16 cco_dfx_report;          /* received by cco to the dbk network */
    td_u16 broadcast_finish_num;    /* default 30 */
    td_u16 broadcast_finish_period; /* default 100ms */
    td_u16 reserved;                /* force-upg broadcast sn */

    td_u16 whether_upg_test_mode; /* cco send upg test mode to sta by this element, 0 for normal upg, 1 for test mode */
} ext_nv_ftm_upg_cfg;

typedef struct _diag_cmd_config_nm_serial_number_ {
    td_u32 form_serial_number;
    td_u32 assoc_random_number;
} diag_cmd_config_nm_serial_number_stru;

/* NMS parameters of the factory area of the network management */

typedef struct {
    td_u8 main_network_snid; /* main network network number */
    td_u8 init_freq;         /* initialize the band */
    td_u8 sub_network_snid;  /* subnet network number */
    td_u8 network_mode;      /* network mode */

    td_u8 network_mode_policy;      /* network mode policy */
    td_u8 preamble_seq_shift_index; /* preamble sequence shift factor index */
    td_u8 rf_channel;               /* rf channel */
    td_u8 rf_rate;                  /* rf rate */

    td_u8 is_dual_module;  /* whether it is a dual mode module, only for measurable */
    td_u8 tx_power;        /* rf transmit power */
    td_u8 modem_type;      /* rf modulation type 2fsk: 0, 2gfsk: 1 */
    td_u8 white_list_mode; /* whitelist mode */

    td_u8 sta_type;            /* sta type */
    td_u8 recover_sg_freq;     /* frequency band when reverting to the national network protocol */
    td_s16 correction_voltage; /* zero crossing correction voltage */

    td_u32 main_network_nid;
    td_u32 lock_snid;

    td_u8 belong_cco_mac[EXT_PLC_MAC_ADDR_LEN]; /* manually set the mac address of the home cco */
    td_u8 history_porxy_flag;
    td_u8 history_level;

    td_u32 mesh_lock_nid;
    td_u8 main_1_mac[EXT_PLC_MAC_ADDR_LEN];
    td_u8 main_2_mac[EXT_PLC_MAC_ADDR_LEN];
    td_u32 reserved1[4]; /* pad 4 * 4 count */
} ext_nv_ftm_nm_init_para;

typedef struct {
    /* upgrade file itself information */
    td_u32 flash_addr; /* compatible with the 11 protocol, used to deliver upgrade conclusions */
    td_u32 flash_size; /* compatible with the 11 protocol, used to deliver upgrade conclusions */
    td_u32 crc;        /* compatible with the 11 protocol, used to deliver upgrade conclusions */
    td_u8 upg_mode;    /* upgrade type */
    td_u8 do_upgrade : 1;
    td_u8 pad1 : 7;
    td_u8 pad2[6]; /* pad 6 byte */
} ext_upg_mode_nv;

typedef struct {
    td_u32 magic;
    td_u8 cur_times;
    td_u8 dst_times;
    td_u8 data[26]; /* 26 count */
} ext_nv_ftm_upg_extension;

typedef struct {
    td_u32 reserved[32]; /* pad 32*4 byte */
} ext_nv_ftm_reserved_id;

typedef struct {
    td_u32 reserved[16]; /* pad 16*4 byte */
} ext_nv_ftm_reserved1_id;

typedef struct {
    td_u32 reserved[16]; /* pad 16*4 byte */
} ext_nv_ftm_reserved2_id;

typedef struct {
    td_u32 reserved[16]; /* pad 16*4 byte */
} ext_nv_ftm_reserved3_id;

typedef struct {
    td_u32 reserved[16]; /* pad 16*4 byte */
} ext_nv_ftm_reserved4_id;

typedef struct {
    td_u32 reserved[16]; /* pad 16*4 byte */
} ext_nv_ftm_reserved5_id;

typedef struct {
    td_u32 reserved[16]; /* pad 16*4 byte */
} ext_nv_ftm_reserved8;

typedef struct {
    td_u32 reserved[16]; /* pad 16*4 byte */
} ext_nv_ftm_reserved9;
/* ***************************************************************************** */
/* ***************************************************************************** */
#endif /* __FT_NV_H__ */
