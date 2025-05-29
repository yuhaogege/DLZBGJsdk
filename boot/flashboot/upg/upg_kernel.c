/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: kernel upg
 * Author: CompanyName
 * Create: 2018-10-31
 */

#include "upg_kernel.h"

td_u32 boot_upg_kernel_lzma_detect(const ext_start_ctrl *start_ctrl, const ext_21_upg_head_section *section_info,
    td_u32 *uncompress_size)
{
    td_u8 lzma_head[13] = { 0 }; /* head 13B */
    td_u32 dic_size = 0;
    td_u32 ret;

    /* get LZMA head. get uncompressed size */
    ret = uapi_flash_read(start_ctrl->flash_addr_upg + section_info->offset, 13, lzma_head); /* read 13B */
    ret |= uapi_lzma_get_uncompress_len(lzma_head, sizeof(lzma_head), uncompress_size, &dic_size);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    /* RAM usage detect for  LAMA uncompressing.
     * Avoid can't uncompress after erasing file, it'll cause unable to start */
    ret = uapi_lzma_mem_detect(lzma_head, sizeof(lzma_head));
    return ret;
}
td_u32 boot_upg_kernel_process(td_void)
{
    td_u32 ret = EXT_ERR_SUCCESS;
    ext_start_ctrl *start_ctrl = &g_start_ctrl;
    ext_21_upg_head_section *section_info0 = TD_NULL;
    ext_21_upg_head_section *section_info1 = TD_NULL;
    td_u32 erase_size0, erase_size1;
    td_u32 uncompress_size0, uncompress_size1;
    if (g_start_ctrl.upg_sections == TD_NULL) {
        return EXT_ERR_FAILURE;
    }
    section_info0 = ext_upg_head_get_section(0);
    section_info1 = ext_upg_head_get_section(1);

    if (section_info0->attribute_compress) { /* kernel image is compressed file: execute currently */
        ret = boot_upg_kernel_lzma_detect(start_ctrl, section_info0, &uncompress_size0);
        if (ret != EXT_ERR_SUCCESS) {
            return ret;
        }
    } else {
        uncompress_size0 = section_info0->compress_size;
    }

    if (uncompress_size0 == 0) {
        return EXT_ERR_UPG_FAIL_2;
    }

    if (section_info1->attribute_compress) { /* NV image is compressed file: execute currently */
        ret = boot_upg_kernel_lzma_detect(start_ctrl, section_info1, &uncompress_size1);
        if (ret != EXT_ERR_SUCCESS) {
            return ret;
        }
    } else {
        uncompress_size1 = section_info1->compress_size;
    }

    if (uncompress_size1 == 0) {
        return EXT_ERR_UPG_FAIL_3;
    }

    /* make 4K allignment for kernal and NV before compression. Erasing target space */
    erase_size0 = uncompress_size0;
    erase_size0 = align_length(erase_size0, ALIGNTYPE_4K);
    erase_size1 = uncompress_size1;
    erase_size1 = align_length(erase_size1, ALIGNTYPE_4K);
    boot_msg2("[LOG][UPG]erase", start_ctrl->flash_addr_ker, erase_size0);
    boot_msg2("[LOG][UPG]erase", start_ctrl->flash_addr_nv_file, erase_size1);
    ret |= uapi_flash_erase(start_ctrl->flash_addr_ker, erase_size0);
    ret |= uapi_flash_erase(start_ctrl->flash_addr_nv_file, erase_size1);

    /* uncompress kernel and nv file to target space */
    ret |= upg_copy_flash_2_flash(start_ctrl->flash_addr_upg + section_info0->offset, start_ctrl->flash_addr_ker,
        section_info0->compress_size, uncompress_size0, section_info0->attribute_compress);

    ret |= upg_copy_flash_2_flash(start_ctrl->flash_addr_upg + section_info1->offset, start_ctrl->flash_addr_nv_file,
        section_info1->compress_size, uncompress_size1, section_info1->attribute_compress);

    return ret;
}
