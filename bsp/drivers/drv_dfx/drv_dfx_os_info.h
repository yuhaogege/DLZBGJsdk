/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Modify the drv directory, remove the first stage of the romization error,
 * delete the useless code to compile and adjust the dfx directory, remove the count
 * function driver part drv_dfx_os_info.h code
 * Author: CompanyName
 * Create: 2012-04-04
 */

#ifndef __DRV_DFX_OS_INFO_H__
#define __DRV_DFX_OS_INFO_H__
#include <soc_types.h>
#include <soc_mdm_types.h>

extern td_u32 __reset_handler_size__;
extern td_u32 __rom_data_size__;
extern td_u32 __rom_bss_size__;
extern td_u32 __data_size__;
extern td_u32 __bss_size__;
extern td_u32 __SYSTEM_STACK_SIZE__;
extern td_u32 __HEAP_SIZE__;
extern td_u32 __dyn_size__;
extern td_u32 __permanent_size__;

td_u32 mem_info_show(td_u16 id, EXT_IN td_pvoid cmd_param, td_u16 cmd_param_size, td_u8 option);
td_u32 os_resource_show(td_u16 id, EXT_IN td_pvoid cmd_param, td_u16 cmd_param_size, td_u8 option);
td_u32 os_task_show(td_u16 id, EXT_IN td_pvoid cmd_param, td_u16 cmd_param_size, td_u8 option);

#endif
