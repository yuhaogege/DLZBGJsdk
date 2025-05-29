/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: DFX interfaces of topo
 */
#ifndef DFX_MAC_TP_H
#define DFX_MAC_TP_H

typedef enum {
    NFC_DIAG_CMD_TOPO_ENTRY_VER0 = 0,
    NFC_DIAG_CMD_TOPO_ENTRY_VER1,
} nfc_diag_cmd_topo_entry_ver;
#define NFC_DIAG_CMD_TOPO_ENTRY_VER (NFC_DIAG_CMD_TOPO_ENTRY_VER1)
typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN]; /* station MAC address */
    td_u8 spc_ver;                  /* code+nv SPC image.hupg */
    td_u8 spc_nv_ver;               /* nv image.hupg: SPC Ver */
    td_u32 level : 4;               /* Site level */
    td_u32 ability : 4;             /* Site role */
    td_u32 tei : 12;                /* TEI address of the site */
    td_u32 proxy_tei : 12;          /* TEI address of the parent proxy of the site */
    td_u32 join_time;               /* Time when the site is added to the network */
    td_u8 product_type : 3;         /* Product form */
    td_u8 in_use : 1;               /* Whether the workstation is online */
    td_u8 pad : 4;
    td_u8 sta_phase0 : 2; /* Site phase information */
    td_u8 sta_phase1 : 2; /* Site phase information */
    td_u8 sta_phase2 : 2; /* Site phase information */
    td_u8 proxy_type : 2; /* Trunk type */
    td_u8 nv_c_ver;       /* nv image.hupg: C Ver */
    td_u8 chip_ver_idx;
    td_u8 boot_ver_idx;
    td_u8 ver[3];     /* image.hupg 3 characters */
    td_u32 year : 6;  /* Customer version number-year */
    td_u32 month : 4; /* Customer version number-month */
    td_u32 day : 5;   /* Customer version number-day */
    td_u32 hour : 5;
    td_u32 minute : 6;
    td_u32 second : 6;
    td_u16 b_ver;            /* code+nv image.hupg */
    td_u16 b_nv_ver;         /* nv image.hupg: B Ver */
    td_u16 leave_cnt;        /* Number of offline times */
    td_u16 proxy_change_cnt; /* Number of proxy changes */
    td_u32 leave_total_time; /* Offline time */
    td_u16 hw_reset_cnt;     /* Record the accumulated number of device hard reset times. */
    td_u16 mac_sw_reset_cnt; /* records the accumulated times of MAC soft reset. */
} nfc_diag_cmd_query_network_topo_entry_s_ver0;
/* Structure corresponding to each site in the NDA channel topology reporting structure, 44 bytes */
typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN]; /* station MAC address */
    td_u8 spc_ver;                  /* code+nv SPC image.hupg */
    td_u8 sw_ver_lo;                /* Least significant bits of the client version number */
    td_u32 level : 4;               /* Site level */
    td_u32 ability : 4;             /* Site role */
    td_u32 tei : 12;                /* TEI address of the site */
    td_u32 proxy_tei : 12;          /* TEI address of the parent proxy of the site */
    td_u32 join_time;               /* Time when the site is added to the network */
    td_u8 product_type : 3;         /* Product form */
    td_u8 in_use : 1;               /* Whether the workstation is online */
    td_u8 stru_ver : 4;             /* structure version number. The value is 1 this time. */
    td_u8 sta_phase0 : 2;           /* Site phase information */
    td_u8 sta_phase1 : 2;           /* Site phase information */
    td_u8 sta_phase2 : 2;           /* Site phase information */
    td_u8 proxy_type : 2;           /* Trunk type */
    td_u8 sw_ver_hi;                /* Upper bytes of the client version number */
    td_u8 chip_ver_idx;
    td_u8 boot_ver_idx;
    td_u8 ver[3];     /* image.hupg 3 characters */
    td_u32 year : 6;  /* Customer version number-year */
    td_u32 month : 4; /* Customer version number-month */
    td_u32 day : 5;   /* Customer version number-day */
    td_u32 hour : 5;
    td_u32 minute : 6;
    td_u32 second : 6;
    td_u16 b_ver;            /* code+nv image.hupg */
    td_char manu_code[2];    /* Manufacturer code, 2 characters */
    td_u16 leave_cnt;        /* Number of offline times */
    td_u16 proxy_change_cnt; /* Number of proxy changes */
    td_u32 leave_total_time; /* Offline time */
    td_u16 hw_reset_cnt;     /* Record the accumulated number of device hard reset times. */
    td_u16 mac_sw_reset_cnt; /* records the accumulated times of MAC soft reset. */
} nfc_diag_cmd_query_network_topo_entry;

