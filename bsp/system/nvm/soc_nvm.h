/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: nv implementation modification: implemented as 2 ping-pong,
 * and divided into factory area, and ordinary nv area soc_nvm.h code
 * Author: CompanyName
 * Create: 2012-08-26
 */
#ifndef __SOC_NVM_H__
#define __SOC_NVM_H__

#include <soc_types.h>

#define HNV_FILE_SIGNATURE uapi_make_identifier('H', 'N', 'V', '$')
#define FNV_FILE_SIGNATURE uapi_make_identifier('F', 'N', 'V', '#')

#define HNV_BLOCK_SIZE 0x1000 /* Valid data maximum */
#define HNV_CRC_SIZE (HNV_BLOCK_SIZE - 8)

#define HNV_FAULT_TOLERANT_TIMES 3 /* Fault tolerance */

#define HNV_MANAGE_FIXED_LEN 24 /* As of reserve */

/* Factory area NV structure needs to be exactly the same as under the kernel. It cannot be modified after fixing */
#define KEEP_ID_RANGE_SIZE 2

typedef enum {
    EXT_NV_READ_OPERATE,
    EXT_NV_WRITE_OPERATE,
    EXT_NV_OPERATE_MAX_TYPE
} ext_nv_operate_type;

typedef struct {
    td_u8 nv_type;
    td_u8 start_id; /* including */
    td_u8 end_id; /* excluding */
    td_u8 pad;
} ext_nv_allocte_info;
typedef struct {
    td_u8 nv_type;  /* ext_nv_type */
    td_u8 nv_operate;  /* ext_nv_operate_type */
    td_bool notify_enable;
    td_u8 pad;
} ext_nv_pipeline_ctrl;
typedef struct _ext_nvm_manage_s_ {
    td_u32 magic;      /* nv head magic word */
    td_u32 crc;        /* nv management area and data area crc32 from: ver; stop: end of flash_size */
    td_u8 ver;         /* nv management area structure version number */
    td_u8 head_len;    /* length of nv header, from: magic; stop: end of reserve, and 4 bytes backward aligned */
    td_u16 total_num;  /* total number of nv */
    td_u32 seq;        /* write the nv sequence number flag (the upgrade is still retained to calculate the lifetime) */
    td_u32 ver_magic;  /* version magic word, matching the kernel version magic word */
    td_u32 flash_size; /* the size of the flash occupied by nv, such as 4096 (4k), 65536 (64k) */
    td_u8 keep_id_range[KEEP_ID_RANGE_SIZE]; /* upgrade reserved id range, 0: id lower boundary 1: id upper boundary */
    td_u8 reserve[2];                        /* reserved 2 bytes for alignment */
    td_u8 nv_item_data[0];                   /* index table */
} ext_nv_manage;

typedef struct ext_nv_item_index_s_ {
    td_u8 nv_id;
    td_u8 nv_len;     /* nv actual length, not counting crc32 value, crc32 next to the store */
    td_u16 nv_offset; /* offset from this nv area */
} ext_nv_item_index;

typedef struct _ext_nv_ctrl_s_ {
    td_u32 current_addr;
    td_u32 next_addr;
    td_u32 seq;
    td_u32 sem_handle;

    td_u8 init_flag;
    td_u8 reserve;
    td_u16 total_num;
    td_u32 ver_magic;
    ext_nv_item_index *index;
} ext_nv_ctrl;

#endif // __SOC_NVM_H__