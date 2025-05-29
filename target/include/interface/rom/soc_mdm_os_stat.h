/**
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * @defgroup os_stat System maintenance and testing
 * @ingroup osa
 */

#ifndef __SOC_MDM_OS_STAT_H__
#define __SOC_MDM_OS_STAT_H__
#include <soc_types_rom.h>
#include <soc_mdm_types_rom.h>

#define EXT_OS_STAT_MSG_WAIT_FAIL 0x1 /**< Used in the interrupt context */
#define EXT_OS_STAT_MSG_SEND_FAIL 0x2
/**< Timeout period not 0 for the interrupt context configuration. The queue is full. */
#define EXT_OS_STAT_SEM_WAIT_FAIL 0x4      /**< Used in the interrupt context */
#define EXT_OS_STAT_SEM_SIGNAL_FAIL 0x8    /**< Repeated release */
#define EXT_OS_STAT_MUX_PEND_FAIL 0x10     /**< Used in the interrupt context */
#define EXT_OS_STAT_MUX_POST_FAIL 0x20     /**< Cross-task use, not created */
#define EXT_OS_STAT_EVENT_WAIT_FAIL 0x40   /**< Used in the interrupt context */
#define EXT_OS_STAT_EVENT_SEND_FAIL 0x80   /**< Initialized EVT resources used up */
#define EXT_OS_STAT_EVENT_CLR_FAIL 0x100   /**< Invalid input argument */
#define EXT_OS_STAT_SLEEP_FAIL 0x200       /**< Used in the interrupt context */
#define EXT_OS_STAT_START_TIMER_FAIL 0x400 /**< Invalid input argument */
#define EXT_OS_STAT_ADD_JOB_FAIL 0x1000    /**< WorkQueue used up */

/**
 * @ingroup os_stat
 * System resource usage statistic
 */
typedef struct {
    td_u8 timer_usage; /**< Number of used system timers */
    td_u8 task_usage;  /**< Number of used tasks */
    td_u8 sem_usage;   /**< Number of used semaphores */
    td_u8 queue_usage; /**< Number of used message queues */
    td_u8 mux_usage;   /**< Number of used mutexes */
    td_u8 event_usage; /**< Number of used events */
    td_u16 err_info;   /**< Error statistic EXT_OS_STAT_XXX, used to log occurred errors */
} ext_os_resource_use_stat_s;

/**
 * @ingroup  os_stat
 * @brief  Obtains the system resource usage.
 *
 * @par Description:
 * Obtains the system resource usage.
 *
 * @attention None
 * @param  os_resource_stat [OUT] Type #ext_os_resource_use_stat_s* System resource usage statistic
 *
 * @retval #0               Success
 * @retval #Other values             Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_os_stat.h: Describes system resource usage APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_os_get_resource_status(ext_os_resource_use_stat_s *os_resource_stat);

#endif
