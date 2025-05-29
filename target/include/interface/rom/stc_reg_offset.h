/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Project line  :  Platform And Key Technologies Development
 * Department    :  CAD Development Department
 * Version       :  1.0
 * Date          :  2017/10/29
 * Description   :  The description of xxx project
 * Others        :  Generated automatically by nManager V4.2
 * History       :  xxx 2018/05/18 10:43:51 Create file
 */

#ifndef __STC_REG_OFFSET_H__
#define __STC_REG_OFFSET_H__

/* STC Base address of Module's Register */
#define dw21v100_STC_BASE (0x40020e00)

/***************************************************************************** */
/* dw21v100 STC Registers' Definitions */
/***************************************************************************** */
#define dw21v100_STC_STC_CTRL_REG_REG (dw21v100_STC_BASE + 0x80)
/* STC_CTRL_REG is the data capture control register. */
#define dw21v100_STC_STC_RX_STATUS_REG_REG (dw21v100_STC_BASE + 0x84)
/* STC_RX_STATUS_REG is the data capture completion status register for the RX end. */
#define dw21v100_STC_STC_RX_START_ADDR_REG_REG (dw21v100_STC_BASE + 0x88)
/* STC_RX_START_ADDR_REG is the SDRAM start address configuration register for the RX end. */
#define dw21v100_STC_STC_RX_END_ADDR_REG_REG (dw21v100_STC_BASE + 0x8C)
/* STC_RX_END_ADDR_REG is the SDRAM end address configuration register for the RX end. */
#define dw21v100_STC_STC_RX_CURR_ADDR_REG_REG (dw21v100_STC_BASE + 0x90)
/* STC_RX_CURR_ADDR_REG is the SDRAM address writing register for the RX end. */
#define dw21v100_STC_STC_TX_STATUS_REG_REG (dw21v100_STC_BASE + 0x94)
/* STC_TX_STATUS_REG is the data capture completion status register for the TX end. */
#define dw21v100_STC_STC_TX_START_ADDR_REG_REG (dw21v100_STC_BASE + 0x98)
/* STC_TX_START_ADDR_REG is the SDRAM start address configuration register for the TX end. */
#define dw21v100_STC_STC_TX_END_ADDR_REG_REG (dw21v100_STC_BASE + 0x9C)
/* STC_TX_END_ADDR_REG is the SDRAM end address configuration register for the TX end. */
#define dw21v100_STC_STC_TX_CURR_ADDR_REG_REG (dw21v100_STC_BASE + 0xA0)
/* STC_TX_CURR_ADDR_REG is the SDRAM address reading register for the TX end. */
#define dw21v100_STC_STC_DELAY_COUNT_REG_REG (dw21v100_STC_BASE + 0xAC)
/* STC_DELAY_COUNT_REG is the delay register before data capture stops in collection mode. */
#define dw21v100_STC_STC_CAP_CTRL_REG_REG (dw21v100_STC_BASE + 0xB0)
/* STC_CAP_CTRL_REG is the capture mode control register. */
#define dw21v100_STC_STC_TRIG_MASK_REG_REG (dw21v100_STC_BASE + 0xB4)
/* STC_TRIG_MASK_REG is the mask data bit register for triggering the data capture mode. */
#define dw21v100_STC_STC_TRIG_DATA_REG_REG (dw21v100_STC_BASE + 0xB8)
/* STC_TRIG_MASK_REG is the mask data register for triggering the data capture mode. */
#define dw21v100_STC_STC_TRIG_ADDR_REG (dw21v100_STC_BASE + 0xBC)
/* STC_TRIG_ADDR is the SDRAM write address register corresponding to the trigger signal for the RX end. */
#define dw21v100_STC_PKT_GAP_THRES_REG (dw21v100_STC_BASE + 0xC0)
/* PKT_GAP_THRES is the packet interval threshold configuration register. */
#define dw21v100_STC_HADDRM_REG (dw21v100_STC_BASE + 0xC4)
/* HADDRM is the address register corresponding to the next DDR write. */
#define dw21v100_STC_DFX_REG_CTRL_REG (dw21v100_STC_BASE + 0xD0)
/* DFX_REG_CTRL is the DFX data capture control register. */
#define dw21v100_STC_DFX_REG_CAP_CTRL_REG (dw21v100_STC_BASE + 0xD4)
/* DFX_REG_CAP_CTRL is the DFX data capture configuration register. */
#define dw21v100_STC_DFX_REG_DELAY_COUNT_REG (dw21v100_STC_BASE + 0xD8)
/* DFX_REG_DELAY_COUNT is the delay count configuration register in the DFX trigger mode. */
#define dw21v100_STC_DFX_REG_PREAM_PAR_REG (dw21v100_STC_BASE + 0xDC)
/* DFX_REG_PREAM_PAR is the latch peak-to-average ratio register in DFX trigger mode. */
#endif // __STC_REG_OFFSET_H__
