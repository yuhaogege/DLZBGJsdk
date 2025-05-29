/*
 * Copyright (c) CompanyNameMagicTag 2019-2020. All rights reserved.
 * Description: plc packet test of application
 */

#include "app_plc_test.h"
#include "soc_mdm_mem.h"
#include "app_common.h"

#if defined(PRODUCT_CFG_PLC_MODULE_APP_TEST)

#define ID_PLC_TEST_FRAME                           (0x5ffe)
#define ID_APP_MSG_PLC_TEST_TCP_EVENT               (0x5ffd)
#define ID_PLC_TEST_FRAME_UDP_ACTION                (0x5ffc)
#define ID_PLC_TEST_FRAME_UDP                       (0x5ffb)
#define ID_PLC_TEST_FRAME_UDP_INFO                  (0x5ffa)
#define ID_APP_MSG_PLC_TEST_UDP_EVENT               (0x5ff9)
#define ID_APP_MSG_PLC_TEST_OUTTIME_END_EVENT       (0x5ff8)
#define ID_APP_MSG_PLC_TEST_SEARCH_EVENT            (0x5ff7)
#define ID_UDP_MSG_PLC_TEST_SEND_EVENT              (0x5ff6)  /* send frame in udp mode */
#define ID_UDP_MSG_PLC_TEST_SEARCH_EVENT            (0x5ff5)
#define ID_APP_MSG_PLC_TEST_END_EVENT               (0x5ff4)

#define PLC_TEST_ACTION_START     1
#define PLC_TEST_ACTION_STOP      0

typedef struct {
    td_u16 id;                      /* PLC packet id */
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN]; /* destination address */
    td_u16 option;                  /* optional field */
    td_u16 payload_length;          /* payload length */
    td_pbyte payload;               /* data frame payload */
    td_u8 data[0];
} app_plc_frame_data;

typedef struct {
    td_u16 crc_16;      /* CRC16 check: CRC16-CCITT */
    td_u16 data_length; /* data length */
    td_u8 data[0];
} app_plc_trans_frame;

typedef struct {
    td_u16 index;          /* index of nodes in the global */
    td_u16 data_length;
    td_u32 frame_index;    /* message sequence number */
    td_u32 total_count;
    td_u32 up_time;        /* ms */
    td_u32 down_time;      /* ms */
    td_u32 cco_count_time; /* ms */
    td_u8 data[0];
} plc_test_frame;

typedef struct {
    td_u16 total_time;
    td_u8 action;
    td_u8 index;            /* index of nodes in the global */
    td_u8 ret_code;
    td_u8 pad[3];
} plc_test_udp_action_frame;

typedef struct {
    td_u32 count;
    td_u32 down_time;
    td_u32 total_count;
    td_u16 index;           /* index of nodes in the global */
    td_u8 data[2];
} plc_test_udp_info_frame;

app_send_msg_func g_app_send_msg = TD_NULL;

td_u32 app_plc_test_send(td_pbyte data, td_u32 data_length, td_u8 des_addr[EXT_PLC_MAC_ADDR_LEN], td_u16 frame_id);
td_u32 app_proto_plc_test_udp_action(td_pbyte data, td_u16 data_length, td_pbyte src_addr);
td_void uapi_timer_define(app_plc_test_end_call_back);
td_u8 g_cmd_option = EXT_DIAG_CMD_INSTANCE_DEFAULT;

td_void app_send_msg_setup(app_send_msg_func function)
{
    g_app_send_msg = function;
}

app_send_msg_func get_app_send_msg_func(td_void)
{
    return g_app_send_msg;
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
typedef struct {
    td_u32 success_count;
    td_u32 total_count;
    td_u32 success_up_time;   /* ms */
    td_u32 success_down_time; /* ms */
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN];
    td_u8 retrans_action_times;
    td_u8 retrans_search_times;
    td_u8 pad[0];
} plc_test_station_info;

typedef struct {
    td_u32 frame_index;          /* message sequence number */
    td_u32 frame_rounds;
    td_u32 start_time;           /* start time */
    td_u64 dialog_start_time;
    td_u16 index;                /* to which station it was send in TCP mode */
    td_u16 total_num;            /* total number of nodes to be received */
    td_u16 current_num;          /* number of received nodes */
    td_u16 frame_length;         /* length of sending message */
    td_u16 total_time;
    td_u16 opt_sta_index;        /* to which station it was send in operation of search info and search status */
    td_u16 timeout;
    td_u8 statue;                /* 0:idle,  1:testing */
    td_u8 parallel_num;          /* expected parallel number */
    td_u8 current_parallel_num;  /* current parallel number */
    td_u8 only_cco_count_time;   /* mode:&0x1:only calculate total time */
    td_u8 send_interval;
    td_u8 refresh_interval;
    td_u8 test_mode;             /* 0:TCP 1:UDP */
    td_u8 action;                /* 1:start 0:stop */
    td_u8 pad[2];
    plc_test_station_info data[0];
} plc_test_controller;

typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN];
    td_u8 pad[2];
    ext_timer_handle_s plc_send_timer; /* timer */
} plc_test_mac_to_timer_info;

typedef struct {
    td_u8 num;
    td_u8 pad[3];
    plc_test_mac_to_timer_info data[0]; /* timer */
} plc_test_mac_to_timer_controller;

#define PLC_TEST_MAX_NUM                 2047 /* it's 500 now, don't know which macro is 2047 */
#define PLC_TEST_MAX_PARALLEL_NUM        20
#define INVALID_MAC_ADDR                 "\x00\x00\x00\x00\x00\x00"
#define is_invalid_mac_addr(x)           (memcmp((x), INVALID_MAC_ADDR, EXT_PLC_MAC_ADDR_LEN) == 0)
#define PLC_TEST_STATUS_START            1
#define PLC_TEST_STATUS_STOP             0
#define PLC_TEST_TCP_MODE                0
#define PLC_TEST_UDP_MODE                1
#define APP_SRV_TASK_STACK_SIZE          1024
#define APP_SRV_TASK_PRIORITY            25
#define NDM_PLC_TEST_END_TIMER           1
#define PLC_TEST_PER_BATCH_MAX           20
#define PLC_TEST_PER_RETRANS_NUM         1

typedef struct {
    td_pbyte plc_test_frame;
    plc_test_controller *plc_test_ctrl;
    plc_test_mac_to_timer_controller *mac_to_timer_ctrl;
    ext_timer_handle_s *g_plc_end_cco_timer;
    td_u32 task_id;
    td_u32 queue_id;
    td_u16 start_stop_sigle;
    td_u16 search_info_sigle;
    td_bool is_search_before_end;
    td_u8 pad[3];
} plc_test_ctx_stru;

plc_test_ctx_stru *g_plc_test_ctx = TD_NULL;

td_u32 plc_test_tcp_process(td_u32 is_start);
td_u32 init_plc_test_ctrl(diag_cmd_plc_test_req *req);
td_void uapi_timer_define(app_plc_test_call_back);
td_void app_plc_test_release_resources(td_void);
td_void plc_test_main_process(td_u32 action);
td_u32 plc_test_udp_info(td_void);
td_void plc_test_udp_task_body(td_u32 param);
td_void uapi_timer_define(app_plc_test_udp_call_back);
td_u32 plc_test_statistic_info(td_void);
td_void uapi_timer_define(app_plc_test_udp_search_call_back);
td_void app_handle_plc_test_udp_event(ext_sys_queue_msg *msg);
td_void plc_test_udp_send_frame(td_void);
td_u32 plc_test_udp_process_action(td_u32 action);
td_u32 plc_test_udp_startstop_single(td_u8 action, td_u16 index);
td_u32 plc_test_udp_info_single(td_u16 index);
td_u32 stop_all_timer(td_void);

plc_test_ctx_stru *get_plc_test_ctx(td_void)
{
    return g_plc_test_ctx;
}

