/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: 1901.1 hplc protocol, which is only used for DBK.
 * Author: CompanyName
 * Create: 2021-03-17
 */

#ifndef __DFX_DBK_MPDU_STRUCT_DEF_H__
#define __DFX_DBK_MPDU_STRUCT_DEF_H__
#include <soc_types.h>

// SACK
typedef struct {
    td_u32 dt : 3;
    td_u32 network_type : 5;
    td_u32 nid : 24;

    td_u32 result : 4; // Variant begin
    td_u32 stei : 12;
    td_u32 dtei : 12;
    td_u32 received_pb_cnt : 3;
    td_u32 rsvd1 : 1;

    td_u32 channel_quality : 8;
    td_u32 station_load : 8;
    td_u32 rsvd2 : 16;

    td_u32 extend_frame_type : 4; // Variant end

    td_u32 ver : 4;
    td_u32 fccs : 24;
} MPDU_TYPE_SACK_FC_19011;

// Coordinatinon Frame
typedef struct {
    td_u32 dt : 3;
    td_u32 network_type : 5;
    td_u32 nid : 24;

    td_u16 duration; // Variant begin
    td_u16 next_bandwidth_start_offset;
    td_u32 nnid : 24;
    td_u32 rsvd1 : 8;

    td_u32 rsvd2 : 4; // Variant end

    td_u32 ver : 4;
    td_u32 fccs : 24;
} MPDU_TYPE_COD_FC_19011;

// RTS/CTS
typedef struct {
    td_u32 dt : 3;
    td_u32 network_type : 5;
    td_u32 nid : 24;

    td_u32 stei : 12; // Variant begin
    td_u32 dtei : 12;
    td_u32 lid : 8;

    td_u32 mnbf : 1;
    td_u32 rtsf : 1;
    td_u32 encryption_key_select : 2;
    td_u32 dur : 12;
    td_u32 rsvd1 : 16;

    td_u32 rsvd2 : 4; // Variant end

    td_u32 ver : 4;
    td_u32 fccs : 24;
} MPDU_TYPE_RTS_CTS_FC_19011;

// Beacon Frame
typedef struct {
    td_u32 dt : 3;
    td_u32 network_type : 5;
    td_u32 nid : 24;

    td_u32 bts; // Variant begin

    td_u32 stei : 12;
    td_u32 tmi : 4;
    td_u32 phase_line : 2;
    td_u32 pss : 4;
    td_u32 rsvd1 : 10;

    td_u32 rsvd2 : 4; // Variant end

    td_u32 ver : 4;
    td_u32 fccs : 24;
} MPDU_TYPE_BEACON_FC_19011;

// SOF Frame
typedef struct {
    td_u32 dt : 3;
    td_u32 network_type : 5;
    td_u32 nid : 24;

    td_u32 stei : 12; // Variant begin
    td_u32 dtei : 12;
    td_u32 lid : 8;

    td_u32 frame_len : 12;
    td_u32 pbcnt : 4;
    td_u32 broadcast_flag : 1;
    td_u32 resend_flag : 1;
    td_u32 encryption_key_select : 2;
    td_u32 tmi : 4;
    td_u32 extend_tmi : 1;
    td_u32 pss : 4;
    td_u32 rsvd1 : 3;

    td_u32 rsvd2 : 4; // Variant end

    td_u32 ver : 4;
    td_u32 fccs : 24;
} MPDU_TYPE_SOF_FC_19011;

/* beacon_pb start   1901beacon */
const int BEACON_PB_FIXED_LEN = 20;

/* Beacon frame payload(fixed) */
typedef struct {
    td_u8 beacon_type : 3;
    td_u8 network_flag : 1;
    td_u8 rsvd1 : 2;
    td_u8 start_associate : 1;
    td_u8 beacon_evaluation_flag : 1;

    td_u8 form_sequence;
    td_u8 cco_mac[6]; /* CCO mac address, 6 count */

    td_u32 bpc;
    td_u32 rsvd2[2]; /* Reserved, 2 count */
} MPDU_TYPE_BEACON_PB_FIXED_19011;

/* Beacon: STA capability list */
const int STATION_INFO_ENTRY_LEN = 13;

