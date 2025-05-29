/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: boot security
 * Author: CompanyName
 * Create: 2018-10-31
 */

#include "boot_check_secure_info.h"

#ifdef PRODUCT_CFG_SECURE_UPG
td_u8 g_boot_key_n[256] = { /* length 256 */
    0xb1, 0xd7, 0x94, 0x0d, 0x16, 0xc3, 0xef, 0x0e, 0x94, 0x5c, 0x93, 0xc2, 0xc8, 0xe3, 0x61, 0x9e,
    0x7c, 0xa1, 0x35, 0x87, 0x71, 0xbe, 0x01, 0xc3, 0x68, 0xf2, 0x6e, 0xa2, 0x6d, 0xb4, 0x50, 0x24,
    0x5a, 0xea, 0xfe, 0x18, 0x73, 0x47, 0x98, 0xf6, 0x46, 0xe7, 0xb3, 0xdb, 0xce, 0xaf, 0x79, 0x8f,
    0xf3, 0xe0, 0x7c, 0xcd, 0xdb, 0x60, 0xb1, 0xa9, 0x8f, 0x72, 0xa0, 0x14, 0xde, 0x4f, 0xa0, 0xad,
    0x05, 0x70, 0xbf, 0xae, 0xbf, 0x65, 0xce, 0x61, 0x97, 0x42, 0xc6, 0xf9, 0x46, 0xbb, 0x1a, 0x19,
    0x0c, 0x50, 0x11, 0x54, 0xb0, 0x5e, 0x2a, 0xfe, 0xab, 0xcf, 0xe2, 0xb6, 0x3f, 0x37, 0x0e, 0x4d,
    0x0e, 0x1d, 0x69, 0xdb, 0xf1, 0xd0, 0x7d, 0x46, 0x6e, 0x18, 0x19, 0x59, 0x8e, 0x3d, 0xdd, 0xbd,
    0x9d, 0x9a, 0x6b, 0x4d, 0x5d, 0x22, 0x5c, 0xed, 0xfa, 0x07, 0xbf, 0xfa, 0x4a, 0x60, 0xb8, 0x38,
    0xee, 0x1c, 0x92, 0x7c, 0x11, 0x01, 0x2a, 0x1b, 0xc1, 0x17, 0x7c, 0x25, 0x47, 0x5f, 0xe7, 0x61,
    0x3f, 0x77, 0xc7, 0x1b, 0x85, 0x2a, 0x83, 0xdd, 0xe4, 0xd7, 0x7e, 0x45, 0x4c, 0x73, 0x75, 0x6f,
    0x28, 0x08, 0xe9, 0x40, 0xb8, 0x1a, 0x51, 0x52, 0x13, 0xdd, 0xc3, 0xfb, 0xee, 0x48, 0x83, 0xec,
    0x7a, 0x68, 0x2c, 0xaf, 0x2f, 0xef, 0xdc, 0x5e, 0xda, 0xf2, 0x91, 0x0e, 0xc1, 0xcd, 0xc9, 0xab,
    0x62, 0x34, 0xdd, 0xe6, 0x87, 0x15, 0x29, 0xd8, 0x81, 0x5b, 0xa2, 0xdf, 0x0d, 0x65, 0xcd, 0xa6,
    0xfc, 0xfe, 0x51, 0xe1, 0xb4, 0xaf, 0x0a, 0xf0, 0x6e, 0xd9, 0xa0, 0xfb, 0x50, 0xf7, 0xfb, 0xbb,
    0x33, 0xf0, 0xed, 0xf9, 0x01, 0xcd, 0xe3, 0x57, 0xc7, 0x4a, 0xae, 0xce, 0x90, 0xa1, 0x29, 0xd7,
    0xe1, 0xa0, 0x52, 0x1b, 0x9c, 0x37, 0x81, 0x65, 0x92, 0x5c, 0xf3, 0x32, 0xd2, 0x23, 0xdf, 0x33
};

td_void uapi_secure_save_key_pos(td_void)
{
    uintptr_t key_flash_addr = ((uintptr_t)(&g_boot_key_n[0]) - (uintptr_t)(&__data_begin__)) +
        ((uintptr_t)(&__data_flash_begin__) - 0x1000000);
    *(volatile unsigned int *)(EXT_SYSCTRL_REG_BASE + REG_RSA_KEY_ADDR) = key_flash_addr;
}

EXT_PRV td_bool uapi_is_secure_start(td_void)
{
    td_u8 data = { 0 };
    td_u32 ret;

    ret = (td_u32)uapi_efuse_read(EXT_EFUSE_SECURE_BOOT_RW_ID, &data);
    if (ret != EXT_ERR_SUCCESS) {
        return TD_TRUE;
    }

    if (data & 0x1) {
        boot_msg0("secure boot");
        return TD_TRUE;
    } else {
        boot_msg0("unsecure boot");
        return TD_FALSE;
    }
}

