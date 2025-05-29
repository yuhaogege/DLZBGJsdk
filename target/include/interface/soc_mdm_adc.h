/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Analog-to-digital conversion (ADC) module.
 * By default, cascaded integrator-comb (CIC) and FIR are enabled for ADC.
 * The interrupt context is supported.
 * Usage process:
 * 1) Call uapi_sar_adc_start to set the channel number.
 * 2) Obtain the data of the configured channel.
 * 3) Stop the acquisition on the channel. \n
 * Author: CompanyName
 * Create: 2018-10-31
 */

/**
 * @defgroup hct_sar_adc Analog-to-digital conversion (ADC) module
 * @ingroup  drivers
 */

#ifndef __SOC_MDM_ADC_H__
#define __SOC_MDM_ADC_H__

#include <soc_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup hct_sar_adc
 * Reference voltage configuration
 */
typedef enum {
    EN_VREF_INTERNEL = 0x0, /**< The internal 2.75 V power is used as the reference voltage of the SAR ADC. */
    EN_VREF_OUTSIDE = 0x1   /**< (Default) The external 3.3 V power is used as the reference voltage of the SAR ADC. */
} ext_adc_vref_en;

/**
 * @ingroup  hct_sar_adc
 * @brief   Configures the reference voltage.
 *
 * @par Description:
 *          Configures the reference voltage.
 *
 * @attention The interrupt context is supported.
 * @param  vref             [IN] Type #ext_adc_vref_en Reference voltage configuration
 *
 * @retval None

 * @par Dependency:
 *            @li soc_mdm_adc.h: Describes ADC APIs.
 * @see  uapi_sar_adc_start | uapi_sar_adc_get_data | uapi_sar_adc_stop
 * @since DW21_V100R001C00
 */
td_void uapi_sar_adc_vref_cfg(ext_adc_vref_en vref);

/**
 * @ingroup hct_sar_adc
 * Noise shaping switch configuration
 */
typedef enum {
    EN_NOISE_SHAPING_ON = 0x0, /**< The noise shaping switch is on. */
    EN_NOISE_SHAPING_OFF = 0x1 /**< The noise shaping switch is off. */
} ext_adc_noiseshaping_en;

/**
 * @ingroup  hct_sar_adc
 * @brief   Configures the noise shaping switch.
 *
 * @par Description:
 *          Configures the noise shaping switch.
 *
 * @attention The interrupt context is supported.
 * @param  noiseshaping_pd             [IN] Type #ext_adc_noiseshaping_en Noise shaping switch configuration
 *
 * @retval None

 * @par Dependency:
 *            @li soc_mdm_adc.h: Describes ADC APIs.
 * @see  uapi_sar_adc_start | uapi_sar_adc_get_data | uapi_sar_adc_stop
 * @since DW21_V100R001C00
 */
td_void uapi_sar_adc_noiseshaping_cfg(ext_adc_noiseshaping_en noiseshaping_pd);

/**
 * @ingroup hct_sar_adc
 * Channel number. Only single-channel input is supported.
 */
typedef enum {
    EN_CHANNEL_1 = 0x1,  /**< Channel 1 */
    EN_CHANNEL_2 = 0x2,  /**< Channel 2 */
    EN_CHANNEL_3 = 0x4,  /**< Channel 3 */
    EN_CHANNEL_4 = 0x8,  /**< Channel 4 */
    EN_CHANNEL_5 = 0x10, /**< Channel 5 */
    EN_CHANNEL_6 = 0x20, /**< Channel 6 */
} ext_adc_channel_index_en;

/**
 * @ingroup  hct_sar_adc
 * @brief   Starts ADC.
 *
 * @par Description:
 * Starts ADC according to the parameters configured by the user.
 *
 * @attention The interrupt context is supported. The first sampled data may not be sampled based on the configured
 * sampling rate. You are advised to discard the data.
 * @param  channel_idx  [IN] Type #ext_adc_channel_index_en User-defined channel number.
 * Only single-channel input is supported.
 *
 * @retval #EXT_ERR_SUCCESS  Success
 * @retval #EXT_ERR_FAILURE  Failure.
 * @par Dependency:
 * @li soc_mdm_adc.h: Describes ADC APIs.
 * @see  uapi_sar_adc_get_data | uapi_sar_adc_stop
 * @since DW21_V100R001C00
 */
td_u32 uapi_sar_adc_start(EXT_CONST ext_adc_channel_index_en channel_idx);

/**
 * @ingroup  hct_sar_adc
 * @brief   Reads the converted data.
 *
 * @par Description:
 * Reads the converted data.
 *
 * @attention The interrupt context is supported.
 * @param None
 *
 * @retval #data   Read data. The last 12 bits are valid
 * @retval #0xFFFF  Uninitialized channel or incorrect channel number
 * @par Dependency:
 * @li soc_mdm_adc.h: Describes ADC APIs.
 * @see  uapi_sar_adc_start | uapi_sar_adc_stop
 * @since DW21_V100R001C00
 */
__hot td_u16 uapi_sar_adc_get_data(td_void);

/**
 * @ingroup  hct_sar_adc
 * @brief   Stops ADC.
 *
 * @par Description:
 * Stops ADC.
 *
 * @attention The interrupt context is supported.
 * @param None
 *
 * @retval #EXT_ERR_SUCCESS  Success
 * @retval #EXT_ERR_FAILURE  Failure.
 * @par Dependency:
 * @li soc_mdm_adc.h: Describes ADC APIs.
 * @see  uapi_sar_adc_start | uapi_is_adc_finished
 * @since DW21_V100R001C00
 */
td_void uapi_sar_adc_stop(td_void);

#ifdef __cplusplus
}
#endif
#endif /* __SOC_MDM_ADC_H__ */
