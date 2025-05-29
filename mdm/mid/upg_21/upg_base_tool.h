/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Upgrade base funciton - tool.
 * Author: CompanyName
 * Create: 2018-10-31
 */

#ifndef __UPG_BASE_TOOL_H__
#define __UPG_BASE_TOOL_H__

#include <soc_types.h>
#include <soc_mdm_cipher.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FLASH_ADDR_NUM 2
#define TOTAL_SIZE_NUM 2
#define FLASH_FLAG_NUM 2

typedef struct {
    uintptr_t flash_addr[FLASH_ADDR_NUM];
    uintptr_t total_size[TOTAL_SIZE_NUM];
    td_u32 buffer_count;
    td_bool flash_flag[FLASH_FLAG_NUM];
    td_bool rsv[2]; /* pad(2) */
    td_u8 *key_n;
    td_u8 *key_e;
    td_u8 *sign;
    td_u32 key_len;
    ext_rsa_padding_mode pad_mode;
} upg_rsa_verify_param;

td_u32 uapi_upg_get_cur_kernel_size(td_void);
td_u32 uapi_hash_flash_content(td_u32 flash_addr, td_u32 total_size, td_u8 *result, td_u32 result_size);
td_u32 uapi_crc32_flash_content(td_u32 crc_start, td_u32 flash_addr, td_u32 total_size, td_u32 *result);
td_u32 uapi_rsa_verify_flash_content(const upg_rsa_verify_param *param);

#ifdef __cplusplus
}
#endif
#endif /* __UPG_BASE_TOOL_H__ */
