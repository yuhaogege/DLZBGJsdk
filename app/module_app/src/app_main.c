/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: APP main entry.
 */

#include "app_main.h"
#include <dw21_platform.h>
#include <soc_mid.h>
#include <soc_mdm_phy.h>
#include <soc_mdm_mac.h>
#include <soc_mdm_tl.h>
#include <app_nv.h>
#include "app_cco_whitelist.h"
#include "app_plc.h"
#include "app_uart.h"
#include "app_cmd_msg.h"
#include "dfx_app.h"
#include "app_proto_proc.h"
#include "app_proto_proc_cco.h"
#include "app_common.h"
#include "app_trans_file.h"
#include "app_diag_cmd.h"
#include "app_power_manager.h"
#include "app_fast_send_demo.h"
#include "app_network_identify.h"
#include "soc_mdm_ndm.h"
#include "demo_main.h"

#if defined(PRODUCT_CFG_PLC_MODULE_APP_TEST)
#include "app_plc_test.h"
#endif
#ifdef __cplusplus
extern "C" {
#endif

td_u32 app_srv_init(td_void);
td_u32 app_ctx_init(td_void);
td_u32 app_res_init(EXT_CONST app_srv_context *ctx);
td_u32 app_plc_chl_init(td_void);
td_void app_srv_task_body(td_u32 param);
td_void app_main_msg_proc(EXT_CONST ext_sys_queue_msg *msg);
td_void app_on_msg_timer(EXT_CONST ext_sys_queue_msg *msg);
td_void app_on_msg_frame_input(EXT_CONST ext_sys_queue_msg *msg);
td_void app_on_msg_plc_input(EXT_CONST ext_sys_queue_msg *msg);
td_void app_handle_mac_addr(td_void);
td_void app_register_mac_event(td_void);
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
td_void app_handle_mac_event_sta(td_u32 event, td_pvoid event_info);
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
td_void app_handle_mac_event_cco(td_u32 event, td_pvoid event_info);
#endif
td_void app_handle_mac_event(EXT_CONST ext_sys_queue_msg *msg);
td_u32 app_query_module_version_info(EXT_OUT td_pbyte ver_info, EXT_IN EXT_CONST td_u16 ver_info_size);

app_srv_context *g_app_srv_context = TD_NULL;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
EXT_CONST ext_diag_cmd_reg_obj g_app_diag_cmd[] = {
    { ID_DIAG_CMD_WHITELIST_CLEAR, ID_DIAG_CMD_APP_END, app_diag_cmd_proc },
};
#endif

__hot app_srv_context *app_get_srv_context(td_void)
{
    return g_app_srv_context;
}

#if defined(PRODUCT_CFG_DEBUG_CRASH_MODULE_APP)
__isr td_void led_flash_when_exc(td_void)
{
    uapi_watchdog_disable();
    uapi_io_set_func(EXT_GPIO_IDX_6, 0);
    uapi_io_set_dir(EXT_GPIO_IDX_6, EXT_GPIO_DIRECTION_OUT);
    uapi_io_set_ouput_val(EXT_GPIO_IDX_6, EXT_GPIO_VALUE0);

    while (1) {
        for (uintptr_t i = 0x02000000; i < 0x02038000; i += APP_LED_STEP) {
            if ((i & 0x3ff) == 0 && (i & 0x7ff) != 0) {
                uapi_io_set_ouput_val(EXT_GPIO_IDX_6, EXT_GPIO_VALUE1);
                uapi_led_on(EXT_LED_IDX_4);
                uapi_led_on(EXT_LED_IDX_1);
                uapi_led_on(EXT_LED_IDX_0);
            } else if ((i & 0x3ff) == 0) {
                uapi_led_off(EXT_LED_IDX_4);
                uapi_led_off(EXT_LED_IDX_1);
                uapi_led_off(EXT_LED_IDX_0);
                uapi_io_set_ouput_val(EXT_GPIO_IDX_6, EXT_GPIO_VALUE0);
            }
            printf(" 0x%08x: 0x%08x 0x%08x 0x%08x 0x%08x \n",
                   i, *(td_u32 *)i, *(td_u32 *)(i + APP_LED_STEP1),
                   *(td_u32 *)(i + APP_LED_STEP2), *(td_u32 *)(i + APP_LED_STEP3));
        }
    }
}

#endif

td_void app_main(td_void)
{
#if defined(PRODUCT_CFG_DEBUG_CRASH_MODULE_APP)
    uapi_syserr_exc_register((ext_syserr_exc_callback)led_flash_when_exc);
#endif
    (td_void) uapi_board_init();
    (td_void) uapi_aes_init();
    uapi_phy_init();
    uapi_mac_cp_init();
    uapi_load_1901_protocol();
    uapi_load_tl(APP_TL_QUEUE_SIZE);
#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
    uapi_load_p2p_component_p1901();
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    uapi_mdm_register_process_p2p_fc_function();
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA) || defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    uapi_mdm_mesh_rt_component();
#if defined(PRODUCT_CFG_SUPPORT_UPG_FROM_DBK) && defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    uapi_upg_sta_cmd_init();
#endif
#endif
    (td_void) uapi_mac_init();
    app_pm_init();
    (td_void) uapi_diag_init();
    (td_void) uapi_mid_init();
    /* set the list upgrade partition information, and use the user data area by default:[0x7B000,0x7C000). */
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    ext_flash_partition_table *partition = uapi_get_partition_table();
    if (partition->table[EXT_FLASH_PARTITON_USER_INFO_1].size_blk >= 1) {
        uapi_upg_list_set_cache_info(partition->table[EXT_FLASH_PARTITON_USER_INFO_1].addr_blk, 1);
    }
#endif
    uapi_upg_init();
    (td_void) app_srv_init();
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
#if defined(PRODUCT_CFG_FAST_SEND_DEMO)
    app_fast_send_demo_init();
#endif
#endif

    (td_void) uapi_diag_register_query_app_ver_cmd_callback((ext_mdm_query_app_ver)app_query_module_version_info);
}

