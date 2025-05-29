#ifndef __SOC_BOOT_BASIC_H__
#define __SOC_BOOT_BASIC_H__
#include <soc_types.h>
#include <soc_boot_err.h>
#include <soc_boot_rom.h>
#include <dw21_platform.h>

#define EXT_I_AM_IN_BOOT

#define BSP_ROM_RODATA_SECTION
#define EXT_SFC_REG_BASE (0x40000000)

#define reg_write32(addr, val) (*(volatile unsigned int *)(addr) = (val))
#define reg_read32(addr, val)  ((val) = *(volatile unsigned int *)(addr))
#define reg_read_val(addr)     (*(volatile unsigned *)(addr))

#define DW21_CRG_REG_BASE               EXT_CRG_REG_BASE
#define DW21_CRG_REG_SC_PERI_SRST_REG   (DW21_CRG_REG_BASE + 0x28) /* SC_PERI_SRST is a peripheral soft reset 
                                                                          request register. */
#define CRG_REG_SFC_SRST_REQ_OFFSET       2
#define DW21_CRG_REG_SC_PERI_CLKEN0_REG (DW21_CRG_REG_BASE + 0x14) /* SC_PERI_CLKEN is peripheral clock enable 
                                                                          register 0. */
#define CRG_REG_SFC_CLK_EN_OFFSET         16

#endif

