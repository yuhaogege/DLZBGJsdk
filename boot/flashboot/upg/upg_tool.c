/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: upg tools
 * Author: CompanyName
 * Create: 2018-10-31
 */

#include "upg_tool.h"

#define EXT_BLOCK_SIZE 0x100
td_u8 g_upg_tool_block[EXT_BLOCK_SIZE];

/* *
 * @ingroup  UPG
 * @brief  :flash section check with CRC
 *
 * @par describe:
 * flash section check with CRC
 *
 * @attention :nothing
 * @param  flash_addr                [IN] type #UAPI_U32, FLASH start address
 * @param  total_size                [IN] type #UAPI_U32, FLASH size
 * @param  p_crc_result              [OUT] type #UAPI_U32, check result
 *
 * @retval #EXT_ERR_SUCCESS
 * @retval #other value: fail. see soc_errno.h for details.
 * @par dependence:None.
 *
 * @see  :nothing
 * @since DW21_V100R001C00
 */
td_u32 upg_crc_block(td_u32 flash_addr, td_u32 total_size, td_u32 *crc_result_point)
{
    td_u8 *block = g_upg_tool_block;
    td_u32 offset;
    td_u32 this_size;
    td_u32 crc_result = 0;

    for (offset = 0; offset < total_size;) {
        this_size = (total_size - offset > EXT_BLOCK_SIZE) ? EXT_BLOCK_SIZE : (total_size - offset);
        uapi_flash_read(flash_addr + offset, this_size, block);
        crc_result = uapi_crc32(crc_result, block, this_size);
        offset += this_size;
    }

    *crc_result_point = crc_result;

    return EXT_ERR_SUCCESS;
}

/* *
 * @ingroup  UPG
 * @brief  :flash section check with HASH
 *
 * @par describe:
 * flash section check with HASH
 *
 * @attention :nothing
 * @param  flash_addr                [IN] type#UAPI_U32, FLASH start address
 * @param  total_size                [IN] type#UAPI_U32, FLASH size
 * @param  hash_result               [OUT] type#UAPI_U32, check result
 *
 * @retval #EXT_ERR_SUCCESS
 * @retval #other value: fail. see soc_errno.h for details.
 * @par dependence:None.
 *
 * @see  :nothing
 * @since DW21_V100R001C00
 */
td_u32 upg_hash_block(td_u32 flash_addr, td_u32 total_size, td_u8 hash_result[32]) /* length 32 */
{
    td_u8 *block = g_upg_tool_block;
    td_u32 ret;
    td_u32 offset;
    td_u32 this_size;

    uapi_hash_start();

    for (offset = 0; offset < total_size;) {
        this_size = (total_size - offset > EXT_BLOCK_SIZE) ? EXT_BLOCK_SIZE : (total_size - offset);
        uapi_flash_read(flash_addr + offset, this_size, block);
        uapi_hash_update(block, this_size);
        offset += this_size;
    }

    ret = (td_u32)uapi_hash_final(hash_result);
    return ret;
}

static td_u32 g_compress_src_addr = 0;
static td_u32 g_compress_dst_addr = 0;

EXT_PRV td_u32 kernel_comprss_flash_read(td_u32 offset, td_u8 *buffer, td_u32 size)
{
    uapi_flash_read(g_compress_src_addr + offset, size, buffer);
    return 0;
}

EXT_PRV td_u32 kernel_comprss_flash_write(td_u32 offset, td_u8 *buffer, td_u32 size)
{
    uapi_flash_write(g_compress_dst_addr + offset, size, buffer, TD_FALSE);
    return size;
}

/* *
 * @ingroup  UPG
 * @brief  : flash section copy OR lzma uncompressing
 *
 * @par describe:
 * flash section copy OR lzma uncompressing
 *
 * @attention: nothing
 * @param  src_flash_addr                [IN] type#UAPI_U32, original file flash start address
 * @param  dst_flash_addr                [IN] type#UAPI_U32, destination flash start address
 * @param  src_flash_size                [IN] type#UAPI_U32, original file size
 * @param  dst_flash_size                [IN] type#UAPI_U32, destination file size
 * @param  b_comprss                     [IN] type#UAPI_U32, original file is lzma compressed flag
 *
 * @retval #EXT_ERR_SUCCESS
 * @retval #other value: fail. see soc_errno.h for details.
 * @par dependence:None.
 *
 * @see  :nothing
 * @since DW21_V100R001C00
 */
