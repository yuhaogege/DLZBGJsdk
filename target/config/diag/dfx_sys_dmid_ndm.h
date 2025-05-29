/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: dfx sys dmid ndm.
 * Author: CompanyName
 * Create: 2021-1-27
 */
#ifndef __DFX_SYS_DMID_NDM_H__
#define __DFX_SYS_DMID_NDM_H__

#include "soc_config.h"

/* Reader module message definition, range [0x810~0x830) */
#define EXT_DMID_NDM_BASE (0x810)
#define EXT_DMID_NDM_CIU_REFRESH_STATE (EXT_DMID_NDM_BASE + 0x01)
#define EXT_DMID_NDM_CIU_UPDATE_WORK_FREQ (EXT_DMID_NDM_BASE + 0x02)
#define EXT_DMID_NDM_STA_REFRESH_P2P_STATE (EXT_DMID_NDM_BASE + 0x03)
#define EXT_DMID_NDM_P2P_SEND_PACKET (EXT_DMID_NDM_BASE + 0x04)
#define EXT_DMID_NDM_SEND_SEARCH_REQ (EXT_DMID_NDM_BASE + 0x05)
#define EXT_DMID_NDM_SEND_SEARCH_RSP (EXT_DMID_NDM_BASE + 0x06)
#define EXT_DMID_NDM_RCV_SEARCH_REQ (EXT_DMID_NDM_BASE + 0x07)
#define EXT_DMID_NDM_RCV_SEARCH_RSP (EXT_DMID_NDM_BASE + 0x08)
#define EXT_DMID_NDM_P2P_RCV_PACKET (EXT_DMID_NDM_BASE + 0x09)

typedef struct {
    td_u8 cur_state;
    td_u8 cur_freq;
    td_u8 oda[EXT_PLC_MAC_ADDR_LEN];
} ext_dmid_ndm_ciu_refresh_state;

typedef struct {
    td_u8 previous_freq;
    td_u8 current_freq;
    td_u8 round_count;
    td_u8 pad;
} ext_dmid_ndm_ciu_update_work_freq;

typedef struct {
    td_u8 cur_freq;
    td_u8 p2p_state;
    td_u8 connect_mode;
    td_u8 pad;
} ext_dmid_ndm_sta_refresh_p2p_state;

typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN];
    td_u8 pad[2]; /* pad 2 byte */
} ext_dmid_ndm_p2p_packet_msg;

typedef struct {
    td_u8 cur_freq;
    td_u8 connect_mode;
    td_u8 oda[EXT_PLC_MAC_ADDR_LEN];
} ext_dmid_ndm_ciu_send_search_req;

typedef struct {
    td_u8 cur_freq;
    td_u8 allow_to_link;
    td_u8 oda[EXT_PLC_MAC_ADDR_LEN];
} ext_dmid_ndm_sta_send_search_rsp;

typedef struct {
    td_u8 connect_mode;
    td_u8 is_need_ack;
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN];
} ext_dmid_ndm_rcv_search_req;

typedef struct {
    td_u8 freq;
    td_u8 allow_link;
    td_u16 tei;
    td_u32 snid;
} ext_dmid_ndm_rcv_search_rsp;


#endif /* __DFX_SYS_DMID_NDM_H__ */

