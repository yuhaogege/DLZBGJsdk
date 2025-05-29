/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: upg check
 * Author: CompanyName
 * Create: 2018-10-31
 */

#ifndef __BOOT_UPG_CHECK_H__
#define __BOOT_UPG_CHECK_H__

#include "upg_common.h"

#include <soc_fnv.h>
#include "upg_tool.h"
#include "boot_check_secure_info.h"

#define EXT_UPG_IN_BOOT_CHECK_PARTITION
td_u32 boot_upg_clear_upg_magic_one(td_void);
td_u32 boot_upg_clear_upg_magic_all(td_void);
td_u32 boot_upg_check_upg_phase0(td_void);
td_u32 boot_upg_check_upg_phase1(td_void);
td_u32 boot_upg_check_upg_phase2(td_void);

#endif
