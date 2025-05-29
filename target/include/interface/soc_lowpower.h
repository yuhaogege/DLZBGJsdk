/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Supports shallow sleep and deep sleep.
 * Author: CompanyName
 * Create: 2018-10-31
 */

/**
 * @defgroup hct_lp Power consumption management
 * @ingroup  system
 */

#ifndef __SOC_LOWPOWER_H__
#define __SOC_LOWPOWER_H__

#include <soc_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup hct_lp
 * Version of the power consumption management module.
 */
#define EXT_LP_VERSION 0x1 /**< 13 valid bits */

/**
 * @ingroup hct_lp
 * Wait time for the power consumption management module to start after the system is powered on,
 * which is not an accurate value.
 */
#define EXT_LP_DELAY_DEFAULT_TIME 3 /**< Unit: second */

/**
 * @ingroup hct_lp
 * Light sleep tick threshold.
 */
#define EXT_LIGHT_SLEEP_THRESHOLD 2 /**< Unit: 10 ms */

/**
 * @ingroup hct_lp
 * Deep sleep tick threshold.
 */
#define EXT_DEEP_SLEEP_THRESHOLD 5 /**< Unit: 10 ms */

/**
 * @ingroup hct_lp
 * Sleep level enumeration.
 */
typedef enum {
    EXT_SLEEP_LIGHT = 0, /**< Light sleep. */
    EXT_SLEEP_DEEP = 1,  /**< Deep sleep */
} ext_lowpower_level;

/**
 * @ingroup hct_lp
 * Sleep module ID enumeration. The value cannot exceed 32. (The value can be 32.)
 */
#define EXT_LOWPOWER_ID_LIMIT 24 /**< Number of #ext_lowpower_id members. */

/**
 * @ingroup hct_lp
 * Sleep module ID enumeration. #EXT_LOWPOWER_ID_LIMIT needs to be modified accordingly if the enumeration is extended.
 */
typedef enum {
    EXT_LOWPOWER_ID_WDG = 0,           /**< Watchdog module */
    EXT_LOWPOWER_ID_PHY = 1,           /**< PLC PHY module */
    EXT_LOWPOWER_ID_AD = 2,            /**< AC data acquisition module */
    EXT_LOWPOWER_ID_SPI0 = 3,          /**< SPI module 0 */
    EXT_LOWPOWER_ID_I2C = 4,           /**< I2C module */
    EXT_LOWPOWER_ID_UART1 = 5,         /**< UART module 1 */
    EXT_LOWPOWER_ID_UART0 = 6,         /**< UART module 0 */
    EXT_LOWPOWER_ID_TIMER0 = 7,        /**< TIMER module 0 */
    EXT_LOWPOWER_ID_TIMER1 = 8,        /**< TIMER module 1 */
    EXT_LOWPOWER_ID_TIMER2 = 9,        /**< TIMER module 2 */
    EXT_LOWPOWER_ID_TIMER3 = 10,       /**< TIMER module 3 */
    EXT_LOWPOWER_ID_TIMER4 = 11,       /**< TIMER module 4 */
    EXT_LOWPOWER_ID_TIMER5 = 12,       /**< TIMER module 5 */
    EXT_LOWPOWER_ID_GPIO = 13,         /**< GPIO module */
    EXT_LOWPOWER_ID_FLASH = 14,        /**< FLASH module */
    EXT_LOWPOWER_ID_SPACC = 15,        /**< AES module */
    EXT_LOWPOWER_ID_SPI1 = 16,         /**< SPI module 1 */
    EXT_LOWPOWER_ID_INT_BOUNDARY = 17, /**< Interrupt boundary. The enumeration within this boundary is strongly
                                       * related to the chip and cannot be changed.
                                       */
    EXT_LOWPOWER_ID_HASH = 17,         /**< HASH module */
    EXT_LOWPOWER_ID_TRNG = 18,         /**< TRNG module */
    EXT_LOWPOWER_ID_RSA = 19,          /**< RSA module */
    EXT_LOWPOWER_ID_RSV1 = 20,         /**< Reserved user ID 1 */
    EXT_LOWPOWER_ID_RSV2 = 21,         /**< Reserved user ID 2 */
    EXT_LOWPOWER_ID_RSV3 = 22,         /**< Reserved user ID 3 */
    EXT_LOWPOWER_ID_RSV4 = 23,         /**< Reserved user ID 4 */
    EXT_LOWPOWER_ID_DC = 24,           /**<  Collect data module */
} ext_lowpower_id;

/**
 * @ingroup hct_lp
 * Registers the structure parameters of the check function.
 */
