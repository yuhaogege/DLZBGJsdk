/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Active reset.
 * Author: CompanyName
 * Create: 2018-10-31
 */
/**
 * @defgroup hct_sys Active reset
 * @ingroup system
 */

#ifndef __SOC_MDM_SYS_H__
#define __SOC_MDM_SYS_H__

#include <soc_types.h>
#include <soc_mdm_types.h>
#include <soc_ft_nv.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Active reset condition */
#define EXT_RST_STA_SECURE_TIME (75 * 24 * 60 * 60) /**< Default value: 75 days */

/* Delay definition Unit: ms */
#define EXT_RST_DELAY_USR_TIME_MAX (5 * 60 * 1000) /**< 5min */
#define EXT_RST_DELAY_WRT_NET_SERIAL 10000         /**< 10s */
#define EXT_RST_DELAY_WRT_SNID 10000               /**< 10s */
#define EXT_RST_DELAY_WRT_RNG 10000                /**< 10s */

#define EXT_SYS_REBOOT_CAUSE_USR_BEGIN 0x8000
#define EXT_SYS_REBOOT_CAUSE_USR_END 0x9FFF
#define EXT_SYS_REBOOT_CAUSE_MASK 0xFFFF

/**
 * @ingroup  hct_sys
 * Cause for active restart
 */
typedef enum {
    EXT_SYS_REBOOT_CAUSE_UNKNOWN = 0,

    EXT_SYS_REBOOT_CAUSE_CMD = 0x1, /**< System reset command. */

    EXT_SYS_REBOOT_CAUSE_UPG = 0x100, /**< Upgrade reset. */
    EXT_SYS_REBOOT_CAUSE_UPG_SOFT,    /**< During the upgrade,MAC soft reset and CCO automatic reset are performed. */
    EXT_SYS_REBOOT_CAUSE_MAC_EXIT_NETWORK_TEST_MODE = 0x200,
    EXT_SYS_REBOOT_CAUSE_MAC1,

    EXT_SYS_REBOOT_CAUSE_RST0 = 0x300, /**< Active reset. For details, see ext_rst_con. */

    EXT_SYS_REBOOT_CAUSE_USR0 = EXT_SYS_REBOOT_CAUSE_USR_BEGIN, /* User-defined. */
    EXT_SYS_REBOOT_CAUSE_USR1,
    EXT_SYS_REBOOT_CAUSE_USR2,
    EXT_SYS_REBOOT_CAUSE_USR3_STC_DS,
    EXT_SYS_REBOOT_CAUSE_USR4_EXIT_PHY_TEST,
    EXT_SYS_REBOOT_CAUSE_MAX = 0xFFFF,
} ext_sys_reboot_cause;

/**
 * @ingroup  hct_sys
 * @brief  Restarts the board.
 *
 * @par Description:
 * Restarts the board.
 *
 * @attention This API is affected by the restart delay function. When this API is called during the restart delay,
 * the board does not restart immediately. Instead, the board restarts only after the restart delay time expires.
 * @param cause [IN] Type #ext_sys_reboot_cause Restart ID. Value range: [0x8000, 0xA000). The restart ID is used to
 * view the cause of system restart.
 * @retval #EXT_ERR_SUCCESS  Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_sys.h: This file describes the APIs for obtaining system information.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_usr_reboot(ext_sys_reboot_cause cause);

/**
 * !!! NOTICE !!!:
 * Do NOT adjust the sequence defined by the enumeration. Otherwise, the program processing may be affected.
 */
typedef enum {
    EXT_RST_CON_CCO_MAX_PROXY,       /**< CCO The maximum number of proxies exceeds the threshold.
                                     * The default duration is 12 hours.
                                     */
    EXT_RST_CON_PLC_NO_TX,           /**< PLC No SOF frame is sent. The default duration is 8 hours. */
    EXT_RST_CON_PLC_NO_RX,           /**< PLC No SOF frame is received. The default duration is 8 hours. */
    EXT_RST_CON_STA_SECURE_RESET,    /**< STA Random reset (to prevent unrecoverable problems caused by long-time
                                     * running of the board) is performed at an interval of 60 to 90 days. */
    EXT_RST_CON_STA_NO_JOIN_NETWORK, /**< STA The network is not connected. The default duration is 24 hours. */
    EXT_RST_CON_NO_ROUTE_EVALUATE,   /**< STA Route evaluation is not started. The default duration is 24 hours. */

    EXT_RST_CON_USR0, /**< Reserved API 0 for users. */
    EXT_RST_CON_USR1, /**< Reserved API 1 for users. */

    EXT_RST_CON_MAX,
} ext_rst_con;

/**
 * @ingroup  hct_sys
 * @brief  Registers the active reset function. (This is the callback function prototype.)
 *
 * @par Description:
 * Registers the active reset function. (This is the callback function prototype.)
 *
 * @attention None
 * @retval #td_u32  Statistical result.
 * @par Dependency:
 * @li soc_mdm_sys.h: This file describes the APIs for obtaining system information.
 * @see uapi_sys_reset_delay_enable¡£
 * @since DW21_V100R001C00
 */
typedef td_u32 (*ext_sys_reset_notify_fct)(td_void);

