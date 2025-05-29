/*
 * Copyright (c) CompanyNameMagicTag 2019-2019. All rights reserved.
 * Description: Usr DC initialization.
 * Author: CompanyName
 * Create: 2019-07-31
 */

#ifndef USR_DC_INIT
#define USR_DC_INIT
#if defined PRODUCT_CFG_NDM_COLLECT_DATA
#include <soc_types.h>

EXT_EXTERN td_u32 usr_dc_init(td_void);
#endif
#endif