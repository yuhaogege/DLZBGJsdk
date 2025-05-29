/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Project line  :  Platform And Key Technologies Development
 * Department    :  CAD Development Department
 * Version       :  1.0
 * Date          :  2017/10/29
 * Description   :  The description of xxx project
 * Others        :  Generated automatically by nManager V4.2
 * History       :  xxx 2018/05/19 17:23:34 Create file
 */

#ifndef __SYSCTRL_REG_OFFSET_H__
#define __SYSCTRL_REG_OFFSET_H__

/* SYSCTRL Base address of Module's Register */
#define DW21_SYSCTRL_BASE EXT_SYSCTRL_REG_BASE

/* DW21 SYSCTRL Registers' Definitions */
#define DW21_SYSCTRL_SC_CTRL_REG (DW21_SYSCTRL_BASE + 0x0)
/* SC_CTRL is the system control register. It is used to specify the operations to be performed by the system. */
#define DW21_SYSCTRL_SC_SYSRES_REG (DW21_SYSCTRL_BASE + 0x4)
/* SC_SYSRES is the system soft reset register.
 * When any value is written to this register, the system controller sends a system soft reset request to the reset
 * module. Then the reset module resets the system. */
/* SC_IMCTRL is the interrupt mode control register. */
#define DW21_SYSCTRL_SC_IMCTRL_REG (DW21_SYSCTRL_BASE + 0x8)
/* SC_IMSTAT is the interrupt mode status register. */
#define DW21_SYSCTRL_SC_IMSTAT_REG (DW21_SYSCTRL_BASE + 0xC)
/* SC_XTALCTRL is the crystal oscillator control register.
 * It is used to control the stable time after the clock module is initialized. The stable time is the wait time during
 * which the status is switched from XTAL CTL to SWtoXTAL.
 */
#define DW21_SYSCTRL_SC_XTALCTRL_REG (DW21_SYSCTRL_BASE + 0x10)
/* SC_PLLCTRL is the ARM PLL control register. It is used to set the wait time for the APLL to become stable.
 * The APLL clock frequency is controlled by SC_APLL_CTRL1 bit[20:0] and SC_APLL_CTRL0 bit[29:0].
 */
