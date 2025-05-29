/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: APP module protocol frame handle.
 */

#include "app_proto_proc.h"
#include "app_uart.h"
#include "app_res.h"
#include "app_common.h"
#include "app_proto_proc_cco.h"
#include "app_plc.h"
#include "app_cmd_msg.h"
#include "app_nv.h"
#include "app_proto_upg.h"
#include "soc_mdm_mem.h"
#include "soc_mdm_ver.h"
#include "soc_stdlib.h"
#include "soc_mdm_crc.h"
#include "soc_mdm_nv.h"
#include "soc_mdm_init.h"
#include "soc_mdm_time.h"
#include "soc_mdm_mac.h"

typedef td_u32 (*proto_proc_fn)(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length);
typedef struct {
    td_u16 id;
    td_u16 pad;
    proto_proc_fn func;
} proto_proc_func_pair;

static td_u32 app_proto_frame_dispatch(app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length);
static td_u32 app_proto_frame_tx(td_pbyte data, td_u16 data_length, app_proto_source source, td_pbyte dst_addr);
static td_u32 app_proto_get_version(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length);
static td_u32 app_proto_get_mac_addr(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length);
static td_u32 app_proto_get_dev_addr(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length);
static td_u32 app_proto_set_dev_addr(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length);
static td_u32 app_proto_reboot_module(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length);
static td_u32 app_proto_data_trans_tx(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length);
static td_u32 app_proto_data_trans_rx(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length);
static td_u32 app_proto_qry_on_power_time(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length);

td_u16 g_proto_frame_seq = 0;

proto_proc_func_pair g_proto_proc_pair[] = {
    { ID_PROTO_CMD_GET_VERSION,   0, app_proto_get_version },
    { ID_PROTO_CMD_GET_MAC_ADDR,  0, app_proto_get_mac_addr },
    { ID_PROTO_CMD_GET_DEV_ADDR,  0, app_proto_get_dev_addr },
    { ID_PROTO_CMD_SET_DEV_ADDR,  0, app_proto_set_dev_addr },
    { ID_PROTO_CMD_REBOOT_MODULE, 0, app_proto_reboot_module },
    { ID_PROTO_CMD_TRANS_FILE,    0, app_proto_trans_file },
    { ID_PROTO_CMD_QRY_ONPOWER_TIME, 0, app_proto_qry_on_power_time},

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    { ID_PROTO_CMD_GET_WLIST_NUM,  0, app_proto_get_whitelist_num },
    { ID_PROTO_CMD_GET_WLIST_INFO, 0, app_proto_get_whitelist_info },
    { ID_PROTO_CMD_ADD_WLIST_NODE, 0, app_proto_add_whitelist_node },
    { ID_PROTO_CMD_DEL_WLIST_NODE, 0, app_proto_del_whitelist_node },
    { ID_PROTO_CMD_CLEAR_WLIST,    0, app_proto_clear_whitelist },
    { ID_PROTO_CMD_SET_SELF_IDY,   0, app_proto_set_network_identify_state},
    { ID_PROTO_CMD_SET_WLIST_STATE, 0, app_proto_set_whitelist_state},
    { ID_PROTO_CMD_GET_WLIST_STATE, 0, app_proto_get_whitelist_state},

    { ID_PROTO_CMD_GET_TOPO_NUM, 0, app_proto_get_topo_num },
    { ID_PROTO_CMD_GET_TOPO_INFO, 0, app_proto_get_topo_info },
#endif

    { ID_PROTO_CMD_DATA_TRANS_TX, 0, app_proto_data_trans_tx },
    { ID_PROTO_CMD_DATA_TRANS_RX, 0, app_proto_data_trans_rx },

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    { ID_PROTO_CMD_REMOTE_CMD_TX, 0, app_proto_remote_cmd_tx },
    { ID_PROTO_CMD_REMOTE_CMD_RX, 0, app_proto_remote_cmd_rx },
#endif
};

td_u32 app_proto_frame_rx(td_pbyte data, td_u16 data_length, app_proto_source source, td_pbyte src_addr)
{
    td_u32 ret;
    td_pbyte out_buffer = TD_NULL;
    td_u16 out_length = 0;

    if (check_app_proto_frame(data, data_length) == TD_FALSE) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    ret = app_proto_frame_dispatch((app_proto_frame_head *)data, &out_buffer, &out_length);
    if (ret == EXT_ERR_SUCCESS) {
        if (out_buffer != TD_NULL) {
            (td_void) app_proto_frame_tx(out_buffer, out_length, source, src_addr);
        }
    }

    if (out_buffer != TD_NULL) {
        uapi_free(EXT_MOD_ID_APP_COMMON, out_buffer);
        out_buffer = TD_NULL;
    }

    return EXT_ERR_SUCCESS;
}

