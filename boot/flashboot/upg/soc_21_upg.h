/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: 21 upg
 * Author: CompanyName
 * Create: 2018-10-31
 */
#ifndef __SOC_21_UPG_H__
#define __SOC_21_UPG_H__
#include <dw21_platform.h>
#include <soc_boot_partition_table.h>

/* ******************upgrade file format describtion******************************** */
#define EXT_UPG_HEAD_CUR_MAX_SECTION_COUNT 2   /* *< maximum count of upgrade file sub partition \
                                               * which boot need  analisis */
#define EXT_UPG_HEAD_CHECK_SUM_START_OFFSET 36 /* *< check size of upgrade file head */
#define EXT_UPG_HEAD_CHECK_SUM_SIZE  32
#define EXT_UPG_NEW_PROCESS_MAGIC 0xA509B84E
#define EXT_UPG_DO_UPG_MAGIC 0xDE72

/* *
 * @ingroup UPG
 *
 * upgrade file type
 */
typedef enum {
    EXT_21_UPG_TYPE_NONE = 0,   /* *< reserved: file type */
    EXT_21_UPG_TYPE_KERNEL = 1, /* *< kernel upgrade file */
    EXT_21_UPG_TYPE_APP,        /* *< reserved */
    EXT_21_UPG_TYPE_BOOT,       /* *< BOOT upgrade file */
    EXT_21_UPG_TYPE_PARTITION,  /* *< upgrade file of flash partition list */
} ext_21_upg_type;              /* *< upgrade file type */

/* *
 * @ingroup UPG
 *
 * upgrade file:  secure head check info
 */
typedef struct {
    td_u32 hash_alg : 16; /* *< 0:PKCS1_PSS  1:PKCS1_v1_5 */
    td_u32 sign_alg : 6;
    td_u32 sign_param : 10;
    td_u8 signature_1[256]; /* length 256 */
    td_u8 signature_2[256]; /* length 256 */
    td_u8 key_n_2[256];     /* length 256 */
} ext_upg_secure_info;

/* *
 * @ingroup UPG
 *
 * upgrade file: sub section description
 */
typedef struct {
    td_u32 attribute_compress : 1; /* *< 0: uncompressing  1:compressed */
    td_u32 attribute_pad : 31;     /* *< reserved */
    td_u32 offset;                 /* *< sub section address shifts according to hupg starting address */
    td_u32 compress_size;          /* *< size of hupg part occupied by sub section */
} ext_21_upg_head_section;

/* *
 * @ingroup UPG
 *
 * upgrade file: product ID check info
 */
typedef struct {
    td_u16 min_id;
    td_u16 max_id;
} ext_21_upg_head_product;

/* *
 * @ingroup UPG
 *
 * upgrade file: file head description
 */
typedef struct {
    /* *< HASH value, using for check integrity. check  (file_len-32) bytes, check position start from file_len */
    td_u32 chip_magic;    /* *< chip magic byte, using for discriminating incompatible chips.
                           * magic byte is the same if 21V100 and 21xxx are compatible */
    td_u8 check_sum[32];  /* (file_len-32) */
    td_u32 file_len;      /* *< upgrade file length */
    td_u32 ver_magic;     /* *< version magic byte, execute upgrade when magic byte of
                           * running version is different from upgrade file */
    td_u32 attribute_pad; /* *< upgrade file performance, reserved */

    td_u16 manufactuer_code; /* *< manufacturer magic byte. using for discriminating different manufacturer */
    td_u8 struct_type;       /* *< upgrade file struct version.
                              * using this region for partition when upgrade struct change a lot */
    td_u8 file_type;         /* *< upgrade file type, value is from EXT_21_UPG_TYPE_E */
    /* Use the file when product ID is within the range of the 2 groups of product ID */
    ext_21_upg_head_product product_list[2]; /* *< 2 groups of product ID check region. */

    td_u8 section_count; /* *< upgrade file consists of several sub section */
    td_u8 pad[3];        /* *< reserved 3B */

    td_u32 section_list_offset; /* *< head array position in upgrade file shifts size to file head */
    td_u32 section_list_size;   /* *< head array space usage size */

    td_u32 secure_info_offset; /* *< secure-check info position in upgrade file shifts to file head.
                                * 0-without file head */
    td_u32 secure_info_size;   /* *< secure-check info position in upgrade file shifts to file head.
                                * 0-without file head */

    td_u32 partition_info_offset; /* *< partition replace info position in upgrade file shifts to file head.
                                   * 0-without partition replace info. */
    td_u32 partition_info_size;   /* *< partition replace info position in upgrade file shifts to file head.
                                   * 0-without partition replace info. */

    ext_partition_ver_info partion_ver; /* *< partition version, using for identifying
                                        * if upgrade file can use for upgrading in current partition */

    /* other optional info
     * 1.sub section description info list
     * 2.partition info
     * 3.secure head
     * 4.sub section content
     * 5.file tail uapi_21_upg_tail_s,
     * using this region for searching file head if upgrade file aligned to back boundary
     */
} ext_21_upg_head;

