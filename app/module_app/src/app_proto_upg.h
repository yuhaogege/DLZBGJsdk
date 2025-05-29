/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: APP module protocol frame handle (UPG special).
 */

#ifndef APP_PROTO_UPG_H
#define APP_PROTO_UPG_H

#include <soc_types.h>
#include "app_proto.h"

#ifdef __cplusplus
extern "C" {
#endif

td_u32 app_proto_trans_file(EXT_CONST app_proto_frame_head *frame, td_pbyte *out_buffer, td_u16 *length);

#ifdef __cplusplus
}
#endif
#endif

