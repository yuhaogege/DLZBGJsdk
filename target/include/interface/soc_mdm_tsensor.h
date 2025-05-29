/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description:  Tsensor APIs.
 * Author: CompanyName
 * Create: 2018-10-31
 */
/**
 * @defgroup hct_tsensor Tsensor
 * @ingroup drivers
 */

#ifndef __SOC_MDM_TSENSOR_H__
#define __SOC_MDM_TSENSOR_H__
#include <soc_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup hct_tsensor
 * Tsensor report mode.
 */
typedef enum {
    EXT_TSENSOR_MODE_16_POINTS_SINGLE = 0, /**< Mode: 16 points single. */
    EXT_TSENSOR_MODE_16_POINTS_LOOP = 1,   /**< Mode: 16 points loop. */
    EXT_TSENSOR_MODE_1_POINT_LOOP = 2,     /**< Mode: 1 point loop. */
    EXT_TSENSOR_MODE_MAX,                  /**< Invalid input parameter, which cannot be used. */
} ext_tsensor_mode;

/**
 * @ingroup hct_tsensor
 * Tsensor temperature output register choice.
 */
typedef enum {
    EXT_TSENSOR_TEMP0 = 0,
    EXT_TSENSOR_TEMP1 = 1,
    EXT_TSENSOR_TEMP2 = 2,
    EXT_TSENSOR_TEMP3 = 3,
    EXT_TSENSOR_TEMP_MAX,
} ext_tsensor_temp;

#define EXT_TSENSOR_INVALID_CODE 0xFFFF    /* Tsensor invalid temperature code, valid range [0,255]. */
#define EXT_TSENSOR_INVALID_TEMP (-1000.0) /* Tsensor invalid temperature, valid range [-40, 125]. */

/**
 * @ingroup  hct_tsensor
 * @brief tsensor module start.
 *
 * @par Description:
 * Tsensor start temperature acquisition.
 *
 * @attention  Period is valid when mode = #EXT_TSENSOR_MODE_16_POINTS_LOOP or EXT_TSENSOR_MODE_1_POINT_LOOP.
 * @param  mode             [IN] Type #ext_tsensor_mode Mode of Tsensor temperature acquisition.
 * @param  ms               [IN] Type #td_u32 Acquisition period, it is the multiple of 2 ms.
 *
 * @retval #0           Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_tsensor.h for Tsensor APIs.
 * @see None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_tsensor_start(ext_tsensor_mode mode, td_u32 ms);

/**
 * @ingroup  hct_tsensor
 * @brief Read temperature data.
 *
 * @par Description:
 * After start temperature acquisition, should delay at least 30ms, then read temperature register.
 * @attention no
 * @param  temperature      [IN] Type #td_float*, address wait to be writed the temperature value.
 * @param  num              [IN] Type #ext_tsensor_temp, temperature register, if in mode
 * EXT_TSENSOR_MODE_16_POINTS_SINGLE,
 * the temp register 0 is valid, other temp registers is invalid.
 *
 * @retval #0           Success.
 * @retval #Other values         Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_tsensor.h for Tsensor APIs.
 * @see None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_tsensor_read_temperature(td_float *temperature, ext_tsensor_temp num);

/**
 * @ingroup  hct_tsensor
 * @brief  Tsensor module stop.
 *
 * @par Description:
 * Tsensor stop temperature acquisition.
 *
 * @attention None
 * @retval None
 * @par Dependency:
 * @li soc_mdm_tsensor.h for Tsensor APIs.
 * @see None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_void uapi_tsensor_stop(td_void);

#ifdef __cplusplus
}
#endif
#endif /* __SOC_MDM_TSENSOR_H__ */
