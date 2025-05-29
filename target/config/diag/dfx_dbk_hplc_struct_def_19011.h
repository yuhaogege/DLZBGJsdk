/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: 1901.1 hplc protocol, which is only used for DBK.
 * Author: CompanyName
 * Create: 2012-08-11
 */

#ifndef DFX_DBK_HPLC_STRUCT_DEF_19011
#define DFX_DBK_HPLC_STRUCT_DEF_19011
#include <soc_types.h>
#include <dfx_sys.h>

EXT_START_HEADER
#pragma once

#include "dfx_dbk_mpdu_struct_def.h"
#include "dfx_dbk_mme_struct_def.h"

/* MAC Frame Head (fixed part) */
const int MAC_FRAME_HEAD_FIXED_LEN_19011 = 8;
typedef struct {
    td_u32 ver : 4;
    td_u32 send_type : 4;
    td_u32 maximum_retransmission_cnt : 5;
    td_u32 rsvd1 : 3;
    td_u32 msdu_index : 16;

    td_u32 msdu_type : 8;
    td_u32 msdu_len : 11;
    td_u32 reset_cnt : 4;
    td_u32 mac_frame_type : 1;
    td_u32 rsvd2 : 8;
} MAC_FRAME_HEAD_FIXED_19011;

typedef struct {
    td_u32 ostei : 12;
    td_u32 odtei : 12;
    td_u32 hop_cnt : 4;
    td_u32 route_remaining_hops : 4;

    td_u32 proxy_main_path_flag : 1;
    td_u32 broadcast_direction : 2;
    td_u32 mac_exist_flag : 1;
    td_u32 rsvd1 : 4;
    td_u32 form_sequence : 8;
    td_u32 rsvd2 : 16;
} MAC_FRAME_HEAD_RELAY_VARIANT_19011;

typedef struct {
    td_u8 os_mac[6]; /* os mac address: 6 count */
    td_u8 od_mac[6]; /* od mac address: 6 count */

    td_u32 total_hop_cnt : 4;
    td_u32 remain_hop_cnt : 4;
    td_u32 pad : 24;

    td_u8 send_mac[6];
    td_u8 rcv_mac[6];
} mf_head_mesh_route_1901;

typedef struct {
    MAC_FRAME_HEAD_FIXED_19011 head_fixed;
    MAC_FRAME_HEAD_RELAY_VARIANT_19011 relay_variant;
    td_u8 src_mac[6]; /* src mac address: 6 count */
    td_u8 dst_mac[6]; /* dst mac address: 6 count */
} MAC_FRAME_HEAD_19011;

typedef struct {
    MAC_FRAME_HEAD_FIXED_19011 head_fixed;
    mf_head_mesh_route_1901 mesh_variant;
} MAC_MESH_HEAD_19011;

/* management packet header */
typedef struct {
    td_u16 mmType;
    td_u16 rsvd;
} MANAGE_NEWS_HEAD_19011;

typedef struct {
    td_u32 datagram_port : 8;
    td_u32 datagram_ID : 16;
    td_u32 datagram_ctrl : 8;
} APP_HEAD_19011;

typedef struct {
    MAC_FRAME_HEAD_FIXED_19011 macFrameHead;
    APP_HEAD_19011 appHead;
} MAC_APP_FRAME_HEAD;

typedef struct {
    td_u8 data[1]; /* data, 1 bytes */
} APP_INFO_19011;

/* 抄表报文下行帧信息 */
typedef struct {
    td_u16 app_ver : 6;
    td_u16 frame_head_len : 6;
    td_u16 rsvd : 4;
    td_u16 transdata_type : 4;
    td_u16 transdata_len : 12;

    td_u16 datagram_sequenc;
    td_u8  dev_timeout;
    td_u8  option;

    td_u8 data[1]; /* data, 1 bytes */
} APP_METERREAD_DOWN_INFO;

/* 抄表报文上行帧信息 */
typedef struct {
    td_u16 app_ver : 6;
    td_u16 frame_head_len : 6;
    td_u16 response_state : 4;
    td_u16 transdata_type : 4;
    td_u16 transdata_len : 12;

    td_u16 datagram_sequenc;
    td_u16 option;

    td_u8 data[1]; /* data, 1 bytes */
} APP_METERREAD_UP_INFO;

