/**
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 *
 * Description: time APIs
 * @defgroup hct_time Time
 * @ingroup osa
 * @li System time of the uapi_get_tick, uapi_get_tick64, uapi_get_milli_seconds, and uapi_get_seconds operations. The system
 * time is calculated starting from kernel startup and cannot be modified through APIs.
 * @li The uapi_get_real_time and uapi_set_real_time APIs are about real-time operations. The real time is calculated
 * starting from kernel startup and can be modified by uapi_set_real_time.
 */
#ifndef __SOC_MDM_TIME_H__
#define __SOC_MDM_TIME_H__
#include <soc_types_rom.h>
#ifdef __cplusplus
extern "C" {
#endif
/**
 * @ingroup  hct_time
 * @brief  Delay, in microseconds
 *
 * @par Description:
 * Delay operation implemented by software based on the system clock, blocking the CPU
 *
 * @attention This API cannot be used for a long time in an interrupt.
 * @param  us                [IN] Type #td_u32, delay period (unit: microsecond)
 *
 * @retval #EXT_ROM_ERR_SUCCESS   Success
 * @retval #Other values       Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_time.h: This file contains the system time APIs.
 * @see  uapi_hw_udelay
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_udelay(td_u32 us);

/**
 * @ingroup  hct_time
 * @brief  Obtains the tick value of the system (32-bit).
 *
 * @par Description:
 * Obtains the tick value of the system (32-bit).
 *
 * @attention None
 * @param None
 *
 * @retval Type #td_u32 Tick value of the system
 * @par Dependency:
 * @li soc_mdm_time.h: This file contains the system time APIs.
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_get_tick(td_void);

/**
 * @ingroup  hct_time
 * @brief  Obtains the tick value of the system (64-bit).
 *
 * @par Description:
 * Obtains the tick value of the system (64-bit).
 *
 * @attention The soc_mdm_time.h file must be included where the API is called. Otherwise, the API is considered not
 * declared, and the tick value is returned as an int type, resulting in a truncation error.
 * @param None
 *
 * @retval Type #TD_U64 Tick value of the system
 * @par Dependency:
 * @li soc_mdm_time.h: This file contains the system time APIs.
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u64 uapi_get_tick64(td_void);

/**
 * @ingroup  hct_time
 * @brief  Obtains the system time (unit: ms).
 *
 * @par Description:
 * Obtains the system time (unit: ms).
 *
 * @attention None
 * @param None
 *
 * @retval Type #td_u32 System time
 * @par Dependency:
 * @li soc_mdm_time.h: This file contains the system time APIs.
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_get_milli_seconds(td_void);

/**
 * @ingroup  hct_time
 * @brief  Obtains the system time (unit: s).
 *
 * @par Description:
 * Obtains the system time (unit: s).
 *
 * @attention None
 * @param None
 *
 * @retval Type #td_u32 System time
 * @retval #EXT_ROM_ERR_FAILURE The system time failed to be obtained.
 * @par Dependency:
 * @li soc_mdm_time.h: This file contains the system time APIs.
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_get_seconds(td_void);

/**
 * @ingroup  hct_time
 * @brief  Obtains the real time of the system (unit: s).
 *
 * @par Description:
 * Obtains the real time of the system (unit: s).
 *
 * @attention None
 * @param None
 *
 * @retval Type #td_u32 Real time of the system
 * @retval #EXT_ROM_ERR_FAILURE The system time failed to be obtained.
 * @par Dependency:
 * @li soc_mdm_time.h: This file contains the system time APIs.
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_get_real_time(td_void);

/**
 * @ingroup  hct_time
 * @brief  Sets the real time of the system.
 *
 * @par Description:
 * Sets the real time of the system.
 *
 * @attention None
 * @param  seconds            [IN] Type #td_u32, set the real time of the system to this value.
 *
 * @retval #EXT_ROM_ERR_SUCCESS    Setting succeeded.
 * @retval #EXT_ROM_ERR_FAILURE    Setting failed.
 * @par Dependency:
 * @li soc_mdm_time.h: This file contains the system time APIs.
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_set_real_time(td_u32 seconds);

#ifdef __cplusplus
}
#endif
#endif
