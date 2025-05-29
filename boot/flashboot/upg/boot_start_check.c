/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: boot check
 * Author: CompanyName
 * Create: 2018-10-31
 */

#include "boot_start_check.h"

/*
 * 1.verify the legality of tbl;
 * 2.read the key info of the tbl to RAM;
 */
td_u32 boot_start_check_read_tbl_info(td_void)
{
    td_u32 ret;
    ext_start_tbl *start_tbl = ext_start_tbl_get_tbl();
    td_u32 crc_result = 0;
    ext_start_ctrl *start_ctrl = &g_start_ctrl;

    /* read start info list */
    ret = uapi_flash_read(start_ctrl->flash_addr_ker, sizeof(ext_start_tbl), (td_u8 *)start_tbl);
    if (ret != EXT_ERR_SUCCESS) {
        return EXT_ERR_UPG_CHECK_TBL_READ_FAIL;
    }

    /* length of start info list is exceptional:  the maximum length is 16K temporarily */
    if (start_tbl->tbl_len > EXT_START_TBL_MAX_SIZE_LIMIT || start_tbl->tbl_len < EXT_TBL_HEAD_CHECH_SUM_START_OFFSET) {
        memset_s(start_tbl, sizeof(ext_start_tbl), 0, sizeof(ext_start_tbl));
        return EXT_ERR_UPG_CHECK_TBL_LEN_FAIL;
    }

    /* crc chek for start info list */
    ret = upg_crc_block(start_ctrl->flash_addr_ker + EXT_TBL_HEAD_CHECH_SUM_START_OFFSET,
        start_tbl->tbl_len - EXT_TBL_HEAD_CHECH_SUM_START_OFFSET, &crc_result);
    if (start_tbl->check_sum != crc_result || ret != EXT_ERR_SUCCESS) {
        memset_s(start_tbl, sizeof(ext_start_tbl), 0, sizeof(ext_start_tbl));
        return EXT_ERR_UPG_CHECK_TBL_CRC_FAIL;
    }

    /* check for secure head of start info list */
    ret = boot_start_check_tbl_secure_info();
    if (ret != EXT_ERR_SUCCESS) {
        memset_s(start_tbl, sizeof(ext_start_tbl), 0, sizeof(ext_start_tbl));
        return ret;
    }

    start_ctrl->tbl_ok = TD_TRUE;
    return EXT_ERR_SUCCESS;
}

td_u32 boot_start_check_kernel(td_void)
{
    td_u32 ret;
    td_u32 crc_result = 0;
    if (g_start_ctrl.start_tbl == TD_NULL || g_start_ctrl.start_tbl->section == TD_NULL) {
        return EXT_ERR_FAILURE;
    }
    ext_start_tbl_section *start_tbl_sections = ext_start_tbl_get_sections();
    ext_start_ctrl *start_ctrl = &g_start_ctrl;

    /* crc check for kernel :  won't make error if with secure head */
    ret = upg_crc_block(start_ctrl->flash_addr_ker + start_tbl_sections[0].offset, start_tbl_sections[0].size,
        &crc_result);
    if (start_tbl_sections->check_sum != crc_result || ret != EXT_ERR_SUCCESS) {
        return EXT_ERR_UPG_CHECK_KERNEL_CRC;
    }

    start_ctrl->kernel_ok = TD_TRUE;
    return EXT_ERR_SUCCESS;
}

td_u32 boot_start_check_nvfile(td_void)
{
    td_u32 ret;
    td_u32 ver_magic = 0;
    ext_start_tbl *start_tbl = ext_start_tbl_get_tbl();

    /* whether verify the legality of original nv file dependence on start info list */
    if (start_tbl->attribute_check_nvfile) {
        ret = uapi_nv_check_file(g_start_ctrl.flash_addr_nv_file);
        if (ret != EXT_ERR_SUCCESS) {
            return EXT_ERR_UPG_CHECK_NVFILE_CRC;
        }
    }

    /* whether or not verify the version of the original nv file is matching with start table
     * dependence on start info list */
    if (start_tbl->attribute_check_nvfile_ver) {
        ret = uapi_nv_get_ver(g_start_ctrl.flash_addr_nv_file, &ver_magic);
        if (ret != EXT_ERR_SUCCESS || ver_magic != start_tbl->ver_magic) {
            return EXT_ERR_UPG_CHECK_NVFILE_VER_MAGIC;
        }
    }

    g_start_ctrl.nv_file_ok = TD_TRUE;
    return EXT_ERR_SUCCESS;
}
