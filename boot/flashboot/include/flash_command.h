/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Definitions for Command Processor
 */

#ifndef FLASH_COMMAND_H
#define FLASH_COMMAND_H
#include <command.h>

#define FLASH_STRUCT_SECTION __attribute__((unused, section(".u_flash_boot_cmd")))

#define u_flash_boot_cmd(name, maxargs, rep, cmd, usage) \
    cmd_tbl_t __u_flash_boot_cmd_##name FLASH_STRUCT_SECTION = { #name, maxargs, rep, cmd, usage }

extern cmd_tbl_t __u_flash_boot_cmd_start;
extern cmd_tbl_t __u_flash_boot_cmd_end;

#endif
