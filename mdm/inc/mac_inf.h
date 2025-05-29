/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Mac interface
 */

#ifndef __MAC_INF_H__
#define __MAC_INF_H__

#include <soc_types.h>

#ifdef __cplusplus
extern "C" {
#endif

EXT_EAPI td_u32 uapi_send_app(EXT_IN td_pvoid frame, td_u16 frame_size, EXT_CONST EXT_IN dms_mac_frame_ctrl *frame_ctr);
EXT_EAPI td_u32 dms_send_frame(EXT_IN td_pvoid frame, td_u16 frame_size,
    EXT_CONST EXT_IN dms_mac_frame_ctrl *frame_ctr);
EXT_EAPI td_u32 MAC_SendFrame(EXT_IN td_pvoid frame, td_u16 frame_size, EXT_IN dms_mac_frame_ctrl *frm_ctrl);

EXT_EAPI td_u32 dms_send_message(td_u32 msg_id, EXT_IN td_pvoid msg_param);
EXT_EAPI td_u32 mac_send_message(td_u32 msg_id, EXT_IN td_pvoid msg_param);
EXT_EAPI td_u16 mac_get_tl_large_pkt_max_size(td_void);

#if defined(PRODUCT_CFG_FEATURE_MAC_TASK)
#define mac_send_frame(frame, frame_size, frm_ctrl) uapi_send_app(frame, frame_size, frm_ctrl)
#define mac_send_message(msg_id, msg_param)         mac_send_message(msg_id, msg_param)
#else
#define mac_send_frame(frame, frame_size, frm_ctrl) 0
#define mac_send_message(msg_id, msg_param)         0
#endif

enum {
    ID_MSG_MAC_BEGIN = 0x100,
    ID_MSG_POWERON_REQ,
    ID_MSG_POWERON_CNF,
    ID_MSG_POWEROFF_REQ,
    ID_MSG_POWEROFF_CNF,
    ID_MSG_ROUTER_STATUS_IND,
    ID_MSG_DIAG_CMD,
    ID_MSG_NV_REFRESH_NOTIFY,

    ID_MSG_DFX_CTRL_PK,
    ID_MSG_DFX_QUERY_NOTIFY,
    ID_MSG_DFX_MGT_MSG,

    ID_MSG_TX_STATUS_IND,      /* MSG_TX_STATUS_IND_S */
    ID_MSG_ASSOC_SYS_INFO_IND, /* MSG_ASSOC_SYS_INFO_IND_S */
    ID_MSG_TX_FRM_QRY_IND,     /* MSG_TX_FRM_QRY_IND_S */
    ID_MSG_BIDB_CHANGED,       /* MSG_BIDB_CHANGED_S */
    ID_MSG_MAC_CTRL,           /* MSG_MAC_CTRL_S */
    ID_MSG_RSB_REASSEMBLE_NOTIFY,
    ID_MSG_MAC_END = 0x400,

    ID_MSG_DFX_REQ = 0x410, /* Channel connection request */
    ID_MSG_DFX_RSP,         /* Channel connection response */
    ID_MSG_DFX_HEART,       /* Channel heartbeat packet */
    ID_MSG_DFX_NDC,         /* Use of the network search function of the NCR */
    ID_MSG_DFX_CHL_PKT,
    ID_MSG_DFX_BIG_PKT_RPT,
    ID_MSG_DFX_HIGHEST_LID,
    ID_MSG_DFX_NOTIFY_CCO_ACTIVE_QUERY,
    ID_MSG_DFX_CCO_ACTIVE_TIMER_TIMEOUT,
    ID_MSG_DFX_P2P_FC,
    /* NDA */
    ID_MSG_NFC_BEGIN = 0x500,
    ID_MSG_NFC_REQ_APK, /* The NCR channel forwards the request to the packet flow control channel.-->NDM */
    ID_MSG_NFC_RCV_APK, /* CCO/STA-->NDM */
    ID_MSG_NFC_MGT,     /* Internal management message */
    ID_MSG_NFC_END = 0x600,
};
/* After the dms_send_message interface is open-source, this definition can be open-source. */
#define EXT_DMS_CHL_MSG_MIN 0x1000
#define EXT_DMS_CHL_MSG_MAX 0x4000

typedef enum {
    EXT_CHL_MSG_MIN = EXT_DMS_CHL_MSG_MIN,

    EXT_CHL_MSG_MAX = EXT_DMS_CHL_MSG_MAX
} ext_chl_msg;

typedef struct {
    td_bool router_avail;
    td_bool is_cco;
    td_u16 tei; /* Only the lower 12 bits are valid. */
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN];
    td_u8 cco_mac[EXT_PLC_MAC_ADDR_LEN];
} msg_router_status_ind_stru;

typedef struct {
    td_u16 id; /* DIAG Package ID */
    td_u16 payload_size;
    td_byte puc_payload[0];
} msg_diag_cmd_stru;

#define NV_NOTIFY_NUM_MAX 4
typedef struct {
    td_u16 aus_nvid[NV_NOTIFY_NUM_MAX];
} msg_nv_refresh_notify_stru;

typedef struct {
    td_u32 idx;
    td_u16 pb_size;
    td_u16 arq_cnt;
    td_u32 err;
    td_u32 time_stamp;
} msg_tx_status_ind;

typedef struct {
    td_u8 data[EXT_DMS_PLC_FRM_LEN2 - 1];
    td_u8 data_size;
} msg_tx_frm_qry_ind;

typedef ext_sys_status_info msg_assoc_sys_info_ind_s;

typedef struct {
    td_u32 rc;
    td_u8 reserved[8]; /* reserved 8 bytes */
} msg_bidb_changed;

td_void uapi_notice_upgrade_state(td_bool upgrade_state);

#ifdef __cplusplus
}
#endif

#endif /* __MAC_INF_H__ */

