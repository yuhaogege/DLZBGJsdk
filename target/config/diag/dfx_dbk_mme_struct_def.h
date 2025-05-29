/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: 1901.1 hplc protocol, which is only used for DBK. mme module.
 * Author: CompanyName
 * Create: 2021-03-17
 */

#ifndef __DFX_DBK_MME_STRUCT_DEF_H__
#define __DFX_DBK_MME_STRUCT_DEF_H__

#include <soc_types.h>

/* managemet packet start */
/* MMeAssocReq */
typedef struct {
    td_u8 mac[6]; /* STA MAC address: 6 count */
    td_u8 candidate_proxy_tei0_l;
    td_u8 candidate_proxy_tei0_h : 4;
    td_u8 rsvd0 : 4;

    td_u16 candidate_proxy_tei1 : 12;
    td_u16 rsvd1 : 4;
    td_u16 candidate_proxy_tei2 : 12;
    td_u16 rsvd2 : 4;

    td_u16 candidate_proxy_tei3 : 12;
    td_u16 rsvd3 : 4;
    td_u16 candidate_proxy_tei4 : 12;
    td_u16 rsvd4 : 4;

    td_u8 phase0 : 2;
    td_u8 phase1 : 2;
    td_u8 phase2 : 2;
    td_u8 rsvd5 : 2;
    td_u8 product_type;
    td_u8 mac_type;
    td_u8 rsvd6;

    td_u32 associate_random;

    /* td_u8 user_defined[18]; Manufacturer Information */
    td_u8 system_mode; /* Begin */
    td_u8 boot_ver_idx;
    td_u8 system_abnormal_reason;
    td_u8 main_ver[3];   /* main version : 3 count */
    td_u8 build_time[6]; /* bulid time : 6 count */

    td_u16 second_ver[2]; /* second version : 2 count */
    td_u16 chip_ver;      /* End */

    /* STA Version */
    td_u16 system_start_reason : 8; /* Begin */
    td_u16 boot_ver : 8;
    td_u16 sw_ver;
    td_u16 usYear : 7;
    td_u16 usMonth : 4;
    td_u16 usDay : 5;
    td_u8 manu_id[2]; /* manufacturer ID: 2 count */
    td_u8 chip_id[2]; /* chip id: 2 count */
                      /* END */

    td_u16 hard_reset_cnt;
    td_u16 soft_reset_cnt;
    td_u32 proxy_type : 8;
    td_u32 rsvd8 : 24;
    td_u32 end_sequence;
} MME_ASSOC_REQ_19011;

// MMeAssocCnf
typedef struct {
    td_u8 sta_mac[6]; /* STA MAC address: 6 count */
    td_u8 cco_mac[6]; /* CCO MAC address: 6 count */

    td_u16 result : 8;
    td_u16 level : 8;
    td_u16 tei : 12;
    td_u16 rsvd1 : 4;

    td_u16 proxy_tei : 12;
    td_u16 rsvd2 : 4;
    td_u16 total_packet_cnt : 8;
    td_u16 packet_index : 8;

    td_u32 associate_random;
    td_u32 reassociate_time;
    td_u32 end_sequence;
    td_u32 path_sequence;
    td_u32 rsvd3;

    /* Route Info */
    td_u16 straight_sta_num;
    td_u16 straight_pco_num;
    td_u16 table_size;
    td_u16 rsvd4;

    td_u8 router_table[1]; /* route table, variable, N 8-byte sets(straight tei(12b), rsvd(4b) */
} MME_ASSOC_CNF_19011;

/* MMeAssocGatherInd */
typedef struct {
    td_u8 result;
    td_u8 level;
    td_u8 cco_mac[6]; /* CCO MAC address */

    td_u16 proxy_tei : 12;
    td_u16 rsvd1 : 4;
    td_u16 rsvd2 : 8;
    td_u16 sta_num : 8;

    td_u32 rsvd3;

    td_u8 sta_info[1]; /* Variable, 1 count */
} MME_ASSOC_GATHER_IND_19011;

