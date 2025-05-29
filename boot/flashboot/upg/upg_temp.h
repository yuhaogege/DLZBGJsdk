/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: upg common head file
 * Author: CompanyName
 * Create: 2018-10-31
 */

#ifndef __UPG_TEMP_H__
#define __UPG_TEMP_H__

#include <soc_types.h>

extern void boot_kernel(uintptr_t kaddr);
extern int do_reset(int, int, int, int);

td_void boot_firmware_new(td_u32 flash_offset);
void boot_reset(void);

#endif