/* NDA channel topology report structure */
typedef struct {
    td_u16 rsp_idx;              /* Flow control channel response packet ID */
    td_u16 total_num : 10;       /* Total number of workstations to be reported */
    td_u16 is_last : 1;          /* Identifier of the last response packet of the current round of requests */
    td_u16 num : 5;              /* Number of sites contained in the current data packet */
    td_u32 cur_req_send_idx : 8; /* ID of the current request command in the flow control channel */
    td_u32 pad : 24;
    nfc_diag_cmd_query_network_topo_entry entry[MAC_DIAG_CMD_QUERY_NETWORK_TOPO_ENTRY_NUM];
} nfc_diag_query_network_topo_ind;


/* Interface for reporting topology information to the DBK */
typedef struct _diag_cmd_query_network_topo_stru_ {
    td_u16 sn;          /* Sequence number of each round of topology reporting. For a query command,
                         * the sequence numbers of multiple corresponding reported data packets are the same.
                         * The value varies depending on the request reporting cycle.
                         */
    td_u16 is_last : 1; /* Flag of the last packet reported in the current round: The value of the last packet is 1.
                         * The value of the non-last packet is 0.
                         */
    td_u16 num : 15;    /* Number of valid sites contained in the data packet */
    nfc_diag_cmd_query_network_topo_entry entry[DFX_NETWORK_TOPO_ENTRY_MAX];
} diag_cmd_query_network_topo;
typedef struct {
    td_u16 current_num;
    td_u16 reserved;
    td_u16 sn;
    td_u16 total_num : 10;
    td_u16 is_last : 1;
    td_u16 num : 5;
    nfc_diag_cmd_query_network_topo_entry ast_entry[0];
} mac_ndc_pkfc_network_topo_ind;
typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN];
    td_u16 pad;
    td_u32 proxy_change_count;
    td_u32 offline_count;
} diag_cmd_query_all_sta_info_entry_stru;
typedef struct _diag_cmd_query_all_sta_info_stru_ {
    td_u16 sn;
    td_u16 is_last;
    td_u32 num;
    diag_cmd_query_all_sta_info_entry_stru entry[DFX_NETWORK_TOPO_ENTRY_MAX];
} diag_cmd_query_all_sta_info;

