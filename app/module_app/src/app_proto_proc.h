/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: APP module protocol frame handle.
 */

#ifndef APP_PROTO_PROC_H
#define APP_PROTO_PROC_H

#include "app_proto.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PROTO_1S_CONVERT_MS       1000
#define PROTO_DELAY_MIN_TIME_MS   500
#define PROTO_PAD_LEN1            2
#define PROTO_PAD_LEN2            3
#define PROTO_CRC_HIGH_BIT_OFFSET 8
#define PROTO_MANU_CODE_LEN       2
#define EXT_CHIP21                0x7862 /* Chip Type */

typedef enum {
    APP_PROTO_SOURCE_UART,
    APP_PROTO_SOURCE_SPI,
    APP_PROTO_SOURCE_PLC,
} app_proto_source;

#define PROTO_RET_CODE_SUCCESS 0x00
#define PROTO_RET_CODE_FAILED  0x01

/* Abnormal status code */
#define PROTO_REASON_NORMAL  0x00 /* The status is normal. */
#define PROTO_REASON_TIMEOUT 0x01 /* communication timeout */
#define PROTO_REASON_FORMAT  0x02 /* format error */
#define PROTO_REASON_BUSY    0x03 /* node busy */
#define PROTO_REASON_OTHER   0xFF /* Other errors */

typedef enum {
    ID_PROTO_CMD_GET_VERSION = 0x0001,
    ID_PROTO_CMD_GET_MAC_ADDR,
    ID_PROTO_CMD_GET_DEV_ADDR,
    ID_PROTO_CMD_SET_DEV_ADDR,
    ID_PROTO_CMD_REBOOT_MODULE,
    ID_PROTO_CMD_TRANS_FILE,
    ID_PROTO_CMD_QRY_ONPOWER_TIME,

    ID_PROTO_CMD_GET_WLIST_NUM = 0x0010,
    ID_PROTO_CMD_GET_WLIST_INFO,
    ID_PROTO_CMD_ADD_WLIST_NODE,
    ID_PROTO_CMD_DEL_WLIST_NODE,
    ID_PROTO_CMD_CLEAR_WLIST,
    ID_PROTO_CMD_SET_SELF_IDY,
    ID_PROTO_CMD_SET_WLIST_STATE,
    ID_PROTO_CMD_GET_WLIST_STATE,

    ID_PROTO_CMD_GET_TOPO_NUM = 0x0020,
    ID_PROTO_CMD_GET_TOPO_INFO,

    ID_PROTO_CMD_DATA_TRANS_TX = 0x0100,
    ID_PROTO_CMD_DATA_TRANS_RX,

    ID_PROTO_CMD_REMOTE_CMD_TX = 0x0110,
    ID_PROTO_CMD_REMOTE_CMD_RX,
} id_proto_cmd;

/* Response structure for reading the version information */
typedef struct {
    td_char manu_code[PROTO_MANU_CODE_LEN]; /* vendor code */
    td_u16 chip_type;                       /* Chip model: 11H */
    td_u16 version;                         /* Version number */
    td_u16 reserved;
} proto_get_version_ind;

/* Structure of the response to the request for reading the MAC address */
typedef struct {
    td_u8 addr[EXT_PLC_MAC_ADDR_LEN]; /* address */
    td_u8 reserved[PROTO_PAD_LEN1];
} proto_get_mac_addr_ind;

/* Structure of the response to the request for reading the MAC address */
typedef struct {
    td_u8 addr[EXT_PLC_MAC_ADDR_LEN]; /* address */
    td_u16 frame_index;

    td_u32 on_power_ms_time;
} proto_qry_on_power_time_ind;

/* Response structure for reading the communication address */
typedef proto_get_mac_addr_ind proto_get_dev_addr_ind;

/* Request structure for setting the communication address */
typedef proto_get_mac_addr_ind proto_set_dev_addr_req;

