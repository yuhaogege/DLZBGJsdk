/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: Command/Message Define.
 */

#ifndef APP_CMD_MSG_H
#define APP_CMD_MSG_H
#include "soc_types.h"

/* PLC data transmit */
#define ID_PLC_DATA_TRANSMIT 0x50

/* PLC remote cmd */
#define ID_PLC_REMOTE_CMD 0x51

/* Message Definition */
typedef enum {
    ID_APP_MSG_TIMER = 1,
    ID_APP_MSG_DIAG,
    ID_APP_MSG_FRAME_INPUT,
    ID_APP_MSG_PLC_INPUT,
    ID_APP_MSG_PLC_CHL,
    ID_APP_MSG_TRANS_FILE,
    ID_APP_MSG_PLC_MAC_EVENT,
    ID_APP_MSG_FAST_SEND_EVENT,
} id_app_msg;

/* PLC frame data structure */
typedef struct {
    td_u16 crc_16;      /* CRC16 check: CRC16-CCITT */
    td_u16 data_length; /* Data field length */
    td_u8 data[0];
} app_plc_trans_frame;

#endif
