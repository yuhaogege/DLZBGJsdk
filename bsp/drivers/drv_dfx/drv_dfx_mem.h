/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Restore OSM/OSSMdrv_dfx_mem.h code
 * Author: CompanyName
 * Create: 2012-06-27
 */

#ifndef __DRV_DFX_MEM_H__
#define __DRV_DFX_MEM_H__
#include <soc_types.h>
#include <soc_mdm_types.h>

#ifdef PRODUCT_CFG_MEM_OPERATE
td_u32 mem_read(td_u16 id, EXT_IN td_pvoid cmd_param, td_u16 cmd_param_size, td_u8 option);
td_u32 mem_write(td_u16 id, EXT_IN td_pvoid cmd_param, td_u16 cmd_param_size, td_u8 option);
#endif

#endif
