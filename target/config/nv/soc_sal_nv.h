/*
 * Copyright (c) CompanyNameMagicTag 2018-2018. All rights reserved.
 * Description: SAL NV structure define.
 * Author: CompanyName
 * Create: 2018-12-15
 */

#ifndef __SOC_SAL_NV_H__
#define __SOC_SAL_NV_H__

#define EXT_NV_SYS_STARTUP_ID (0x34) /* *< EXT_NV_SYS_STARTUP_S */
#define EXT_NV_SYS_RST_CFG_ID (0x35) /* *< EXT_NV_RESET_CFG_ID_S */
#define EXT_NV_SYS_RST_TIMES (0x36)  /* *< EXT_SYS_RESET_TIMES_S */
#define EXT_NV_SYS_TRANS_HOST (0x37) /* *< EXT_SYS_RESET_TIMES_S */

/* Active reset configuration */
typedef struct {
    td_u8 enable_rst;         /* *< active reset enable switch */
    td_u8 rsv[3];             /* pad 3 byte */
    td_u32 max_proxy_time;    /* *< Maximum number of agents duration, in seconds: */
    td_u32 plc_no_tx_time;    /* *< plc no transmission duration, unit: second */
    td_u32 plc_no_rx_time;    /* *< plc no reception duration, unit: second */
    td_u32 max_abnormal_time; /* *< MAC protocol stack abnormal restart time upper limit, unit: second */
    td_u32 secure_begin_time; /* *< Periodic restart time lower limit, unit: second */
    td_u32 secure_end_time;   /* *< Periodic restart time limit, in seconds */
    td_u32 max_time_usr0;     /* *< User reserved 0 time threshold in seconds */
    td_u32 max_time_usr1;     /* *< User reserved 1 time threshold in seconds */
} ext_nv_reset_cfg_id;

/* module reset count */
typedef struct {
    td_u32 rst_times; /* *< restart count statistics */
} ext_sys_reset_times;

typedef struct {
    td_u32 lightsleep : 1;
    td_u32 deepsleep : 1;
    td_u32 reserve : 30;
} ext_nv_sys_startup;

typedef struct {
    td_u32 addr;
    td_u32 size;
    td_u32 crc;
} ext_trans_host_nv;

#endif /* __SOC_SAL_NV_H__ */
