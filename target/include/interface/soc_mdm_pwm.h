/*
 * Copyright (c) CompanyNameMagicTag 2012-2018. All rights reserved.
 * Description: PWM APIs.
 * Author: CompanyName
 * Create: 2012-10-31
 */
/**
 * @defgroup hct_pwm PWM
 * @ingroup drivers
 */

#ifndef SOC_MDM_PWM_H
#define SOC_MDM_PWM_H

#include <soc_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup hct_pwm
 * Enumerates the PWM clock sources.
 */
typedef enum {
    PWM_CLK_APB,  /**< 150M APB clock. */
    PWM_CLK_XTAL, /**< 25M External crystal. */
    PWM_CLK_MAX   /**< Maximum value, which cannot be used. */
} ext_pwm_clk_source;

/**
 * @ingroup hct_pwm
 * Enumerates the PWM ports.
 */
typedef enum {
    EXT_PWM_PORT_PWM0, /**< PWM port. Register base address: 0x4003d200, multiplexed with GPIO0. */
    EXT_PWM_PORT_PWM1, /**< PWM1 port. Register base address: 0x4003d400, multiplexed with GPIO5. */
    EXT_PWM_PORT_MAX   /**< Maximum value, which cannot be used. */
} ext_pwm_port;

/**
 * @ingroup hct_pwm
 * @brief  Initializes the PWM module.
 *
 * @par Description:
 * Initializes the PWM module.
 * @attention
 * @li Before using the PWM function, ensure that the GPIO multiplexing relationship has been configured.
 * @li For details, see section 1.3 "Demo Overview" in the DW21 V100 API Development Guide.
 * @li For details about the multiplexed GPIO, see the DW21 V100 Data Sheet.
 * @li Before using the PWM function, initialize the PWM.
 * @param  port [IN] Type #ext_pwm_port PWM port number.
 * @retval #EXT_ERR_SUCCESS  Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_pwm.h: This file describes the external APIs of the PWM module.
 * @see uapi_pwm_deinit | uapi_pwm_start | uapi_pwm_stop。
 * @since DW21_V100R001C00
 */
td_u32 uapi_pwm_init(ext_pwm_port port);

/**
 * @ingroup hct_pwm
 * @brief  Deinitializes the PWM module.
 *
 * @par Description:
 * Deinitializes the PWM module.
 * @attention Deinitialize the PWM module when the function iss not used.
 * @param  port [IN] Type #ext_pwm_port PWM port number.
 * @retval #EXT_ERR_SUCCESS  Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_pwm.h: This file describes the external APIs of the PWM module.
 * @see uapi_pwm_init。
 * @since DW21_V100R001C00
 */
td_u32 uapi_pwm_deinit(ext_pwm_port port);

/**
 * @ingroup hct_pwm
 * @brief  Sets the clock type of the PWM module.
 *
 * @par Description:
 * This function is used to set the clock type of the PWM module.
 * @attention This setting takes effect for all PWM modules. The 150 MHz clock is used by default.
 * @param  clk_type [IN] Type #ext_pwm_clk_source Clock type.
 * @retval #EXT_ERR_SUCCESS  Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_pwm.h: This file describes the external APIs of the PWM module.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_pwm_set_clock(ext_pwm_clk_source clk_type);

/**
 * @ingroup hct_pwm
 * @brief  Outputs the PWM signal according to the configured parameter.
 * PWM signal duty cycle = duty/freq Frequency = Clock source frequency/freq.
 *
 * @par Description:
 * Starts the PWM signal output.
 * @attention This API cannot be called in an interrupt.
 * @param  port [IN] Type #ext_pwm_port PWM port number.
 * @param  duty [IN] Type #td_u16 PWM duty cycle count. Value range: [1, 65535]. The default value is 750.
 * @param  freq [IN] Type #td_u16 Frequency division multiple. Value range: [1, 65535]. The default value is 1500.
 * @retval #EXT_ERR_SUCCESS  Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_pwm.h: This file describes the external APIs of the PWM module.
 * @see  uapi_pwm_init | uapi_pwm_stop。
 * @since DW21_V100R001C00
 */
td_u32 uapi_pwm_start(ext_pwm_port port, td_u16 duty, td_u16 freq);

/**
 * @ingroup hct_pwm
 * @brief  Stops the PWM signal output.
 *
 * @par Description:
 * Stops the PWM signal output.
 * @attention This API cannot be called in an interrupt.
 * @param  port [IN] Type #ext_pwm_port PWM port number.
 * @retval #EXT_ERR_SUCCESS  Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_pwm.h: This file describes the external APIs of the PWM module.
 * @see  uapi_pwm_init | uapi_pwm_start。
 * @since DW21_V100R001C00
 */
td_u32 uapi_pwm_stop(ext_pwm_port port);

#ifdef __cplusplus
}
#endif
#endif /* SOC_MDM_PWM_H */
