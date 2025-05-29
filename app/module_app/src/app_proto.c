/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: APP module protocol base API.
 */

#include "app_proto.h"
#include "app_common.h"
#include "app_cmd_msg.h"
#include "soc_mdm_mem.h"
#include "soc_stdlib.h"
#include "soc_mdm_crc.h"
#ifdef __cplusplus
extern "C" {
#endif

static td_u32 app_proto_send_msg_to_parse(app_data_buf *buf_head, td_pbyte data, td_u16 frame_length)
{
    td_pbyte payload;
    td_u16 payload_len = frame_length;
    ext_sys_queue_msg msg;

    if (buf_head == TD_NULL || data == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    payload = (td_pbyte)uapi_malloc(EXT_MOD_ID_APP_COMMON, payload_len);
    if (payload == TD_NULL) {
        buf_head->rx_length = 0;
        return EXT_ERR_FAILURE;
    }

    (td_void) memcpy_s(payload, payload_len, data, frame_length);

    msg.msg_id = ID_APP_MSG_FRAME_INPUT;
    msg.param[0] = (uintptr_t)payload;
    msg.param[1] = (uintptr_t)frame_length;
    msg.param[APP_PROTO_MSG_PARAM_INDEX2] = 0;
    msg.param[APP_PROTO_MSG_PARAM_INDEX3] = 0;

    if (app_send_message(&msg) != EXT_ERR_SUCCESS) {
        uapi_free(EXT_MOD_ID_APP_COMMON, payload);
        payload = TD_NULL;
        buf_head->rx_length = 0;
        return EXT_ERR_FAILURE;
    }

    return EXT_ERR_SUCCESS;
}

td_void app_proto_parse(app_data_buf *buf_head)
{
    app_proto_frame_head *frame = TD_NULL;
    td_u16 frame_length;

    if (buf_head == TD_NULL) {
        return;
    }
    td_u16 data_length = buf_head->rx_length;
    td_pbyte data = buf_head->buffer;

    for (;;) {
        td_pbyte head = find_byte_in_stream(data, data_length, APP_FRAME_HEAD_BYTE);
        if (head == TD_NULL) {
            buf_head->rx_length = 0;
            return;
        }

        data_length = data_length - (td_u16)(head - data);
        data = head;

        if (data_length < sizeof(app_proto_frame_head)) {
            break;
        }

        frame = (app_proto_frame_head *)data;
        frame_length = sizeof(app_proto_frame_head) + frame->length + PROTO_CRC_SIZE;

        /* head + data + crc */
        if (frame_length > APP_DATA_BUF_SIZE) {
            /* Over size */
            buf_head->rx_length = 0;
            return;
        }

        if (data_length < frame_length) {
            break;
        }

        if (check_app_proto_frame(data, frame_length) == TD_FALSE) {
            data++;
            data_length--;
            continue;
        } else {
            if (app_proto_send_msg_to_parse(buf_head, data, frame_length) != EXT_ERR_SUCCESS) {
                return;
            }

            data += frame_length;
            data_length -= frame_length;
        }
    }

    if (data != buf_head->buffer) {
        int ret;
        ret = memmove_s(buf_head->buffer, sizeof(buf_head->buffer), data, data_length);
        buf_head->rx_length = data_length;
        if (ret == 0) {
            return;
        }
    }
}

td_bool check_app_proto_frame(td_pbyte data, td_u16 data_length)
{
    app_proto_frame_head *frame = (app_proto_frame_head *)data;
    td_u16 frame_length;
    td_u16 calc_crc;

    if ((data == TD_NULL) || (data_length == 0)) {
        return TD_FALSE;
    }

    frame_length = sizeof(app_proto_frame_head) + frame->length + PROTO_CRC_SIZE;
    if (frame_length != data_length) {
        return TD_FALSE;
    }

    if (frame->head != APP_FRAME_HEAD_BYTE) {
        return TD_FALSE;
    }

    calc_crc = uapi_crc16(0, data, frame_length);
    if (calc_crc != 0) {
        return TD_FALSE;
    }

    return TD_TRUE;
}

#ifdef __cplusplus
}
#endif