/* MMeChangeProxyReq */
typedef struct {
    td_u16 sta_tei : 12;
    td_u16 rsvd : 4;
    td_u16 new_proxy_tei0 : 12;
    td_u16 rsvd0 : 4;

    td_u16 new_proxy_tei1 : 12;
    td_u16 rsvd1 : 4;
    td_u16 new_proxy_tei2 : 12;
    td_u16 rsvd2 : 4;

    td_u16 new_proxy_tei3 : 12;
    td_u16 rsvd3 : 4;
    td_u16 new_proxy_tei4 : 12;
    td_u16 rsvd4 : 4;

    td_u16 old_proxy_tei : 12;
    td_u16 rsvd5 : 4;
    td_u16 proxy_type : 8;
    td_u16 reason : 8;

    td_u32 end_sequence;

    td_u8 sta_phase0 : 2;
    td_u8 sta_phase1 : 2;
    td_u8 sta_phase2 : 2;
    td_u8 rsvd6 : 2;
    td_u8 rsvd7[3]; /* reserved, 3 count */
} MME_CHANGE_PROXY_REQ_19011;

/* MMeChangeProxyCnf */
typedef struct {
    td_u8 result;
    td_u8 total_packet_num;
    td_u8 packet_index;
    td_u8 rsvd;

    td_u16 tei : 12;
    td_u16 rsvd1 : 4;
    td_u16 proxy_tei : 12;
    td_u16 rsvd2 : 4;

    td_u32 end_sequence;
    td_u32 path_sequence;

    td_u16 child_sum;
    td_u16 rsvd3;

    td_u8 child_entry[1]; /* Variable, 1 count */
} MME_CHANGE_PROXY_CNF_19011;

/* MMeChangeProxyBitMapCnf */
typedef struct {
    td_u8 result;
    td_u8 rsvd;
    td_u16 tei : 12;
    td_u16 rsvd1 : 4;

    td_u16 proxy_tei : 12;
    td_u16 rsvd2 : 4;
    td_u16 rsvd3;

    td_u32 end_sequence;
    td_u32 path_sequence;

    td_u16 rsvd4;
    td_u16 bitmap_size;

    td_u8 child_bitmap_entry[1]; /* Variable, 1 count */
} MME_CHANGE_PROXY_BITMAP_CNF_19011;

/* MMeLeaveInd */
typedef struct {
    td_u16 reason;
    td_u16 leave_sta_num;

    td_u16 leave_delay_time;
    td_u16 rsvd;

    td_u8 rsvd2[8]; /* reserved , 8 count */

    td_u8 sta_mac_entry[1]; /* Variable, 1 count */
} MME_LEAVE_IND_19011;

/* MMeHeartBeatCheck */
typedef struct {
    td_u16 ostei : 12;
    td_u16 rsvd : 4;
    td_u16 discover_count_tei : 12;
    td_u16 rsvd2 : 4;

    td_u16 discover_cnt;
    td_u16 discover_btm_cnt;

    td_u8 discover_btm[1]; /* Variable, 1 count */
} MME_HEART_BEAT_CHECK_19011;

/* MMeDiscoverNodeList */
typedef struct {
    td_u32 tei : 12;
    td_u32 proxy_tei : 12;
    td_u32 role : 4;
    td_u32 level : 4;

    td_u8 mac[6];     /* MAC address, 6 count */
    td_u8 cco_mac[6]; /* CCO MAC address, 6 count */

    td_u8 sta_phase1 : 2;
    td_u8 sta_phase2 : 2;
    td_u8 sta_phase3 : 2;
    td_u8 rsvd1 : 2;
    td_u8 proxy_channel_quality;
    td_u8 proxy_communication_rate;
    td_u8 proxy_down_communication_rate;

    td_u16 sta_num;
    td_u16 send_discovery_packet_cnt : 8;
    td_u16 up_route_entry_number : 8;

    td_u16 evaluate_begin_timeout;
    td_u16 discovery_sta_list_btm_size;

    td_u8 min_communication_rate;
    td_u8 rsvd2[3]; /* reserved2, 3 count */

    td_u8 up_route_info[1]; /* Variable, 1 count, Up Route Entry Info */
                            /* Discovery STA List Bitmap */
                            /* Received Discovery Information */
} MME_DISCOVER_NODE_LIST_19011;

