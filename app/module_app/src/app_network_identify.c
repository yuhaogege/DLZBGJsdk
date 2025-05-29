/*
 * Copyright (c) CompanyNameMagicTag 2019-2020. All rights reserved.
 * Description: impl of APP module protocol network self identify
 */

#include "soc_types.h"
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
#include "app_network_identify.h"
#include "app_nv.h"
#include "app_common.h"
#include "app_res.h"
#include "app_proto.h"
#include "app_proto_proc.h"
#include "app_cmd_msg.h"
#include "app_cco_whitelist.h"
#include "soc_mdm_nv.h"
#include "soc_mdm_mac.h"
#include "soc_stdlib.h"
#include "soc_mdm_mem.h"
#include "soc_mdm_time.h"
#include "soc_mdm_crc.h"

app_network_identify_ctrl g_app_ni_ctrl = { 0 };
diag_cmd_ni_dfx_info g_app_ni_dfx_info = { 0 };

td_bool app_ni_is_same_network(td_u16 tei);
td_bool app_ni_is_rcv_respond(td_u16 tei);
td_void app_ni_set_rsp_state(td_u16 tei);
td_void app_ni_set_same_network_state(td_u16 tei, td_bool is_same);
td_u32 app_ni_get_next_qry_sta_mac(td_u8 *next_sta_mac, td_u8 mac_len, td_u16 cur_qry_tei);
td_bool app_ni_is_need_qrying_sta_by_tei(td_u16 qry_tei);
td_u16 app_ni_get_blk_idex(td_u16 tei);
td_u16 app_ni_get_pos_idex(td_u16 tei);
td_void app_ni_create_and_save_white_list(td_void);
td_void app_ni_set_cur_state(app_ni_state state);
td_u8 app_ni_get_cur_state(td_void);
td_bool app_ni_is_able_to_qry(td_void);
td_u32 app_ni_get_tei_by_mac(td_u8 *target_mac, td_u8 mac_len, td_u16 *tei);
td_u32 app_ni_send_qry_frame_to_plc(td_u8 *mac_addr, td_u8 mac_len, td_u16 frame_index);
td_u32 app_ni_get_timer_id_by_index(td_u8 index);
td_u32 app_ni_get_free_timer_id(td_u8 *index, td_u32 *timer_id);
td_void app_ni_set_new_timer(td_u8 *dst_mac, td_u8 mac_len, td_u32 timer_id, td_u8 index);
td_void app_ni_reset_timer(td_u8 *dst_mac, td_u8 mac_len);
td_u8 app_ni_get_index_by_timer_id(td_u32 timer_id);

diag_cmd_ni_dfx_info *app_ni_get_dfx_info(td_void)
{
    return &g_app_ni_dfx_info;
}

app_network_identify_ctrl *app_ni_get_ctrl_info(td_void)
{
    return &g_app_ni_ctrl;
}

td_void app_ni_update_dfx_info(td_void)
{
    app_network_identify_ctrl *ni_ctrl_info = app_ni_get_ctrl_info();
    diag_cmd_ni_dfx_info *ni_dfx_info = app_ni_get_dfx_info();

    ni_dfx_info->cur_state = ni_ctrl_info->cur_state;
    ni_dfx_info->cur_send_cycle_cnt = ni_ctrl_info->cur_qry_cycle;
    ni_dfx_info->next_qry_sta_tei = ni_ctrl_info->next_qry_sta_tei;
}

td_u8 app_ni_get_cur_state(td_void)
{
    app_network_identify_ctrl *ni_ctrl_info = app_ni_get_ctrl_info();

    return ni_ctrl_info->cur_state;
}

td_void app_ni_set_cur_state(app_ni_state state)
{
    app_network_identify_ctrl *ni_ctrl_info = app_ni_get_ctrl_info();

    ni_ctrl_info->cur_state = (td_u8)state;
}

td_bool app_ni_is_able_to_qry(td_void)
{
    app_network_identify_ctrl *ni_ctrl_info = app_ni_get_ctrl_info();
    td_u16 index;
    td_u16 max_tei = uapi_topo_get_max_tei();
    td_u32 sta_state;

    if (ni_ctrl_info->cur_state != APP_NI_IDLE_STATE && ni_ctrl_info->cur_state != APP_NI_QUERY_STATE) {
        return TD_FALSE;
    }

    for (index = APP_START_QRY_TEI; index < max_tei; index++) {
        sta_state = uapi_topo_get_state_by_tei(index);
        if (sta_state == EXT_TP_NOT_USED || sta_state == EXT_TP_INVALID_PARAMETER) {
            continue;
        }

        if (app_ni_is_rcv_respond(index) == TD_FALSE) {
            return TD_TRUE;
        }
    }

    return TD_FALSE;
}

