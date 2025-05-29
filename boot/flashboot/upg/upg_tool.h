/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: upg tools head file
 * Author: CompanyName
 * Create: 2018-10-31
 */

#ifndef __UPG_TOOL_H__
#define __UPG_TOOL_H__

#include <crc32.h>
#include <lzmaram.h>
#include <soc_boot_rom.h>
#include <soc_flashboot_flash.h>

typedef struct {
    td_u8 *key_n;
    td_u8 *signature;
    ext_rsa_padding_mode pad_mode;
} verify_flash_content_params;

td_u32 upg_crc_block(td_u32 flash_addr, td_u32 total_size, td_u32 *crc_result);
td_u32 upg_hash_block(td_u32 flash_addr, td_u32 total_size, td_u8 hash_result[32]); /* length 32 */
td_u32 upg_copy_flash_2_flash(td_u32 src_flash_addr, td_u32 dst_flash_addr, td_u32 src_flash_size,
    td_u32 dst_flash_size, td_bool comprss);
td_u32 uapi_rsa_verify_flash_content(const td_u32 flash_addr[3], const td_u32 total_size[3],     /* length 3 */
    td_u32 buffer_count, const td_bool b_flash[3], const verify_flash_content_params *params); /* length 3 */
#endif
