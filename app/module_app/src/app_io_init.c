/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: I/O Initialize.
 */

#include <dw21_platform.h>
#include "soc_mdm_io.h"
#include "soc_mdm_led.h"
#include "soc_mdm_ver.h"
#ifdef __cplusplus
extern "C" {
#endif

#define IO_FUNC_MODE3 3

/* not allow to call the context of dynamic load zone */
__hot td_void app_io_init(td_void)
{
    td_u32 io_dir;
    td_u32 io_val;

    /* setting of LED */
#ifdef PRODUCT_CFG_DEBUG_JTAG_LED_MODULE_APP
    uapi_io_set_func(EXT_GPIO_IDX_6, 0);
    uapi_io_set_func(EXT_GPIO_IDX_1, 1);
    uapi_io_set_dir(EXT_GPIO_IDX_6, EXT_GPIO_DIRECTION_OUT);
    uapi_io_set_ouput_val(EXT_GPIO_IDX_6, EXT_GPIO_VALUE1);
    uapi_led_on(EXT_LED_IDX_4);
#endif
    uapi_io_set_func(EXT_GPIO_IDX_2, 1);
    uapi_io_set_func(EXT_GPIO_IDX_3, 1);
    uapi_led_init(EXT_LED_MODE_HIGH);
    uapi_led_on(EXT_LED_IDX_0);
    uapi_led_on(EXT_LED_IDX_1);

    /* set UART0 multiplexing  relation */
    uapi_io_set_func(EXT_GPIO_IDX_9, 1);              /* GPIO_9_SEL:  UART0_RXD */
    uapi_io_set_func(EXT_GPIO_IDX_10, IO_FUNC_MODE3); /* GPIO_10_SEL: UART0_N_TXD */

    if (uapi_get_hw_chip_type() == EXT_FTM_CHIP_TYPE_21EV) {
        /* set PWM multiplexing relation */
        uapi_io_set_func(EXT_GPIO_IDX_5, IO_FUNC_MODE3);

        io_val = 0x6600;
        io_dir = 0x587ffe;
        uapi_io_init(&io_val, &io_dir);

        /* set drive capability of pin */
        uapi_io_set_driver_strength(EXT_IO_CTL_SFC_CLK, EXT_IO_DRIVER_STRENGTH_2);
        uapi_io_set_driver_strength(EXT_IO_CTL_SFC_WPN, EXT_IO_DRIVER_STRENGTH_2);
        uapi_io_set_driver_strength(EXT_IO_CTL_SFC_HOLDN, EXT_IO_DRIVER_STRENGTH_2);
        uapi_io_set_driver_strength(EXT_IO_CTL_SFC_DI, EXT_IO_DRIVER_STRENGTH_2);
        uapi_io_set_driver_strength(EXT_IO_CTL_SFC_DO, EXT_IO_DRIVER_STRENGTH_2);
    } else if (uapi_get_hw_chip_type() == EXT_FTM_CHIP_TYPE_21SV) {
        /* set PWM multiplexing relation */
        uapi_io_set_func(EXT_GPIO_IDX_0, IO_FUNC_MODE3);
        uapi_io_set_func(EXT_GPIO_IDX_5, IO_FUNC_MODE3);

        io_val = 0x6600;
        io_dir = 0x3fee3f;
        uapi_io_init(&io_val, &io_dir);
    } else {
        uapi_io_init(TD_NULL, TD_NULL);
    }
}

#ifdef __cplusplus
}
#endif

