/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Middle layer - command / message.
 * Author: CompanyName
 * Create: 2018-10-31
 */

#ifndef __MID_CMD_MSG_H__
#define __MID_CMD_MSG_H__

#ifdef __cplusplus
extern "C" {
#endif

#define MID_SESSION_HEAD_LEN 16

#define EXT_NV_BW 0x3510
#define EXT_NV_BR 0x3511

#define ID_CMD_NV_READ_PLC 0x0526
#define ID_CMD_NV_WRITE_PLC 0x0527

typedef struct {
    td_u16 nv_id;
    td_u16 crc;
    td_u8 mac[6]; /* mac address, size 6. */
    td_u8 times;
    td_u8 rsv[7]; /* reserved 7 bytes for compatibility. */
    td_u16 payload_len;
    td_u8 payload[0];
} ext_nv_bw_req;

typedef struct {
    td_u32 rc;
    td_u16 nv_id;
    td_u8 mac[6]; /* mac address, size 6. */
    td_u8 rsv[4]; /* reserved 4 bytes for compatibility. */
} ext_nv_bw_ind;

typedef struct {
    td_u16 nv_id;
    td_u8 mac[6]; /* mac address, size 6. */
    td_u8 times;
    td_u8 rsv[7]; /* reserved 7 bytes for compatibility. */
} ext_nv_br_req;

typedef struct {
    td_u32 rc;
    td_u16 crc;
    td_u16 nv_id;
    td_u8 mac[6]; /* mac address, size 6. */
    td_u8 rsv[4]; /* reserved 4 bytes for compatibility. */
    td_u16 payload_len;
    td_u8 payload[0];
} ext_nv_br_ind;

typedef struct {
    td_u16 plc_id;
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN];
    td_u16 pad;
    td_u16 payload_len;
    td_u8 *payload;
} mid_plc_frame;

td_u32 mid_chl_rx(td_u16 port, const ext_dms_chl_rx *rx);

#ifdef __cplusplus
}
#endif
#endif /* __MID_CMD_MSG_H__ */