td_void app_plc_test_release_resources(td_void)
{
    if (g_plc_test_ctx == TD_NULL) {
        return;
    }
    if (g_plc_test_ctx->plc_test_ctrl != TD_NULL) {
        uapi_free(EXT_MOD_ID_APP_COMMON, g_plc_test_ctx->plc_test_ctrl);
        g_plc_test_ctx->plc_test_ctrl = TD_NULL;
    }

    if (g_plc_test_ctx->mac_to_timer_ctrl != TD_NULL) {
        for (td_u32 i = 0; i < g_plc_test_ctx->mac_to_timer_ctrl->num; i++) {
            (td_void)uapi_timer_delete(&g_plc_test_ctx->mac_to_timer_ctrl->data[i].plc_send_timer);
        }
        uapi_free(EXT_MOD_ID_APP_COMMON, g_plc_test_ctx->mac_to_timer_ctrl);
        g_plc_test_ctx->mac_to_timer_ctrl = TD_NULL;
    }

    if (g_plc_test_ctx->g_plc_end_cco_timer != TD_NULL) {
        (td_void)uapi_timer_delete(g_plc_test_ctx->g_plc_end_cco_timer);
        uapi_free(EXT_MOD_ID_APP_COMMON, g_plc_test_ctx->g_plc_end_cco_timer);
        g_plc_test_ctx->g_plc_end_cco_timer = TD_NULL;
    }

    if (g_plc_test_ctx->plc_test_frame != TD_NULL) {
        uapi_free(EXT_MOD_ID_APP_COMMON, g_plc_test_ctx->plc_test_frame);
        g_plc_test_ctx->plc_test_frame = TD_NULL;
    }

    if (g_plc_test_ctx != TD_NULL) {
        uapi_free(EXT_MOD_ID_APP_COMMON, g_plc_test_ctx);
        g_plc_test_ctx = TD_NULL;
    }
}

td_void build_plc_test_ctrl(diag_cmd_plc_test_req *req)
{
    plc_test_ctx_stru *ctx = get_plc_test_ctx();
    if (ctx == TD_NULL) {
        return;
    }
    plc_test_controller *plc_test_ctrl = ctx->plc_test_ctrl;
    if (plc_test_ctrl == TD_NULL) {
        return;
    }
    plc_test_ctrl->total_num = req->total_num;
    plc_test_ctrl->frame_index = 1;
    plc_test_ctrl->frame_rounds = 1;
    plc_test_ctrl->parallel_num = req->parallel_num;
    plc_test_ctrl->current_num = 0;
    plc_test_ctrl->only_cco_count_time = req->only_cco_count_time;
    plc_test_ctrl->start_time = uapi_get_milli_seconds();
    plc_test_ctrl->frame_length =
        req->frame_length < sizeof(plc_test_frame) ? sizeof(plc_test_frame) : req->frame_length;
    plc_test_ctrl->test_mode = req->test_mode;
    plc_test_ctrl->send_interval = req->send_interval;
    plc_test_ctrl->action = req->action;
    plc_test_ctrl->total_time = req->total_time;
    plc_test_ctrl->timeout = req->timeout;
    plc_test_ctrl->dialog_start_time = req->dialog_start_time;
    plc_test_ctrl->refresh_interval = req->refresh_interval;
    plc_test_ctrl->opt_sta_index = 0;
}

td_u32 app_plc_test_create_resources(diag_cmd_plc_test_req *req)
{
    if (req->total_num > PLC_TEST_MAX_NUM || req->frame_length < sizeof(plc_test_frame) ||
        req->frame_length > APP_PLC_TEST_FRAME_LEN || req->parallel_num > PLC_TEST_MAX_PARALLEL_NUM) {
        return DIAG_CMD_PLC_TEST_IND_RET_INVALID_PARAMENT;
    }

    g_plc_test_ctx = (plc_test_ctx_stru *)uapi_malloc(EXT_MOD_ID_APP_COMMON, sizeof(plc_test_ctx_stru));
    if (g_plc_test_ctx == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }
    memset_s(g_plc_test_ctx, sizeof(plc_test_ctx_stru), 0, sizeof(plc_test_ctx_stru));
    g_plc_test_ctx->plc_test_ctrl = (plc_test_controller *)uapi_malloc(EXT_MOD_ID_APP_COMMON,
        sizeof(plc_test_controller) + req->total_num * sizeof(plc_test_station_info));
    if (g_plc_test_ctx->plc_test_ctrl == TD_NULL) {
        app_plc_test_release_resources();
        return EXT_ERR_MALLOC_FAILUE;
    }
    (td_void)memset_s(g_plc_test_ctx->plc_test_ctrl,
                      sizeof(plc_test_controller) + req->total_num * sizeof(plc_test_station_info),
                      0x00, sizeof(plc_test_controller) + req->total_num * sizeof(plc_test_station_info));
    build_plc_test_ctrl(req);

    td_u32 timer_ctrl_size = sizeof(plc_test_mac_to_timer_controller) +
                      req->parallel_num * sizeof(plc_test_mac_to_timer_info);
    g_plc_test_ctx->mac_to_timer_ctrl =
        (plc_test_mac_to_timer_controller *)uapi_malloc(EXT_MOD_ID_APP_COMMON, timer_ctrl_size);
    g_plc_test_ctx->g_plc_end_cco_timer =
        (ext_timer_handle_s *)uapi_malloc(EXT_MOD_ID_APP_COMMON, sizeof(ext_timer_handle_s));
    g_plc_test_ctx->plc_test_frame =
        (td_pbyte)uapi_malloc(EXT_MOD_ID_APP_COMMON, g_plc_test_ctx->plc_test_ctrl->frame_length);

    if (g_plc_test_ctx->mac_to_timer_ctrl == TD_NULL ||  g_plc_test_ctx->g_plc_end_cco_timer == TD_NULL ||
        g_plc_test_ctx->plc_test_frame == TD_NULL) {
            app_plc_test_release_resources();
            return EXT_ERR_MALLOC_FAILUE;
    }
    memset_s(g_plc_test_ctx->mac_to_timer_ctrl, timer_ctrl_size, 0, timer_ctrl_size);
    g_plc_test_ctx->mac_to_timer_ctrl->num = req->parallel_num;
    memset_s(g_plc_test_ctx->g_plc_end_cco_timer, sizeof(ext_timer_handle_s), 0, sizeof(ext_timer_handle_s));
    memset_s(g_plc_test_ctx->plc_test_frame, sizeof(plc_test_frame), 0, sizeof(plc_test_frame));
    return EXT_ERR_SUCCESS;
}

td_u32 init_plc_test_ctrl(diag_cmd_plc_test_req *req)
{
    if (req->action != PLC_TEST_ACTION_START) {
        return DIAG_CMD_PLC_TEST_IND_RET_STOP_SUCCESS;
    }

    if (g_plc_test_ctx == TD_NULL) {
        td_u32 ret = app_plc_test_create_resources(req);
        if (ret != EXT_ERR_SUCCESS) {
            return ret;
        }
    }

    if (g_plc_test_ctx->start_stop_sigle > 0) {
        return DIAG_CMD_PLC_TEST_IND_RET_INITIALIZING;
    }

    if (g_plc_test_ctx->plc_test_ctrl->statue == PLC_TEST_STATUS_START) {
        return DIAG_CMD_PLC_TEST_IND_RET_BUSY;
    }

    if (req->num > sizeof(req->mac) / EXT_PLC_MAC_ADDR_LEN) {
        req->num = sizeof(req->mac) / EXT_PLC_MAC_ADDR_LEN;
    }

    if (g_plc_test_ctx->plc_test_ctrl->current_num + req->num > g_plc_test_ctx->plc_test_ctrl->total_num) {
        return DIAG_CMD_PLC_TEST_IND_RET_INVALID_PARAMENT;
    }
    for (int i = 0; i < req->num; i++) {
        if (memcpy_s(g_plc_test_ctx->plc_test_ctrl->data[g_plc_test_ctx->plc_test_ctrl->current_num + i].mac,
            EXT_PLC_MAC_ADDR_LEN, req->mac + i * EXT_PLC_MAC_ADDR_LEN, EXT_PLC_MAC_ADDR_LEN) != 0) {
            continue;
        }
    }

    g_plc_test_ctx->plc_test_ctrl->current_num += req->num;

    if (req->is_last_frame == 1) {
        return DIAG_CMD_PLC_TEST_IND_RET_START_SUCCESS;
    } else {
        return DIAG_CMD_PLC_TEST_IND_RET_PREPARING;
    }
}
td_u32 plc_test_udp_process(td_u32 action)
{
    td_u32 ret = 0;
    plc_test_ctx_stru *ctx = get_plc_test_ctx();
    plc_test_controller *plc_test_ctrl = ctx->plc_test_ctrl;
    if (action == PLC_TEST_ACTION_STOP && plc_test_ctrl->action == PLC_TEST_ACTION_START) {
        uapi_task_delete(ctx->task_id);
        uapi_msg_queue_delete(ctx->queue_id);
        ret |= stop_all_timer();
        plc_test_ctrl->current_parallel_num = 0;
        plc_test_ctrl->index = 0;
        plc_test_ctrl->action = PLC_TEST_ACTION_STOP;
    } else if (action == PLC_TEST_ACTION_START && plc_test_ctrl->action == PLC_TEST_ACTION_STOP) {
        plc_test_ctrl->action = PLC_TEST_ACTION_START;
    }
    while (plc_test_ctrl->current_parallel_num < plc_test_ctrl->parallel_num &&
           plc_test_ctrl->index < plc_test_ctrl->total_num) {
        ret |= plc_test_udp_startstop_single(action, plc_test_ctrl->index);
        plc_test_ctrl->index++;
    }
    return ret;
}

