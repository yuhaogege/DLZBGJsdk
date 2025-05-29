/**
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * @defgroup hct_watchdog Watchdog
 * @ingroup drivers
 */

#ifndef __SOC_MDM_WATCHDOG_H__
#define __SOC_MDM_WATCHDOG_H__
#include <soc_types_rom.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup  hct_watchdog
 * @brief Enables the watchdog.
 *
 * @par Description:
 * Enables the watchdog.
 *
 * @attention None
 * @param  None
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_watchdog.h: This file describes the watchdog APIs.
 * @see None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_void uapi_watchdog_enable(td_void);

/**
 * @ingroup  hct_watchdog
 * @brief Feeds the watchdog.
 *
 * @par Description:
 * After this API is called once, the watchdog will be restarted based on the timeout period set by
 * uapi_watchdog_register. If the timeout period has not been set, the default reset duration of the system is used.
 *
 * @attention None
 * @param  None
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_watchdog.h: This file describes the watchdog APIs.
 * @see None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_void uapi_watchdog_feed(td_void);

/**
 * @ingroup  hct_watchdog
 * @brief Disables the watchdog.
 *
 * @par Description:
 * @li Disable the clock enable control of the watchdog.
 * @li Mask the watchdog reset function.
 *
 * @attention None
 * @param  None
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_watchdog.h: This file describes the watchdog APIs.
 * @see None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_void uapi_watchdog_disable(td_void);

/**
 * @ingroup  hct_watchdog
 * @brief  Clears the watchdog interrupt.
 *
 * @par Description:
 * Clears the watchdog interrupt.
 *
 * @attention None
 * @param  None
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_watchdog.h: This file describes the watchdog APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_void uapi_watchdog_int_clear(td_void);

#ifdef __cplusplus
}
#endif
#endif
