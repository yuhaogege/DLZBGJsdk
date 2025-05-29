/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: DFX interfaces of network management
 */
#ifndef _DFX_MAC_NM_H_
#define _DFX_MAC_NM_H_
/* 2 Query class
 * 2.1 Headend: network convergence time
 */
#define DFX_CHOSE_PROXY_SUM DFX_PROXY_SUM
typedef struct _diag_cmd_query_network_convergence_time_stru_ {
    td_u32 convergence_time; /* Networking completion time (unit: s) */
} diag_cmd_query_network_convergence_time;
/* *
 * 2.1.1 Headend: frequency band status
 */
typedef struct _diag_cmd_query_frequency_status_stru_ {
    td_u8 is_rcv_change_freq_mode_notify; /* 0: The frequency band remains unchanged. 1: The frequency band changes.
                                           * 2: The frequency band does not change.
                                           */
    td_u8 pad[3];                         /* Reserved 3 bytes */
} diag_cmd_query_frequency_status;

typedef struct {
    td_u32 snid;
    td_u8 is_single_comm; /* Whether one-way audio occurs */
    td_u8 rsv[3];         /* Reserved 3 bytes */
    td_u32 bandwidth;
} diag_cmd_query_neighbor_network_entry_stru;
typedef struct _diag_cmd_query_neighbor_network_stru_ {
    td_u16 num;                                                         /* Number of valid networks */
    td_u16 pad;                                                         /* Reserved */
    diag_cmd_query_neighbor_network_entry_stru entry[MAX_NEIGHBOR_SUM]; /* neighbor network information */
} diag_cmd_query_neighbor_network;

typedef struct _diag_cmd_query_request_join_failed_count_stru_ {
    td_u32 request_join_failed_count;
} diag_cmd_query_request_join_failed_count;

typedef struct _diag_cmd_query_join_network_expired_stru_ {
    td_u32 expire;
} diag_cmd_query_join_network_expired;

typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN];
    td_u16 tei;
} diag_cmd_query_associated_stas_entry_stru;
/* Associated object information of the site */
typedef struct _diag_cmd_query_associated_stas_stru_ {
    td_u16 num; /* Number of valid sites */
    td_u16 rsvd;
    diag_cmd_query_associated_stas_entry_stru entry[DFX_ASSOCIATED_STAS_ENTRY_MAX]; /* associated site */
} diag_cmd_query_associated_stas;
/* Records of successful site network access */
typedef struct {
    td_u16 assoc_req_cnt;             /* Number of association requests */
    td_u16 proxy_tei;                 /* Final proxy selected */
    td_u32 snid;                      /* Network SNID */
    td_u8 freq;                       /* Frequency band */
    td_u8 network_form_serial_number; /* Networking serial number */
    td_u8 pad[2];                     /* Reserved 2 bytes */
    td_u32 join_time;                 /* Add to network time */
} diag_cmd_assoc_entry_info_stru;
#define NM_ASSOC_DFX_INFO_NUM 10
/* Query the IND record of successful network access of the site */
typedef struct {
    td_u32 index; /* Index number */
    diag_cmd_assoc_entry_info_stru asso_info[NM_ASSOC_DFX_INFO_NUM];
} diag_cmd_assoc_info;
/* Records of network entry attempts of the BTS */
typedef struct {
    td_u32 snid;   /* Network ID */
    td_u8 freq;    /* Band number */
    td_u8 status;  /* Status. Bit 0 indicates whether the beacon is received,
                      and bit 1 indicates whether the beacon is rejected. */
    td_u8 req_cnt; /* Number of association times */
    td_u8 pad;
    td_u32 starttime; /* Start time of the attempt */
    td_u32 first_bc_time;
} diag_cmd_multi_network_assoc_info_stru;
#define NM_MULTI_NETWORK_ASSOC_INFO_NUM 18
/* Query the network entry attempt record of the BTS (IND) */
typedef struct {
    td_u8 index;               /* Index number */
    td_u8 assoc_state_err_cnt; /* association status errors */
    td_u16 pad1;
    diag_cmd_multi_network_assoc_info_stru multi_network_assoc_entry[NM_MULTI_NETWORK_ASSOC_INFO_NUM];
    /* Site associated network entry record */
} diag_cmd_multi_network_assoc_dfx;
typedef struct {
    td_u8 cco_mac[EXT_PLC_MAC_ADDR_LEN]; /* CCO MAC address */
    td_u8 pad;
    td_u8 network_serial_num; /* Networking serial number */
    td_u32 snid;              /* Network ID */
    td_u32 change_time;       /* Change time */
} diag_cmd_serial_num_change_entry_stru;
#define NM_SERIAL_NUM_CHANGE_INFO_NUM 10
typedef struct {
    td_u32 index; /* Index number */
    /* Network Sequence Change History */
    diag_cmd_serial_num_change_entry_stru serial_num_change_entry[NM_SERIAL_NUM_CHANGE_INFO_NUM];
} diag_cmd_serial_num_change_dfx;

