/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: upg check
 * Author: CompanyName
 * Create: 2018-10-31
 */

#include "boot_upg_check.h"

EXT_PRV td_u32 uapi_product_get_type(td_void)
{
    ext_start_ctrl *start_ctrl = &g_start_ctrl;
    return start_ctrl->product_type;
}

EXT_PRV td_u32 upg_check_chip_maigc(td_u32 chip_magic)
{
    if (chip_magic != (td_u32)PRODUCT_CFG_CHIP_MAGIC) {
        return EXT_ERR_UPG_HEAD_CHECK_MAGIC_WRONG;
    }

    return EXT_ERR_SUCCESS;
}

td_u32 upg_check_manufactuer(td_u16 manufactuer_code)
{
    if (manufactuer_code != PRODUCT_CFG_MANUCODE) {
        return EXT_ERR_UPG_HEAD_CHECK_MANUCODE_WRONG;
    }

    return EXT_ERR_SUCCESS;
}

EXT_PRV td_u32 upg_check_product_list(const ext_21_upg_head_product *product_info, td_u32 list_count)
{
    td_u32 i;

    for (i = 0; i < list_count; i++) {
        const ext_21_upg_head_product *item = &product_info[i];

        if (uapi_product_get_type() >= item->min_id && uapi_product_get_type() <= item->max_id) {
            return EXT_ERR_SUCCESS;
        }
    }

    return EXT_ERR_UPG_HEAD_CHECK_PRODUCT_LIST_WRONG;
}

td_u32 upg_check_file_type(td_u8 file_type)
{
    if (file_type == EXT_21_UPG_TYPE_KERNEL || file_type == EXT_21_UPG_TYPE_BOOT) {
        return EXT_ERR_SUCCESS;
    }
    return EXT_ERR_UPG_HEAD_CHECK_FILE_TYPE_WRONG;
}

EXT_PRV td_u32 upg_check_file_len(td_u32 size) /* size is a little difference */
{
    if (size > (16 * 1024 * 1024)) { /* size less than 16*1024*1024 */
        return EXT_ERR_UPG_HEAD_CHECK_FILE_LEN_WRONG;
    }
    return EXT_ERR_SUCCESS;
}

td_u32 upg_conver_tail_addr_2_head_addr(td_u32 upg_tail_addr, td_u32 *upg_head_addr)
{
    td_u32 ret;
    ext_21_upg_tail_new tail = { 0 };
    ext_start_ctrl *start_ctrl = &g_start_ctrl;

    /* read file tail */
    ret = uapi_flash_read(upg_tail_addr - sizeof(ext_21_upg_tail_new), sizeof(ext_21_upg_tail_new), (td_u8 *)(&tail));
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    if (tail.head_end_magic != (td_u32)PRODUCT_CFG_UPG_FILE_END_SIGNATURE) {
        return EXT_ERR_UPG_CHECK_UPG_END_MAGIC_FAIL;
    }
    /* assure the position of upgrade file head */
    if (upg_tail_addr <= tail.head_before_offset) {
        return EXT_ERR_UPG_CHECK_UPG_END_OFFSET_FAIL;
    }

    if (tail.mode_magic == EXT_UPG_NEW_PROCESS_MAGIC) {
        start_ctrl->new_kernel = TD_TRUE;
        if (tail.upg_magic[2] == EXT_UPG_DO_UPG_MAGIC) { /* index 2 */
            start_ctrl->upg_magics_exist |= 0x4;
        }

        if (tail.upg_magic[1] == EXT_UPG_DO_UPG_MAGIC) {
            start_ctrl->upg_magics_exist |= 0x2;
        }

        if (tail.upg_magic[0] == EXT_UPG_DO_UPG_MAGIC) {
            start_ctrl->upg_magics_exist |= 0x1;
        }
    } else {
        start_ctrl->new_kernel = TD_FALSE;
    }

    *upg_head_addr = upg_tail_addr - tail.head_before_offset;
    return EXT_ERR_SUCCESS;
}

EXT_PRV td_u32 upg_get_head_from_flash(td_bool end_addr, td_u32 flash_addr,
    ext_21_upg_head *upg_head, td_u32 *upg_head_addr)
{
    td_u32 ret;

    if (end_addr) {
        /* get the position of upg head */
        ret = upg_conver_tail_addr_2_head_addr(flash_addr, upg_head_addr);
        if (ret != EXT_ERR_SUCCESS) {
            return ret;
        }
    } else {
        *upg_head_addr = flash_addr;
    }

    /* read upg head */
    return uapi_flash_read(*upg_head_addr, sizeof(ext_21_upg_head), (td_u8 *)upg_head);
}

