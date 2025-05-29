/*
 * Copyright (c) CompanyNameMagicTag 2019-2020. All rights reserved.
 * Description: Fast send function.
 */

#include "app_fast_send_demo.h"
#include "app_cmd_msg.h"
#include "app_common.h"
#include "soc_mdm_mem.h"

#if defined(PRODUCT_CFG_FAST_SEND_DEMO)

app_fast_send_demo_ctrl_st g_fast_send_demo_ctrl = { 0 };

app_fast_send_demo_ctrl_st *app_fast_send_ctrl(td_void)
{
    return &g_fast_send_demo_ctrl;
}

td_u32 app_fast_send_demo_cmd(diag_cmd_fast_send_demo_req *req_cmd)
{
    if (req_cmd->set_enable == TD_TRUE) {
        return app_fast_send_demo_start(req_cmd, TD_FALSE);
    }

    return app_fast_send_demo_stop();
}

td_u32 app_fast_send_demo_start(diag_cmd_fast_send_demo_req *req_cmd, td_bool is_retranf)
{
    td_u32 ret;
    td_bool is_start_timer = TD_FALSE;
    app_fast_send_demo_ctrl_st *demo_ctrl = app_fast_send_ctrl();

    demo_ctrl->send_mode = req_cmd->set_send_mode;
    demo_ctrl->send_broadcast_flag = req_cmd->send_broadcast_flag;
    demo_ctrl->send_resend_cnt = req_cmd->resend_cnt;
    demo_ctrl->send_period = req_cmd->set_send_period;
    demo_ctrl->period_send_cnt = req_cmd->set_send_cnt;
    demo_ctrl->send_plc_mode = req_cmd->set_plc_mode;
    (td_void) memcpy_s(demo_ctrl->oda, EXT_PLC_MAC_ADDR_LEN, req_cmd->oda, EXT_PLC_MAC_ADDR_LEN);

    if (demo_ctrl->period_send_cnt > 0) {
        if (demo_ctrl->send_period < APP_FAST_SEND_PERIOD_MS) {
            return EXT_ERR_INVALID_PARAMETER;
        }
        is_start_timer = TD_TRUE;
    } else {
        uapi_timer_stop(&demo_ctrl->fast_send_timer);
    }

    if (demo_ctrl->send_plc_mode == APP_FAST_SEND_FC_MODE) {
        ret = app_fast_send_fc_process(is_retranf);
    } else {
        ret = app_fast_send_pb_process(is_retranf);
    }

    if ((ret == EXT_ERR_SUCCESS) && (is_start_timer == TD_TRUE)) {
        (td_void) uapi_timer_start(&demo_ctrl->fast_send_timer, (timer_proc_func)app_fast_send_call_back,
                                   demo_ctrl->send_period, EXT_TIMER_TYPE_PERIOD, 0);
    }

    return ret;
}

td_u32 app_fast_send_demo_stop(td_void)
{
    app_fast_send_demo_ctrl_st *demo_ctrl = app_fast_send_ctrl();

    uapi_timer_stop(&demo_ctrl->fast_send_timer);
    demo_ctrl->send_broadcast_flag = 0;
    demo_ctrl->send_mode = 0;
    demo_ctrl->send_period = 0;
    demo_ctrl->send_resend_cnt = 0;

    return EXT_ERR_SUCCESS;
}

__hot td_void uapi_timer_define(app_fast_send_call_back)
{
    ext_sys_queue_msg msg;

    memset_s(&msg, sizeof(msg), 0, sizeof(msg));
    msg.msg_id = ID_APP_MSG_FAST_SEND_EVENT;
    app_send_message(&msg);
}

td_void app_handle_fast_send_event(EXT_CONST ext_sys_queue_msg *msg)
{
    app_fast_send_demo_ctrl_st *demo_ctrl = app_fast_send_ctrl();
    ext_unref_param(msg);

    demo_ctrl->period_send_index++;
    if (demo_ctrl->period_send_index >= demo_ctrl->period_send_cnt) {
        app_fast_send_demo_stop();
        return;
    }

    if (demo_ctrl->send_plc_mode == 0) {
        app_fast_send_fc_process(TD_FALSE);
    } else {
        app_fast_send_pb_process(TD_FALSE);
    }
}

