/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Upgrade base funciton - tool.
 * Author: CompanyName
 * Create: 2018-10-31
 */

#include "upg_base_tool.h"
#include <soc_mdm_types.h>
#include <soc_mdm_flash.h>
#include <soc_mdm_crc.h>
#include <soc_stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define EXT_BLOCK_SIZE 0x100
td_u8 g_upg_tool_block[EXT_BLOCK_SIZE];

EXT_PRV td_u32 uapi_hash_flash_content_extend(const upg_rsa_verify_param *param, td_u8 *hash_result,
    td_u32 hash_result_size);

td_u32 uapi_crc32_flash_content(td_u32 crc_start, td_u32 flash_addr, td_u32 total_size, td_u32 *crc_result)
{
    td_u8 *block = g_upg_tool_block;
    td_u32 offset;
    td_u32 this_size;
    td_u32 ret;
    if (crc_result == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    for (offset = 0; offset < total_size;) {
        this_size = (total_size - offset > EXT_BLOCK_SIZE) ? EXT_BLOCK_SIZE : (total_size - offset);

        ret = uapi_flash_read(flash_addr + offset, this_size, block);
        if (ret != EXT_ERR_SUCCESS) {
            return ret;
        }

        crc_start = uapi_crc32(crc_start, block, this_size);
        offset += this_size;
    }

    *crc_result = crc_start;

    return EXT_ERR_SUCCESS;
}

td_u32 uapi_hash_flash_content(td_u32 flash_addr, td_u32 total_size, td_u8 *hash_result, td_u32 hash_result_size)
{
    td_u8 *block = g_upg_tool_block;
    td_u32 offset;
    td_u32 this_size;
    td_u32 ret;
    td_u16 hash_handle = 0;

    ext_unref_param(hash_result_size);

    ret = uapi_hash_start(&hash_handle);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    for (offset = 0; offset < total_size;) {
        this_size = (total_size - offset > EXT_BLOCK_SIZE) ? EXT_BLOCK_SIZE : (total_size - offset);

        ret = uapi_flash_read(flash_addr + offset, this_size, block);
        if (ret != EXT_ERR_SUCCESS) {
            return ret;
        }

        ret = uapi_hash_update(hash_handle, block, this_size);
        if (ret != EXT_ERR_SUCCESS) {
            return ret;
        }

        offset += this_size;
    }

    return uapi_hash_final(hash_handle, hash_result);
}

EXT_PRV td_u32 uapi_hash_flash_content_extend(const upg_rsa_verify_param *param, td_u8 *hash_result,
    td_u32 hash_result_size)
{
    td_u8 *block = g_upg_tool_block;
    uintptr_t offset;
    uintptr_t this_size;
    td_u32 ret;
    td_u32 i;
    td_u16 hash_handle = 0;

    ext_unref_param(hash_result_size);

    if (param == TD_NULL) {
        return EXT_ERR_FAILURE;
    }

    ret = uapi_hash_start(&hash_handle);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    for (i = 0; i < param->buffer_count; i++) {
        for (offset = 0; offset < param->total_size[i];) {
            this_size =
                (param->total_size[i] - offset > EXT_BLOCK_SIZE) ? EXT_BLOCK_SIZE : (param->total_size[i] - offset);

            if (param->flash_flag[i]) {
                uapi_flash_read(param->flash_addr[i] + offset, this_size, block);
            } else {
                ret = (td_u32)memcpy_s(block, EXT_BLOCK_SIZE, (char *)(param->flash_addr[i] + offset), this_size);
                ext_unref_param(ret);
            }

            ret = uapi_hash_update(hash_handle, block, this_size);
            if (ret != EXT_ERR_SUCCESS) {
                return ret;
            }
            offset += this_size;
        }
    }

    return uapi_hash_final(hash_handle, hash_result);
}

td_u32 uapi_rsa_verify_flash_content(const upg_rsa_verify_param *param)
{
    td_u32 ret;
    td_u8 hash_result[EXT_SHA256_SIZE] = { 0 };

    /* calculate SHA256 */
    ret = uapi_hash_flash_content_extend(param, hash_result, sizeof(hash_result));
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    /* Verify signature. */
    return uapi_rsa_verify(param->key_n, param->key_e, param->key_len, param->pad_mode, hash_result, param->sign);
}

#ifdef __cplusplus
}
#endif
