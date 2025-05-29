/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: APP module protocol frame handle (CCO special).
 */

#ifndef APP_PROTO_PROC_CCO_H
#define APP_PROTO_PROC_CCO_H

#include "app_proto.h"

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
#ifdef __cplusplus
extern "C" {
#endif

#define APP_PROTO_SIZE1      6
#define APP_PROTO_MIN_LENGTH 2
#define APP_PROTO_SIZE2      6

td_u32 app_proto_get_whitelist_num(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length);
td_u32 app_proto_get_whitelist_info(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length);
td_u32 app_proto_add_whitelist_node(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length);
td_u32 app_proto_del_whitelist_node(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length);
td_u32 app_proto_clear_whitelist(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length);
td_u32 app_proto_get_topo_num(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length);
td_u32 app_proto_get_topo_info(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length);
td_u32 app_proto_remote_cmd_tx(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length);
td_u32 app_proto_remote_cmd_rx(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length);
td_u32 app_proto_remote_cmd(td_pbyte data, td_u16 data_length, td_pbyte src_addr);
td_u32 app_proto_set_network_identify_state(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer,
    td_u16 *length);
td_u32 app_proto_get_whitelist_state(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length);
td_u32 app_proto_set_whitelist_state(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length);
td_u32 app_ni_set_self_identify_params(td_bool state, td_u8 wait_time, td_u16 time_diff);
#ifdef __cplusplus
}
#endif
#endif
#endif

