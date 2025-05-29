/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: APP module protocol base API.
 */

#ifndef APP_PROTO_H
#define APP_PROTO_H

#include "app_main.h"

#ifdef __cplusplus
extern "C" {
#endif

#define APP_FRAME_HEAD_BYTE        0x48
#define PROTO_CTRL_DIR_DN          0x00
#define PROTO_CTRL_DIR_UP          0x80
#define PROTO_CTRL_PRM_HOST        0x40
#define PROTO_CTRL_PRM_SLAVE       0x00
#define PROTO_CTRL_RESPONSE        (PROTO_CTRL_DIR_UP | PROTO_CTRL_PRM_SLAVE)
#define PROTO_CTRL_REQ             (PROTO_CTRL_DIR_UP | PROTO_CTRL_PRM_HOST)
#define PROTO_CRC_SIZE             2
#define APP_PROTO_MSG_PARAM_INDEX0 0
#define APP_PROTO_MSG_PARAM_INDEX1 1
#define APP_PROTO_MSG_PARAM_INDEX2 2
#define APP_PROTO_MSG_PARAM_INDEX3 3

typedef struct {
    td_u8 head;
    td_u8 ctrl;
    td_u16 cmd;
    td_u16 seq;
    td_u16 length;
    td_u8 data[0];
} app_proto_frame_head;

td_void app_proto_parse(app_data_buf *buf_head);
td_bool check_app_proto_frame(td_pbyte data, td_u16 data_length);

#ifdef __cplusplus
}
#endif
#endif

