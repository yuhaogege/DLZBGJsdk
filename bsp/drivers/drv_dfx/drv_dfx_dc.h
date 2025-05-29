/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: add data collect
 * Create: 2012-09-10
 */
#ifndef __DRV_DFX_DC_H__
#define __DRV_DFX_DC_H__
#include <soc_types.h>
#include <soc_mdm_types.h>
#if defined PRODUCT_CFG_NDM_COLLECT_DATA

td_u32 upload_data_config(td_u16 us_id, EXT_IN td_pvoid cmd_param, td_u16 cmd_param_size, td_u8 option);
td_u32 upload_data(td_u16 us_id, EXT_IN td_pvoid cmd_param, td_u16 cmd_param_size, td_u8 option);
td_u32 receive_data_config(td_u16 us_id, EXT_IN td_pvoid cmd_param, td_u16 cmd_param_size, td_u8 option);


#endif
#endif