/* MMeSuccessRateReport */
typedef struct {
    td_u16 tei : 12;
    td_u16 rsvd : 4;
    td_u16 sta_num;

    td_u8 success_rate_info[1]; /* Variable, 1 count */
} MME_SUCCESSRATE_REPORT_19011;

/* MMeNetworkConflictReport */
typedef struct {
    td_u8 cco_mac[6]; /* CCO MAC address */
    td_u8 neighbor_net_cnt;
    td_u8 nid_size;

    td_u8 neighborNetContent[1]; /* Variable, 1 count */
} MME_NETWORK_CONFLICT_REP_19011;

/* MMeZeroCrossNTBCollectionReq */
typedef struct {
    td_u16 tei : 12;
    td_u16 rsvd : 4;
    td_u8 collection_mode;
    td_u8 collection_period;
    td_u8 collection_quantity;
    td_u8 rsvd2[3];
} MME_ZEROCROSS_NTB_COLLECT_REQ_19011;

/* MMeZeroCrossNTBReport */
typedef struct {
    td_u16 tei : 12;
    td_u16 rsvd : 4;
    td_u8 total_cnt;
    td_u8 diff_ntb_cnt_phase1;

    td_u8 diff_ntb_cnt_phase2;
    td_u8 diff_ntb_cnt_phase3;
    td_u8 ntb_base[4]; /* ntb_base, 4 bytes */
    td_u8 ntb_deviation[1];
} MME_ZEROCROSS_NTB_REPORT_19011;

/* MMeLBDAtReq  NULL */

/* MMeLBDAtCnf */
typedef struct {
    td_u8 BSF;

    /* LBDAT */
    td_u16 bridge_tei : 12;
    td_u16 rsvd : 4;
    td_u8 bridge_mac[6]; /* Bridge MAC, 6 count */
    td_u8 bridge_destination_number;
    td_u8 ntb_deviation[1]; /* Variable , 1 count, n*BDA+BDPID */
} MME_LBD_AT_CNF_19011;

/* MMeSetKeyRequest */
typedef struct {
    td_u8 key_type;
    td_u16 protocol_run_number;
    td_u8 protocol_message_number;
    td_u32 my_nonce;
    td_u32 your_nonce;
    td_u8 new_key[8]; /* New Key, 8 count */
    td_u8 mac[6];     /* MAC, 6 count */
    td_u16 rsvd;
} MME_SET_KEY_REQ_19011;

/* MMeSetKeyConfirm */
typedef struct {
    td_u8 result;
    td_u16 protocol_run_number;
    td_u8 protocol_message_number;
    td_u32 my_nonce;
    td_u32 your_nonce;
    td_u32 rsvd;
} MME_SET_KEY_CNF_19011;

/* MMeGetKeyRequest */
typedef struct {
    td_u8 key_type;
    td_u16 protocol_run_number;
    td_u8 protocol_message_number;
    td_u32 my_nonce;
    td_u8 mac[4];  /* MAC, 4 count */
    td_u8 rsvd[4]; /* reserved, 4 count */
} MME_GET_KEY_REQ_19011;

/* MMeGetKeyConfirm */
typedef struct {
    td_u8 result;
    td_u8 requested_key_type;
    td_u8 protocol_run_number; /* question */
    td_u8 protocol_message_number[2]; /* protocol_message_number, 2 count */
    td_u8 my_nonce[4]; /* my_nonce, 4 count */
    td_u8 your_nonce[4]; /* your_nonce, 4 count */
    td_u8 rsvd[3]; /* reserved, 3 count */
} MME_GET_KEY_CNF_19011;


