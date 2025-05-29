/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Upgrade base funciton - secure check.
 */

#ifndef __UPG_BASE_SECURE_CHECK_H__
#define __UPG_BASE_SECURE_CHECK_H__

#include <soc_types.h>
#include <soc_mdm_types.h>
#include <soc_21_upg.h>

#ifdef __cplusplus
extern "C" {
#endif

td_u32 upg_check_secure_head(td_u32 flash_addr, EXT_CONST ext_21_upg_head *upg_head);

#ifdef __cplusplus
}
#endif
#endif /* __UPG_BASE_SECURE_CHECK_H__ */
