/*
 * Copyright (c) CompanyNameMagicTag 2012-2018. All rights reserved.
 * Description: Secondary development APIs for power failure detection of the PLC protocol stack.
 * Author: CompanyName
 * Create: 2012-07-16
 */

/** @defgroup power_failure report
 * @ingroup mid
 */

#ifndef __SOC_MDM_POWER_FAILURE_H__
#define __SOC_MDM_POWER_FAILURE_H__

#include <soc_types.h>
#include "soc_mdm_types.h"
#include "soc_config.h"
#ifdef __cplusplus
EXT_EXTERN "C"
{
#endif

    /**
     * @ingroup  power_failure
     * @brief    start power_failure check
     *
     * @par Description:
     * start power_failure check
     *
     * @attention
     * @li The support product type is STA.
     * @param  None
     *
     * @retval None
     * @par Dependency
     * @li soc_mdm_power_failure.h Describes the external APIs of power_failure check.
     * @see None
     * @since DW21_V100R001C00
     */
    td_void uapi_start_power_failure_check(td_void);

    /**
     * @ingroup  power_failure
     * @brief    Obtain whether power_failure is finished.
     *
     * @par  Description:
     * Obtain whether power_failure is finished.
     *
     * @attention    @li The support product type is STA.
     * @li Before use this APIs, need call uapi_start_power_failure_check() first.
     * @param  None
     *
     * @retval ret Type #td_bool
     * @li TD_TRUE power_failure have finished.
     * @li TD_FALSE power_failure have not finish.
     * @par Dependency:
     * @li soc_mdm_power_failure.h Describes the external APIs of power_failure check.
     * @see None
     * @since DW21_V100R001C00
     */
    td_bool uapi_is_check_finish(td_void);

    /**
     * @ingroup  power_failure
     * @brief    Obtain the result of power failure check.
     *
     * @par  Description:
     * Obtain the result of power failure check.
     *
     * @attention    @li The support product type is STA.
     * @li Before use this APIs, need call uapi_start_power_failure_check() first.
     * @param  None
     *
     * @retval ret #td_bool
     * @li TD_TRUE power failure status.
     * @li TD_FALSE not power failure status.
     * @par Dependency:
     * @li soc_mdm_power_failure.hDescribes the external APIs of power_failure check.
     * @see None.
     * @since DW21_V100R001C00
     */
    td_bool uapi_get_power_failure(td_void);

    /**
     * @ingroup  power_failure
     * @brief    Set power_failure state of protocol stack.
     *
     * @par Description:
     * Set power_failure state of protocol stack.
     *
     * @attention
     * @li The support product type is STA.
     * @param  is_power_failure [IN]type #td_bool :
     * @li TD_TRUE : Enter the power_failure state.
     * @li TD_FALSE: Exit the power_failure state.
     *
     * @retval None
     * @par Dependency:
     * @li soc_mdm_power_failure.h Describes the external APIs of power_failure check.
     * @see None.
     * @since DW21_V100R001C00
     */
    td_void uapi_set_power_failure_state(td_bool is_power_failure);

    /**
     * @ingroup  power_failure
     * @brief    Obtain the network state.
     *
     * @par Dependency:
     * Obtain the network state.
     *
     * @attention None
     * @param  None
     *
     * @retval ret #td_bool
     * @li TD_TRUE Have join the network
     * @li TD_FALSE Not join the network
     * @par Dependency:
     * @li soc_mdm_power_failure.h Describes the external APIs of power_failure check.
     * @see None.
     * @since DW21_V100R001C00
     */
    td_bool uapi_get_join_net_state(td_void);

#ifdef __cplusplus
}
#endif
#endif
