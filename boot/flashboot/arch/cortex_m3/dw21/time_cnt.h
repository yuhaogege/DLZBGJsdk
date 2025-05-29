/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: for time_cnt driver .
 */

#ifndef __TIME_CNT_H__
#define __TIME_CNT_H__

#include <soc_types.h>
#include <soc_boot_rom.h>

#define TIME_S (0x0)
#define TIME_MS (0x4)
#define TIME_US (0x8)
#define TIME_CNT_CTRL (0xC)
#define TIME_RD_EN (1 << 1)
#define TIME_CNT_EN (1 << 0)
#define TIME_RD_EN_WAIT_MAX (10000)

#define EXT_TIME_CNT_REG_WRITE(reg_off, value) UAPI_REG_WRITE((EXT_TIME_CNT_REG_BASE + (reg_off)), value)

#define EXT_TIME_CNT_REG_READ(reg_off, value) UAPI_REG_READ((EXT_TIME_CNT_REG_BASE + (reg_off)), value)
#endif
