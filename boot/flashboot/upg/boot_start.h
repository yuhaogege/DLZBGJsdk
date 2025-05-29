/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: boot start
 * Author: CompanyName
 * Create: 2018-10-31
 */

#ifndef __BOOT_START_H__
#define __BOOT_START_H__

#include "upg_common.h"

#include <soc_fnv.h>
#include <soc_boot_partition_table.h>
#include "upg_ftm_nv.h"
#include "boot_start_check.h"
#include "boot_start_init.h"
#include "boot_upg_check.h"
#include "upg_boot.h"
#include "upg_kernel.h"
#include "upg_temp.h"
#include "boot_check_secure_info.h"

td_u32 boot_start_main(td_void);

#endif
