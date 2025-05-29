/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: remap_usr.h code
 * Author: CompanyName
 * Create: 2012-04-19
 */

#ifndef __REMAP_USR_H__
#define __REMAP_USR_H__
#include <soc_types.h>
#include <soc_mdm_types.h>
#include <soc_mdm_remap.h>

#define EXT_CUR_DYNAMIC_CODE_SIZE 0x2D000

typedef struct {
    td_u32 remap_in_times;
} ext_remap_dfx_soft_entry;

typedef struct {
    td_u32 record_count;
    ext_remap_dfx_soft_entry sw_entrys[EXT_CUR_DYNAMIC_CODE_SIZE >> EXT_REMAP_SHFT_BIT];
} ext_remap_dfx;

extern ext_remap_dfx g_remap_usr_dfx;

td_void remap_usr_dfx_hook(td_u32 vir_addr, td_u32 phy_addr, td_u32 access_cnt);
td_u32 remap_usr_diag_init(td_void);

#endif
