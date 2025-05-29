/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: APP common API.
 */

#include <soc_mdm_mac.h>
#include "app_cmd_msg.h"
#include "app_main.h"
#include "soc_stdlib.h"
#include "soc_mdm_mem.h"
#include "app_common.h"
#ifdef __cplusplus
extern "C" {
#endif

td_void app_timer_callback(td_u32 param);

td_pbyte find_byte_in_stream(td_pbyte data, td_u16 data_length, td_u8 ch)
{
    td_u16 i;

    if (data == TD_NULL) {
        return TD_NULL;
    }

    for (i = 0; i < data_length; i++) {
        if (data[i] == ch) {
            return &data[i];
        }
    }

    return TD_NULL;
}

__isr td_void app_timer_callback(td_u32 param)
{
    if (param < APP_TIMER_ID_MAX) {
        ext_sys_queue_msg msg;

        (td_void) memset_s(&msg, sizeof(msg), 0, sizeof(msg));
        msg.msg_id = ID_APP_MSG_TIMER;
        msg.param[0] = param;

        app_send_message(&msg);
    }
}

td_u32 app_timer_start(app_timer_id id, td_u32 period, ext_timer_type_e option)
{
    app_srv_context *ctx = app_get_srv_context();
    app_res_cfg *res = &ctx->res;

    if (id >= APP_TIMER_ID_MAX) {
        return EXT_ERR_INVALID_ID;
    }

    return uapi_timer_start(&res->timer_handle[id], app_timer_callback, period, option, id);
}

td_u32 app_timer_stop(app_timer_id id)
{
    app_srv_context *ctx = app_get_srv_context();
    app_res_cfg *res = &ctx->res;

    if (id >= APP_TIMER_ID_MAX) {
        return EXT_ERR_INVALID_ID;
    }

    return uapi_timer_stop(&res->timer_handle[id]);
}

__hot td_u32 app_send_message(EXT_CONST ext_sys_queue_msg *msg)
{
    app_srv_context *ctx = app_get_srv_context();
    app_res_cfg *res = &ctx->res;

    return uapi_msg_queue_send(res->queue_id, (td_pvoid)msg, 0, sizeof(ext_sys_queue_msg));
}

td_u32 app_set_mac_addr(EXT_CONST td_u8 mac[EXT_PLC_MAC_ADDR_LEN], td_u32 mac_size)
{
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    ext_unref_param(mac_size);
    return uapi_set_cco_mac_addr(mac);
#else
    td_u32 ret;
    td_u16 length = sizeof(ext_mac_attr) + sizeof(ext_mac_entry);
    ext_mac_attr *mac_attr = (ext_mac_attr *)uapi_malloc(EXT_MOD_ID_APP_COMMON, length);

    if (mac_attr == TD_NULL || mac_size != EXT_PLC_MAC_ADDR_LEN || mac == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }

    (td_void) memset_s(mac_attr, length, 0, length);
    mac_attr->list_num = 1;
    mac_attr->clear_flag = TD_TRUE;
    mac_attr->type = EXT_MDM_APP_MAC;
    if (memcpy_s(mac_attr->mac_list[0].mac, sizeof(mac_attr->mac_list[0].mac), mac, EXT_PLC_MAC_ADDR_LEN) != EOK) {
        uapi_free(EXT_MOD_ID_APP_COMMON, mac_attr);
        mac_attr = TD_NULL;
    }

    ret = uapi_set_backup_mac_addr(mac_attr);

    uapi_free(EXT_MOD_ID_APP_COMMON, mac_attr);
    mac_attr = TD_NULL;

    return ret;
#endif
}

#ifdef __cplusplus
}
#endif

