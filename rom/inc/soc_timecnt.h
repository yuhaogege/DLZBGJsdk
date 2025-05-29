
#ifndef __SOC_TIMECNT_H__
#define __SOC_TIMECNT_H__
#include <soc_types_rom.h>
#include <soc_mdm_timecnt.h>

/**
* @ingroup  hct_timecnt
* @brief  Slow TIME_CNT enable.
*
* @par Description:
*           Slow TIME_CNT enable.
*
* @attention None.
* @param  None.
*
* @retval None.
* @par Dependency:
*            @li soc_mdm_timecnt.h£ºThis file describes the interfaces related to the TIME_CNT.
* @see  None.
* @since DW21_V100R001C00
 */
EXT_EXTERN td_void uapi_low_accuracy_time_counter_enable(td_void);

/**
* @ingroup  hct_timecnt
* @brief  Disables slow TIME_CNT.
*
* @par Description:
*           Disables slow TIME_CNT.
*
* @attention None.
* @param  None.
*
* @retval None.
* @par Dependency:
*            @li soc_mdm_timecnt.h£ºThis file describes the interfaces related to the TIME_CNT.
* @see  None.
* @since DW21_V100R001C00
 */
EXT_EXTERN td_void uapi_low_accuracy_time_counter_disable(td_void);

/**
* @ingroup  hct_timecnt
* @brief  Obtains the running time of the slow TIME_CNT (unit: ¦Ìs, ms, or s).
*
* @par Description:
*           Obtains the running time of the slow TIME_CNT (unit: ¦Ìs, ms, or s).
*
* @attention The interval between two consecutive invocations must be greater 
*                  than or equal to 50 us. Otherwise, TIME_CNT cannot be used.
* @param  p_us  [OUT] Type #td_u32*, unit: ¦Ìs.
* @param  p_ms  [OUT] Type #td_u32*, unit: ms.
* @param  p_sec [OUT] Type #td_u32*, unit: s.
*
* @retval #0    Success.
* @retval #A non-zero value indicates a failure. For details, see soc_errno.h.
* @par Dependency:
*            @li soc_mdm_timecnt.h£ºThis file describes the interfaces related to the TIME_CNT.
* @see  None.
* @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_low_accuracy_time_counter_gettime(td_u32 *p_us, td_u32 *p_ms, td_u32 *p_sec);

/**
* @ingroup  hct_timecnt
* @brief   Obtains the running time of the slow TIME_CNT (unit: s).
*
* @par Description:  
*           Obtains the running time of the slow TIME_CNT (unit: s).
*
* @attention The interval between two consecutive invocations must 
*                  be greater than or equal to 50 us. Otherwise, TIME_CNT cannot be used.
* @param  p_sec  [OUT] Type #td_u32*, unit: s.
*
* @retval #0     Success.
* @retval #A non-zero value indicates a failure. For details, see soc_errno.h.
* @par Dependency:
*            @li soc_mdm_timecnt.h£ºThis file describes the interfaces related to the TIME_CNT.
* @see  None.
* @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_low_accuracy_time_counter_getsec(td_u32 *p_sec);

/**
* @ingroup  hct_timecnt
* @brief  Obtains the running time of the slow TIME_CNT (unit: ms).
*
* @par Description:
*           Obtains the running time of the slow TIME_CNT (unit: ms).
*
* @attention The interval between two consecutive invocations must 
*                  be greater than or equal to 50 us. Otherwise, TIME_CNT cannot be used.
* @param  p_ms   [[OUT] Type #td_u32*, in ms.
*
* @retval #0     Success.
* @retval #A non-zero value indicates a failure. For details, see soc_errno.h.
* @par Dependency:
*            @li soc_mdm_timecnt.h£ºThis file describes the interfaces related to the TIME_CNT.
* @see  None.
* @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_low_accuracy_time_counter_getms(td_u32 *p_ms);

/**
* @ingroup  hct_timecnt
* @brief     Obtains the running time of the slow TIME_CNT (unit: us).
*
* @par Description:
*           Obtains the running time of the slow TIME_CNT (unit: us).
*
* @attention The interval between two consecutive invocations must 
*                  be greater than or equal to 50 us. Otherwise, TIME_CNT cannot be used.
* @param  p_us  [OUT] Type #td_u32*, unit: us.
*
* @retval #0    Success.
* @retval #A non-zero value indicates a failure. For details, see soc_errno.h.
* @par Dependency:
*            @li soc_mdm_timecnt.h£ºThis file describes the interfaces related to the TIME_CNT.
* @see  None.
* @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_low_accuracy_time_counter_getus(td_u32 *p_us);

#endif

