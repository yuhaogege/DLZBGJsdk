/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: CPU usage APIs.
 * Author: CompanyName
 * Create: 2018-10-31
 */
/**
 * @defgroup hct_cpup CPU usage
 * @ingroup  osa
 */

#ifndef __SOC_MDM_CPUP_H__
#define __SOC_MDM_CPUP_H__

#include <soc_types.h>
#include <soc_mdm_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup hct_cpup
 * CPU usage information structure.
 */
typedef struct {
    td_u8 valid : 1;   /**< Whether the information is valid. TD_TRUE: yes; TD_FALSE: no. */
    td_u8 task : 1;    /**< Whether the message is a task or an interrupt. TD_TRUE: task; TD_FALSE: interrupt. */
    td_u8 pad0 : 6;    /**< Reserved. */
    td_u8 id;          /**< Task ID/Interrupt number. */
    td_u16 pad1;       /**< Reserved. */
    td_u32 permillage; /**< CPU usage (per mil). */
    td_u64 cpu_time;   /**< CPU usage time (unit: cputick), where, cputick x 100000000 = 1s. */
} ext_cpup_item;

/**
 * @ingroup  hct_cpup
 * @brief  Obtains the CPU usage.
 *
 * @par Description:
 * Obtains the CPU usage between the initialization of the CPU usage module is started or
 * the CPU usage statistics are reset to each task/interrupt.
 *
 * @attention
 * @li A task/interrupt not scheduled after the CPU usage module is initialized or
 * the CPU usage statistics are reset is excluded.
 *
 * @param array_count [IN] Type #td_u32 Number of CPU usage records that can be stored
 * in the space corresponding to cpup_items.
 * @param cpup_items [OUT] Type #ext_cpup_item* CPU usage information space, applied by the caller.
 *
 * @retval #0               Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_cpup.h: Describes CPU usage APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
td_u32 uapi_cpup_get_usage(td_u32 array_count, ext_cpup_item *cpup_items);

/**
 * @ingroup  hct_cpup
 * @brief  Resets CPU usage statistics.
 *
 * @par Description:
 * Resets CPU usage statistics. After the reset, the CPU usage statistics of all tasks and interrupts are cleared.
 *
 * @attention None
 *
 * @par Dependency:
 * @li soc_mdm_cpup.h: Describes CPU usage APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
td_void uapi_cpup_reset_usage(td_void);

/**
 * @ingroup  hct_cpup
 * @brief  Initializes the CPU usage statistics module.
 *
 * @par Description:
 * Initializes the CPU usage statistics module.
 *
 * @attention
 * @li This API needs to be called before other APIs. This API can be called only once.
 * @li The user needs to modify the API implementation as required.
 *
 *
 * @retval #0   Success
 * @retval #Other values Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_cpup.h: Describes CPU usage APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
td_u32 usr_cpup_init(td_void);

#ifdef __cplusplus
}
#endif
#endif /* __SOC_MDM_CPUP_H__ */
