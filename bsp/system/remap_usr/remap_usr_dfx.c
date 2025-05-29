/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: remap_usr_dfx.c code
 * Author: CompanyName
 * Create: 2012-04-19
 */
#include "remap_usr_dfx.h"
#include <soc_types.h>
#include <soc_mdm_types.h>
#include <soc_mdm_remap.h>
#include "remap_usr.h"

#ifdef PRODUCT_CFG_USR_REMAP_DFX
ext_remap_dfx g_remap_usr_dfx;

#include <soc_stdlib.h>
__isr td_void remap_usr_dfx_hook(td_u32 vir_addr, td_u32 phy_addr, td_u32 access_cnt)
{
    td_u32 vir_addr_align = (vir_addr & (~(EXT_REMAP_BLOCK_SIZE - 1)));
    td_u32 vir_idx = ((vir_addr_align - EXT_DYNAMIC_MEM_START_ADDR) >> EXT_REMAP_SHFT_BIT);

    printf("%s[0x%x][0x%x]\n", __func__, vir_addr, phy_addr);
    ext_unref_param(phy_addr);
    ext_unref_param(access_cnt);

    if (vir_idx < sizeof(g_remap_usr_dfx.sw_entrys) / sizeof(g_remap_usr_dfx.sw_entrys[0])) {
        g_remap_usr_dfx.sw_entrys[vir_idx].remap_in_times++;
    } else {
        g_remap_usr_dfx.record_count++;
    }
}
#endif
