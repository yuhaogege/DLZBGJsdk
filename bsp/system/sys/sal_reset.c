/*
 * Copyright (c) CompanyNameMagicTag 2018-2018. All rights reserved.
 * Description: SAL reset handle.
 * Author: CompanyName
 * Create: 2018-12-15
 */

#include "sal_reset.h"

#include <soc_mdm_types.h>
#include <soc_ft_nv.h>
#include <soc_mdm_sys.h>
#include <soc_stdlib.h>
#include <soc_mdm_random.h>
#include <soc_mdm_time.h>
#include <soc_mdm_watchdog.h>
#include <soc_sal_nv.h>
#include <soc_sal.h>
#include <soc_mdm_crash.h>
#include <soc_mdm_io.h>
#include "soc_system_error.h"

EXT_CPP_START

#define EXT_SAL_RESET_MIN_TIME 1800
#define SAL_RESET_MON_MS (EXT_MONITOR_INTERVAL_MS)
#define SAL_RESET_MON_SEC (EXT_MONITOR_INTERVAL_SEC)

typedef struct {
    ext_sys_reset_notify_fct notify_func; /* Register reset condition notify entry */
    td_u32 condition_stat;               /* Status of the reset condition */
    td_u32 max_time;                     /* Max timeout of the reset condition */
    td_u32 run_time;                     /* Running time when condition enabled */
} sal_reset_notify;

typedef struct {
    td_bool enable;        /* Enable flag */
    td_u8 enable_mask;     /* Enable mask */
    td_u16 rst_delay_flag; /* Reset delay flag */
    sal_reset_notify rst_notify[EXT_RST_CON_MAX];
    td_u32 delay_time_tbl[EXT_RST_DELAY_CON_MAX];
} sal_reset_proc_ctx;

EXT_PRV sal_reset_proc_ctx g_rst_proc_ctx = {
    TD_FALSE, 0, 0,
    {
        { TD_NULL, 0, 0, 0 },
        { TD_NULL, 0, 0, 0 },
        { TD_NULL, 0, 0, 0 },
        { TD_NULL, 0, 0, 0 },
        { TD_NULL, 0, 0, 0 },
        { TD_NULL, 0, 0, 0 },
        { TD_NULL, 0, 0, 0 },
        { TD_NULL, 0, 0, 0 },
    },
    { 0 }
};

ext_sys_reboot_cause g_reboot_cause = EXT_SYS_REBOOT_CAUSE_MAX;

#if defined(PRODUCT_CFG_SYS_REBOOT_DELAY)
td_u32 g_reboot_delay = PRODUCT_CFG_SYS_REBOOT_DELAY;
#else
td_u32 g_reboot_delay = 500; /* 500ms, enough to write 4k flash */
#endif

EXT_PRV td_bool reset_check(sal_reset_proc_ctx *ctx, ext_rst_con *condition);
EXT_PRV td_bool reset_condition_check(EXT_CONST sal_reset_proc_ctx *ctx, ext_rst_con *condition);
EXT_PRV td_u32 get_reset_delay_sec(EXT_CONST sal_reset_proc_ctx *ctx);
EXT_PRV td_u32 secure_reset_callback(td_void);
td_void uapi_sys_pre_reboot(td_void);

td_void uapi_sys_reset_init(td_void)
{
    td_u32 ret;
    ext_nv_reset_cfg_id cfg;
    sal_reset_notify *rst_notify = g_rst_proc_ctx.rst_notify;
    td_u8 i;

    (td_void) memset_s(&cfg, sizeof(cfg), 0, sizeof(cfg));
    ret = uapi_nv_read(EXT_NV_SYS_RST_CFG_ID, &cfg, sizeof(cfg));
    if (ret != EXT_ERR_SUCCESS) {
        /* Reset turn off when NV read failed */
        g_rst_proc_ctx.enable = TD_FALSE;
        return;
    }

    if (cfg.enable_rst == 0) {
        g_rst_proc_ctx.enable = TD_FALSE;
        return;
    }

    /* Enable reset functions as default */
    g_rst_proc_ctx.enable = TD_TRUE;
    g_rst_proc_ctx.enable_mask = 0xFF;

    rst_notify[EXT_RST_CON_CCO_MAX_PROXY].max_time = cfg.max_proxy_time;
    rst_notify[EXT_RST_CON_PLC_NO_TX].max_time = cfg.plc_no_tx_time;
    rst_notify[EXT_RST_CON_PLC_NO_RX].max_time = cfg.plc_no_rx_time;
    rst_notify[EXT_RST_CON_STA_NO_JOIN_NETWORK].max_time = cfg.max_abnormal_time;
    rst_notify[EXT_RST_CON_NO_ROUTE_EVALUATE].max_time = cfg.max_abnormal_time;
    rst_notify[EXT_RST_CON_USR0].max_time = cfg.max_time_usr0;
    rst_notify[EXT_RST_CON_USR1].max_time = cfg.max_time_usr1;

    if ((cfg.secure_begin_time == 0) && (cfg.secure_end_time == 0)) {
        rst_notify[EXT_RST_CON_STA_SECURE_RESET].max_time = 0;
    } else if (cfg.secure_begin_time > cfg.secure_end_time) {
        rst_notify[EXT_RST_CON_STA_SECURE_RESET].max_time = EXT_RST_STA_SECURE_TIME;
        rst_notify[EXT_RST_CON_STA_SECURE_RESET].notify_func = secure_reset_callback;
    } else {
        rst_notify[EXT_RST_CON_STA_SECURE_RESET].max_time =
            uapi_get_random_num32(cfg.secure_begin_time, cfg.secure_end_time);
        rst_notify[EXT_RST_CON_STA_SECURE_RESET].notify_func = secure_reset_callback;
    }

    for (i = 0; i < EXT_RST_CON_MAX; i++) {
        if ((rst_notify[i].max_time > 0) && (rst_notify[i].max_time < EXT_SAL_RESET_MIN_TIME)) {
            rst_notify[i].max_time = EXT_SAL_RESET_MIN_TIME;
        }
    }
}