typedef struct {
    td_u16 excepted_reply_count;     /* Number of faulty trunks that are detected */
    td_u16 excepted_meter_count;     /* Number of detected faulty terminals */
    td_u16 first_formed_net_elapsed; /* networking duration for the first time */
    td_u16 pad;
    td_u32 last_formed_net_elapsed; /* last networking duration */
    td_u32 re_formed_net_count;     /* Number of re-networking times */
} diag_cmd_query_network_state_info_cco_entry;
typedef struct {
    td_u32 assoced_failed_count;       /* Number of Association Initiation Failures */
    td_u32 assoced_succed_count;       /* Number of Successful Association Initiation Times */
    td_u32 assoced_last_succed_expire; /* the time for the last successful association */
} diag_cmd_query_network_state_info_sta_entry;
typedef struct {
    diag_cmd_query_network_state_info_cco_entry cco_entry; /* CCO networking time information */
    diag_cmd_query_network_state_info_sta_entry sta_entry; /* STA networking time information */
} diag_cmd_query_network_state_info;

typedef struct {
    td_u32 create_cnt;       /* Number of generated packets */
    td_u32 rcv_cnt;          /* Number of received packets */
    td_u32 last_create_time; /* Last packet generation time */
    td_u32 last_rcv_time;    /* Last time when the packet is received */
} nm_packet_info;
/* Maximum number of recorded packets */
#define MAX_RCV_PACKET_INFO 20
typedef struct {
    td_u8 req_mac[EXT_PLC_MAC_ADDR_LEN];
    td_u16 pad;
    td_u32 rcv_time;
} nm_rcv_assoc_object_info;
typedef struct {
    td_u8 index;
    td_u8 pad[3]; /* Reserved 3 bytes */
    nm_rcv_assoc_object_info pk_info[MAX_RCV_PACKET_INFO];
} nm_rcv_assoc_packet_info;
typedef struct {
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA) || defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    nm_packet_info assoc_req_info;               /* association request packet information */
    nm_packet_info assoc_cnf_info;               /* association response packet information */
    nm_packet_info assoc_ind_info;               /* association acknowledgment packet information */
    nm_packet_info assoc_gather_ind_info;        /* associated summary packet information */
    nm_packet_info change_proxy_req_info;        /* proxy change request information */
    nm_packet_info change_proxy_cnf_info;        /* proxy change confirmation information */
    nm_rcv_assoc_packet_info rcv_assoc_req_info; /* Information about the received association request packet */
    nm_rcv_assoc_packet_info rcv_assoc_cnf_info; /* Information about the received association reply packet */
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    td_u16 rcv_heartbeat_pk_cnt; /* Number of received heartbeat packets */
    td_u16 heartbeat_cycle_cnt;  /* Heartbeat period count */
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    td_u16 create_heartbeat_by_timer_cnt; /* Number of heartbeat packets triggered by the timer */
    td_u16 create_heartbeat_by_rcv_cnt;   /* Number of received heartbeat packets that
                                             trigger heartbeat packet creation */
#endif
    td_u16 creat_discovery_list_cnt; /* Number of packets for creating a discovery list */
    td_u16 rcv_discovery_list_cnt;   /* Number of received discovery list packets */
    td_u16 send_msdu_success_cnt;    /* Number of packets that are successfully sent to the aggregation layer */
    td_u16 send_msdu_fail_cnt;       /* Number of times packets fail to be sent to the aggregation node */
    td_u16 rcv_msdu_cnt;             /* Number of received NMS packets */
    td_u16 not_support_msdu_cnt;     /* Number of packets that cannot be processed */
} nm_packet_stat_info_st;            /* Query structure:
                                      * DIAG_CMD_NM_PACKET_STAT_STRU
                                      */