/* MMeAuthrzRequest */
typedef struct {
    td_u8 protocol_run_number;
    td_u16 protocol_message_number; /* question */
    td_u8  my_nonce[4]; /* my_nonce, 4 count */
    td_u8 mac[6];  /* MAC, 6 count */
    td_u8 rsvd[3]; /* reserved, 3 count */
} MME_AUTH_REQ_19011;

/* MMeAuthrzConfirm */
typedef struct {
    td_u8 result;
    td_u8 protocol_run_number;
    td_u16 protocol_message_number;
    td_u32 my_nonce;
    td_u32 your_nonce;
    td_u8 mac[6];  /* MAC, 6 count */
    td_u8 NMK[8];  /* NMK, 6 count */
    td_u8 rsvd[2]; /* reserved, 2 count */
} MME_AUTH_CNF_19011;

/* MMeEncryptedPlInd */
typedef struct {
    td_u8 payload_type;
    td_u8 PEKS;
    td_u8 protocol_run_number; /* question */
    td_u8 protocol_message_number[2]; /* protocol_message_number, 3 count */
    td_u16 length;
    td_u8 variable[1]; /* Variable, 1 count */
} MME_ENCRYPTED_PL_IND_19011;

/* MMeSetAmpMapReq */
typedef struct {
    td_u16 AmpLength;
    td_u8 variable[1]; /* Variable, 1 count */
} MME_SET_AMP_REQ_19011;

/* MMeSetAmpMapCnf */
typedef struct {
    td_u8 response;
} MME_SET_AMP_CNF_19011;

/* MMeNetworkDiagnosisReport */
typedef struct {
    td_u8 manufacturer_id[2]; /* Manufacturer Id, 2 count */
    td_u8 variable[1];        /* Variable, 1 count */
} MME_NETWORK_DIAGNOSIS_REPORT_19011;

typedef struct {
    td_u8 variable[1]; /* Variable, 1 count */
} MME_OTHER_SOF_MSDU_19011;

/* Dynamic route repair request packet */
typedef struct {
    td_u8 dmac[6];            /* destination, 6 bytes */
    td_u8 requset_sta_mac[6]; /* request sta mac, 6 bytes */

    td_u32 route_request_sequence;
    td_u32 des_sequence;

    td_u8 req_mode : 1;
    td_u8 unknow_sequence : 1;
    td_u8 des_reply : 1;
    td_u8 rsvd : 5;
    td_u8 hop_num;
    td_u16 rsvd;

    td_u8 pass_sta_sequence[1]; /* pass sta sequence variable, 1 bytes */
} MME_DYNAMIC_ROUTE_REPAIR_REQ_19011;

/* Dynamic route repair reply packet */
typedef struct {
    td_u8 dmac[6];    /* destination, 6 bytes */
    td_u8 req_mac[6]; /* request sta mac, 6 bytes */

    td_u32 des_sequence;

    td_u8 hop_num;
    td_u8 rsvd[3]; /* reservd 3 bytes */
} MME_DYNAMIC_ROUTE_REPAIR_CNF_19011;

/* MESH discivery list */
typedef struct {
    td_u8 send_sta_mac[6]; /* sta mac, 6 bytes */
    td_u16 discovery_list_num;

    td_u8 send_period;
    td_u8 rsvd[3]; /* reservd 3 bytes */

    td_u8 discovery_item[1];
} MME_NET_DISCOVERY_LIST_19011;

/* Route information packet */
typedef struct {
    td_u8 send_sta_mac[6]; /* sta mac, 6 bytes */
    td_u16 entry_num;

    td_u8 rt_entry[1]; /* route infomation entry variable, 1 bytes */
} MME_ROUTE_INFO_19011;

/* Route invalid inform packet */
typedef struct {
    td_u8 dmac[6]; /* destination, 6 bytes */
    td_u16 entry_num;

    td_u16 invalid_sequence;
    td_u16 rsvd;

    td_u8 invalid_rt[1]; /* invaliable mac inforamtion variable, 1 bytes */
} MME_ROUTE_INVALID_PKT_19011;

#endif /* __DFX_DBK_MME_STRUCT_DEF_H__ */