td_u32 app_ni_set_identify_state(td_bool is_enable)
{
    module_cco_identify_inf nv_value = { 0 };
    td_u32 ret;

    (td_void) memset_s(&nv_value, sizeof(nv_value), 0, sizeof(nv_value));
    ret = uapi_nv_read(ID_NV_APP_MOUDULE_CCO_IDENTIFY, &nv_value, sizeof(nv_value));
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    if (nv_value.is_network_idf_enable == is_enable) {
        return EXT_ERR_SUCCESS;
    }

    nv_value.is_network_idf_enable = is_enable;
    return uapi_nv_write(ID_NV_APP_MOUDULE_CCO_IDENTIFY, &nv_value, sizeof(nv_value));
}

td_u32 app_ni_set_self_identify_params(td_bool state, td_u8 wait_time, td_u16 time_diff)
{
    module_cco_identify_inf nv_value = { 0 };
    td_u32 ret;

    ret = uapi_nv_read(ID_NV_APP_MOUDULE_CCO_IDENTIFY, &nv_value, sizeof(nv_value));
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    if (nv_value.is_network_idf_enable == state && ((nv_value.time_diff == time_diff) ||
        (nv_value.time_diff == APP_DEFAULT_WAIT_STA_TIME && time_diff == 0)) &&
        nv_value.wait_new_sta_max_time == wait_time) {
        return EXT_ERR_SUCCESS;
    }
    nv_value.time_diff = (time_diff == 0) ? APP_DEFAULT_WAIT_STA_TIME : time_diff;
    nv_value.is_network_idf_enable = state;
    nv_value.wait_new_sta_max_time = wait_time;
    return uapi_nv_write(ID_NV_APP_MOUDULE_CCO_IDENTIFY, &nv_value, sizeof(nv_value));
}

td_u32 app_ni_handle_init_nv_value(td_u8 *state)
{
    app_network_identify_ctrl *ni_ctrl_info = app_ni_get_ctrl_info();
    module_cco_identify_inf nv_value = { 0 };
    td_u32 ret;
    td_bool is_update = TD_FALSE;

    ret = uapi_nv_read(ID_NV_APP_MOUDULE_CCO_IDENTIFY, &nv_value, sizeof(nv_value));
    if (ret != EXT_ERR_SUCCESS) {
        return EXT_ERR_FAILURE;
    }

    if (nv_value.is_network_idf_enable == TD_FALSE) {
        return EXT_ERR_SUCCESS;
    }
    *state = TD_TRUE;
    ni_ctrl_info->total_parallel_num = nv_value.parallel_num;
    if (ni_ctrl_info->total_parallel_num > APP_NI_MAX_PARAL_NUM || ni_ctrl_info->total_parallel_num == 0) {
        ni_ctrl_info->total_parallel_num = APP_NI_MAX_PARAL_NUM;
        nv_value.parallel_num = APP_NI_MAX_PARAL_NUM;
        is_update = TD_TRUE;
    }

    ni_ctrl_info->wait_new_sta_join_time = nv_value.wait_new_sta_max_time;
    if (ni_ctrl_info->wait_new_sta_join_time == 0) {
        ni_ctrl_info->wait_new_sta_join_time = APP_DEFAULT_WAIT_STA_TIME;
        nv_value.wait_new_sta_max_time = APP_DEFAULT_WAIT_STA_TIME;
        is_update = TD_TRUE;
    }

    ni_ctrl_info->time_diff_nv = nv_value.time_diff;
    if (is_update == TD_TRUE) {
        uapi_nv_write(ID_NV_APP_MOUDULE_CCO_IDENTIFY, &nv_value, sizeof(nv_value));
    }

    return EXT_ERR_SUCCESS;
}

