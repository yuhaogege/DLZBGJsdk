/*----------------------------------------------------------------------------
 * Copyright (c) <2013-2015>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

/**@defgroup los_config System configuration items
 * @ingroup kernel
 */

#ifndef _TARGET_CONFIG_H
#define _TARGET_CONFIG_H

#include "los_typedef.h"
#include "hal_sectors.h"
#include "dw21.h"
#include "core_cm3.h"
#include "system_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define LOSCFG_CORTEX_M3

#define _T(x)                                               x

/*=============================================================================
                                        Lib configurable feature configuration
=============================================================================*/

/**
 * @ingroup los_config
 * Configuration lib configurable feature to open
 */
#ifdef LIB_CONFIGURABLE
#define LOSCFG_LIB_CONFIGURABLE                             YES
#else
#define LOSCFG_LIB_CONFIGURABLE                             NO
#endif


/*=============================================================================
                                       Test module configuration
=============================================================================*/

/**
 * @ingroup los_config
 * Configuration test case to open
 */
#ifdef DW21TEST
#define LOSCFG_TEST                                         YES
#endif

/*=============================================================================
                                       LIBC module configuration
=============================================================================*/

/**
 * @ingroup los_config
 * Configuration to support libc
 */
#define LOSCFG_LIB_LIBC                                     YES

/*=============================================================================
                                        System clock module configuration
=============================================================================*/

/**
 * @ingroup los_config
 * System clock (unit: HZ)
 */
#if (LOSCFG_LIB_CONFIGURABLE == YES)
extern UINT32                                               g_uwOsSysClock;
#define OS_SYS_CLOCK                                        g_uwOsSysClock
#else
#define OS_SYS_CLOCK                                        (80000000UL)
#endif

/**
 * @ingroup los_config
 * Number of Ticks in one second
 */
#if (LOSCFG_LIB_CONFIGURABLE == YES)
extern UINT32                                               g_uwMinusOneTickPerSecond;
#define LOSCFG_BASE_CORE_TICK_PER_SECOND                    (g_uwMinusOneTickPerSecond + 1)   /* tick per sencond plus one */
#else
#define LOSCFG_BASE_CORE_TICK_PER_SECOND                    (100UL)
#endif

/**
 * @ingroup los_config
 * External configuration item for timer tailoring
 */
#define LOSCFG_BASE_CORE_TICK_HW_TIME                       YES

/*=============================================================================
                                        Hardware interrupt module configuration
=============================================================================*/

/**
 * @ingroup los_config
 * Configuration item for hardware interrupt tailoring
 */
#define LOSCFG_PLATFORM_HWI                                 YES

/**
 * @ingroup los_config
 * Maximum number of used hardware interrupts, including Tick timer interrupts.
 */
#define LOSCFG_PLATFORM_HWI_LIMIT                           32

/**
 * @ingroup los_config
 * Maximum number of used hardware interrupts.
 */
#define OS_HWI_MAX_NUM                                      17

/**
 * @ingroup los_config
 * Highest priority of a hardware interrupt.
 */
#define OS_HWI_PRIO_HIGHEST                                 1

/**
 * @ingroup los_config
 * Lowest priority of a hardware interrupt.
 */
#define OS_HWI_PRIO_LOWEST                                  7

/*=============================================================================
                                       Task module configuration
=============================================================================*/

/**
 * @ingroup los_config
 * Minimum stack size.
 *
 * 0x80 bytes, aligned on a boundary of 4.
 */
#if (LOSCFG_LIB_CONFIGURABLE == YES)
extern UINT32                                               g_uwTskMinStkSize;
#define LOSCFG_BASE_CORE_TSK_MIN_STACK_SIZE                 g_uwTskMinStkSize
#else
#define LOSCFG_BASE_CORE_TSK_MIN_STACK_SIZE                 (ALIGN(0x80, 4))
#endif
    
/**
 * @ingroup los_config
 * Idle task stack size.
 */
#if (LOSCFG_LIB_CONFIGURABLE == YES)
extern UINT32                                               g_uwTskIdleStkSize;
#define LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE                g_uwTskIdleStkSize
#else
#define LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE                (0x180UL)
#endif

/**
 * @ingroup los_config
 * Maximum supported number of tasks except the idle task rather than the number of usable tasks
 */
#if (LOSCFG_LIB_CONFIGURABLE == YES)
extern UINT32                                               g_uwTskLimit;
#define LOSCFG_BASE_CORE_TSK_LIMIT                          g_uwTskLimit
#else
#define LOSCFG_BASE_CORE_TSK_LIMIT                          32
#endif

/**
 * @ingroup los_config
 * Default task stack size
 */
#if (LOSCFG_LIB_CONFIGURABLE == YES)
extern UINT32                                               g_uwTskDfltStkSize;
#define LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE             g_uwTskDfltStkSize
#else
#define LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE             (0x1000UL)
#endif

/**
 * @ingroup los_config
 * Longest execution time of tasks with the same priorities
 */
#if (LOSCFG_LIB_CONFIGURABLE == YES)
extern UINT32                                               g_uwTimeSliceTimeOut;
#define LOSCFG_BASE_CORE_TIMESLICE_TIMEOUT                  g_uwTimeSliceTimeOut
#else
#define LOSCFG_BASE_CORE_TIMESLICE_TIMEOUT                  10
#endif

#define LOS_TASK_PRIORITY_HIGHEST                           0

/*=============================================================================
                                       Semaphore module configuration
=============================================================================*/

/**
 * @ingroup los_config
 * Maximum supported number of semaphores
 */
#if (LOSCFG_LIB_CONFIGURABLE == YES)
extern UINT32                                               g_uwSemLimit;
#define LOSCFG_BASE_IPC_SEM_LIMIT                           g_uwSemLimit
#else
#define LOSCFG_BASE_IPC_SEM_LIMIT                           100
#endif