/* Acknowledgement/Negative acknowledgment structure */
typedef struct {
    td_u8 ret_code; /* Error code. 0: success; 1: failure */
    td_u8 reason;   /* Cause of the error */
    td_u8 reserved[PROTO_PAD_LEN1];
} proto_reply_ind;

typedef struct {
    td_u8 state; /* reply state */
    td_u8 reserved[PROTO_PAD_LEN2];
} proto_reply_info;

/* Set the communication address response structure. */
typedef proto_reply_ind proto_set_dev_addr_ind;

/* Structure of the module restart request */
typedef struct {
    td_u8 delay; /* Delay restart time, in seconds */
    td_u8 reserved[PROTO_PAD_LEN2];
} proto_module_reboot_req;

/* Structure of the module restart response */
typedef proto_reply_ind proto_module_reboot_ind;

/* Transfer files. */
#define PROTO_FN_START_TRANS_FILE   0x01
#define PROTO_FN_TRANS_FILE_DATA    0x02
#define PROTO_FN_TRANS_FILE_QUERY   0x03
#define PROTO_FILE_ATTR_CLEAR       0x00
#define PROTO_FILE_ATTR_LOCAL       0x01
#define PROTO_FILE_ATTR_REMOTE      0x02
#define PROTO_FILE_ATTR_INVALID     0xFF
#define PROTO_TRANS_FILE_DONE       0x00
#define PROTO_TRANS_FILE_INPROGRESS 0x01
#define PROTO_TRANS_FILE_UNCOMPLETE 0x02

/* Structure of the request for starting the transfer */
typedef struct {
    td_u8 fn;             /* Function code: 01H-start transmission */
    td_u8 file_attr;      /* File attribute: 0-Clear the download.
                             1: local upgrade file; 2: network-wide upgrade file. */
    td_u16 seg_total;     /* Total number of segments in the file transfer content. */
    td_u32 file_len;      /* File length. */
    td_u32 file_crc;      /* File content CRC32 checksum */
    td_u32 trans_timeout; /* Timeout interval for file transfer, in minutes. */
} proto_start_trans_file_req;

/* Structure of the transmission start response */
typedef struct {
    td_u8 fn;     /* Function code: 01H-start transmission */
    td_u8 state;  /* Error code. 0: success; 1: failure */
    td_u8 reason; /* Cause of the error */
    td_u8 rsv;
} proto_start_trans_file_ind;

/* Data transfer request structure */
typedef struct {
    td_u8 fn; /* Function code: 02H-Transmission data */
    td_u8 rsv;
    td_u16 seg_num;    /* File transfer segment number. */
    td_u16 seg_size;   /* File transfer segment size. */
    td_u16 seg_crc;    /* File transfer segment CRC16-CCITT checksum. */
    td_u8 seg_data[0]; /* File transfer segment data. */
} proto_trans_file_data_req;

/* Structure of the data transfer response */
typedef struct {
    td_u8 fn;     /* Function code: 02H-Transmission data */
    td_u8 state;  /* Error code. 0: success; 1: failure */
    td_u8 reason; /* Cause of the error */
    td_u8 rsv;
} proto_trans_file_data_ind;

/* Structure of the request for querying the processing progress */
typedef struct {
    td_u8 fn; /* Function code: 03H-Query processing progress */
    td_u8 rsv[PROTO_PAD_LEN2];
} proto_trans_file_query_req;

/* Structure of the response to the request for querying the processing progress */
typedef struct {
    td_u8 fn; /* Function code: 03H-Query processing progress */
    td_u8 state;
    td_u16 fail_sta;
} proto_trans_file_query_ind;

/*
 * Whitelist operation
 * Read the number of nodes in the whitelist.
 */
typedef struct {
    td_u16 num; /* Number of Nodes */
    td_u16 reserved;
} proto_get_whitelist_num_ind;

/* Number of nodes */
typedef struct {
    td_u16 start; /* Start sequence number, starting from 1 */
    td_u16 count; /* Number of nodes to be queried */
} proto_get_whitelist_info_req;