td_u32 upg_check_head_common_info(const ext_21_upg_head *upg_head, td_u8 *file_type)
{
    if (upg_check_file_len(upg_head->file_len) != EXT_ERR_SUCCESS) {
        return EXT_ERR_UPG_HEAD_CHECK_FILE_LEN_WRONG;
    }

    /* verify the chip magic byte and manufacturer magic byte of upgrade file */
    if (upg_check_chip_maigc(upg_head->chip_magic) != EXT_ERR_SUCCESS) {
        return EXT_ERR_UPG_HEAD_CHECK_MAGIC_WRONG;
    }

    if (upg_check_manufactuer(upg_head->manufactuer_code) != EXT_ERR_SUCCESS) {
        return EXT_ERR_UPG_HEAD_CHECK_MANUCODE_WRONG;
    }

    *file_type = upg_head->file_type;
    if (upg_check_file_type(upg_head->file_type) != EXT_ERR_SUCCESS) {
        return EXT_ERR_UPG_HEAD_CHECK_FILE_TYPE_WRONG;
    }

    /* verify product type of upgrade file */
    if (upg_check_product_list(upg_head->product_list, 2) != EXT_ERR_SUCCESS) { /* list count 2 */
        return EXT_ERR_UPG_HEAD_CHECK_PRODUCT_LIST_WRONG;
    }

    return EXT_ERR_SUCCESS;
}

td_u32 uapi_upg_check_head(const ext_21_upg_head *upg_head)
{
    td_u32 ret;
    td_u8 file_type = EXT_21_UPG_TYPE_NONE;
#ifdef EXT_UPG_IN_BOOT_CHECK_PARTITION
    ext_partition_ver ver_ret;
#endif

    ret = upg_check_head_common_info(upg_head, &file_type);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }
#ifdef EXT_UPG_IN_BOOT_CHECK_PARTITION
    ver_ret = uapi_flash_partition_check_ver(&upg_head->partion_ver);
    if (file_type != EXT_21_UPG_TYPE_KERNEL && file_type != EXT_21_UPG_TYPE_BOOT) {
        return EXT_ERR_UPG_HEAD_CHECK_PARTION_NO_GO;
    }
    if (file_type == EXT_21_UPG_TYPE_KERNEL && ver_ret != EXT_PARTITION_VER_THE_SAME) {
        return EXT_ERR_UPG_HEAD_CHECK_PARTION_NOT_SAME;
    }
#endif
    return EXT_ERR_SUCCESS;
}

EXT_PRV td_u32 boot_upg_check_by_file_type(const ext_21_upg_head *upg_head)
{
    td_u32 ret;
    ext_start_ctrl *start_ctrl = &g_start_ctrl;
    ext_start_tbl *start_tbl = start_ctrl->start_tbl;
    ext_21_upg_head_section *sections = ext_upg_head_get_sections();

    start_ctrl->upg_file_type = upg_head->file_type;
    if (upg_head->file_type == EXT_21_UPG_TYPE_KERNEL &&
        ((upg_head->ver_magic != start_tbl->ver_magic || start_ctrl->new_kernel) || start_ctrl->tbl_ok == TD_FALSE)) {
        /* upgrding */
        start_ctrl->ver_magic_different = TD_TRUE;
    } else if (upg_head->file_type == EXT_21_UPG_TYPE_BOOT &&
        ((upg_head->ver_magic != start_ctrl->boot_ver_magic || start_ctrl->new_kernel))) {
        /* not verify start info list */
        /* BOOT upgrade */
        start_ctrl->ver_magic_different = TD_TRUE;
    }

    /* read sub section info */
    ret = uapi_flash_read(g_start_ctrl.flash_addr_upg + upg_head->section_list_offset,
        sizeof(ext_21_upg_head_section) * upg_head->section_count, (td_u8 *)sections);
    if (ret != EXT_ERR_SUCCESS) {
        return EXT_ERR_UPG_CHECK_UPG_READ_SECTION_FAIL;
    }

    return EXT_ERR_SUCCESS;
}