td_u32 app_ni_init(td_void)
{
    app_network_identify_ctrl *ni_ctrl_info = app_ni_get_ctrl_info();
    td_u32 total_bit_num = EXT_MDM_NM_TOPO_NODE_NUM_MAX * APP_PER_STA_USE_BIT_NUM + EXT_U8_BITS - 1;
    td_u16 size;
    td_u8 open_state = TD_FALSE;

    if (ni_ctrl_info->cur_state != APP_NI_INIT_STATE) {
        return EXT_ERR_SUCCESS;
    }

    (td_void) memset_s(ni_ctrl_info, sizeof(app_network_identify_ctrl), 0, sizeof(app_network_identify_ctrl));
    if (app_ni_handle_init_nv_value(&open_state) != EXT_ERR_SUCCESS) {
        return EXT_ERR_FAILURE;
    }
    if (open_state == TD_FALSE) {
        return EXT_ERR_SUCCESS;
    }
    ni_ctrl_info->sta_qry_info = (td_u8 *)uapi_malloc(EXT_MOD_ID_APP_COMMON, total_bit_num / EXT_U8_BITS);
    if (ni_ctrl_info->sta_qry_info == TD_NULL) {
        return EXT_ERR_MALLOC_FAILURE;
    }
    size = ni_ctrl_info->total_parallel_num * sizeof(app_parall_timer_info);
    ni_ctrl_info->timer_ctrl = (app_parall_timer_info *)uapi_malloc(EXT_MOD_ID_APP_COMMON, size);
    if (ni_ctrl_info->timer_ctrl == TD_NULL) {
        return EXT_ERR_MALLOC_FAILURE;
    }
    (td_void)memset_s(ni_ctrl_info->timer_ctrl, size, 0, size);
    (td_void)memset_s(ni_ctrl_info->sta_qry_info, total_bit_num / EXT_U8_BITS, 0, total_bit_num / EXT_U8_BITS);
    uapi_get_topo_info(&ni_ctrl_info->topo_list);
    if (ni_ctrl_info->topo_list == TD_NULL) {
        return EXT_ERR_FAILURE;
    }

    ni_ctrl_info->cur_state = APP_NI_IDLE_STATE;
    ni_ctrl_info->next_qry_sta_tei = APP_START_QRY_TEI;
    ni_ctrl_info->max_tei = EXT_MDM_NM_TOPO_NODE_NUM_MAX;
    ni_ctrl_info->current_parallel_num = 0;
    ni_ctrl_info->frame_index = 0;
    ni_ctrl_info->white_list_state = APP_NI_WL_INVALID;
    uapi_get_white_list_switch(&ni_ctrl_info->white_list_switch);
    if (ni_ctrl_info->white_list_switch == TD_TRUE) {
        (td_void)uapi_set_white_list_switch(TD_FALSE, TD_FALSE, EXT_MAC_WHITE_LIST_CHANGE_REASON_APP_INIT);
    }

    return EXT_ERR_SUCCESS;
}

td_void app_ni_update_cycle_by_next_tei(td_u16 next_tei)
{
    app_network_identify_ctrl *ni_ctrl_info = app_ni_get_ctrl_info();

    if (next_tei > uapi_topo_get_max_tei()) {
        ni_ctrl_info->next_qry_sta_tei = APP_START_QRY_TEI;
        ni_ctrl_info->cur_qry_cycle++;
        ni_ctrl_info->frame_index++;
    }
}

td_void app_ni_handle_plc_send_result(td_u32 result)
{
    diag_cmd_ni_dfx_info *ni_dfx_info = app_ni_get_dfx_info();
    app_network_identify_ctrl *ni_ctrl_info = app_ni_get_ctrl_info();

    if (result == EXT_ERR_SUCCESS) {
        ni_dfx_info->send_qry_frame_success_cnt++;
    } else {
        ni_dfx_info->send_qry_frame_fail_cnt++;
    }
    ni_ctrl_info->current_parallel_num++;
    ni_ctrl_info->next_qry_sta_tei++;
}
/* parallel 5 */
td_void app_ni_start_send_qry_process(td_void)
{
    app_network_identify_ctrl *ni_ctrl_info = app_ni_get_ctrl_info();
    td_u16 total_node_num = uapi_get_topo_node_num();
    td_u8 next_mac[EXT_PLC_MAC_ADDR_LEN] = { 0 };
    td_u32 ret, sta_state;
    td_u16 max_tei = uapi_topo_get_max_tei();
    td_u32 timer_id = 0;
    td_u8 index = 0;

    if (app_ni_is_able_to_qry() == TD_FALSE || total_node_num <= APP_NODE_NUM_ONE) { /*  */
        return;
    }

    /* Send query frame by simulating five concurrent frames */
    while (ni_ctrl_info->current_parallel_num < ni_ctrl_info->total_parallel_num &&
           ni_ctrl_info->cur_qry_cycle < APP_NI_MAX_QRY_CLCYE) {
        if (ni_ctrl_info->current_parallel_num > 0 && ni_ctrl_info->next_qry_sta_tei > max_tei) {
            break;
        }

        app_ni_update_cycle_by_next_tei(ni_ctrl_info->next_qry_sta_tei);
        if (ni_ctrl_info->cur_qry_cycle >= APP_NI_MAX_QRY_CLCYE) {
            ni_ctrl_info->cur_state = APP_NI_END_QRY_STATE;
            break;
        }

        sta_state = uapi_topo_get_state_by_tei(ni_ctrl_info->next_qry_sta_tei);
        if (sta_state == EXT_TP_NOT_USED || sta_state == EXT_TP_INVALID_PARAMETER) {
            ni_ctrl_info->next_qry_sta_tei++;  /* site not online need to be skipped */
            continue;
        }

        /* Whether the response from the site has been received */
        if (app_ni_is_need_qrying_sta_by_tei(ni_ctrl_info->next_qry_sta_tei) == TD_FALSE) {
            ni_ctrl_info->next_qry_sta_tei++;
            continue;
        }

        if (memcpy_s(next_mac, sizeof(next_mac), ni_ctrl_info->topo_list[ni_ctrl_info->next_qry_sta_tei - 1].mac,
                     EXT_PLC_MAC_ADDR_LEN) != EXT_ERR_SUCCESS) {
            break;
        }

        if (app_ni_get_free_timer_id(&index, &timer_id) != EXT_ERR_SUCCESS) {
            break;
        }

        ret = app_ni_send_qry_frame_to_plc(next_mac, sizeof(next_mac), ni_ctrl_info->frame_index);
        app_ni_handle_plc_send_result(ret);

        app_ni_set_new_timer(next_mac, sizeof(next_mac), timer_id, index);
        app_timer_start(APP_TIMER_ID_NI_WAIT_NEW_STA_JOIN, ni_ctrl_info->wait_new_sta_join_time * APP_NI_MS_PER_MIN,
                        EXT_TIMER_TYPE_ONCE);
    }
}

