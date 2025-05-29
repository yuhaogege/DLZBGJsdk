/**
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * @defgroup hct_task Task
 * @ingroup osa
 */
#ifndef __SOC_MDM_TASK_H__
#define __SOC_MDM_TASK_H__

#include <soc_types_rom.h>

#define EXT_INVALID_TASK_ID (0xFFFFFFFF)
/* The length of the task file name cannot be changed. */
#define EXT_TASK_NAME_LEN (32)

typedef struct {
    td_char name[EXT_TASK_NAME_LEN]; /**< Task entrance function */
    td_u32 id;                      /**< Task ID */
    td_u16 status;                  /**< Task status */
    td_u16 priority;                /**< Task priority */
    td_pvoid task_sem;              /**< Semaphore pointer */
    td_pvoid task_mutex;            /**< Mutex pointer */
    td_u32 event_stru[3];           /**< Event 3 count */
    td_u32 event_mask;              /**< Event mask */
    td_u32 stack_size;              /**< Task stack size */
    td_u32 top_of_stack;            /**< Task stack top */
    td_u32 bottom_of_stack;         /**< Task stack bottom */
    td_u32 sp;                      /**< Task SP pointer */
    td_u32 pc;                      /**< Task current PC */
    td_u32 lr;                      /**< Task LR */
    td_u32 psr;                     /**< Task CPSR */
    td_u32 curr_used;               /**< Current task stack usage */
    td_u32 peak_used;               /**< Task stack usage peak */
    td_u32 overflow_flag;           /**< Flag that indicates whether a task stack overflow occurs */
} ext_task_info_s;

/**
 * @ingroup  hct_task
 * @brief  task Body callback function
 *
 * @par Description:
 * task Body callback function
 *
 * @attention None
 * @param  param [IN] Type #uintptr_t, user parameter
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_task.h: This file describes the task APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
typedef td_void (*tsk_body)(uintptr_t param);

/**
 * @ingroup  hct_task
 * @brief  Creates a task.
 *
 * @par Description:
 * Creates a task.
 *
 * @attention
 * @li The space occupied by a task name string must be applied for by the caller and saved statically. The task name is
 * not stored internally in the API.
 * @li If the size of the specified task stack is 0, use the default size specified by #OS_TSK_DEFAULT_STACK_SIZE.
 * @li The size of the task stack should be 8-byte aligned. The principle for determining the task stack size is as
 * follows: Do not use a too large or too small task stack size (to avoid waste or overflow)
 * @li The recommended user priority should be within the range of [20, 30]. Do not use the priorities of [0, 2] and
 * [31].
 * @param  id         [OUT] Type #td_u32*, task ID
 * @param  name       [IN]  Type #td_char, task name
 * @param  task_body  [IN]  Type #TSK_BODY, task entry function
 * @param  param      [IN]  Type #uintptr_t, parameter that needs to be input to the task entry when a task is created.
 * If this parameter does not need to be input, set this parameter to 0.
 * @param  stack_size [IN]  Type #td_u32, stack size (unit: byte)
 * @param  priority   [IN]  Type #td_u32, task priority
 *
 * @retval #0         Success.
 * @retval #Other values       Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_task.h: This file describes the APIs related to tasks.
 * @see  uapi_task_delete
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_task_create(td_u32 *id, td_char name[8], tsk_body task_body, uintptr_t param, /* 8 len */
    td_u32 stack_size, td_u32 priority);