td_u32 app_srv_init(td_void)
{
    td_u32 ret;
    app_srv_context *ctx = TD_NULL;

    ret = app_ctx_init();
    if (ret != EXT_ERR_SUCCESS) {
        app_printf("[-ERR-] app_ctx_init() failed.\n");
        return ret;
    }

    ctx = app_get_srv_context();
#if 0
    ret = app_uart_init();
    if (ret != EXT_ERR_SUCCESS) {
        app_printf("[-ERR-] app_uart_init() failed.\n");
        return ret;
    }
#endif
    ret = app_res_init(ctx);
    if (ret != EXT_ERR_SUCCESS) {
        app_printf("[-ERR-] app_res_init() failed.\n");
        return ret;
    }

    (td_void) app_plc_chl_init();
    app_register_mac_event();
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    ret = app_ni_init();
    if (ret != EXT_ERR_SUCCESS) {
        app_printf("[-ERR-] app_ni_init() failed.\n");
    }
#endif
    return ret;
}

td_u32 app_ctx_init(td_void)
{
    if (g_app_srv_context != TD_NULL) {
        return EXT_ERR_INITILIZATION_ALREADY;
    }

    g_app_srv_context = (app_srv_context *)uapi_malloc(EXT_MOD_ID_APP_COMMON, sizeof(app_srv_context));
    if (g_app_srv_context == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }

    (td_void) memset_s(g_app_srv_context, sizeof(app_srv_context), 0, sizeof(app_srv_context));

    return EXT_ERR_SUCCESS;
}

td_u32 app_res_init(EXT_CONST app_srv_context *ctx)
{
    td_u32 ret;
    td_u32 task_id = EXT_ERR_INVALID_ID;
    app_res_cfg *res = TD_NULL;
    td_s8 i;

    if (ctx == TD_NULL) {
        return EXT_ROM_ERR_FAILURE;
    }
    res = (app_res_cfg *)&ctx->res;
    ret = uapi_msg_queue_create(&res->queue_id, "appq", APP_QUEUE_SIZE, sizeof(ext_sys_queue_msg));
    if (ret != EXT_ERR_SUCCESS) {
        app_printf("[-ERR-] uapi_msg_queue_create() failed.\n");
        return EXT_ERR_FAILURE;
    }

    for (i = 0; i < APP_SEM_ID_MAX; i++) {
        ret = uapi_sem_bcreate(&res->sem_id[i], "SEM_APP", 1);
        if (ret != EXT_ERR_SUCCESS) {
            app_printf("[-ERR-] uapi_sem_bcreate() failed.\n");
            return EXT_ERR_FAILURE;
        }
    }

    ret = uapi_task_create(&task_id, "SRV_APP", app_srv_task_body, res->queue_id, APP_SRV_TASK_STACK_SIZE,
                         APP_SRV_TASK_PRIORITY);
    if (ret != EXT_ERR_SUCCESS) {
        app_printf("[-ERR-] uapi_task_create() SRV_APP failed.\n");
        return EXT_ERR_FAILURE;
    }

    ret = uapi_task_create(&task_id, "COM_APP", app_uart_task_body, (td_u32)(uintptr_t) & (ctx->buffer),
                         APP_UART_TASK_STACK_SIZE, APP_UART_TASK_PRIORITY);
    if (ret != EXT_ERR_SUCCESS) {
        app_printf("[-ERR-] uapi_task_create() UARTAPP failed. ret = %d\n", ret);
        return EXT_ERR_FAILURE;
    }
    printf("running demo application!\n");
    demo_app_main();
    return ret;
}