typedef struct {
    nm_rcv_assoc_packet_info rcv_assoc_req_info; /* Information about the received association request packet */
} diag_cmd_nm_assoc_req_stat;
typedef struct {
    nm_rcv_assoc_packet_info rcv_assoc_cnf_info; /* Information about the received association reply packet */
} diag_cmd_nm_assoc_cnf_stat;
typedef struct {
    nm_packet_info assoc_req_info;        /* association request packet information */
    nm_packet_info assoc_cnf_info;        /* association response packet information */
    nm_packet_info assoc_ind_info;        /* association acknowledgment packet information */
    nm_packet_info change_proxy_req_info; /* proxy change request information */
    nm_packet_info change_proxy_cnf_info; /* proxy change confirmation information */
    nm_packet_info assoc_gather_ind_info; /* associated summary packet information */
    td_u32 creat_discovery_list_cnt;      /* Number of packets for creating a discovery list */
    td_u32 rcv_discovery_list_cnt;        /* Number of received discovery list packets */
    td_u32 rcv_heartbeat_pk_cnt;          /* Number of received heartbeat packets */
    td_u32 heartbeat_cycle_cnt;           /* Heartbeat period count */
    td_u32 create_heartbeat_by_timer_cnt; /* Number of heartbeat packets triggered by the timer */
    td_u32 create_heartbeat_by_rcv_cnt;   /* Number of received heartbeat packets that
                                             trigger heartbeat packet creation */
    td_u32 send_msdu_success_cnt;         /* Number of packets that are successfully sent to the aggregation layer */
    td_u32 send_msdu_fail_cnt;            /* Number of times packets fail to be sent to the aggregation node */
    td_u32 rcv_msdu_cnt;                  /* Number of received NMS packets */
    td_u32 not_support_msdu_cnt;          /* Number of packets that cannot be processed */
} diag_cmd_nm_packet_stat;

typedef struct {
    td_u32 nm_leave_sta_num;                      /* CCO statistics offline indication station number */
    td_u32 nm_send_delay_leave_pk_cnt;            /* Number of delayed offline indication packets sent by the CCO */
    td_u32 nm_send_short_delay_leave_pk_fail_cnt; /* Number of delayed offline indication packets that fail to
                                                   * be sent by the CCO
                                                   */
    td_u32 nm_send_leave_ind_white_cnt;        /* Number of times the CCO sends whitelist offline indication packets */
    td_u32 nm_send_leave_ind_cnt;              /* Number of offline indication packets sent by the CCO */
    td_u32 nm_rcv_delay_leave_pk_cnt;          /* Number of Delayed Offline Indication Packets Received by a STA */
    td_u32 nm_rcv_last_delay_leave_pk_time;    /* System time when the STA last receives
                                                * the delay offline indication packet
                                                */
    td_u32 nm_handle_last_delay_leave_pk_time; /* workstation processing delay offline indication packet system time */
    td_u32 nm_delay_leave_cnt;                 /* Number of delayed offline times of the site */
    td_u32 nm_delay_leave_time;                /* workstation offline delay system time */
    /*
     * Number of offline indication packets kicked out
     * by the workstation from the whitelist
     */
    td_u32 nm_handle_leave_ind_white_cnt;
    td_u32 nm_handle_leave_ind_cnt; /* Number of times that the workstation processes offline
                                     * indication packets for non-whitelist users
                                     */
} diag_cmd_query_delay_leave_ind_info;
typedef struct {
    td_u32 create_proxy_change_req_cnt;              /* create proxy change request packets */
    td_u32 handle_proxy_change_cnf_cnt;              /* Number of times that the processes proxy change reply packets */
    td_u32 handle_proxy_change_bitmap_cnf_cnt;       /* Number of times that the processes bitmap
                                                          proxy change reply packets */
    td_u32 proxy_change_timeout_cnt;                 /* Number of proxy change request timeouts */
    td_u32 proxy_change_cnf_sequence_err_cnt;        /* Number of Proxy Change Response Sequence Number Exceptions */
    td_u16 candinate_proxy_tei[DFX_CHOSE_PROXY_SUM]; /* current candidate proxy */
    td_u16 proxy_tei;                                /* Current proxy site */
    td_u16 no_return_cnt; /* Number of times that the current proxy is changed but not returned */
    td_u8 proxy_changing; /* Change status of the current proxy */
    td_u8 pad;
} diag_cmd_query_proxy_change_pk_stat;
typedef struct {
    td_u32 nm_rcv_assoc_req_cnt;                  /* Number of received association requests */
    td_u32 nm_assoc_req_sequence_err_cnt;         /* Number of association request sequence number errors */
    td_u32 nm_assoc_req_network_form_seq_err_cnt; /* Number of association requests with incorrect networking
                                                   * sequence numbers
                                                   */
    td_u32 nm_send_assoc_cnf_cnt;                 /* Number of sent associated replies */
    td_u32 nm_create_assoc_ind_cnt;               /* Number of sent association indication packets */
    td_u32 nm_create_assoc_gather_ind_cnt;        /* Total Packets */
    td_u32 nm_mesh_over_fail_limit_cnt;
    td_u32 nm_rcv_proxy_change_req;          /* Number of received agent change requests */
    td_u32 nm_send_proxy_change_cnf;         /* Number of sent agent change reply packets */
    td_u32 nm_send_proxy_change_bitmap_cnf;  /* Bitmap Proxy change reply packets */
    td_u32 nm_proxy_change_sequence_err_cnt; /* Number of proxy change requests with incorrect sequence numbers */
    td_u32 nm_handle_proxy_change_fail_cnt;  /* Number of proxy change requests that fail to be processed */
    td_u16 nm_get_mac_buffer_fail_cnt;       /* Number of times that the network management fails to
                                              * apply for the MAC buffer
                                              */
    td_u16 nm_send_msg_fail_cnt;             /* Number of messages that the network management module fails to send */
    /*
     * Time when the first local network association request is
     * received after re-networking. The unit is ms.
     */
    td_u32 first_rcv_assoc_req_time;
    td_u32 last_rcv_assoc_req_time; /* Time when the last local network association request is
                                     * received during re-networking. The unit is ms.
                                     */
    td_u16 mallc_fail_cnt;
    td_u16 nm_send_mesh_rreq_cnt;

    td_u16 nm_send_mesh_rreply_cnt;
    td_u16 nm_rcv_mesh_rreq_cnt;

    td_u16 nm_rcv_mesh_rreply_cnt;
    td_u16 nm_mesh_check_rt_cnt;

    td_u16 nm_mesh_rt_del_cnt;
    td_u16 nm_mesh_req_transmit_cnt;

    td_u16 nm_mesh_reply_transmit_cnt;
    td_u16 nm_mesh_send_rt_info_cnt;

    td_u16 nm_mesh_rcv_rt_info_cnt;
    td_u16 nm_mesh_send_rt_invalid_cnt;

    td_u16 nm_mesh_rcv_rt_invalid_cnt;
    td_u16 nm_mesh_transmit_rt_invalid_cnt;
} diag_cmd_query_assoc_req_pk_stat;

