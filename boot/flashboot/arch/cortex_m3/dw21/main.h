/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: boot start main file
 */

#ifndef __MAIN_H__
#define __MAIN_H__

#include <cli.h>
#include <soc_flash_boot.h>
#include <secure/uart_auth.h>
#include <flash_command.h>
#include <soc_flashboot_flash.h>
#include <soc_fix_sf_cmd.h>
#include <malloc_rom.h>
#include <soc_fnv.h>
#include <soc_boot_partition_table.h>

extern unsigned int __HEAP_BEGIN__;
extern unsigned int __HEAP_END__;
extern td_u32 boot_start_main(td_void);

#endif