td_u32 app_plc_chl_init(td_void)
{
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    ext_dms_chl_port_obj mac_chl_obj = { app_plc_chl_status, app_plc_chl_rx, TD_NULL };
#else
    ext_dms_chl_port_obj mac_chl_obj = { TD_NULL, app_plc_chl_rx, TD_NULL };
#endif

    return uapi_dms_chl_setup(&mac_chl_obj, EXT_DMS_CHL_MAC_PORT_APP);
}

td_void app_srv_task_body(td_u32 param)
{
    td_u32 ret;
    ext_sys_queue_msg msg = { 0 };
    td_u32 queue_id = param;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    uapi_diag_register_cmd(g_app_diag_cmd, ext_array_count(g_app_diag_cmd));
    app_whitelist_setup();
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    /* Disable all reset events. STA doesn't enable any event when it starts up */
    for (td_u32 i = EXT_RST_CON_CCO_MAX_PROXY; i < EXT_RST_CON_MAX; i++) {
        uapi_sys_enable_reset((ext_rst_con)i, TD_FALSE);
    }
#endif

    app_handle_mac_addr();
    trans_file_init();
#if defined(PRODUCT_CFG_PLC_MODULE_APP_TEST) && \
    (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA))
    app_send_msg_setup((app_send_msg_func)app_send_message);
#endif
    for (;;) {
        ret = uapi_msg_queue_wait(queue_id, &msg, EXT_SYS_WAIT_FOREVER, sizeof(ext_sys_queue_msg));
        if (ret == EXT_ERR_SUCCESS) {
#if defined(PRODUCT_CFG_PLC_MODULE_APP_TEST)
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
            app_handle_plc_test_event(&msg);
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
            app_handle_plc_test_sta_event(&msg);
#endif
#endif
            app_main_msg_proc(&msg);
        } else {
            uapi_sleep(APP_MODULE_SLEEP_DURTION_MS);
        }
    }
}

td_void app_handle_mac_addr(td_void)
{
    app_dev_addr_inf nv;
    td_u32 ret;

    /* if communication address is abnormal, the default communication address is used */
    (td_void) memset_s(&nv, sizeof(nv), 0, sizeof(nv));
    ret = uapi_nv_read(ID_NV_APP_DEV_ADDR_INF, &nv, sizeof(nv));
    if ((ret != EXT_ERR_SUCCESS) || !is_valid_mac_addr(nv.dev_addr)) {
        td_u8 mac[EXT_PLC_MAC_ADDR_LEN] = { 0 };

        (td_void) uapi_get_local_mac_addr(mac, sizeof(mac));
        if (is_valid_mac_addr(mac)) {
            (td_void) memcpy_s(nv.dev_addr, sizeof(nv.dev_addr), mac, sizeof(mac));
        }

        (td_void) uapi_nv_write(ID_NV_APP_DEV_ADDR_INF, &nv, sizeof(nv));
    }

    (td_void) app_set_mac_addr(nv.dev_addr, sizeof(nv.dev_addr));
}