/**
 * @ingroup  hct_sys
 * @brief  Registers the active reset function.
 *
 * @par Description:
 * Registers the active reset function. The active reset duration is configured through of the NV item 0x35.
 *
 * @attention None
 *
 * @param notify_func [IN] Type #ext_sys_reset_notify_fct Statistics query callback function, which returns the
 * statistics result. The system periodically checks the statistics. If the system detects
 * that the statistics exceed the preset time and periodic check results do not vary, active
 * reset is performed.
 * @param condition   [IN] Type #ext_rst_con ID of an active reset event. EXT_RST_CON_USR0 and EXT_RST_CON_USR1 are
 * reserved for users. Do not use other enumerated values since they have been used in the SDK.
 * The cause of active reset can be viewed in the system crash information. For details,
 * see the DW21 V100R001 System Crash and Restart Fault Location Guide.
 * @retval #EXT_ERR_SUCCESS  Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_sys.h: This file describes the APIs for obtaining system information.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_sys_register_reset_notify(EXT_CONST ext_sys_reset_notify_fct notify_func, ext_rst_con condition);

/**
 * @ingroup  hct_sys
 * @brief  Enable active reset condition.
 *
 * @par Description:
 *         Enable active reset condition. The conditions list from ext_rst_con.
 *
 * @attention None
 *
 * @param condition [IN] Type #ext_rst_con ID of an active reset event. Condition list from ext_rst_con.
 *                       The condition will be set separately.

 * @param enable   [IN] Type #td_bool Enable/Disable the active reset event. Set TD_TRUE to enable, TD_FALSE to disable.

 * @retval None
 * @par Dependency:
 *   @li soc_mdm_sys.h: This file describes the APIs for obtaining system information.
 * @see None
 * @since DW21_V100R001C00
 */
EXT_EAPI td_void uapi_sys_enable_reset(EXT_CONST ext_rst_con condition, EXT_CONST td_bool enable);

/**
 * @ingroup  hct_sys
 * Indicates the ID of the delayed restart event.
 */
typedef enum {
    EXT_RST_DELAY_CON_USR0,           /**< Reserved API 0 for users. */
    EXT_RST_DELAY_CON_USR1,           /**< Reserved API 1 for users. */
    EXT_RST_DELAY_CON_UPG,            /**< Do not perform reset during the upgrade. */
    EXT_RST_DELAY_CON_WRT_NET_SERIAL, /**< CCO Reset is not allowed when the network serial number is written. */
    EXT_RST_DELAY_CON_WRT_SNID,       /**< CCO/ STA Reset is not allowed when the SNID is written. */
    EXT_RST_DELAY_CON_WRT_RNG,        /**< STA Reset is not allowed when the associated random number is written. */
    EXT_RST_DELAY_CON_LOCK_NETWORK,   /**< Reset is not allowed when the network is locked. */
    EXT_RST_DELAY_CON_MAX,
} ext_rst_delay_con;

#define EXT_RST_DELAY_CON_USR_BEGIN (EXT_RST_DELAY_CON_USR0)
#define EXT_RST_DELAY_CON_USR_END (EXT_RST_DELAY_CON_USR1)

/**
 * @ingroup  hct_sys
 * @brief  Enables the restart delay function.
 *
 * @par Description:
 * Enables the restart delay function.
 *
 * @attention None
 *
 * @param condition [IN] Type #ext_rst_delay_con ID of a restart delay event. When this parameter is reserved for users,
 * the value range is [0, 1]. Other values are used by the SDK.
 * @param ms        [IN] Type #td_u32 Restart delay time (unit: ms). The maximum value is 5 minutes.
 * @retval #EXT_ERR_SUCCESS  Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_sys.h: This file describes the APIs for obtaining system information.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_sys_reset_delay_enable(ext_rst_delay_con condition, td_u32 ms);

/**
 * @ingroup  hct_sys
 * @brief  Disables the restart delay function.
 *
 * @par Description:
 * Disables the restart delay function.
 *
 * @attention
 *
 * @param condition [IN] Type #ext_rst_delay_con ID of a restart delay event. When this parameter is reserved for users,
 * the value range is [0, 1]. Other values are used by the SDK.
 * @retval #EXT_ERR_SUCCESS  Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_sys.h: This file describes the APIs for obtaining system information.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_sys_reset_delay_disable(ext_rst_delay_con condition);

/**
 * @ingroup  hct_sys
 * @brief  Enables or disables the function of recording the reset times.
 *
 * @par Description:
 * @li Enables or disables the function of recording the reset times. When the function of recording the reset times
 * is enabled, the NV is recorded every 30 seconds when the board is started, and the number of reset times is
 * recorded normally.
 * @li When the number of reset times is updated, the flash memory needs to be written. Generally, when the system
 * frequently powers on and off and power-on NV write greatly hurts the flash service life, set enable to
 * TD_FALSE and check whether the service function is affected.
 * @attention
 * @li This feature should be enabled by default.
 * @li It should be called in the initialization function of the app layer.
 * @li Disabling this function may result in some influences. You are advised to disable it in scenarios where the
 * system is frequently powered on and off.
 * @retval None
 * @par Dependency:
 * @li soc_mdm_sys.h: This file describes the APIs for obtaining system information.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_enable_reset_times_save(td_bool enable);

#ifdef __cplusplus
}
#endif
#endif /* __SOC_MDM_SYS_H__ */