typedef struct {
    td_u32 tei : 12;
    td_u32 proxy_tei : 12;
    td_u32 path_min_commun_succ_rate : 8;

    td_u8 mac[6]; /* Send beacon sta mac address, 6 count */
    td_u8 role : 4;
    td_u8 level : 4;
    td_u8 pco_channel_quality;

    td_u8 phase : 2;
    td_u8 rsvd2 : 6;
    td_u8 rsvd3[3]; /* different with paper, reserved: 3 count */
} STATION_INFO_ENTRY_19011;

/* Beacon: Route parameter list */
const int ROUTE_PARAM_ENTRY_LEN = 8;
typedef struct {
    td_u16 route_period;
    td_u16 assessmet_remain_time;
    td_u16 pco_discovery_list_send_interval;
    td_u16 sta_discover_send_interval;
} ROUTE_PARAM_19011;

/* Beacon: Frequency change list */
const int FREQ_NOTIFY_ENTRY_LEN = 5;
typedef struct {
    td_u32 dest_freq; /* different with paper */
    td_u32 freq_change_time;
} FREQ_NOTIFY_19011;

/* Beacon: Timeslot allocation list */
const int BEACON_SLOT_FIXED_LEN = 8;
const int BEACON_SLOT_ENTRY_INFO_LEN = 20;
typedef struct {
    td_u8 non_center_beacon_slot_cnt;
    td_u8 central_beacon_slot_cnt : 4;
    td_u8 csma_slot_phase_cnt : 2;
    td_u8 rsvd1_l : 2;
    td_u8 rsvd1_h;
    td_u8 proxy_beacon_slot_cnt;

    td_u8 beacon_slot_len;
    td_u8 csma_slot_len;
    td_u8 bind_csma_slot_phase_count;
    td_u8 bind_csma_slot_lid;
} BEACON_SLOT_FIXED_INFO_19011;

#define MAX_UNKNOW_BEACON_BUFFER_LEN 200
typedef struct {
    BEACON_SLOT_FIXED_INFO_19011 beacon_slot_fixed;

    td_u16 tdma_slot_len; /* different with paper */
    td_u16 tdma_slot_lid; /* different with paper */

    td_u32 beacon_start_ntb;
    td_u32 beacon_period_len;

    td_u8 data[MAX_UNKNOW_BEACON_BUFFER_LEN];
} BEACON_SLOT_INFO_19011;

typedef struct {
    td_u8 rsvd3; /* different with paper */

    td_u8 station_entry_cnt;
    td_u8 station_entry_head;               /* head */
    td_u8 station_entry_len;                /* length */
    STATION_INFO_ENTRY_19011 station_entry; /* STA capability list */

    td_u32 route_para_notify_head : 16;        /* head */
    td_u32 route_para_notify_len : 16;         /* length */
    ROUTE_PARAM_19011 route_para_notify_entry; /* Route parameter list */

    td_u32 freq_head : 16; /* head */
    td_u32 freq_len : 16;  /* length */
    FREQ_NOTIFY_19011 freq_entry;

    td_u32 beacon_slot_head : 16;             /* head */
    td_u32 beacon_slot_len : 16;              /* length */
    BEACON_SLOT_INFO_19011 beacon_slot_entry; /* Frequency change list */

    td_u32 unknow_head : 16; /* head */
    td_u32 unknow_len : 16;  /* length */
    td_u8 unknow_buffer[MAX_UNKNOW_BEACON_BUFFER_LEN];
} MPDU_TYPE_BEACON_STATION_ENTRY_19011;

typedef struct {
    MPDU_TYPE_BEACON_PB_FIXED_19011 beacon_pb_fixed;
    MPDU_TYPE_BEACON_STATION_ENTRY_19011 beacon_station_entry;
} MPDU_TYPE_BEACON_PB_19011;

/* Mesh SACK */
typedef struct {
    td_u8 dt : 3;
    td_u8 network_type : 5;
    td_u8 result : 4;
    td_u8 received_pb_count : 3;
    td_u8 rsvd1 : 1;
    td_u8 dmac[6]; /* mac address, 6 count */

    td_u32 channel_quality : 8;
    td_u32 station_load : 8;
    td_u32 channel_attenuation : 8;
    td_u32 rsvd2 : 8;

    td_u32 extend_frame_type : 4;
    td_u32 ver : 4;
    td_u32 fccs : 24;
} MPDU_TYPE_MESH_SACK_FC_19011;

/* beacon_pb end */

#endif /* __DFX_DBK_MPDU_STRUCT_DEF_H__ */

