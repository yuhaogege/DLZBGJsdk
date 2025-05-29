/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Secondary development APIs for AD data acquisition control
 * of the PLC protocol stack. The APIs are used mainly for phase identification
 * and power failure detection.
 * Author: CompanyName
 * Create: 2018-10-31
 */

/**
 * @defgroup adc_ctrl  AD data acquisition control secondary development APIs
 * @ingroup mid
 */

#ifndef __SOC_MDM_ADC_CONTROL_H__
#define __SOC_MDM_ADC_CONTROL_H__

#include "soc_types.h"
#include "soc_mdm_adc.h"
#include "soc_list.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup  adc_ctrl
 * @brief    Initializes the data acquisition control module.
 *
 * @par  Description:
 * Initializes the parameter control module.
 *
 * @attention
 * @li This API can be called only once.
 * @param  None
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_adc_control.h: Describes the external APIs of phase identification.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_adc_control_init(td_void);

/**
 * @ingroup adc_ctrl
 * @brief   Sets the AD data acquisition channels.
 *
 * @par  Description:
 * Sets the AD data acquisition channels.
 *
 * @attention
 * @li Only two channels are supported.
 *
 * @param  channel [IN] Type #ext_adc_channel_index_en Channels.
 * @param  channel_num [IN] Type #td_u8 Number of AD channels.
 * @retval ret Type #td_u32
 * @li #EXT_ERR_SUCCESS: Channel setting succeeded.
 * @li #EXT_ERR_INVALID_PARAMETER: Invalid parameter
 * @par Dependency:
 * @li soc_mdm_adc_control.h: Describes the external APIs for data acquisition control.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_adc_set_channel(EXT_CONST ext_adc_channel_index_en *channel, td_u8 channel_num);

/**
 * @ingroup adc_ctrl
 * @brief   Sets the self-check parameters of the AD channel.
 *
 * @par  Description:
 * Sets the self-check parameters of the AD channel.
 *
 * @param self_check_collect_cycle    [IN] Type #td_u16 AD data acquisition cycle, AD value acquisition frequency (ms)
 * @param self_check_collect_duration [IN] Type #td_u16 Duration of the AD data acquisition self-check of a channel(ms)
 * @param power_failure_ad_diff_th    [IN] Type #td_u16 AD difference threshold for power failure self-check
 * @retval ret Type #td_u32
 * @li #EXT_ERR_SUCCESS: Channel setting succeeded.
 * @li #EXT_ERR_INVALID_PARAMETER: Invalid parameter
 * @par Dependency:
 * @li soc_mdm_adc_control.h: Describes the external APIs for data acquisition control.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_adc_set_self_check_para(td_u16 self_check_collect_cycle, td_u16 self_check_collect_duration,
    td_u16 power_failure_ad_diff_th);

/**
 * @ingroup adc_ctrl
 * @brief  Data acquisition handling function registered with the AD data acquisition control by
 * the phase identification module or the power failure detection module.
 *
 * @par  Description:
 * Data acquisition handling function registered with the AD data acquisition control by
 * the phase identification module or the power failure detection module.
 *
 * @attention
 * @li This API is executed in an interrupt and needs to be prefixed with __hot.
 * @param  ad  [IN] Type #td_u16 AD value
 * @param  ntb [IN] Type #td_u32 NTB value at the acquisition time
 * @retval  None
 * @par Dependency:
 * @li soc_mdm_adc_control.h: Describes the external APIs for data acquisition control.
 * @see None
 * @since DW21_V100R001C00
 */
typedef td_void (*ext_ad_collect_fun)(td_u16 ad, td_u32 ntb);

/**
 * @ingroup adc_ctrl
 * ad acquisition structure.
 */
typedef struct {
    ext_list node;

    td_u8 collect_state;             /**< Acquisition status.
                                      * 0: not started;
                                      * 1: acquiring;
                                      * 2: acquisition succeeded;
                                      * 3: acquisition failed.
                                      */
    td_bool auto_self_check;         /**< Whether to perform self-check before data acquisition.
                                      * TD_TRUE: yes;
                                      * TD_FALSE: no.
                                      */
    td_u16 collect_cycle;            /**< Acquisition cycle. */
    ext_adc_channel_index_en channel; /**< Acquisition channel. */
    ext_ad_collect_fun ad_collect_if; /**< Acquisition handling function.
                                      * It is executed in an interrupt and needs to be prefixed with __hot.
                                      */
} ad_collect_task_st;

/**
 * @ingroup adc_ctrl
 * Minimum cycle of AD data acquisition
 */
#define AD_MIN_COLLECT_CYCLE 50

/**
 * @ingroup adc_ctrl
 * Acquisition not started
 */
#define AD_NOT_COLLECT 0

/**
 * @ingroup adc_ctrl
 * Acquiring.
 */
#define AD_COLLECTING 1

/**
 * @ingroup adc_ctrl
 * Acquisition succeeded.
 */
#define AD_COLLECT_SUCCESS 2

/**
 * @ingroup adc_ctrl
 * Acquisition failed.
 */
#define AD_COLLECT_FAIL 3

/**
 * @ingroup adc_ctrl
 * 2-channel AD data acquisition
 */
#define AD_COLLECT_ENTRY_NUM 1

/**
 * @ingroup adc_ctrl
 * Self-check acquisition cycle (unit: ms)
 */
#define AD_SELF_CHECK_COLLECT_CYCLE 500

/**
 * @ingroup adc_ctrl
 * Self-check acquisition duration (unit: ms)
 */
#define AD_SELF_CHECK_COLLECT_DURATION 80

/**
 * @ingroup adc_ctrl
 * AD difference threshold for channel power failure
 */
#define AD_POWER_FAILURE_DIFF_TH 2000

/**
 * @ingroup adc_ctrl
 * @brief   Adds an AD data acquisition task.
 *
 * @par  Description:
 * Adds an AD data acquisition task.
 *
 * @attention
 * @li Phase identification data acquisition and power failure detection data acquisition must be performed together.
 * @li Semaphore is used in the API. The API cannot be executed in an interrupt.
 * @li If self-check before the acquisition task is required, the data acquisition API parameter can be set to null.
 * After the self-check is complete, the acquisition task is automatically stopped.
 *
 * @param  ad_collect_task [IN] Type #ad_collect_task_st* AD data acquisition handling function
 * @retval ret Type #td_u32
 * @li #EXT_ERR_SUCCESS: Registration succeeded
 * @li #EXT_ERR_INVALID_PARAMETER: Invalid parameter
 * @par Dependency:
 * @li soc_mdm_adc_control.h: Describes the external APIs for data acquisition control.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_start_collect_data(ad_collect_task_st *ad_collect_task);

/**
 * @ingroup adc_ctrl
 * @brief   Stops an AD data acquisition task.
 *
 * @par  Description:
 * Stops an AD data acquisition task.
 *
 * @attention
 * @li Phase identification data acquisition and power failure detection data acquisition must be performed together.
 * @li Semaphore is used in the API. The API cannot be executed in an interrupt.
 *
 * @param  ad_collect_task [IN] Type #ad_collect_task_st* AD data acquisition handling function
 * @retval ret Type #td_u32
 * @li #EXT_ERR_SUCCESS: Registration succeeded
 * @li #EXT_ERR_INVALID_PARAMETER: Invalid parameter
 * @par Dependency:
 * @li soc_mdm_adc_control.h: Describes the external APIs for data acquisition control.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_stop_collect_data(EXT_CONST ad_collect_task_st *ad_collect_task);

#ifdef __cplusplus
}
#endif
#endif /* _SOC_MDM_ADC_CONTROL_H_ */
