/**
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 *
 * Description: message APIs
 * @defgroup hct_systimer System timer
 * @ingroup osa
 * @li The timer callback function is executed in the interrupt context, and therefore the interrupt context programming
 * precautions must be followed.
 * @li The space of the timer handle ext_timer_handle_s is applied by the caller and cannot be released within the timer
 * running declaration period (before the timer is stopped or expired).
 * @li The timer handle ext_timer_handle_s needs to be initialized to 0 using memset() before it is used for the first
 * time. It cannot be modified within the timer declaration period (before the timer is stopped or expired).
 */
#ifndef __SOC_MDM_TIMER_H__
#define __SOC_MDM_TIMER_H__
#include <soc_types_rom.h>
#include <soc_mdm_isr.h>

/**
 * @ingroup hct_systimer
 *
 * Timer handle
 */
typedef struct {
    td_u32 data[2]; /**< This API is used internally and cannot be operated by users.2 count */
} ext_timer_handle_s;

/**
 * @ingroup hct_systimer
 *
 * Specifies the type of the timer to be created.
 */
typedef enum {
    EXT_TIMER_TYPE_ONCE,   /**< Single-period timer */
    EXT_TIMER_TYPE_PERIOD, /**< Periodic timer */
    EXT_TIMER_TYPE_MAX     /**< Maximum value, which cannot be used */
} ext_timer_type_e;

/**
 * @ingroup  hct_systimer
 * @brief  Defines the type of the timer callback function.
 *
 * @par Description:
 * Defines the type of the timer callback function.
 *
 * @attention None
 * @param  data [IN] Type #td_u32, callback input parameter
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_timer.h: This file describes the timer APIs.
 * @see uapi_timer_start | uapi_timer_stop
 * @since DW21_V100R001C00
 */
typedef td_void (*timer_proc_func)(td_u32 data);

/**
 * @ingroup  hct_systimer
 * @brief  Creates and starts the system timer.
 *
 * @par Description:
 *   This API is used in the following scenarios:
 * @li If no timer is created, create and start a timer.
 * @li If the timer expires and is called again, start the timer directly.
 * @li If the timer does not expire, stop and restart the timer.
 *
 * @attention This timer callback function is executed in the interrupt context. Therefore, the callback function should
 * be as simple as possible and the functions such as sleep and wait semaphores that trigger scheduling cannot be used.
 * @param  handle  [IN/ OUT] Type #ext_timer_handle_s*, handle
 * @param  routine [IN]     Type #TIMER_PROC_FUNC, timer callback function
 * @param  timeout [IN]     Type #td_u32, timeout period of the timer (unit: ms). If this parameter is set to 0, the
 * default value is 10 ms.
 * @param  type    [IN]     Type #ext_timer_type_e, timer type
 * @param  data    [IN]     Type #td_u32, callback input parameter
 *
 * @retval #0       Success.
 * @retval #Other values     Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_timer.h: This file describes the timer APIs.
 * @see uapi_timer_stop
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_timer_start(ext_timer_handle_s *handle, timer_proc_func routine, td_u32 timeout,
    ext_timer_type_e type, td_u32 data);

/**
 * @ingroup  hct_systimer
 * @brief  Stops the system timer.
 *
 * @par Description:
 * Stops the system timer.
 *
 * @attention This API only stops the timer and does not delete the timer.
 * @param  handle [IN] Type #ext_timer_handle_s*, handle
 *
 * @retval #0       Success.
 * @retval #Other values     Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_timer.h: This file describes the timer APIs.
 * @see uapi_timer_start | uapi_timer_delete
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_timer_stop(ext_timer_handle_s *handle);

/**
 * @ingroup  hct_systimer
 * @brief  Deletes the timer.
 *
 * @par Description:
 * Deletes the timer.
 *
 * @attention
 * @li If the timer does not expire, stop the timer before deleting it.
 * @li After the timer is deleted, if the handle has been dynamically applied for, it needs to be released.
 * @param  handle [IN] Type #ext_timer_handle_s*, handle
 *
 * @retval #0       Success.
 * @retval #Other values     Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_timer.h: This file describes the timer APIs.
 * @see uapi_timer_start | uapi_timer_stop
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_timer_delete(ext_timer_handle_s *handle);

EXT_EXTERN td_void uapi_usr_timer_start(td_u32 time_out_us, lisr_proc_func routine);
EXT_EXTERN td_void uapi_usr_timer_stop(td_void);
EXT_EXTERN td_void uapi_usr_timer_clear(td_void);

#endif
