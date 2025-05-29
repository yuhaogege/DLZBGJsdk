/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: remap_usr_init.c code
 * Author: CompanyName
 * Create: 2012-04-19
 */
#include "remap_usr_init.h"
#include "remap_usr.h"

#include <soc_types.h>
#include <soc_mdm_types.h>
#include <soc_mdm_remap.h>
#include <soc_stdlib.h>

__isr td_u32 usr_remap_init(td_void)
{
    td_u32 ret;
#if defined PRODUCT_CFG_USR_REMAP_32K
    ret = uapi_remap_32k_init();
#elif defined(PRODUCT_CFG_USR_REMAP_48K)
    ret = uapi_remap_48k_init();
#elif defined(PRODUCT_CFG_USR_REMAP_64K)
    ret = uapi_remap_64k_init();
#endif
    if (ret != EXT_ERR_SUCCESS) {
        while (1) {
            printf("usr_remap_init ret=%d\n", ret);
        }
    }

#ifdef PRODUCT_CFG_USR_REMAP_DFX
    ret |= uapi_remap_register_debug_hook(remap_usr_dfx_hook);
#endif

    return ret;
}
