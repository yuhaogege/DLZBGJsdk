/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: for watchdog driver .
 */

#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

#include <dw21_platform.h>

/* WDG reg offset */
#define WDT_CR (0x00)
#define WDT_TORR (0x04)
#define WDT_CRR (0x0c)

#define SYSCTRL_BASE_ADDR EXT_SYSCTRL_REG_BASE
#define SC_CTRL REG_SC_CTRL

#define WDG_CLK_U_S (CONFIG_APB_CLOCK / 1000000)
#define WDG_BIT_0_IS_1 (0x1)
#endif