typedef struct {
    td_u16 tei;                         /* Site TEI */
    td_u16 proxy_tei;                   /* Proxy site TEI */
    td_u16 childsum;                    /* Number of subnodes */
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN];     /* MAC address */
    td_u8 level;                        /* Level */
    td_u8 ability : 3;                  /* type -> sta, cco, proxy, unkwon */
    td_u8 state : 5;                    /* working state */
    td_u8 heartbeat_count;              /* Number of heartbeats. The heartbeat_count is determined within the heartbeat
                                         * detection week. If there is only one heartbeat, the heartbeat is not offline.
                                         */
    td_u8 sta_phase[3];                 /* 3 phases */
    td_bool new_joiner;                 /* Indicates whether the site is the site newly added in this round.
                                         * The heartbeat detection is different from that of the old site.
                                         * The heartbeat detection offline processing is not performed.
                                         */
    td_u8 product_type;                 /* Product type. For details, see EXT_ND_TYPE_CCO, EXT_ND_TYPE_RELAY */
    td_u8 not_rcv_heartbeat_continuous; /* Number of detection periods during which heartbeat packets are
                                           not received. */
    td_bool in_use;                     /* Flag indicating whether the site is online */
    td_bool is_relay_ability;           /* Trunk flag */
    td_u8 d2d_proxy_comm_rate;          /* path communication rate */
    td_u8 proxy_type;                   /* Proxy type */
    td_u8 freq_mode_present;            /* Current frequency band number */
    td_u8 nm_mmv;                       /* Network management packet version number */
    td_u8 down_comm_rate;               /* Downlink communication success rate */
    td_u8 up_comm_rate;                 /* Uplink communication success rate */
    td_u8 is_support_ared_recognition : 1; /* Whether to support transformer district identification */
    td_u8 is_dual_module : 1;              /* Dual-Mode Module */
    td_u8 network_mode : 1;                /* Network mode */
    td_u8 pad : 5;
    td_u8 is_notify_leave;      /* Indicates whether to notify the offline status. */
    td_u8 notify_leave_cnt;     /* Number of notifications */
    ext_sys_status_info sta_ver; /* Version information */
    td_u32 addtime;             /* Time when the site is added to the network */
    td_u32 assoc_random;        /* Random number of the association request.
                                 * For details, see the service background description. -2
                                 */
    td_u32 proxy_change_cnt;    /* Number of proxy changes */
    td_u32 leave_cnt;           /* Number of offline times */
    td_u32 leave_time;          /* Offline time */
    td_u32 leave_total_time;    /* Total offline duration */
    td_u16 hw_reset_cnt;        /* Record the accumulated number of device hard reset times. */
    td_u16 mac_sw_reset_cnt;    /* records the accumulated times of MAC soft reset. */
    td_u8 leave_reason;         /* Offline reason */
    td_u8 hw_reset_reason;      /* Cause of the hard restart */
    td_u8 sw_reset_reason;      /* Soft restart cause */
    td_u8 pad1;
    td_u32 last_heartbeat_time; /* Last heartbeat time */
    td_u8 management_id[24];    /* management_id is of 24 bytes */
} diag_cmd_query_choose_topoinfo;
typedef struct {
    td_u32 tei;
} diag_cmd_query_choose_req_para;

/* Simplify the topology structure-site information (12 bytes) */
typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN]; /* station MAC address */
    td_u16 tei : 12;                /* TEI address of the site */
    td_u16 ability : 4;             /* Site role */
    td_u16 level : 4;               /* Site level */
    td_u16 proxy_tei : 12;          /* TEI address of the parent proxy of the site */
    td_u8 product_type : 3;         /* Product form */
    td_u8 in_use : 1;               /* Whether the workstation is online */
    td_u8 ver_index : 4;            /* Version number index */
    td_u8 sta_phase0 : 2;           /* Site phase information */
    td_u8 sta_phase1 : 2;           /* Site phase information */
    td_u8 sta_phase2 : 2;           /* Site phase information */
    td_u8 proxy_type : 2;           /* Trunk type (reserved) */
} diag_cmd_network_topo_entry_sm_stru;


typedef struct {
    td_u16 sn;          /* Sequence number of each round of topology reporting. For a query command,
                         * the sequence numbers of multiple corresponding reported data packets are the same.
                         * The value varies depending on the request reporting cycle.
                         */
    td_u16 is_last : 1; /* Flag of the last packet reported in the current round: The value of the last packet is 1.
                         * The value of the non-last packet is 0.
                         */
    td_u16 num : 15;    /* Number of valid sites contained in the data packet */
    diag_cmd_network_topo_entry_sm_stru entry[DFX_TOPO_STA_INFO_SM_MAX];
} diag_cmd_network_topo_entry;
typedef struct {
    td_u8 num;
    td_u8 pad[3]; /* Reserved 3 bytes */
    ext_sys_status_info ver[DFX_STA_VER_INFO_SM_MAX];
} diag_cmd_topo_ver;

typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN];
    td_u16 tei;
} ext_dmid_mac_get_tei_from_topo;
#endif