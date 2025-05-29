/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: boot check
 */

#ifndef __BOOT_START_CHECK_H__
#define __BOOT_START_CHECK_H__

#include "upg_common.h"

#include <soc_fnv.h>
#include "upg_tool.h"
#include "boot_check_secure_info.h"

td_u32 boot_start_check_read_tbl_info(td_void);
td_u32 boot_start_check_kernel(td_void);
td_u32 boot_start_check_nvfile(td_void);

#endif
