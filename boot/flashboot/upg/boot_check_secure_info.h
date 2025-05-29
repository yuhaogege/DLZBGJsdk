/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: boot security
 * Author: CompanyName
 * Create: 2018-10-31
 */

#ifndef __BOOT_CHECK_SECURE_INFO_H__
#define __BOOT_CHECK_SECURE_INFO_H__

#include "upg_secure_boot.h"

#include <soc_fnv.h>
#include "upg_common.h"
#include "upg_tool.h"

extern unsigned int __data_begin__;
extern unsigned int __data_flash_begin__;

#if defined PRODUCT_CFG_SECURE_UPG
td_u32 boot_upg_check_secure_head(td_void);
td_u32 boot_upg_check_secure_boot(td_void);
td_u32 boot_start_check_tbl_secure_info(td_void);
td_void uapi_secure_save_key_pos(td_void);
#else
EXT_PRVL td_u32 boot_upg_check_secure_head(td_void)
{
    return EXT_ERR_SUCCESS;
}
EXT_PRVL td_u32 boot_upg_check_secure_boot(td_void)
{
    return EXT_ERR_SUCCESS;
}
EXT_PRVL td_u32 boot_start_check_tbl_secure_info(td_void)
{
    return EXT_ERR_SUCCESS;
}
EXT_PRVL td_void uapi_secure_save_key_pos(td_void)
{
    return;
}
#endif

#endif
