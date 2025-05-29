/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: APP PLC frame handle.
 */

#ifndef APP_PLC_H
#define APP_PLC_H

#include "soc_mdm_msg.h"
#include "soc_types.h"
#include "soc_mdm_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define APP_PLC_MODULE_PRIO 3

typedef struct {
    td_u16 id;                      /* PLC packet ID */
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN]; /* destination address */
    td_u16 option;                  /* Optional field */
    td_u16 payload_length;          /* Data frame length */
    td_pbyte payload;               /* Data frame payload */
} app_plc_frame_data;

td_u32 app_send_plc_frame(EXT_CONST app_plc_frame_data *frame);
td_u32 app_plc_chl_rx(td_u16 port, EXT_CONST ext_dms_chl_rx *chl_rx);
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
td_u32 app_plc_chl_status(td_u16 port, td_u8 status);
td_u32 app_plc_chl_status_handle(EXT_CONST ext_sys_queue_msg *msg);
td_bool app_is_plc_available(td_void);
#endif

#ifdef __cplusplus
}
#endif

#endif
