/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Add io driver capability interface, and modify the app layer to
 * configure the io_config.c code for flash-related io drivers.
 * Author: CompanyName
 * Create: 2012-11-19
 */
#include "io_config.h"
#include "dw21_platform_rom.h"
#include "soc_mdm_io.h"
#include "soc_mdm_isr.h"
#include "soc_errno.h"

#define IO_DRV_OFFSET_MIN 0x800
#define IO_DRV_OFFSET_MAX 0x948
#define IO_DRV_MASK_BITS 0x3
#define IO_DRV_MOV_BITS 0x4
#define IO_PU_MASK_BITS 0x1
#define IO_PU_MOV_BITS 0x8

EXT_PRV td_u32 reg_val_process(ext_io_ctl_idx_e id, td_u32 val, td_u32 mask_bits, td_u32 mov_bits)
{
    td_u32 reg_val = 0;
    td_u32 int_val;

    if ((id < IO_DRV_OFFSET_MIN) || (id > IO_DRV_OFFSET_MAX)) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    int_val = uapi_int_lock();
    UAPI_REG_READ32(((td_u32)EXT_IOCFG_REG_BASE + id), reg_val);
    reg_val &= ~((td_u32)mask_bits << mov_bits);
    reg_val |= ((td_u32)val & (td_u32)mask_bits) << mov_bits;
    UAPI_REG_WRITE32(((td_u32)EXT_IOCFG_REG_BASE + id), reg_val);
    uapi_int_restore(int_val);

    return EXT_ERR_SUCCESS;
}

td_u32 uapi_io_set_driver_strength(ext_io_ctl_idx_e id, ext_io_driver_strength_e level)
{
    td_u32 ret;

    if (level >= EXT_IO_DRIVER_STRENGTH_MAX) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    ret = reg_val_process(id, level, IO_DRV_MASK_BITS, IO_DRV_MOV_BITS);
    if (ret == EXT_ERR_INVALID_PARAMETER) {
        return EXT_ERR_INVALID_PARAMETER;
    } else {
        return EXT_ERR_SUCCESS;
    }
}

td_u32 uapi_io_set_pull(ext_io_ctl_idx_e id, ext_io_pull_e pu)
{
    td_u32 ret;
    if (pu >= EXT_GPIO_PU_MAX) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    ret = reg_val_process(id, pu, IO_PU_MASK_BITS, IO_PU_MOV_BITS);
    if (ret == EXT_ERR_INVALID_PARAMETER) {
        return EXT_ERR_INVALID_PARAMETER;
    } else {
        return EXT_ERR_SUCCESS;
    }
}