#define DIAG_CMD_QUERY_DEVICE_FREQ_LIST_SIZE 16
#define DIAG_CMD_QUERY_DEVICE_FREQ_INFO_LIST_SIZE 6
typedef struct {
    td_char list[DIAG_CMD_QUERY_DEVICE_FREQ_LIST_SIZE];
} diag_cmd_query_device_freq_info_list_entry;
typedef struct {
    td_u32 max_freq_num;                                     /* Total number of frequency bands */
    td_u32 freq_change_num;                                  /* Number of frequency band switchovers */
    td_char curr_freq[DIAG_CMD_QUERY_DEVICE_FREQ_LIST_SIZE]; /* Current frequency band */
    diag_cmd_query_device_freq_info_list_entry
        freq_list[DIAG_CMD_QUERY_DEVICE_FREQ_INFO_LIST_SIZE]; /* indicates the current frequency band character. */
    diag_cmd_query_device_freq_info_list_entry
        freq_total_list[DIAG_CMD_QUERY_DEVICE_FREQ_INFO_LIST_SIZE]; /* characters of all frequency bands */
} diag_cmd_query_device_freq_info;

#define NM_MAX_BACKUP_MAC_CNT 6
typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN];
    td_u8 pad[2]; /* Reserved 2 bytes */
} ext_mac_stru;
typedef struct {
    td_u8 index;     /* Currently, the MAC index is used. */
    td_u8 total_cnt; /* Total number of MAC addresses */
    td_u8 clear_cnt; /* Times of clearing MAC addresses */
    td_u8 set_cnt;   /* Set the number of MAC address backups. */
    ext_mac_stru mac_list[NM_MAX_BACKUP_MAC_CNT];
} diag_cmd_query_backup_mac;
/*
 * MAC: SNID for multi-network settings. What is the difference between the STA and CCO behaviors?
 * CMD ID: ID_DIAG_CMD_CCO_MAC_MULTYNET_SET.
 * REQ STRU:DIAG_CMD_CCO_MAC_MULTYNET_SET_REQ_STRU.
 * ACk STRU:DIAG_CMD_CCO_MAC_MULTYNET_SET_ACK_STRU.
 */
