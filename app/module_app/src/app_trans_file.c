/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: Transmit file process.
 */

#include "app_trans_file.h"
#include <soc_mdm_mac.h>
#include "app_common.h"
#include "app_cmd_msg.h"
#include "app_uart.h"
#include "soc_stdlib.h"
#include "soc_mdm_upg.h"
#include "soc_mdm_mem.h"
#include "soc_mdm_crc.h"
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
#include "app_plc.h"
#include "app_nv.h"
#include "soc_mdm_nv.h"
#endif

#define APP_TIMER_CLEAR_FILE_TIMEOUT   800
#define APP_TIMER_RX_FILE_TIMEOUT      600000 /* 10 minutes */
#define UPG_STATUS_QUERY_TIME_OUT      2000
#define UPG_STATUS_QUERY_INTERVAL      60000    /* 1 minute */
#define UPG_WAIT_PLC_CHL_READY         10800000 /* 180 minutes */
#define UPG_STATUS_QUERY_POWERON_TIMES 5
#define FRAME_RETRY_MAX                3
#define UPG_FILE_TYPE_LOCAL            0 /* type: UNICAST */
#define UPG_FILE_OPTION_LOCAL          EXT_UPG_REQ_OPTION_UNICAST
#define UPG_FILE_TYPE_REMOTE           1 /* type: STA-->STA */
#define UPG_FILE_OPTION_REMOTE         EXT_UPG_REQ_OPTION_STA

typedef void (*trans_file_msg_proc)(td_pbyte packet, td_u16 packet_size);

typedef struct {
    td_u32 msg_id;
    trans_file_msg_proc msg_proc;
} trans_file_msg_pair;

trans_file_context g_trans_file_ctx;

static td_void trans_file_destroy(td_void);
static td_u32 remote_upg_msg_rx(td_u32 msg_id, td_pbyte packet, td_u16 packet_size);
static td_bool upg_msg_id_valid(td_u32 msg_id);
static td_u32 remote_upg_start(EXT_CONST trans_file_context *ctx);
static td_u32 remote_upg_stop(trans_file_context *ctx);
static td_u32 remote_upg_status(td_u32 option);
static td_u32 remote_upg_proc(trans_file_context *ctx);
static td_void trans_file_clear_reply(EXT_CONST trans_file_context *ctx, td_u8 state, td_u8 reason);
static td_void remote_upg_start_msg_rx(td_pbyte packet, td_u16 packet_size);
static td_void remote_upg_stop_msg_rx(td_pbyte packet, td_u16 packet_size);
static td_void remote_upg_status_bs_msg_rx(td_pbyte packet, td_u16 packet_size);
static td_void remote_upg_stat_info_msg_rx(td_pbyte packet, td_u16 packet_size);
static td_void remote_upg_status_msg_rx(td_pbyte packet, td_u16 packet_size);
static td_void trans_file_failed_proc(td_void);
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
td_void poweron_active_query(trans_file_context *ctx);
td_void trans_file_save_file_inf(td_u32 filc_crc, td_u8 file_attr);
#endif

trans_file_context *get_trans_file_context(td_void)
{
    return &g_trans_file_ctx;
}

td_u32 trans_file_init(td_void)
{
    (td_void) memset_s(&g_trans_file_ctx, sizeof(g_trans_file_ctx), 0, sizeof(g_trans_file_ctx));

    uapi_upg_register_msg_notify(EXT_UPG_START, remote_upg_msg_rx);
    uapi_upg_register_msg_notify(EXT_UPG_STOP, remote_upg_msg_rx);
    uapi_upg_register_msg_notify(EXT_UPG_STATUS_BS_IND, remote_upg_msg_rx);
    uapi_upg_register_msg_notify(EXT_UPG_STAT_INFO_IND, remote_upg_msg_rx);
    uapi_upg_register_msg_notify(EXT_UPG_STATUS, remote_upg_msg_rx);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    {
        app_upg_inf nv;

        (td_void) memset_s(&nv, sizeof(nv), 0, sizeof(nv));
        (td_void) uapi_nv_read(ID_NV_APP_UPG_INF, (EXT_CONST td_pvoid)&nv, sizeof(nv));
        if ((nv.filc_crc != 0) && (nv.file_attr == PROTO_FILE_ATTR_REMOTE)) {
            g_trans_file_ctx.stage = TRANS_FILE_STAGE_UPG;
            g_trans_file_ctx.result = PROTO_TRANS_FILE_INPROGRESS;
            g_trans_file_ctx.file_crc = nv.filc_crc;
            g_trans_file_ctx.file_attr = nv.file_attr;

            poweron_active_query(&g_trans_file_ctx);
        }
    }
#endif

    return EXT_ERR_SUCCESS;
}