td_void uapi_sys_enable_reset(EXT_CONST ext_rst_con condition, EXT_CONST td_bool enable)
{
    td_u8 val = g_rst_proc_ctx.enable_mask;
    td_u32 temp_mask = condition;

    if ((g_rst_proc_ctx.enable == TD_FALSE) || (condition >= EXT_RST_CON_MAX)) {
        return;
    }

    if (enable == TD_TRUE) {
        val |= (td_u8)(1u << temp_mask); /* Set 1 to enable event */
    } else {
        val &= ~(td_u8)(1u << temp_mask); /* Set 0 to disable event */
    }
    g_rst_proc_ctx.enable_mask = val;
}

td_u32 uapi_sys_register_reset_notify(EXT_CONST ext_sys_reset_notify_fct notify_func, ext_rst_con condition)
{
    if ((notify_func == TD_NULL) || (condition >= EXT_RST_CON_MAX)) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    if (g_rst_proc_ctx.rst_notify[condition].max_time == 0) {
        g_rst_proc_ctx.rst_notify[condition].notify_func = TD_NULL;
    } else {
        g_rst_proc_ctx.rst_notify[condition].notify_func = notify_func;
    }

    return EXT_ERR_SUCCESS;
}

td_u32 uapi_sys_reset_delay_enable(ext_rst_delay_con condition, td_u32 ms)
{
    td_u32 cur_sec = uapi_get_seconds();
    td_u32 sec;

    if (condition >= EXT_RST_DELAY_CON_MAX) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    /* Reset delay must be less than 5 minutes */
    if ((condition <= EXT_RST_DELAY_CON_USR_END) && (ms > EXT_RST_DELAY_USR_TIME_MAX)) {
        return EXT_ERR_TOO_LARGE_DATA;
    }

    /* Round-up */
    if (ms + SAL_RESET_MON_MS + 999 < ms) { /* Add 999£¬round-up */
        /* Turning over */
        ms = 0xFFFFFFFF;
    } else {
        ms += SAL_RESET_MON_MS + 999; /* Add 999£¬round-up */
    }

    sec = ms / 1000; /* Convert to second del 1000 */
    if ((cur_sec + sec < cur_sec) || (cur_sec + sec < sec)) {
        g_rst_proc_ctx.delay_time_tbl[condition] = 0xFFFFFFFF;
    } else {
        g_rst_proc_ctx.delay_time_tbl[condition] = cur_sec + sec;
    }

    g_rst_proc_ctx.rst_delay_flag |= 1 << (td_u32)condition;

    return EXT_ERR_SUCCESS;
}

td_u32 uapi_sys_reset_delay_disable(ext_rst_delay_con condition)
{
    if (condition >= EXT_RST_DELAY_CON_MAX) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    g_rst_proc_ctx.rst_delay_flag &= ((1 << (td_u32)condition) ^ 0xFFFFFFFF);

    return EXT_ERR_SUCCESS;
}

td_u32 sal_reset_monitor(td_u32 cur_sec)
{
    ext_rst_con condition = EXT_RST_CON_MAX;
    td_u32 permit_reset_sec;

    if (g_rst_proc_ctx.enable == TD_FALSE) {
        return EXT_ERR_SUCCESS;
    }

    if (g_reboot_cause >= EXT_SYS_REBOOT_CAUSE_MAX) {
        /* Checking condition */
        if (reset_check(&g_rst_proc_ctx, &condition) == TD_FALSE) {
            /* Not reach the condition */
            return EXT_ERR_SUCCESS;
        }
    }

    /* Get delay reset settings */
    permit_reset_sec = get_reset_delay_sec(&g_rst_proc_ctx);
    if (cur_sec >= permit_reset_sec) {
        if (g_reboot_cause < EXT_SYS_REBOOT_CAUSE_MAX) {
            uapi_sys_reboot(g_reboot_cause);
        } else {
            td_u32 cause = (td_u32)EXT_SYS_REBOOT_CAUSE_RST0;
            cause += (td_u32)condition;
            uapi_sys_reboot(cause);
        }
    }

    return EXT_ERR_SUCCESS;
}

