/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Upgrade base funciton - nv.
 */

#include "upg_base_nv.h"
#include "upg_base_prv.h"

#include <soc_mdm_types.h>
#include <soc_21_upg.h>
#include <soc_ft_nv.h>
#include <soc_mdm_flash.h>
#include <soc_mdm_nv.h>

#ifdef __cplusplus
extern "C" {
#endif

EXT_EXTERN td_u32 uapi_nv_flush_keep_ids(td_u8 *addr, td_u32 len);

td_u32 update_keep_nv(td_pvoid buf)
{
    td_u32 ret;
    ext_flash_partition_table *table = uapi_get_partition_table();

    ret = uapi_flash_read(g_upg_kernel_ctrl.flash_addr_nv_file, BLOCK_SIZE, buf);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    ret = uapi_nv_flush_keep_ids(buf, BLOCK_SIZE);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    ret = uapi_nv_sector_write(buf);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    ret = uapi_nv_init(table->table[EXT_FLASH_PARTITON_NV_FILE].addr_blk * BLOCK_SIZE,
        table->table[EXT_FLASH_PARTITON_NV_BACKUP].addr_blk * BLOCK_SIZE);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    return EXT_ERR_SUCCESS;
}

#ifdef __cplusplus
}
#endif
