/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: upgrade boot process
 * Author: CompanyName
 * Create: 2018-10-31
 */

#include "upg_boot.h"

td_u32 boot_upg_process_boot(td_void)
{
    ext_start_ctrl *start_ctrl = &g_start_ctrl;
    ext_21_upg_head_section *section_info = TD_NULL;
    td_u32 erase_size;
    td_u32 uncompress_size = 0;
    td_u32 ret = EXT_ERR_SUCCESS;
    if (g_start_ctrl.upg_sections == TD_NULL) {
        return EXT_ERR_FAILURE;
    }
    section_info = ext_upg_head_get_section(0);
    if (section_info == TD_NULL) {
        return EXT_ERR_FAILURE;
    }

    /* boot sub section is compressed file: do not enter this branch if boot is uncomppressed because of secure check */
    if (section_info->attribute_compress) {
        td_u8 lzma_head[13] = { 0 }; /* head 13B */
        td_u32 dic_size = 0;
        /* get LZMA head, get size before compression */
        ret |= uapi_flash_read(start_ctrl->flash_addr_upg + section_info->offset, 13, lzma_head); /* read 13B */
        if (ret != EXT_ERR_SUCCESS) {
            return ret;
        }

        ret |= uapi_lzma_get_uncompress_len(lzma_head, sizeof(lzma_head), &uncompress_size, &dic_size);
        if (ret != EXT_ERR_SUCCESS) {
            return ret;
        }

        /* predict RAM usage when LZMA is uncompressing. Avoid causing unable start after erasing file */
        ret = uapi_lzma_mem_detect(lzma_head, sizeof(lzma_head));
        if (ret != EXT_ERR_SUCCESS) {
            return ret;
        }
    } else { /* situation of uncompressed: pres ent size equal to before compression */
        uncompress_size = section_info->compress_size;
    }

    if (uncompress_size == 0) {
        return EXT_ERR_FAILURE;
    }

    /* make 4K alignment for size before compression . erase BOOT */
    erase_size = uncompress_size;
    erase_size = align_length(erase_size, ALIGNTYPE_4K);
    ret |= uapi_flash_erase(start_ctrl->flash_addr_boot, erase_size);

    /* uncompress or copy upgrade file content to running space */
    ret |= upg_copy_flash_2_flash(start_ctrl->flash_addr_upg + section_info->offset, start_ctrl->flash_addr_boot,
        section_info->compress_size, uncompress_size, section_info->attribute_compress);
    return ret;
}
