/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: boot init
 * Author: CompanyName
 * Create: 2018-10-31
 */

#include "boot_start_init.h"

td_u32 boot_upg_init_phase1(td_void)
{
    td_u32 ret;
    ext_flash_partition_table partithion = { 0 };
    ext_nv_ftm_product_id product_id = { 0 };
    ext_start_ctrl *start_ctrl = &g_start_ctrl;

    ret = uapi_factory_nv_read(EXT_NV_FTM_FLASH_PARTIRION_TABLE_ID, &partithion, sizeof(ext_flash_partition_table));
    ret |= uapi_factory_nv_read(EXT_NV_FTM_PRODUCT_ID, &product_id, sizeof(product_id));
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }
    if (uint_2_multiply(partithion.table[EXT_FLASH_PARTITON_KERNEL].addr_blk, BLOCK_SIZE, NULL) == TD_FALSE ||
        uint_2_multiply(partithion.table[EXT_FLASH_PARTITON_NV_BACKUP].addr_blk, BLOCK_SIZE, NULL) == TD_FALSE ||
        uint_2_multiply(partithion.table[EXT_FLASH_PARTITON_UPG_BACKUP].addr_blk, BLOCK_SIZE, NULL) == TD_FALSE ||
        uint_2_multiply(partithion.table[EXT_FLASH_PARTITON_BOOT].addr_blk, BLOCK_SIZE, NULL) == TD_FALSE) {
        return EXT_ERR_FAILURE;
    }
    start_ctrl->flash_addr_ker = partithion.table[EXT_FLASH_PARTITON_KERNEL].addr_blk * BLOCK_SIZE;
    start_ctrl->flash_addr_nv_file = partithion.table[EXT_FLASH_PARTITON_NV_BACKUP].addr_blk * BLOCK_SIZE;
    start_ctrl->flash_addr_upg = 0;
    start_ctrl->flash_addr_tail_upg = (partithion.table[EXT_FLASH_PARTITON_UPG_BACKUP].addr_blk +
        partithion.table[EXT_FLASH_PARTITON_UPG_BACKUP].size_blk) *
        BLOCK_SIZE;
    start_ctrl->flash_addr_boot = partithion.table[EXT_FLASH_PARTITON_BOOT].addr_blk * BLOCK_SIZE;
    start_ctrl->product_type = product_id.product_type;
    start_ctrl->boot_ver_magic = PRODUCT_CFG_BOOT_VER_MAGIC;
    start_ctrl->env_ok = TD_TRUE;
    boot_msg4("env= ", start_ctrl->flash_addr_ker, start_ctrl->flash_addr_nv_file, start_ctrl->flash_addr_tail_upg,
        start_ctrl->flash_addr_boot);
    return EXT_ERR_SUCCESS;
}

td_u32 boot_upg_init_phash2(td_void)
{
    static ext_21_upg_head g_upg_head;
    static ext_21_upg_head_section g_upg_section_info[EXT_UPG_HEAD_CUR_MAX_SECTION_COUNT];
    static ext_start_tbl g_start_tbl;
    ext_start_ctrl *start_ctrl = &g_start_ctrl;
    start_ctrl->upg_head = &g_upg_head;            /* can point to flash space */
    start_ctrl->upg_sections = g_upg_section_info; /* can point to flash space */
    start_ctrl->start_tbl = &g_start_tbl;          /* can point to flash space */
    return EXT_ERR_SUCCESS;
}

td_u32 boot_upg_init_phash3(td_void)
{
    td_u32 ret;
    ext_start_ctrl *start_ctrl = &g_start_ctrl;
    ret = uapi_flash_read(start_ctrl->flash_addr_ker, sizeof(ext_start_tbl), (td_u8 *)start_ctrl->start_tbl);
    if (ret != EXT_ERR_SUCCESS) {
        return EXT_ERR_UPG_CHECK_TBL_READ_FAIL;
    }

    return EXT_ERR_SUCCESS;
}
