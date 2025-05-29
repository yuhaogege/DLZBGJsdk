/*
 * Copyright (c) CompanyNameMagicTag 2019-2020. All rights reserved.
 * Description: Fast send function.
 */

#ifndef APP_FAST_SEND_DEMO_H
#define APP_FAST_SEND_DEMO_H

#include "soc_mdm_msg.h"
#include "dfx_app.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(PRODUCT_CFG_FAST_SEND_DEMO)

#define APP_FAST_SEND_FC_DEMO_FRAME_SIZE      2
#define APP_FAST_SEND_PB_DEMO_FRAME_SIZE      4
#define APP_FAST_SEND_PERIOD_MS               1000
#define APP_FAST_SEND_DISABLE_SEND_DURTION_MS 2000
#define APP_FAST_SEND_FC_MODE                 0
#define APP_FAST_SEND_PB_MODE                 1
#define APP_FAST_SEND_RESEND_CNT              3
#define APP_FAST_SEND_PB_DATA_LEN             10
#define APP_FAST_SEND_FC_DATA_LEN             6
#define APP_FAST_SEND_PB_PORT_ID              0x01
#define APP_FAST_SEND_PAD_LEN                 2
#define APP_FAST_DEMO_INDEX0                  0
#define APP_FAST_DEMO_INDEX1                  1
#define APP_FAST_DEMO_INDEX2                  2
#define APP_FAST_DEMO_INDEX3                  3
#define APP_FAST_DEMO_INDEX4                  4
#define APP_FAST_DEMO_INDEX5                  5
#define APP_FAST_DEMO_INDEX6                  6
#define APP_FAST_DEMO_INDEX7                  7
#define APP_FAST_DEMO_INDEX8                  8
#define APP_FAST_DEMO_INDEX9                  9
#define APP_FAST_DEMO_DATA0                   0x11
#define APP_FAST_DEMO_DATA1                   0x22
#define APP_FAST_DEMO_DATA2                   0x33
#define APP_FAST_DEMO_DATA3                   0x44
#define APP_FAST_DEMO_DATA4                   0x55
#define APP_FAST_DEMO_DATA5                   0x66
#define APP_FAST_DEMO_DATA6                   0x77
#define APP_FAST_DEMO_DATA7                   0x88
#define APP_FAST_DEMO_DATA8                   0x99
#define APP_FAST_DEMO_DATA9                   0xaa

typedef struct {
    td_u32 id : 8;
    td_u32 data_index : 8;
    td_u32 payload_len : 16;
    td_u8 data[0];
} app_fast_send_pb_demo_frame_st;

typedef struct {
    td_u8 data_index;
    td_u8 payload_len;
    td_u8 pad[APP_FAST_SEND_PAD_LEN];
} app_fast_send_fc_demo_frame_st;

typedef struct {
    td_u32 send_period;
    td_u32 period_send_cnt;
    td_u32 period_send_index;

    td_u8 send_index;
    td_u8 send_mode;
    td_u8 send_broadcast_flag;
    td_u8 send_resend_cnt;

    td_u8 rcv_send_index;
    td_u8 send_plc_mode;
    td_u8 send_phase;
    td_u8 pad;

    td_u8 oda[EXT_PLC_MAC_ADDR_LEN];
    td_u8 pad1[APP_FAST_SEND_PAD_LEN];

    ext_dms_chl_rx_f rx_func;
    ext_timer_handle_s fast_send_timer;
} app_fast_send_demo_ctrl_st;

td_u32 app_fast_send_demo_cmd(diag_cmd_fast_send_demo_req *req_cmd);
td_u32 app_fast_send_demo_start(diag_cmd_fast_send_demo_req *req_cmd, td_bool is_retranf);
td_u32 app_fast_send_demo_stop(td_void);
td_void uapi_timer_define(app_fast_send_call_back);
td_u32 app_fast_send_pb_process(td_bool is_retranf);
td_u32 app_rcv_fast_send_plc_chl_demo(td_u16 port, EXT_CONST ext_dms_chl_rx *chl_rx);
td_u32 app_fast_send_plc_chl_demo_init(td_void);
td_u32 app_fast_send_fc_process(td_bool is_retranf);
td_void app_fast_send_fc_rcv_data(ext_mac_report_rcv_fast_send_pkt *rcv_data);
td_u32 app_fast_send_fc_rcv_init(td_void);
td_u32 app_rcv_fast_send_plc_chl_demo_interface(td_u16 port, EXT_CONST ext_dms_chl_rx *chl_rx);
td_void app_handle_fast_send_event(EXT_CONST ext_sys_queue_msg *msg);
td_void app_fast_send_demo_init(td_void);

#endif

#ifdef __cplusplus
}
#endif

#endif

