/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: secondary development APIs of the transport layer
 * Author: CompanyName
 * Create: 2018-10-31
 */
/** @defgroup transfer  Transfer
 * @ingroup mid
 */

#ifndef __SOC_MDM_TL_H__
#define __SOC_MDM_TL_H__

#include <soc_list.h>
#include <soc_types.h>
#include "soc_mdm_types.h"
#include "soc_config.h"

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @ingroup transfer
 * Configuration parameters of the transport layer
 */
typedef struct {
    td_u8 send_num; /* Number of send */
    td_u8 pad[3];   /* Send window size, reserved 3 bytes for 4-byte alignment */

    td_u32 retry_timeout;  /* Retry timeout period (unit: ms) */
    td_u32 rcv_timeout;    /* Receive timeout period (unit: ms) */
    td_u16 snd_interval;   /* Send interval (unit: ms) */
    td_u16 delay_ack_time; /* ACK message delay (unit: ms) */
} ext_tl_param_st;
/**
 * @ingroup transfer
 * Buffer data format
 */
typedef struct {
    td_u8 *buf;
    td_u16 buf_len;
    td_u8 send_ack_mode; /* only use low 4 bits */
    td_u8 pad;           /* reserved */
} ext_tl_buf_info_st;
/**
 * @ingroup transfer
 * Data transmission format of the transport layer
 */
typedef struct {
    ext_tl_buf_info_st data_info;
    td_u8 oda[EXT_PLC_MAC_ADDR_LEN];
    td_u8 app_head_len;
    td_u8 pad;
    td_u8 *app_head;
} ext_tl_send_info;
/**
 * @ingroup transfer
 * @brief  Defines the function for sending fragmented packets at the transport layer.
 *
 * @par  Description:
 * Defines the function for sending fragmented packets at the transport layer.
 *
 * @attention None
 * @param  send_info     [IN]  Type #ext_tl_send_info *, send data.
 *
 * @retval   #td_u32
 * @par Dependency:
 * @li soc_mdm_tl.h: This file describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
typedef td_u32 (*tl_send_fun)(EXT_CONST ext_tl_send_info *send_info);
/**
 * @ingroup transfer
 * @brief  Load of the transport layer
 *
 * @par  Description:
 * Component load of the transport layer
 *
 * @attention
 * @li This function can be called only once.
 * @li This API can be called only after the component is initialized, that is, after uapi_mac_cp_init is called.
 * @param  queue_size [IN] Type #td_u8, queue size. This parameter must be set based on the actual requirements.
 * The value must be less than or equal to 32 and not equal to 0.
 * @retval #EXT_ERR_SUCCESS Success
 * @retval #Other values Failure
 * @par Dependency:
 * @li soc_mdm_tl.h: This file describes the external APIs of the link layer.
 * @since DW21_V100R001C00
 */
td_u32 uapi_load_tl(td_u8 queue_size);

/**
 * @ingroup transfer
 * @brief  Creates a transport layer handle.
 *
 * @par  Description:
 * Creates a transport layer handle.
 *
 * @attention Only one handle needs to be created by an app.
 * @param  tl_id [OUT] Type #td_u32*, transport layer handle created by the corresponding app
 * @param  tl_send_func [IN] Type #tl_send_func, fragment packet sending API. The buffer send_info->data_info.buf does
 * not need to be released by the app.
 * @param  tl_mss [IN] Type #td_u16, maximum segment length. This parameter must be set based on the actual
 * requirements. The value must be greater than 5.
 * @param  snd_window [IN] Type #td_u8, size of the send window. The value cannot be greater than 5.
 * @retval #EXT_ERR_SUCCESS Success
 * @retval #Other values Failure
 * @par Dependency:
 * @li soc_mdm_tl.h: This file describes the external APIs of the link layer.
 * @since DW21_V100R001C00
 */