td_u32 app_set_fast_send_pb_para(ext_mdm_bb_plc_send_para *send_para, app_fast_send_demo_ctrl_st *demo_ctrl,
                                 td_bool is_retranf, td_u8 oda[EXT_PLC_MAC_ADDR_LEN], td_u16 data_size)
{
    send_para->send_phase = demo_ctrl->send_phase;
    send_para->broadcast_flag = demo_ctrl->send_broadcast_flag;
    send_para->send_mode = demo_ctrl->send_mode;
    send_para->resend_cnt = demo_ctrl->send_resend_cnt;
    if (memcpy_s(send_para->oda, EXT_PLC_MAC_ADDR_LEN, demo_ctrl->oda, EXT_PLC_MAC_ADDR_LEN) != EXT_ERR_SUCCESS) {
        return EXT_ERR_FAILURE;
    }

    if (memcmp(send_para->oda, oda, EXT_PLC_MAC_ADDR_LEN) == 0 && (send_para->broadcast_flag == 0)) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    send_para->data_length = data_size;

    if (is_retranf == TD_TRUE) {
        demo_ctrl->send_index = demo_ctrl->rcv_send_index;
    } else {
        demo_ctrl->send_index++;
    }

    if (demo_ctrl->send_index == 0) {
        demo_ctrl->send_index++;
    }

    return EXT_ERR_SUCCESS;
}

td_void app_set_fast_send_pb_data(td_u8 data[APP_FAST_SEND_PB_DATA_LEN])
{
    data[0] = APP_FAST_DEMO_DATA0;
    data[1] = APP_FAST_DEMO_DATA1;
    data[APP_FAST_DEMO_INDEX2] = APP_FAST_DEMO_DATA2;
    data[APP_FAST_DEMO_INDEX3] = APP_FAST_DEMO_DATA3;
    data[APP_FAST_DEMO_INDEX4] = APP_FAST_DEMO_DATA4;
    data[APP_FAST_DEMO_INDEX5] = APP_FAST_DEMO_DATA5;
    data[APP_FAST_DEMO_INDEX6] = APP_FAST_DEMO_DATA6;
    data[APP_FAST_DEMO_INDEX7] = APP_FAST_DEMO_DATA7;
    data[APP_FAST_DEMO_INDEX8] = APP_FAST_DEMO_DATA8;
    data[APP_FAST_DEMO_INDEX9] = APP_FAST_DEMO_DATA9;
}

td_u32 app_fast_send_pb_process(td_bool is_retranf)
{
    app_fast_send_demo_ctrl_st *demo_ctrl = app_fast_send_ctrl();
    ext_mdm_bb_plc_send_para *send_para = TD_NULL;
    app_fast_send_pb_demo_frame_st *frame = TD_NULL;
    td_u16 malloc_size;
    td_u16 payload_size = APP_FAST_SEND_PB_DATA_LEN;
    td_u16 data_size;
    td_u8 offset;
    td_u8 data_demo[APP_FAST_SEND_PB_DATA_LEN] = { 0 };
    td_u8 oda[EXT_PLC_MAC_ADDR_LEN] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    td_u32 ret;

    data_size = APP_FAST_SEND_PB_DEMO_FRAME_SIZE + payload_size;
    malloc_size = sizeof(ext_mdm_bb_plc_send_para) + data_size;
    send_para = uapi_malloc(EXT_MOD_ID_APP_COMMON, malloc_size);
    if (send_para == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }

    if (memset_s(send_para, malloc_size, 0, malloc_size) != EXT_ERR_SUCCESS) {
        uapi_free(EXT_MOD_ID_APP_COMMON, send_para);
        return EXT_ERR_FAILURE;
    }

    if (app_set_fast_send_pb_para(send_para, demo_ctrl, is_retranf, oda, data_size) != EXT_ERR_SUCCESS) {
        uapi_free(EXT_MOD_ID_APP_COMMON, send_para);
        return EXT_ERR_FAILURE;
    }

    frame = (app_fast_send_pb_demo_frame_st *)send_para->data;
    frame->id = APP_FAST_SEND_PB_PORT_ID;
    frame->data_index = demo_ctrl->send_index;
    frame->payload_len = payload_size;
    offset = sizeof(ext_mdm_bb_plc_send_para) + APP_FAST_SEND_PB_DEMO_FRAME_SIZE;

    app_set_fast_send_pb_data(data_demo);

    if (memcpy_s(((td_u8 *)send_para) + offset, malloc_size - offset, data_demo, payload_size) != EXT_ERR_SUCCESS) {
        uapi_free(EXT_MOD_ID_APP_COMMON, send_para);
        return EXT_ERR_FAILURE;
    }

    if (send_para->send_mode == EXT_MAC_FAST_SEND_PREEMPT) {
        uapi_set_disable_sending(APP_FAST_SEND_DISABLE_SEND_DURTION_MS);
    }

    ret = uapi_fast_send_pb_on_plc(send_para);
    uapi_free(EXT_MOD_ID_APP_COMMON, send_para);

    return ret;
}

