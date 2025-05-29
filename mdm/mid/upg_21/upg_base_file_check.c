/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Upgrade base funciton - file check.
 */

#include "upg_base_file_check.h"
#include <soc_mdm_types.h>
#include <soc_21_upg.h>
#include <soc_ft_nv.h>
#include <soc_mdm_flash.h>
#include <soc_mdm_mem.h>
#include <soc_stdlib.h>
#include <upg_base_tool.h>
#include <soc_partition_table.h>
#include <soc_mdm_init.h>
#include <soc_upg_api.h>
#include "upg_base_secure_check.h"

#ifdef __cplusplus
extern "C" {
#endif

EXT_PRV td_u32 upg_check_product_list(const ext_21_upg_head_product *product_info, td_u32 list_count);
EXT_PRV td_u32 upg_conver_tail_addr_2_head_addr(td_u32 upg_tail_addr, td_u32 *upg_head_addr);
EXT_PRV td_u32 upg_get_head_from_flash(td_bool end_addr, td_u32 flash_addr, ext_21_upg_head *upg_head,
    td_u32 *upg_head_addr);
EXT_PRV td_u32 upg_check_head_common_info(const ext_21_upg_head *upg_head, td_u32 head_size, td_u8 *file_type);
EXT_PRV td_u32 uapi_upg_partion_update(td_u32 head_addr, EXT_CONST ext_21_upg_head *upg_head);

td_u32 upg_check_chip_magic(td_u32 chip_magic)
{
    if (chip_magic != PRODUCT_CFG_CHIP_MAGIC) {
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
    td_u16 product_type = (td_u16)uapi_get_product_type();

    for (i = 0; i < list_count; i++) {
        const ext_21_upg_head_product *item = &product_info[i];

        if ((product_type >= item->min_id) && (product_type <= item->max_id)) {
            return EXT_ERR_SUCCESS;
        }
    }

    return EXT_ERR_UPG_HEAD_CHECK_PRODUCT_LIST_WRONG;
}

td_u32 upg_check_file_type(td_u8 file_type)
{
    if (file_type == EXT_21_UPG_TYPE_KERNEL || file_type == EXT_21_UPG_TYPE_BOOT ||
        file_type == EXT_21_UPG_TYPE_PARTITION) {
        return EXT_ERR_SUCCESS;
    } else {
        return EXT_ERR_UPG_HEAD_CHECK_FILE_TYPE_WRONG;
    }
}

EXT_PRV td_u32 upg_conver_tail_addr_2_head_addr(td_u32 upg_tail_addr, td_u32 *upg_head_addr)
{
    td_u32 ret;
    ext_21_upg_tail tail = { 0 };

    /* read file tail */
    ret =
        uapi_flash_read((td_u32)(upg_tail_addr - sizeof(ext_21_upg_tail)), sizeof(ext_21_upg_tail), ((td_u8 *)(&tail)));
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    /* file ending magic byte verify */
    if (tail.head_end_magic != PRODUCT_CFG_UPG_FILE_END_SIGNATURE) {
        return EXT_ERR_UPG_CHECK_UPG_END_MAGIC_FAIL;
    }

    /* assure upgrade file head position */
    if (upg_tail_addr > tail.head_before_offset) {
        *upg_head_addr = upg_tail_addr - tail.head_before_offset;
    } else {
        return EXT_ERR_UPG_CHECK_UPG_END_OFFSET_FAIL;
    }

    return EXT_ERR_SUCCESS;
}

EXT_PRV td_u32 upg_get_head_from_flash(td_bool end_addr, td_u32 flash_addr, ext_21_upg_head *upg_head,
    td_u32 *upg_head_addr)
{
    td_u32 ret;

    /* get upg head position */
    if (end_addr == TD_TRUE) {
        ret = upg_conver_tail_addr_2_head_addr(flash_addr, upg_head_addr);
        if (ret != EXT_ERR_SUCCESS) {
            return ret;
        }
    } else {
        *upg_head_addr = flash_addr;
    }

    /* read upg head */
    ret = uapi_flash_read(*upg_head_addr, sizeof(ext_21_upg_head), (td_u8 *)upg_head);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    return ret;
}

EXT_PRV td_u32 upg_check_head_common_info(const ext_21_upg_head *upg_head, td_u32 head_size, td_u8 *file_type)
{
    td_u32 ret;
    td_u32 not_check_ret = EXT_ERR_SUCCESS;

    if (head_size < ext_struct_offset(ext_21_upg_head, chip_magic) + sizeof(upg_head->chip_magic)) {
        return not_check_ret;
    }

    /* file head chip  magic byte and manufacturer magic verify */
    ret = upg_check_chip_magic(upg_head->chip_magic);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    if (head_size < ext_struct_offset(ext_21_upg_head, manufactuer_code) + sizeof(upg_head->manufactuer_code)) {
        return not_check_ret;
    }

    ret = upg_check_manufactuer(upg_head->manufactuer_code);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    if (head_size < ext_struct_offset(ext_21_upg_head, file_type) + sizeof(upg_head->file_type)) {
        return not_check_ret;
    }

    *file_type = upg_head->file_type;
    ret = upg_check_file_type(upg_head->file_type);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    if (head_size < ext_struct_offset(ext_21_upg_head, product_list[0]) + sizeof(upg_head->product_list)) {
        return not_check_ret;
    }

    /* upg file product type verify */
    ret = upg_check_product_list(upg_head->product_list, PRODUCT_LIST_SIZE);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    return EXT_ERR_SUCCESS;
}

td_u32 uapi_upg_check_head(const ext_21_upg_head *upg_head, td_u32 head_size)
{
    td_u32 ret;
    ext_partition_ver ver_ret;
    td_u8 file_type = EXT_21_UPG_TYPE_NONE;
    td_u32 not_check_ret = EXT_ERR_SUCCESS;

    ret = upg_check_head_common_info(upg_head, head_size, &file_type);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    if (head_size < ext_struct_offset(ext_21_upg_head, partion_ver) + sizeof(upg_head->partion_ver)) {
        return not_check_ret;
    }

    ver_ret = uapi_flash_partition_check_ver(&upg_head->partion_ver);
    if (file_type == EXT_21_UPG_TYPE_KERNEL) {
        if (ver_ret == EXT_PARTITION_VER_THE_SAME) {
            return EXT_ERR_SUCCESS;
        } else {
            return EXT_ERR_UPG_HEAD_CHECK_PARTION_NOT_SAME;
        }
    } else if (file_type == EXT_21_UPG_TYPE_PARTITION) {
        if (ver_ret == EXT_PARTITION_VER_THE_SAME || ver_ret == EXT_PARTITION_VER_NEW) {
            return EXT_ERR_SUCCESS;
        } else {
            return EXT_ERR_UPG_HEAD_CHECK_PARTION_NOT_SUPPORT_UPG;
        }
    } else if (file_type == EXT_21_UPG_TYPE_BOOT) {
        /* boot not discriminated by product type. All of product use the same boot */
        return EXT_ERR_SUCCESS;
    } else {
        return EXT_ERR_UPG_HEAD_CHECK_PARTION_NOT_SUPPORT_UPG;
    }
}

EXT_PRV td_u32 uapi_upg_partion_update(td_u32 head_addr, EXT_CONST ext_21_upg_head *upg_head)
{
    td_u32 ret;
    td_u8 *partion_info = TD_NULL;

    if (upg_head->partition_info_offset == 0 || upg_head->partition_info_size == 0 ||
        upg_head->file_type != EXT_21_UPG_TYPE_PARTITION) {
        return EXT_ERR_SUCCESS;
    }

    partion_info = uapi_malloc(EXT_MOD_ID_DRV_UPG, upg_head->partition_info_size);
    if (partion_info == TD_NULL) {
        return EXT_ERR_FAILURE;
    }

    ret = uapi_flash_read(head_addr + upg_head->partition_info_offset, upg_head->partition_info_size, partion_info);
    if (ret != EXT_ERR_SUCCESS) {
        uapi_free(EXT_MOD_ID_DRV_UPG, partion_info);
        return ret;
    }

    ret = uapi_flash_partition_config((ext_flash_partition_chage_info *)partion_info,
        (td_u16)upg_head->partition_info_size);
    if (ret == EXT_ERR_PARTITION_UPDATA_TBL_SAME || ret == EXT_ERR_PARTITION_UPDATA_VER_SAME) {
        /* flash partition list process with success */
        ret = EXT_ERR_SUCCESS;
    }

    uapi_free(EXT_MOD_ID_DRV_UPG, partion_info);
    return ret;
}

/* weak check function. This function has no meaning if power down with exception before checking */
td_u32 uapi_upg_check_upg_file(td_bool end_addr, td_u32 flash_addr, ext_upg_file_base_info *upg_info)
{
    td_u32 ret;
    td_u8 check_sum[EXT_SHA256_SIZE] = { 0 };
    ext_21_upg_head *upg_head = TD_NULL;
    td_u32 upg_head_addr = 0;

    /* apply for upgrading file head space */
    upg_head = (ext_21_upg_head *)uapi_malloc(EXT_MOD_ID_DRV_UPG, sizeof(ext_21_upg_head));
    if (upg_head == TD_NULL) {
        ret = EXT_ERR_UPG_ERR_MALLOC_FAIL;
        goto END;
    }

    /* get upg head position */
    ret = upg_get_head_from_flash(end_addr, flash_addr, upg_head, &upg_head_addr);
    if (ret != EXT_ERR_SUCCESS) {
        goto END;
    }

    ret = uapi_upg_check_head(upg_head, sizeof(ext_21_upg_head));
    if (ret != EXT_ERR_SUCCESS) {
        goto END;
    }
    /* file section check with hash */
    ret = uapi_hash_flash_content(upg_head_addr + EXT_UPG_HEAD_CHECK_SUM_START_OFFSET,
        upg_head->file_len - EXT_UPG_HEAD_CHECK_SUM_START_OFFSET, check_sum, EXT_SHA256_SIZE);
    if ((ret != EXT_ERR_SUCCESS) || memcmp(upg_head->check_sum, check_sum, sizeof(upg_head->check_sum))) {
        ret = EXT_ERR_UPG_FILE_CHECK_SUM_WRONG;
        goto END;
    }

    ret = upg_check_secure_head(upg_head_addr, upg_head);
    if (ret != EXT_ERR_SUCCESS) {
        goto END;
    }

    ret = uapi_upg_partion_update(upg_head_addr, upg_head);
    if (ret != EXT_ERR_SUCCESS) {
        goto END;
    }

    /* return with file info */
    upg_info->file_type = upg_head->file_type;
    upg_info->flash_addr = upg_head_addr;
    upg_info->flash_size = upg_head->file_len;
    uapi_crc32_flash_content(0, upg_info->flash_addr, upg_info->flash_size, &(upg_info->crc));
    ret = EXT_ERR_SUCCESS;

END:
    if (upg_head != TD_NULL) {
        uapi_free(EXT_MOD_ID_DRV_UPG, upg_head);
        upg_head = TD_NULL;
    }

    return ret;
}

#ifdef __cplusplus
}
#endif