EXT_PRV td_bool reset_check(sal_reset_proc_ctx *ctx, ext_rst_con *condition)
{
    if (g_reboot_cause < EXT_SYS_REBOOT_CAUSE_MAX) {
        return TD_TRUE;
    }

    if (ctx->enable != TD_TRUE) {
        return TD_FALSE;
    }

    return reset_condition_check(ctx, condition);
}

EXT_PRV td_bool reset_condition_check(EXT_CONST sal_reset_proc_ctx *ctx, ext_rst_con *condition)
{
    td_u32 i;
    td_u32 tmp_stat;
    sal_reset_notify *notify = TD_NULL;

    *condition = EXT_RST_CON_MAX;

    for (i = 0; i < EXT_RST_CON_MAX; i++) {
        if (((1 << i) & ctx->enable_mask) == 0) { /* Event was disabled */
            continue;
        }

        notify = (sal_reset_notify*)ctx->rst_notify + i;
        if (notify->notify_func == TD_NULL) {
            continue;
        }

        tmp_stat = notify->notify_func();
        if (notify->condition_stat != tmp_stat) {
            notify->condition_stat = tmp_stat;
            notify->run_time = 0;
            continue;
        }

        notify->run_time += SAL_RESET_MON_SEC;
        if (notify->run_time >= notify->max_time) {
            *condition = (ext_rst_con)i;
            return TD_TRUE;
        }
    }

    return TD_FALSE;
}

EXT_PRV td_u32 get_reset_delay_sec(EXT_CONST sal_reset_proc_ctx *ctx)
{
    td_u32 time_sec = 0;
    td_u32 i = 0;

    for (i = 0; i < EXT_RST_DELAY_CON_MAX; i++) {
        if (ctx->rst_delay_flag & (1 << i)) {
            if (time_sec < ctx->delay_time_tbl[i]) {
                time_sec = ctx->delay_time_tbl[i];
            }
        }
    }

    return time_sec;
}

td_u32 uapi_usr_reboot(ext_sys_reboot_cause cause)
{
    if ((cause < EXT_SYS_REBOOT_CAUSE_USR_BEGIN) || (cause > EXT_SYS_REBOOT_CAUSE_USR_END)) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    g_reboot_cause = cause;
    return EXT_ERR_SUCCESS;
}

__hot td_void uapi_sys_pre_reboot(td_void)
{
    /* Resolve the problem that chip entries jtag after rebooting.
      1.Chipset entries jtag if it detects pin EXT_GPIO_IDX_14 was high value after rebooting.
      2.HW add electric capacity to anti-ESD, when EXT_GPIO_14 that set to be uart1 tx pin set to be high as default.
      3.Chipset entries jtag if the electric capacity still contains power when rebooting.
 */
    uapi_io_set_func(EXT_GPIO_IDX_14, 1);
    uapi_io_set_dir(EXT_GPIO_IDX_14, EXT_GPIO_DIRECTION_IN);
    uapi_io_set_ouput_val(EXT_GPIO_IDX_14, EXT_GPIO_VALUE0);
    uapi_udelay(20); /* Must delay above 20us */
}

__hot td_u32 uapi_sys_reboot(td_u32 cause)
{
    if ((cause == EXT_SYS_REBOOT_CAUSE_UNKNOWN) || (cause >= EXT_SYS_REBOOT_CAUSE_MAX)) {
        return EXT_ERR_INVALID_PARAMETER;
    } else {
#ifdef PRODUCT_CFG_PRINT_UART0
        uapi_io_set_func(EXT_GPIO_IDX_14, 1);
        uapi_io_set_dir(EXT_GPIO_IDX_14, 1);
        uapi_io_set_ouput_val(EXT_GPIO_IDX_14, 0);
#endif

        uapi_int_lock();
        uapi_watchdog_feed();
        uapi_syserr_save_cmn_info_step1(EXT_EID_SYS_REBOOT, (td_u16)cause);
        uapi_syserr_save_cmn_info_step2(EXT_EID_SYS_REBOOT, (td_u16)cause);
        uapi_sys_pre_reboot();
        uapi_watchdog_register(EXT_WDG_NORMAL, TD_NULL, g_reboot_delay);

        for (;;) { }
    }

    return EXT_ERR_SUCCESS;
}

EXT_PRV td_u32 secure_reset_callback(td_void)
{
    return 0;
}

EXT_CPP_END
