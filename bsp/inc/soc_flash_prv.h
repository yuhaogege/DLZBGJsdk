/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Optimize deadlock storage
 * iteration 1: basic functions,
 * iteration 2: secondary development interface and perfection, etc. soc_flash_prv.h code
 * Author: CompanyName
 * Create: 2012-11-28
 */
#ifndef __SOC_FLASH_PRV_H__
#define __SOC_FLASH_PRV_H__
#include <soc_types.h>
#include <soc_mdm_timer.h>

EXT_START_HEADER

typedef struct {
    td_u32 default_type : 6;
    td_u32 timer_created : 1;
    td_u32 enable : 1;
    td_u32 protected_times : 23;
    td_u32 timer_started : 1;

    td_u32 timer_timeout;

    td_u32 current_block : 13;
    td_u32 reserve : 3;
    td_u32 flash_status_reg : 16;

    td_u8 start_timer_errs;
    td_u8 protect_errs;
    td_u8 job_add_errs;
    td_u8 job_run_errs;

    ext_timer_handle_s timer_handle;
} ext_flash_protect_ctrl;

ext_flash_protect_ctrl *uapi_flash_protect_get_ctrl_info(td_void);

EXT_END_HEADER
#endif