typedef struct {
    td_u8 id : 6;    /**< Corresponding to #ext_lowpower_id */
    td_u8 light : 1; /**< Supports check in light sleep. */
    td_u8 deep : 1;  /**< Supports check in deep sleep. */
} ext_lowpower_check_id;

/**
 * @ingroup hct_lp
 * Defines the number of check functions for the sleep module.
 */
#define EXT_LOWPOWER_CHECK_FUNC_LIMIT 8

/**
 * @ingroup hct_lp
 * Defines whether the low-power function is used for module check.
 */
typedef td_bool (*lowpower_check_permit_func)(td_u32);

/**
 * @ingroup hct_lp
 * Sleep management structure. Because there are 64-bit members, 64 bit alignment must be ensured.
 */
typedef struct {
    td_u32 int2wakeup; /**< Internal management: Interrupt that allows wakeup. */
    td_u32 int2backup; /**< Internal management: Interrupt configuration backup. */

    td_u32 light : 1;    /**< Internal management: Light sleep enabling flag. */
    td_u32 deep : 1;     /**< Internal management: Deep sleep enabling flag. */
    td_u32 inited : 1;   /**< Internal management: Initialization completion flag. */
    td_u32 ver : 13;     /**< Internal management: Version. */
    td_u32 delay_s : 16; /**< Power-on start wait Time (unit: second). */
    td_u32 timestamp;    /**< Internal management: Timestamp. */

    td_u32 total_count_forbid; /**< Maintenance and testing: Total sleep rejection times,
                                * which may rolls over. For reference only.
                                */
    td_u16 count_light;        /**< Maintenance and testing: Total light sleep times,
                                * which may rolls over. For reference only.
                                */
    td_u16 count_deep;         /**< Maintenance and testing: Total deep sleep times,
                                * which may rolls over. For reference only.
                                */

    td_u32 vote_light; /**< Management, Maintenance and testing: Light sleep vote. Each ID occupies 1 bit. */
    td_u32 vote_deep;  /**< Management, Maintenance and testing: Deep sleep vote. Each ID occupies 1 bit. */

    td_u16 count_forbid[EXT_LOWPOWER_ID_LIMIT]; /**< Maintenance and testing:
                                                * Deep/Light sleep rejection times of each ID.
                                                */
    td_u16 count_wakeup[EXT_LOWPOWER_ID_LIMIT]; /**< Maintenance and testing:
                                                * Wakeup (from deep or light sleep) times of each ID
                                                */

    lowpower_check_permit_func func[EXT_LOWPOWER_CHECK_FUNC_LIMIT]; /**< Internal management: Pointer array of the
                                                                    * function monitoring whether sleep is allowed.
                                                                    */
    td_u32 para[EXT_LOWPOWER_CHECK_FUNC_LIMIT];                     /**< Internal management: Parameter array of
                                                                    * the function monitoring whether sleep is allowed.
                                                                    */

    ext_lowpower_check_id check_id[EXT_LOWPOWER_CHECK_FUNC_LIMIT]; /**< Internal management: Array monitoring
                                                                  * whether sleep ID is allowed.
                                                                  */

    td_u32 under_tick_threshold;   /**< Maintenance and testing:
                                    * Number of times that the time threshold is not reached.
                                    */
    td_u16 under_handle_threshold; /**< Maintenance and testing:
                                    * Number of times that the handling threshold is not reached.
                                    */
    td_u8 unforbid_delay_id_light; /**< Management, Maintenance and testing:
                                    * ID corresponding to canceling the light sleep rejection delay status.
                                    */
    td_u8 unforbid_delay_id_deep;  /**< Management, Maintenance and testing:
                                    * ID corresponding to canceling the deep sleep rejection delay status.
                                    */

    td_u64 unforbid_delay_tick_light; /**< Management, Maintenance and testing:
                                       * Tick for canceling the light sleep rejection delay.
                                       */
    td_u64 unforbid_delay_tick_deep;  /**< Management, Maintenance and testing:
                                       * Tick for canceling the deep sleep rejection delay.
                                       */
    td_u64 time_lightsleep;           /**< Maintenance and testing: Total light sleep time (unit: microsecond). */
    td_u64 time_deepsleep;            /**< Maintenance and testing: Total deep sleep time (unit: microsecond). */
    td_u64 time_wakeup;               /**< Maintenance and testing: Total wakeup time (unit: microsecond). */
    td_u64 current_tick;              /**< Maintenance and testing: Current tick value (unit: 10 ms). */
} ext_lowpower_info;

