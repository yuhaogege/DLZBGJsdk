/************************************************************************
*             Copyright (CompanyNameMagicTag) CompanyNameMagicTag
*                           ALL RIGHTS RESERVED
* FileName: cmd_upload.c
* Description:
*
*************************************************************************/

#include <soc_types.h>
#include <soc_boot_rom.h>
#include <soc_boot_basic.h>
#include <dw21_platform.h>

/* 获取芯片类型 */
__rom_addition td_u32 uapi_get_hw_chip_type(td_void)
{
    td_u32 asic_ver = 0;

    reg_read32(EXT_SYSCTRL_REG_BASE + REG_VERSION_ID, asic_ver);

    if (EXT_GET_BIT_I(asic_ver, 1)) {
        return EXT_CHIP_TYPE_21SV;
    } else {
        return EXT_CHIP_TYPE_21EV;
    }
}