/* 启动节点注册下行报文 */
typedef struct {
    td_u16 app_ver : 6;
    td_u16 frame_head_len : 6;
    td_u16 force_resp_flag : 1;
    td_u16 nodereg_para : 3;
    td_u16 rsvd;

    td_u32 datagram_sequenc;
} APP_START_NODE_REG_DOWN;

/* 查询节点注册下行报文 */
typedef struct {
    td_u16 app_ver : 6;
    td_u16 frame_head_len : 6;
    td_u16 force_resp_flag : 1;
    td_u16 nodereg_para : 3;
    td_u16 rsvd;

    td_u32 datagram_sequenc;

    td_u8 src_mac[6];  /* source mac, 6 bytes */
    td_u8 dest_mac[6]; /* dest mac, 6 bytes */
} APP_QUERY_NODE_REG_DOWN;

/* 查询节点注册上行报文 */
typedef struct {
    td_u16 app_ver : 6;
    td_u16 frame_head_len : 6;
    td_u16 force_resp_flag : 1;
    td_u16 nodereg_para : 3;
    td_u8 elecmeter_cnt;
    td_u8 product_type;

    td_u8 dev_add[6]; /* device address, 6 bytes */
    td_u8 dev_id[6];  /* device id, 6 bytes */

    td_u32 datagram_sequenc;
    td_u32 rsvd;

    td_u8 src_mac[6];  /* src mac, 6 bytes */
    td_u8 dest_mac[6]; /* dest mac, 6 bytes */

    td_u8 data[1]; /* data, 1 bytes */
} APP_QUERY_NODE_REG_UP;

/* 停止节点注册下行报文 */
typedef struct {
    td_u16 app_ver : 6;
    td_u16 frame_head_len : 6;
    td_u16 rsvd : 4;
    td_u16 rsvd1;

    td_u32 datagram_sequenc;
} APP_STOP_NODE_REG_DOWN;

/* 校时下行报文 */
typedef struct {
    td_u16 app_ver : 6;
    td_u16 frame_head_len : 6;
    td_u16 pad0 : 4;
    td_u16 pad1 : 4;
    td_u16 data_len : 12;
    td_u8 data[1]; /* data, 1 bytes */
} APP_CHECK_TIME_DOWN;

/* 事件报文 */
typedef struct {
    td_u32 app_ver : 6;
    td_u32 frame_head_len : 6;
    td_u32 direction : 1;
    td_u32 start : 1;
    td_u32 function : 6;
    td_u32 transdata_len : 12;

    td_u8 datagram_sequenc[2]; /* datagram sequence, 2 bytes */
    td_u8 elecmeter_add[6];    /* elecmeter address, 6 bytes */
    td_u8 data[1];             /* data, 1 bytes */
} APP_EVENT_INFO;

/* 通信测试命令报文 */
typedef struct {
    td_u16 app_ver : 6;
    td_u16 frame_head_len : 6;
    td_u16 rsvd_testmode : 4;
    td_u16 transdata_type : 4;
    td_u16 transdatalen_testmodetime : 12;
    td_u8 data[1]; /* data, 1 bytes */
} APP_COMMUNICATE_TEST_CMD;

/* 确认/否认报文 */
typedef struct {
    td_u16 app_ver : 6;
    td_u16 frame_head_len : 6;
    td_u16 direction : 1;
    td_u16 confirm : 1;
    td_u16 rsvd : 2;
    td_u16 datagram_sequenc;
    td_u16 option;
} APP_CONFIRM_OR_NOT;

/* 开始升级下行报文 */
typedef struct {
    td_u32 app_ver : 6;
    td_u32 frame_head_len : 6;
    td_u32 rsvd : 20;

    td_u32 upgrade_id;

    td_u16 upgrade_timewin;
    td_u16 block_size;

    td_u32 file_size;
    td_u32 crc;
} APP_START_UPGRADE_DOWN;

