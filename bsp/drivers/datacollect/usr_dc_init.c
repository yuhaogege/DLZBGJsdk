/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: add data collect
 * Create: 2012-09-10
 */


#include <soc_types.h>
#if defined PRODUCT_CFG_NDM_COLLECT_DATA
#include <soc_mdm_types.h>
#include <soc_mdm_datacollect.h>
#include <usr_dc_init.h>
td_u32 usr_dc_init(td_void)
{
    // initialize all forms of data collection drive. CNComments: 所有形态上初始化采数驱动
    return uapi_dc_init();
}
#endif
