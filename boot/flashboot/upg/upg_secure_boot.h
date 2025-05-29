/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: boot security
 * Author: CompanyName
 * Create: 2018-10-31
 */

#ifndef __UPG_SECURE_BOOT_H__
#define __UPG_SECURE_BOOT_H__

#include "boot_header.h"

#include <lzmaram.h>
#include <soc_types.h>

typedef struct {
    td_u8 *code_section;
    td_u32 section_length;
} bootloader_section_and_len;
td_u32 flash_boot_security_boot_verify(td_pvoid flash_boot_addr);

#endif
