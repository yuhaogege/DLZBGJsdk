/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Synchronous bsp/driversbsp/incbsp/system/inc directory soc_21_upg.h code
 * Author: CompanyName
 * Create: 2012-11-11
 */
#ifndef __SOC_21_UPG_H__
#define __SOC_21_UPG_H__
#include <dw21_platform.h>
#include <soc_ft_nv.h>
#include <soc_partition_table.h>

/* *********************************Upgrade file format description*********** ******************************* */
#define EXT_UPG_HEAD_CUR_MAX_SECTION_COUNT 2 /* The number of sections that the current boot can support for parsing */
#define EXT_UPG_HEAD_CHECK_SUM_START_OFFSET 36
#define EXT_UPG_NEW_PROCESS_MAGIC 0xA509B84E
#define EXT_UPG_DO_UPG_MAGIC 0xDE72

EXT_EXTERN EXT_CONST td_u32 g_ver_magic;

typedef enum {
    EXT_21_UPG_TYPE_NONE = 0,   /* Reserved */
    EXT_21_UPG_TYPE_KERNEL = 1, /* Kernel upgrade file */
    EXT_21_UPG_TYPE_APP,        /* Reserved */
    EXT_21_UPG_TYPE_BOOT,       /* BOOT upgrade file */
    EXT_21_UPG_TYPE_PARTITION,  /* partition table upgrade file */
} ext_21_upg_type;

#define SIGNATURE_SIZE 256
#define KEY_N_SIZE 256

typedef struct {
    td_u32 hash_alg : 16;              /* <hash algorithm 0x0: sha256, other: reserved. */
    td_u32 sign_alg : 6;               /* < rsa padding mode 0: rsa_pkcs1; 1: rsa_pss; other: reserved */
    td_u32 sign_param : 10;            /* < the signature algorithm: signature parameter, default '0'. */
    td_u8 signature_1[SIGNATURE_SIZE]; /* signature 1 for public key 2 stored in the upgrade file using key 1 */
    td_u8 signature_2[SIGNATURE_SIZE]; /* signature of the upgrade file content using key 2 2 */
    td_u8 key_n_2[KEY_N_SIZE];         /* the public key stored in the upgrade file 2 */
} ext_upg_secure_info;

typedef struct {
    td_u32 attribute_compress : 1; /* whether the subsection is lzma compressed 0: uncompressed 1: compressed */
    td_u32 attribute_pad : 31;     /* reserved */
    td_u32 offset;                 /* the offset of the subsection content from the start of the upgrade file */
    td_u32 compress_size;          /* subsection content size */
} ext_21_upg_head_section;

typedef struct {
    td_u16 min_id; /* product id range lower limit */
    td_u16 max_id; /* product id range upper limit */
} ext_21_upg_head_product;

#define CHECK_SUM_SIZE 32
#define PRODUCT_LIST_SIZE 2

typedef struct {
    td_u32 chip_magic;               /* chip version */
    td_u8 check_sum[CHECK_SUM_SIZE]; /* check field */
    td_u32 file_len;                 /* total file length */
    td_u32 ver_magic;                /* version magic word */
    td_u32 attribute_pad;            /* reserved */

    td_u16 manufactuer_code;                                  /* vendor code */
    td_u8 struct_type;                                        /* upgrade file structure version */
    td_u8 file_type;                                          /* upgrade file type, value ext_21_upg_type_e */
    ext_21_upg_head_product product_list[PRODUCT_LIST_SIZE]; /* product id range */

    td_u8 section_count; /* number of subsections */
    td_u8 pad[3];        /* reserved 3 bytes for alignment */

    td_u32 section_list_offset; /* subsection description information offset */
    td_u32 section_list_size;   /* subsection description information size */

    td_u32 secure_info_offset; /* security signature information offset */
    td_u32 secure_info_size;   /* security signature information size */

    td_u32 partition_info_offset; /* partition replacement information offset */
    td_u32 partition_info_size;   /* partition replacement information size */

    ext_partition_ver_info partion_ver; /* partition version */

    /* description: store ext_21_upg_section_info_s[n] here */
    /* description: here you can extend the head */
    /* description: store file data */
    /* description: store uapi_21_upg_tail_s */
} ext_21_upg_head; /* kernel upgrade file */

typedef struct {
    td_u32 head_before_offset; /* upgrade file length */
    td_u32 head_end_magic;     /* upgrade file trail magic word */
} ext_21_upg_tail;

typedef struct {
    td_u32 mode_magic;
    td_u16 pad;
    td_u16 upg_magic[3];       /* 3 devil numbers */
    td_u32 head_before_offset; /* *< file length, find the file header location through this field */
    td_u32 head_end_magic;     /* *< file end magic word */
} ext_21_upg_tail_new;

#define EXT_TBL_HEAD_CHECH_SUM_START_OFFSET 4

typedef struct {
    td_u32 offset;
    td_u32 size;
    td_u32 check_sum;
} ext_start_tbl_section; /* Start the section configuration in tbl */

typedef struct {
    td_u32 check_sum; /* start information table check value, [tbl_len member position, start information table end] */
    td_u32 tbl_len;   /* startup message table length */
    td_u32 ver_magic; /* version magic word */
    td_u32 attribute_check_nvfile : 1;     /* whether the nv file is correct as a prerequisite for version startup */
    td_u32 attribute_check_nvfile_ver : 1; /* whether the nv file version matches the kernel is necessary for startup */
    td_u32 attribute_pad_1 : 30;           /* reserved */
    td_u8 tbl_type;                        /* startup information table structure version */
    td_u8 pad[3];                          /* reserved 3 bytes for alignment */
    td_u32 info_2_kernel;
    td_u32 secure_info_offset; /* security header offset */
    td_u32 secure_info_size;   /* security header size */
    ext_start_tbl_section section[1];
} ext_start_tbl; /* start the table table */
/* Upgrade NV configuration */
/* BOOT enter the upgrade process reason */
typedef enum {
    EXT_START_UPG_REASON_NONE,
    EXT_START_UPG_REASON_TBL_ERR,
    EXT_START_UPG_REASON_TBL_CODE1_ERR,
    EXT_START_UPG_REASON_TBL_CODE2_ERR,
    EXT_START_UPG_REASON_TBL_NV_ERR,
    EXT_START_UPG_REASON_TBL_NV_NOT_MATCH,
    EXT_START_UPG_REASON_UPG,
} ext_start_upg_reason;
/* kernel upgrade interface */
td_u32 uapi_upg_nv_clear_mode(td_void);
td_u32 uapi_upg_get_start_upg_mode(td_u8 *start_upg_mode);
td_u32 uapi_upg_restart_get_file_info(td_u32 *flash_addr, td_u32 *flash_size, td_u32 *crc);
#endif
