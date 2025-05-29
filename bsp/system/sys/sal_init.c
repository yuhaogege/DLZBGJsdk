/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Implementation of sal module init stage.
 * Author: CompanyName
 * Create: 2012-07-14
 */

#include "sal_init.h"
#include "sal_common.h"
#include "sal_monitor.h"
#include "sal_reset.h"
#include "sal_sys.h"
#include "soc_upg_api.h"

#include <sal_inf.h>
#include <soc_mdm.h>
#include <soc_partition_table.h>
#include <dw21_platform.h>
#include <soc_mdm_hrtimer.h>
#include <soc_mdm_crash.h>
#if defined PRODUCT_CFG_NDM_COLLECT_DATA
#include <soc_diag_async.h>
#include <usr_dc_init.h>
#endif
#define SAL_EXP_MON_TASK_STACK_SIZE 1024
#define SAL_EXP_MON_PRIORITY_INIT 3
#define SAL_EXP_MON_PRIORITY_NORMAL 30
#define BLOCK_SIZE PRODUCT_CFG_FLASH_BLOCK_SIZE
#define PRODUCT_CFG_AUTO_RESET_SYSTEM_TIMEOUT 15000 /* bsp\inc */

#if defined(PRODUCT_CFG_AUTO_RESET_SYSTEM_TIMEOUT)
#ifdef UT_SWITCH
EXT_PRV td_u32 g_auto_reset_system_time_out_cfg = PRODUCT_CFG_AUTO_RESET_SYSTEM_TIMEOUT;
#else
__attribute__((unused)) EXT_PRV td_u32 g_auto_reset_system_time_out_cfg = PRODUCT_CFG_AUTO_RESET_SYSTEM_TIMEOUT;
#endif
#define SAL_CHK_WD_ACTIVE_TO (g_auto_reset_system_time_out_cfg)
#endif

#define WD_TIMEOUT_MIN 5 /* ms */

#define check_wd_timerout_enable() (SAL_CHK_WD_ACTIVE_TO > WD_TIMEOUT_MIN)

/* version/product information definition constant */
EXT_EXTERN EXT_CONST ext_ue_product_ver g_product_version;
EXT_EXTERN EXT_CONST ext_product_info g_product_info;

#if defined(PRODUCT_CFG_HAVE_FEATURE_SYS_ERR_INFO1)
#if defined(EXT_HAVE_CROSS_COMPILER_ARM_ARMCC)
#pragma arm section rwdata = ".daigdata"
#endif
#if defined(EXT_HAVE_CROSS_COMPILER_ARM_ARMCC)
#pragma arm section rwdata
#endif
#endif

EXT_EXTERN td_u32 dms_init(td_void);
EXT_EXTERN td_void uapi_drv_dfx_init(td_void);
EXT_PRV td_u32 sal_exp_task_init(td_void);
EXT_PRV td_void sal_exp_task_body(td_u32 param);
EXT_PRV td_void sal_init_sys_phase2(td_void);

td_u32 g_sal_exp_task_id = 0;

EXT_PRV td_u32 sal_exp_task_init(td_void)
{
    return uapi_task_create(&g_sal_exp_task_id, "EXP_MON", sal_exp_task_body, 0, SAL_EXP_MON_TASK_STACK_SIZE,
        SAL_EXP_MON_PRIORITY_INIT);
}

EXT_PRV td_void sal_exp_task_body(td_u32 param)
{
    ext_unref_param(param);

    sal_init_sys_phase2();
    uapi_task_set_priority(g_sal_exp_task_id, SAL_EXP_MON_PRIORITY_NORMAL);

    for (;;) {
        if (check_wd_timerout_enable()) {
            sal_monitor();
            uapi_sleep(EXT_MONITOR_INTERVAL_MS);
        } else {
            uapi_sleep(60 * 1000); /* Sleep 60*1000ms */
        }
    }
}

/* product version number, data structure format */
EXT_CONST ext_ue_product_ver *sal_get_product_version(td_void)
{
    return &g_product_version;
}

/* Product information, such as chip name, version number, etc. String format */
__hot EXT_CONST ext_product_info *uapi_get_product_info(td_void)
{
    return &g_product_info;
}

/* function name: sal_init */
/* Description of function: SAL internal context, HEAP, DIAG, semaphore, */
/* event, queue, initialization of task creation (startup), */
/* System level initialization of DRV, ASL, MAC, DSP code loading, etc. */
/* salCtxInit(NV+RAM+FLASH) --> Heap --> DIAG --> OS RES-->DFX-->RESET MGR-->SDM ENG --> TASK --> */
/* Parameter Description: */
/* pCacheMemPool [in] : The first memory address available in the system */
/* If the parameter is zero, it can only be used in the simulation environment, otherwise the initialization fails. */
/* return value: */
/* EXT_ERR_SUCCESS : Success */
/* other values are error codes */
/* Call request: This function is called after the system is powered on and initialized. */
/* Call example: After the OS starts, call the interface */
/* Author: CompanyName */
EXT_PRV td_u32 sal_init(td_void)
{
    td_u32 ret;
    ext_flash_partition_table *table = TD_NULL;

    (td_void)uapi_hash_init();
    uapi_watchdog_feed();

    /* SAL context initialization, please do not adjust the following order */
    table = uapi_get_partition_table();

    ret = uapi_nv_init(table->table[EXT_FLASH_PARTITON_NV_FILE].addr_blk * BLOCK_SIZE,
        table->table[EXT_FLASH_PARTITON_NV_BACKUP].addr_blk * BLOCK_SIZE);
    printf("[INFO][NV_START_INIT]ret:%d\r\n", ret);

    uapi_upg_base_init(); /* SAL context initialization, please do not adjust the following order */

    ret += dms_init();

    uapi_hrtimer_init(Timer0_IRQn, TIMER0_REG_BASE);

    uapi_rst_times_init();

    sal_dfx_init();

    uapi_drv_dfx_init();
#if defined PRODUCT_CFG_NDM_COLLECT_DATA
    sal_diag_async_task_init();
    usr_dc_init();
#endif
    uapi_sys_reset_init();

    app_main();

    return EXT_ERR_SUCCESS;
}

/* Main task after power-on, including initialization, master */
EXT_PRV td_void sal_init_sys_phase2(td_void)
{
    uapi_syserr_ptr()->basic_info.init_phase = INIT_PHASE_SAL_INIT_BEGIN;
    td_u32 ret = sal_init();

    /*
     * After this, before the salInitComplete, the task scheduling operation is prohibited.
     * System Error Handling II Initialization
     */
    uapi_syserr_ptr()->basic_info.init_phase = INIT_PHASE_SAL_INIT_END;

    uapi_init_build_version();
    if (ret != EXT_ERR_SUCCESS) {
        return;
    }
}

td_void uapi_app_init(td_void)
{
    /*
     * NOTICE!!!: Initialize the entry, please do not change the initialization order,
     * do not add other initialization entries. partition table initialization
     */
    uapi_flash_partition_init();

    uapi_syserr_init();

    uapi_syserr_wdg_init(TD_TRUE, g_auto_reset_system_time_out_cfg, TD_TRUE);

    /* System Phase 1 initialization, including default parameter initialization,
       version number initialization, context variable initialization */
    (td_void)sal_init_sys_phase1();
    /* Start SC initialization */
    (td_void)sal_exp_task_init();
}
