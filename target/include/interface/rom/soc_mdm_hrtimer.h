/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 *
 * Description: message APIs. hct_hrtimer High resolution timer (hrtimer).
 * @li When the resolution requirement is not strict, do not use hrtimer, but use a common timer instead.
 * Because each time the hrtimer starts, stops, or expires, the errors of other high resolution timers in the queue
 * increases.
 * @li The hrtimer callback function is executed in the interrupt context and
 * therefore the interrupt context programming precautions must be followed.
 * @li The space of the hrtimer handle ext_hrtimer_type_s is applied by the caller and cannot be
 * released within the timer running declaration period (before hrtimer is stopped or expired).
 * @li The hrtimer handle ext_hrtimer_type_s needs to be memset to 0 before it is used for the first time.
 * It cannot be modified within the timer declaration period (before hrtimer is stopped or expired).
 */
#ifndef __SOC_MDM_HRTIMER_H__
#define __SOC_MDM_HRTIMER_H__
#include <soc_types_rom.h>
#include <soc_mdm_types_rom.h>

#define EXT_HRTIMER_UDELAY_MAX_TIME (10 * 1000 * 1000) /**< Maximum delay (unit: microsecond), 10s */
/*
 * @ingroup  hct_hrtimer
 * @brief  High resolution timer callback function
 *
 * @par Description:
 * High resolution timer callback function. When a high resolution timer expires, the high
 * resolution timer module calls this function to notify the user.
 *
 * @attention None
 * @param  data [IN] Type #td_u32 Callback function parameter input when the user starts the timer
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_hrtimer.h: Describes timer APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
typedef td_void (*ext_timer_callback_f)(td_u32 data);

/*
 * @ingroup hct_hrtimer
 *
 * hrtimer internal linked list structure
 */
typedef struct ext_hrtimer_list_node_type_s {
    struct ext_hrtimer_list_node_type_s *prv;  /**< Forward pointer */
    struct ext_hrtimer_list_node_type_s *next; /**< Backward pointer */
    td_void *parent;                          /**< Point to the parent node */
} ext_hrtimer_list_node_type_s;

/**
 * @ingroup hct_hrtimer
 *
 * hrtimer handle structure.
 */
typedef struct {
    td_u32 expires;                   /**< Expiration time (unit: microsecond) */
    td_u32 data;                      /**< Callback function parameter */
    ext_timer_callback_f hrtimer_func; /**< Callback function */
    td_u32 remaining_time;            /**< Remaining time (unit: microsecond) */
    ext_hrtimer_list_node_type_s node; /**< Linked list node */
} ext_hrtimer_type_s;

/**
 * @ingroup  hct_hrtimer
 * @brief  Initializes the high resolution timer module.
 *
 * @par Description:
 * Before calling a high resolution timer, the module of the high resolution timer must be initialized through this API.
 *
 * @attention Only the first initialization is valid.
 * @param  irq_num    [IN] Type #td_u32 Interrupt ID corresponding to the high resolution timer.
 * For details about the value, see #IRQn_Type.
 * @param  base_addr  [IN] Type #td_u32 Base address of the high resolution timer. A possible value is #TIMER0_REG_BASE.
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_hrtimer.h: Describes timer APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_void uapi_hrtimer_init(td_u32 irq_num, td_u32 base_addr);

/**
 * @ingroup  hct_hrtimer
 * @brief  Starts a high resolution timer.
 *
 * @par Description:
 * Starts the high resolution timer. If the timer has been started,
 * the current timer is stopped and restarted when this API is called.
 *
 * @attention None
 * @param  timer        [IN] Type #ext_hrtimer_type_s* Timer handle
 * @param  hrtimer_func [IN] Type #EXT_TIMER_CALLBACK_F Callback function when the timer expires
 * @param  us           [IN] Type #td_u32 Expiration time of the timer (unit: microsecond)
 * @param  data         [IN] Type #td_u32 Input parameter of the timer callback function
 *
 * @retval #0           Success
 * @retval #Other values         Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_hrtimer.h: Describes timer APIs.
 * @see  uapi_hrtimer_stop
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_hrtimer_start(ext_hrtimer_type_s *timer, ext_timer_callback_f hrtimer_func, td_u32 us, td_u32 data);

/**
 * @ingroup  hct_hrtimer
 * @brief  Stops a high resolution timer.
 *
 * @par Description:
 * Stops a high resolution timer. If the timer is stopped when the API is called, no effect is achieved.
 *
 * @attention None
 * @param  timer [IN] Type #ext_hrtimer_type_s* Timer handle
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_hrtimer.h: Describes timer APIs.
 * @see  uapi_hrtimer_start
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_void uapi_hrtimer_stop(ext_hrtimer_type_s *timer);

/*
 * @ingroup  hct_hrtimer
 * @brief  The delay archived by using a hardware timer is more accurate than that of the software delay by using
 * uapi_udelay.
 *
 * @par Description:
 * The delay archived by using a hardware timer is more accurate than that of the software delay.
 * The hardware timer resource requires the user to implement critical resource protection.
 *
 * @attention The CPU is not yielded during the delay.
 * @param  us [IN] Type #td_u32 Delay period (unit: microsecond) The delay period cannot exceed
 * EXT_HRTIMER_UDELAY_MAX_TIME.
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_hrtimer.h: Describes timer APIs.
 * @see  uapi_udelay
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_hw_udelay(td_u32 us);

/*
 * uapi_hrtimer_refresh_tick Updates a high resolution timer,
 * which is used for the low power mode. It is not an SDK API.
 */
EXT_EXTERN td_void uapi_hrtimer_refresh_tick(td_u32 tick);
/*
 * uapi_hrtimer_get_current_tick Obtains the current tick value to determine
 * whether to enter low power mode. It is not an SDK API
 */
EXT_EXTERN td_u32 uapi_hrtimer_get_current_tick(td_u32 *tick);
EXT_EXTERN td_u32 uapi_hw_udelay_get_cur_val(td_void);

#endif
