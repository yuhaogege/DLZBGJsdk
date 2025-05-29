/*
 * Copyright (c) CompanyNameMagicTag 2012-2018. All rights reserved.
 * Description: secondary development APIs for phase identification of the PLC protocol stack
 * Author: CompanyName
 * Create: 2012-07-16
 */

/** @defgroup phase_identify  Phase identification
 * @ingroup mid
 */
#ifndef __SOC_MDM_PI_H__
#define __SOC_MDM_PI_H__
#include "soc_types.h"

#ifdef __cplusplus
EXT_EXTERN "C"
{
#endif

    /**
     * @ingroup phase_identify
     * @brief    Loads the phase identification module.
     *
     * @par  Description:
     * Loads the phase identification module.
     *
     * @attention
     * @li This API can be called only once, and the product type is CCO or STA.
     * @li This API can be called only after the component is initialized, that is, after uapi_mac_cp_init is called.
     *
     * @param  None
     *
     * @retval None
     * @par Dependency:
     * @li soc_mdm_pi.h: This file describes the external APIs for phase identification.
     * @see None
     * @since DW21_V100R001C00
     */
    td_void uapi_load_pi(td_void);

/**
 * @ingroup phase_identify
 * Phase identification status: unknown
 */
#define EXT_MAC_PHASE_IDENTIFY_STATE_UNKNOWN 0
/**
 * @ingroup phase_identify
 * Phase identification status: identification not supported
 */
#define EXT_MAC_PHASE_IDENTIFY_STATE_NOT_SUPPORT 1
/**
 * @ingroup phase_identify
 * Phase identification status: identification in progress
 */
#define EXT_MAC_PHASE_IDENTIFY_STATE_DOING 2
/**
 * @ingroup phase_identify
 * Phase identification status: identification completed
 */
#define EXT_MAC_PHASE_IDENTIFY_STATE_OVER 3

    /**
     * @ingroup phase_identify
     * @brief   Obtains the phase recognition result table.
     *
     * @par  Description:
     * Obtains the phase recognition result table. The phase identification result of each STA occupies one
     * byte, and the terminal equipment identifier (TEI) of an STA indicates a table entry index. For example,
     * if the TEI of the CCO is 1, the phase identification result of the CCO
     * is stored in entry 1, while entry 0 is idle. The entry storage structure is as follows:
     * bit[0:1], phase identification result. PHASE_A indicates phase A, PHASE_B indicates phase B, and PHASE_C
     * indicates phase C.
     * bit[2:3], phase identification status. EXT_MAC_PHASE_IDENTIFY_STATE_UNKNOWN indicates that the phase
     * identification state is unknown. EXT_MAC_PHASE_IDENTIFY_STATE_NOT_SUPPORT indicates that the phase
     * identification is not supported.
     * EXT_MAC_PHASE_IDENTIFY_STATE_DOING indicates that the phase identification is going on.
     * EXT_MAC_PHASE_IDENTIFY_STATE_OVER indicates that the phase recognition ends.
     * @attention
     * @li The product type is CCO.
     *
     * @param  None
     *
     * @retval #td_u8 * Address of the phase identification result table
     * @par Dependency:
     * @li soc_mdm_pi.h: This file describes the external APIs for phase identification.
     * @see None
     * @since DW21_V100R001C00
     */
    td_u8 *uapi_get_phase_result_table(td_void);

#ifdef __cplusplus
}
#endif
#endif
