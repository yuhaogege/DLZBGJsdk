/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: flash boot
 */
#ifndef SOC_FLASH_BOOT_H
#define SOC_FLASH_BOOT_H

/* __ASM__ signify the situation of calling assembly file */
#ifndef __ASM__
#include <soc_types.h>
#include <soc_boot_rom.h>
#include <soc_product_config.h>
#endif

/* RAM distribution in boot
1.total size is 256K
using for loading binary from flash to RAM. start-up after jumping */
#define FIRMWARE_LOAD_ADDR (0x02000000) /* SRAM ADDR */
#define FIRMWARE_MAX_SIZE (0x19000)     /* loading binary size from flash, 100KB */
#define FIRMWARE_FLASH_OFFSET (0x7000)  /* by default,  firmware  FLASH address: 16 KB */
#define BURN_FLASH_OFFSET (0x6000)      /* by default,  firmware  FLASH address: 16 KB */
#define FACTORY_NV_ADDR (0x4000)        /* factory nv address, by default,  firmware  FLASH address: 16 KB */

#define BOOTLOADER_LOAD_ADDR (FIRMWARE_LOAD_ADDR) /* by default, loady download address */

#define BOOTLOADER_FLASH_HEAD_ADDR (0x01000000)

#ifdef START_FROM_FLASH
#define BOOT_FROM_FLASH 1 /* flash start-up immediately,  not start-up bootrom switch */
#else
#define BOOT_FROM_FLASH 0 /* flash start-up immediately,  not start-up bootrom switch */
#endif

#define CHAR_CTRL_C (0x03)        /* ^C,using Ctrl+C for interrupt serial.  encapsulate before using */
#define CFG_PROMPT "interrupt $ " /* Monitor Command Prompt */

#define UART_FLASHBOOT_INTERRUPT_TIMEOUT                                \
    (40) /* timeout time for first byte of flashboot interrupted frame. \
          * by default, equal to 40ms */

#endif
