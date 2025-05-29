/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: APP PLC frame handle.
 */

#include "app_plc.h"
#include "app_cmd_msg.h"
#include "app_common.h"
#include "soc_mdm_mem.h"
#include "soc_stdlib.h"
#include "soc_mdm_dms.h"
#include "soc_mdm_crc.h"
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
#include "app_trans_file.h"
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
#include "app_fast_send_demo.h"
#endif
#if defined(PRODUCT_CFG_PLC_MODULE_APP_TEST)
#include "app_plc_test.h"
#endif
#ifdef __cplusplus
extern "C" {
#endif

td_u32 app_send_plc_frame(EXT_CONST app_plc_frame_data *frame)
{
    ext_dms_chl_tx_data *tx_data = TD_NULL;
    ext_dms_chl_tx_ctrl *tx_ctrl = TD_NULL;
    td_u32 frame_length;

    if (frame == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    if (frame->payload == TD_NULL && frame->payload_length != 0) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    tx_ctrl = (ext_dms_chl_tx_addr_s *)uapi_malloc(EXT_MOD_ID_APP_COMMON, sizeof(ext_dms_chl_tx_addr_s));
    if (tx_ctrl == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }

    frame_length = sizeof(ext_dms_chl_tx_data) + frame->payload_length;
    tx_data = (ext_dms_chl_tx_data *)uapi_malloc(EXT_MOD_ID_APP_COMMON, frame_length);
    if (tx_data == TD_NULL) {
        uapi_free(EXT_MOD_ID_APP_COMMON, tx_ctrl);
        return EXT_ERR_MALLOC_FAILUE;
    }

    (td_void) memset_s(tx_ctrl, sizeof(ext_dms_chl_tx_addr_s), 0, sizeof(ext_dms_chl_tx_addr_s));
    (td_void) memset_s(tx_data, frame_length, 0, frame_length);
    if (frame->payload_length > 0 &&
        memcpy_s(tx_data->data, frame_length - sizeof(ext_dms_chl_tx_data), frame->payload,
            frame->payload_length) != EOK) {
        uapi_free(EXT_MOD_ID_APP_COMMON, tx_data);
        uapi_free(EXT_MOD_ID_APP_COMMON, tx_ctrl);
        return EXT_ERR_MEMCPY_FAIL;
    }
    tx_data->data_size = frame->payload_length;

    if (memcpy_s(tx_ctrl->dest_mac, EXT_PLC_MAC_ADDR_LEN, frame->mac, sizeof(frame->mac)) != EOK) {
        uapi_free(EXT_MOD_ID_APP_COMMON, tx_data);
        uapi_free(EXT_MOD_ID_APP_COMMON, tx_ctrl);
        return EXT_ERR_MEMCPY_FAIL;
    }
    tx_ctrl->ctrl = EXT_DMS_CHL_TX_CTRL_FORCE;
    tx_ctrl->type = EXT_DMS_CHL_TX_TYPE_MR;
    tx_ctrl->prio = APP_PLC_MODULE_PRIO;
    tx_ctrl->id = frame->id;

    uapi_dms_chl_tx(EXT_DMS_CHL_MAC_PORT_APP, tx_data, tx_ctrl);

    uapi_free(EXT_MOD_ID_APP_COMMON, tx_data);
    uapi_free(EXT_MOD_ID_APP_COMMON, tx_ctrl);

    return EXT_ERR_SUCCESS;
}
/*
static td_bool app_plc_rx_valid_check(EXT_CONST ext_dms_chl_rx *chl_rx, td_u16 data_len, td_u16 pkt_crc)
{
    td_u16 crc_16 = 0;

    if (chl_rx == TD_NULL) {
        return TD_FALSE;
    }

    if ((chl_rx->id != ID_PLC_DATA_TRANSMIT) && (chl_rx->id != ID_PLC_REMOTE_CMD)) {
        return TD_FALSE;
    }
    if (sizeof(app_plc_trans_frame) + data_len != chl_rx->payload_len) {
        return TD_FALSE;
    }
    crc_16 = uapi_crc16(crc_16, chl_rx->payload + sizeof(app_plc_trans_frame), data_len);
    crc_16 = ext_swap16(crc_16);
    if (crc_16 != pkt_crc) {
        return TD_FALSE;
    }

    return TD_TRUE;
}*/

td_u32 app_plc_chl_rx(td_u16 port, EXT_CONST ext_dms_chl_rx *chl_rx)
{
#if defined(PRODUCT_CFG_PLC_MODULE_APP_TEST)
    app_plc_test_chl_rx(port, (ext_dms_chl_rx *)chl_rx);
#endif
    app_plc_trans_frame *app_frame = TD_NULL;
    ext_sys_queue_msg msg;
    app_plc_frame_data *info = (app_plc_frame_data *)msg.param;
    td_u32 ret;

    ext_unref_param(port);
    if (chl_rx == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    app_frame = (app_plc_trans_frame *)chl_rx->payload;
    if (app_frame == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
#if defined(PRODUCT_CFG_FAST_SEND_DEMO)
    app_rcv_fast_send_plc_chl_demo_interface(port, chl_rx);
#endif
#endif
#if 0
    if (app_plc_rx_valid_check(chl_rx, app_frame->data_length, app_frame->crc_16) == TD_FALSE) {
        return EXT_ERR_INVALID_PARAMETER;
    }
#endif
    (td_void) memset_s(&msg, sizeof(msg), 0, sizeof(msg));
    msg.msg_id = ID_APP_MSG_PLC_INPUT;

    info->payload_length = app_frame->data_length;
    info->payload = (td_pbyte)uapi_malloc(EXT_MOD_ID_APP_COMMON, info->payload_length);
    if (info->payload == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }

    if (memcpy_s(info->payload, info->payload_length, app_frame->data, app_frame->data_length) != EOK) {
        uapi_free(EXT_MOD_ID_APP_COMMON, info->payload);
        info->payload = TD_NULL;
        return EXT_ERR_MEMCPY_FAIL;
    }
    (td_void) memcpy_s(info->mac, sizeof(info->mac), chl_rx->src_mac, sizeof(chl_rx->src_mac));
    info->option = app_frame->crc_16;
    info->id = chl_rx->id;

    ret = app_send_message(&msg);
    if (ret != EXT_ERR_SUCCESS) {
        uapi_free(EXT_MOD_ID_APP_COMMON, info->payload);
        info->payload = TD_NULL;
    }

    return ret;
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
static td_bool g_plc_chl_status = TD_FALSE;
td_u32 app_plc_chl_status(td_u16 port, td_u8 status)
{
    ext_sys_queue_msg msg;
    (td_void) memset_s(&msg, sizeof(msg), 0, sizeof(msg));
    msg.msg_id = ID_APP_MSG_PLC_CHL;
    msg.param[0] = (uintptr_t)status;
    msg.param[1] = (uintptr_t)port;
    return app_send_message(&msg);
}

td_u32 app_plc_chl_status_handle(EXT_CONST ext_sys_queue_msg *msg)
{
    if (msg->param[0] == EXT_DMS_CHL_CONNECT) {
        g_plc_chl_status = TD_TRUE;
        trans_file_on_plc_chl_available();
    } else {
        g_plc_chl_status = TD_FALSE;
    }

    return EXT_ERR_SUCCESS;
}

td_bool app_is_plc_available(td_void)
{
    return g_plc_chl_status;
}
#endif

#ifdef __cplusplus
}
#endif