#define MAC_MUTYNET_OP_VA_SET_SNID_ONLY 1     /* Set SN ID ONLY */
#define MAC_MUTYNET_OP_VA_SET_MUTY_SWT_ONLY 2 /* Set multi-network switch ONLY */
#define MAC_MUTYNET_OP_VA_SET_NORMAL 3        /* all set */
#define INVALID_MUTY_OPEN_FLAG 0xff
typedef struct {
    td_u32 snid;         /* V100: Valid value:[1, 15] Invalid value: 0xff. This parameter
                            is not set when the value is 0xff. */
    td_u8 is_multy_open; /* TRUE: enabled; FALSE: disabled; 0xFF: invalid. Do not set this
                            parameter when it is set to 0xFF. */
    td_u8 muty_op_va;    /* Operation code: 1Set enSnid; Set ucIsMultyOpen. 3: all settings */
    td_u8 rsv[2];        /* Reserved 2 bytes */
} diag_cmd_cco_mac_multynet_set_req;
/* ACK */
typedef struct {
    td_u32 ret; /* OK EXT_ERR_SUCCESS, OTHER FAIL */
} diag_cmd_cco_mac_multynet_set_ack;
/*
 * MAC: Query related to multiple networks, including the SNID, switch status,
 * and differences between STA and CCO behaviors.
 * CMD ID: ID_DIAG_CMD_CCO_MAC_MULTYNET_QUERY.
 * REQ STRU:NULL
 * ACk STRU:DIAG_CMD_CCO_MAC_MULTYNET_QUERY_ACK_STRU.
 */
typedef struct {
    td_u32 snid;         /* V100: Valid value: [1,6]; Invalid value: 0xff */
    td_u8 is_multy_open; /* TRUE: multi-network enabledFALSE: multi-network disabled0xFF: invalid */
    td_u8 rsv[3];        /* Reserved 3 bytes */
} diag_cmd_cco_mac_multynet_query_ack;
/*
 * MAC: frequency band configuration STA: not supported; CCO: supported
 * CMD ID: ID_DIAG_CMD_MAC_FREQ_SET.
 * REQ STRU:DIAG_CMD_MAC_FREQ_SET_REQ_STRU.
 * ACk STRU:DIAG_CMD_MAC_FREQ_SET_ACK_STRU.
 */
typedef enum mac_freq_ {
    EN_MAC_FREQ_0 = 0,
    EN_MAC_FREQ_1,
    EN_MAC_FREQ_2,
    EN_MAC_FREQ_3,
    EN_MAC_FREQ_4,
    EN_MAC_FREQ_5,
    EN_MAC_FREQ_MAX,
    EN_MAC_FREQ_INVALID = 0xff
} mac_freq;
typedef struct {
    /* Full frequency 0; Low frequency 1; High frequency 2: 0xff (invalid value) */
    mac_freq freq;
    td_u32 freq_swt_time; /* The unit is second. The default value is 2 minutes.
                           * The value 0 indicates that the frequency band is invalid and is not set.
                           */
} diag_cmd_mac_freq_set_req;
/* ACK STRU */
/* Synchronous immediate return */
typedef struct {
    td_u32 rc;            /* The result code EXT_ERR_SUCCESS OTHER VALUE fail is returned. */
    mac_freq freq;        /* Full frequency 0; Low frequency 1; High frequency 2: 0xff (invalid value) */
    td_u32 freq_swt_time; /* The unit is s. Frequency band switching time (2 minutes by default, 120s) */
} diag_cmd_mac_freq_set_ack;
/*
 * MAC: frequency band query CCO AND STA:
 * query the current frequency band, frequency band to be switched to, and remaining switching time.
 * CMD ID: ID_DIAG_CMD_MAC_FREQ_QUERY.
 * REQ STRU:NULL
 * ACk STRU:DIAG_CMD_MAC_FREQ_QUERY_ACK_STRU.
 */
typedef struct {
    mac_freq cur_freq;  /* The current frequency band 0xff is invalid. */
    mac_freq swt2_freq; /* Invalid 0xff. It is returned only during the handover. */
    td_u8 is_swtching;  /* Indicates whether the switchover is being performed.
                         * TD_TRUE indicates that the switchover is in progress,
                         * and TD_FALSE indicates that the switchover is not in progress.
                         */
    td_u8 blink_freq_seq;
    td_u8 rsv[2];         /* Reserved 2 bytes */
    td_u32 freq_swt_time; /* The unit is second. The minimum unit is second.
                           * If isSwtching is TRUE, how long will it take for the switchover to complete
                           */
} diag_cmd_mac_freq_query_ack;

#define MAC_DFX_WHITE_SET_WHITE_ONLY 1  /* Only isWhiteOpen is set. */
#define MAC_DFX_WHITE_SET_MTFILE_ONLY 2 /* Only isMtFileInWhite is set. */
#define MAC_DFX_WHITE_SET_NORMAL 3      /* Both isWhiteOpen and isMtFileInWhite are set. \
                                         * The dbk file system uses only this value.     \
                                         */
