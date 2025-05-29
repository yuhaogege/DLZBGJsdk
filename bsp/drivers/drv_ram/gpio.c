/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: gpio drive adjustment. gpio_ram.c code
 * Author: CompanyName
 * Create: 2012-06-08
 */
#include "dw21_platform_rom.h"
#include "gpio.h"


#define GPIO_SWPORT_DR     0x00
#define GPIO_SWPORT_DDR    0x04

/* 因在动态加载前，不能放在动态加载区域 */
__isr td_u32 uapi_io_init(EXT_CONST td_u32 *val, EXT_CONST td_u32 *dir)
{
    td_u32 ulRegVal = 0;

    if (TD_NULL != val) {
        ulRegVal = *val;
        UAPI_REG_WRITE32((EXT_GPIO_REG_BASE + GPIO_SWPORT_DR), ulRegVal);
    }
    if (TD_NULL != dir) {
        ulRegVal = *dir;
        UAPI_REG_WRITE32((EXT_GPIO_REG_BASE + GPIO_SWPORT_DDR), ulRegVal);
    }

    return EXT_ROM_ERR_SUCCESS;
}