td_u32 uapi_tl_create_transport_layer(td_u32 *tl_id, tl_send_fun tl_send_func, td_u16 tl_mss, td_u8 snd_window);
/**
 * @ingroup transfer
 * @brief  Deletes the transport layer handle.
 *
 * @par  Description:
 * Deletes the transport layer handle.
 *
 * @param  tl_id [IN] Type #td_u32, transport layer handle created by the corresponding app
 * @retval #EXT_ERR_SUCCESS Success
 * @retval #Other values Failure
 * @par Dependency:
 * @li soc_mdm_tl.h: This file describes the external APIs of the link layer.
 * @since DW21_V100R001C00
 */
td_u32 uapi_tl_delete_transport_layer(td_u32 tl_id);
/**
 * @ingroup transfer
 * @brief  Sets the transport layer parameters.
 *
 * @par  Description:
 * Sets the transport layer parameters.
 *
 * @attention The receive timeout period must be greater than the total retry timeout period and not more than one hour.
 * No parameter value can be 0.
 * @param  tl_id [IN] Type #td_u32, transport layer handle created by the corresponding app
 * @param  trans_param [IN] Type #ext_tl_param_st, parameter setting information
 * @retval #EXT_ERR_SUCCESS Success
 * @retval #Other values Failure
 * @par Dependency:
 * @li soc_mdm_tl.h: This file describes the external APIs of the link layer.
 * @since DW21_V100R001C00
 */
td_u32 uapi_tl_set_transport_layer_param(td_u32 tl_id, EXT_CONST ext_tl_param_st *trans_param);
/**
 * @ingroup transfer
 * @brief  TX interface of the transport layer
 *
 * @par  Description:
 * TX interface of the transport layer
 *
 * @attention Fragment + Transmission header
 * @attention send_info->data_info.buf requires the caller to apply for dynamic memory, which is released by the
 * transport layer. The return value EXT_ERR_NO_INITILIZATION is excluded.
 * @attention send_info->app_head determines whether the dynamic memory exists based on the actual requirements.
 * If yes, the caller needs to apply for dynamic memory, which is released by the transport layer.
 * The return value EXT_ERR_NO_INITILIZATION is excluded.
 * @param  tl_id [IN] Type #td_u32, transport layer handle created by the corresponding app
 * @param  send_info [IN] Type #ext_tl_send_info *, send information.
 * @retval #EXT_ERR_SUCCESS  Success.
 * @retval #Other values  Failure.
 * @par Dependency:
 * @li soc_mdm_tl.h: This file describes the external APIs of the link layer.
 * @since DW21_V100R001C00
 */
td_u32 uapi_tl_send_packet(td_u32 tl_id, EXT_CONST ext_tl_send_info *send_info);
/**
* @ingroup transfer
* @brief  RX interface of the transport layer
*
* @par  Description:
*           RX interface of the transport layer
*
* @attention Reassembly + de-transmission layer header
* @attention The data memory for fragment_info->buf is dynamically applied by the app and released by the transport
*            layer. The return value EXT_ERR_NO_INITILIZATION is excluded.
* @param  tl_id [IN] Type #td_u32, transport layer handle created by the corresponding app
* @param  fragment_info [IN] Type #ext_tl_buf_info_st, received buffer information
* @param  src_mac [IN] Type #td_u8 *, MAC address of the buffer source
* @param  src_mac_len [IN] Type #td_u8 *, length of the MAC address
* @param  out_buf [OUT] Type #ext_tl_buf_info_st *, the reassembled buffer information is output. When the return value
*         is EXT_ERR_SUCCESS, out_buf->buf is dynamically applied by the transport layer and released by the caller.

* @retval #EXT_ERR_SUCCESS  Success.
* @retval #EXT_ERR_CONTINUE  Waiting for reception.
* @retval #Other values  Error
* @par Dependency:
*            @li soc_mdm_tl.h: This file describes the external APIs of the link layer.
* @since DW21_V100R001C00
*/
td_u32 uapi_tl_rcv_packet(td_u32 tl_id, ext_tl_buf_info_st *fragment_info, EXT_CONST td_u8 *src_mac, td_u8 src_mac_len,
    ext_tl_buf_info_st *out_buf);

#ifdef __cplusplus
}
#endif

#endif
