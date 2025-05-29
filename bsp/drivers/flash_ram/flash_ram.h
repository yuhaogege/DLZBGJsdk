/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: FLASH drive adjustment. Flash_ram.c code
 * Author: CompanyName
 * Create: 2012-06-08
 */
#ifndef __FLASH_RAM_H__
#define __FLASH_RAM_H__
#include "soc_types.h"
td_u32 uapi_flash_fast_read(td_u32 flash_addr, td_u32 flash_read_size, td_u8 *flash_read_data);

#endif
