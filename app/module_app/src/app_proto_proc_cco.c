/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: APP module protocol frame handle (CCO special).
 */

#include "soc_types.h"
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
#include "app_proto_proc_cco.h"
#include "app_proto_proc.h"
#include "app_cco_whitelist.h"
#include "app_network_identify.h"
#include <soc_mdm_mac.h>
#include "app_plc.h"
#include "app_cmd_msg.h"
#include "app_uart.h"
#include "app_common.h"
#include "soc_mdm_mem.h"
#include "soc_stdlib.h"
#include "soc_mdm_crc.h"

#ifdef __cplusplus
extern "C" {
#endif

td_u32 app_proto_get_whitelist_num(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length)
{
    app_proto_frame_head *head = TD_NULL;
    proto_get_whitelist_num_ind *ind = TD_NULL;
    app_whitelist_head *white_list = app_get_whitelist();
    td_pbyte payload;
    td_u16 payload_length = sizeof(app_proto_frame_head) + sizeof(proto_get_whitelist_num_ind) + PROTO_CRC_SIZE;

    payload = (td_pbyte)uapi_malloc(EXT_MOD_ID_APP_COMMON, payload_length);
    if (payload == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }

    (td_void) memset_s(payload, payload_length, 0, payload_length);
    head = (app_proto_frame_head *)payload;

    ind = (proto_get_whitelist_num_ind *)head->data;
    if (white_list != TD_NULL) {
        ind->num = white_list->total;
    }

    fill_proto_frame_fixed_info(head, frame->cmd, frame->seq, sizeof(proto_get_whitelist_num_ind));

    *out_buffer = payload;
    *length = payload_length;

    return EXT_ERR_SUCCESS;
}

td_u32 app_proto_get_whitelist_info(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length)
{
    app_proto_frame_head *head = TD_NULL;
    proto_get_whitelist_info_req *req = (proto_get_whitelist_info_req *)frame->data;
    proto_get_whitelist_info_ind *ind = TD_NULL;
    app_whitelist_head *white_list = app_get_whitelist();
    td_pbyte payload;
    td_u16 payload_length = sizeof(app_proto_frame_head) + sizeof(proto_get_whitelist_info_ind) + PROTO_CRC_SIZE;
    td_u16 count = 0;
    td_u16 total = 0;

    if (white_list != TD_NULL) {
        total = white_list->total;
    }

    if (frame->length == sizeof(proto_get_whitelist_info_req)) {
        if (req->start < total) {
            count = uapi_min(req->count, total - req->start);
        }
    }

    payload_length += (td_u16)(WHITE_LIST_NODE_SIZE * count);

    payload = (td_pbyte)uapi_malloc(EXT_MOD_ID_APP_COMMON, payload_length);
    if (payload == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }

    (td_void) memset_s(payload, payload_length, 0, payload_length);
    head = (app_proto_frame_head *)payload;

    ind = (proto_get_whitelist_info_ind *)head->data;
    ind->total = total;
    ind->start = req->start;
    ind->count = count;
    if ((ind->count > 0) && (white_list != TD_NULL)) {
        if (count > 0) {
            if (memcpy_s(ind->data, APP_PROTO_SIZE1 * count, white_list->list + APP_PROTO_SIZE1 * ind->start,
                         APP_PROTO_SIZE1 * count) != EOK) {
                uapi_free(EXT_MOD_ID_APP_COMMON, payload);
                payload = TD_NULL;
                return EXT_ERR_MEMCPY_FAIL;
            }
        }
    }

    fill_proto_frame_fixed_info(head, frame->cmd, frame->seq,
                                (td_u16)(sizeof(proto_get_whitelist_info_ind) + APP_PROTO_SIZE1 * count));

    *out_buffer = payload;
    *length = payload_length;

    return EXT_ERR_SUCCESS;
}

td_u32 app_proto_add_whitelist_node(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length)
{
    app_proto_frame_head *head = TD_NULL;
    proto_add_whitelist_ind *ind = TD_NULL;
    td_pbyte data = (td_pbyte)frame->data;
    td_pbyte payload;
    td_u16 payload_length = sizeof(app_proto_frame_head) + sizeof(proto_add_whitelist_ind) + PROTO_CRC_SIZE;
    td_u16 count;
    td_u32 ret;

    payload = (td_pbyte)uapi_malloc(EXT_MOD_ID_APP_COMMON, payload_length);
    if (payload == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }

    (td_void) memset_s(payload, payload_length, 0, payload_length);
    head = (app_proto_frame_head *)payload;
    ind = (proto_add_whitelist_ind *)head->data;
    ind->ret_code = PROTO_RET_CODE_FAILED;
    ind->reason = PROTO_REASON_FORMAT;

    if (frame->length > APP_PROTO_MIN_LENGTH) {
        count = uapi_make_u16(data[0], data[1]);
        if (frame->length == APP_PROTO_MIN_LENGTH + count * APP_PROTO_SIZE1) {
            ret = app_whitelist_add(data + APP_PROTO_MIN_LENGTH, count);
            if (ret == EXT_ERR_SUCCESS) {
                ind->ret_code = PROTO_RET_CODE_SUCCESS;
                ind->reason = PROTO_REASON_NORMAL;
            } else {
                ind->reason = PROTO_REASON_OTHER;
            }
        }
    }

    fill_proto_frame_fixed_info(head, frame->cmd, frame->seq, sizeof(proto_add_whitelist_ind));

    *out_buffer = payload;
    *length = payload_length;

    return EXT_ERR_SUCCESS;
}

#define DELETE_WHITELIST_MAX 1024
td_u32 app_proto_del_whitelist_node(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length)
{
    app_proto_frame_head *head = TD_NULL;
    proto_del_whitelist_ind *ind = TD_NULL;
    td_pbyte data = (td_pbyte)frame->data;
    td_pbyte payload;
    td_u16 payload_length = sizeof(app_proto_frame_head) + sizeof(proto_del_whitelist_ind) + PROTO_CRC_SIZE;
    td_u16 count;

    payload = (td_pbyte)uapi_malloc(EXT_MOD_ID_APP_COMMON, payload_length);
    if (payload == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }

    (td_void) memset_s(payload, payload_length, 0, payload_length);
    head = (app_proto_frame_head *)payload;
    ind = (proto_del_whitelist_ind *)head->data;
    ind->ret_code = PROTO_RET_CODE_FAILED;
    ind->reason = PROTO_REASON_FORMAT;

    if (frame->length > APP_PROTO_MIN_LENGTH) {
        count = uapi_make_u16(data[0], data[1]);
        if (count > DELETE_WHITELIST_MAX) {
            uapi_free(EXT_MOD_ID_APP_COMMON, payload);
            return EXT_ERR_FAILURE;
        }
        if (frame->length == (APP_PROTO_MIN_LENGTH + count * APP_PROTO_SIZE2)) {
            td_u32 ret = app_whitelist_delete(data + APP_PROTO_MIN_LENGTH, count);
            if (ret == EXT_ERR_SUCCESS) {
                ind->ret_code = PROTO_RET_CODE_SUCCESS;
                ind->reason = PROTO_REASON_NORMAL;
            } else {
                ind->reason = PROTO_REASON_OTHER;
            }
        }
    }

    fill_proto_frame_fixed_info(head, frame->cmd, frame->seq, sizeof(proto_del_whitelist_ind));

    *out_buffer = payload;
    *length = payload_length;

    return EXT_ERR_SUCCESS;
}

td_u32 app_proto_clear_whitelist(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length)
{
    app_proto_frame_head *head = TD_NULL;
    proto_clear_whitelist_ind *ind = TD_NULL;
    td_u32 ret;
    td_pbyte payload;
    td_u16 payload_length = sizeof(app_proto_frame_head) + sizeof(proto_clear_whitelist_ind) + PROTO_CRC_SIZE;

    payload = (td_pbyte)uapi_malloc(EXT_MOD_ID_APP_COMMON, payload_length);
    if (payload == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }

    (td_void) memset_s(payload, payload_length, 0, payload_length);
    head = (app_proto_frame_head *)payload;
    ind = (proto_clear_whitelist_ind *)head->data;

    ret = app_whitelist_clear();
    if (ret == EXT_ERR_SUCCESS) {
        ind->ret_code = PROTO_RET_CODE_SUCCESS;
        ind->reason = PROTO_REASON_NORMAL;
    } else {
        ind->ret_code = PROTO_RET_CODE_FAILED;
        ind->reason = PROTO_REASON_OTHER;
    }

    fill_proto_frame_fixed_info(head, frame->cmd, frame->seq, sizeof(proto_clear_whitelist_ind));

    *out_buffer = payload;
    *length = payload_length;

    return EXT_ERR_SUCCESS;
}

td_u32 app_proto_get_topo_num(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length)
{
    app_proto_frame_head *head = TD_NULL;
    proto_get_topo_num_ind *ind = TD_NULL;
    td_pbyte payload;
    td_u16 payload_length = sizeof(app_proto_frame_head) + sizeof(proto_get_topo_num_ind) + PROTO_CRC_SIZE;

    payload = (td_pbyte)uapi_malloc(EXT_MOD_ID_APP_COMMON, payload_length);
    if (payload == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }

    (td_void) memset_s(payload, payload_length, 0, payload_length);
    head = (app_proto_frame_head *)payload;

    ind = (proto_get_topo_num_ind *)head->data;
    ind->num = uapi_get_topo_node_num();

    fill_proto_frame_fixed_info(head, frame->cmd, frame->seq, sizeof(proto_get_topo_num_ind));

    *out_buffer = payload;
    *length = payload_length;

    return EXT_ERR_SUCCESS;
}

td_u16 app_proto_fill_topo_info(proto_get_topo_info_ind *ind, ext_mdm_nm_topo_node_info *topo_head, td_u16 start)
{
    td_u16 count = 0;
    td_u16 i;
    proto_topo_node *node = TD_NULL;
    ext_mdm_nm_topo_node_info *topo_info = TD_NULL;

    node = (proto_topo_node *)ind->data;

    if (topo_head == TD_NULL) {
        return count;
    }
    for (i = start - 1; (i < EXT_MDM_NM_TOPO_NODE_NUM_MAX) && (count < ind->count); i++) {
        topo_info = topo_head + i;
        if (topo_head[i].state != STATE_NOT_USED) {
            (td_void) memcpy_s(node->mac, sizeof(node->mac), topo_info->mac, sizeof(topo_info->mac));
            node->tei = i + 1;
            node->proxy = topo_info->proxy_tei;
            node->level = topo_info->level;
            node->role = topo_info->ability;
            node++;
            count++;
        }
    }
    return count;
}

td_u32 app_proto_get_topo_info(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length)
{
    app_proto_frame_head *head = TD_NULL;
    proto_get_topo_info_req *req = (proto_get_topo_info_req *)frame->data;
    proto_get_topo_info_ind *ind = TD_NULL;
    ext_mdm_nm_topo_node_info *topo_head = TD_NULL;
    td_pbyte payload;
    td_u16 payload_length = sizeof(app_proto_frame_head) + sizeof(proto_get_topo_info_ind) + PROTO_CRC_SIZE;
    td_u16 count = 0;
    td_u16 i;

    do {
        if ((frame->length != sizeof(proto_get_topo_info_req)) || (req->start == 0)) {
            break;
        }

        uapi_get_topo_info(&topo_head);
        if (topo_head == TD_NULL) {
            break;
        }

        for (i = req->start - 1; (i < EXT_MDM_NM_TOPO_NODE_NUM_MAX) && (count < req->count); i++) {
            if (topo_head[i].state != STATE_NOT_USED) {
                count++;
            }
        }
    } while (0);

    payload_length += (td_u16)(sizeof(proto_topo_node) * count);

    payload = (td_pbyte)uapi_malloc(EXT_MOD_ID_APP_COMMON, payload_length);
    if (payload == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }

    (td_void) memset_s(payload, payload_length, 0, payload_length);
    head = (app_proto_frame_head *)payload;

    ind = (proto_get_topo_info_ind *)head->data;
    ind->total = uapi_get_topo_node_num();
    ind->start = req->start;
    ind->count = count;

    count = app_proto_fill_topo_info(ind, topo_head, req->start);

    fill_proto_frame_fixed_info(head, frame->cmd, frame->seq,
                                (td_u16)(sizeof(proto_get_topo_info_ind) + sizeof(proto_topo_node) * count));

    *out_buffer = payload;
    *length = payload_length;

    return EXT_ERR_SUCCESS;
}

td_u32 app_proto_send_remote_cmd_frame(proto_remote_cmd_tx_req *req, proto_remote_cmd_tx_ind *ind)
{
    td_u32 ret;
    app_plc_frame_data plc;
    app_plc_trans_frame *trans = TD_NULL;
    td_u16 buffer_length = sizeof(app_plc_trans_frame) + req->data_length;
    td_pbyte buffer;

    buffer = (td_pbyte)uapi_malloc(EXT_MOD_ID_APP_COMMON, buffer_length);
    if (buffer == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }

    (td_void) memset_s(buffer, buffer_length, 0, buffer_length);

    trans = (app_plc_trans_frame *)buffer;
    trans->data_length = req->data_length;
    if (trans->data_length) {
        if (memcpy_s(trans->data, trans->data_length, req->data, req->data_length) != EOK) {
            uapi_free(EXT_MOD_ID_APP_COMMON, buffer);
            return EXT_ERR_MEMCPY_FAIL;
        }
    }

    trans->crc_16 = uapi_crc16(0, trans->data, trans->data_length);
    trans->crc_16 = ext_swap16(trans->crc_16);

    plc.id = ID_PLC_REMOTE_CMD;
    (td_void) memcpy_s(plc.mac, sizeof(plc.mac), req->dst_addr, sizeof(req->dst_addr));
    plc.payload_length = buffer_length;
    plc.payload = buffer;

    ret = app_send_plc_frame(&plc);
    if (ret == EXT_ERR_SUCCESS) {
        ind->ret_code = PROTO_RET_CODE_SUCCESS;
        ind->reason = PROTO_REASON_NORMAL;
    } else {
        ind->reason = PROTO_REASON_BUSY;
    }

    uapi_free(EXT_MOD_ID_APP_COMMON, buffer);
    buffer = TD_NULL;

    return EXT_ERR_SUCCESS;
}

td_u32 app_proto_remote_cmd_tx(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length)
{
    app_proto_frame_head *head = TD_NULL;
    proto_remote_cmd_tx_req *req = (proto_remote_cmd_tx_req *)frame->data;
    proto_remote_cmd_tx_ind *ind = TD_NULL;
    td_u16 payload_length = sizeof(app_proto_frame_head) + sizeof(proto_remote_cmd_tx_ind) + PROTO_CRC_SIZE;
    td_pbyte payload = (td_pbyte)uapi_malloc(EXT_MOD_ID_APP_COMMON, payload_length);
    td_u32 ret;

    if (payload == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }

    (td_void) memset_s(payload, payload_length, 0, payload_length);
    head = (app_proto_frame_head *)payload;
    ind = (proto_remote_cmd_tx_ind *)head->data;
    ind->ret_code = PROTO_RET_CODE_FAILED;
    ind->reason = PROTO_REASON_FORMAT;

    if ((frame->length > sizeof(proto_remote_cmd_tx_req)) &&
        (frame->length == sizeof(proto_remote_cmd_tx_req) + req->data_length) &&
        is_valid_mac_addr(req->dst_addr)) {
        ret = app_proto_send_remote_cmd_frame(req, ind);
        if (ret == EXT_ERR_MALLOC_FAILUE) {
            uapi_free(EXT_MOD_ID_APP_COMMON, payload);
            payload = TD_NULL;
            return EXT_ERR_MALLOC_FAILUE;
        } else if (ret != EXT_ERR_SUCCESS) {
            return ret;
        }
    }

    fill_proto_frame_fixed_info(head, frame->cmd, frame->seq, sizeof(proto_data_transmit_tx_ind));

    *out_buffer = payload;
    *length = payload_length;

    return EXT_ERR_SUCCESS;
}

td_u32 app_proto_remote_cmd_rx(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length)
{
    ext_unref_param(frame);
    ext_unref_param(out_buffer);
    ext_unref_param(length);

    return EXT_ERR_SUCCESS;
}

td_u32 app_proto_remote_cmd(td_pbyte data, td_u16 data_length, td_pbyte src_addr)
{
    td_u32 ret;
    app_proto_frame_head *head = TD_NULL;
    proto_remote_cmd_rx_req *req = TD_NULL;
    app_proto_frame_head *rcv_cmd = (app_proto_frame_head *)data;
    td_pbyte payload;
    td_u16 frame_seq;
    td_u16 payload_length = sizeof(app_proto_frame_head) + sizeof(proto_remote_cmd_rx_req) +
                            data_length + PROTO_CRC_SIZE;

    payload = (td_pbyte)uapi_malloc(EXT_MOD_ID_APP_COMMON, payload_length);
    if (payload == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }

    (td_void) memset_s(payload, payload_length, 0, payload_length);

    head = (app_proto_frame_head *)payload;
    if (rcv_cmd->cmd == ID_PROTO_CMD_QRY_ONPOWER_TIME) {
        app_ni_handle_rcv_sta_rsp(rcv_cmd->data, rcv_cmd->length, src_addr, EXT_PLC_MAC_ADDR_LEN);
    }
    req = (proto_remote_cmd_rx_req *)head->data;
    if (memcpy_s(req->src_addr, sizeof(req->src_addr), src_addr, EXT_PLC_MAC_ADDR_LEN) != 0) {
        return EXT_ERR_MEMCPY_FAIL;
    }
    req->data_length = data_length;
    if (req->data_length > 0) {
        if (memcpy_s(req->data, req->data_length, data, data_length) != EOK) {
            return EXT_ERR_MEMCPY_FAIL;
        }
    }
    frame_seq = app_proto_get_frame_seq();
    app_proto_frame_seq_add();
    fill_proto_frame_fixed_info(head, ID_PROTO_CMD_REMOTE_CMD_RX, frame_seq,
                                sizeof(proto_remote_cmd_rx_req) + data_length);

    ret = app_uart_tx(payload, payload_length);

    uapi_free(EXT_MOD_ID_APP_COMMON, payload);
    payload = TD_NULL;

    return ret;
}

td_u32 app_proto_set_network_identify_state(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length)
{
    app_proto_frame_head *head = TD_NULL;
    proto_enable_ni_ind *ind = TD_NULL;
    td_u32 ret;
    td_pbyte payload;
    proto_set_ni_state_info *set_info = TD_NULL;
    td_u16 payload_length = sizeof(app_proto_frame_head) + sizeof(proto_enable_ni_ind) + PROTO_CRC_SIZE;

    payload = (td_pbyte)uapi_malloc(EXT_MOD_ID_APP_COMMON, payload_length);
    if (payload == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }
    (td_void) memset_s(payload, payload_length, 0, payload_length);
    head = (app_proto_frame_head *)payload;
    ind = (proto_enable_ni_ind *)head->data;
    if (frame->length != sizeof(proto_set_ni_state_info)) {
        ret = PROTO_RET_CODE_FAILED;
        ind->reason = PROTO_REASON_FORMAT;
    } else {
        set_info = (proto_set_ni_state_info *)frame->data;
        ret = app_ni_set_self_identify_params(set_info->state, set_info->wait_sta_join_time,
            set_info->valid_time_diff);
        if (ret == EXT_ERR_SUCCESS) {
            ind->ret_code = PROTO_RET_CODE_SUCCESS;
            ind->reason = PROTO_REASON_NORMAL;
        } else {
            ind->ret_code = PROTO_RET_CODE_FAILED;
            ind->reason = PROTO_REASON_OTHER;
        }
    }

    fill_proto_frame_fixed_info(head, frame->cmd, frame->seq, sizeof(proto_reply_ind));
    *out_buffer = payload;
    *length = payload_length;

    return EXT_ERR_SUCCESS;
}

td_u32 app_proto_set_whitelist_state(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length)
{
    td_u16 payload_length = sizeof(app_proto_frame_head) + sizeof(proto_reply_info) + PROTO_CRC_SIZE;
    td_u8 *payload = TD_NULL;

    if (frame->data[0] == TD_TRUE) {
        uapi_set_white_list_switch(TD_TRUE, TD_FALSE, EXT_MAC_WHITE_LIST_CHANGE_REASON_APP_CON);
    } else {
        uapi_set_white_list_switch(TD_FALSE, TD_FALSE, EXT_MAC_WHITE_LIST_CHANGE_REASON_APP_CON);
    }

    payload = (td_u8 *)uapi_malloc(EXT_MOD_ID_APP_COMMON, payload_length);
    if (payload == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }
    (td_void) memset_s(payload, payload_length, 0, payload_length);

    payload[sizeof(app_proto_frame_head)] = PROTO_RET_CODE_SUCCESS;

    fill_proto_frame_fixed_info((app_proto_frame_head *)payload, frame->cmd, frame->seq, sizeof(proto_reply_info));
    *out_buffer = (td_pbyte)payload;
    *length = payload_length;

    return EXT_ERR_SUCCESS;
}

td_u32 app_proto_get_whitelist_state(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length)
{
    td_u16 payload_length = sizeof(app_proto_frame_head) + sizeof(proto_reply_info) + PROTO_CRC_SIZE;
    td_bool state = TD_FALSE;
    td_u8 *payload = TD_NULL;

    uapi_get_white_list_switch(&state);
    payload = (td_u8 *)uapi_malloc(EXT_MOD_ID_APP_COMMON, payload_length);
    if (payload == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }
    (td_void) memset_s(payload, payload_length, 0, payload_length);

    payload[sizeof(app_proto_frame_head)] = state;

    fill_proto_frame_fixed_info((app_proto_frame_head *)payload, frame->cmd, frame->seq, sizeof(proto_reply_info));
    *out_buffer = (td_pbyte)payload;
    *length = payload_length;

    return EXT_ERR_SUCCESS;
}
#ifdef __cplusplus
}
#endif
#endif

