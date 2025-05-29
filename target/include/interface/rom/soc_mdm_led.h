/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * @defgroup hct_led  LED
 * @ingroup drivers
 */

#ifndef __SOC_MDM_LED_H__
#define __SOC_MDM_LED_H__

#include <soc_types_rom.h>

/**
 * @ingroup hct_led
 *
 * LED index
 */
typedef enum {
    EXT_LED_IDX_0,   /**< LED1 */
    EXT_LED_IDX_1,   /**< LED2 */
    EXT_LED_IDX_2,   /**< LED3 */
    EXT_LED_IDX_3,   /**< LED4 */
    EXT_LED_IDX_4,   /**< LED5 */
    EXT_LED_IDX_MAX, /**< Maximum value, which cannot be used */
} ext_led_idx_e;

/**
 * @ingroup hct_led
 *
 * Configures the LED mode.
 */
typedef enum {
    EXT_LED_MODE_HIGH = 0, /**< Output pin high active */
    EXT_LED_MODE_LOW = 1,  /**< Output pin low active */
    EXT_LED_MODE_MAX,
} ext_led_mode_e;

/**
 * @ingroup  hct_led
 * @brief  Configures the LED mode.
 *
 * @par Description:
 * Configures the LED mode.
 *
 * @attention None
 * @param  led_mode [IN] Type #ext_led_mode_e Pull-down control bit of the output pin
 *
 * @retval #0       Success
 * @retval #Other values     Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_led.h: Describes I/O APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_void uapi_led_init(ext_led_mode_e led_mode);

/**
 * @ingroup  hct_led
 * @brief  Turns on the LED.
 *
 * @par Description:
 * Turns on the LED.
 *
 * @attention None
 * @param  led_id [IN] Type #ext_led_idx_e LED index
 *
 * @retval #0      Success
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_led.h: Describes LED APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_led_on(ext_led_idx_e led_id);

/**
 * @ingroup  hct_led
 * @brief   Turns off the LED.
 *
 * @par Description:
 * Turns off the LED.
 *
 * @attention None
 * @param  led_id [IN] Type #ext_led_idx_e LED index
 *
 * @retval #0      Success
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_led.h: Describes LED APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_led_off(ext_led_idx_e led_id);

/**
 * @ingroup  hct_led
 * @brief    Flashes the LED cyclically.
 *
 * @par Description:
 * Flashes the LED cyclically.
 *
 * @attention None
 * @param  led_id    [IN] Type #ext_led_idx_e LED index
 * @param  on_ms     [IN] Type #td_u32 LED on duration (unit: ms)
 * @param  off_ms    [IN] Type #td_u32 LED off duration (unit: ms)
 *
 * @retval #0        Success
 * @retval #Other values      Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_led.h: Describes LED APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_led_flash(ext_led_idx_e led_id, td_u32 on_ms, td_u32 off_ms);
#endif
