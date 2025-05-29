/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Upgrade base funciton - nv mode.
 */

#ifndef __UPG_BASE_NV_MODE_H__
#define __UPG_BASE_NV_MODE_H__

#include <soc_types.h>

#ifdef __cplusplus
extern "C" {
#endif

td_u32 uapi_upg_nv_init_mode(td_void);
td_u32 uapi_upg_nv_set_val(td_u8 upg_mode, td_u32 flash_addr, td_u32 flash_size, td_u32 crc);

#ifdef __cplusplus
}
#endif
#endif /* __UPG_BASE_NV_MODE_H__ */