td_bool app_ni_is_start_to_qry(td_void)
{
    td_bool is_white_list_open = TD_FALSE;
    app_network_identify_ctrl *ni_ctrl_info = app_ni_get_ctrl_info();

    if (uapi_get_white_list_switch(&is_white_list_open) != EXT_ERR_SUCCESS) {
        return TD_FALSE;
    }

    if (is_white_list_open == TD_TRUE) {
        return TD_FALSE;
    }

    if (ni_ctrl_info->cur_state == APP_NI_INIT_STATE || ni_ctrl_info->cur_state == APP_NI_STOP_STATE) {
        return TD_FALSE;
    }

    return TD_TRUE;
}

td_void app_ni_new_sta_join_notify(td_u16 tei)
{
    app_network_identify_ctrl *ni_ctrl_info = app_ni_get_ctrl_info();

    if (app_ni_is_start_to_qry() == TD_FALSE || ni_ctrl_info->cur_state == APP_NI_IDLE_STATE) {
        return;
    }

    if (is_sta_tei_valid(tei)) {
        if (app_ni_is_need_qrying_sta_by_tei(tei) == TD_FALSE) {
            return;
        }
    }

    ni_ctrl_info->cur_state = APP_NI_QUERY_STATE;
    ni_ctrl_info->cur_qry_cycle = 0;

    app_timer_start(APP_TIMER_ID_NI_WAIT_NEW_STA_JOIN, ni_ctrl_info->wait_new_sta_join_time * APP_NI_MS_PER_MIN,
                    EXT_TIMER_TYPE_ONCE);
}

td_void app_ni_restart_querying_sta_poweron_time(td_void)
{
    app_network_identify_ctrl *ni_ctrl_info = app_ni_get_ctrl_info();

    if (app_ni_is_start_to_qry() == TD_FALSE || ni_ctrl_info->cur_state == APP_NI_IDLE_STATE) {
        return;
    }

    if (ni_ctrl_info->cur_state == APP_NI_END_QRY_STATE) {
        ni_ctrl_info->cur_state = APP_NI_QUERY_STATE;
        ni_ctrl_info->cur_qry_cycle = 0;
    }

    app_ni_start_send_qry_process();
}

td_void app_ni_cco_formed_notify(td_void)
{
    diag_cmd_ni_dfx_info *ni_dfx_info = app_ni_get_dfx_info();

    if (app_ni_is_start_to_qry() == TD_FALSE) {
        return;
    }

    ni_dfx_info->start_query_time = uapi_get_milli_seconds();
    app_ni_set_cur_state(APP_NI_QUERY_STATE);
    app_ni_start_send_qry_process();
}