/**
 * @ingroup  hct_task
 * @brief  Deletes a task.
 *
 * @par Description:
 * Deletes a task.
 *
 * @attention
 * @li Use this API with caution. A task can be deleted only after the confirmation of the user. The idle task and
 * Swt_Task cannot be deleted.
 * @li When deleting a task, ensure that the resources (such as mutex and semaphore) applied by the task have been
 * released.
 * @param  id      [IN] Type #td_u32, task ID
 *
 * @retval #0      Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_task.h: This file describes the task APIs.
 * @see  uapi_task_create
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_task_delete(td_u32 id);

/**
 * @ingroup  hct_task
 * @brief  Suspends a task.
 *
 * @par Description:
 * Suspends a task.
 *
 * @attention
 * @li A task cannot be suspended if it is the current task and is locked.
 * @li The idle task and Swt_Task cannot be suspended.
 * @li The task cannot be blocked or suspended in the lock task status.
 * @param  id      [IN] Type #td_u32, task ID
 *
 * @retval #0      Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_task.h: This file describes the task APIs.
 * @see  uapi_task_resume
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_task_suspend(td_u32 id);

/**
 * @ingroup  hct_task
 * @brief  Resumes a task.
 *
 * @par Description:
 * Resumes a task.
 *
 * @attention None
 * @param  id      [IN] Type #td_u32, task ID
 *
 * @retval #0      Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_task.h: This file describes the task APIs.
 * @see  uapi_task_suspend
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_task_resume(td_u32 id);

/**
 * @ingroup  hct_task
 * @brief  Obtains the task priority.
 *
 * @par Description:
 * Obtains the task priority.
 *
 * @attention None
 * @param  id       [IN]  Type #td_u32, task ID
 * @param  priority [OUT] Type #td_u32*, task priority
 *
 * @retval #0       Success.
 * @retval #Other values     Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_task.h: This file describes the task APIs.
 * @see  uapi_task_set_priority
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_task_get_priority(td_u32 id, td_u32 *priority);

/**
* @ingroup  hct_task
* @brief  Sets the task priority.
*
* @par Description:
            Sets the task priority.
*
* @attention
*           @li Only the ID of the task created by the user can be configured.
*           @li The recommended user priority should be within the range of [20, 30]. Do not use the priorities of [0,
2] and [31].
*           @li Setting user priorities may affect task scheduling. The user needs to plan tasks in the SDK.
* @param  id       [IN] Type #td_u32, task ID
* @param  priority [IN] Type #td_u32, task priority. A smaller value indicates a higher priority.
*
* @retval #0       Success.
* @retval #Other values     Failure. For details, see soc_errno.h.
* @par Dependency:
*           @li soc_mdm_task.h: This file describes the task APIs.
* @see  uapi_task_get_priority
* @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_task_set_priority(td_u32 id, td_u32 priority);

/**
 * @ingroup  hct_task
 * @brief  Obtains the task information.
 *
 * @par Description:
 * Obtains the task information.
 *
 * @attention None
 * @param  id     [IN]  Type #td_u32, task ID
 * @param  inf    [OUT] Type #ext_task_info_s*, task information
 *
 * @retval #0      Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_task.h: This file describes the task APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_task_get_info(td_u32 id, ext_task_info_s *inf);

/**
 * @ingroup  hct_task
 * @brief  Obtains the current task ID.
 *
 * @par Description:
 * Obtains the current task ID.
 *
 * @attention None
 * @param  None
 *
 * @retval #td_u32  Task ID. If the task fails, #EXT_INVALID_TASK_ID is returned.
 * @par Dependency:
 * @li soc_mdm_task.h: This file describes the task APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_task_get_current_id(td_void);

/**
 * @ingroup  hct_task
 * @brief  Elevates the task priority.
 *
 * @par Description:
 * This feature is similar to the lock task function and must be used with uapi_task_pri_resume.
 *
 * @attention An API that triggers task scheduling cannot be used in the scope of effect. Otherwise, the system may
 * crash.
 * @param  None
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_task.h: This file describes the task APIs.
 * @see  uapi_task_pri_resume
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_void uapi_task_pri_elevate(td_void);

/**
 * @ingroup  hct_task
 * @brief  Restores the task priority.
 *
 * @par Description:
 * This feature is similar to the unlock task function and must be used with uapi_task_pri_elevate.
 *
 * @attention None
 * @param  None
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_task.h: This file describes the task APIs.
 * @see  uapi_task_pri_elevate
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_void uapi_task_pri_resume(td_void);

/**
 * @ingroup  hct_task
 * @brief Task sleep.
 *
 * @par Description:
 * Task sleep
 *
 * @attention
 * @li In the interrupt processing function or in the case of a lock task, the uapi_sleep operation fails.
 * @li When less than 10 ms, the input parameter value should be replaced by 10 ms. When greater than 10 ms, the input
 * parameter value should be exactly divided and then rounded-down to the nearest integer.
 * @param  ms      [IN] Type #td_u32, sleep time (unit: ms). The precision is 10 ms.
 *
 * @retval #0      Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 *
 * @par Dependency:
 * @li soc_mdm_task.h: This file describes the task APIs.
 * @see None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_sleep(td_u32 ms);

#endif
