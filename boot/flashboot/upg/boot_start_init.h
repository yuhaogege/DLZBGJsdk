/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: boot init
 * Author: CompanyName
 * Create: 2018-10-31
 */

#ifndef __BOOT_START_INIT_H__
#define __BOOT_START_INIT_H__

#include "upg_common.h"

#include <soc_fnv.h>

td_u32 boot_upg_init_phase1(td_void);
td_u32 boot_upg_init_phash2(td_void);
td_u32 boot_upg_init_phash3(td_void);

#endif
