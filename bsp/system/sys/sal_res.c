/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Implementation of sal module phase 1 init.
 * Author: CompanyName
 * Create: 2012-07-14
 */

#include "sal_res.h"
#include "sal_common.h"
#include "soc_ft_nv.h"
#include "sal_nvm.h"
#include "soc_mac_nv.h"

#include <soc_mdm_sys.h>
#include <soc_mdm.h>

EXT_PRV sal_ctx_stru g_sal_ctx;

/* Get the SAL context */
__isr sal_ctx_stru *get_sal_ctx(td_void)
{
    return (sal_ctx_stru *)&g_sal_ctx;
}

/* SAL context initialization phase 1 */
/* Initialize the SAL context, version number */
/* GLOBAL-->VER */
td_u32 sal_init_sys_phase1(td_void)
{
    td_u32 ret = EXT_ERR_SUCCESS;

    uapi_syserr_ptr()->basic_info.init_phase = INIT_PHASE_SAL_INIT_1;
    return ret;
}
