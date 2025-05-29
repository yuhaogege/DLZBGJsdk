/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 */
#ifndef __DW21_PLATFORM_ROM_H__
#define __DW21_PLATFORM_ROM_H__

#include <soc_types_rom.h>
#include <crg_reg_offset.h>
#include <crg_reg_offset_field.h>
#include <sysctrl_reg_offset.h>
#include <sysctrl_reg_offset_field.h>
#include "dw21.h"

EXT_EXTERN td_u32 g_cfg_apb_clock;
EXT_EXTERN td_u32 g_cfg_arm_clock;
EXT_EXTERN td_u32 g_cfg_uart_clock;

#define EXT_XTAL_CLOCK_BASE (25) // Crystal oscillator clock, timer, and LED clock
#define EXT_XTAL_CLOCK (25000000)
#define CONFIG_TIMER_CLOCK EXT_XTAL_CLOCK

#ifdef TEST_ON_FPGA
#error
#define CONFIG_APB_CLOCK (40000000)
#define CONFIG_AHB_CLOCK (80000000)
#else
#define CONFIG_APB_CLOCK (100000000)
#define CONFIG_AHB_CLOCK (200000000)
#endif
#define CONFIG_UART_CLOCK (150000000)

#define EXT_SFC_MEM_BASE (0x01000000)
#define EXT_RAM_MEM_BASE (0x02000000)

/* ode/include/interface/rom/dw21_platform_rom.h: Base address */
#define EXT_SFC_REG_BASE (0x40000000)
#define EXT_SYSCTRL_REG_BASE (0x40030000)
#define EXT_WDG_REG_BASE (0x40031000)
#define EXT_TIMER_REG_BASE (0x40032000)
#define EXT_GPIO_REG_BASE (0x40033000)
#define EXT_BUCK_REG_BASE (0x40034000)
#define EXT_SSP_REG_BASE (0x40035000)
#define EXT_IOCFG_REG_BASE (0x40036000)
#define EXT_I2C_REG_BASE (0x40037000)
#define EXT_SPACC_REG_BASE (0x40038000)
#define EXT_RSA_REG_BASE (0x40038c00)
#define EXT_EFUSE_REG_BASE (0x40038d00)
#define EXT_RNG_REG_BASE (0x40038f00)
#define EXT_LED_REG_BASE (0x40039000)
#define EXT_TIMECNT_REG_BASE (0x4003A000)
#define EXT_CRG_REG_BASE (0x4003B000)
#define EXT_UART0_REG_BASE (0x4003C000)
#define EXT_UART1_REG_BASE (0x4003D000)
#define EXT_TSENSOR_REG_BASE (0x4003D100)
#define EXT_PWM_REG_BASE (0x4003D200)
#define EXT_OSC_REG_BASE (0x4003D300)
#define EXT_PWM1_REG_BASE (0x4003D400)
#define EXT_TIMECNT1_REG_BASE (0x4003D500)
#define EXT_SSP2_REG_BASE (0x4003E000)
#define EXT_HPM_REG_BASE (0x4003F000)
#define EXT_AFE_REG_BASE (0x40040000)
#define EXT_MMU_REG_BASE (0x40041000)

#define TIMER5_REG_BASE 0x40032064 /* System timer, CPUP get cycle, supporting the 32 kHz mode */
#define TIMER4_REG_BASE 0x40032050 /* Reserved for the user, supporting the 32 kHz mode */
#define TIMER3_REG_BASE 0x4003203c /* Reserved for the user */
#define TIMER2_REG_BASE 0x40032028 /* hw_delay */
#define TIMER1_REG_BASE 0x40032014 /* Reserved for the user */
#define TIMER0_REG_BASE 0x40032000 /* High-precision timer */

#define TIMER_LOAD 0x0
#define TIMER_VALUE 0x4
#define TIMER_CONTROL 0x8
#define TIMER_INT_CLR 0xc
#define TIMER_RIS 0x10

#define UAPI_REG_WRITE(addr, val) (*(volatile unsigned int *)(uintptr_t)(addr) = (val))
#define UAPI_REG_READ(addr, val) ((val) = *(volatile unsigned int *)(uintptr_t)(addr))

#endif /* __DW21_PLATFORM_ROM_H */
