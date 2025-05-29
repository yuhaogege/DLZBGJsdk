/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description:   dfx_remap
 * Author: CompanyName
 * Create: 2018-10-31
 */

#ifndef __DFX_REMAP_H__
#define __DFX_REMAP_H__
#include <soc_types.h>
#include <soc_mdm_types.h>

typedef struct {
    td_u32 pad;
} ext_dfx_diag_remap_stat_cmd;

typedef struct {
    td_u32 block_count;
    td_u32 total_remap_count_low;
    td_u32 total_remap_count_high;
    td_u32 record_count;
} ext_dfx_diag_remap_stat_total_ind;

typedef struct {
    td_u32 vir_addr;
    td_u32 remap_in_times;
} ext_dfx_diag_remap_stat_item_ind;

#endif
