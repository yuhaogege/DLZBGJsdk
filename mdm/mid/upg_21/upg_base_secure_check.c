/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Upgrade base funciton - secure check.
 */

#include "upg_base_secure_check.h"
#include "upg_base_tool.h"

#include <soc_mdm_cipher.h>
#include <soc_mdm_flash.h>
#include <soc_mdm_mem.h>
#include <soc_stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

static inline td_void rsa_gen_exp_e(td_u8 *key_e, td_u32 key_len)
{
    (td_void)memset_s((key_e), (key_len), 0x0, (key_len));
    (key_e)[(key_len) - 1] = 0x01;
    (key_e)[(key_len) - 3] = 0x01; // last 3 index
}

#define upg_base_free(p)                 \
    if ((p) != TD_NULL) {                \
        uapi_free(EXT_MOD_ID_DRV_UPG, (p)); \
        (p) = TD_NULL;                   \
    }

EXT_PRV td_bool upg_secure_is_secure_efuse(td_void)
{
    td_u8 data = 0;
    td_u32 ret;

    ret = uapi_efuse_read(EXT_EFUSE_SECURE_BOOT_RW_ID, &data);
    if (ret != EXT_ERR_SUCCESS) {
        return TD_TRUE;
    }

    if (data & 0x1) {
        return TD_TRUE;
    } else {
        return TD_FALSE;
    }
}

EXT_PRV td_u32 upg_secure_get_boot_key(td_u8 *key_n, td_u32 key_len)
{
    td_u32 boot_key_addr = *(volatile unsigned int *)(DW21_SYSCTRL_SC_GEN_REG2_REG);

    if (boot_key_addr == 0) {
        return EXT_ERR_FAILURE;
    }

    return uapi_flash_read(boot_key_addr, key_len, key_n);
}

EXT_PRV td_u32 upg_check_secure_pubkey(EXT_CONST ext_upg_secure_info *secure_info,
    td_u8 *key_n, td_u8 *key_e, td_u32 key_len)
{
    upg_rsa_verify_param param;

    memset_s(&param, sizeof(param), 0, sizeof(param));
    param.flash_addr[0] = (uintptr_t)secure_info->key_n_2;
    param.total_size[0] = (uintptr_t)key_len;
    param.buffer_count = 1; /* 1 segment */
    param.key_n = key_n;
    param.key_e = key_e;
    param.key_len = key_len;
    param.sign = (td_u8 *)secure_info->signature_1;
    param.pad_mode = (ext_rsa_padding_mode)secure_info->sign_alg;
    return uapi_rsa_verify_flash_content(&param);
}

EXT_PRV td_u32 upg_check_secure_file_data(td_u32 flash_addr, EXT_CONST ext_21_upg_head *upg_head,
    ext_upg_secure_info *secure_info, td_u8 *key_e, td_u32 key_len)
{
    upg_rsa_verify_param param;

    memset_s(&param, sizeof(param), 0, sizeof(param));

    /* (Checksum, security header) partial verification */
    param.flash_addr[0] = flash_addr + EXT_UPG_HEAD_CHECK_SUM_START_OFFSET;
    param.total_size[0] = (uintptr_t)(upg_head->secure_info_offset - EXT_UPG_HEAD_CHECK_SUM_START_OFFSET);
    param.flash_flag[0] = TD_TRUE;

    /* (Security header, file tail) Partial verification */
    param.flash_addr[1] = flash_addr + upg_head->secure_info_offset + upg_head->secure_info_size;
    param.total_size[1] = upg_head->file_len - (upg_head->secure_info_offset + upg_head->secure_info_size);
    param.flash_flag[1] = TD_TRUE;

    param.buffer_count = 2; /* 2 segments */
    param.key_n = secure_info->key_n_2;
    param.key_e = key_e;
    param.key_len = key_len;
    param.sign = secure_info->signature_2;
    param.pad_mode = (ext_rsa_padding_mode)secure_info->sign_alg;

    return uapi_rsa_verify_flash_content(&param);
}

td_u32 upg_check_secure_head(td_u32 flash_addr, EXT_CONST ext_21_upg_head *upg_head)
{
    td_u32 ret;
    ext_upg_secure_info *secure_info = TD_NULL;

    td_u8 *key_n = TD_NULL;
    td_u8 *key_e = TD_NULL;
    td_u32 key_len = EXT_RSA_2048_LEN;

    /* If the EFUSE is not verified, a success message is returned. */
    if (upg_secure_is_secure_efuse() == TD_FALSE) {
        return EXT_ERR_SUCCESS;
    }

    /* No security header return fail */
    if (upg_head->secure_info_offset == 0 || upg_head->secure_info_size == 0) {
        return EXT_ERR_FAILURE;
    }

    /* Memory space required for security verification */
    key_n = uapi_malloc(EXT_MOD_ID_DRV_UPG, key_len);
    key_e = uapi_malloc(EXT_MOD_ID_DRV_UPG, key_len);
    secure_info = uapi_malloc(EXT_MOD_ID_DRV_UPG, sizeof(ext_upg_secure_info));
    if ((key_n == TD_NULL) || (key_e == TD_NULL) || (secure_info == TD_NULL)) {
        ret = EXT_ERR_FAILURE;
        goto END;
    }

    ret = upg_secure_get_boot_key(key_n, key_len);
    if (ret != EXT_ERR_SUCCESS) {
        goto END;
    }

    ret = uapi_rsa_init((td_u8 *)TD_NULL, (td_u16)key_len);
    if (ret != EXT_ERR_SUCCESS) {
        goto END;
    }

    /* Generate p_key_e */
    rsa_gen_exp_e(key_e, key_len);

    /* Read the security header. */
    ret = uapi_flash_read(flash_addr + upg_head->secure_info_offset, sizeof(ext_upg_secure_info), (td_u8 *)secure_info);
    if (ret != EXT_ERR_SUCCESS) {
        goto END;
    }

    /* Use the public key in the boot code to verify the signature of the public key in the security header. */
    ret = upg_check_secure_pubkey(secure_info, key_n, key_e, key_len);
    if (ret != EXT_ERR_SUCCESS) {
        goto END;
    }

    /*
     * The security header splits the file into two parts and needs to be checked by segment.
     * Use the public key in the security header to verify the upgrade file.
     * The verification content contains all contents
     * except the security header and file hash.
     */
    ret = upg_check_secure_file_data(flash_addr, upg_head, secure_info, key_e, key_len);
    if (ret != EXT_ERR_SUCCESS) {
        goto END;
    }

END:

    upg_base_free(secure_info);
    upg_base_free(key_e);
    upg_base_free(key_n);

    return ret;
}

#ifdef __cplusplus
}
#endif