/* 停止升级下行报文 */
typedef struct {
    td_u32 app_ver : 6;
    td_u32 frame_head_len : 6;
    td_u32 rsvd : 20;

    td_u32 upgrade_id;
} APP_STOP_UPGRADE_DOWN;

/* 传输文件数据下行报文 */
typedef struct {
    td_u16 app_ver : 6;
    td_u16 frame_head_len : 6;
    td_u16 rsvd : 4;
    td_u16 block_size;

    td_u32 upgrade_id;
    td_u32 data_num;

    td_u8 data[1]; /* data, 1 bytes */
} APP_TRANS_FILEDATA_DOWN;

/* 查询站点升级状态下行报文 */
typedef struct {
    td_u16 app_ver : 6;
    td_u16 frame_head_len : 6;
    td_u16 rsvd : 4;
    td_u16 block_cnt;

    td_u32 start_block_num;
    td_u32 upgrade_id;
} APP_QUERY_STATION_UPGRADESTATE_DOWN;

/* 执行升级下行报文 */
typedef struct {
    td_u16 app_ver : 6;
    td_u16 frame_head_len : 6;
    td_u16 rsvd : 4;
    td_u16 wait_reset_time;

    td_u32 upgrade_id;
    td_u32 test_runtime;
} APP_EXE_UPGRADE_DOWN;

/* 查询站点信息下行报文 */
typedef struct {
    td_u32 app_ver : 6;
    td_u32 frame_head_len : 6;
    td_u32 rsvd : 12;
    td_u32 list_elem_cnt : 8;

    td_u8 data[1]; /* data, 1 bytes */
} APP_QUERY_STATION_INFO_DOWN;

/* 开始升级上行报文 */
typedef struct {
    td_u32 app_ver : 6;
    td_u32 frame_head_len : 6;
    td_u32 rsvd : 12;
    td_u32 result : 8;
    td_u32 upgrade_id;
} APP_START_UPGRADE_UP;

/* 查询站点升级状态上行报文 */
typedef struct {
    td_u16 app_ver : 6;
    td_u16 frame_head_len : 6;
    td_u16 upgrade_state : 4;
    td_u16 valid_block_cnt;

    td_u32 start_block_num;
    td_u32 upgrade_id;

    td_u8 data[1]; /* data, 1 bytes */
} APP_QUERY_STATION_UPGRADESTATE_UP;

/* 查询站点信息上行报文 */
typedef struct {
    td_u32 app_ver : 6;
    td_u32 frame_head_len : 6;
    td_u32 rsvd : 12;
    td_u32 list_elem_cnt : 8;

    td_u32 upgrade_id;
    td_u8 data[1]; /* data, 1 bytes */
} APP_QUERY_STATION_INFO_UP;

/* 台区户变关系识别报文 */
typedef struct {
    td_u16 app_ver : 6;
    td_u16 frame_head_len : 6;
    td_u16 direction : 1;
    td_u16 start : 1;
    td_u16 collect_phase : 2;
    td_u16 datagram_sequenc;
    td_u8 mac[6]; /* mac address, 6 bytes */
    td_u8 feature_type;
    td_u8 collect_type;
    td_u8 data[1]; /* data, 1 bytes */
} APP_FAMILY_CHANGE_RELATION_ID;

/* 鉴权安全报文 */
typedef struct {
    td_u8 data[1]; /* data, 1 bytes */
} APP_AUTHENTICATION_SECURITY;

/* 查询ID信息 */
typedef struct {
    td_u16 app_ver : 6;
    td_u16 frame_head_len : 6;
    td_u16 direction : 1;
    td_u16 id_type : 3;
    td_u16 pak_sequenc;
} APP_QUERY_ID_INFO_DOWN;

typedef struct {
    td_u16 app_ver : 6;
    td_u16 frame_head_len : 6;
    td_u16 direction : 1;
    td_u16 id_type : 3;
    td_u16 pak_sequenc;
    td_u8 id_len_n;
    td_u8 id_info[1]; /* data, 1 bytes */
} APP_QUERY_ID_INFO_UP;

EXT_END_HEADER
#endif /* DFX_DBK_HPLC_STRUCT_DEF_19011 */