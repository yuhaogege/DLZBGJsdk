/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Upgrade - Transmit file handling.
 * Author: CompanyName
 * Create: 2018-10-31
 */

#include "trans_file_slave.h"
#include "trans_file_host.h"
#include "trans_extra_info.h"

#include <upg_base_file_check.h>
#include <soc_partition_table.h>
#include <upg_base_tool.h>
#include <soc_mdm_flash.h>
#include <soc_21_upg.h>
#include <soc_mdm_mem.h>
#include <soc_stdlib.h>
#include <soc_upg_api.h>
#include <soc_mdm_nv.h>
#include <soc_mdm_flash_protect.h>
#include <soc_ft_nv.h>

#ifdef __cplusplus
extern "C" {
#endif

#define EXT_FTM_PRODUCT_TYPE_STA_MIN EXT_FTM_PRODUCT_TYPE_STA_BEGIN
#define EXT_FTM_PRODUCT_TYPE_STA_MAX EXT_FTM_PRODUCT_TYPE_STA_END
#define EXT_FTM_PRODUCT_TYPE_CCO_MIN EXT_FTM_PRODUCT_TYPE_CCO_BEGIN
#define EXT_FTM_PRODUCT_TYPE_CCO_MAX EXT_FTM_PRODUCT_TYPE_CCO_END

td_u32 g_trans_slave_file_addr = 0;
td_u32 g_trans_slave_file_size = 0;
td_u32 g_trans_slave_file_crc = 0;
td_bool g_trans_slave_started = TD_FALSE;
td_bool g_trans_slave_first_writed = TD_FALSE;

EXT_PRV EXT_CONST td_u8 g_tail_offset[UPG_TRANS_FILE_MAX] = {sizeof(ext_21_upg_tail_new), TRANS_OTHER_FILE_OFFSET};

td_u32 uapi_trans_get_partition_end_addr(td_void)
{
    td_u32 tail_flash_addr = 0;
    ext_flash_partition_table *partion_tbl = uapi_get_partition_table();
    ext_flash_partition_info *tbl = &partion_tbl->table[EXT_FLASH_PARTITON_UPG_BACKUP];
    if (uint_2_multiply((tbl->addr_blk + tbl->size_blk), BLOCK_SIZE, &tail_flash_addr) == TD_FALSE) {
        return EXT_ERR_FAILURE;
    }
    return tail_flash_addr;
}

static td_u32 uapi_trans_get_partition_start_addr(td_void)
{
    td_u32 flash_addr_tbl = 0;
    ext_flash_partition_table *partion_tbl = uapi_get_partition_table();
    ext_flash_partition_info *tbl = &partion_tbl->table[EXT_FLASH_PARTITON_KERNEL];
    if (uint_2_multiply(tbl->addr_blk, BLOCK_SIZE, &flash_addr_tbl) == TD_FALSE) {
        return EXT_ERR_FAILURE;
    }
    return flash_addr_tbl;
}

td_u32 uapi_trans_slave_get_max_file_size(td_void)
{
    td_u32 left_size = uapi_trans_get_partition_end_addr() - uapi_trans_get_partition_start_addr();

    left_size -= uapi_upg_get_cur_kernel_size();
    left_size -= (td_u32)sizeof(ext_21_upg_tail_new);
    left_size -= TRANS_OTHER_FILE_OFFSET;

    return left_size;
}

td_u32 uapi_trans_slave_save_content(td_u32 offset, td_u32 size, EXT_CONST td_u8 *buffer)
{
    td_u32 base_addr = g_trans_slave_file_addr;

    if ((size == 0) || (buffer == TD_NULL)) {
        return EXT_ERR_UPG_TRANS_SAVE_INVALID_PARAM;
    }

    if (((offset + size) > uapi_trans_slave_get_max_file_size()) || ((offset + size) > g_trans_slave_file_size) ||
        ((offset + size) < offset)) {
        return EXT_ERR_UPG_TRANS_SAVE_CONTENT_OUT_RANGE;
    }

    if (!g_trans_slave_first_writed) { /* Erase the entire flash memory before writing the file for the first time. */
        td_u32 erase_st_addr = base_addr;
        td_u32 erase_end_addr = base_addr + g_trans_slave_file_size;

        erase_st_addr = align_pre(erase_st_addr, PRODUCT_CFG_FLASH_BLOCK_SIZE);
        erase_end_addr = align_next(erase_end_addr, PRODUCT_CFG_FLASH_BLOCK_SIZE);

#ifdef PRODUCT_CFG_SUPPORT_FLASH_PROTECT
        /* The configuration is disabled to prevent others from being configured. */
        uapi_flash_protect_enable(TD_FALSE);
#endif
        uapi_flash_erase(erase_st_addr, erase_end_addr - erase_st_addr);
        g_trans_slave_first_writed = TD_TRUE;
    }

    return uapi_flash_write(base_addr + offset, size, buffer, TD_FALSE);
}

td_u32 uapi_trans_slave_start_prepare(upg_trans_file_type file_type, td_u32 file_len, td_u32 crc)
{
    td_u32 partition_end_addr = uapi_trans_get_partition_end_addr();

    if (g_trans_slave_started) { /* It is not the first time that the transmission storage command is received. */
        return EXT_ERR_SUCCESS;
    }

    if (file_len > uapi_trans_slave_get_max_file_size()) {
        return EXT_ERR_UPG_TRANS_SAVE_CONTENT_OUT_RANGE;
    }

    g_trans_slave_started = TD_TRUE;

    if (g_trans_slave_file_addr == 0) {
        g_trans_slave_file_addr = (td_u32)(partition_end_addr - g_tail_offset[file_type] - file_len);
    }

    g_trans_slave_file_size = file_len;
    g_trans_slave_file_crc = crc;

    return EXT_ERR_SUCCESS;
}

td_u32 uapi_trans_slave_stop_restore(td_void)
{
    td_u32 ret = EXT_ERR_SUCCESS;
    td_u32 partition_end_addr = uapi_trans_get_partition_end_addr();
    ext_upg_mode_nv upg_nv = { 0 };

    if (g_trans_slave_first_writed == TD_TRUE) {
        /* The flash memory is operated through the secondary development interface but the upgrade is not started. */
        ret = uapi_nv_read(EXT_NV_FTM_UPG, &upg_nv, sizeof(upg_nv));
        if (((ret == EXT_ERR_SUCCESS) && (upg_nv.upg_mode != EXT_21_UPG_TYPE_NONE)) || (!g_trans_slave_started)) {
            ret = uapi_flash_erase(partition_end_addr - PRODUCT_CFG_FLASH_BLOCK_SIZE, PRODUCT_CFG_FLASH_BLOCK_SIZE);
        }
    }

    uapi_upg_nv_clear_mode();
    g_trans_slave_file_addr = 0;
    g_trans_slave_file_size = 0;
    g_trans_slave_file_crc = 0;
    g_trans_slave_started = TD_FALSE;
    g_trans_slave_first_writed = TD_FALSE;
    uapi_trans_host_stop_restore();
    return ret;
}

td_u32 uapi_trans_slave_finish(td_void)
{
    return uapi_upg_real_start(g_trans_slave_file_addr, uapi_trans_get_partition_end_addr());
}

/* Verify that the file transfer is correct. */
td_u32 uapi_trans_slave_check_file(td_void)
{
    td_u32 crc_result = 0;
    td_u32 ret;

    ret = uapi_crc32_flash_content(0, g_trans_slave_file_addr, g_trans_slave_file_size, &crc_result);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    if (crc_result != g_trans_slave_file_crc) {
        return EXT_ERR_UPG_TRANS_CHECK_FILE_FAIL;
    }
    return EXT_ERR_SUCCESS;
}

td_u32 uapi_upg_transmit_mem_write_prepare(upg_trans_file_type file_type, td_u32 file_len)
{
    ext_flash_partition_table *partion_tbl = uapi_get_partition_table();
    td_u32 tail_flash_addr = (partion_tbl->table[EXT_FLASH_PARTITON_UPG_BACKUP].addr_blk +
                              partion_tbl->table[EXT_FLASH_PARTITON_UPG_BACKUP].size_blk) *
                             BLOCK_SIZE;

    g_trans_slave_file_addr = (td_u32)(tail_flash_addr - g_tail_offset[file_type] - file_len);
    g_trans_slave_file_size = file_len;

    uapi_trans_host_prepare(file_type, file_len, 0);

    g_trans_slave_started = TD_FALSE;
    g_trans_slave_first_writed = TD_FALSE;
    return EXT_ERR_SUCCESS;
}

td_u32 uapi_receive_plc_start_fram(td_u8 *data, td_u32 data_size)
{
    td_u32 ret;
    td_u8 *temp_head = TD_NULL;
    td_u16 temp_head_size = 0;

    ext_21_upg_head *upg_head = TD_NULL;
    td_u16 check_head_size;

    ret = uapi_std_get_extra_info_item(data, data_size, TRANS_FILE_CTRL_ID_FILE_HEAD, &temp_head, &temp_head_size);
    /*
     * The extended information cannot be obtained, the judgment cannot be performed,
     * and the error processing cannot be performed, and a success message is returned.
     */
    if (ret != EXT_ERR_SUCCESS || temp_head == TD_NULL || temp_head_size == 0) {
        return EXT_ERR_SUCCESS;
    }

    check_head_size = uapi_min((td_u16)sizeof(ext_21_upg_head), temp_head_size);
    upg_head = uapi_malloc(EXT_MOD_ID_APP_UPG, check_head_size);
    /*
     * The memory cannot be applied for, the judgment cannot be performed,
     * the error cannot be handled, and a success message is returned.
     */
    if (upg_head == TD_NULL) {
        return EXT_ERR_SUCCESS;
    }

    (td_void)memcpy_s(upg_head, check_head_size, temp_head, uapi_min((td_u16)sizeof(ext_21_upg_head), temp_head_size));
    ret = uapi_upg_check_head(upg_head, check_head_size);

    uapi_free(0, upg_head);
    upg_head = TD_NULL;
    return ret;
}

td_u32 uapi_upg_head_check_res(td_void)
{
    td_u32 ret;
    ext_21_upg_head tmp_upg_head = { 0 };
    td_u8 check_sum[EXT_SHA256_SIZE] = { 0 };
    td_u8 i;
    td_bool is_in_list = TD_FALSE;

    ret = uapi_flash_read(g_trans_slave_file_addr, sizeof(ext_21_upg_head), ((td_u8 *)(&tmp_upg_head)));
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    ret = upg_check_chip_magic(tmp_upg_head.chip_magic);
    if (ret != EXT_ERR_SUCCESS) {
        return EXT_ERR_SUCCESS;
    }

    ret = upg_check_manufactuer(tmp_upg_head.manufactuer_code);
    if (ret != EXT_ERR_SUCCESS) {
        return EXT_ERR_SUCCESS;
    }

    for (i = 0; i < sizeof(tmp_upg_head.product_list) / sizeof(tmp_upg_head.product_list[0]); i++) {
        if (tmp_upg_head.product_list[i].max_id >= EXT_FTM_PRODUCT_TYPE_STA_MIN &&
            tmp_upg_head.product_list[i].min_id <= EXT_FTM_PRODUCT_TYPE_STA_MAX) {
            is_in_list = TD_TRUE;
            break;
        }
    }

    if (is_in_list == TD_FALSE) {
        return EXT_ERR_FAILURE;
    }

    ret = uapi_hash_flash_content(g_trans_slave_file_addr + EXT_UPG_HEAD_CHECK_SUM_START_OFFSET,
        tmp_upg_head.file_len - EXT_UPG_HEAD_CHECK_SUM_START_OFFSET, check_sum, EXT_SHA256_SIZE);
    if ((ret != EXT_ERR_SUCCESS) || memcmp(tmp_upg_head.check_sum, check_sum, sizeof(tmp_upg_head.check_sum))) {
        return EXT_ERR_FAILURE;
    }

    return EXT_ERR_SUCCESS;
}

td_u32 uapi_local_upg_head_check_res(td_void)
{
    td_u32 ret;
    ext_21_upg_head tmp_upg_head = { 0 };
    td_u8 i;
    td_bool is_in_list = TD_FALSE;

    ret = uapi_flash_read(g_trans_slave_file_addr, sizeof(ext_21_upg_head), ((td_u8 *)(&tmp_upg_head)));
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    ret = upg_check_chip_magic(tmp_upg_head.chip_magic);
    if (ret != EXT_ERR_SUCCESS) {
        return EXT_ERR_SUCCESS;
    }

    ret = upg_check_manufactuer(tmp_upg_head.manufactuer_code);
    if (ret != EXT_ERR_SUCCESS) {
        return EXT_ERR_SUCCESS;
    }

    for (i = 0; i < sizeof(tmp_upg_head.product_list) / sizeof(tmp_upg_head.product_list[0]); i++) {
        if (tmp_upg_head.product_list[i].max_id >= EXT_FTM_PRODUCT_TYPE_CCO_MIN &&
            tmp_upg_head.product_list[i].min_id <= EXT_FTM_PRODUCT_TYPE_CCO_MAX) {
            is_in_list = TD_TRUE;
            break;
        }
    }

    if (is_in_list == TD_FALSE) {
        return EXT_ERR_FAILURE;
    }
    return EXT_ERR_SUCCESS;
}

td_u32 uapi_upg_get_slave_addr(td_void)
{
    return g_trans_slave_file_addr;
}

#ifdef __cplusplus
}
#endif