#define DW21_SYSCTRL_SC_PLLCTRL_REG (DW21_SYSCTRL_BASE + 0x14)
#define DW21_SYSCTRL_SC_AFE_CTRL_REG (DW21_SYSCTRL_BASE + 0x20)   /* SC_AFE_CTRL is the AFE control register. */
#define DW21_SYSCTRL_SC_VERSION_ID_REG (DW21_SYSCTRL_BASE + 0x24) /* SC_VERSION_ID is the chip version register. */
#define DW21_SYSCTRL_SC_HW_ID_REG (DW21_SYSCTRL_BASE + 0x28)      /* SC_HW_ID is the hardware version register. */
/* SC_PERI_CTRL is the peripheral control register. */
#define DW21_SYSCTRL_SC_PERI_CTRL_REG (DW21_SYSCTRL_BASE + 0x2C)
/* SC_LOCKEN is a write protection control register. */
#define DW21_SYSCTRL_SC_LOCKEN_REG (DW21_SYSCTRL_BASE + 0x38)
/* SC_PAD_CONFIG is the PAD configuration register. */
#define DW21_SYSCTRL_SC_PAD_CONFIG_REG (DW21_SYSCTRL_BASE + 0x3C)
#define DW21_SYSCTRL_SC_GEN_REG0_REG (DW21_SYSCTRL_BASE + 0x40)   /* SC_GEN_REG0 is general register 0. */
#define DW21_SYSCTRL_SC_GEN_REG1_REG (DW21_SYSCTRL_BASE + 0x44)   /* SC_GEN_REG1 is general register 1. */
#define DW21_SYSCTRL_SC_GEN_REG2_REG (DW21_SYSCTRL_BASE + 0x48)   /* SC_GEN_REG2 is general register 2. */
/* It is used by software to transfer the factory NV address. */
#define DW21_SYSCTRL_SC_GEN_REG3_REG (DW21_SYSCTRL_BASE + 0x4C)   /* SC_GEN_REG3 is general register 3. */
#define DW21_SYSCTRL_SC_GEN_REG4_REG (DW21_SYSCTRL_BASE + 0x50)   /* SC_GEN_REG4 is general register 4. */
#define DW21_SYSCTRL_SC_GEN_REG5_REG (DW21_SYSCTRL_BASE + 0x54)   /* SC_GEN_REG5 is general register 5. */
#define DW21_SYSCTRL_SC_GEN_REG6_REG (DW21_SYSCTRL_BASE + 0x58)   /* SC_GEN_REG6 is general register 6. */
#define DW21_SYSCTRL_SC_GEN_REG7_REG (DW21_SYSCTRL_BASE + 0x5C)   /* SC_GEN_REG7 is general register 7. */
#define DW21_SYSCTRL_SC_PERCLK_EN_REG (DW21_SYSCTRL_BASE + 0x820) /* Reserved control register 0 */
#define DW21_SYSCTRL_SC_PER_SRST_REG (DW21_SYSCTRL_BASE + 0x828)  /* Reserved control register 2 */
#define DW21_SYSCTRL_SC_CA_RST_REG (DW21_SYSCTRL_BASE + 0x82C)    /* Reserved control register 3 */
#define DW21_SYSCTRL_SC_SYSID0_REG (DW21_SYSCTRL_BASE + 0xEE0)    /* Internal test register */
#define DW21_SYSCTRL_SC_SYSID1_REG (DW21_SYSCTRL_BASE + 0xEE4)    /* Internal test register */
#define DW21_SYSCTRL_SC_SYSID2_REG (DW21_SYSCTRL_BASE + 0xEE8)    /* Internal test register */
#define DW21_SYSCTRL_SC_SYSID3_REG (DW21_SYSCTRL_BASE + 0xEEC)    /* Internal test register */
#define DW21_SYSCTRL_SCITCR_REG (DW21_SYSCTRL_BASE + 0xF00)       /* Internal test register */
#define DW21_SYSCTRL_SCITIR0_REG (DW21_SYSCTRL_BASE + 0xF04)      /* Internal test register */
#define DW21_SYSCTRL_SCITIR1_REG (DW21_SYSCTRL_BASE + 0xF08)      /* Internal test register */
#define DW21_SYSCTRL_SC_CNT_CTRL_REG (DW21_SYSCTRL_BASE + 0xF10)  /* Internal test register */
#define DW21_SYSCTRL_SC_CNT_DATA_REG (DW21_SYSCTRL_BASE + 0xF14)  /* Internal test register */
#define DW21_SYSCTRL_SC_CNT_STEP_REG (DW21_SYSCTRL_BASE + 0xF18)  /* Internal test register */
#define DW21_SYSCTRL_SC_PERIPHID0_REG (DW21_SYSCTRL_BASE + 0xFE0) /* Internal test register */
#define DW21_SYSCTRL_SC_PERIPHID1_REG (DW21_SYSCTRL_BASE + 0xFE4) /* Internal test register */
#define DW21_SYSCTRL_SC_PERIPHID2_REG (DW21_SYSCTRL_BASE + 0xFE8) /* Internal test register */
#define DW21_SYSCTRL_SC_PERIPHID3_REG (DW21_SYSCTRL_BASE + 0xFEC) /* Internal test register */
#define DW21_SYSCTRL_SC_PCELLID0_REG (DW21_SYSCTRL_BASE + 0xFF0)  /* Internal test register */
#define DW21_SYSCTRL_SC_PCELLID1_REG (DW21_SYSCTRL_BASE + 0xFF4)  /* Internal test register */
#define DW21_SYSCTRL_SC_PCELLID2_REG (DW21_SYSCTRL_BASE + 0xFF8)  /* Internal test register */
#define DW21_SYSCTRL_SC_PCELLID3_REG (DW21_SYSCTRL_BASE + 0xFFC)  /* Internal test register */

#endif // __SYSCTRL_REG_OFFSET_H__
