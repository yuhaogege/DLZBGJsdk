/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: upg temp
 * Author: CompanyName
 * Create: 2018-10-31
 */

#include "upg_temp.h"

#include <soc_boot_rom.h>
#include <soc_flashboot_flash.h>
#include <soc_flash_boot.h>

td_void boot_firmware_new(td_u32 flash_offset)
{
    unsigned int ret;
    boot_msg1("boot kernel at==================", flash_offset);
    /* FIRMWARE_FLASH_OFFSET */
    ret = uapi_flash_read(flash_offset, FIRMWARE_MAX_SIZE, (void *)FIRMWARE_LOAD_ADDR);
    if (ret != 0) {
        boot_msg0("SPI flash read full kernel FLASH failed");
    }

    boot_kernel(FIRMWARE_LOAD_ADDR);
}

void boot_reset(void)
{
    do_reset(0, 0, 0, 0);
}
