/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description:  Device management service (DMS) application channel
 * Author: CompanyName
 * Create: 2018-10-31
 */
/**
 * @defgroup hct_dms Device management service (DMS) application channel
 * @ingroup  plc
 */

#ifndef __SOC_MDM_DMS_H__
#define __SOC_MDM_DMS_H__

#include <soc_mdm_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup  hct_dms
 * @brief  Registers a data transmission channel.
 *
 * @par Description:
 * Establishes a data transmission channel between the MAC and app layers.
 *
 * @attention The port number that can be used by the SDK user is EXT_DMS_CHL_MAC_PORT_APP.
 * Other ports are used internally in the SDK.
 * @param  chl_port [IN] Type #ext_dms_chl_port_obj* Channel port object. TD_NULL indicates that the port is disabled.
 * @param  port_num [IN] Type #td_u16 Channel port. EXT_DMS_CHL_MAC_PORT_APP indicates the MAC channel corresponding
 * to the app.
 *
 * @retval #0               Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_dms.h: Describes channel APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
td_u32 uapi_dms_chl_setup(EXT_CONST ext_dms_chl_port_obj *chl_port, td_u16 port_num);

/**
 * @ingroup  hct_dms
 * @brief  Sends application packets through a registered channel.
 *
 * @par Description:
 * Sends application packets through a specified port.
 *
 * @attention For details, see the description of the parameter structure.
 * @param  port_num    [IN] Type #td_u16 Channel port.
 * @param  frame_data  [IN] Type #ext_dms_chl_tx_data* Frame data of the DMS channel.
 * @param  frame_ctrl  [IN] Type #ext_dms_chl_tx_ctrl* Frame control information of the DMS.
 *
 * @retval #0               Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_dms.h: Describes channel APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
td_u32 uapi_dms_chl_tx(td_u16 port_num,
                     EXT_IN EXT_CONST ext_dms_chl_tx_data *frame_data,
                     EXT_IN EXT_CONST ext_dms_chl_tx_ctrl *frame_ctrl);

#ifdef __cplusplus
}
#endif
#endif /* __SOC_MDM_DMS_H__ */
