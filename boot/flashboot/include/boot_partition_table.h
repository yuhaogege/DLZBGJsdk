/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: boot partition
 * Author: CompanyName
 * Create: 2018-10-31
 */

#ifndef __BOOT_PARTITION_TABLE_H__
#define __BOOT_PARTITION_TABLE_H__

#include <soc_fnv.h>
#include <soc_boot_rom.h>
#include <soc_boot_err.h>
#include <soc_flashboot_flash.h>
#include <soc_flash_boot.h>

#define partition_id(i) (i)  /* according to partition list. input index, output ID */
#define array_index(id) (id) /* according to partition list. input ID, output index */
#define is_user_id(i) ((i) & (1 << 6))

#define REFRESH_IDLE 0
#define REFRESH_BUSY 1

#define EXT_MOD_ID_PARTITION 1 /* debug */
#define uapi_malloc(id, size) boot_malloc(size)
#define uapi_free(id, addr) boot_free(addr)
#define pt_debug1()
/* codex: need verify the scape of nv address */
#define NV_ADDR_VALUE_MAX (16 * 1024 * 1024)

/* *
 * @ingroup hct_flash_partiton
 *
 * total number of partition
 */
#define EXT_FLASH_PARTITON_MAX 12 /* *< count of part, correspondence with #ext_flash_partition_table_id */
#define BLOCK_SIZE (0x1000)
/* *
 * @ingroup hct_flash_partiton
 *
 * move partition start from lower address
 */
#define EXT_PARTITION_TB_BOTTOM (0)

/* *
 * @ingroup hct_flash_partiton
 *
 * move partition start from higher address
 */
#define EXT_PARTITION_TB_TOP (1)

/* *
 * @ingroup hct_flash_partiton
 *
 * move partition by automatic recognition according to address. expending for multiple version
 */
#define EXT_PARTITION_TB_AUTO (2)

/* *
 * @ingroup hct_flash_partiton
 *
 * partition option ID
 */
typedef enum {
    EXT_FLASH_PARTITON_BOOT = 0,   /* *< BOOT partiton ID */
    EXT_FLASH_PARTITON_FACTORY_NV, /* *< factory NV partition ID */
    EXT_FLASH_PARTITON_NV_BACKUP,  /* *< not factory NV backup partition ID */
    EXT_FLASH_PARTITON_KERNEL,     /* *< kernel partition ID */

    EXT_FLASH_PARTITON_UPG_BACKUP,  /* *< upgrade file compressed part ID */
    EXT_FLASH_PARTITON_NV_FILE,     /* *< NV working part ID */
    EXT_FLASH_PARTITON_CRASH_INFO,  /* *< strorage part for crashing */
    EXT_FLASH_PARTITON_USER_INFO_1, /* *< user part 1 */

    EXT_FLASH_PARTITON_USER_INFO_2, /* *< user part 2 */
    EXT_FLASH_PARTITON_USER_INFO_3, /* *< user part 3 */
    EXT_FLASH_PARTITON_USER_INFO_4, /* *< user part 4 */
    EXT_FLASH_PARTITON_USER_INFO_5, /* *< user part 5 */
} ext_flash_partition_table_id;

/* *
 * @ingroup hct_flash_partiton
 *
 * partition option ID
 */
typedef struct {
    td_u32 boot_support : 1; /* *< supportable for boot replace partition list */
    td_u32 reserve : 10;     /* *< reserved */
    td_u32 ver_num : 5;      /* *< version, iterate from smaller to lager version.
                              * each increment, the larger can replace the smaller */
    td_u32 map : 16;         /* *< version compatible list. use the method of bit&bit. be able to upgrade if not 0 */
} ext_partition_ver_info;

/* *
 * @ingroup hct_flash_partiton
 *
 * partition change status log
 */
typedef struct {
    td_u32 finished_block : 12; /* *< blocks already move finished */
    td_u32 reserve : 12;        /* *< reserved */
    td_u32 id : 5;              /* *< partition id */
    td_u32 move : 1;            /* *< flag of continue move */
    td_u32 top1_or_bottom0 : 2; /* *< flag of move start address(lower address OR higher address,
                                 * can fill with EXT_PARTITION_TB_BOTTOM,etc) */
} ext_pt_change_status;


/* *
 * @ingroup hct_flash_partiton
 *
 * partition move control info
 */
