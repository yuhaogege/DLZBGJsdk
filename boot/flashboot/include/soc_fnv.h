/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: factory nv
 * Author: CompanyName
 * Create: 2018-10-31
 */
#ifndef __SOC_FNV_H__
#define __SOC_FNV_H__

#include <soc_types.h>

#define uapi_make_identifier(a, b, c, d) ext_make_u32(uapi_make_u16(a, b), uapi_make_u16(c, d))
#define HNV_FILE_SIGNATURE uapi_make_identifier('H', 'N', 'V', '$')
#define FNV_FILE_SIGNATURE uapi_make_identifier('F', 'N', 'V', '#')

#define HNV_BLOCK_SIZE (0x1000) /* maximum size of valid data */
#define HNV_CRC_SIZE (HNV_BLOCK_SIZE - 8)

#define HNV_FAULT_TOLERANT_TIMES (3) /* fault tolerant times */

#define HNV_MANAGE_FIXED_LEN (24) /* up to reserve */

#define HNV_ITEM_MAXLEN (256 - 4)

#define ext_struct_offset(s, m) ((td_u32)(uintptr_t) & (((s *)0)->m)) /* offset of struct element */

typedef enum _ext_nv_type_ {
    EXT_TYPE_NV = 0,
    EXT_TYPE_FACTORY_NV,
    EXT_TYPE_TEMP, /* temporary storage area */
    EXT_TYPE_NV_MAX,
} ext_nv_type;

/* factory nv struct need keep the same with kernel.  And can't change after fixation */
typedef struct _ext_nvm_manage_s_ {
    td_u32 magic;           /* magic byte of nv head */
    td_u32 crc;             /* crc32 of nv management part and data part : start from ver to flash_size */
    td_u8 ver;              /* struct version of nv management part */
    td_u8 head_len;         /* length of nv head : start from magic to reserve, And 4K aligne to backward */
    td_u16 total_num;       /* total count of nv */
    td_u32 seq;             /* write nv flag(reserved for upgrade to calc lifetime) */
    td_u32 ver_magic;       /* magic byte of version, match with kernel version */
    td_u32 flash_size;      /* falsh size occupied by NV. like 4096(4K),65536(64K) */
    td_u8 keep_id_range[2]; /* reserved ID for upgrade (2B), 0:lower boundary 1:upper boundary */
    td_u8 reserve[2];       /* reserved 2B */
    td_u8 nv_item_data[0];  /* index list */
} ext_nv_manage;

typedef struct ext_nv_item_index_s_ {
    td_u8 nv_id;
    td_u8 nv_len;     /* actual length of nv. not calc crc32. storage joint with crc32 */
    td_u16 nv_offset; /* relative offset to nv part */
} ext_nv_item_index;

typedef struct _ext_nv_ctrl_s_ {
    td_u32 current_addr;
    td_u32 next_addr;
    td_u32 seq;
    td_u32 sem_handle;

    td_u8 init_flag;
    td_u8 reserve;
    td_u16 total_num;
    ext_nv_item_index *index;
} ext_nv_ctrl;

#define EXT_NV_FTM_MANUFACOTR_ID 0x01 /* EXT_NV_FTM_MANUFACOTR_ID_S */
#define EXT_NV_FTM_PRODUCT_ID 0x03    /* EXT_NV_FTM_PRODUCT_ID_S */
#define EXT_NV_FTM_FLASH_PARTIRION_TABLE_ID (0x04)
#define EXT_NV_FTM_FLASH_PARTIRION_TABLE_B_ID (0x05) /* use partition list for changing address */
#define EXT_NV_FTM_FLASH_PARTIRION_CHANGE_ID (0x06)  /* use struct for changing address */
#define EXT_NV_FTM_UPG_EXTENSION_ID (0x07)

typedef struct {
    td_u32 product_type; /* *< product type */
    td_u8 plc_mac[6];    /* *< MAC address : 6B */
    td_u8 reserved[6];   /* reserved 6B */
} ext_nv_ftm_product_id;

typedef struct {
    td_u32 magic;
    td_u8 cur_times;
    td_u8 dst_times;
    td_u8 data[26]; /* data 26B */
} ext_nv_ftm_upg_extension;

td_u32 uapi_factory_nv_init(td_u32 flash_addr);
td_u32 uapi_factory_nv_read(td_u8 id, td_pvoid pdata, td_u8 len);
td_u32 uapi_factory_nv_write(td_u8 id, td_pvoid pdata, td_u8 len);
td_u32 uapi_nv_check_file(td_u32 flash_addr);
td_u32 uapi_nv_get_ver(td_u32 flash_addr, td_u32 *ver);

td_u32 uapi_temp_nv_init(td_u32 flash_addr, td_u32 backup_addr);
td_u32 uapi_common_nv_read(td_u8 id, td_pvoid pdata, td_u8 len, ext_nv_type nv_type);
td_u32 uapi_common_nv_write(td_u8 id, const td_pvoid pdata, td_u8 len, ext_nv_type nv_type);
#endif