td_u32 app_rcv_fast_send_plc_chl_demo(td_u16 port, EXT_CONST ext_dms_chl_rx *chl_rx)
{
    app_fast_send_demo_ctrl_st *demo_ctrl = app_fast_send_ctrl();
    diag_cmd_fast_send_demo_req req_cmd = { 0 };
    app_fast_send_pb_demo_frame_st *frame = (app_fast_send_pb_demo_frame_st *)chl_rx->payload;

    ext_unref_param(port);

    if (chl_rx->id != 0) {
        return EXT_ERR_NOT_SUPPORT_ID;
    }

    if (frame->id != APP_FAST_SEND_PB_PORT_ID) {
        return EXT_ERR_NOT_SUPPORT_ID;
    }

    if (chl_rx->payload_len != APP_FAST_SEND_PB_DEMO_FRAME_SIZE + frame->payload_len) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    if (frame->data_index == demo_ctrl->rcv_send_index) {
        return EXT_ERR_FAILURE;
    }

    demo_ctrl->rcv_send_index = frame->data_index;

    if (uapi_get_my_role() == IS_PROXY) {
        req_cmd.set_enable = TD_TRUE;
        req_cmd.set_plc_mode = APP_FAST_SEND_PB_MODE;
        req_cmd.resend_cnt = APP_FAST_SEND_RESEND_CNT;
        req_cmd.send_broadcast_flag = 1;
        req_cmd.set_send_cnt = 0;
        req_cmd.set_send_mode = EXT_MAC_FAST_SEND_PRIORITY;
        req_cmd.set_send_period = 0;
        req_cmd.set_send_phase = 0;
        return app_fast_send_demo_start(&req_cmd, TD_TRUE);
    }
    return EXT_ERR_SUCCESS;
}

td_u32 app_fast_send_plc_chl_demo_init(td_void)
{
    app_fast_send_demo_ctrl_st *demo_ctrl = app_fast_send_ctrl();

    demo_ctrl->rx_func = app_rcv_fast_send_plc_chl_demo;

    return EXT_ERR_SUCCESS;
}

td_u32 app_rcv_fast_send_plc_chl_demo_interface(td_u16 port, EXT_CONST ext_dms_chl_rx *chl_rx)
{
    app_fast_send_demo_ctrl_st *demo_ctrl = app_fast_send_ctrl();

    if (demo_ctrl->rx_func != TD_NULL) {
        return demo_ctrl->rx_func(port, chl_rx);
    }

    return EXT_ERR_FAILURE;
}

td_void app_set_fast_send_fc_data(td_u8 data[APP_FAST_SEND_FC_DATA_LEN])
{
    data[0] = APP_FAST_DEMO_DATA0;
    data[1] = APP_FAST_DEMO_DATA1;
    data[APP_FAST_DEMO_INDEX2] = APP_FAST_DEMO_DATA2;
    data[APP_FAST_DEMO_INDEX3] = APP_FAST_DEMO_DATA3;
    data[APP_FAST_DEMO_INDEX4] = APP_FAST_DEMO_DATA4;
    data[APP_FAST_DEMO_INDEX5] = APP_FAST_DEMO_DATA5;
}

