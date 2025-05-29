/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Upgrade base funciton - start.
 * Author: CompanyName
 * Create: 2018-10-31
 */

#ifndef __UPG_BASE_START_H__
#define __UPG_BASE_START_H__

#include <soc_types.h>
#include <soc_mdm_types.h>
#include <soc_21_upg.h>

#ifdef __cplusplus
extern "C" {
#endif

td_u32 uapi_upg_real_start_prv(td_u32 tail_flash_addr);
td_u32 uapi_upg_write_magic(td_u32 upg_head_address, td_u32 upg_tail_address);
td_u32 uapi_upg_get_boot_status(td_void);
#ifdef __cplusplus
}
#endif
#endif /* __UPG_BASE_START_H__ */
