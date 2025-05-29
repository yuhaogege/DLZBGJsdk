/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: APP module protocol frame handle (UPG special).
 */

#include "app_proto_upg.h"
#include "app_proto_proc.h"
#include "app_trans_file.h"
#include "soc_mdm_mem.h"
#include "soc_stdlib.h"
#ifdef __cplusplus
extern "C" {
#endif

static td_u32 app_proto_start_trans_file(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length);
static td_u32 app_proto_trans_file_data(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length);
static td_u32 app_proto_trans_file_query(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length);

td_u32 app_proto_trans_file(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length)
{
    if (frame == TD_NULL) {
        return EXT_ERR_FAILURE;
    }

    switch (frame->data[0]) {
        case PROTO_FN_START_TRANS_FILE:
            return app_proto_start_trans_file(frame, out_buffer, length);

        case PROTO_FN_TRANS_FILE_DATA:
            return app_proto_trans_file_data(frame, out_buffer, length);

        case PROTO_FN_TRANS_FILE_QUERY:
            return app_proto_trans_file_query(frame, out_buffer, length);

        default:
            return EXT_ERR_NOT_SUPPORT;
    }
}

static td_u32 app_proto_handle_trans_file_attr(EXT_CONST app_proto_frame_head *frame, td_bool *is_continue)
{
    td_u32 ret = EXT_ERR_FAILURE;
    proto_start_trans_file_req *req = TD_NULL;

    if (frame == TD_NULL || is_continue == TD_NULL) {
        return EXT_ERR_FAILURE;
    }
    req = (proto_start_trans_file_req *)frame->data;
    switch (req->file_attr) {
        case PROTO_FILE_ATTR_CLEAR:
            ret = trans_file_clear();
            if (ret == EXT_ERR_SUCCESS) {
                trans_file_set_state(PROTO_TRANS_FILE_DONE);
            } else if (ret == EXT_ERR_CONTINUE) {
                *is_continue = TD_TRUE;
                return EXT_ERR_SUCCESS;
            }
            break;

        case PROTO_FILE_ATTR_LOCAL:
        case PROTO_FILE_ATTR_REMOTE:
            ret = trans_file_start(req);
            break;

        default:
            ret = EXT_ERR_FAILURE;
            break;
    }

    return ret;
}

static td_u32 app_proto_start_trans_file(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length)
{
    td_u32 ret = EXT_ERR_FAILURE;
    proto_start_trans_file_ind *ind = TD_NULL;
    td_pbyte payload;
    td_u16 payload_length = sizeof(app_proto_frame_head) + sizeof(proto_start_trans_file_ind) + PROTO_CRC_SIZE;
    td_bool is_con = TD_FALSE;

    if (frame == TD_NULL || out_buffer == TD_NULL || length == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    payload = (td_pbyte)uapi_malloc(EXT_MOD_ID_APP_COMMON, payload_length);
    if (payload == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }

    (td_void) memset_s(payload, payload_length, 0, payload_length);
    ind = (proto_start_trans_file_ind *)(payload + sizeof(app_proto_frame_head));
    ind->fn = PROTO_FN_START_TRANS_FILE;
    ind->state = PROTO_RET_CODE_FAILED;
    ind->reason = PROTO_REASON_FORMAT;

    *out_buffer = TD_NULL;
    *length = 0;

    if (frame->length == (td_u16)sizeof(proto_start_trans_file_req)) {
        ind->reason = PROTO_REASON_OTHER;

        ret = app_proto_handle_trans_file_attr(frame, &is_con);
        if (is_con == TD_TRUE) {
            uapi_free(EXT_MOD_ID_APP_COMMON, payload);
            payload = TD_NULL;

            trans_file_set_seq(frame->seq);

            return EXT_ERR_SUCCESS;
        }
    }

    if (ret == EXT_ERR_SUCCESS) {
        ind->state = PROTO_RET_CODE_SUCCESS;
        ind->reason = PROTO_REASON_NORMAL;
    }

    fill_proto_frame_fixed_info((app_proto_frame_head *)payload, frame->cmd, frame->seq,
                                sizeof(proto_start_trans_file_ind));

    *out_buffer = payload;
    *length = payload_length;

    return EXT_ERR_SUCCESS;
}

static td_u32 app_proto_trans_file_data(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length)
{
    td_u32 ret;
    proto_trans_file_data_req *req = TD_NULL;
    proto_trans_file_data_ind *ind = TD_NULL;
    td_pbyte payload;
    td_u16 payload_length = sizeof(app_proto_frame_head) + sizeof(proto_trans_file_data_ind) + PROTO_CRC_SIZE;

    if (frame == TD_NULL || frame->data == TD_NULL || length == TD_NULL || out_buffer == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    req = (proto_trans_file_data_req *)frame->data;
    payload = (td_pbyte)uapi_malloc(EXT_MOD_ID_APP_COMMON, payload_length);
    if (payload == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }

    (td_void) memset_s(payload, payload_length, 0, payload_length);
    ind = (proto_trans_file_data_ind *)(payload + sizeof(app_proto_frame_head));
    ind->fn = PROTO_FN_TRANS_FILE_DATA;
    ind->state = PROTO_RET_CODE_FAILED;
    ind->reason = PROTO_REASON_OTHER;

    if (frame->length == (td_u16)sizeof(proto_trans_file_data_req) + req->seg_size) {
        ret = trans_file_data(req);
        if (ret == EXT_ERR_SUCCESS) {
            ind->state = PROTO_RET_CODE_SUCCESS;
            ind->reason = PROTO_REASON_NORMAL;
        } else if (ret == EXT_ERR_BAD_DATA) {
            ind->reason = PROTO_REASON_FORMAT;
        }
    }

    fill_proto_frame_fixed_info((app_proto_frame_head *)payload, frame->cmd, frame->seq,
                                sizeof(proto_trans_file_data_ind));

    *out_buffer = payload;
    *length = payload_length;

    return EXT_ERR_SUCCESS;
}

static td_u32 app_proto_trans_file_query(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length)
{
    trans_file_context *ctx = get_trans_file_context();
    proto_trans_file_query_ind *ind = TD_NULL;
    td_pbyte payload;
    td_u16 payload_length = sizeof(app_proto_frame_head) + sizeof(proto_trans_file_query_ind) + PROTO_CRC_SIZE;

    if (frame == TD_NULL || out_buffer == TD_NULL || length == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    payload = (td_pbyte)uapi_malloc(EXT_MOD_ID_APP_COMMON, payload_length);
    if (payload == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }

    (td_void) memset_s(payload, payload_length, 0, payload_length);
    ind = (proto_trans_file_query_ind *)(payload + sizeof(app_proto_frame_head));
    ind->fn = PROTO_FN_TRANS_FILE_QUERY;
    ind->state = ctx->result;
    if (ctx->result == PROTO_TRANS_FILE_UNCOMPLETE) {
        ind->fail_sta = ctx->stat.failed;
    }

    fill_proto_frame_fixed_info((app_proto_frame_head *)payload, frame->cmd, frame->seq,
                                sizeof(proto_trans_file_query_ind));

    *out_buffer = payload;
    *length = payload_length;

    return EXT_ERR_SUCCESS;
}

#ifdef __cplusplus
}
#endif

