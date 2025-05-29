
#ifndef __SOC_WATCHDOG_H__
#define __SOC_WATCHDOG_H__
#include <soc_types_rom.h>
#include <soc_mdm_watchdog.h>

EXT_START_HEADER

/**
* @ingroup  hct_watchdog
* @brief WDT expiration callback function.
*
* @par Description:
*           WDT expiration callback function.
* 
* @attention None.
* @param  data [IN]: type #td_u32, callback function parameter transfer.
*
* @retval None.
* @par Dependency:
*            @li soc_mdm_watchdog.h£ºThis file describes the interfaces for obtaining the watchdog.
* @see None.
* @since DW21_V100R001C00
 */

typedef td_void (*ext_watchdog_func)(td_u32 data);

/**
 * @ingroup hct_watchdog
 *
 * Indicates the watchdog mode.
 */
typedef enum {
    EXT_WDG_NORMAL = 0, /**< In common mode, the chip is reset when the watchdog expires. */
    EXT_WDG_INTER = 1,  /**< In interrupt mode, the registered callback function is invoked when 
                                     * the watchdog expires for the first time, and the chip is reset 
                                     * when the watchdog expires for the second time. 
                                     */
    EXT_WDG_ERR,        /**< Incorrect input parameter. It cannot be used.*/
} ext_wdg_mode_e;

/**
* @ingroup  hct_watchdog
* @brief  Enable the watchdog.
*
* @par Description:  
*           Enable the watchdog.
*
* @attention None.
* @param  mode         [IN] Type #ext_wdg_mode_e, watchdog mode.
* @param  p_func       [IN] The type is #UAPI_WATCH_DOG_FUNC. When the watchdog is in interrupt mode, 
*                               this function is called to notify the user when the watchdog expires for the first time.
* @param  over_time_ms [IN] Type #td_u32, watchdog expiration time (unit: ms)
*
* @retval #0           Success.
* @retval #A non-zero value indicates a failure. For details, see soc_errno.h.
* @par Dependency:
*            @li soc_mdm_watchdog.h£ºThis file describes the interfaces related to the watchdog.
* @see  None.
* @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_watchdog_register(ext_wdg_mode_e mode, ext_watchdog_func p_func, td_u32 over_time_ms);

EXT_END_HEADER
#endif

