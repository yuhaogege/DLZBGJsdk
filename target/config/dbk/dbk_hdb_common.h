/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: DBK_HDB_Common
 * Author: CompanyName
 * Create: 2018-10-31
 */
#ifndef DBK_HBD_COMMON_H
#define DBK_HBD_COMMON_H

#define SAL_ALIGNED4

#include "soc_config.h"

typedef struct {
} null_stru;

typedef struct {
} dbk_null_stru;

/* Don't need to show ACK */
typedef struct {
} dbk_nack_null_stru;

typedef struct {
    char str[1];
} dbk_str_type;

typedef struct {
    long data[2]; /* 2 count */
} dbk_s32_array_2_stru;

typedef struct {
    long data[3]; /* 3 count */
} dbk_s32_array_3_stru;

typedef struct {
    long data[4]; /* 4 count */
} dbk_s32_array_4_stru;

typedef struct {
    long data[5]; /* 5 count */
} dbk_s32_array_5_stru;

typedef struct {
    long data[6]; /* 6 count */
} dbk_s32_array_6_stru;

typedef struct {
    td_u32 data[2]; /* 2 count */
} dbk_u32_array_2_stru;

typedef struct {
    unsigned long data[3]; /* 3 count */
} dbk_u32_array_3_stru;

typedef struct {
    td_u32 data[4]; /* 4 count */
} dbk_u32_array_4_stru;

typedef struct {
    char str[128]; /* 128 byte */
} dbk_file_type;

typedef struct {
    char str[1];
} dbk_dump_data_stru;

typedef struct {
    char str[1];
} dbk_dump_hex_stru;

typedef struct {
} dbk_file_type_stru;

typedef struct {
    td_u32 data;
} dbk_u32_type_stru;

typedef struct {
    td_u32 opt;
    td_u32 val;
} dbk_u32_2_type_stru;

typedef struct {
    td_u8 data[19]; /* 19 count */
} dbk_au8_19_stru;

typedef struct {
    td_u32 data[20]; /* 20 count */
} dbk_array20_stru;

typedef struct {
    td_u32 data[23]; /* 23 count */
} dbk_array23_stru;

typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN];
    td_u8 data[90]; /* 90 byte */
} dbk_md90_b_stru;

typedef struct {
    td_u8 mac[8]; /* 8 bytes */
} dbk_plc_sdm_mac_stru;

typedef struct {
    td_u32 id;
    td_u32 size;
    td_u8 data[128]; /* 128 byte */
} dbk_nv_req_stru;

typedef struct {
    td_u32 rc;
    td_u32 nv_id;
    td_u32 data_size;
    td_u8 data[128]; /* 128 byte */
} dbk_nv_ack_stru;

typedef struct {
    td_u32 addr;
    td_u32 total_size;
    td_u32 size;
    td_u32 sn;
} dbk_dump_bin_req_stru;
#endif