typedef struct {
    td_u8 oper_va;         /* Operation code: MAC_DFX_WHITE_SET_NOMAR etc, DBK Currently,
                            * only MAC_DFX_WHITE_SET_NORMAL is delivered. This value is not used.
                            */
    td_bool is_white_open; /* TRUE: enabled FALSE: disabled */
    td_u8 mode;            /* Whitelist mode */
    td_u8 rsv;
} diag_cmd_cco_mac_white_sta_set_req;
/* ACK STRU */
typedef struct {
    td_u32 rc; /* The result code EXT_ERR_SUCCESS,OTHER VALUE fail is returned if the setting is successful. */
} diag_cmd_cco_mac_white_sta_set_ack;
/*
 * MAC: blacklist/whitelist status query. Indicates whether to enable the status query function for the whitelist and
 * whether to add the meter archive to the whitelist.
 * CMD ID: ID_DIAG_CMD_CCO_MAC_WHITE_STA_QUERY.
 * REQ STRU:NULL
 * ACk STRU:DIAG_CMD_CCO_MAC_WHITE_STA_QUERY_ACK_STRU.
 */
typedef struct {
    td_u8 is_white_open; /* Whether to enable the whitelist in the NV item TD_TRUE Yes; TD_FALSE: no; */
    td_u8 mode;          /* Whitelist mode */
    td_u8 rsv1;          /* Whether MAC authentication is enabled in actual operation TD_TRUE Yes;
                          * TD_FALSE: no; DBK not used
                          */
    td_u8 rsv;
} diag_cmd_cco_mac_white_sta_query_ack;
/*
 * MAC: Clear MAC-related information, such as the CCO rejection list.
 * CMD ID: ID_DIAG_CMD_CCO_MAC_CLR.
 * REQ STRU: DIAG_CMD_CCO_MAC_CLR_REQ_STRU.
 * ACk STRU: DIAG_CMD_CCO_MAC_CLR_ACK_STRU.
 */
typedef enum mac_clr_oper_ {
    EN_MAC_CLR_CCO_REJECT_LIST = 1, /* clears the multi-network rejection list. */
    EN_MAC_CLR_INVALID = 0xff
} mac_clr_oper;
typedef struct {
    mac_clr_oper clr_op; /* See the preceding information. */
} diag_cmd_cco_mac_clr_req;
/* ACK STRU */
typedef struct {
    td_u32 rc; /* If the result code is cleared successfully, EXT_ERR_SUCCESS and OTHER VALUE fail are returned. */
} diag_cmd_cco_mac_clr_req_stru_ack;
typedef struct {
    td_u16 proxy_num; /* Number of current proxies */
    td_u16 pad;
} diag_cmd_proxy_num_req;
typedef struct {
    td_u16 child_sum; /* Number of sub-sites of a site */
    td_u16 pad;
} diag_cmd_child_sum_ack;
typedef struct {
    td_u16 tei; /* Query the site. */
    td_u16 pad;
} diag_cmd_child_sum_req;

typedef struct {
    td_bool is_tried; /* Indicates whether the SNID is tried in the current round. */
    td_u8 state;      /* 0 indicates no action. */
    td_s16 snr;
    td_u32 reassoc_time;          /* Next association time */
    td_u32 rcv_fc_cnt;            /* Number of received FCs */
    td_u32 refuse_duration;       /* Rejection time */
    td_u32 chose_cnt : 28;        /* Number of times the SNID is selected */
    td_u32 main_network_snid : 4; /* primary network SN ID */
} sta_snid_stat_info;
typedef struct {
    sta_snid_stat_info sta_snid_info[15]; /* STA associated network number, 15 at maximum */
} diag_cmd_sta_snid_info;

typedef struct {
    td_u16 my_tei;       /* TEI of the local site */
    td_u16 my_proxy_tei; /* Parent proxy of the local site */
    td_u8 my_level;      /* Local site level */
    /*
     * Indicates the capability of the local site, which can be a CCO, a regeneration site,
     * or a common site.
     */
    td_u8 my_ability;
    td_u8 my_mac[EXT_PLC_MAC_ADDR_LEN];   /* MAC address of the local site */
    td_u32 my_snid;                      /* Local site SNID */
    td_bool network_formed;              /* Whether the self-networking is complete */
    td_bool send_data_in_self_phase;     /* Indicates whether data is transmitted in the local site phase. */
    td_u8 my_phase_mode;                 /* Phase mode of the local site, multi-phase or single-phase */
    td_u8 my_phase;                      /* Phase type of the local site. A B C ALL */
    td_u8 form_serial_number;            /* Networking serial number */
    td_u8 freq_mode_present;             /* Frequency band mode of the site */
    td_u8 fast_evaluate_flag;            /* Fast route evaluation flag */
    td_u8 network_state;                 /* Network status */
    td_u32 select_snid;                  /* Preferentially select the SNID */
    td_u8 proxy_flag_exist;              /* Whether a static proxy exists */
    td_u8 chl_state;                     /* MAC channel status. */
    td_u8 pad[2];                        /* Reserved 2 bytes */
    td_u32 blind_check_count;            /* Number of blind detection times */
    td_u32 rcv_freq_change_notify_count; /* Number of Received Band Handover Notifications */
} diag_cmd_report_network_attribute;

