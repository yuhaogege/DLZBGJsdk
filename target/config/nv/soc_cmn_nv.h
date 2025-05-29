/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description:  COMMON NV item setting
 * Author: CompanyName
 * Create: 2018-10-31
 */
#ifndef SOC_CMN_NV_H
#define SOC_CMN_NV_H

/* NV ID define [0x0120-0x0150) */
#define NDM_DC_FOLDER_NAME_SIZE (21)
#define NV_ID_SECURE_CONN (0x8c) /* Secure connection parameter */
#define NV_ID_TEST (0x75)        /* Test parameters:Contains uart configuration */

/* NV item data structure definition */
typedef struct {
    td_u32 ram_size;    /* blind start address */
    td_s32 agc_gain;    /* gain is effective for blind mining */
    td_u16 triger_type; /* pick type */
    td_u16 period_time; /* blind mining period, in minutes */
    td_u16 triger_time; /* how long does it take to receive pb fast? */
    td_u16 name_id;     /* each time the button is activated, the number is +1 */

    td_u8 times_per_period; /* blind how many acquisitions are performed continuously in each cycle. */
    td_u8 c_power_on_eable; /* restart the continuous mining function enable switch */
    td_u8 collecting;       /* whether the number is in progress */
    td_u8 is_follow_mode;
} ext_ndm_data_collect_nv;

typedef struct {
    ext_ndm_data_collect_nv data_collect_nv;

    td_u8 spi_write_time_out;
    td_u8 spi_read_time_out;
    td_u8 sd_full_led_fresh_duration;
    /* Temperature Threshold is the low temperature to turn off the wifi temperature point,
    the low temperature to restore the wifi */
    /* temperature point, the high temperature to restore the wifi temperature point,
    and the high temperature to turn off the wifi temperature point. */
    td_s8 ndm_temp_thres[4]; /* 4count */
    td_char sc_folder_name[NDM_DC_FOLDER_NAME_SIZE];

    td_u8 ccomac[EXT_PLC_MAC_ADDR_LEN]; /* Followed by the CCO's MAC address */
    td_u8 freq;                        /* Follow the network freq */
    td_u8 is_fix_freq;

    td_u8 snid;
    td_u8 reserved[3];  /* pad 3 byte */
    td_u8 target_ip[4]; /* 3518 side IP address,4count */
} ext_nv_cmn_cfg_id;

typedef struct {
    td_u8 conn_vertify[128]; /* 128 byte */
    td_u8 conn_salt[20];     /* 20 count */
    td_u32 salt_len;
} ext_nv_secure_conn;
/* ***************************************************************************** */
typedef struct {
    td_u8 uart_cfg : 1; /* 0:app uart0¡¢diag:uart1; 1:app uart1¡¢diag:uart0 */
    td_u8 pad1 : 7;
    td_u8 pad2;
    td_u16 pad3;
    td_u32 pad4;
} ext_nv_test;

/* ***************************************************************************** */
#endif /* SOC_CMN_NV_H */