static td_void trans_file_destroy(td_void)
{
    trans_file_context *ctx = get_trans_file_context();

    uapi_upg_release_cache();

    app_timer_stop(APP_TIMER_ID_TRANS_FILE_RX);
    app_timer_stop(APP_TIMER_ID_TRANS_FILE_STOP_RETRY);
    app_timer_stop(APP_TIMER_ID_TRANS_FILE_STATUS_QRY);
    app_timer_stop(APP_TIMER_ID_TRANS_FILE_QRY_ACTIVE);
    app_timer_stop(APP_TIMER_ID_TRANS_FILE_QRY_INFO);

    ctx->stage = TRANS_FILE_STAGE_IDLE;
    ctx->clear_flag = TD_FALSE;
    ctx->clear_resp = TD_FALSE;
    ctx->allow_stop = TD_TRUE;
    ctx->stop_times = 0;
    ctx->seq = 0;
    ctx->seg_num = 0;
    ctx->seg_total = 0;
    ctx->seg_size = 0;
    ctx->file_len = 0;
    ctx->file_crc = 0;
    ctx->timeout = 0;
    ctx->retry = 0;
    ctx->wait_chl = TD_FALSE;

    if (ctx->result != PROTO_TRANS_FILE_UNCOMPLETE) {
        (td_void) memset_s(&ctx->stat, sizeof(ctx->stat), 0, sizeof(ctx->stat));
    }

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    trans_file_save_file_inf(0, 0);
#endif
}

td_void trans_file_set_seq(td_u16 seq)
{
    trans_file_context *ctx = get_trans_file_context();
    ctx->seq = seq;
}

td_void trans_file_set_state(td_u8 state)
{
    trans_file_context *ctx = get_trans_file_context();
    ctx->result = state;
}

td_u32 trans_file_clear(td_void)
{
    trans_file_context *ctx = get_trans_file_context();

    if (ctx->stage == TRANS_FILE_STAGE_IDLE) {
        return EXT_ERR_SUCCESS;
    } else if (ctx->stage == TRANS_FILE_STAGE_LOCAL) {
        trans_file_destroy();
        return EXT_ERR_SUCCESS;
    } else {
        if (ctx->allow_stop == UPG_STOP_DISABLE) {
            return EXT_ERR_FAILURE;
        }

        ctx->clear_flag = TD_TRUE;
        ctx->clear_resp = TD_FALSE;
        return remote_upg_stop(ctx);
    }
}

static td_void trans_file_clear_reply(EXT_CONST trans_file_context *ctx, td_u8 state, td_u8 reason)
{
    td_pbyte payload;
    td_u16 payload_length = sizeof(app_proto_frame_head) + sizeof(proto_start_trans_file_ind) + PROTO_CRC_SIZE;
    proto_start_trans_file_ind *ind = TD_NULL;

    if (ctx == TD_NULL) {
        return;
    }
    payload = (td_pbyte)uapi_malloc(EXT_MOD_ID_APP_COMMON, payload_length);
    if (payload == TD_NULL) {
        return;
    }

    (td_void) memset_s(payload, payload_length, 0, payload_length);
    ind = (proto_start_trans_file_ind *)(payload + sizeof(app_proto_frame_head));
    ind->fn = PROTO_FN_START_TRANS_FILE;
    ind->state = state;
    ind->reason = reason;
    ind->rsv = 0;
    fill_proto_frame_fixed_info((app_proto_frame_head *)payload, ID_PROTO_CMD_TRANS_FILE, ctx->seq,
                                sizeof(proto_start_trans_file_ind));

    app_uart_tx(payload, payload_length);

    uapi_free(EXT_MOD_ID_APP_COMMON, payload);
    payload = TD_NULL;
}

