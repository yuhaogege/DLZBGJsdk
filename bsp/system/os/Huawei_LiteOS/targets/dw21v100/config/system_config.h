/*************************************************************/
#ifndef _SYSTEM_CONFIG_H
#define _SYSTEM_CONFIG_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */
/*=============================================================================
                                       LiteOS kernel version configuration
=============================================================================*/

/**
 * @ingroup los_config
 * Version number
 */
#define LITEOS_VER                                          "Huawei LiteOS Kernel V200R002C00B060"

#define KERNEL_START                                        "Kernel Start OK"

/** system source configuration**/
/**
 * @ingroup los_config
 * system clock
 */
#define OS_SYS_CLOCK_CONFIG                               80000000

/** system source configuration**/
/**
 * @ingroup los_config
 * Number of Ticks in one second
 */
#define LOSCFG_BASE_CORE_TICK_PER_SECOND_CONFIG           100


/** system source configuration**/
/**
 * @ingroup los_config
 * Maximum supported number of tasks except the idle task rather than the number of usable tasks
 */
#define LOSCFG_BASE_CORE_TSK_LIMIT_CONFIG                 14 //hct_modify 32

/**
 * @ingroup los_config
 * Maximum supported number of semaphores
 */
#define LOSCFG_BASE_IPC_SEM_LIMIT_CONFIG                  30    //hct_modify 100
/**
 * @ingroup los_config
 * Maximum supported number of mutexes
 */
#define LOSCFG_BASE_IPC_MUX_LIMIT_CONFIG                  1     //hct_modify  64
/**
 * @ingroup los_config
 * Maximum supported number of queues rather than the number of usable queues
 */
#define LOSCFG_BASE_IPC_QUEUE_LIMIT_CONFIG                7 //hct_modify 64, data collect +1
/**
 * @ingroup los_config
 * Maximum supported number of software timers rather than the number of usable software timers
 */
#define LOSCFG_BASE_CORE_SWTMR_LIMIT_CONFIG                59 //hct_modify 80, mac + 4, mrs add 28 £¬diag + 1(26 + 4 + 28 + 1 = 59)

/**
 * @ingroup los_config
 * Default task stack size
 */
#define LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE_CONFIG          (0x1000UL)
/**
 * @ingroup los_config
 * Idle task stack size.
 */
#define LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE_CONFIG             (0x180UL)
/**
 * @ingroup los_config
 * Minimum stack size.
 */
#define LOSCFG_BASE_CORE_TSK_MIN_STACK_SIZE_CONFIG              (0x80UL)
/**
 * @ingroup los_config
 * Longest execution time of tasks with the same priorities
 */
#define LOSCFG_BASE_CORE_TIMESLICE_TIMEOUT_CONFIG               10
/**
 * @ingroup los_cpup
 * Number of track items
 */
#define LOSCFG_BASE_MISC_TRACK_COUNT_CONFIG                     30

/**
 * @ingroup los_config
 * Configuration item for workqueue priority
 */
#define LOSCFG_BASE_WORKQUEUE_TASK_PRIO_CONFIG                  2

/**
 * @ingroup los_config
 * Configuration item for workqueue task stack size
 */
#define LOSCFG_BASE_WORKQUEUE_TASK_STACK_SIZE_CONFIG            (0x400UL)


#define SUPPORT_DEEP_SLEEP                                     YES

 /* retain part of trace function in RAM, in oder not to affect the ROM area binary */
#define LOSCFG_BASE_MISC_TRACK_RAM                      YES   /* open part of trace function in RAM */

#define LOSCFG_BASE_CORE_CPUP_RAM                      YES   /* close part of CPUP function in RAM */
#define LOSCFG_BASE_CORE_CPUP_HWI_RAM                      YES   /* close part of CPU function in RAM */

/* abnormal information printing function */
#define  LOSCFG_EXCEPTION_INFO_DISPLAY   NO


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


#endif /* _SYSTEM_CONFIG_H*/
/*************************************************************/
