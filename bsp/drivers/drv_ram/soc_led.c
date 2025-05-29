/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: soc led code
 */
#include <soc_types_rom.h>
#include <soc_mdm_led.h>
#include <dw21_platform_rom.h>

// reg_offset
#define EXT_LED_PH_CTRL  (0x00)
#define EXT_LED_PH_START (0x64)

__isr td_void uapi_led_init(ext_led_mode_e led_mode)
{
    /* LED low or high effect */
    td_u32 dir_val = 0;

    UAPI_REG_READ32((EXT_LED_REG_BASE + EXT_LED_PH_CTRL), dir_val);
    if (EXT_LED_MODE_HIGH == led_mode) {
        // led1-5,switch1-3,total:8bit
        dir_val &= (~(td_u32)(0x1f << 16)); /* bit 16  polar ind */
    } else {
        /*
                    【代码分析】二次开发接口，客户有可能使用该分支，需保留。
                    【功能影响】不影响正常业务，屏蔽告警
 */
        dir_val |= (0x1f << 16); /* bit 16  polar ind */
    }
    UAPI_REG_WRITE32((EXT_LED_REG_BASE + EXT_LED_PH_CTRL), dir_val);
    UAPI_REG_WRITE32((EXT_LED_REG_BASE + EXT_LED_PH_START), 0x01);
}