/* MAC channel status. The definition is as follows: */
typedef enum {
    EXT_MAC_CHL_STATE_UNKNOWN = 0, /* The channel status is unknown. */
    EXT_MAC_CHL_STATE_FORMING,     /* Networking is in progress. */
    EXT_MAC_CHL_STATE_FORMED,      /* Networking is complete. */
    EXT_MAC_CHL_STATE_NORMAL,      /* Normal state, supporting all services of the specified APP. */
    EXT_MAC_CHL_STATE_MAINTAIN,    /* channel maintenance status. App services are not supported. */
} ext_mac_chl_state_enum;
/* 4.16 Initiate a proxy change or association request object */

typedef struct {
    td_u8 start_change_proxy;
    td_u8 start_assoc_req;
    td_u16 old_proxy;
    td_u16 chose_proxy_tei[DFX_CHOSE_PROXY_SUM]; /* Selected Proxy Site */
    td_u16 rsvd;
} diag_cmd_report_chose_proxy_tei_stru;
typedef struct {
    td_u32 bpc;
    td_u8 reason_code; /* Cause */
    td_u8 rsvd[3];     /* Reserved 3 bytes */
} diag_cmd_report_leave_network_stru;
#define VIOLENCE_STA_CNT_PER_PK 50 /* maximum number of rejection lists in each packet */
typedef struct {
    td_u8 mac[VIOLENCE_STA_CNT_PER_PK][EXT_PLC_MAC_ADDR_LEN]; /* Rejecting the MAC address of a site */
} diag_cmd_query_violence_sta;
#define MAX_VIOLENCE_STA_COUNT  EXT_REJECT_JOIN_NET_STA_NUM_MAX /* Maximum number of rejection lists, 200 */
#define MAC_PLC_QUERY_VIOLEDNCE_VER_R0 0 /* Defines the version number of the rejection list query. */
#define MAC_PLC_QUERY_VIOLEDNCE_VER_R1 1 /* Defines the version number of the rejection list query. */
typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN];
    td_u8 pad[2]; /* Reserved 2 bytes */
} diag_cmd_query_violence_sta_mac_stru;
typedef struct {
    td_u32 interface_ver; /* Version number */
    td_u16 sn;            /* Packet sequence number */
    td_bool is_last;      /* Whether the packet is the last packet */
    td_u8 num;            /* Number of reported messages */
    diag_cmd_query_violence_sta_mac_stru mac[VIOLENCE_STA_CNT_PER_PK];
} diag_cmd_query_max_violence_sta;
typedef struct {
    td_u16 whitelist_num;       /* Number of sites in the whitelist */
    td_bool mac_authentication; /* Whitelist switch status */
    td_u8 reason;               /* Whitelist change reason */
    td_u32 pad2;
} diag_cmd_report_whitelist_changed;
/* Report the current rate when the rate at the MAC layer is lower than the threshold. */
typedef struct {
    td_u32 timestamp;
    td_u32 ntb;
    td_u32 bpc;
    td_u32 rx_speed;
    td_u32 tx_speed;
} diag_cmd_notify_mac_speed_low;
typedef struct {
    td_u32 ndm_set_pk_cnt;        /* Total number of delivered packets */
    td_u32 ndm2nm_pk_cnt;         /* Indicates the number of packets delivered to the nm module for distribution. */
    td_u32 nm2cvg_pk_cnt;         /* Indicates the total number of packets delivered by the NDM module and
                                   * sent to the aggregation layer through the nm interface.
                                   */
    td_u32 cvg2seg_pk_cnt;        /* aggregates the total number of received NDM packets. */
    td_u32 seg_in_queue_pk_cnt;   /* Total number of NDM packets that enter the queue in segments */
    td_u32 seg_in_queue_fail_cnt; /* Total number of packets that fail to enter the fragment queue */
    td_u32 tx_all_pk_cnt;         /* Total number of NDM packets received by the sender */
    td_u32 tx_send_success_cnt;   /* Number of NDM packets successfully sent by the sender */
    td_u32 tx_send_fail_cnt;      /* Total number of NDM packets that fail to be sent by the sender. */
    td_u32 tx_send_timeout_cnt;   /* Total number of NDM packets that time out and are sent by the sender. */
    td_u32 tx_resend_cnt;         /* Total number of NDM packets retransmitted by the sender. */
    td_u32 rx_rcv_pk_cnt;         /* Number of NDM packets received by the receiver */
    td_u32 rsb2cvg_pk_cnt;        /* Total number of reassembled and distributed aggregated NDM packets */
    td_u32 rsb_timeout_cnt;       /* Number of reassembly timeout packets */
    td_u32 rsb_filter_pk_cnt;     /* Number of NDM packets filtered out during reassembly */
    td_u32 cvg2nm_pk_cnt;         /* Total number of NDM packets aggregated and distributed to the nm module */
    td_u32 nm_filter_pk_cnt;      /* Number of packets filtered out by the NMS */
    td_u32 nm2ndm_pk_cnt;         /* Total number of NDM packets processed and
                                   * distributed by the nm module to the NDM module
                                   */
} diag_cmd_query_ndm_pk_info;