/* *
 * @ingroup UPG
 *
 * upgrade file: file tail description info
 */
typedef struct {
    td_u32 mode_magic;
    td_u16 pad;
    td_u16 upg_magic[3];       /* 3*16B */
    td_u32 head_before_offset; /* *< file length, using this region for searching file head position */
    td_u32 head_end_magic;     /* *< magic byte of file tail */
} ext_21_upg_tail_new;

/* *********************************start configurate info description****************************************** */
#define EXT_TBL_HEAD_CHECH_SUM_START_OFFSET 4

typedef struct {
    td_u32 offset;    /* *< relative migration to TBL */
    td_u32 size;      /* *< size */
    td_u32 check_sum; /* *< CRC check value */
} ext_start_tbl_section;

/* *
 * @ingroup UPG
 *
 * start info list locate at kernel image head, using for guide boot to conduct kernel
 */
typedef struct {
    td_u32 check_sum; /* *< kernel integrity check, check position from tbl_len for (tbl_len-4) bytes */
    td_u32 tbl_len;   /* *< start info list length */
    td_u32 ver_magic; /* *< magic byte of present kernel version */
    /* *< whether or not to check  original nv file, enter upgrading process if check failed */
    td_u32 attribute_check_nvfile : 1;
    td_u32 attribute_check_nvfile_ver : 1; /* *< whether or not to check magic byte of original nv file and kernel .
                                            * enter upgrading process if different */
    td_u32 attribute_pad_1 : 30;           /* *< reserved */

    td_u8 tbl_type;                  /* *< tbl struct version */
    td_u8 pad[3];                    /* *< reserved 3B */
    td_u32 info_2_kernel;            /* *< kernel+tbl  total size. Calculate the maximum size for
                                      * save upgrading file according to this size, after kernel start-up */
    td_u32 secure_info_offset;       /* *< relative offset of secure check info position in TBL to file head,
                                      * 0-without secure head */
    td_u32 secure_info_size;         /* *< relative offset of secure check info position in TBL to file head,
                                      * 0-without secure head */
    ext_start_tbl_section section[1]; /* *< kernel image info description */
} ext_start_tbl;

/* ****reason for entering upgrading process in BOOT********* */
typedef enum {
    EXT_START_UPG_REASON_NONE,             /* *< not upgrading */
    EXT_START_UPG_REASON_TBL_ERR,          /* *< upgrade reason: TBL check fail */
    EXT_START_UPG_REASON_TBL_CODE1_ERR,    /* *< upgrade reason: kernel check failed */
    EXT_START_UPG_REASON_TBL_CODE2_ERR,    /* *< reserved */
    EXT_START_UPG_REASON_TBL_NV_ERR,       /* *< original nv file incorrect */
    EXT_START_UPG_REASON_TBL_NV_NOT_MATCH, /* *< magic byte of original nv file version not match to kernel image */
    EXT_START_UPG_REASON_UPG,              /* *< upgrading: upgrade file is correct and
                                           * magic byte of upgrade file version is equal to runing version */
} ext_start_upg_reason;

#endif
