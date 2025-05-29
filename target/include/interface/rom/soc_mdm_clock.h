/**
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: hct_clock Clock configuration.
 */

#ifndef __SOC_MDM_CLOCK_H__
#define __SOC_MDM_CLOCK_H__
#include <soc_types_rom.h>
#include <soc_mdm_types_rom.h>

/**
 * @ingroup  hct_clock
 * @brief Enables the clock.
 *
 * @par Description:
 * Enables the clock source of the module.
 *
 * @attention None
 * @param  reg  [IN] Type #td_u32 Address of the clock register
 * @param  bits [IN] Type #td_u32 Clock bit, for example, 0x1<<2 for bit[2]
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_clock.h: Describes APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_void uapi_clock_enable(td_u32 reg, td_u32 bits);

/**
 * @ingroup  hct_clock
 * @brief Disables the clock.
 *
 * @par Description:
 * Disables the clock source of a module.
 *
 * @attention None
 * @param  reg  [IN] Type #td_u32 Address of the clock register
 * @param  bits [IN] Type #td_u32 Clock bit, for example, 0x1<<2 for bit[2]
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_clock.h: Describes APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_void uapi_clock_disable(td_u32 reg, td_u32 bits);

/**
 * @ingroup hct_clock
 *
 * UART clock source enumeration
 */
typedef enum {
    UART_CLK_XTAL, /**< External crystal */
    UART_CLK_APB   /**< APB clock */
} ext_uart_clk_source;

/**
 * @ingroup  hct_clock
 * @brief Sets the UART clock source.
 *
 * @par Description:
 * Sets the UART clock source.
 *
 * @attention None
 * @param  clk_source_type [IN] Type #ext_uart_clk_source UART clock source
 *
 * @retval #0              Success
 * @retval #Other values            Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_clock.h: Describes APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_void uapi_uart_set_clock(ext_uart_clk_source clk_source_type);

#endif