td_u32 upg_copy_flash_2_flash(td_u32 src_flash_addr, td_u32 dst_flash_addr, td_u32 src_flash_size,
    td_u32 dst_flash_size, td_bool comprss)
{
    td_u8 *block = g_upg_tool_block;
    td_u32 offset;
    td_u32 this_size;
    td_u8 lzma_head[13] = { 0 }; /* head 13B */
    td_u32 ret = EXT_ERR_SUCCESS;

    if (!comprss) {
        for (offset = 0; offset < dst_flash_size;) {
            this_size = (dst_flash_size - offset > EXT_BLOCK_SIZE) ? EXT_BLOCK_SIZE : (dst_flash_size - offset);
            uapi_flash_read(src_flash_addr + offset, this_size, block);
            uapi_flash_write(dst_flash_addr + offset, this_size, block, TD_FALSE);
            offset += this_size;
        }
    } else {
        boot_msg4("\n copyz param", src_flash_addr, src_flash_size, dst_flash_addr, dst_flash_size);
        uapi_flash_read(src_flash_addr, 13, lzma_head); /* read 13B */
        g_compress_src_addr = src_flash_addr;
        g_compress_dst_addr = dst_flash_addr;
        ret = uapi_lzma_decompress(lzma_head, sizeof(lzma_head), src_flash_size, kernel_comprss_flash_read,
            kernel_comprss_flash_write);
    }

    return ret;
}

td_u32 upg_get_uncompress_size(td_u32 src_flash_addr, td_u32 *uncompress_size)
{
    td_u32 ret = EXT_ERR_SUCCESS;
    td_u8 lzma_head[13] = { 0 }; /* head 13B */
    td_u32 dic_size = 0;

    ret |= uapi_flash_read(src_flash_addr, 13, lzma_head); /* read 13B */
    ret |= uapi_lzma_get_uncompress_len(lzma_head, sizeof(lzma_head), uncompress_size, &dic_size);
    return ret;
}

EXT_PRV td_u32 upg_hash_block_2(const td_u32 flash_addr[2], const td_u32 total_size[2], /* length 2*4 */
    td_u32 buffer_count, const td_bool b_flash[3], td_u8 hash_result[32])       /* length 3 and 32 */
{
    td_u8 *block = g_upg_tool_block;
    td_u32 i;
    td_u32 ret;
    td_u32 offset;
    td_u32 this_size;

    uapi_hash_start();
    for (i = 0; i < buffer_count; i++) {
        for (offset = 0; offset < total_size[i];) {
            this_size = (total_size[i] - offset > EXT_BLOCK_SIZE) ? EXT_BLOCK_SIZE : (total_size[i] - offset);
            if (b_flash[i] != 0) {
                uapi_flash_read(flash_addr[i] + offset, this_size, block);
            } else if (memcpy_s(block, EXT_BLOCK_SIZE, (void *)((uintptr_t)flash_addr[i] + offset),
                this_size) != BOOT_SUCCESS) {
                return BOOT_FAILURE;
            }
            uapi_hash_update(block, this_size);
            offset += this_size;
        }
    }
    ret = (td_u32)uapi_hash_final(hash_result);
    return ret;
}

/* *
 * @ingroup  UPG
 * @brief: flash section check with RSA
 *
 * @par describe:
 * flash section check with RSA
 *
 * @attention: nothing
 * @param  flash_addr   [IN] type #UAPI_U32, check start adress list of content. support multiple section content
 * @param  total_size    [IN] type #UAPI_U32, check content size list. support multiple section content
 * @param  buffer_count         [IN] type #UAPI_U32, check section count of content
 * @param  b_flash       [IN] type #UAPI_U32, check content position(RAM or FLASH), support multiple section content
 * @param  key_n         [IN] type #UAPI_U32, value of key_n
 * @param  signature    [IN] type #TD_U8, signature
 * @param  pad_mode  [IN] type #TD_U8, padding mode
 *
 * @retval #EXT_ERR_SUCCESS
 * @retval #other value: fail. see soc_errno.h for details.
 * @par dependence:None.
 *
 * @see  :nothing
 * @since DW21_V100R001C00
 */
td_u32 uapi_rsa_verify_flash_content(const td_u32 flash_addr[3],                 /* length 3*4 */
    const td_u32 total_size[3], td_u32 buffer_count, const td_bool b_flash[3], /* length 3 */
    const verify_flash_content_params *params)
{
    td_u32 ret;
    td_u8 hash_result[32] = { 0 }; /* length 32 */
    td_u8 *key_e = TD_NULL;
    const td_u32 key_len = 256; /* key len 256 */
    td_u8 *hash_result_point = hash_result;

    /* apply for key_e space */
    key_e = boot_malloc(256); /* length 256 */
    if (key_e == TD_NULL) {
        return EXT_ERR_FAILURE;
    }

    /* key_e equal to 0x010001 */
    (td_void)memset_s(key_e, key_len, 0x0, key_len);
    key_e[255] = 0x01; /* index 255 */
    key_e[253] = 0x01; /* index 253 */

    /* calculate content with sha256 */
    ret = upg_hash_block_2(flash_addr, total_size, buffer_count, b_flash, hash_result_point);
    if (ret != EXT_ERR_SUCCESS) {
        boot_free(key_e);
        return ret;
    }

    /* verify signature */
    ret = (td_u32)uapi_rsa_verify(params->key_n, key_e, 256, /* key len 256 */
        params->pad_mode, hash_result, params->signature);
    boot_free(key_e);
    return ret;
}