typedef struct {
    td_u16 ptcl_ver;
    td_u16 pad;
} diag_cmd_coordinate_conflict_stat_req;
typedef struct {
    td_bool is_reported;                         /* Whether to report CCO */
    td_u8 pad[3];                                /* Reserved 3 bytes */
    td_u32 change_snid;                          /* Not used */
    td_u32 last_report_time;                     /* Last reported time */
    td_u32 report_cnt;                           /* Number of reporting times */
    td_u32 cco_mac_exception_cnt;                /* Total number of CCO MAC address exceptions */
    td_u32 change_snid_time;                     /* Change the SNID time */
    td_u16 exception_by_cco_mac_cnt;             /* Number of CCO MAC address conflicts */
    td_u8 conflict_cco_mac[EXT_PLC_MAC_ADDR_LEN]; /* conflict network CCO MAC address */
    td_u32 cco_mac_exception_detect_cnt;         /* Number of CCO MAC address conflicts */
    td_u32 random_backoff_time;                  /* Random backoff time */
    td_u32 rcv_conflict_report_cnt;              /* Number of Received Conflict Reports */
    td_u32 rcv_query_response_cnt;               /* Measurement point of Number of Received Query Responses */
} diag_cmd_coordinate_conflict_stat;
typedef struct {
    td_u8 cco_mac[EXT_PLC_MAC_ADDR_LEN]; /* CCO MAC address */
    td_u8 form_serial_number;           /* Networking serial number */
    td_u8 state;                        /* Status */
} diag_cmd_coordinate_cco_entry_stru;
typedef struct {
    diag_cmd_coordinate_cco_entry_stru cco_entry[6]; /* Coordinate CCO info entry is of 6 items */
} diag_cmd_coordinate_multi_cco_info_stru;
/* Set the multi-network listening time. */
typedef struct {
    td_u32 listen_time;
} diag_cmd_listen_time_before_first_bea_req_stru;
typedef struct {
    td_u32 ret_code; /* Returned error code */
} diag_cmd_listen_time_before_first_bea_rsp_stru;
/* Record the information about the frequency bands that the STA and NDM receive during the network-wide handover. */
typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN];
    td_u8 freq;
    td_u8 rcv_freq_bit_map; /* Received bitmaps of all frequency bands. The first bit indicates frequency band 1,
                             * the second bit indicates frequency band 2, and the third bit indicates frequency band 3.
                             * Bit 4 indicates frequency band 4, and bit 5 indicates frequency band 5.
                             */
    td_u32 rcv_sys_time;
    td_u32 switch_freq_cnt;
} diag_cmd_mac_switch_freq_entry_st;
typedef struct {
    td_u16 ptcl_ver; /* Protocol version number */
    td_u16 pad1;
    td_u32 nid;             /* Network NID */
    td_u32 select_nid;      /* Preferred NID */
    td_u32 ptcl_detect_nid; /* NID detected by the protocol */
    td_u32 pad2[8];         /* Reserved 8 words */
} diag_cmd_report_network_attribute_v200;

typedef struct {
    td_u32 mac_buf_freecnt;     /* Number of idle MAC BUFFER */
    td_u32 get_buf_fail_cnt;    /* Times of failing to obtain the MAC buffer */
    td_u32 get_buf_success_cnt; /* Number of times the MAC buffer is successfully obtained */
    td_u32 reset_monitor_cnt;   /* Monitor the MAC buffer idle threshold count. If the count is increasing,
                                 * it indicates that the count is not continuously below the threshold.
                                 */
    td_u32 use_black_num;
    td_u32 repeat_free_cnt; /* Number of repeated releases */
} diag_cmd_mac_buff_info;
#endif