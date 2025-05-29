/**
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 *
 * Description: message APIs
 * @defgroup hct_sem Semaphore
 * @ingroup osa
 * @li uapi_sem_wait(): In the interrupt function, disable interrupt context, and lock task context, this API must not be
 * called. Otherwise, uncontrollable exception scheduling may result.
 * @li uapi_sem_signal(): In the disable interrupt context, this API must not be called. Otherwise, uncontrollable
 * exception scheduling may result.
 */

#ifndef __SOC_MDM_SEM_H__
#define __SOC_MDM_SEM_H__
#include <soc_types_rom.h>

#define EXT_SEM_ONE ((td_u8)1)  /**< ucInit Obtained value of the input: Critical resource protection */
#define EXT_SEM_ZERO ((td_u8)0) /**< ucInit Obtained value of the input: Synchronization */

/**
 * @ingroup  hct_sem
 * @brief  Creates a semaphore.
 *
 * @par Description:
 * Creates a semaphore.
 *
 * @attention None
 * @param  id      [OUT] Type #td_u32*, semaphore ID
 * @param  name    [IN]  Type #td_char, semaphore name
 * @param  init    [IN]  Type #td_u8, initial value. Generally, when the value is EXT_SEM_ONE, the API is used for
 * critical resource protection. When the value is EXT_SEM_ZERO, the API is used for synchronization.
 *
 * @retval #0      Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_sem.h: This file describes the semaphore APIs.
 * @see  uapi_sem_delete
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_sem_bcreate(td_u32 *id, td_char name[8], td_u8 init); /* 8 len */

/**
 * @ingroup  hct_sem
 * @brief  Deletes the semaphore.
 *
 * @par Description:
 * Deletes the semaphore.
 *
 * @attention None
 * @param  id [IN] Type #td_u32, semaphore ID
 *
 * @retval #0      Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_sem.h: This file describes the semaphore APIs.
 * @see  uapi_sem_bcreate
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_sem_delete(td_u32 id);

/**
 * @ingroup  hct_sem
 * @brief  Obtains the semaphore.
 *
 * @par Description:
 * Obtains the semaphore.
 *
 * @attention None
 * @param  id      [IN] Type #td_u32, semaphore ID
 * @param  timeout [IN] Type #td_u32, timeout period (unit: ms). EXT_SYS_WAIT_FOREVER indicates permanent wait.
 *
 * @retval #0      Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_sem.h: This file describes the semaphore APIs.
 * @see  uapi_sem_signal
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_sem_wait(td_u32 id, td_u32 timeout);

/**
 * @ingroup  hct_sem
 * @brief  Releases the semaphore.
 *
 * @par Description:
 * Releases the semaphore.
 *
 * @attention None
 * @param  id      [IN] Type #td_u32, semaphore ID
 *
 * @retval #0      Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_sem.h: This file describes the semaphore APIs.
 * @see  uapi_sem_wait
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_sem_signal(td_u32 id);

#endif
