/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * File name     :  crg_reg_reg_offset.h
 * Project line  :  Platform And Key Technologies Development
 * Department    :  CAD Development Department
 * Version       :  1.0
 * Date          :  2013/3/10
 * Description   :  The description of xxx project
 * Others        :  Generated automatically by nManager V4.2
 * History       :  xxx 2018/05/19 17:32:50 Create file
 */

#ifndef __CRG_REG_OFFSET_H__
#define __CRG_REG_OFFSET_H__

/* CRG_REG Base address of Module's Register */
#define DW21_CRG_REG_BASE EXT_CRG_REG_BASE

/***************************************************************************** */
/* xxx CRG_REG Registers' Definitions */
/***************************************************************************** */
#define DW21_CRG_REG_SC_PLLLOCK_STAT_REG (DW21_CRG_REG_BASE + 0x0)
/* SC_PLLLOCK_STAT is the PLL lock status indicator register. */
#define DW21_CRG_REG_SC_APLL_CTRL0_REG (DW21_CRG_REG_BASE + 0x4)
/* SC_APLL_CTRL0 is ARM PLL control register 0. */
#define DW21_CRG_REG_SC_APLL_CTRL1_REG (DW21_CRG_REG_BASE + 0x8)
/* SC_APLL_CTRL1 is ARM PLL control register 1. */
#define DW21_CRG_REG_SC_PERI_CLKEN0_REG (DW21_CRG_REG_BASE + 0x14)
/* SC_PERI_CLKEN is peripheral clock enable register 0. */
#define DW21_CRG_REG_SC_PERI_CLKEN1_REG (DW21_CRG_REG_BASE + 0x18)
/* SC_PERI_CLKEN is peripheral clock enable register 1. */
#define DW21_CRG_REG_SC_PERI_CLKSTAT0_REG (DW21_CRG_REG_BASE + 0x1C)
/* SC_PERI_CLKSTAT is peripheral clock status indicator register 0. */
#define DW21_CRG_REG_SC_PERI_CLKSTAT1_REG (DW21_CRG_REG_BASE + 0x20)
/* SC_PERI_CLKSTAT is peripheral clock status indicator register 1. */
#define DW21_CRG_REG_SC_PERI_CLKSEL_REG (DW21_CRG_REG_BASE + 0x24)
/* SC_PERI_CLKSEL is the peripheral clock source select register. */
#define DW21_CRG_REG_SC_PERI_SRST_REG (DW21_CRG_REG_BASE + 0x28)
/* SC_PERI_SRST is the peripheral soft reset request register. */
#define DW21_CRG_REG_SC_SLEEP0_CTRL_REG (DW21_CRG_REG_BASE + 0x2C)
/* SC_SLEEP0_CTRL is the low-power clock sleep enable register for each module. */
#define DW21_CRG_REG_SC_PHY_CLK_EN_REG (DW21_CRG_REG_BASE + 0x30)
/* SC_PHY_CLK_EN is the PHY clock gating software control register. */
#define DW21_CRG_REG_SC_PHY_SRST_REG (DW21_CRG_REG_BASE + 0x34)
/* SC_PHY_SRST is the PHY soft reset control register. */
#define DW21_CRG_REG_SC_PHY_RST_MASK_REG (DW21_CRG_REG_BASE + 0x38)
/* SC_PHY_RST_MASK is the PHY reset mask register. */
#define DW21_CRG_REG_MEM_TOP_GATE_EN_REG (DW21_CRG_REG_BASE + 0x3C)
/* MEM_TOP clock gating register */
#define DW21_CRG_REG_BOOT_ROM_GATE_EN_REG (DW21_CRG_REG_BASE + 0x40)
/* Bootrom clock gating register */
#define DW21_CRG_REG_SC_PHY_RST1_MASK_REG (DW21_CRG_REG_BASE + 0x44)
/* SC_PHY_RST_MASK is PHY reset mask register 1. */
#define DW21_CRG_REG_SC_SLEEP1_CTRL_REG (DW21_CRG_REG_BASE + 0x48)
/* SC_SLEEP1_CTRL is the low-power clock sleep enable register for each module. */
#define DW21_CRG_REG_SC_DEEP_SLEEP0_CTRL_REG (DW21_CRG_REG_BASE + 0x4C)
/* SC_DEEP_SLEEP0_CTRL is the low-power clock sleep enable control register for each module. */
#define DW21_CRG_REG_SC_DEEP_SLEEP1_CTRL_REG (DW21_CRG_REG_BASE + 0x50)
/* SC_DEEP_SLEEP1_CTRL is the low-power clock sleep enable register for each module. */
#define DW21_CRG_REG_SC_PERI_SRST1_REG (DW21_CRG_REG_BASE + 0x54)
/* SC_PERI_SRST1 is the peripheral soft reset request register. */
#define DW21_CRG_REG_SC_PHY1_SRST1_REG (DW21_CRG_REG_BASE + 0x58)
/* SC_PHY_SRST1 is the PHY soft reset control register. */
#define DW21_CRG_REG_XTAL_IO_CTRL_REG (DW21_CRG_REG_BASE + 0x5C)
/* Crystal I/O configuration register */
#endif // __CRG_REG_REG_OFFSET_H__
