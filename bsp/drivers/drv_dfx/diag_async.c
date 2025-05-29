/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: add data collect
 * Create: 2012-09-10
 */
#include <soc_types.h>
#include <soc_mdm_types.h>
#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
#include <sal_inf.h>
#include <soc_diag_async.h>
#include <dfx_sys_sdm.h>

#define TASK_STACK_SIZE_DIAG_ASYNC 1024
#define DIAG_ASYNC_TASK_PRIO 27

typedef struct {
    td_u16 id;
    td_u16 cmd_param_size;
    td_u32 option;
    uapi_diag_async_cmd_f func;
    td_u8 data[0];
} ext_diag_async_msg_s;

td_u32 g_diag_queue_id;
td_u32 g_diag_async_task_id;

ext_diag_sync_stat_s g_diag_sysnc_stat;
td_void sal_diag_async_task_body(td_u32 param);

td_u32 uapi_diag_async_cmd(td_u16 id, EXT_IN td_pvoid cmd_param, td_u16 cmd_param_size, td_u32 option,
    uapi_diag_async_cmd_f func)
{
    size_t msg_data_size = sizeof(ext_diag_async_msg_s) + cmd_param_size;
    ext_diag_async_msg_s *msg_data = TD_NULL;
    ext_sys_queue_msg msg = { 0 };
    if (func == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    g_diag_sysnc_stat.call_total_times++;
    msg_data = (ext_diag_async_msg_s *)uapi_malloc(EXT_MOD_ID_SAL_DIAG, msg_data_size);
    if (msg_data == TD_NULL) {
        g_diag_sysnc_stat.call_fail_times++;
        g_diag_sysnc_stat.last_fail_reason = EXT_ERR_FAILURE;
        return EXT_ERR_FAILURE;
    }

    // copy data
    msg_data->id = id;
    msg_data->cmd_param_size = cmd_param_size;
    msg_data->option = option;
    msg_data->func = func;
    if (cmd_param_size > 0) {
        if (memcpy_s(msg_data->data, cmd_param_size, cmd_param, cmd_param_size) != EOK) {
            uapi_free(EXT_MOD_ID_SAL_DIAG, msg_data);
            return EXT_ERR_MEMCPY_FAIL;
        }
    }

    msg.msg_id = SAL_QUEUE_ASYNC_DIAG_MSG_CMD;
    msg.param[0] = (uintptr_t)msg_data;

    if (EXT_ERR_SUCCESS != uapi_msg_queue_send(g_diag_queue_id, &msg,
        EXT_SYS_WAIT_FOREVER, sizeof(ext_sys_queue_msg))) {
        uapi_free(EXT_MOD_ID_SAL_DIAG, msg_data);
        g_diag_sysnc_stat.call_fail_times++;
        g_diag_sysnc_stat.last_fail_reason = EXT_ERR_QUEUE_FULL;
        return EXT_ERR_QUEUE_FULL;
    }

    return EXT_ERR_SUCCESS;
}

EXT_PRV td_u32 uapi_diag_async_cmd_proc(ext_sys_queue_msg *msg)
{
    ext_diag_async_msg_s *msg_data = (ext_diag_async_msg_s *)msg->param[0];
    msg_data->func(msg_data->id, msg_data->data, msg_data->cmd_param_size, msg_data->option);
    uapi_free(EXT_MOD_ID_SAL_DIAG, msg_data);
    return EXT_ERR_SUCCESS;
}
EXT_PRV td_u32 sal_diag_init_create_os_res(td_void)
{
    td_u32 ret =
        uapi_msg_queue_create(&g_diag_queue_id, "DIAG_ASYNC_CMD_PROC_Q", DIAG_QUEUE_MSG_NUM, sizeof(ext_sys_queue_msg));
    if (ret != EXT_ERR_SUCCESS) {
        g_diag_sysnc_stat.call_fail_times++;
        g_diag_sysnc_stat.last_fail_reason = EXT_ERR_QUEUE_FULL;
        return EXT_ERR_QUEUE_FULL;
    }
    ret = uapi_task_create(&g_diag_async_task_id, "DIAG_ASYNC_TSK", sal_diag_async_task_body, 0,
        TASK_STACK_SIZE_DIAG_ASYNC, DIAG_ASYNC_TASK_PRIO);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }
    return EXT_ERR_SUCCESS;
}

EXT_EAPI td_void sal_diag_async_task_init(td_void)
{
    td_u32 ret = sal_diag_init_create_os_res();
    if (ret != EXT_ERR_SUCCESS) {
        return;
    }
    return;
}
td_void sal_diag_async_task_body(td_u32 param)
{
    ext_unref_param(param);
    ext_sys_queue_msg msg = { 0 };
    td_u32 ret;
    printf("[INFO][sal_diag_async_task_body] in\r\n");
    for (;;) {
        ret = uapi_msg_queue_wait(g_diag_queue_id, &msg, EXT_SYS_WAIT_FOREVER, sizeof(ext_sys_queue_msg));
        if (ret == EXT_ERR_SUCCESS) {
            switch (msg.msg_id) {
                case SAL_QUEUE_ASYNC_DIAG_MSG_CMD:
                    uapi_diag_async_cmd_proc(&msg);
                    break;
                default:
                    break;
            }
        } else {
            uapi_sleep(1000); // wait 1000 ms
        }
    }
}
#endif