td_u32 boot_start_check_tbl_secure_info(td_void)
{
    ext_start_tbl *start_tbl = ext_start_tbl_get_tbl();
    if (g_start_ctrl.start_tbl == TD_NULL || g_start_ctrl.start_tbl->section == TD_NULL) {
        return EXT_ERR_FAILURE;
    }
    ext_start_tbl_section *start_tbl_sections = ext_start_tbl_get_sections();
    ext_upg_secure_info secure_info = { 0 };
    uintptr_t addr[1] = { 0 };
    td_u32 size[1] = { 0 };
    td_bool flash[1] = { 0 };
    td_u32 ret;

    /* insecure check efuse. return success immediately */
    if (!uapi_is_secure_start()) {
        return EXT_ERR_SUCCESS;
    }

    /* without secure head, return fail immediately */
    if (start_tbl->secure_info_offset == 0 || start_tbl->secure_info_size == 0) {
        return EXT_ERR_UPG_SECURE_BOOT_28;
    }

    /* read secure head */
    ret = uapi_flash_read(g_start_ctrl.flash_addr_ker + start_tbl->secure_info_offset, sizeof(ext_upg_secure_info),
        (td_u8 *)(&secure_info));
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    /* using the public key in boot code.  verify the signature of public key in secure head */
    addr[0] = (uintptr_t)secure_info.key_n_2;
    size[0] = 256; /* size 256 */
    flash[0] = TD_FALSE;
    verify_flash_content_params params1 = { g_boot_key_n, secure_info.signature_1, secure_info.sign_alg };
    ret = uapi_rsa_verify_flash_content(addr, size, 1, flash, &params1);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    /* using the public key in secure head. verify the content, kernel, conducted by starting info list */
    addr[0] = g_start_ctrl.flash_addr_ker + start_tbl_sections[0].offset;
    size[0] = start_tbl_sections[0].size;
    flash[0] = TD_TRUE;
    verify_flash_content_params params2 = { secure_info.key_n_2, secure_info.signature_2, secure_info.sign_alg };
    ret = uapi_rsa_verify_flash_content(addr, size, 1, flash, &params2);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    return EXT_ERR_SUCCESS;
}

td_u32 boot_upg_check_secure_head(td_void)
{
    td_u32 ret;
    ext_21_upg_head *upg_head = ext_upg_head_get_head();
    ext_upg_secure_info secure_info = { 0 };
    size_t addr[2] = { 0 }; /* length 2*4 */
    size_t size[2] = { 0 };    /* length 2*4 */
    td_bool flash[2] = { 0 };  /* length 2 */

    /* insecure check efuse. return success immediately */
    if (!uapi_is_secure_start()) {
        return EXT_ERR_SUCCESS;
    }

    /* without secure head, return fail immediately */
    if (upg_head->secure_info_offset == 0 || upg_head->secure_info_size == 0) {
        return EXT_ERR_UPG_SECURE_BOOT_27;
    }

    /* read secure head */
    ret = uapi_flash_read(g_start_ctrl.flash_addr_upg + upg_head->secure_info_offset, sizeof(ext_upg_secure_info),
        (td_u8 *)(&secure_info));
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }
    boot_msg4("upg sig_1 ", secure_info.signature_1[0], secure_info.signature_1[1],
        secure_info.signature_1[254], secure_info.signature_1[255]); /* index 254&255 */
    boot_msg4("upg sig_2 ", secure_info.signature_2[0], secure_info.signature_2[1],
        secure_info.signature_2[254], secure_info.signature_2[255]); /* index 254&255 */
    boot_msg4("upg key_n_2 ", secure_info.key_n_2[0], secure_info.key_n_2[1],
        secure_info.key_n_2[254], secure_info.key_n_2[255]); /* index 254&255 */
    /* using the public key in boot code.  verify the signature of public key in secure head */
    addr[0] = (uintptr_t)secure_info.key_n_2;
    size[0] = 256; /* size 256 */
    flash[0] = TD_FALSE;
    verify_flash_content_params params1 = { g_boot_key_n, secure_info.signature_1, secure_info.sign_alg };
    ret = uapi_rsa_verify_flash_content(addr, size, 1, flash, &params1);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }
    /* using the public key in secure head. checking upgrade file.
     * Checking includes: all the content except secure head and file hash .
     * file splited into 2 pieces by secure head , so need sectional check. (check sum, secure head) partial check.
     */
    addr[0] = g_start_ctrl.flash_addr_upg + EXT_UPG_HEAD_CHECK_SUM_START_OFFSET;
    size[0] = upg_head->secure_info_offset - EXT_UPG_HEAD_CHECK_SUM_START_OFFSET;
    flash[0] = TD_TRUE;

    /* (secure head, file tail ) partial check */
    addr[1] = g_start_ctrl.flash_addr_upg + upg_head->secure_info_offset + upg_head->secure_info_size;
    size[1] = upg_head->file_len - (upg_head->secure_info_offset + upg_head->secure_info_size);
    flash[1] = TD_TRUE;
    verify_flash_content_params params2 = { secure_info.key_n_2, secure_info.signature_2, secure_info.sign_alg };
    ret = uapi_rsa_verify_flash_content(addr, size, 2, flash, &params2); /* buffer count :2 */
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    return EXT_ERR_SUCCESS;
}

td_u32 boot_upg_check_secure_boot(td_void)
{
    td_u32 ret;
    ext_start_ctrl *start_ctrl = &g_start_ctrl;
    ext_21_upg_head_section *section_info = TD_NULL;

    /* insecure check efuse. return success immediately */
    if (!uapi_is_secure_start()) {
        return EXT_ERR_SUCCESS;
    }

    /* boot security check */
    if (g_start_ctrl.upg_sections == TD_NULL) {
        return EXT_ERR_FAILURE;
    }
    section_info = ext_upg_head_get_section(0);
    ret = flash_boot_security_boot_verify(
        (td_pvoid)(uintptr_t)(0x1000000 + start_ctrl->flash_addr_upg + section_info->offset));
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    return EXT_ERR_SUCCESS;
}
#endif /* PRODUCT_CFG_SECURE_UPG */
