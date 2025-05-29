/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Upgrade base funciton - init.
 */

#include "upg_base_init.h"
#include "upg_base_nv_mode.h"
#include "upg_base_nv.h"
#include "upg_base_prv.h"

#include <soc_ft_nv.h>
#include <soc_mdm_nv.h>
#include <soc_mdm_flash.h>
#include <soc_mdm_mem.h>
#include <soc_hb_ver.h>

#ifdef __cplusplus
extern "C" {
#endif

EXT_PRV td_u32 upg_init_env_cfg(td_void)
{
    td_u32 ret;
    ext_upg_kernel_ctrl *upg_ctrl = ext_upg_kerclt_ctrl();
    ext_flash_partition_table partition = { 0 };
    ext_flash_partition_info *table = TD_NULL;
    td_u32 cur_kernel_size = 0;

    ret = uapi_factory_nv_read(EXT_NV_FTM_FLASH_PARTIRION_TABLE_ID, &partition, sizeof(ext_flash_partition_table));
    if (ret != EXT_ERR_SUCCESS) {
        upg_ctrl->dfx_err_type |= EXT_UPG_BASE_DFX_ERR_READ_FTNV_FAIL;
        return ret;
    }

    g_upg_kernel_ctrl.ver_magic = uapi_ger_ver_magic();

    table = partition.table + EXT_FLASH_PARTITON_KERNEL;
    g_upg_kernel_ctrl.flash_addr_ker = table->addr_blk * BLOCK_SIZE;

    table = partition.table + EXT_FLASH_PARTITON_NV_BACKUP;
    g_upg_kernel_ctrl.flash_addr_nv_file = table->addr_blk * BLOCK_SIZE;
    g_upg_kernel_ctrl.flash_addr_tail_upg = (table->addr_blk + table->size_blk) * BLOCK_SIZE;

    table = partition.table + EXT_FLASH_PARTITON_NV_FILE;
    g_upg_kernel_ctrl.flash_addr_nv_file_work = table->addr_blk * BLOCK_SIZE;

    ret = uapi_flash_read(g_upg_kernel_ctrl.flash_addr_ker + ext_field_offset(ext_start_tbl, info_2_kernel),
        4, (td_u8 *)&cur_kernel_size); /* Reads 4 bytes. */
    if (ret != EXT_ERR_SUCCESS) {
        upg_ctrl->dfx_err_type |= EXT_UPG_BASE_DFX_ERR_READ_FTNV_FAIL;
        return ret;
    }

    g_upg_kernel_ctrl.cur_kernel_size = align_next(cur_kernel_size, PRODUCT_CFG_FLASH_BLOCK_SIZE);

    return EXT_ERR_SUCCESS;
}

/* Replacing the NV Process in the Factory Area */
EXT_PRV td_u32 upg_init_refresh_nv(td_void)
{
    td_pvoid buf;
    ext_upg_kernel_ctrl *upg_ctrl = ext_upg_kerclt_ctrl();
    td_u8 start_upg_mode = 0;

    (td_void)uapi_upg_get_start_upg_mode(&start_upg_mode);

    if ((g_upg_kernel_ctrl.ver_magic != uapi_nv_get_ver_magic()) || (start_upg_mode == EXT_21_UPG_TYPE_KERNEL)) {
        g_upg_kernel_ctrl.dfx_start_refresh_nv = TD_TRUE;

        buf = uapi_malloc(EXT_MOD_ID_DRV_UPG, 0x1000);
        if (buf == TD_NULL) {
            upg_ctrl->dfx_err_type |= EXT_UPG_BASE_DFX_ERR_MALLOC_FAIL;
            return EXT_ERR_UPG_REFRESH_NV_MALLOC_FAIL;
        }

        td_u32 ret = update_keep_nv(buf);
        uapi_free(EXT_MOD_ID_DRV_UPG, (td_pvoid)buf);
        return ret;
    }

    return EXT_ERR_SUCCESS;
}

td_u32 uapi_upg_base_init(td_void)
{
    td_u32 ret = EXT_ERR_SUCCESS;
    ext_upg_kernel_ctrl *upg_ctrl = ext_upg_kerclt_ctrl();

    ret |= upg_init_env_cfg();
    ret |= uapi_upg_nv_init_mode();
    ret |= uapi_upg_nv_clear_mode();
    ret |= upg_init_refresh_nv();
    upg_ctrl->dfx_init_ret = ret;

    return ret;
}

td_u32 uapi_upg_get_cur_kernel_size(td_void)
{
    return g_upg_kernel_ctrl.cur_kernel_size;
}

#ifdef __cplusplus
}
#endif
