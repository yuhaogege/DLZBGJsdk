/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: Diag command handle.
 */

#ifndef APP_DIAG_CMD_H
#define APP_DIAG_CMD_H

#include "soc_types.h"
#include "soc_mdm_msg.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
#define APP_MODULE_MSG_PARAM_INDEX0 0
#define APP_MODULE_MSG_PARAM_INDEX1 1
#define APP_MODULE_MSG_PARAM_INDEX2 2
#define APP_MODULE_MSG_PARAM_INDEX3 3

td_u32 app_diag_cmd_proc(td_u16 cmd_id, td_pvoid cmd, td_u16 cmd_size, td_u8 option);
td_void app_on_msg_diag(EXT_CONST ext_sys_queue_msg *msg);
#endif

#ifdef __cplusplus
}
#endif
#endif

