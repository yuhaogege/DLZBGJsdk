/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: upg from nv
 * Author: CompanyName
 * Create: 2018-10-31
 */

#ifndef __UPG_FTM_NV_H__
#define __UPG_FTM_NV_H__

#include <soc_boot_err.h>
#include <soc_fnv.h>
#define EXT_UPG_NV_MAGIC 0x31564E55
td_bool boot_upg_is_do_upg_from_nv(td_void);

#endif