/* Structure of the response to the request for reading the whitelist information */
typedef struct {
    td_u16 total; /* Total number of nodes */
    td_u16 start; /* Start sequence number, starting from 1 */
    td_u16 count; /* Number of nodes to be queried */
    td_u16 reserved;
    td_u8 data[0]; /* Length of the data reported by the current frame */
} proto_get_whitelist_info_ind;

/* Request structure for adding a node to the whitelist */
typedef struct {
    td_u16 count;  /* Number of nodes */
    td_u8 data[0]; /* Whitelist data */
} proto_add_whitelist_req;

typedef struct {
    td_bool state;
    td_u8 wait_sta_join_time; /* min */
    td_u16 valid_time_diff;  /* ms */
} proto_set_ni_state_info;
/* Response structure for adding a node to the whitelist */
typedef proto_reply_ind proto_add_whitelist_ind;

/* Request structure for deleting a node from the whitelist */
typedef proto_add_whitelist_req proto_del_whitelist_req;

/* Response structure for deleting a node to the whitelist */
typedef proto_reply_ind proto_del_whitelist_ind;

/* Clear the whitelist response structure. */
typedef proto_reply_ind proto_clear_whitelist_ind;

/* Enable network identify response structure. */
typedef proto_reply_ind proto_enable_ni_ind;

/*
 * Topology operations
 * Read the number of nodes in the topology.
 */
typedef proto_get_whitelist_num_ind proto_get_topo_num_ind;

/* Structure of the request for reading topology information */
typedef proto_get_whitelist_info_req proto_get_topo_info_req;

/* Structure of the response to the request for reading topology information */
typedef proto_get_whitelist_info_ind proto_get_topo_info_ind;

typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN]; /* MAC address */
    td_u16 tei;                     /* Node ID */
    td_u16 proxy;                   /* Proxy node ID */
    td_u8 level : 4;                /* Network layer: 0 indicates layer 0, and so on. */
    td_u8 role : 4;                 /* Network role. */
    td_u8 reserved;
} proto_topo_node;

/*
 * Channel forwarding command
 * Structure of a data sending request
 */
typedef struct {
    td_u8 dst_addr[EXT_PLC_MAC_ADDR_LEN]; /* destination communication address;
                                             * FFFFFFFFFFFF indicates the network-wide broadcast */
    td_u16 data_length;                  /* User data length */
    td_u8 data[0];                       /* User data */
} proto_data_transmit_tx_req;

/* Structure of the data transmission response */
typedef proto_reply_ind proto_data_transmit_tx_ind;

/* Structure of the request for receiving data */
typedef struct {
    td_u8 src_addr[EXT_PLC_MAC_ADDR_LEN]; /* transmit device communication address */
    td_u16 data_length;                  /* User data length */
    td_u8 data[0];                       /* User data */
} proto_data_transmit_rx_req;

/* Structure of the response to the received data */
typedef proto_reply_ind proto_data_transmit_rx_ind;

/*
 * Remote commissioning command
 * Remotely send a command.
 */
typedef proto_data_transmit_tx_req proto_remote_cmd_tx_req;
typedef proto_data_transmit_tx_ind proto_remote_cmd_tx_ind;

/* Receive commands remotely */
typedef proto_data_transmit_rx_req proto_remote_cmd_rx_req;
typedef proto_data_transmit_rx_ind proto_remote_cmd_rx_ind;

td_u16 app_proto_get_frame_seq(td_void);
td_void app_proto_frame_seq_add(td_void);
td_u32 app_proto_frame_rx(td_pbyte data, td_u16 data_length, app_proto_source source, td_pbyte src_addr);
td_u32 app_proto_data_transmit(td_pbyte data, td_u16 data_length, td_pbyte src_addr);
td_void fill_proto_frame_fixed_info(app_proto_frame_head *head, td_u16 cmd, td_u16 seq, td_u16 data_length);
td_u32 app_proto_plc_tx(td_pbyte data, td_u16 data_length, td_pbyte dst_addr, td_u16 id);

#ifdef __cplusplus
}
#endif
#endif

