#ifndef DW21_PLATFORM_H
#define DW21_PLATFORM_H

#define DW21_ASIC

#ifdef DW21_FPGA
#define EXT_XTAL_CLOCK_BASE     (25)
#define CONFIG_XTAL_CLOCK      (25000000)
#define CONFIG_CPU_CLOCK       (80000000)
#define CONFIG_APB_CLOCK       (40000000)
#define CONFIG_UART_150M_CLOCK (150000000)
#endif

#ifdef DW21_ASIC
#define EXT_XTAL_CLOCK_BASE     (25) /* LED blinking frequency */
#define CONFIG_XTAL_CLOCK      (25000000)
#define CONFIG_CPU_CLOCK       (200000000) /* Used for udelay */
#define CONFIG_APB_CLOCK       (100000000) /* Used by the watchdog */
#define CONFIG_UART_150M_CLOCK (150000000)
#endif

#define BOOTROM_SFC_REMAP_ADDR (0x0)
#define BOOTROM_ADDR           (0x00010000)
#define SFC_MEM_ADDR           (0x01000000)
#define SRAM_ADDR              (0x02000000)
#define SFC_REG_BASE           (0x40000000)
#define CIPHER_REG_BASE        (0x40010000)

/* System controller */
#define EXT_SYSCTRL_REG_BASE (0x40030000)
#define REG_SC_CTRL         (0x00)
#define REG_SC_SYSRES       (0x04)
#define REG_VERSION_ID      (0x24)
#define SYSCTRL_CFG         (0x3c)
#define SC_GEN_REG0         (0x40) /* General register 0, used to set software breakpoints for the boot */
#define REG_BOOT_VER        (0x44)
#define REG_RSA_KEY_ADDR    (0x48)
#define REG_FNV_ADDR        (0x4C) /* General register 3. The lower 24 bits are used for the factory area address. */
#define REG_FLASH_PROTECT   (0x4C) /* General register 3. The MSB indicates whether the boot supports 
                                      flash deprotection. */

#define MASK_FNV_ADDR         (0xFFFFFF)
#define SUPPORT_FLASH_PROTECT ((td_u32)1 << 31)

#define JTAG_SWE_ENABLE  (1 << 2)
#define UART0_SWE_ENABLE (1)
#define UART1_SWE_ENABLE (1 << 1)

#define EXT_WDG_REG_BASE    (0x40031000)
#define EXT_TIMER1_REG_BASE (0x40032000)
#define EXT_TIMER2_REG_BASE (0x40032014)
#define EXT_GPIO_REG_BASE   (0x40033000)
#define EXT_BUCK_REG_BASE   (0x40034000)
#define EXT_SSP_REG_BASE    (0x40035000)
#define EXT_GPIO_SEL_BASE   (0x40036000)
#define EXT_I2C_REG_BASE    (0x40037000)
/* CIPHER */
#define EXT_SPACC_REG_BASE (0x40038000)
#define EXT_RSA_REG_BASE   (0x40038c00)
#define EXT_EFUSE_REG_BASE (0x40038d00)
#define EXT_RNG_REG_BASE   (0x40038f00)

#define EXT_LED_REG_BASE      (0x40039000)
#define EXT_TIME_CNT_REG_BASE (0x4003A000)
#define EXT_CRG_REG_BASE      (0x4003B000)
#define SC_PERI_CLKSEL       (0x24)
#define SC_PERI_SRST         (0x28)
#define UART_CLK_SEL         (0x1 << 3)

#define EXT_UART0_REG_BASE (0x4003C000)
#define EXT_UART1_REG_BASE (0x4003D000)
#define EXT_RESV_REG_BASE  (0x4003E000)
#define EXT_IOCFG_REG_BASE (0x40036000)

#define GPIO_5_offset  (0x01C)
#define GPIO_6_offset  (0x020)
#define GPIO_11_offset (0x034)
#define GPIO_12_offset (0x038)
#define GPIO_19_offset (0x054)
#define GPIO_20_offset (0x058)

#endif /* DW21_PLATFORM_H */