td_void app_main_msg_proc(EXT_CONST ext_sys_queue_msg *msg)
{
    if (msg == TD_NULL) {
        return;
    }
    switch (msg->msg_id) {
        case ID_APP_MSG_TIMER:
            app_on_msg_timer(msg);
            break;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
        case ID_APP_MSG_DIAG:
            app_on_msg_diag(msg);
            break;

        case ID_APP_MSG_PLC_CHL:
            app_plc_chl_status_handle(msg);
            break;
#endif

        case ID_APP_MSG_FRAME_INPUT:
            app_on_msg_frame_input(msg);
            break;

        case ID_APP_MSG_PLC_INPUT:
            app_on_msg_plc_input(msg);
            break;

        case ID_APP_MSG_TRANS_FILE:
            app_on_message_trans_file(msg);
            break;

        case ID_APP_MSG_PLC_MAC_EVENT:
            app_handle_mac_event(msg);
            break;

#if defined(PRODUCT_CFG_FAST_SEND_DEMO)
        case ID_APP_MSG_FAST_SEND_EVENT:
            app_handle_fast_send_event(msg);
            break;
#endif

        default:
            break;
    }
}
td_bool app_handle_ni_timer(td_u32 timer_id)
{
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    if (timer_id >= APP_TIMER_ID_NI_WAIT_STA_RSP && timer_id <= APP_TIMER_ID_NI_WAIT_STA_RSP5) {
        app_ni_wait_sta_rsp_timeout(timer_id);
        return TD_TRUE;
    } else {
        return TD_FALSE;
    }
#else
    ext_unref_param(timer_id);
    return TD_FALSE;
#endif
}
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
td_void app_cco_handle_whitelist_timer(td_u32 timer_id)
{
    switch (timer_id) {
        case APP_TIMER_ID_WHITELIST:
            app_whitelist_timeout_notify_mac();
            break;
#if !defined(PRODUCT_CFG_SDK_WHTIE_LIST_SUPPORT)
        case APP_TIMER_ID_WRITE_WHITELIST_TO_FLASH:
            app_whitelist_timeout();
            break;
#endif
        default:
            break;
    }
}
#endif
td_void app_on_msg_timer(EXT_CONST ext_sys_queue_msg *msg)
{
    if (msg == TD_NULL) {
        return;
    }

    if (app_handle_ni_timer(msg->param[0]) == TD_TRUE) {
        return;
    }
    switch (msg->param[0]) {
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
        case APP_TIMER_ID_WHITELIST:
        case APP_TIMER_ID_WRITE_WHITELIST_TO_FLASH:
            app_cco_handle_whitelist_timer(msg->param[0]);
            break;
        case APP_TIMER_ID_TRANS_FILE_QRY_ACTIVE:
            trans_file_on_timer_query_active();
            break;

        case APP_TIMER_ID_WAIT_PLC_AVAILABLE:
            trans_file_on_timer_plc_available();
            break;
        case APP_TIMER_ID_NI_WAIT_NEW_STA_JOIN:
            app_ni_handle_wait_sta_join_timeout();
            break;
#endif

        case APP_TIMER_ID_REBOOT:
            uapi_usr_reboot(EXT_SYS_REBOOT_CAUSE_USR0);
            break;

        case APP_TIMER_ID_TRANS_FILE_RX:
            trans_file_on_timer_frame();
            break;

        case APP_TIMER_ID_TRANS_FILE_STOP_RETRY:
            trans_file_on_timer_stop_retry();
            break;

        case APP_TIMER_ID_TRANS_FILE_STATUS_QRY:
            trans_file_on_timer_status_query();
            break;

        case APP_TIMER_ID_TRANS_FILE_QRY_INFO:
            trans_file_on_timer_query_info();
            break;

        default:
            break;
    }
}

td_void app_on_msg_frame_input(EXT_CONST ext_sys_queue_msg *msg)
{
    td_pbyte frame;

    if (msg == TD_NULL) {
        return;
    }
    frame = (td_pbyte)msg->param[0];

    (td_void) app_proto_frame_rx(frame, (td_u16)msg->param[1], APP_PROTO_SOURCE_UART, TD_NULL);

    uapi_free(EXT_MOD_ID_APP_COMMON, frame);
    frame = TD_NULL;
}