td_u32 app_on_msg_ni_send_frame(ext_sys_queue_msg *msg)
{
    diag_cmd_ni_dfx_info *ni_dfx_info = app_ni_get_dfx_info();
    app_proto_frame_head *data_field;
    td_u8 mac_addr[EXT_PLC_MAC_ADDR_LEN] = { 0 };
    td_pbyte payload;
    td_u8 head_len = sizeof(app_proto_frame_head);
    td_u16 crc, data_size;
    td_u16 dtei = 0;
    td_u32 ret;

    if (msg == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    if (memcpy_s(mac_addr, sizeof(mac_addr), msg->param, EXT_PLC_MAC_ADDR_LEN) != EXT_ERR_SUCCESS) {
        return EXT_ERR_FAILURE;
    }

    if (!is_valid_mac_addr(mac_addr)) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    if (app_ni_get_tei_by_mac(mac_addr, sizeof(mac_addr), &dtei) != EXT_ERR_SUCCESS) {
        return EXT_ERR_FAILURE;
    }

    if (app_ni_is_rcv_respond(dtei) == TD_TRUE) {
        return EXT_ERR_SUCCESS;
    }

    data_size = head_len + PROTO_CRC_SIZE;
    data_field = (app_proto_frame_head *)uapi_malloc(EXT_MOD_ID_APP_COMMON, data_size);
    if (data_field == TD_NULL) {
        return EXT_ERR_MALLOC_FAILURE;
    }

    (td_void)memset_s(data_field, data_size, 0, data_size);
    data_field->head = APP_FRAME_HEAD_BYTE;
    data_field->ctrl = PROTO_CTRL_REQ;
    data_field->cmd = ID_PROTO_CMD_QRY_ONPOWER_TIME;
    data_field->seq = app_proto_get_frame_seq();
    data_field->length = 0;

    payload = (td_pbyte)data_field;
    crc = uapi_crc16(0, (EXT_CONST td_u8 *)payload, head_len);
    payload[head_len] = (td_u8)(crc >> PROTO_CRC_HIGH_BIT_OFFSET);
    payload[head_len + 1] = (td_u8)crc;

    app_proto_frame_seq_add();
    ret = app_proto_plc_tx((td_pbyte)data_field, data_size, (td_pbyte)mac_addr, ID_PLC_REMOTE_CMD);
    if (ret != EXT_ERR_SUCCESS) {
        ni_dfx_info->send_qry_frame_fail_cnt++;
    } else {
        ni_dfx_info->send_qry_frame_success_cnt++;
    }

    uapi_free(EXT_MOD_ID_APP_COMMON, data_field);

    return EXT_ERR_SUCCESS;
}

td_void app_ni_handle_wait_sta_join_timeout(td_void)
{
    app_network_identify_ctrl *ni_ctrl_info = app_ni_get_ctrl_info();

    ni_ctrl_info->cur_state = APP_NI_STOP_STATE;
    app_ni_create_and_save_white_list();
    if (ni_ctrl_info->sta_qry_info != TD_NULL) {
        uapi_free(EXT_MOD_ID_APP_COMMON, ni_ctrl_info->sta_qry_info);
        ni_ctrl_info->sta_qry_info = TD_NULL;
    }

    if (ni_ctrl_info->timer_ctrl != TD_NULL) {
        uapi_free(EXT_MOD_ID_APP_COMMON, ni_ctrl_info->timer_ctrl);
        ni_ctrl_info->timer_ctrl = TD_NULL;
    }

    if (app_ni_set_identify_state(TD_FALSE) != EXT_ERR_SUCCESS) {}
}

td_void app_ni_wait_sta_rsp_timeout(td_u32 timer_id)
{
    app_network_identify_ctrl *ni_ctrl_info = app_ni_get_ctrl_info();
    diag_cmd_ni_dfx_info *ni_dfx_info = app_ni_get_dfx_info();
    td_u8 index = app_ni_get_index_by_timer_id(timer_id);

    ni_dfx_info->wait_sta_rsp_timeout_cnt++;
    if (index > ni_ctrl_info->total_parallel_num) {
        return;
    }

    if (ni_ctrl_info->timer_ctrl == TD_NULL) {
        return;
    }

    (td_void)memset_s(ni_ctrl_info->timer_ctrl[index].mac_addr, sizeof(ni_ctrl_info->timer_ctrl[index].mac_addr),
                      0, EXT_PLC_MAC_ADDR_LEN);

    if (ni_ctrl_info->current_parallel_num > 0) {
        ni_ctrl_info->current_parallel_num--;
    }

    app_ni_start_send_qry_process();
}

td_u32 app_ni_get_next_qry_sta_mac(td_u8 *next_sta_mac, td_u8 mac_len, td_u16 cur_qry_tei)
{
    app_network_identify_ctrl *ni_ctrl_info = app_ni_get_ctrl_info();
    td_u16 max_tei = uapi_topo_get_max_tei();

    if (next_sta_mac == TD_NULL || mac_len < EXT_PLC_MAC_ADDR_LEN) {
        return EXT_ERR_FAILURE;
    }

    if (cur_qry_tei >= max_tei || cur_qry_tei <= APP_NI_CCO_TEI) {
        return EXT_ERR_FAILURE;
    }

    if (memcpy_s(next_sta_mac, mac_len, ni_ctrl_info->topo_list[cur_qry_tei - 1].mac,
                 EXT_PLC_MAC_ADDR_LEN) != EXT_ERR_SUCCESS) {
        return EXT_ERR_FAILURE;
    }

    return EXT_ERR_SUCCESS;
}

td_bool app_ni_is_need_qrying_sta_by_tei(td_u16 qry_tei)
{
    app_network_identify_ctrl *ni_ctrl_info = app_ni_get_ctrl_info();
    td_bool is_rcv_rsp;

    if (qry_tei < APP_START_QRY_TEI || qry_tei > EXT_MDM_NM_TOPO_NODE_NUM_MAX) {
        return TD_FALSE;
    }

    if (ni_ctrl_info->sta_qry_info == TD_NULL) {
        return TD_FALSE;
    }

    is_rcv_rsp = app_ni_is_rcv_respond(qry_tei);

    return (is_rcv_rsp == TD_FALSE) ? TD_TRUE : TD_FALSE;
}

td_void app_ni_set_same_network_state(td_u16 tei, td_bool is_same)
{
    app_network_identify_ctrl *ni_ctrl_info = app_ni_get_ctrl_info();
    td_u16 blk_idx;
    td_u16 pos_idx;
    td_u16 network_idx;

    if (!is_sta_tei_valid(tei)) {
        return;
    }

    blk_idx = app_ni_get_blk_idex(tei);
    pos_idx = app_ni_get_pos_idex(tei);
    network_idx = pos_idx + 1;
    if (is_same == TD_FALSE) {
        ni_ctrl_info->sta_qry_info[blk_idx] &= (td_u8)(~(td_u8)(1 << network_idx));
    } else {
        ni_ctrl_info->sta_qry_info[blk_idx] |= (td_u8)(1 << network_idx);
    }
}

td_void app_ni_set_rsp_state(td_u16 tei)
{
    app_network_identify_ctrl *ni_ctrl_info = app_ni_get_ctrl_info();
    td_u16 blk_idx;
    td_u16 pos_idx;

    if (!is_sta_tei_valid(tei)) {
        return;
    }

    blk_idx = app_ni_get_blk_idex(tei);
    pos_idx = app_ni_get_pos_idex(tei);
    ni_ctrl_info->sta_qry_info[blk_idx] |= (td_u8)(1 << pos_idx);
}

td_bool app_ni_is_same_network(td_u16 tei)
{
    app_network_identify_ctrl *ni_ctrl_info = app_ni_get_ctrl_info();
    td_u16 blk_idx = app_ni_get_blk_idex(tei);
    td_u16 pos_idx = app_ni_get_pos_idex(tei);
    td_u16 network_idx = pos_idx + 1;
    td_u16 cur_value = ni_ctrl_info->sta_qry_info[blk_idx];

    return (td_bool)((cur_value >> network_idx) & 0x1);
}

td_bool app_ni_is_rcv_respond(td_u16 tei)
{
    app_network_identify_ctrl *ni_ctrl_info = app_ni_get_ctrl_info();
    td_u16 blk_idx = app_ni_get_blk_idex(tei);
    td_u16 pos_idx = app_ni_get_pos_idex(tei);
    td_u16 rsp_idx = pos_idx;
    td_u16 cur_value = ni_ctrl_info->sta_qry_info[blk_idx];

    return (td_bool)((td_u8)(cur_value >> rsp_idx) & 0x1);
}

td_u32 app_ni_time_get_duration(EXT_IN td_u32 starttime, EXT_IN td_u32 endtime)
{
    if (endtime >= starttime) {
        return (endtime - starttime);
    } else {
        return (starttime - endtime);
    }
}

td_u32 app_ni_get_tei_by_mac(td_u8 *target_mac, td_u8 mac_len, td_u16 *tei)
{
    td_u16 index;
    app_network_identify_ctrl *ni_ctrl_info = app_ni_get_ctrl_info();
    td_u16 mac_tei = uapi_topo_get_max_tei();

    if (target_mac == TD_NULL || mac_len != EXT_PLC_MAC_ADDR_LEN) {
        return EXT_ERR_FAILURE;
    }

    for (index = 0; index < mac_tei; index++) {
        if (memcmp(ni_ctrl_info->topo_list[index].mac, target_mac, mac_len) == 0) {
            *tei = index + 1;
            return EXT_ERR_SUCCESS;
        }
    }

    return EXT_ERR_FAILURE;
}

td_u32 app_ni_handle_rcv_sta_rsp(td_pbyte data, td_u16 data_length, td_pbyte src_addr, td_u8 addr_len)
{
    proto_qry_on_power_time_ind *sta_rsp = TD_NULL;
    app_network_identify_ctrl *ni_ctrl_info = app_ni_get_ctrl_info();
    diag_cmd_ni_dfx_info *ni_dfx_info = app_ni_get_dfx_info();
    td_u32 time_diff;
    td_u16 rcv_tei;
    td_bool is_same;
    td_u32 cco_cur_ms_time;

    if (data == TD_NULL || data_length != sizeof(proto_qry_on_power_time_ind) || src_addr == TD_NULL ||
        addr_len != EXT_PLC_MAC_ADDR_LEN) {
        return EXT_ERR_FAILURE;
    }

    if (ni_ctrl_info->cur_state == APP_NI_INIT_STATE) {
        return EXT_ERR_SUCCESS;
    }

    ni_dfx_info->rcv_sta_rsp_cnt++;
    sta_rsp = (proto_qry_on_power_time_ind *)data;
    if (app_ni_get_tei_by_mac(sta_rsp->addr, sizeof(sta_rsp->addr), &rcv_tei) != EXT_ERR_SUCCESS) {
        return EXT_ERR_FAILURE;
    }

    if (ni_ctrl_info->frame_index != sta_rsp->frame_index) {
        return EXT_ERR_SUCCESS;
    }

    if (app_ni_is_rcv_respond(rcv_tei) == TD_TRUE) {
        ni_dfx_info->repeat_sta_rsp_cnt++;
        return EXT_ERR_SUCCESS;
    }

    if (ni_ctrl_info->current_parallel_num > 0) {
        ni_ctrl_info->current_parallel_num--;
    }

    app_ni_reset_timer(sta_rsp->addr, sizeof(sta_rsp->addr));
    cco_cur_ms_time = uapi_get_milli_seconds();
    time_diff = app_ni_time_get_duration(sta_rsp->on_power_ms_time, cco_cur_ms_time);
    if (time_diff < ni_ctrl_info->time_diff_nv) {
        is_same = TD_TRUE;
        ni_dfx_info->same_network_sta_cnt++;
    } else {
        is_same = TD_FALSE;
        ni_dfx_info->different_network_sta_cnt++;
    }

    app_ni_set_rsp_state(rcv_tei);
    app_ni_set_same_network_state(rcv_tei, is_same);

    app_ni_start_send_qry_process();

    return EXT_ERR_SUCCESS;
}

td_u16 app_ni_get_blk_idex(td_u16 tei)
{
    return (tei - APP_START_QRY_TEI) / (EXT_U8_BITS / APP_PER_STA_USE_BIT_NUM);
}

td_u16 app_ni_get_pos_idex(td_u16 tei)
{
    return (tei - APP_START_QRY_TEI) % (EXT_U8_BITS / APP_PER_STA_USE_BIT_NUM) * APP_PER_STA_USE_BIT_NUM;
}

td_void app_ni_create_and_save_white_list(td_void)
{
    app_network_identify_ctrl *ni_ctrl_info = app_ni_get_ctrl_info();
    diag_cmd_ni_dfx_info *ni_dfx_info = app_ni_get_dfx_info();
    td_u16 sta_tei;
    td_u16 white_list_num = 0;
    td_u16 max_tei = uapi_topo_get_max_tei();
    td_u16 exist_num;

    if (ni_ctrl_info->cur_state != APP_NI_STOP_STATE) {
        return;
    }
    exist_num = app_get_whitelist_sta_num();
    app_whitelist_clear();
    ni_dfx_info->start_add_whitelist_time = uapi_get_milli_seconds();
    for (sta_tei = APP_START_QRY_TEI; sta_tei <= max_tei; sta_tei++) {
        if (app_ni_is_same_network(sta_tei) == TD_FALSE || app_ni_is_rcv_respond(sta_tei) == TD_FALSE) {
            continue;
        }

        if (app_whitelist_add(ni_ctrl_info->topo_list[sta_tei - 1].mac, 1) == EXT_ERR_SUCCESS) {
            ni_dfx_info->add_sta_to_white_list_num++;
            white_list_num++;
        }
    }
    ni_ctrl_info->white_list_state = APP_NI_WL_UNUSED;
    if (exist_num == white_list_num && white_list_num == 0) {
        app_ni_open_white_list_switch();
    }
}

td_u32 app_ni_send_qry_frame_to_plc(td_u8 *mac_addr, td_u8 mac_len, td_u16 frame_index)
{
    app_proto_frame_head *data_field;
    td_pbyte payload;
    td_u8 head_len = sizeof(app_proto_frame_head);
    td_u16 crc, data_size;
    td_u16 dtei = 0;
    td_u32 ret;
    td_u8 offset;

    if (mac_addr == TD_NULL || mac_len != EXT_PLC_MAC_ADDR_LEN) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    if (!is_valid_mac_addr(mac_addr)) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    if (app_ni_get_tei_by_mac(mac_addr, mac_len, &dtei) != EXT_ERR_SUCCESS) {
        return EXT_ERR_FAILURE;
    }

    if (app_ni_is_rcv_respond(dtei) == TD_TRUE) {
        return EXT_ERR_SUCCESS;
    }

    data_size = head_len + sizeof(frame_index) + PROTO_CRC_SIZE;
    data_field = (app_proto_frame_head *)uapi_malloc(EXT_MOD_ID_APP_COMMON, data_size);
    if (data_field == TD_NULL) {
        return EXT_ERR_MALLOC_FAILURE;
    }

    (td_void)memset_s(data_field, data_size, 0, data_size);
    data_field->head = APP_FRAME_HEAD_BYTE;
    data_field->ctrl = PROTO_CTRL_REQ;
    data_field->cmd = ID_PROTO_CMD_QRY_ONPOWER_TIME;
    data_field->seq = app_proto_get_frame_seq();
    data_field->length = sizeof(frame_index);

    if (memcpy_s(data_field->data, sizeof(frame_index), &frame_index, sizeof(frame_index)) != EXT_ERR_SUCCESS) {
        return EXT_ERR_FAILURE;
    }

    offset = head_len + sizeof(frame_index);
    payload = (td_pbyte)data_field;
    crc = uapi_crc16(0, (EXT_CONST td_u8 *)payload, head_len + sizeof(frame_index));
    payload[offset] = (td_u8)(crc >> PROTO_CRC_HIGH_BIT_OFFSET);
    payload[offset + 1] = (td_u8)crc;

    app_proto_frame_seq_add();
    ret = app_proto_plc_tx((td_pbyte)data_field, data_size, (td_pbyte)mac_addr, ID_PLC_REMOTE_CMD);

    uapi_free(EXT_MOD_ID_APP_COMMON, data_field);

    return ret;
}

td_void app_ni_set_new_timer(td_u8 *dst_mac, td_u8 mac_len, td_u32 timer_id, td_u8 index)
{
    app_network_identify_ctrl *ni_ctrl_info = app_ni_get_ctrl_info();

    if (timer_id > APP_TIMER_ID_MAX || index > ni_ctrl_info->total_parallel_num) {
        return;
    }

    if (memcpy_s(ni_ctrl_info->timer_ctrl[index].mac_addr, sizeof(ni_ctrl_info->timer_ctrl[index].mac_addr),
                 dst_mac, mac_len) != EXT_ERR_SUCCESS) {
        return;
    }

    app_timer_start((app_timer_id)timer_id, APP_WAIT_RESPOND_MAX_TIME, EXT_TIMER_TYPE_ONCE);
}

td_u8 app_ni_get_index_by_timer_id(td_u32 timer_id)
{
    return(td_u8)(timer_id - APP_TIMER_ID_NI_WAIT_STA_RSP);
}

td_void app_ni_reset_timer(td_u8 *dst_mac, td_u8 mac_len)
{
    app_network_identify_ctrl *ni_ctrl_info = app_ni_get_ctrl_info();
    td_u8 index;
    td_u32 timer_id;

    if (ni_ctrl_info->timer_ctrl == TD_NULL) {
        return;
    }

    for (index = 0; index < ni_ctrl_info->total_parallel_num; index++) {
        if (memcmp(ni_ctrl_info->timer_ctrl[index].mac_addr, dst_mac, mac_len) == 0) {
            timer_id = app_ni_get_timer_id_by_index(index);
            app_timer_stop((app_timer_id)timer_id);
            (td_void)memset_s(ni_ctrl_info->timer_ctrl[index].mac_addr,
                              sizeof(ni_ctrl_info->timer_ctrl[index].mac_addr), 0, EXT_PLC_MAC_ADDR_LEN);
            break;
        }
    }
}

td_u32 app_ni_get_timer_id_by_index(td_u8 index)
{
    if (index > APP_TIMER_ID_MAX) {
        return APP_TIMER_ID_NI_WAIT_STA_RSP;
    }

    return index + APP_TIMER_ID_NI_WAIT_STA_RSP;
}

td_u32 app_ni_get_free_timer_id(td_u8 *index, td_u32 *timer_id)
{
    app_network_identify_ctrl *ni_ctrl_info = app_ni_get_ctrl_info();
    td_u8 cur_index;

    for (cur_index = 0; cur_index < ni_ctrl_info->total_parallel_num; cur_index++) {
        if (is_invalid_mac_addr(ni_ctrl_info->timer_ctrl[cur_index].mac_addr)) {
            *index = cur_index;
            *timer_id = cur_index + APP_TIMER_ID_NI_WAIT_STA_RSP;
            return EXT_ERR_SUCCESS;
        }
    }

    return EXT_ERR_FAILURE;
}

td_void app_ni_open_white_list_switch(td_void)
{
    diag_cmd_ni_dfx_info *ni_dfx_info = app_ni_get_dfx_info();
    app_network_identify_ctrl *ni_ctrl = app_ni_get_ctrl_info();

    if (ni_ctrl->white_list_state != APP_NI_WL_UNUSED) {
        return;
    }
    ni_dfx_info->set_whitelist_time = uapi_get_milli_seconds();
    ni_ctrl->white_list_state = APP_NI_WL_EFFECTIVE;
    if (ni_ctrl->white_list_switch == TD_TRUE) {
        (td_void)uapi_set_white_list_switch(TD_TRUE, TD_FALSE, EXT_MAC_WHITE_LIST_CHANGE_REASON_APP_INIT);
    }
}
#endif
