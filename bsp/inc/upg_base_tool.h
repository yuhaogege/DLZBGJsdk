/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Synchronous bsp/driversbsp/incbsp/system/inc directory upg_base_tool.h code
 * Author: CompanyName
 * Create: 2012-11-11
 */

#ifndef __UPG_BASE_TOOL_H__
#define __UPG_BASE_TOOL_H__
#include <soc_types.h>
#include <soc_mdm_types.h>
#include <soc_mdm_cipher.h>
#define HASH_RESULT_SIZE 32
#define KEY_E_SIZE 256
#define KEY_N_SIZE 256
#define SIGNATURE_SIZE 256
td_u32 uapi_upg_get_cur_kernel_size(td_void);
td_u32 uapi_hash_flash_content(td_u32 flash_addr, td_u32 total_size, td_u8 hash_result[HASH_RESULT_SIZE],
    td_u32 hash_result_size);
td_u32 uapi_crc32_flash_content(td_u32 crc_start, td_u32 flash_addr, td_u32 total_size, td_u32 *crc_result);

#endif /* __UPG_BASE_TOOL_H__ */