td_u32 plc_test_tcp_process(td_u32 action)
{
    td_u32 ret = 0;
    plc_test_ctx_stru *ctx = get_plc_test_ctx();
    plc_test_controller *plc_test_ctrl = ctx->plc_test_ctrl;
    plc_test_mac_to_timer_controller *mac_to_timer_ctrl = ctx->mac_to_timer_ctrl;
    td_pbyte plc_test_frame_buff = ctx->plc_test_frame;
    if (mac_to_timer_ctrl == TD_NULL || plc_test_frame_buff == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    if (action != PLC_TEST_ACTION_START) {
        app_plc_test_release_resources();
        return ret;
    }
    plc_test_ctrl->statue = PLC_TEST_STATUS_START;
    while (plc_test_ctrl->current_parallel_num < plc_test_ctrl->parallel_num) {
        if ((plc_test_ctrl->index >= plc_test_ctrl->total_num && plc_test_ctrl->current_parallel_num > 0) ||
            plc_test_ctrl->total_num == 0) {
            break;
        } else if (plc_test_ctrl->index >= plc_test_ctrl->total_num) {
            plc_test_ctrl->frame_index++;
            plc_test_ctrl->index = 0;
        }
        /* memory can be allocated once during initialization, and global variables can be saved */
        plc_test_frame *frame = (plc_test_frame *)plc_test_frame_buff;
        if (frame == TD_NULL) {
            return EXT_ERR_MALLOC_FAILUE;
        }
        frame->index = plc_test_ctrl->index;
        frame->frame_index = plc_test_ctrl->frame_index;
        frame->data_length = plc_test_ctrl->frame_length - sizeof(plc_test_frame);
        td_bool is_sync;
        frame->down_time = uapi_get_ntb(&is_sync);
        frame->cco_count_time = frame->down_time;
        ret = app_plc_test_send((td_pbyte)frame, plc_test_ctrl->frame_length,
                                plc_test_ctrl->data[plc_test_ctrl->index].mac, ID_PLC_TEST_FRAME);
        for (td_u32 i = 0; i < mac_to_timer_ctrl->num; i++) {
            if (is_invalid_mac_addr(mac_to_timer_ctrl->data[i].mac)) {
                (td_void)uapi_timer_start(&mac_to_timer_ctrl->data[i].plc_send_timer,
                    (timer_proc_func)app_plc_test_call_back, plc_test_ctrl->timeout,
                    EXT_TIMER_TYPE_ONCE, i);
                (td_void)memcpy_s(mac_to_timer_ctrl->data[i].mac, EXT_PLC_MAC_ADDR_LEN,
                    plc_test_ctrl->data[plc_test_ctrl->index].mac, EXT_PLC_MAC_ADDR_LEN);
                break;
            }
        }
        plc_test_ctrl->current_parallel_num++;
        plc_test_ctrl->index++;
    }
    return ret;
}

__hot td_void uapi_timer_define(app_plc_test_call_back)
{
    plc_test_ctx_stru *ctx = get_plc_test_ctx();
    if (ctx == TD_NULL) {
        return;
    }
    plc_test_mac_to_timer_controller *mac_to_timer_ctrl = ctx->mac_to_timer_ctrl;

    if (mac_to_timer_ctrl != TD_NULL) {
        (td_void)memset_s(&mac_to_timer_ctrl->data[ulData].mac, EXT_PLC_MAC_ADDR_LEN, 0, EXT_PLC_MAC_ADDR_LEN);
    }

    ext_sys_queue_msg msg;
    memset_s(&msg, sizeof(msg), 0, sizeof(msg));
    msg.msg_id = ID_APP_MSG_PLC_TEST_TCP_EVENT;

    app_send_msg_func app_send_func = get_app_send_msg_func();
    if (app_send_func != TD_NULL) {
        app_send_func(&msg);
    }
}

td_void plc_test_stop_timer(td_u8 index)
{
    plc_test_ctx_stru *ctx = get_plc_test_ctx();
    plc_test_controller *plc_test_ctrl = ctx->plc_test_ctrl;
    plc_test_mac_to_timer_controller *mac_to_timer_ctrl = ctx->mac_to_timer_ctrl;
    if (mac_to_timer_ctrl == TD_NULL) {
        return;
    }
    td_u32 i;
    for (i = 0; i < mac_to_timer_ctrl->num; i++) {
        if (memcmp(plc_test_ctrl->data[index].mac, mac_to_timer_ctrl->data[i].mac,
            EXT_PLC_MAC_ADDR_LEN) == 0) {
            (td_void)uapi_timer_stop(&mac_to_timer_ctrl->data[i].plc_send_timer);
            (td_void)memset_s(&mac_to_timer_ctrl->data[i].mac, EXT_PLC_MAC_ADDR_LEN, 0,
                              EXT_PLC_MAC_ADDR_LEN);
            break;
        }
    }
}

td_void plc_test_tcp_event(td_void)
{
    plc_test_ctx_stru *ctx = get_plc_test_ctx();
    if (ctx == TD_NULL) {
        return;
    }
    plc_test_controller *plc_test_ctrl = ctx->plc_test_ctrl;
    if (plc_test_ctrl == TD_NULL) {
        return;
    }
    plc_test_ctrl->current_parallel_num--;
    if (plc_test_tcp_process(PLC_TEST_ACTION_START) != EXT_ERR_SUCCESS) {
        return;
    }
}

td_void plc_test_udp_event(ext_sys_queue_msg *msg)
{
    plc_test_ctx_stru *ctx = get_plc_test_ctx();
    plc_test_controller *plc_test_ctrl = ctx->plc_test_ctrl;
    plc_test_mac_to_timer_controller *mac_to_timer_ctrl = ctx->mac_to_timer_ctrl;
    td_u16 index = msg->param[0];

    if (mac_to_timer_ctrl != TD_NULL) {
        for (int i = 0; i < mac_to_timer_ctrl->num; i++) {
            if (memcmp(plc_test_ctrl->data[index].mac, mac_to_timer_ctrl->data[i].mac,
                EXT_PLC_MAC_ADDR_LEN) == 0) {
                (td_void)memset_s(&mac_to_timer_ctrl->data[i].mac, EXT_PLC_MAC_ADDR_LEN, 0, EXT_PLC_MAC_ADDR_LEN);
                break;
            }
        }
    }

    plc_test_ctrl->current_parallel_num--;
    td_u32 ret = plc_test_udp_startstop_single(plc_test_ctrl->action, index);
    if (ret != EXT_ERR_SUCCESS) {
        return;
    }
}

void plc_test_search_event(ext_sys_queue_msg *msg)
{
    plc_test_ctx_stru *ctx = get_plc_test_ctx();
    if (ctx == TD_NULL) {
        return;
    }
    plc_test_controller *plc_test_ctrl = ctx->plc_test_ctrl;
    if (plc_test_ctrl == TD_NULL) {
        return;
    }
    plc_test_ctrl->current_parallel_num--;
    td_u32 index = msg->param[0];
    plc_test_stop_timer(index);
    if (ctx->is_search_before_end) {
        plc_test_udp_info_single(index);
    } else {
        ctx->search_info_sigle--;
        ext_sys_queue_msg send_msg;
        memset_s(&send_msg, sizeof(send_msg), 0, sizeof(send_msg));
        send_msg.msg_id = ID_UDP_MSG_PLC_TEST_SEARCH_EVENT;
        uapi_msg_queue_send(ctx->queue_id, &send_msg, 0, sizeof(ext_sys_queue_msg));
    }
}

void plc_test_timeout_end_event(td_void)
{
    plc_test_ctx_stru *ctx = get_plc_test_ctx();
    plc_test_controller *plc_test_ctrl = ctx->plc_test_ctrl;
    if (plc_test_ctrl->test_mode == PLC_TEST_TCP_MODE) {
        plc_test_statistic_info();
        plc_test_main_process(PLC_TEST_ACTION_STOP);
        diag_cmd_plc_test_ind ind = { 0 };
        ind.ret_code = DIAG_CMD_PLC_TEST_IND_RET_STOP_SUCCESS;
        uapi_diag_report_packet(ID_DIAG_CMD_PLC_TEST, EXT_DIAG_CMD_INSTANCE_LOCAL, (td_pbyte)&ind,
            sizeof(diag_cmd_plc_test_ind), TD_TRUE);
    } else if (plc_test_ctrl->test_mode == PLC_TEST_UDP_MODE) {
        ctx->is_search_before_end = TD_TRUE;
        ctx->search_info_sigle = plc_test_ctrl->total_num;
        ext_sys_queue_msg msg;
        memset_s(&msg, sizeof(msg), 0, sizeof(msg));
        msg.msg_id = ID_UDP_MSG_PLC_TEST_SEARCH_EVENT;
        uapi_msg_queue_send(ctx->queue_id, &msg, 0, sizeof(ext_sys_queue_msg));
    }
}

td_void app_handle_plc_test_event(ext_sys_queue_msg *msg)
{
    plc_test_ctx_stru *ctx = get_plc_test_ctx();
    if (ctx == TD_NULL) {
        return;
    }
    plc_test_controller *plc_test_ctrl = ctx->plc_test_ctrl;
    plc_test_mac_to_timer_controller *mac_to_timer_ctrl = ctx->mac_to_timer_ctrl;
    if (plc_test_ctrl == TD_NULL || mac_to_timer_ctrl == TD_NULL) {
        return;
    }
    switch (msg->msg_id) {
        case ID_APP_MSG_PLC_TEST_TCP_EVENT:
            plc_test_tcp_event();
            break;
        case ID_APP_MSG_PLC_TEST_UDP_EVENT:
            plc_test_udp_event(msg);
            break;
        case ID_APP_MSG_PLC_TEST_SEARCH_EVENT:
            plc_test_search_event(msg);
            break;
        case ID_APP_MSG_PLC_TEST_OUTTIME_END_EVENT:
            plc_test_timeout_end_event();
            break;
        case ID_APP_MSG_PLC_TEST_END_EVENT:
            plc_test_main_process(PLC_TEST_ACTION_STOP);
            break;
        default:
            break;
    }
}

td_u32 plc_test_udp_startstop_single(td_u8 action, td_u16 index)
{
    plc_test_ctx_stru *ctx = get_plc_test_ctx();
    plc_test_controller *plc_test_ctrl = ctx->plc_test_ctrl;
    plc_test_mac_to_timer_controller *mac_to_timer_ctrl = ctx->mac_to_timer_ctrl;
    if (mac_to_timer_ctrl == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    if (index >= plc_test_ctrl->total_num || is_invalid_mac_addr(plc_test_ctrl->data[index].mac)) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    // if the retransmission fails, skip this station
    if (plc_test_ctrl->data[index].retrans_action_times > PLC_TEST_PER_RETRANS_NUM) {
        // station does not proactively and repeatedly send responses, so the minimum value of start_stop_sigle is 0.
        ctx->start_stop_sigle--;
        plc_test_ctrl->index++;
        memset_s(&plc_test_ctrl->data[index].mac, EXT_PLC_MAC_ADDR_LEN, 0, EXT_PLC_MAC_ADDR_LEN);
        if (ctx->start_stop_sigle == 0) {
            plc_test_udp_process_action(plc_test_ctrl->action);
        }

        return EXT_ERR_SUCCESS;
    }
    plc_test_udp_action_frame *frame =
        (plc_test_udp_action_frame*)uapi_malloc(EXT_MOD_ID_APP_COMMON, sizeof(plc_test_udp_action_frame));
    if (frame == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }
    memset_s(frame, sizeof(plc_test_udp_action_frame), 0, sizeof(plc_test_udp_action_frame));
    frame->action = action;
    frame->total_time = plc_test_ctrl->total_time;
    frame->index = index;

    td_u32 ret = app_plc_test_send((td_pbyte)frame, sizeof(plc_test_udp_action_frame),
                                   plc_test_ctrl->data[index].mac, ID_PLC_TEST_FRAME_UDP_ACTION);
    for (td_u32 i = 0; i < mac_to_timer_ctrl->num; i++) {
        if (is_invalid_mac_addr(mac_to_timer_ctrl->data[i].mac)) {
            (td_void)uapi_timer_start(&mac_to_timer_ctrl->data[i].plc_send_timer,
                (timer_proc_func)app_plc_test_udp_call_back, APP_PLC_TEST_UDP_STARTSTOP_TIME_MS,
                EXT_TIMER_TYPE_ONCE, index);
            (td_void)memcpy_s(mac_to_timer_ctrl->data[i].mac, EXT_PLC_MAC_ADDR_LEN,
                plc_test_ctrl->data[index].mac, EXT_PLC_MAC_ADDR_LEN);
            break;
        }
    }
    plc_test_ctrl->data[index].retrans_action_times++;
    plc_test_ctrl->current_parallel_num++;
    uapi_free(EXT_MOD_ID_APP_COMMON, frame);
    frame = TD_NULL;
    return ret;
}

__hot td_void uapi_timer_define(app_plc_test_udp_call_back)
{
    ext_sys_queue_msg msg;
    memset_s(&msg, sizeof(msg), 0, sizeof(msg));
    msg.msg_id = ID_APP_MSG_PLC_TEST_UDP_EVENT;
    msg.param[0] = ulData;
    app_send_msg_func app_send_func = get_app_send_msg_func();
    if (app_send_func != TD_NULL) {
        app_send_func(&msg);
    }
}

td_void plc_test_build_status_ind(diag_cmd_plc_test_status_ind *ind, td_u8 num)
{
    plc_test_ctx_stru *ctx = get_plc_test_ctx();
    plc_test_controller *plc_test_ctrl = ctx->plc_test_ctrl;
    ind->is_last_frame = 1;
    ind->action = PLC_TEST_STATUS_START;
    ind->num = num;
    ind->frame_length = plc_test_ctrl->frame_length;
    ind->parallel_num = plc_test_ctrl->parallel_num;
    ind->only_cco_count_time = plc_test_ctrl->only_cco_count_time;
    ind->send_interval = plc_test_ctrl->send_interval;
    ind->total_time = plc_test_ctrl->total_time;
    ind->test_mode = plc_test_ctrl->test_mode;
    ind->timeout = plc_test_ctrl->timeout;
    ind->dialog_start_time = plc_test_ctrl->dialog_start_time;
    ind->refresh_interval = plc_test_ctrl->refresh_interval;
}

td_u32 app_cmd_plc_test_status(td_u16 cmd_id, td_pvoid cmd, td_u16 cmd_size, td_u8 option)
{
    diag_cmd_plc_test_status_req *req = (diag_cmd_plc_test_status_req *)cmd;
    if ((req == TD_NULL) || (cmd_size != sizeof(diag_cmd_plc_test_status_req))) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    diag_cmd_plc_test_status_ind ind = { 0 };
    plc_test_ctx_stru *ctx = get_plc_test_ctx();
    if (ctx == TD_NULL) {
        ind.action = PLC_TEST_STATUS_STOP;
    } else {
        plc_test_controller *plc_test_ctrl = ctx->plc_test_ctrl;
        int i;
        plc_test_ctrl->opt_sta_index = 0;
        for (i = 0; i < PLC_TEST_PER_BATCH_MAX && (plc_test_ctrl->opt_sta_index + i < plc_test_ctrl->total_num); i++) {
            // length of mac in ind is 120
            memcpy_s(&ind.mac[i * EXT_PLC_MAC_ADDR_LEN], 120 - i * EXT_PLC_MAC_ADDR_LEN,
                     plc_test_ctrl->data[plc_test_ctrl->opt_sta_index + i].mac, EXT_PLC_MAC_ADDR_LEN);
        }
        if (plc_test_ctrl->opt_sta_index + i >= plc_test_ctrl->total_num) {
            plc_test_build_status_ind(&ind, i);
            plc_test_ctrl->opt_sta_index = 0;
        } else {
            plc_test_ctrl->opt_sta_index += PLC_TEST_PER_BATCH_MAX;
        }
        ind.total_num = plc_test_ctrl->total_num;
    }
    return uapi_diag_report_packet(cmd_id, option, (td_pbyte)&ind, sizeof(ind), TD_TRUE);
}

td_u32 app_cmd_plc_test_action(td_u16 cmd_id, td_pvoid cmd, td_u16 cmd_size, td_u8 option)
{
    if ((cmd == TD_NULL) || (cmd_size != sizeof(diag_cmd_plc_test_req))) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    diag_cmd_plc_test_req *req = (diag_cmd_plc_test_req *)cmd;
    diag_cmd_plc_test_ind ind = { 0 };

    ind.ret_code = init_plc_test_ctrl(req);
    if (ind.ret_code == DIAG_CMD_PLC_TEST_IND_RET_INITIALIZING) {
        return uapi_diag_report_packet(cmd_id, option, (td_pbyte)&ind, sizeof(ind), TD_TRUE);
    }
    plc_test_ctx_stru *ctx = get_plc_test_ctx();
    if (ctx == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    if (req->is_last_frame) {
        plc_test_main_process(req->action);
    }
    return uapi_diag_report_packet(cmd_id, option, (td_pbyte)&ind, sizeof(ind), TD_TRUE);
}

td_u32 stop_all_timer(td_void)
{
    plc_test_ctx_stru *ctx = get_plc_test_ctx();
    plc_test_controller *plc_test_ctrl = ctx->plc_test_ctrl;
    plc_test_mac_to_timer_controller *mac_to_timer_ctrl = ctx->mac_to_timer_ctrl;
    if (mac_to_timer_ctrl == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    plc_test_ctrl->current_parallel_num = 0;
    for (td_u32 i = 0; i < mac_to_timer_ctrl->num; i++) {
        if (!is_invalid_mac_addr(mac_to_timer_ctrl->data[i].mac)) {
            (td_void)uapi_timer_stop(&mac_to_timer_ctrl->data[i].plc_send_timer);
            (td_void)memset_s(&mac_to_timer_ctrl->data[i].mac, EXT_PLC_MAC_ADDR_LEN, 0,
                              EXT_PLC_MAC_ADDR_LEN);
        }
    }
    return EXT_ERR_SUCCESS;
}

td_void plc_test_main_process(td_u32 action)
{
    td_u32 ret = 0;
    plc_test_ctx_stru *ctx = get_plc_test_ctx();
    plc_test_controller *plc_test_ctrl = ctx->plc_test_ctrl;
    ext_timer_handle_s *plc_end_cco_timer = ctx->g_plc_end_cco_timer;
    if (plc_test_ctrl == TD_NULL || plc_end_cco_timer == TD_NULL) {
        app_plc_test_release_resources();
        return;
    }

    if (action == PLC_TEST_ACTION_START) {
        // ahead of time to end: (PLC_TEST_PER_RETRANS_NUM + 1) * APP_PLC_TEST_UDP_SEARCH_TIME_MS
        td_u32 advance_time = (PLC_TEST_PER_RETRANS_NUM + 1) * APP_PLC_TEST_UDP_SEARCH_TIME_MS;
        (td_void)uapi_timer_start(plc_end_cco_timer, (timer_proc_func)app_plc_test_end_call_back,
            // total millisecond = req->total_time * 60 * 1000
            (plc_test_ctrl->total_time * 60 * 1000) - advance_time, EXT_TIMER_TYPE_ONCE, 0);
    }

    if (plc_test_ctrl->test_mode == PLC_TEST_TCP_MODE) {
        ret = plc_test_tcp_process(action);
    } else if (plc_test_ctrl->test_mode == PLC_TEST_UDP_MODE) {
        ctx->start_stop_sigle = plc_test_ctrl->total_num;
        ret = plc_test_udp_process(action);
    }
    if (ret != EXT_ERR_SUCCESS) {
        app_plc_test_release_resources();
    }
}

td_u32 plc_test_statistic_info()
{
    plc_test_ctx_stru *ctx = get_plc_test_ctx();
    if (ctx == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    plc_test_controller *plc_test_ctrl = ctx->plc_test_ctrl;
    if (plc_test_ctrl == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    diag_cmd_plc_test_info_ind *ind =
        (diag_cmd_plc_test_info_ind *)uapi_malloc(EXT_MOD_ID_APP_COMMON, sizeof(diag_cmd_plc_test_info_ind));
    if (ind == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }
    memset_s(ind, sizeof(diag_cmd_plc_test_info_ind), 0, sizeof(diag_cmd_plc_test_info_ind));

    ind->statue = PLC_TEST_STATUS_START;
    ind->search_statue = DIAG_CMD_PLC_TEST_SEARCH_IND_RET_IDLE;
    ind->count_time = uapi_get_milli_seconds() - plc_test_ctrl->start_time;
    td_u32 ret = 0;
    int i;
    int j;
    for (i = 0; i < plc_test_ctrl->total_num; i += APP_PLC_TEST_SEND_TOTAL_NUM) {
        for (j = 0; j < APP_PLC_TEST_SEND_TOTAL_NUM && i + j < plc_test_ctrl->total_num; j++) {
            memcpy_s(ind->data[j].mac, EXT_PLC_MAC_ADDR_LEN, plc_test_ctrl->data[i + j].mac,
                     EXT_PLC_MAC_ADDR_LEN);
            ind->data[j].success_count = plc_test_ctrl->data[i + j].success_count;
            ind->data[j].success_down_time = plc_test_ctrl->data[i + j].success_down_time;
            ind->data[j].success_up_time = plc_test_ctrl->data[i + j].success_up_time;
            if (plc_test_ctrl->test_mode == PLC_TEST_TCP_MODE) {
                ind->data[j].total_count = plc_test_ctrl->frame_index;
            } else if (plc_test_ctrl->test_mode == PLC_TEST_UDP_MODE) {
                ind->data[j].total_count = plc_test_ctrl->data[i + j].total_count;
            }
        }
        if (i + j == plc_test_ctrl->total_num) {
            ind->is_last_frame = 1;
        }
        ind->num = j;
        ret |= uapi_diag_report_packet(ID_DIAG_CMD_PLC_TEST_INFO, g_cmd_option, (td_pbyte)ind, sizeof(*ind), TD_TRUE);
    }
    uapi_free(EXT_MOD_ID_APP_COMMON, ind);
    ind = TD_NULL;
    return ret;
}

td_u32 app_cmd_plc_test_info(td_u16 cmd_id, td_pvoid cmd, td_u16 cmd_size, td_u8 option)
{
    td_u32 ret = 0;
    diag_cmd_plc_test_info_req *req = (diag_cmd_plc_test_info_req *)cmd;
    if ((req == TD_NULL) || (cmd_size != sizeof(diag_cmd_plc_test_info_req))) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    diag_cmd_plc_test_info_ind *ind =
        (diag_cmd_plc_test_info_ind *)uapi_malloc(EXT_MOD_ID_APP_COMMON, sizeof(diag_cmd_plc_test_info_ind));
    if (ind == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }
    memset_s(ind, sizeof(diag_cmd_plc_test_info_ind), 0, sizeof(diag_cmd_plc_test_info_ind));

    plc_test_ctx_stru *ctx = get_plc_test_ctx();
    plc_test_controller *plc_test_ctrl = ctx->plc_test_ctrl;
    if (ctx->search_info_sigle > 0 || ctx->start_stop_sigle > 0) {
        ind->statue = PLC_TEST_STATUS_START;
        ind->search_statue = DIAG_CMD_PLC_TEST_SEARCH_IND_RET_BUSY;
        ret |= uapi_diag_report_packet(cmd_id, option, (td_pbyte)ind, sizeof(*ind), TD_TRUE);
    } else {
        if (plc_test_ctrl->test_mode == PLC_TEST_UDP_MODE) {
            ctx->search_info_sigle = plc_test_ctrl->total_num;
            ctx->is_search_before_end = TD_FALSE;
            plc_test_ctrl->opt_sta_index = 0;
            ext_sys_queue_msg msg;
            memset_s(&msg, sizeof(msg), 0, sizeof(msg));
            msg.msg_id = ID_UDP_MSG_PLC_TEST_SEARCH_EVENT;
            uapi_msg_queue_send(ctx->queue_id, &msg, 0, sizeof(ext_sys_queue_msg));
        } else if (plc_test_ctrl->test_mode == PLC_TEST_TCP_MODE) {
            plc_test_statistic_info();
        }
    }
    uapi_free(EXT_MOD_ID_APP_COMMON, ind);
    ind = TD_NULL;
    return ret;
}

td_u32 app_cmd_plc_test(td_u16 cmd_id, td_pvoid cmd, td_u16 cmd_size, td_u8 option)
{
    td_u32 ret = 0;
    if (cmd == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    g_cmd_option = option;
    if (cmd_id == ID_DIAG_CMD_PLC_TEST) {
        ret = app_cmd_plc_test_action(cmd_id, cmd, cmd_size, option);
    } else if (cmd_id == ID_DIAG_CMD_PLC_TEST_INFO) {
        ret = app_cmd_plc_test_info(cmd_id, cmd, cmd_size, option);
    } else if (cmd_id == ID_DIAG_CMD_PLC_TEST_STATUS) {
        ret = app_cmd_plc_test_status(cmd_id, cmd, cmd_size, option);
    }
    return ret;
}

td_u32 plc_test_udp_info_single(td_u16 index)
{
    plc_test_ctx_stru *ctx = get_plc_test_ctx();
    plc_test_controller *plc_test_ctrl = ctx->plc_test_ctrl;
    plc_test_mac_to_timer_controller *mac_to_timer_ctrl = ctx->mac_to_timer_ctrl;
    if (index >= plc_test_ctrl->total_num || is_invalid_mac_addr(plc_test_ctrl->data[index].mac)) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    // if the retransmission fails, skip this station
    if (plc_test_ctrl->data[index].retrans_search_times > PLC_TEST_PER_RETRANS_NUM) {
        // station does not proactively and repeatedly send responses, so the minimum value of search_info_sigle is 0.
        ctx->search_info_sigle--;
        plc_test_ctrl->index++;
        return EXT_ERR_SUCCESS;
    }
    plc_test_udp_info_frame *frame =
        (plc_test_udp_info_frame*)uapi_malloc(EXT_MOD_ID_APP_COMMON, sizeof(plc_test_udp_info_frame));
    if (frame == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }
    memset_s(frame, sizeof(plc_test_udp_info_frame), 0, sizeof(plc_test_udp_info_frame));
    frame->index = index;

    td_u32 ret = app_plc_test_send((td_pbyte)frame, sizeof(plc_test_udp_info_frame),
                                   plc_test_ctrl->data[index].mac, ID_PLC_TEST_FRAME_UDP_INFO);
    for (td_u32 i = 0; i < mac_to_timer_ctrl->num; i++) {
        if (is_invalid_mac_addr(mac_to_timer_ctrl->data[i].mac)) {
            (td_void)uapi_timer_start(&mac_to_timer_ctrl->data[i].plc_send_timer,
                (timer_proc_func)app_plc_test_udp_search_call_back,
                APP_PLC_TEST_UDP_SEARCH_TIME_MS, EXT_TIMER_TYPE_ONCE, i);
            (td_void)memcpy_s(mac_to_timer_ctrl->data[i].mac, EXT_PLC_MAC_ADDR_LEN,
                              plc_test_ctrl->data[index].mac, EXT_PLC_MAC_ADDR_LEN);
            break;
        }
    }
    plc_test_ctrl->data[index].retrans_search_times++;
    plc_test_ctrl->current_parallel_num++;
    uapi_free(EXT_MOD_ID_APP_COMMON, frame);
    frame = TD_NULL;
    return ret;
}

td_u32 plc_test_udp_info(td_void)
{
    td_u32 ret = 0;
    plc_test_ctx_stru *ctx = get_plc_test_ctx();
    plc_test_controller *plc_test_ctrl = ctx->plc_test_ctrl;
    if (ctx->search_info_sigle == 0) {
        plc_test_statistic_info();
        plc_test_ctrl->opt_sta_index = 0;
        for (td_u32 i = 0; i < plc_test_ctrl->total_num; i++) {
            plc_test_ctrl->data[i].retrans_search_times = 0;
        }
        if (ctx->is_search_before_end) {
            ext_sys_queue_msg msg;
            memset_s(&msg, sizeof(msg), 0, sizeof(msg));
            msg.msg_id = ID_APP_MSG_PLC_TEST_END_EVENT;
            app_send_msg_func app_send_func = get_app_send_msg_func();
            if (app_send_func != TD_NULL) {
                app_send_func(&msg);
            }
        }
        return EXT_ERR_SUCCESS;
    }
    while (plc_test_ctrl->current_parallel_num < plc_test_ctrl->parallel_num &&
        plc_test_ctrl->opt_sta_index < plc_test_ctrl->total_num) {
        ret |= plc_test_udp_info_single(plc_test_ctrl->opt_sta_index);
        plc_test_ctrl->opt_sta_index++;
        uapi_sleep(APP_PLC_TEST_SEARCH_INVERVAL_MS);
    }
    return ret;
}

__hot td_void uapi_timer_define(app_plc_test_udp_search_call_back)
{
    plc_test_ctx_stru *ctx = get_plc_test_ctx();
    if (ctx == TD_NULL) {
        return;
    }
    plc_test_mac_to_timer_controller *mac_to_timer_ctrl = ctx->mac_to_timer_ctrl;

    if (mac_to_timer_ctrl != TD_NULL) {
        (td_void)memset_s(&mac_to_timer_ctrl->data[ulData].mac, EXT_PLC_MAC_ADDR_LEN, 0, EXT_PLC_MAC_ADDR_LEN);
    }
    ext_sys_queue_msg msg;
    memset_s(&msg, sizeof(msg), 0, sizeof(msg));
    msg.msg_id = ID_APP_MSG_PLC_TEST_SEARCH_EVENT;
    msg.param[0] = ulData;
    app_send_msg_func app_send_func = get_app_send_msg_func();
    if (app_send_func != TD_NULL) {
        app_send_func(&msg);
    }
}

td_u32 plc_test_udp_process_action(td_u32 action)
{
    td_u32 ret = 0;
    plc_test_ctx_stru *ctx = get_plc_test_ctx();
    plc_test_controller *plc_test_ctrl = ctx->plc_test_ctrl;
    if (plc_test_ctrl->total_num == 0 || ctx->start_stop_sigle > 0) {
        return EXT_ERR_FAILURE;
    }
    plc_test_ctrl->index = 0;
    if (action == PLC_TEST_ACTION_STOP) {
        diag_cmd_plc_test_ind ind = { 0 };
        ind.ret_code = DIAG_CMD_PLC_TEST_IND_RET_STOP_SUCCESS;
        uapi_diag_report_packet(ID_DIAG_CMD_PLC_TEST, EXT_DIAG_CMD_INSTANCE_LOCAL, (td_pbyte)&ind,
            sizeof(diag_cmd_plc_test_ind), TD_TRUE);
        app_plc_test_release_resources();
    } else if (action == PLC_TEST_ACTION_START) {
        for (td_u32 i = 0; i < plc_test_ctrl->total_num; i++) {
            plc_test_ctrl->data[i].retrans_action_times = 0;
        }
        plc_test_ctrl->statue = PLC_TEST_STATUS_START;
        ret |= uapi_msg_queue_create(&ctx->queue_id, "udpq", APP_PLC_TEST_QUEUE_SIZE, sizeof(ext_sys_queue_msg));
        ret |= uapi_task_create(&(ctx->task_id), "PLC_TEST_UDP", plc_test_udp_task_body, ctx->queue_id,
            APP_SRV_TASK_STACK_SIZE, APP_SRV_TASK_PRIORITY);
    }
    return ret;
}

td_void plc_test_udp_task_body(td_u32 param)
{
    td_u32 ret;
    ext_sys_queue_msg msg = { 0 };
    td_u32 queue_id = param;
    ext_sys_queue_msg msg_tmp;
    memset_s(&msg_tmp, sizeof(msg_tmp), 0, sizeof(msg_tmp));
    msg_tmp.msg_id = ID_UDP_MSG_PLC_TEST_SEND_EVENT;
    uapi_msg_queue_send(queue_id, &msg_tmp, 0, sizeof(ext_sys_queue_msg));
    for (;;) {
        ret = uapi_msg_queue_wait(queue_id, &msg, EXT_SYS_WAIT_FOREVER, sizeof(msg));
        if (ret == EXT_ERR_SUCCESS) {
            app_handle_plc_test_udp_event(&msg);
        } else {
            uapi_sleep(APP_PLC_TEST_SLEEP_DURTION_MS);
        }
    }
}

td_void app_handle_plc_test_udp_event(ext_sys_queue_msg *msg)
{
    plc_test_ctx_stru *ctx = get_plc_test_ctx();
    if (ctx == TD_NULL) {
        return;
    }
    switch (msg->msg_id) {
        case ID_UDP_MSG_PLC_TEST_SEND_EVENT:
            plc_test_udp_send_frame();
            if (!ctx->is_search_before_end) {
                uapi_msg_queue_send(ctx->queue_id, msg, 0, sizeof(ext_sys_queue_msg));
            }
            break;
        case ID_UDP_MSG_PLC_TEST_SEARCH_EVENT:
            plc_test_udp_info();
            break;
        default:
            break;
    }
}

td_void plc_test_udp_send_frame(td_void)
{
    td_u32 ret;
    plc_test_ctx_stru *ctx = get_plc_test_ctx();
    plc_test_controller *plc_test_ctrl = ctx->plc_test_ctrl;
    td_pbyte plc_test_frame_buff = ctx->plc_test_frame;
    if (plc_test_frame_buff == TD_NULL) {
        return;
    }
    plc_test_frame *frame = (plc_test_frame *)plc_test_frame_buff;
    if (frame == TD_NULL) {
        return;
    }
    td_bool is_sync;
    frame->total_count = plc_test_ctrl->frame_rounds;
    frame->data_length = plc_test_ctrl->frame_length - sizeof(plc_test_frame);
    for (int i = 0; i < plc_test_ctrl->total_num; i++) {
        frame->down_time = uapi_get_ntb(&is_sync);
        ret = app_plc_test_send((td_pbyte)frame, plc_test_ctrl->frame_length,
                                plc_test_ctrl->data[i].mac, ID_PLC_TEST_FRAME_UDP);
        if (ret != EXT_ERR_SUCCESS) {
            return;
        }
        uapi_sleep(plc_test_ctrl->send_interval);
    }
    plc_test_ctrl->frame_rounds++;
}

#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

#define PLC_TEST_UDP_START_SUCCESS     0
#define PLC_TEST_UDP_STOP_SUCCESS      1

td_u32 total_count = 0;
td_u32 success_count = 0;
td_u32 success_down_time = 0;

ext_timer_handle_s *g_plc_end_sta_timer = TD_NULL;
td_u32 app_plc_test_sta_create_resources(td_void);
td_void app_plc_test_sta_release_resources(td_void);

ext_timer_handle_s *get_plc_end_sta_timer(td_void)
{
    return g_plc_end_sta_timer;
}

td_void app_plc_test_sta_release_resources(td_void)
{
    if (g_plc_end_sta_timer != TD_NULL) {
        (td_void)uapi_timer_delete(g_plc_end_sta_timer);
        uapi_free(EXT_MOD_ID_APP_COMMON, g_plc_end_sta_timer);
        g_plc_end_sta_timer = TD_NULL;
    }
}

td_u32 app_plc_test_sta_create_resources(td_void)
{
    app_plc_test_sta_release_resources();
    g_plc_end_sta_timer = (ext_timer_handle_s *)uapi_malloc(EXT_MOD_ID_APP_COMMON, sizeof(ext_timer_handle_s));
    if (g_plc_end_sta_timer == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }
    memset_s(g_plc_end_sta_timer, sizeof(ext_timer_handle_s), 0, sizeof(ext_timer_handle_s));
    return EXT_ERR_SUCCESS;
}

td_u32 init_plc_test_sta_ctrl(plc_test_udp_action_frame *req)
{
    app_plc_test_sta_release_resources();
    td_u32 ret = 0;
    if (req->action == PLC_TEST_ACTION_START) {
        total_count = 0;
        success_count = 0;
        success_down_time = 0;
        ret = app_plc_test_sta_create_resources();
        if (ret == EXT_ERR_SUCCESS) {
            return PLC_TEST_UDP_START_SUCCESS;
        }
    }
    return ret;
}

td_void app_handle_plc_test_sta_event(ext_sys_queue_msg *msg)
{
    switch (msg->msg_id) {
        case ID_APP_MSG_PLC_TEST_OUTTIME_END_EVENT:
            app_plc_test_sta_release_resources();
            break;
        default:
            break;
    }
}
#endif

td_u32 app_plc_test_sta_process(td_pbyte data, td_u16 data_length)
{
    if (data_length < sizeof(plc_test_frame)) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    plc_test_frame *buffer = (plc_test_frame *)data;
    ext_unref_param(buffer);
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    td_bool is_sync = TD_FALSE;
    ext_unref_param(is_sync);
    total_count = buffer->total_count;
    success_count++;
    success_down_time += (uapi_get_ntb(&is_sync) - buffer->down_time) / APP_PLC_TEST_NTB_CONVERT_TO_MS;
#endif
    return EXT_ERR_SUCCESS;
}

td_u32 app_proto_plc_test(td_pbyte data, td_u16 data_length, td_pbyte src_addr)
{
    ext_unref_param(data_length);
    ext_unref_param(src_addr);
    plc_test_frame *buffer = (plc_test_frame *)data;
    ext_unref_param(buffer);
    td_bool is_sync = TD_FALSE;
    ext_unref_param(is_sync);
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    plc_test_ctx_stru *ctx = get_plc_test_ctx();
    plc_test_controller *plc_test_ctrl = ctx->plc_test_ctrl;

    if (plc_test_ctrl == TD_NULL || buffer->index >= plc_test_ctrl->total_num ||
        buffer->frame_index != plc_test_ctrl->frame_index) {
        return EXT_ERR_INVALID_PARAMETER;
    } else {
        plc_test_stop_timer(buffer->index);
        plc_test_ctrl->current_parallel_num--;
        plc_test_ctrl->data[buffer->index].success_count++;
        if ((plc_test_ctrl->only_cco_count_time & 0x1) == TD_TRUE) {
            plc_test_ctrl->data[buffer->index].success_down_time +=
                (uapi_get_ntb(&is_sync) - buffer->cco_count_time) / APP_PLC_TEST_NTB_CONVERT_TO_MS;
        } else {
            plc_test_ctrl->data[buffer->index].success_down_time += buffer->down_time / APP_PLC_TEST_NTB_CONVERT_TO_MS;
            plc_test_ctrl->data[buffer->index].success_up_time +=
                (uapi_get_ntb(&is_sync) - buffer->up_time) / APP_PLC_TEST_NTB_CONVERT_TO_MS;
        }
        return plc_test_tcp_process(PLC_TEST_ACTION_START);
    }
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    buffer->down_time = uapi_get_ntb(&is_sync) - buffer->down_time;
    buffer->up_time = uapi_get_ntb(&is_sync);
    return app_plc_test_send((td_pbyte)buffer, data_length, src_addr, ID_PLC_TEST_FRAME);
#endif
    return EXT_ERR_SUCCESS;
}

td_u32 app_proto_plc_test_udp_action(td_pbyte data, td_u16 data_length, td_pbyte src_addr)
{
    td_u32 ret = 0;
    ext_unref_param(src_addr);
    if (data_length < sizeof(plc_test_udp_action_frame)) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    plc_test_udp_action_frame *buffer = (plc_test_udp_action_frame *)data;
    ext_unref_param(buffer);
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    plc_test_ctx_stru *ctx = get_plc_test_ctx();
    plc_test_controller *plc_test_ctrl = ctx->plc_test_ctrl;
    // station does not proactively and repeatedly send responses, so the minimum value of start_stop_sigle is 0.
    ctx->start_stop_sigle--;
    plc_test_ctrl->current_parallel_num--;
    plc_test_stop_timer(buffer->index);

    if (ctx->start_stop_sigle > plc_test_ctrl->total_num) {
        ctx->start_stop_sigle = 0;
        return EXT_ERR_INVALID_PARAMETER;
    }

    ret |= plc_test_udp_process(plc_test_ctrl->action);
    if (ctx->start_stop_sigle == 0) {
        ret |= plc_test_udp_process_action(plc_test_ctrl->action);
    }
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    buffer->ret_code = init_plc_test_sta_ctrl(buffer);
    if (buffer->action == PLC_TEST_ACTION_START) {
        ext_timer_handle_s *plc_end_sta_timer = get_plc_end_sta_timer();
        if (plc_end_sta_timer == TD_NULL) {
            return EXT_ERR_INVALID_PARAMETER;
        }
        (td_void)uapi_timer_start(plc_end_sta_timer, (timer_proc_func)app_plc_test_end_call_back,
            // total millisecond = req->total_time * 60 * 1000
            buffer->total_time * 60 * 1000, EXT_TIMER_TYPE_ONCE, 0);
    } else if (buffer->action == PLC_TEST_ACTION_STOP) {
        app_plc_test_sta_release_resources();
    }
    ret |= app_plc_test_send((td_pbyte)buffer, sizeof(plc_test_udp_action_frame), src_addr,
                             ID_PLC_TEST_FRAME_UDP_ACTION);
#endif
    return ret;
}

td_u32 app_proto_plc_test_udp_info(td_pbyte data, td_u16 data_length, td_pbyte src_addr)
{
    ext_unref_param(src_addr);
    if (data_length < sizeof(plc_test_udp_info_frame)) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    plc_test_udp_info_frame *buffer = (plc_test_udp_info_frame *)data;
    ext_unref_param(buffer);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    buffer->count = success_count;
    buffer->down_time = success_down_time;
    buffer->total_count = total_count;
    return app_plc_test_send((td_pbyte)buffer, sizeof(plc_test_udp_info_frame), src_addr, ID_PLC_TEST_FRAME_UDP_INFO);
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    plc_test_ctx_stru *ctx = get_plc_test_ctx();
    if (ctx == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    plc_test_controller *plc_test_ctrl = ctx->plc_test_ctrl;
    if (plc_test_ctrl == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    // station does not proactively and repeatedly send responses, so the minimum value of search_info_sigle is 0.
    ctx->search_info_sigle--;
    plc_test_ctrl->current_parallel_num--;

    plc_test_ctrl->data[buffer->index].success_down_time = buffer->down_time;
    plc_test_ctrl->data[buffer->index].success_count = buffer->count;
    plc_test_ctrl->data[buffer->index].total_count = buffer->total_count;
    plc_test_stop_timer(buffer->index);
    if (ctx->is_search_before_end) {
        plc_test_udp_info();
    } else {
        ext_sys_queue_msg msg;
        memset_s(&msg, sizeof(msg), 0, sizeof(msg));
        msg.msg_id = ID_UDP_MSG_PLC_TEST_SEARCH_EVENT;
        uapi_msg_queue_send(ctx->queue_id, &msg, 0, sizeof(ext_sys_queue_msg));
    }
    return EXT_ERR_SUCCESS;
#endif
    return EXT_ERR_SUCCESS;
}

td_u32 app_send_plc_test_frame(app_plc_frame_data *frame)
{
    ext_dms_chl_tx_data *tx_data = TD_NULL;
    ext_dms_chl_tx_ctrl *tx_ctrl = TD_NULL;
    td_u32 frame_length;

    if (frame == TD_NULL) {
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
        tx_ctrl = TD_NULL;
        return EXT_ERR_MALLOC_FAILUE;
    }

    (td_void)memset_s(tx_ctrl, sizeof(ext_dms_chl_tx_addr_s), 0, sizeof(ext_dms_chl_tx_addr_s));
    (td_void)memset_s(tx_data, frame_length, 0, frame_length);
    if (frame->payload_length > 0 && frame->payload != TD_NULL) {
        if (memcpy_s(tx_data->data, frame->payload_length, frame->payload, frame->payload_length) != EOK) {
            uapi_free(EXT_MOD_ID_APP_COMMON, tx_data);
            tx_data = TD_NULL;

            uapi_free(EXT_MOD_ID_APP_COMMON, tx_ctrl);
            tx_ctrl = TD_NULL;
            return EXT_ERR_MEMCPY_FAIL;
        }
    }

    tx_data->data_size = frame->payload_length;

    (td_void)memcpy_s(tx_ctrl->dest_mac, EXT_PLC_MAC_ADDR_LEN, frame->mac, sizeof(frame->mac));
    tx_ctrl->ctrl = EXT_DMS_CHL_TX_CTRL_FORCE;
    tx_ctrl->type = EXT_DMS_CHL_TX_TYPE_MR;
    tx_ctrl->prio = APP_PLC_TEST_SEND_PRIO;
    tx_ctrl->id = frame->id;

    uapi_dms_chl_tx(EXT_DMS_CHL_MAC_PORT_APP, tx_data, tx_ctrl);
    uapi_free(EXT_MOD_ID_APP_COMMON, tx_data);
    tx_data = TD_NULL;

    uapi_free(EXT_MOD_ID_APP_COMMON, tx_ctrl);
    tx_ctrl = TD_NULL;

    return EXT_ERR_SUCCESS;
}

td_u32 app_plc_test_send(td_pbyte data, td_u32 data_length, td_u8 des_addr[EXT_PLC_MAC_ADDR_LEN], td_u16 frame_id)
{
    app_plc_trans_frame *trans = TD_NULL;
    td_u32 ret;
    td_u16 buffer_length = sizeof(app_plc_trans_frame) + data_length;
    td_pbyte plc_buffer;

    if (des_addr == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    plc_buffer = (td_pbyte)uapi_malloc(EXT_MOD_ID_APP_COMMON, buffer_length);
    if (plc_buffer == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }
    (td_void)memset_s(plc_buffer, buffer_length, 0, buffer_length);

    trans = (app_plc_trans_frame *)plc_buffer;
    trans->data_length = data_length;

    if (data != TD_NULL && data_length > 0 && memcpy_s(trans->data, trans->data_length, data, data_length) != 0) {
        return EXT_ERR_MEMCPY_FAIL;
    }

    trans->crc_16 = uapi_crc16(0, trans->data, trans->data_length);
    trans->crc_16 = ext_swap16(trans->crc_16);

    app_plc_frame_data plc;
    plc.id = frame_id;
    if (memcpy_s(plc.mac, sizeof(plc.mac), des_addr, EXT_PLC_MAC_ADDR_LEN) != 0) {
        return EXT_ERR_MEMCPY_FAIL;
    }
    plc.payload_length = buffer_length;
    plc.payload = (td_pvoid)plc_buffer;

    ret = app_send_plc_test_frame(&plc);
    uapi_free(EXT_MOD_ID_APP_COMMON, plc_buffer);
    plc_buffer = TD_NULL;
    return ret;
}

td_u32 app_plc_test_chl_rx(td_u16 port, ext_dms_chl_rx *chl_rx)
{
    app_plc_trans_frame *app_frame = TD_NULL;
    td_u32 ret = 0;
    td_u16 crc_16 = 0;

    ext_unref_param(port);
    if (chl_rx == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    app_frame = (app_plc_trans_frame *)chl_rx->payload;
    if (app_frame == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    if (sizeof(app_plc_trans_frame) + app_frame->data_length != chl_rx->payload_len) {
            return EXT_ERR_INVALID_PARAMETER;
    }
    crc_16 = uapi_crc16(crc_16, chl_rx->payload + sizeof(app_plc_trans_frame), app_frame->data_length);
    crc_16 = ext_swap16(crc_16);
    if (crc_16 != app_frame->crc_16) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    if (chl_rx->id == ID_PLC_TEST_FRAME) {
        ret = app_proto_plc_test(app_frame->data, app_frame->data_length, chl_rx->src_mac);
    } else if (chl_rx->id == ID_PLC_TEST_FRAME_UDP_ACTION) {
        ret = app_proto_plc_test_udp_action(app_frame->data, app_frame->data_length, chl_rx->src_mac);
    } else if (chl_rx->id == ID_PLC_TEST_FRAME_UDP) {
        ret = app_plc_test_sta_process(app_frame->data, app_frame->data_length);
    } else if (chl_rx->id == ID_PLC_TEST_FRAME_UDP_INFO) {
        ret = app_proto_plc_test_udp_info(app_frame->data, app_frame->data_length, chl_rx->src_mac);
    }
    return ret;
}

__hot td_void uapi_timer_define(app_plc_test_end_call_back)
{
    ext_sys_queue_msg msg;
    memset_s(&msg, sizeof(msg), 0, sizeof(msg));
    msg.msg_id = ID_APP_MSG_PLC_TEST_OUTTIME_END_EVENT;
    app_send_msg_func app_send_func = get_app_send_msg_func();
    if (app_send_func != TD_NULL) {
        app_send_func(&msg);
    }
}
#endif