td_u32 app_fast_send_fc_process(td_bool is_retranf)
{
    app_fast_send_demo_ctrl_st *demo_ctrl = app_fast_send_ctrl();
    ext_mdm_bb_plc_send_para *send_para = TD_NULL;
    app_fast_send_fc_demo_frame_st *fc_frame = TD_NULL;
    td_u16 malloc_size;
    td_u16 data_size;
    td_u8 payload_size = APP_FAST_SEND_FC_DATA_LEN;
    td_u8 offset;
    td_u8 data_demo[APP_FAST_SEND_FC_DATA_LEN] = { 0 };
    td_u32 ret;

    data_size = APP_FAST_SEND_FC_DEMO_FRAME_SIZE + payload_size;
    malloc_size = sizeof(ext_mdm_bb_plc_send_para) + data_size;
    send_para = uapi_malloc(EXT_MOD_ID_APP_COMMON, malloc_size);
    if (send_para == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }

    if (memset_s(send_para, malloc_size, 0, malloc_size) != EXT_ERR_SUCCESS) {
        uapi_free(EXT_MOD_ID_APP_COMMON, send_para);
        return EXT_ERR_FAILURE;
    }
    send_para->send_phase = 0;
    send_para->broadcast_flag = demo_ctrl->send_broadcast_flag;
    send_para->send_mode = demo_ctrl->send_mode;
    send_para->resend_cnt = demo_ctrl->send_resend_cnt;
    send_para->data_length = data_size;
    if (is_retranf == TD_TRUE) {
        demo_ctrl->send_index = demo_ctrl->rcv_send_index;
    } else {
        demo_ctrl->send_index++;
    }

    if (demo_ctrl->send_index == 0) {
        demo_ctrl->send_index++;
    }
    fc_frame = (app_fast_send_fc_demo_frame_st *)((td_u8 *)send_para + sizeof(ext_mdm_bb_plc_send_para));
    fc_frame->data_index = demo_ctrl->send_index;
    fc_frame->payload_len = payload_size;
    offset = sizeof(ext_mdm_bb_plc_send_para) + APP_FAST_SEND_FC_DEMO_FRAME_SIZE;

    app_set_fast_send_fc_data(data_demo);

    if (memcpy_s(((td_u8 *)send_para) + offset, malloc_size - offset, data_demo, payload_size) != EXT_ERR_SUCCESS) {
        uapi_free(EXT_MOD_ID_APP_COMMON, send_para);
        return EXT_ERR_FAILURE;
    }

    if (send_para->send_mode == EXT_MAC_FAST_SEND_PREEMPT) {
        uapi_set_disable_sending(APP_FAST_SEND_DISABLE_SEND_DURTION_MS);
    }

    ret = uapi_fast_send_sack_on_plc(send_para);
    uapi_free(EXT_MOD_ID_APP_COMMON, send_para);

    return ret;
}

td_void app_fast_send_fc_rcv_data(ext_mac_report_rcv_fast_send_pkt *rcv_data)
{
    app_fast_send_demo_ctrl_st *demo_ctrl = app_fast_send_ctrl();
    app_fast_send_fc_demo_frame_st *fc_data = TD_NULL;
    diag_cmd_fast_send_demo_req req_cmd = { 0 };

    fc_data = (app_fast_send_fc_demo_frame_st *)rcv_data->data;
    if (demo_ctrl->rcv_send_index == fc_data->data_index) {
        return;
    }

    demo_ctrl->rcv_send_index = fc_data->data_index;
    if (uapi_get_my_role() == IS_PROXY) {
        req_cmd.set_enable = TD_TRUE;
        req_cmd.set_plc_mode = APP_FAST_SEND_FC_MODE;
        req_cmd.resend_cnt = APP_FAST_SEND_RESEND_CNT;
        req_cmd.send_broadcast_flag = 1;
        req_cmd.set_send_cnt = 0;
        req_cmd.set_send_mode = EXT_MAC_FAST_SEND_PRIORITY;
        req_cmd.set_send_period = 0;
        req_cmd.set_send_phase = 0;
        app_fast_send_demo_start(&req_cmd, TD_TRUE);
    }
}

td_u32 app_fast_send_fc_rcv_init(td_void)
{
    return uapi_register_fast_send_fc_callback(app_fast_send_fc_rcv_data);
}

td_void app_fast_send_demo_init(td_void)
{
    td_u32 ret;

    ret = app_fast_send_plc_chl_demo_init();
    if (ret != EXT_ERR_SUCCESS) {
        return;
    }

    ret = app_fast_send_fc_rcv_init();
    if (ret != EXT_ERR_SUCCESS) {
        return;
    }
}

#endif
