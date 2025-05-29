/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: Diag command handle.
 */

#include "app_diag_cmd.h"
#include "app_cmd_msg.h"
#include "dfx_app.h"
#include "app_cco_whitelist.h"
#include "app_common.h"
#include "app_fast_send_demo.h"
#include "app_network_identify.h"
#if defined(PRODUCT_CFG_PLC_MODULE_APP_TEST)
#include "app_plc_test.h"
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
#ifdef __cplusplus
extern "C" {
#endif

/* whitelist: clear */
td_u32 app_cmd_whitelist_clear(td_u16 cmd_id, td_pvoid cmd, td_u16 cmd_size, td_u8 option);
/* whitelist: query num */
td_u32 app_cmd_whitelist_query_num(td_u16 cmd_id, td_pvoid cmd, td_u16 cmd_size, td_u8 option);
/* whitelist: query info */
td_u32 app_cmd_whitelist_query_info(td_u16 cmd_id, td_pvoid cmd, td_u16 cmd_size, td_u8 option);
/* whitelist: add node */
td_u32 app_cmd_whitelist_add(td_u16 cmd_id, td_pvoid cmd, td_u16 cmd_size, td_u8 option);
/* whitelist: remove node */
td_u32 app_cmd_whitelist_delete(td_u16 cmd_id, td_pvoid cmd, td_u16 cmd_size, td_u8 option);
td_u32 app_cmd_ni_get_dfx_info(td_u16 cmd_id, td_pvoid cmd, td_u16 cmd_size, td_u8 option);

#if defined(PRODUCT_CFG_FAST_SEND_DEMO)
td_u32 app_cmd_fast_send_demo(td_u16 cmd_id, td_pvoid cmd, td_u16 cmd_size, td_u8 option);
#endif

td_u32 app_diag_cmd_proc(td_u16 cmd_id, td_pvoid cmd, td_u16 cmd_size, td_u8 option)
{
    ext_sys_queue_msg msg;
    td_pbyte payload = TD_NULL;
    td_u32 ret;

    if ((cmd != TD_NULL) && (cmd_size != 0)) {
        payload = (td_pbyte)uapi_malloc(EXT_MOD_ID_APP_COMMON, cmd_size);
        if (payload == TD_NULL) {
            return EXT_ERR_MALLOC_FAILUE;
        }

        (td_void) memcpy_s(payload, cmd_size, cmd, cmd_size);
    }

    msg.msg_id = ID_APP_MSG_DIAG;
    msg.param[0] = (td_u32)cmd_id;
    msg.param[APP_MODULE_MSG_PARAM_INDEX1] = (uintptr_t)payload;
    msg.param[APP_MODULE_MSG_PARAM_INDEX2] = (td_u32)cmd_size;
    msg.param[APP_MODULE_MSG_PARAM_INDEX3] = (td_u32)option;

    ret = app_send_message(&msg);
    if ((ret != EXT_ERR_SUCCESS) && (payload != TD_NULL)) {
        uapi_free(EXT_MOD_ID_APP_COMMON, payload);
        payload = TD_NULL;
    }

    return ret;
}

td_void app_on_msg_diag(EXT_CONST ext_sys_queue_msg *msg)
{
    ext_diag_cmd_reg_obj tbl[] = {
        { ID_DIAG_CMD_WHITELIST_CLEAR,      ID_DIAG_CMD_WHITELIST_CLEAR,      app_cmd_whitelist_clear },
        { ID_DIAG_CMD_WHITELIST_QUERY_NUM,  ID_DIAG_CMD_WHITELIST_QUERY_NUM,  app_cmd_whitelist_query_num },
        { ID_DIAG_CMD_WHITELIST_QUERY_INFO, ID_DIAG_CMD_WHITELIST_QUERY_INFO, app_cmd_whitelist_query_info },
        { ID_DIAG_CMD_WHITELIST_ADD,        ID_DIAG_CMD_WHITELIST_ADD,        app_cmd_whitelist_add },
        { ID_DIAG_CMD_WHITELIST_DELETE,     ID_DIAG_CMD_WHITELIST_DELETE,     app_cmd_whitelist_delete },
        { ID_DIAG_CMD_NI_DFX_INFO,          ID_DIAG_CMD_NI_DFX_INFO,          app_cmd_ni_get_dfx_info},
#if defined(PRODUCT_CFG_FAST_SEND_DEMO)
        { ID_DIAG_CMD_FAST_SEND_DEMO, ID_DIAG_CMD_FAST_SEND_DEMO, app_cmd_fast_send_demo },
#endif
#if defined(PRODUCT_CFG_PLC_MODULE_APP_TEST)
        { ID_DIAG_CMD_PLC_TEST,             ID_DIAG_CMD_PLC_TEST_STATUS,      app_cmd_plc_test }
#endif
    };

    td_u16 i;
    for (i = 0; i < ext_array_count(tbl); i++) {
        if ((tbl[i].min_id <= (td_u16)msg->param[0]) && (tbl[i].max_id >= (td_u16)msg->param[0])) {
            tbl[i].fn_input_cmd((td_u16)msg->param[0], (td_pvoid)msg->param[1],
                                (td_u16)msg->param[APP_MODULE_MSG_PARAM_INDEX2],
                                (td_u8)msg->param[APP_MODULE_MSG_PARAM_INDEX3]);
            break;
        }
    }

    if ((td_pvoid)msg->param[1] != TD_NULL) {
        uapi_free(EXT_MOD_ID_APP_COMMON, (td_pvoid)msg->param[1]);
    }
}

td_u32 app_cmd_whitelist_clear(td_u16 cmd_id, td_pvoid cmd, td_u16 cmd_size, td_u8 option)
{
    diag_cmd_whitelist_clear_req *req = (diag_cmd_whitelist_clear_req *)cmd;
    diag_cmd_whitelist_clear_ind ind = { 0 };

    if ((cmd == TD_NULL) || (cmd_size != sizeof(diag_cmd_whitelist_clear_req))) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    ind.ret_code = app_whitelist_clear();
    ind.seq = req->seq;

    return uapi_diag_report_packet(cmd_id, option, (td_pbyte)&ind, sizeof(ind), TD_TRUE);
}

td_u32 app_cmd_whitelist_query_num(td_u16 cmd_id, td_pvoid cmd, td_u16 cmd_size, td_u8 option)
{
    diag_cmd_whitelist_query_num_req *req = (diag_cmd_whitelist_query_num_req *)cmd;
    diag_cmd_whitelist_query_num_ind ind = { 0 };
    app_whitelist_head *white_list = TD_NULL;

    if ((cmd == TD_NULL) || (cmd_size != sizeof(diag_cmd_whitelist_query_num_req))) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    white_list = app_get_whitelist();

    ind.seq = req->seq;
    if (white_list != TD_NULL) {
        ind.total = white_list->total;
    } else {
        ind.total = 0;
    }

    return uapi_diag_report_packet(cmd_id, option, (td_pbyte)&ind, sizeof(ind), TD_TRUE);
}

td_u32 app_cmd_whitelist_query_info(td_u16 cmd_id, td_pvoid cmd, td_u16 cmd_size, td_u8 option)
{
    diag_cmd_whitelist_query_info_req *req = (diag_cmd_whitelist_query_info_req *)cmd;
    diag_cmd_whitelist_query_info_ind *ind = TD_NULL;
    app_whitelist_head *white_list = TD_NULL;
    td_u32 ret;

    if ((cmd == TD_NULL) || (cmd_size != sizeof(diag_cmd_whitelist_query_info_req))) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    ind = (diag_cmd_whitelist_query_info_ind *)uapi_malloc(EXT_MOD_ID_APP_COMMON,
                                                           sizeof(diag_cmd_whitelist_query_info_ind));
    if (ind == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }

    (td_void) memset_s(ind, sizeof(diag_cmd_whitelist_query_info_ind), 0, sizeof(diag_cmd_whitelist_query_info_ind));
    ind->start = req->start;
    ind->seq = req->seq;

    white_list = app_get_whitelist();
    if (white_list != TD_NULL) {
        ind->total = white_list->total;

        if (req->start >= ind->total) {
            ind->num = 0;
        } else {
            ind->num = uapi_min(req->num, ind->total - req->start);
            ind->num = uapi_min(ind->num, sizeof(ind->data) / WHITE_LIST_NODE_SIZE);
        }

        if (ind->num > 0) {
            if (memcpy_s(ind->data, sizeof(ind->data),
                         white_list->list + WHITE_LIST_NODE_SIZE * req->start,
                         WHITE_LIST_NODE_SIZE * ind->num) != EOK) {
                uapi_free(EXT_MOD_ID_APP_COMMON, ind);
                ind = TD_NULL;
                return EXT_ERR_MEMCPY_FAIL;
            }
        }
    } else {
        ind->total = 0;
    }

    ret = uapi_diag_report_packet(cmd_id, option, (td_pbyte)ind, sizeof(diag_cmd_whitelist_query_info_ind), TD_TRUE);

    uapi_free(EXT_MOD_ID_APP_COMMON, ind);
    ind = TD_NULL;

    return ret;
}

td_u32 app_cmd_whitelist_add(td_u16 cmd_id, td_pvoid cmd, td_u16 cmd_size, td_u8 option)
{
    diag_cmd_whitelist_add_req *req = (diag_cmd_whitelist_add_req *)cmd;
    diag_cmd_whitelist_add_ind ind = { 0 };

    if ((cmd == TD_NULL) || (cmd_size != sizeof(diag_cmd_whitelist_add_req))) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    if (req->num > (sizeof(req->data) / WHITE_LIST_NODE_SIZE)) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    ind.ret_code = app_whitelist_add(req->data, req->num);
    ind.seq = req->seq;

    return uapi_diag_report_packet(cmd_id, option, (td_pbyte)&ind, sizeof(ind), TD_TRUE);
}

/* whitelist: remove node */
td_u32 app_cmd_whitelist_delete(td_u16 cmd_id, td_pvoid cmd, td_u16 cmd_size, td_u8 option)
{
    diag_cmd_whitelist_delete_req *req = (diag_cmd_whitelist_delete_req *)cmd;
    diag_cmd_whitelist_delete_ind ind = { 0 };

    if ((cmd == TD_NULL) || (cmd_size != sizeof(diag_cmd_whitelist_delete_req))) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    if (req->num > (sizeof(req->data) / WHITE_LIST_NODE_SIZE)) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    ind.ret_code = app_whitelist_delete(req->data, req->num);
    ind.seq = req->seq;

    return uapi_diag_report_packet(cmd_id, option, (td_pbyte)&ind, sizeof(ind), TD_TRUE);
}

td_u32 app_cmd_ni_get_dfx_info(td_u16 cmd_id, td_pvoid cmd, td_u16 cmd_size, td_u8 option)
{
    diag_cmd_ni_dfx_info *ind = TD_NULL;

    ext_unref_param(cmd);
    ext_unref_param(cmd_size);

    app_ni_update_dfx_info();
    ind = app_ni_get_dfx_info();

    return uapi_diag_report_packet(cmd_id, option, (td_pbyte)ind, sizeof(diag_cmd_ni_dfx_info), TD_TRUE);
}
#if defined(PRODUCT_CFG_FAST_SEND_DEMO)
td_u32 app_cmd_fast_send_demo(td_u16 cmd_id, td_pvoid cmd, td_u16 cmd_size, td_u8 option)
{
    diag_cmd_fast_send_demo_req *req = (diag_cmd_fast_send_demo_req *)cmd;
    diag_cmd_fast_send_demo_ind ind = { 0 };

    if ((cmd == TD_NULL) || (cmd_size != sizeof(diag_cmd_fast_send_demo_req))) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    ind.ret_code = app_fast_send_demo_cmd(req);

    return uapi_diag_report_packet(cmd_id, option, (td_pbyte)&ind, sizeof(ind), TD_TRUE);
}
#endif
#ifdef __cplusplus
}
#endif
#endif