td_u32 boot_upg_clear_upg_magic_one(td_void)
{
    ext_start_ctrl *start_ctrl = &g_start_ctrl;
    ext_21_upg_tail_new tail = { 0 };
    td_u32 data = 0;
    size_t flash_addr = start_ctrl->flash_addr_tail_upg - sizeof(ext_21_upg_tail_new);
    flash_addr = flash_addr + ext_struct_offset(ext_21_upg_tail_new, upg_magic[0]);
    if (start_ctrl->upg_magics_exist) {
        for (td_u32 i = 0; i < sizeof(tail.upg_magic) / sizeof(tail.upg_magic[0]); i++) {
            if (start_ctrl->upg_magics_exist & (1 << i)) {
                return uapi_flash_write(flash_addr + sizeof(tail.upg_magic[0]) * i, sizeof(tail.upg_magic[0]),
                    (td_u8 *)&data, TD_FALSE);
            }
        }
    }
    return EXT_ERR_SUCCESS;
}

td_u32 boot_upg_clear_upg_magic_all(td_void)
{
    td_u32 ret = EXT_ERR_SUCCESS;
    ext_start_ctrl *start_ctrl = &g_start_ctrl;
    ext_21_upg_tail_new tail = { 0 };
    td_u32 data = 0;
    size_t flash_addr = start_ctrl->flash_addr_tail_upg - sizeof(ext_21_upg_tail_new);
    flash_addr = flash_addr + ext_struct_offset(ext_21_upg_tail_new, upg_magic[0]);

    if (start_ctrl->upg_magics_exist) {
        for (td_u32 i = 0; i < sizeof(tail.upg_magic) / sizeof(tail.upg_magic[0]); i++) {
            if (start_ctrl->upg_magics_exist & (1 << i)) {
                ret |= uapi_flash_write(flash_addr + sizeof(tail.upg_magic[0]) * i, sizeof(tail.upg_magic[0]),
                    (td_u8 *)&data, TD_FALSE);
            }
        }
    }

    return ret;
}

td_u32 boot_upg_check_upg_phase0(td_void)
{
    ext_start_ctrl *start_ctrl = &g_start_ctrl;
    ext_21_upg_head *upg_head = start_ctrl->upg_head; /* point to upgrade file head */
    td_u32 ret;

    if (upg_head == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    /* get file head */
    ret = upg_get_head_from_flash(TD_TRUE, start_ctrl->flash_addr_tail_upg, upg_head, &g_start_ctrl.flash_addr_upg);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    start_ctrl->upg_file_phas0_ok = TD_TRUE;
    return EXT_ERR_SUCCESS;
}

td_u32 boot_upg_check_upg_phase1(td_void)
{
    ext_start_ctrl *start_ctrl = &g_start_ctrl;
    td_u32 ret;
    td_u8 check_sum[32] = { 0 };                       /* length 32 */
    ext_21_upg_head *upg_head = start_ctrl->upg_head; /* point to upgrade file head */
    static td_bool s_b_checked = TD_FALSE;

    if (upg_head == TD_NULL) {
        return EXT_ERR_FAILURE;
    }

    if (start_ctrl->upg_file_phas0_ok == TD_FALSE) {
        return EXT_ERR_FAILURE;
    }

    if (s_b_checked) {
        return EXT_ERR_SUCCESS;
    }

    s_b_checked = TD_TRUE;

    /* verify upgrade file head */
    ret = uapi_upg_check_head(upg_head);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    /* verify crc of upgrade file */
    ret = upg_hash_block(g_start_ctrl.flash_addr_upg + EXT_UPG_HEAD_CHECK_SUM_START_OFFSET,
        upg_head->file_len - EXT_UPG_HEAD_CHECK_SUM_START_OFFSET, check_sum);
    if (ret != EXT_ERR_SUCCESS || memcmp(upg_head->check_sum, check_sum, EXT_UPG_HEAD_CHECK_SUM_SIZE)) {
        return EXT_ERR_UPG_CHECK_UPG_CRC_WRONG;
    }

    ret = boot_upg_check_by_file_type(upg_head);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    start_ctrl->upg_file_phas1_ok = TD_TRUE;
    return EXT_ERR_SUCCESS;
}

td_u32 boot_upg_check_upg_phase2(td_void)
{
    td_u32 ret;
    ext_21_upg_head *upg_head = ext_upg_head_get_head();
    ext_start_ctrl *start_ctrl = &g_start_ctrl;

    if (!start_ctrl->upg_file_phas1_ok) {
        return EXT_ERR_UPG_FAIL_4;
    }

    /* check for hupg secure head */
    ret = boot_upg_check_secure_head();
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    /* check boot security if the type is boot */
    if (upg_head->file_type == EXT_21_UPG_TYPE_BOOT) {
        ret = boot_upg_check_secure_boot();
        if (ret != EXT_ERR_SUCCESS) {
            return ret;
        }
    }

    start_ctrl->upg_file_phas2_ok = TD_TRUE;
    return EXT_ERR_SUCCESS;
}