/*=============================================================================
                                       Mutex module configuration
=============================================================================*/

/**
 * @ingroup los_config
 * Maximum supported number of mutexes
 */
#if (LOSCFG_LIB_CONFIGURABLE == YES)
extern UINT32                                               g_uwMuxLimit;
#define LOSCFG_BASE_IPC_MUX_LIMIT                           g_uwMuxLimit
#else
#define LOSCFG_BASE_IPC_MUX_LIMIT                           64
#endif


/*=============================================================================
                                       Queue module configuration
=============================================================================*/

/**
 * @ingroup los_config
 * Maximum supported number of queues rather than the number of usable queues
 */
#if (LOSCFG_LIB_CONFIGURABLE == YES)
extern UINT32                                               g_uwQueueLimit;
#define LOSCFG_BASE_IPC_QUEUE_LIMIT                         g_uwQueueLimit
#else
#define LOSCFG_BASE_IPC_QUEUE_LIMIT                         64
#endif

/*=============================================================================
                                       Software timer module configuration
=============================================================================*/

/**
 * @ingroup los_config
 * Maximum supported number of software timers rather than the number of usable software timers
 */
#if (LOSCFG_LIB_CONFIGURABLE == YES)
extern UINT32                                               g_uwSwtmrLimit;
#define LOSCFG_BASE_CORE_SWTMR_LIMIT                        g_uwSwtmrLimit
#else
#define LOSCFG_BASE_CORE_SWTMR_LIMIT                        80
#endif

/*=============================================================================
                                       Memory module configuration
=============================================================================*/

extern UINT8 *m_aucSysMem0;

/**
 * @ingroup los_config
 * Starting address of the memory
 */
#define OS_SYS_MEM_ADDR                                     (hal_sectors_ram_heap_start)

/**
 * @ingroup los_config
 * Ending address of the memory
 */
extern UINT32 g_sys_mem_addr_end;

/**
 * @ingroup los_config
 * Memory size
 */
#define OS_SYS_MEM_SIZE                                     (hal_sectors_ram_heap_size)


#define LOSCFG_MEMORY_BESTFIT                               YES

/**
 * @ingroup los_config
 * Configuration module tailoring of membox
 */

#define LOSCFG_MEMBOX                                       NO

/**
 * @ingroup los_config
 * Configuration module tailoring of more mempry pool checking
 */
#define LOSCFG_MEM_MUL_POOL                                 NO

/**
 * @ingroup los_config
 * Configuration module tailoring of slab memory
 */
#define LOSCFG_KERNEL_MEM_SLAB                              NO

/**
 * @ingroup los_config
 * Configuration module momery water line
 */
#define LOSCFG_MEM_WATERLINE                                YES

/**
 * @ingroup los_config
 * Configuration module tailoring of mem node size checking
 */
#define LOSCFG_BASE_MEM_NODE_SIZE_CHECK                     NO


/*=============================================================================
                                        Exception module configuration
=============================================================================*/

/**
 * @ingroup los_config
 * Configuration item for exception tailoring
 */
#define LOSCFG_PLATFORM_EXC                                 YES

/**
 * @ingroup los_config
 * Configuration item for saving exception tailoring
 */
#define LOSCFG_SAVE_EXC_INFO                                NO

/*=============================================================================
                                       Runstop module configuration
=============================================================================*/

/**
 * @ingroup los_config
 * Configuration item for runstop module tailoring
 */
#define LOSCFG_KERNEL_RUNSTOP                               YES


/*=============================================================================
                                        track configuration
=============================================================================*/

/**
 * @ingroup los_config
 * Configuration item for track
 */
#define LOSCFG_BASE_MISC_TRACK                          YES  


/**
 * @ingroup los_config
 * Max count of track items
 */
#define LOSCFG_BASE_MISC_TRACK_MAX_COUNT                    1000


/*=============================================================================
                                        CPUP configuration
=============================================================================*/

/**
 * @ingroup los_config
 * Configuration item for CPU task usage tailoring
 */
#define LOSCFG_BASE_CORE_CPUP                               YES
/**
 * @ingroup los_config
 * Configuration item for CPU hwi usage tailoring
 */
#if (LOSCFG_BASE_CORE_CPUP == YES)
#define LOSCFG_BASE_CORE_CPUP_HWI                           YES
#endif

/**
 * @ingroup los_config
 * Configuration item for task (stack) monitoring module tailoring
 */
#define LOSCFG_BASE_CORE_TSK_MONITOR                        YES

/*=============================================================================
                                        Workqueue module configuration
=============================================================================*/

/**
 * @ingroup los_config
 * Configuration item for workqueue priority
 */
#if (LOSCFG_LIB_CONFIGURABLE == YES)
extern UINT32                                           g_uwWorkQueuePri;
#define LOSCFG_BASE_WORKQUEUE_TASK_PRIO                 g_uwWorkQueuePri
#else
#define LOSCFG_BASE_WORKQUEUE_TASK_PRIO                 2
#endif

/**
 * @ingroup los_config
 * Configuration item for workqueue task stack size
 */
 #if (LOSCFG_LIB_CONFIGURABLE == YES)
extern UINT32                                                 g_uwWorkQueueStackSize;
#define LOSCFG_BASE_WORKQUEUE_TASK_STACK_SIZE                 g_uwWorkQueueStackSize
#else
#define LOSCFG_BASE_WORKQUEUE_TASK_STACK_SIZE                 (0x400UL)
#endif

#define LOSCFG_REMAP_TSK_PRIO                            LOS_TASK_PRIORITY_HIGHEST

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


#endif /* _TARGET_CONFIG_H */
