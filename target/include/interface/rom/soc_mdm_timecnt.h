/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * @defgroup hct_timecnt  TIME_CNT
 * @ingroup drivers
 */

#ifndef __SOC_MDM_TIMECNT_H__
#define __SOC_MDM_TIMECNT_H__

/**
 * @ingroup  hct_timecnt
 * @brief  TIME_CNT enable
 *
 * @par Description:
 * TIME_CNT enable. When TIME_CNT is enabled for the first time or it is disabled and enabled again, the count starts
 * from zero.
 *
 * @attention None
 * @param  None
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_timecnt.h: This file describes the TIME_CNT APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_void uapi_time_counter_enable(td_void);

/**
 * @ingroup  hct_timecnt
 * @brief  Disables the TIME_CNT module.
 *
 * @par Description:
 * Disables the TIME_CNT module.
 *
 * @attention None
 * @param  None
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_timecnt.h: This file describes the TIME_CNT APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_void uapi_time_counter_disable(td_void);

/**
 * @ingroup  hct_timecnt
 * @brief  Obtains the running time of the TIME_CNT (unit: s, ms, or us).
 *
 * @par Description:
 * Obtains the running time of the TIME_CNT (unit: s, ms, or us).
 *
 * @attention None
 * @param  p_us   [OUT] Type #td_u32*, unit: microsecond
 * @param  p_ms   [OUT] Type #td_u32*, unit: millisecond
 * @param  p_sec  [OUT] Type #td_u32*, unit: second
 *
 * @retval #0     Success.
 * @retval #Other values   Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_timecnt.h: This file describes the TIME_CNT APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_time_counter_gettime(td_u32 *p_us, td_u32 *p_ms, td_u32 *p_sec);

/**
 * @ingroup  hct_timecnt
 * @brief   Obtains the running time of the TIME_CNT (unit: second)
 *
 * @par Description:
 * Obtains the running time of the TIME_CNT (unit: second)
 *
 * @attention None
 * @param  p_sec [OUT] Type #td_u32*, unit: second
 *
 * @retval #0    Success.
 * @retval #Other values  Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_timecnt.h: This file describes the TIME_CNT APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_time_counter_getsec(td_u32 *p_sec);

/**
 * @ingroup  hct_timecnt
 * @brief  Obtains the running time of the TIME_CNT (unit: millisecond)
 *
 * @par Description:
 * Obtains the running time of the TIME_CNT (unit: millisecond)
 *
 * @attention None
 * @param  p_ms  [OUT] Type #td_u32*, unit: millisecond
 *
 * @retval #0    Success.
 * @retval #Other values  Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_timecnt.h: This file describes the TIME_CNT APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_time_counter_getms(td_u32 *p_ms);

/**
 * @ingroup  hct_timecnt
 * @brief     Obtains the running time of the TIME_CNT (unit: microsecond)
 *
 * @par Description:
 * Obtains the running time of the TIME_CNT (unit: microsecond)
 *
 * @attention None
 * @param  p_us   [OUT] Type #td_u32*, unit: microsecond
 *
 * @retval #0     Success.
 * @retval #Other values   Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_timecnt.h: This file describes the TIME_CNT APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_time_counter_getus(td_u32 *p_us);

#endif