td_void app_on_msg_plc_input(EXT_CONST ext_sys_queue_msg *msg)
{
    app_plc_frame_data *info = TD_NULL;
    if (msg == TD_NULL) {
        return;
    }

    info = (app_plc_frame_data *)msg->param;
    demo_plc_recv((td_s8*)info->payload, info->payload_length);
    /*
    if (info->id == ID_PLC_REMOTE_CMD) {
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
        (td_void) app_proto_frame_rx(info->payload, info->payload_length, APP_PROTO_SOURCE_PLC, info->mac);
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
        (td_void) app_proto_remote_cmd(info->payload, info->payload_length, info->mac);
#endif
    } else {
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
        (td_void) app_proto_data_transmit(info->payload, info->payload_length, info->mac);
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
        (td_void) app_proto_data_transmit(info->payload, info->payload_length, info->mac);
#endif
    }
    */
    if (info->payload != TD_NULL) {
        uapi_free(EXT_MOD_ID_APP_COMMON, info->payload);
        info->payload = TD_NULL;
    }
}

td_void app_register_mac_event(td_void)
{
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    uapi_register_mac_event_call_back(0xFFFF, app_handle_mac_event_sta);
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    uapi_register_mac_event_call_back(0xFFFF, app_handle_mac_event_cco);
#endif
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
td_void app_handle_mac_event_sta(td_u32 event, td_pvoid event_info)
{
    (td_void) event_info;
    ext_sys_queue_msg msg;

    (td_void) memset_s(&msg, sizeof(msg), 0, sizeof(msg));
    msg.msg_id = ID_APP_MSG_PLC_MAC_EVENT;
    msg.param[0] = event;

    app_send_message(&msg);
    if(event & MAC_STA_ACCESS_NETWORK_SUCCESS){
       printf("----- sta is connect plc networks -----\n");
    }
    else if(event & MAC_STA_OFFLINE){
        printf("----- sta is disconnect plc networks -----\n");
    }
}
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
td_void app_handle_mac_event_cco(td_u32 event, td_pvoid event_info)
{
    ext_sys_queue_msg mgs_info;
    ext_mac_station_join_info *new_sta = TD_NULL;

    if (event & MAC_STA_ACCESS_NETWORK_SUCCESS) {
        return;
    }
    (td_void) memset_s(&mgs_info, sizeof(mgs_info), 0, sizeof(mgs_info));
    mgs_info.msg_id = ID_APP_MSG_PLC_MAC_EVENT;
    mgs_info.param[0] = event;
    if (event & (MAC_ROUTE_OPTIMAL_COMPLETION | MAC_STATION_JOIN | MAC_NETWORK_FORMED | MAC_FRIST_NETWORK_FORMED)) {
        if ((event & MAC_STATION_JOIN) && (event_info != TD_NULL)) {
            new_sta = (ext_mac_station_join_info *)event_info;
            mgs_info.param[2] = new_sta->tei; /* 2 record TEI */
        }
        app_send_message(&mgs_info);
    }
}
#endif

td_void app_handle_mac_event(EXT_CONST ext_sys_queue_msg *msg)
{
    if (msg == TD_NULL) {
        return;
    }
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    if (msg->param[0] & MAC_STA_ACCESS_NETWORK_SUCCESS) {
        /* Enable no join network reset event when STA joins the network */
        uapi_sys_enable_reset(EXT_RST_CON_STA_NO_JOIN_NETWORK, TD_TRUE);
    }
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    if (msg->param[0] & MAC_FRIST_NETWORK_FORMED) {
        app_ni_cco_formed_notify();
    } else if (msg->param[0] & MAC_STATION_JOIN) {
        app_ni_new_sta_join_notify(msg->param[2]); /* 2 is index used in network identify */
        app_ni_restart_querying_sta_poweron_time();
    } else if (msg->param[0] & MAC_NETWORK_FORMED) {
        app_ni_restart_querying_sta_poweron_time();
    }
#endif
}

td_u32 app_query_module_version_info(EXT_OUT td_pbyte ver_info, EXT_IN EXT_CONST td_u16 ver_info_size)
{
    td_char app_name[] = {"MODULE_APP"};
    ext_app_ver_cmd_ind_info *cur_ver_info = TD_NULL;

    if (ver_info == TD_NULL || ver_info_size != sizeof(ext_app_ver_cmd_ind_info)) {
        return EXT_ERR_FAILURE;
    }

    cur_ver_info = (ext_app_ver_cmd_ind_info *)ver_info;
    if (strcpy_s((td_char *)cur_ver_info->app_name, sizeof(app_name), app_name) != EOK) {
        return EXT_ERR_FAILURE;
    }

    return EXT_ERR_SUCCESS;
}

#ifdef __cplusplus
}
#endif