/**
 * @ingroup  hct_lp
 * @brief  Low power initialization.
 *
 * @par Description:
 * Initializes power consumption management.
 *
 * @attention This is an initialization API and does not support multi-task calling or repeated calling.
 * @param  enable_lightsleep     [IN] Type #td_bool Enables light sleep.
 * @param  enable_deepsleep      [IN] Type #td_bool Enables deep sleep.
 * @param  delay_s      [IN] Type #td_u16 Initialization wait time (unit: second)
 *
 * @retval #0                Success.
 * @retval #Other values     Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_lowpower.h: Describes power consumption management APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
td_u32 uapi_lowpower_init(td_bool enable_lightsleep, td_bool enable_deepsleep, td_u16 delay_s);

/**
 * @ingroup  hct_lp
 * @brief  Obtains power consumption management information.
 *
 * @par Description:
 * Obtains power consumption management information.
 *
 * @attention None
 * @param  info     [OUT] Type #ext_lowpower_info Low power consumption information.
 *
 * @retval #0               Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_lowpower.h: Describes power consumption management APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
td_u32 uapi_lowpower_get_info(ext_lowpower_info *info);

/**
 * @ingroup  hct_lp
 * @brief  Low power consumption handling.
 *
 * @par Description:
 * Low power consumption handling.
 *
 * @attention None
 * @param  None
 *
 * @retval None
 * @par Dependency:
 * @li soc_lowpower.h: Describes power consumption management APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
td_void uapi_lowpower_handle(td_void);

/**
 * @ingroup  hct_lp
 * @brief  Checks the power consumption management registration.
 *
 * @par Description:
 * Checks the power consumption management registration.
 *
 * @attention This is an initialization API and does not support multi-task calling or repeated calling.
 * @param  id     [IN] Type #ext_lowpower_check_id Module ID.
 * @param  func   [IN] Type #lowpower_check_permit_func Checks whether the low-power function is allowed.
 * @param  para   [IN] Type #td_u32 Input argument of the function.
 *
 * @retval #0               Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_lowpower.h: Describes power consumption management APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
td_u32 uapi_lowpower_register_checkfunc(ext_lowpower_check_id id, lowpower_check_permit_func func, td_u32 para);

/**
 * @ingroup  hct_lp
 * @brief  Whether to wake up the sleep module.
 *
 * @par Description:
 * Whether to wake up the sleep module.
 *
 * @attention This is an initialization API and does not support multi-task calling or repeated calling.
 * @param  id     [IN] Type #ext_lowpower_id Module ID. Only hardware interrupt wakeup is supported.
 * If the ID value is greater than or equal to EXT_LOWPOWER_ID_INT_BOUNDARY, an error is returned.
 * @param  support_wakeup   [IN] Type #td_bool Whether to support wakeup.
 *
 * @retval #0               Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_lowpower.h: Describes power consumption management APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
td_u32 uapi_lowpower_wakeup_config(ext_lowpower_id id, td_bool support_wakeup);

/**
 * @ingroup  hct_lp
 * @brief  Votes for not entering low power mode.
 *
 * @par Description:
 * Votes for not entering low power mode.
 *
 * @attention None
 * @param  id           [IN] Type #ext_lowpower_id Module ID.
 * @param  deep_sleep   [IN] Type #td_bool Whether to cancel the deep sleep rejection.
 *
 * @retval #0               Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_lowpower.h: Describes power consumption management APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
td_u32 uapi_lowpower_forbid(ext_lowpower_id id, ext_lowpower_level deep_sleep);

/**
 * @ingroup  hct_lp
 * @brief  Votes for canceling the low power rejection.
 *
 * @par Description:
 * Votes for canceling the low power rejection.
 *
 * @attention None
 * @param  id            [IN] Type #ext_lowpower_id Module ID.
 * @param  deep_sleep    [IN] Type #ext_lowpower_level Lowpower level, deep/light.
 * @param  delay_ms      [IN] Type #td_u32 Period after which the rejection is canceled (unit: ms).
 *
 * @retval #0               Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_lowpower.h: Describes power consumption management APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
td_u32 uapi_lowpower_unforbid(ext_lowpower_id id, ext_lowpower_level deep_sleep, td_u32 delay_ms);

/**
 * @ingroup  hct_lp
 * @brief  Whether to wake up the deep sleep module.
 *
 * @par Description:
 * Whether to wake up the deep sleep module.
 *
 * @attention None
 * @param  None
 * @retval None
 *
 * @li soc_lowpower.h: Describes power consumption management APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
td_void uapi_init_lowpower_deep_sleep_proc(td_void);
#ifdef __cplusplus
}
#endif
#endif /* SOC_LOWPOWER_H */
