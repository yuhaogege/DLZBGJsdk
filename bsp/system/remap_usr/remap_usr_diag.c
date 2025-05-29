/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: remap_usr_diag.c code
 * Author: CompanyName
 * Create: 2012-04-19
 */
#include "remap_usr_diag.h"
#include <soc_types.h>
#include <soc_mdm_types.h>
#include <soc_mdm_remap.h>
#include <soc_mdm_diag.h>
#include <dfx_diag_cmd_id.h>
#include <dfx_remap.h>
#include "remap_usr.h"

#ifdef PRODUCT_CFG_USR_REMAP_DIAG
EXT_PRV td_u32 remap_usr_diag_cmd(td_u16 id, EXT_IN td_pvoid cmd_param, td_u16 cmd_param_size, td_u8 option)
{
    ext_dfx_diag_remap_stat_total_ind total_ind = { 0 };
    ext_dfx_diag_remap_stat_item_ind item_ind = { 0 };
    td_u64 total_times = 0;
    ext_unref_param(cmd_param);
    ext_unref_param(cmd_param_size);
    ext_unref_param(id);

    for (td_u32 i = 0; i < sizeof(g_remap_usr_dfx.sw_entrys) / sizeof(g_remap_usr_dfx.sw_entrys[0]); i++) {
        item_ind.vir_addr = (i << EXT_REMAP_SHFT_BIT) + EXT_DYNAMIC_MEM_START_ADDR;
        item_ind.remap_in_times = g_remap_usr_dfx.sw_entrys[i].remap_in_times;
        total_times += (td_u64)(g_remap_usr_dfx.sw_entrys[i].remap_in_times);
        uapi_diag_report_packet(ID_DIAG_IND_DRV_REMAP_ITEM, option, (td_pbyte)&item_ind, sizeof(item_ind), TD_TRUE);
    }
    total_times += g_remap_usr_dfx.record_count;

    total_ind.block_count = sizeof(g_remap_usr_dfx.sw_entrys) / sizeof(g_remap_usr_dfx.sw_entrys[0]);
    total_ind.total_remap_count_low = (td_u32)total_times;
    total_ind.total_remap_count_high = (td_u32)(total_times >> 32);
    total_ind.record_count = g_remap_usr_dfx.record_count;

    uapi_diag_report_packet(ID_DIAG_IND_DRV_REMAP_TOTAL, option, (td_pbyte)&total_ind, sizeof(total_ind), TD_TRUE);
    return EXT_ERR_SUCCESS;
}

EXT_PRV EXT_CONST ext_diag_cmd_reg_obj g_remap_usr_dfx_diag_tbl[] = {
    { ID_DIAG_CMD_DRV_REMAP, ID_DIAG_CMD_DRV_REMAP, remap_usr_diag_cmd },
};

__init td_u32 usr_remap_diag_init(td_void)
{
    return uapi_diag_register_cmd(g_remap_usr_dfx_diag_tbl,
        sizeof(g_remap_usr_dfx_diag_tbl) / sizeof(g_remap_usr_dfx_diag_tbl[0]));
}
#endif
