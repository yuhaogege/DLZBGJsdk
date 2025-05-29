/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Middle layer - NV.
 * Author: CompanyName
 * Create: 2018-10-31
 */

#ifndef __NV_RW_H__
#define __NV_RW_H__

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
#include "nv_rw_cco.h"
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA) || defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
#include "nv_rw_sta.h"
#endif

#endif /* __NV_RW_H__ */
