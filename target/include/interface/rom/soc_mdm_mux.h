/**
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * @defgroup hct_mux Mutex
 * @ingroup osa
 */

#ifndef __SOC_MDM_MUX_H__
#define __SOC_MDM_MUX_H__
#include <soc_types_rom.h>

/**
 * @ingroup  hct_mux
 * @brief  Creates a mutex.
 *
 * @par Description:
 * Creates a mutex.
 *
 * @attention None
 * @param  handle  [OUT] Type #td_u32* Mutex handle
 * @param  name    [IN]  Type #td_char Name of the mutex
 *
 * @retval #0      Success
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_mux.h: Describes mutex APIs.
 * @see  uapi_mux_delete
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_mux_create(td_u32 *handle, td_char name[8]); /* 8 len */

/**
 * @ingroup  hct_mux
 * @brief  Deletes a mutex.
 *
 * @par Description:
 * Deletes a mutex.
 *
 * @attention None
 * @param  handle  [IN] Type #td_u32 Mutex handle
 *
 * @retval #0      Success
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_mux.h: Describes mutex APIs.
 * @see  uapi_mux_create
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_mux_delete(td_u32 handle);

/**
 * @ingroup  hct_mux
 * @brief  Waits for a mutex.
 *
 * @par Description:
 * Waits for a mutex.
 *
 * @attention Mutexes support priority inversion.
 * @param  handle     [IN] Type #td_u32 Mutex handle
 * @param  timeout_ms [IN] Type #td_u32 Timeout period (unit: ms). EXT_SYS_WAIT_FOREVER indicates permanent waiting.
 *
 * @retval #0         Success
 * @retval #Other values       Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_mux.h: Describes mutex APIs.
 * @see  uapi_mux_post
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_mux_pend(td_u32 handle, td_u32 timeout_ms);

/**
 * @ingroup  hct_mux
 * @brief  Releases a mutex.
 *
 * @par Description:
 * Releases a mutex.
 *
 * @attention A mutex can be released only in the task that has obtained the mutex.
 * @param  handle  [IN] Type #td_u32 Mutex handle
 *
 * @retval #0      Success
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_mux.h: Describes mutex APIs.
 * @see  uapi_mux_pend
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_mux_post(td_u32 handle);

#endif