td_void trans_file_on_timer_stop_retry(td_void)
{
    trans_file_context *ctx = get_trans_file_context();

    if (ctx == TD_NULL) {
        return;
    }

    if (ctx->stop_times < FRAME_RETRY_MAX) {
        remote_upg_stop(ctx);
    } else {
        ctx->stop_times = 0;
        ctx->clear_flag = TD_FALSE;
        ctx->clear_resp = TD_TRUE;
        trans_file_clear_reply(ctx, PROTO_RET_CODE_FAILED, PROTO_REASON_TIMEOUT);
    }
}

td_void trans_file_on_timer_frame(td_void)
{
    trans_file_set_state(PROTO_TRANS_FILE_DONE);
    trans_file_destroy();
}

td_void trans_file_on_timer_status_query(td_void)
{
    trans_file_context *ctx = get_trans_file_context();

    ctx->allow_stop = TD_TRUE;
    if (remote_upg_status(EXT_UPG_STATUS_REQ_OPT_BS) != EXT_ERR_SUCCESS) {
        return;
    }
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
td_void trans_file_on_timer_query_active(td_void)
{
    trans_file_context *ctx = get_trans_file_context();

    if (ctx->status_msg_cnt < UPG_STATUS_QUERY_POWERON_TIMES) {
        poweron_active_query(ctx);
    } else {
        app_timer_stop(APP_TIMER_ID_TRANS_FILE_STATUS_QRY);
    }
}

td_void trans_file_on_timer_plc_available(td_void)
{
    trans_file_context *ctx = get_trans_file_context();

    if (ctx->wait_chl == TD_TRUE) {
        trans_file_failed_proc();
        trans_file_destroy();
    }
}

td_void trans_file_on_plc_chl_available(td_void)
{
    trans_file_context *ctx = get_trans_file_context();

    if (ctx->wait_chl == TD_TRUE) {
        remote_upg_proc(ctx);
    }
}
#endif

td_void trans_file_on_timer_query_info(td_void)
{
    trans_file_context *ctx = get_trans_file_context();

    if ((ctx->retry >= FRAME_RETRY_MAX) ||
        (remote_upg_status(EXT_UPG_STATUS_REQ_OPT_NA) != EXT_ERR_SUCCESS)) {
        trans_file_failed_proc();
        trans_file_destroy();
    }
}

td_u32 trans_file_start(EXT_CONST proto_start_trans_file_req *req)
{
    trans_file_context *ctx = get_trans_file_context();
    td_u32 ret;
    td_u32 file_type;

    if (ctx == TD_NULL || req == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    if (ctx->stage != TRANS_FILE_STAGE_IDLE) {
        return EXT_ERR_BUSY;
    }

    switch (req->file_attr) {
        case PROTO_FILE_ATTR_LOCAL:
            file_type = UPG_FILE_TYPE_LOCAL;
            break;

        case PROTO_FILE_ATTR_REMOTE:
            file_type = UPG_FILE_TYPE_REMOTE;
            break;

        default:
            return EXT_ERR_INVALID_PARAMETER;
    }

    ret = uapi_upg_obtain_cache(file_type, req->file_len);
    if (ret != EXT_ERR_SUCCESS) {
        return EXT_ERR_FAILURE;
    }

    ctx->file_attr = req->file_attr;
    ctx->file_len = req->file_len;
    ctx->file_crc = req->file_crc;
    ctx->seg_total = req->seg_total;
    ctx->timeout = req->trans_timeout;
    ctx->seg_num = 0;
    ctx->stage = TRANS_FILE_STAGE_LOCAL;

    trans_file_set_state(PROTO_TRANS_FILE_INPROGRESS);
    app_timer_start(APP_TIMER_ID_TRANS_FILE_RX, APP_TIMER_RX_FILE_TIMEOUT, EXT_TIMER_TYPE_ONCE);

    return EXT_ERR_SUCCESS;
}

td_u32 trans_file_data(EXT_CONST proto_trans_file_data_req *req)
{
    trans_file_context *ctx = get_trans_file_context();
    td_u32 ret;
    td_u16 crc = 0;

    if (req == TD_NULL) {
        return EXT_ERR_BAD_DATA;
    }

    if ((req->seg_num >= ctx->seg_total) || (req->seg_num != ctx->seg_num) || (req->seg_size == 0)) {
        return EXT_ERR_BAD_DATA;
    }

    if (req->seg_num == 0) {
        ctx->seg_size = req->seg_size;
    }

    if (req->seg_num + 1 != ctx->seg_total) {
        if (req->seg_size != ctx->seg_size) {
            return EXT_ERR_BAD_DATA;
        }
    } else {
        if (req->seg_size != (ctx->file_len - ctx->seg_size * ctx->seg_num)) {
            return EXT_ERR_BAD_DATA;
        }
    }

    app_timer_start(APP_TIMER_ID_TRANS_FILE_RX, APP_TIMER_RX_FILE_TIMEOUT, EXT_TIMER_TYPE_ONCE);

    crc = uapi_crc16(crc, req->seg_data, req->seg_size);
    crc = ext_swap16(crc);
    if (crc != req->seg_crc) {
        return EXT_ERR_BAD_DATA;
    }

    ret = uapi_upg_mem_write(ctx->seg_num * ctx->seg_size, req->seg_data, req->seg_size);
    if (ret != EXT_ERR_SUCCESS) {
        return EXT_ERR_FAILURE;
    }

    ctx->seg_num++;

    if (ctx->seg_num == ctx->seg_total) {
        remote_upg_proc(ctx);
    }

    return EXT_ERR_SUCCESS;
}

static td_void trans_file_failed_proc(td_void)
{
    trans_file_context *ctx = get_trans_file_context();
    td_u16 topo_num = 0;

    if (ctx->file_attr == PROTO_FILE_ATTR_LOCAL) {
        topo_num = 1;
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    } else {
        td_u16 temp_num = uapi_get_topo_node_num();
        if (temp_num > 1) {
            topo_num = temp_num - 1;
        }

#endif
    }

    ctx->stat.failed = topo_num;
    ctx->result = PROTO_TRANS_FILE_UNCOMPLETE;
}

static td_bool upg_msg_id_valid(td_u32 msg_id)
{
    switch (msg_id) {
        case EXT_UPG_START:
        case EXT_UPG_STOP:
        case EXT_UPG_STATUS_BS_IND:
        case EXT_UPG_STAT_INFO_IND:
        case EXT_UPG_STATUS:
            return TD_TRUE;

        default:
            return TD_FALSE;
    }
}

static td_u32 remote_upg_msg_rx(td_u32 msg_id, td_pbyte packet, td_u16 packet_size)
{
    ext_sys_queue_msg msg;
    td_pbyte payload;
    td_u16 payload_size = packet_size;
    td_u32 ret;

    if ((packet == TD_NULL) || (packet_size == 0)) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    if (upg_msg_id_valid(msg_id) != TD_TRUE) {
        return EXT_ERR_INVALID_ID;
    }

    payload = (td_pbyte)uapi_malloc(EXT_MOD_ID_APP_COMMON, payload_size);
    if (payload == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }

    (td_void) memcpy_s(payload, payload_size, packet, packet_size);

    msg.msg_id = ID_APP_MSG_TRANS_FILE;
    msg.param[0] = (uintptr_t)msg_id;
    msg.param[1] = (uintptr_t)payload;
    msg.param[UPG_MSG_PARAM_INDEX2] = (uintptr_t)packet_size;
    msg.param[UPG_MSG_PARAM_INDEX3] = 0;
    ret = app_send_message(&msg);
    if (ret != EXT_ERR_SUCCESS) {
        uapi_free(EXT_MOD_ID_APP_COMMON, payload);
        payload = TD_NULL;
    }

    return ret;
}

static td_u32 remote_upg_proc(trans_file_context *ctx)
{
    td_u32 ret;

    if (ctx == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    ctx->stage = TRANS_FILE_STAGE_UPG;
    app_timer_stop(APP_TIMER_ID_TRANS_FILE_RX);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    ctx->wait_chl = TD_FALSE;
    app_timer_stop(APP_TIMER_ID_WAIT_PLC_AVAILABLE);
    if (!app_is_plc_available() && (ctx->file_attr == PROTO_FILE_ATTR_REMOTE)) {
        ctx->wait_chl = TD_TRUE;
        app_timer_start(APP_TIMER_ID_WAIT_PLC_AVAILABLE, UPG_WAIT_PLC_CHL_READY, EXT_TIMER_TYPE_ONCE);
        return EXT_ERR_SUCCESS;
    }
#endif

    ret = remote_upg_start(ctx);
    if (ret != EXT_ERR_SUCCESS) {
        trans_file_destroy();
        return ret;
    }

    ctx->allow_stop = TD_FALSE;

    ret = remote_upg_status(EXT_UPG_STATUS_REQ_OPT_BS);
    if (ret != EXT_ERR_SUCCESS) {
    }
    app_timer_start(APP_TIMER_ID_TRANS_FILE_STATUS_QRY, UPG_STATUS_QUERY_INTERVAL, EXT_TIMER_TYPE_PERIOD);
    return EXT_ERR_SUCCESS;
}

static td_u32 remote_upg_start(EXT_CONST trans_file_context *ctx)
{
    ext_upg_start_req *req = TD_NULL;
    td_u32 ret;
    td_u8 i;

    if (ctx == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    req = (ext_upg_start_req *)uapi_malloc(EXT_MOD_ID_APP_COMMON, sizeof(ext_upg_start_req));
    if (req == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }

    (td_void) memset_s(req, sizeof(ext_upg_start_req), 0, sizeof(ext_upg_start_req));

    if (ctx->file_attr == PROTO_FILE_ATTR_LOCAL) {
        req->cfg.option = UPG_FILE_OPTION_LOCAL | EXT_UPG_VER_UPDATE_OPTION_START;
    } else {
        req->cfg.option = UPG_FILE_OPTION_REMOTE | EXT_UPG_VER_UPDATE_OPTION_START;
    }

    req->cfg.file_len = ctx->file_len;
    req->cfg.file_crc = ctx->file_crc;
    req->cfg.upg_id = ctx->file_crc;
    req->cfg.upg_time_window = ctx->timeout;
    req->cfg.blk_num = 1; /* Fixed value: 1 */

    req->file.upg_id = req->cfg.upg_id;
    req->file.idx = 1; /* Fixed value: 1 */

    for (i = 0; i < UPG_MSG_SEND_CNT; i++) {
        ret = uapi_upg_msg_send(EXT_UPG_START, (td_pbyte)req, sizeof(ext_upg_start_req));
        if (ret == EXT_ERR_SUCCESS) {
            break;
        }
    }

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    if (ret == EXT_ERR_SUCCESS) {
        trans_file_save_file_inf(ctx->file_crc, ctx->file_attr);
    }
#endif

    return ret;
}

static td_u32 remote_upg_stop(trans_file_context *ctx)
{
    ext_upg_stop_req req;
    td_u32 ret;
    td_u8 i;

    if (ctx == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    (td_void) memset_s(&req, sizeof(req), 0, sizeof(req));
    req.stop_req.upg_id = ctx->file_crc;

    for (i = 0; i < UPG_MSG_SEND_CNT; i++) {
        ret = uapi_upg_msg_send(EXT_UPG_STOP, (td_pbyte)&req, sizeof(req));
        if (ret != EXT_ERR_SUCCESS) {
            continue;
        }

        ctx->stop_times++;
        if (ctx->clear_flag == TD_FALSE) {
            break;
        }

        ret = EXT_ERR_CONTINUE;
        app_timer_start(APP_TIMER_ID_TRANS_FILE_STOP_RETRY, APP_TIMER_CLEAR_FILE_TIMEOUT, EXT_TIMER_TYPE_ONCE);
        break;
    }

    return ret;
}

static td_u32 remote_upg_status(td_u32 option)
{
    ext_upg_status_req req;
    td_u32 ret;
    td_u8 i;

    (td_void) memset_s(&req, sizeof(req), 0, sizeof(req));
    req.status_req.option = option;

    for (i = 0; i < UPG_MSG_SEND_CNT; i++) {
        ret = uapi_upg_msg_send(EXT_UPG_STATUS, (td_pbyte)&req, sizeof(req));
        if (ret == EXT_ERR_SUCCESS) {
            return EXT_ERR_SUCCESS;
        }
    }

    return EXT_ERR_FAILURE;
}

static td_void remote_upg_start_msg_rx(td_pbyte packet, td_u16 packet_size)
{
    ext_upg_start_ind *ind = (ext_upg_start_ind *)packet;

    if (ind == TD_NULL) {
        return;
    }
    if (packet_size != sizeof(ext_upg_start_ind)) {
        return;
    }

    if (ind->rc != EXT_ERR_SUCCESS) {
        trans_file_destroy();
    }
}

static td_void remote_upg_stop_msg_rx(td_pbyte packet, td_u16 packet_size)
{
    ext_upg_stop_ind *ind = (ext_upg_stop_ind *)packet;
    trans_file_context *ctx = get_trans_file_context();

    if (ind == TD_NULL || ctx == TD_NULL || (packet_size != sizeof(ext_upg_stop_ind))) {
        return;
    }

    app_timer_stop(APP_TIMER_ID_TRANS_FILE_STOP_RETRY);
    ctx->stop_times = 0;

    if (ctx->clear_flag != TD_TRUE) {
        if (ind->stop_ind.upg_id == 0) {
            trans_file_destroy();
        }
        return;
    }

    if (ind->stop_ind.upg_id == 0) {
        return;
    }

    if ((ind->stop_ind.rc == EXT_ERR_SUCCESS) || (ind->stop_ind.rc == EXT_ERR_SUM_INVALID_UPG_ID)) {
        trans_file_clear_reply(ctx, PROTO_RET_CODE_SUCCESS, PROTO_REASON_NORMAL);
        trans_file_destroy();
    } else if (ctx->clear_resp == TD_FALSE) {
        ctx->clear_flag = TD_FALSE;
        ctx->clear_resp = TD_TRUE;
        trans_file_clear_reply(ctx, PROTO_RET_CODE_FAILED, PROTO_REASON_OTHER);
    }
    return;
}

static td_void remote_upg_handle_status_ok(trans_file_context *ctx, ext_upg_status_bs_ind *ind)
{
    if (ctx == TD_NULL || ind == TD_NULL) {
        return;
    }
    if (ctx->file_attr == PROTO_FILE_ATTR_REMOTE) {
        /* Query all info */
        app_timer_stop(APP_TIMER_ID_TRANS_FILE_STATUS_QRY);
        ctx->retry = 0;

        if (remote_upg_status(EXT_UPG_STATUS_REQ_OPT_NA) != EXT_ERR_SUCCESS) {
        }
        app_timer_start(APP_TIMER_ID_TRANS_FILE_QRY_INFO, UPG_STATUS_QUERY_INTERVAL, EXT_TIMER_TYPE_ONCE);
        ctx->retry++;

        return;
    }

    if (ind->upg_status == EXT_UPG_STATUS_OK) {
        ctx->result = PROTO_TRANS_FILE_DONE;
    } else {
        trans_file_failed_proc();
    }
}

static td_void remote_upg_status_bs_msg_rx(td_pbyte packet, td_u16 packet_size)
{
    trans_file_context *ctx = get_trans_file_context();
    ext_upg_status_bs_ind *ind = (ext_upg_status_bs_ind *)packet;

    if (packet_size != sizeof(ext_upg_status_bs_ind)) {
        return;
    }

    app_timer_stop(APP_TIMER_ID_TRANS_FILE_QRY_ACTIVE);
    ctx->status_msg_cnt = 0;
    ctx->allow_stop = TD_TRUE;

    if (ind == TD_NULL) {
        return;
    }

    if ((ind->upg_status == EXT_UPG_STATUS_OK) || (ind->upg_status == EXT_UPG_STATUS_UNKNOWN)) {
        if (ctx->stage == TRANS_FILE_STAGE_UPG) {
            remote_upg_handle_status_ok(ctx, ind);

            return;
        }

        ctx->result = PROTO_TRANS_FILE_DONE;
        app_timer_stop(APP_TIMER_ID_TRANS_FILE_STATUS_QRY);
        return;
    } else if ((ind->upg_status == EXT_UPG_STATUS_END) || (ind->upg_status == EXT_UPG_STATUS_PROCESS)) {
        if (ind->upg_id == ctx->file_crc) {
            ctx->stage = TRANS_FILE_STAGE_UPG;
            ctx->result = PROTO_TRANS_FILE_INPROGRESS;
            return;
        }

        app_timer_stop(APP_TIMER_ID_TRANS_FILE_STATUS_QRY);

        if (ctx->stage == TRANS_FILE_STAGE_UPG) {
            trans_file_failed_proc();
            trans_file_destroy();
        }

        return;
    } else if (ind->upg_status == EXT_UPG_STATUS_FAIL) {
        trans_file_failed_proc();
        trans_file_destroy();
    }
}

static td_void remote_upg_stat_info_msg_rx(td_pbyte packet, td_u16 packet_size)
{
    trans_file_context *ctx = get_trans_file_context();
    ext_upg_stat_info_ind *ind = (ext_upg_stat_info_ind *)packet;

    if (ind == TD_NULL) {
        return;
    }
    if (packet_size != sizeof(ext_upg_stat_info_ind)) {
        return;
    }

    ctx->stat.success = ind->stat_info.ok_num;
    ctx->stat.failed = ind->stat_info.fail_num;
    ctx->stat.process = ind->stat_info.process_num;
    ctx->stat.complete = ind->stat_info.complete_num;

    if (ctx->stat.failed == 0) {
        trans_file_destroy();
        ctx->result = PROTO_TRANS_FILE_DONE;
        return;
    } else {
        ctx->result = PROTO_TRANS_FILE_UNCOMPLETE;
    }

    app_timer_start(APP_TIMER_ID_TRANS_FILE_QRY_INFO, UPG_STATUS_QUERY_INTERVAL, EXT_TIMER_TYPE_ONCE);
}

static td_void remote_upg_status_msg_rx(td_pbyte packet, td_u16 packet_size)
{
    trans_file_context *ctx = get_trans_file_context();
    ext_upg_status_ind *ind = (ext_upg_status_ind *)packet;
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN] = { 0 };

    if (ind == TD_NULL) {
        return;
    }
    if (packet_size != sizeof(ext_upg_status_ind)) {
        return;
    }

    uapi_get_my_mac(mac, sizeof(mac));
    if ((ind->status_ind.num == 1) && (memcmp(mac, ind->status_ind.entry->mac, EXT_PLC_MAC_ADDR_LEN) == 0)) {
        return;
    }

    app_timer_start(APP_TIMER_ID_TRANS_FILE_QRY_INFO, UPG_STATUS_QUERY_INTERVAL, EXT_TIMER_TYPE_ONCE);

    trans_file_destroy();
    if (ctx->stat.failed == 0) {
        ctx->result = PROTO_TRANS_FILE_DONE;
    } else {
        ctx->result = PROTO_TRANS_FILE_UNCOMPLETE;
    }
}

td_u32 app_on_message_trans_file(EXT_CONST ext_sys_queue_msg *msg)
{
    trans_file_msg_pair msg_pair[] = {
        { EXT_UPG_START,         remote_upg_start_msg_rx },
        { EXT_UPG_STOP,          remote_upg_stop_msg_rx },
        { EXT_UPG_STATUS_BS_IND, remote_upg_status_bs_msg_rx },
        { EXT_UPG_STAT_INFO_IND, remote_upg_stat_info_msg_rx },
        { EXT_UPG_STATUS,        remote_upg_status_msg_rx },
    };

    td_u32 i;
    td_u16 msg_pair_num = sizeof(msg_pair) / sizeof(trans_file_msg_pair);

    if (msg == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    for (i = 0; i < msg_pair_num; i++) {
        if (msg->param[0] == msg_pair[i].msg_id) {
            msg_pair[i].msg_proc((td_pbyte)msg->param[1], (td_u16)msg->param[UPG_MSG_PARAM_INDEX2]);
            break;
        }
    }

    uapi_free(EXT_MOD_ID_APP_COMMON, (td_pvoid)msg->param[1]);

    return EXT_ERR_SUCCESS;
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
td_void poweron_active_query(trans_file_context *ctx)
{
    if (remote_upg_status(EXT_UPG_STATUS_REQ_OPT_BS) != EXT_ERR_SUCCESS) {
    }
    ctx->status_msg_cnt++;
    app_timer_start(APP_TIMER_ID_TRANS_FILE_QRY_ACTIVE, UPG_STATUS_QUERY_TIME_OUT, EXT_TIMER_TYPE_ONCE);
    app_timer_start(APP_TIMER_ID_TRANS_FILE_STATUS_QRY, UPG_STATUS_QUERY_INTERVAL, EXT_TIMER_TYPE_PERIOD);
}

td_void trans_file_save_file_inf(td_u32 filc_crc, td_u8 file_attr)
{
    app_upg_inf nv;

    (td_void) memset_s(&nv, sizeof(nv), 0, sizeof(nv));
    (td_void) uapi_nv_read(ID_NV_APP_UPG_INF, (EXT_CONST td_pvoid)&nv, sizeof(nv));
    nv.filc_crc = filc_crc;
    nv.file_attr = file_attr;
    (td_void) uapi_nv_write(ID_NV_APP_UPG_INF, (EXT_CONST td_pvoid)&nv, sizeof(nv));
}
#endif