static td_u32 app_proto_frame_tx(td_pbyte data, td_u16 data_length, app_proto_source source, td_pbyte dst_addr)
{
    switch (source) {
        case APP_PROTO_SOURCE_UART:
            return app_uart_tx(data, data_length);

        case APP_PROTO_SOURCE_PLC:
            return app_proto_plc_tx(data, data_length, dst_addr, ID_PLC_REMOTE_CMD);

        default:
            break;
    }

    return EXT_ERR_NOT_SUPPORT;
}

td_u32 app_proto_plc_tx(td_pbyte data, td_u16 data_length, td_pbyte dst_addr, td_u16 id)
{
    app_plc_frame_data plc;
    app_plc_trans_frame *trans = TD_NULL;
    td_u16 buffer_length = sizeof(app_plc_trans_frame) + data_length;
    td_u32 ret;
    td_pbyte buffer;

    if (data == TD_NULL || dst_addr == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    buffer = (td_pbyte)uapi_malloc(EXT_MOD_ID_APP_COMMON, buffer_length);
    if (buffer == TD_NULL) {
        return EXT_ERR_MALLOC_FAILURE;
    }

    (td_void) memset_s(buffer, buffer_length, 0, buffer_length);

    trans = (app_plc_trans_frame *)buffer;
    trans->data_length = data_length;
    (td_void) memcpy_s(trans->data, trans->data_length, data, data_length);
    trans->crc_16 = uapi_crc16(0, trans->data, trans->data_length);
    trans->crc_16 = ext_swap16(trans->crc_16);

    plc.id = id;
    (td_void) memcpy_s(plc.mac, sizeof(plc.mac), dst_addr, EXT_PLC_MAC_ADDR_LEN);
    plc.payload_length = buffer_length;
    plc.payload = buffer;

    ret = app_send_plc_frame(&plc);

    uapi_free(EXT_MOD_ID_APP_COMMON, buffer);
    buffer = TD_NULL;

    return ret;
}

static td_u32 app_proto_frame_dispatch(app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length)
{
    td_u32 i;

    for (i = 0; i < ext_array_count(g_proto_proc_pair); i++) {
        if (frame == TD_NULL) {
            break;
        }
        if (frame->cmd == g_proto_proc_pair[i].id) {
            if (g_proto_proc_pair[i].func != TD_NULL) {
                return g_proto_proc_pair[i].func(frame, out_buffer, length);
            }
        }
    }

    return EXT_ERR_NOT_SUPPORT;
}

static td_u32 app_proto_get_version(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length)
{
    ext_mdm_user_info user_info = { 0 };
    proto_get_version_ind *ind = TD_NULL;
    td_pbyte payload;
    td_u16 payload_length = sizeof(app_proto_frame_head) + sizeof(proto_get_version_ind) + PROTO_CRC_SIZE;

    if (frame == TD_NULL || out_buffer == TD_NULL || length == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    payload = (td_pbyte)uapi_malloc(EXT_MOD_ID_APP_COMMON, payload_length);
    if (payload == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }

    (td_void) memset_s(payload, payload_length, 0, payload_length);
    (td_void) uapi_get_user_info(&user_info);
    ind = (proto_get_version_ind *)(payload + sizeof(app_proto_frame_head));
     /* use little-endian */
    (td_void) memcpy_s(ind->manu_code, sizeof(ind->manu_code), user_info.manu_code, sizeof(user_info.manu_code));
    ind->chip_type = EXT_CHIP21;
    ind->version = user_info.sw_ver;
    ind->reserved = 0;

    fill_proto_frame_fixed_info((app_proto_frame_head *)payload, frame->cmd, frame->seq,
                                sizeof(proto_get_version_ind));

    *out_buffer = payload;
    *length = payload_length;

    return EXT_ERR_SUCCESS;
}

static td_u32 app_proto_get_mac_addr(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length)
{
    proto_get_mac_addr_ind *ind = TD_NULL;
    td_pbyte payload;
    td_u16 payload_length = sizeof(app_proto_frame_head) + sizeof(proto_get_mac_addr_ind) + PROTO_CRC_SIZE;

    if (frame == TD_NULL || out_buffer == TD_NULL || length == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    payload = (td_pbyte)uapi_malloc(EXT_MOD_ID_APP_COMMON, payload_length);
    if (payload == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }

    (td_void) memset_s(payload, payload_length, 0, payload_length);
    ind = (proto_get_mac_addr_ind *)(payload + sizeof(app_proto_frame_head));
    uapi_get_local_mac_addr(ind->addr, sizeof(ind->addr));
    fill_proto_frame_fixed_info((app_proto_frame_head *)payload, frame->cmd, frame->seq,
                                sizeof(proto_get_mac_addr_ind));

    *out_buffer = payload;
    *length = payload_length;

    return EXT_ERR_SUCCESS;
}

static td_u32 app_proto_get_dev_addr(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length)
{
    app_dev_addr_inf nv;
    proto_get_dev_addr_ind *ind = TD_NULL;
    td_u32 ret;
    td_pbyte payload;
    td_u16 payload_length = sizeof(app_proto_frame_head) + sizeof(proto_get_dev_addr_ind) + PROTO_CRC_SIZE;

    if (frame == TD_NULL || out_buffer == TD_NULL || length == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    payload = (td_pbyte)uapi_malloc(EXT_MOD_ID_APP_COMMON, payload_length);
    if (payload == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }

    (td_void) memset_s(payload, payload_length, 0, payload_length);
    ind = (proto_get_dev_addr_ind *)(payload + sizeof(app_proto_frame_head));

    (td_void) memset_s(&nv, sizeof(nv), 0, sizeof(nv));
    ret = uapi_nv_read(ID_NV_APP_DEV_ADDR_INF, &nv, sizeof(nv));
    if (ret == EXT_ERR_SUCCESS) {
        (td_void) memcpy_s(ind->addr, sizeof(ind->addr), nv.dev_addr, sizeof(nv.dev_addr));
    }

    fill_proto_frame_fixed_info((app_proto_frame_head *)payload, frame->cmd, frame->seq,
                                sizeof(proto_get_dev_addr_ind));

    *out_buffer = payload;
    *length = payload_length;

    return EXT_ERR_SUCCESS;
}

static td_u32 app_proto_set_dev_addr_to_nv(EXT_CONST td_u8 addr[EXT_PLC_MAC_ADDR_LEN])
{
    td_u32 ret;
    app_dev_addr_inf nv;

    if (addr == TD_NULL) {
        return EXT_ERR_FAILURE;
    }

    (td_void) memset_s(&nv, sizeof(nv), 0, sizeof(nv));
    ret = uapi_nv_read(ID_NV_APP_DEV_ADDR_INF, &nv, sizeof(nv));
    if (ret == EXT_ERR_SUCCESS) {
        if (memcmp(nv.dev_addr, addr, EXT_PLC_MAC_ADDR_LEN) != 0) {
            (td_void) app_set_mac_addr(addr, EXT_PLC_MAC_ADDR_LEN);
            ret = (td_u32)memcpy_s(nv.dev_addr, sizeof(nv.dev_addr), addr, EXT_PLC_MAC_ADDR_LEN);
            if (ret != EXT_ERR_SUCCESS) {
                return ret;
            }
            ret = uapi_nv_write(ID_NV_APP_DEV_ADDR_INF, &nv, sizeof(nv));
        }
    }
    return ret;
}

static td_u32 app_proto_set_dev_addr(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length)
{
    proto_set_dev_addr_req *req = (proto_set_dev_addr_req *)(((td_pbyte)frame) + sizeof(app_proto_frame_head));
    proto_set_dev_addr_ind *ind = TD_NULL;
    td_u32 ret;
    td_pbyte payload;
    td_u16 payload_length = sizeof(app_proto_frame_head) + sizeof(proto_set_dev_addr_ind) + PROTO_CRC_SIZE;

    if (frame == TD_NULL || out_buffer == TD_NULL || length == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    payload = (td_pbyte)uapi_malloc(EXT_MOD_ID_APP_COMMON, payload_length);
    if (payload == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }

    (td_void) memset_s(payload, payload_length, 0, payload_length);
    ind = (proto_set_dev_addr_ind *)(payload + sizeof(app_proto_frame_head));
    ind->ret_code = PROTO_RET_CODE_FAILED;
    ind->reason = PROTO_REASON_FORMAT;

    if (frame->length == sizeof(proto_set_dev_addr_req)) {
        if (is_valid_mac_addr(req->addr)) {
            ret = app_proto_set_dev_addr_to_nv(req->addr);
            if (ret == EXT_ERR_SUCCESS) {
                ind->ret_code = PROTO_RET_CODE_SUCCESS;
                ind->reason = PROTO_REASON_NORMAL;
            } else {
                ind->ret_code = PROTO_RET_CODE_FAILED;
                ind->reason = PROTO_REASON_OTHER;
            }
        }
    }

    fill_proto_frame_fixed_info((app_proto_frame_head *)payload, frame->cmd, frame->seq,
                                sizeof(proto_set_dev_addr_ind));

    *out_buffer = payload;
    *length = payload_length;

    return EXT_ERR_SUCCESS;
}

static td_u32 app_proto_reboot_module(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length)
{
    proto_module_reboot_req *req = (proto_module_reboot_req *)(((td_pbyte)frame) + sizeof(app_proto_frame_head));
    proto_module_reboot_ind *ind = TD_NULL;
    td_pbyte payload;
    td_u16 payload_length = sizeof(app_proto_frame_head) + sizeof(proto_module_reboot_ind) + PROTO_CRC_SIZE;
    td_u32 delay = 0;

    if (frame == TD_NULL || out_buffer == TD_NULL || length == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    payload = (td_pbyte)uapi_malloc(EXT_MOD_ID_APP_COMMON, payload_length);
    if (payload == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }

    (td_void) memset_s(payload, payload_length, 0, payload_length);
    ind = (proto_module_reboot_ind *)(payload + sizeof(app_proto_frame_head));

    if (frame->length == sizeof(proto_module_reboot_req)) {
        static td_bool flag = TD_FALSE;

        if (flag == TD_FALSE) {
            delay = req->delay;                     /* second */
            delay *= PROTO_1S_CONVERT_MS;           /* millisecond */
            if (delay <= PROTO_DELAY_MIN_TIME_MS) { /* delay at least 500ms */
                delay = PROTO_DELAY_MIN_TIME_MS;
            }

            app_timer_start(APP_TIMER_ID_REBOOT, delay, EXT_TIMER_TYPE_ONCE);

            flag = TD_TRUE;
        }
    } else {
        ind->ret_code = PROTO_RET_CODE_FAILED;
        ind->reason = PROTO_REASON_FORMAT;
    }

    fill_proto_frame_fixed_info((app_proto_frame_head *)payload, frame->cmd, frame->seq,
                                sizeof(proto_module_reboot_ind));

    *out_buffer = payload;
    *length = payload_length;

    return EXT_ERR_SUCCESS;
}

static td_u32 app_proto_data_trans_tx(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length)
{
    app_proto_frame_head *head = TD_NULL;
    proto_data_transmit_tx_req *req = TD_NULL;
    proto_data_transmit_tx_ind *ind = TD_NULL;
    td_u16 payload_length = sizeof(app_proto_frame_head) + sizeof(proto_data_transmit_tx_ind) + PROTO_CRC_SIZE;
    td_u32 ret;
    td_pbyte payload;

    if (frame == TD_NULL || frame->data == TD_NULL || out_buffer == TD_NULL || length == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    req = (proto_data_transmit_tx_req *)frame->data;
    payload = (td_pbyte)uapi_malloc(EXT_MOD_ID_APP_COMMON, payload_length);
    if (payload == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }

    (td_void) memset_s(payload, payload_length, 0, payload_length);
    head = (app_proto_frame_head *)payload;
    ind = (proto_data_transmit_tx_ind *)head->data;
    ind->ret_code = PROTO_RET_CODE_FAILED;
    ind->reason = PROTO_REASON_FORMAT;

    if ((frame->length > sizeof(proto_data_transmit_tx_req)) &&
        (frame->length == sizeof(proto_data_transmit_tx_req) + req->data_length)) {
        ret = app_proto_plc_tx(req->data, req->data_length, req->dst_addr, ID_PLC_DATA_TRANSMIT);
        if (ret == EXT_ERR_MALLOC_FAILUE) {
            uapi_free(EXT_MOD_ID_APP_COMMON, payload);
            payload = TD_NULL;
            return EXT_ERR_MALLOC_FAILUE;
        } else if (ret == EXT_ERR_SUCCESS) {
            ind->ret_code = PROTO_RET_CODE_SUCCESS;
            ind->reason = PROTO_REASON_NORMAL;
        } else {
            ind->reason = PROTO_REASON_BUSY;
        }
    }

    fill_proto_frame_fixed_info(head, frame->cmd, frame->seq, sizeof(proto_data_transmit_tx_ind));

    *out_buffer = payload;
    *length = payload_length;

    return EXT_ERR_SUCCESS;
}

td_u32 app_proto_data_transmit(td_pbyte data, td_u16 data_length, td_pbyte src_addr)
{
    td_u32 ret;
    app_proto_frame_head *head = TD_NULL;
    proto_data_transmit_rx_req *req = TD_NULL;
    td_pbyte payload;
    td_u16 payload_length = sizeof(app_proto_frame_head) + sizeof(proto_data_transmit_rx_req) +
                            data_length + PROTO_CRC_SIZE;

    if (src_addr == TD_NULL) {
        return EXT_ERR_FAILURE;
    }
    payload = (td_pbyte)uapi_malloc(EXT_MOD_ID_APP_COMMON, payload_length);
    if (payload == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }

    (td_void) memset_s(payload, payload_length, 0, payload_length);

    head = (app_proto_frame_head *)payload;
    req = (proto_data_transmit_rx_req *)head->data;
    if (memcpy_s(req->src_addr, sizeof(req->src_addr), src_addr, EXT_PLC_MAC_ADDR_LEN) != EOK) {
        uapi_free(EXT_MOD_ID_APP_COMMON, payload);
        payload = TD_NULL;
        return EXT_ERR_MEMCPY_FAIL;
    }
    req->data_length = data_length;
    if (req->data_length > 0 && data != TD_NULL) {
        if (memcpy_s(req->data, req->data_length, data, data_length) != EOK) {
            uapi_free(EXT_MOD_ID_APP_COMMON, payload);
            payload = TD_NULL;
            return EXT_ERR_MEMCPY_FAIL;
        }
    }
    fill_proto_frame_fixed_info(head, ID_PROTO_CMD_DATA_TRANS_RX, g_proto_frame_seq++,
                                sizeof(proto_data_transmit_rx_req) + data_length);

    ret = app_uart_tx(payload, payload_length);

    uapi_free(EXT_MOD_ID_APP_COMMON, payload);
    payload = TD_NULL;

    return ret;
}

static td_u32 app_proto_data_trans_rx(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length)
{
    ext_unref_param(frame);
    ext_unref_param(out_buffer);
    ext_unref_param(length);

    return EXT_ERR_SUCCESS;
}

td_u16 app_proto_get_frame_seq(td_void)
{
    return g_proto_frame_seq;
}

td_void app_proto_frame_seq_add(td_void)
{
    g_proto_frame_seq++;
}

td_void fill_proto_frame_fixed_info(app_proto_frame_head *head, td_u16 cmd, td_u16 seq, td_u16 data_length)
{
    td_u16 crc = 0;
    td_u16 length = sizeof(app_proto_frame_head) + data_length;
    td_pbyte payload;

    if (head == TD_NULL) {
        return;
    }

    head->head = APP_FRAME_HEAD_BYTE;
    head->ctrl = PROTO_CTRL_RESPONSE;
    head->cmd = cmd;
    head->seq = seq;
    head->length = data_length;

    payload = (td_pbyte)head;
    crc = uapi_crc16(crc, (EXT_CONST td_u8 *)payload, length);
    payload[length++] = (td_u8)(crc >> PROTO_CRC_HIGH_BIT_OFFSET);
    payload[length++] = (td_u8)crc;
}

static td_u32 app_proto_qry_on_power_time(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length)
{
    proto_qry_on_power_time_ind *ind = TD_NULL;
    td_pbyte payload;
    td_u16 payload_length = sizeof(app_proto_frame_head) + sizeof(proto_qry_on_power_time_ind) + PROTO_CRC_SIZE;

    if (frame == TD_NULL || out_buffer == TD_NULL || length == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    payload = (td_pbyte)uapi_malloc(EXT_MOD_ID_APP_COMMON, payload_length);
    if (payload == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }

    (td_void) memset_s(payload, payload_length, 0, payload_length);
    ind = (proto_qry_on_power_time_ind *)(payload + sizeof(app_proto_frame_head));
    uapi_get_my_mac(ind->addr, sizeof(ind->addr));
    ind->on_power_ms_time = uapi_get_milli_seconds();

    if (memcpy_s(&ind->frame_index, sizeof(ind->frame_index), frame->data, sizeof(td_u16)) != EXT_ERR_SUCCESS) {
        return EXT_ERR_FAILURE;
    }

    fill_proto_frame_fixed_info((app_proto_frame_head *)payload, frame->cmd, frame->seq,
                                sizeof(proto_qry_on_power_time_ind));

    *out_buffer = payload;
    *length = payload_length;

    return EXT_ERR_SUCCESS;
}