typedef struct {
    ext_partition_ver_info ver_support; /* *< version info, using for compare with local version */
    td_u32 temp_area_blk : 12;         /* *< log storage part temporarily */
    td_u32 total_num : 5;              /* *< part count to be moved */
    td_u32 reserve : 14;               /* *< reserved */
    td_u32 status : 1;                 /* *< part move status, controled by inner */
} ext_pt_change_ctrl;


/* *
 * @ingroup hct_flash_partiton
 *
 * log of Flash partition change
 */
typedef struct {
    ext_pt_change_ctrl ctrl;                           /* *< partition move control struct */
    ext_pt_change_status table[EXT_FLASH_PARTITON_MAX]; /* *< partition move log */
} ext_pt_change_log;

/* *
 * @ingroup hct_flash_partiton
 *
 * options of Flash partition list
 */
typedef struct {
    td_u32 addr_blk : 12; /* *< blocks of Flash partiton lower address(unit: 4KB) */
    td_u32 size_blk : 12; /* *< blocks of Flash partiton size(unit: 4KB) */
    td_u32 pad : 7;       /* *< reserved */
    td_u32 dir : 1;       /* *< direction of Flash partition storage. 0:positive order, 1:reverse order */
} ext_flash_partition_info;

/* *
 * @ingroup hct_flash_partiton
 *
 * Flash partition list
 */
typedef struct {
    ext_partition_ver_info ver;                            /* *< Flash partition version */
    ext_flash_partition_info table[EXT_FLASH_PARTITON_MAX]; /* *< description of Flash partition list */
    td_u32 reserve[2];                                    /* *< reserved 2 */
} ext_flash_partition_table;

/* *
 * @ingroup hct_flash_partiton
 *
 * partition version compare result
 */
typedef enum {
    EXT_PARTITION_VER_THE_SAME = 0, /* *< the version is the same with local version */
    EXT_PARTITION_VER_NEW,          /* *< the version is newer than local version */
    EXT_PARTITION_VER_ERR,          /* *< compare error OR unsupport */
} ext_partition_ver;

/* *
 * @ingroup  hct_flash_partiton
 * @brief  Flash partition list initialization
 *
 * @par description:
 * Flash partition list initialization
 *
 * @attention :nothing
 * @param :nothing
 *
 * @retval #0   success
 * @retval #not 0 ,fail.  see soc_boot_err.h for more details
 * @par dependency:
 * @li soc_partition_table.h: using the file for describing interfaces about flash partition
 * @see :nothing
 * @since DW21_V100R001C00
 */
td_u32 uapi_flash_partition_init(td_void);


/* *
 * @ingroup  hct_flash_partiton
 * @brief  get current part version
 *
 * @par description:
 * get current part version
 *
 * @attention :nothing
 * @param  ver             [IN] type #ext_partition_ver_s, struct of partition version
 *
 * @retval #0   success
 * @retval #not 0 ,fail.  see soc_boot_err.h for more details
 * @par dependency:
 * @li soc_partition_table.h: using the file for describing interfaces about flash partition
 * @see :nothing
 * @since DW21_V100R001C00
 */
td_u32 uapi_flash_partition_get_ver(ext_partition_ver_info *ver);
/* *
 * @ingroup  hct_flash_partiton
 * @brief  check whether the part can be replaced
 *
 * @par description:
 * check whether the part can be replaced
 *
 * @attention :nothing
 * @param  ver             [IN] type #ext_partition_ver_s, struct of partition version to be replaced
 *
 * @retval #ext_partition_ver_e  compare the result
 * @par dependency:
 * @li soc_partition_table.h: using the file for describing interfaces about flash partition
 * @see :nothing
 * @since DW21_V100R001C00
 */
ext_partition_ver uapi_flash_partition_check_ver(const ext_partition_ver_info *ver);

/* *
 * @ingroup  hct_flash_partiton
 * @brief  refresh Flash partition list
 *
 * @par description:
 * move and refresh the flash partition according to partition info settings
 *
 * @attention: call uapi_flash_partition_init before using
 * @param :nothing
 *
 * @retval #0   success
 * @retval #not 0 ,fail.  see soc_boot_err.h for more details
 * @par dependency:
 * @li soc_partition_table.h: using the file for describing interfaces about flash partition
 * @see :nothing
 * @since DW21_V100R001C00
 */
td_u32 uapi_flash_partition_refresh(td_void);
#endif
