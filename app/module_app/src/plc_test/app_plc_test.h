/*
 * Copyright (c) CompanyNameMagicTag 2019-2020. All rights reserved.
 * Description: plc packet test of application
 */

#ifndef APP_PLC_TEST_H
#define APP_PLC_TEST_H
#include "soc_mdm_msg.h"
#include "dfx_app.h"

#if defined(PRODUCT_CFG_PLC_MODULE_APP_TEST)

#define APP_PLC_TEST_PAD_LEN               2
#define APP_PLC_TEST_FRAME_LEN             452
#define APP_PLC_TEST_SEND_TOTAL_NUM        10
#define APP_PLC_TEST_NTB_CONVERT_TO_MS     25000
#define APP_PLC_TEST_SEND_PRIO             3
#define APP_PLC_TEST_UDP_STARTSTOP_TIME_MS 3 * 1000
#define APP_PLC_TEST_UDP_SEARCH_TIME_MS    2 * 1000

#define APP_PLC_TEST_QUEUE_SIZE            16
#define APP_PLC_TEST_SLEEP_DURTION_MS      1000
#define APP_PLC_TEST_SEARCH_INVERVAL_MS    10

td_u32 app_plc_test_chl_rx(td_u16 port, ext_dms_chl_rx *chl_rx);
typedef td_u32 (*app_send_msg_func)(ext_sys_queue_msg *msg);
td_void app_send_msg_setup(app_send_msg_func function);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
td_void app_handle_plc_test_event(ext_sys_queue_msg *msg);
td_u32 app_cmd_plc_test(td_u16 cmd_id, td_pvoid cmd, td_u16 cmd_size, td_u8 option);
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
td_void app_handle_plc_test_sta_event(ext_sys_queue_msg *msg);
#endif

#endif
#endif
