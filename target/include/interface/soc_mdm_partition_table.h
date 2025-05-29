/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Flash partition APIs
 * @li The flash partition APIs are classified into flash partition management APIs and flash partition upgrade APIs.
 * @li The kernel running partition and upgrade backup partition need to be considered as a whole. For details, see
 * the DW21 V100 API Development Guide.
 * @li Flash partition management: The flash partition API is called to obtain the address and size of the flash
 * memory used by the service.
 * @li Flash upgrade: The flash replacement API is called to replace the flash partition.
 * @li Flash upgrade: The FlashBoot partition and factory NV working partition cannot be adjusted. The sizes of these
 * partitions cannot be modified.
 * @li Flash upgrade: The locations of the FlashBoot partition, factory NV working partition, kernel running
 * partition, and upgrade backup partition cannot be adjusted. The location of these partitions cannot be
 * modified, and the information about the moved partition items should not contain this information.
 * @li Flash upgrade complicates the upgrade process and increases the upgrade time. Therefore, you are advised to
 * carefully design the partitions in the beginning instead of relying on the partition upgrade function.
 * Author: CompanyName
 * Create: 2018-10-31
 */

/**
 * @defgroup hct_flash_partiton Flash partition management
 * @ingroup system
 */

#ifndef __SOC_MDM_PARTITION_TABLE_H__
#define __SOC_MDM_PARTITION_TABLE_H__

#include <soc_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup hct_flash_partiton
 * Total number of partitions
 */
#define EXT_FLASH_PARTITON_MAX 12 /**< Corresponding to  #ext_flash_partition_table_id */

/**
 * @ingroup hct_flash_partiton
 * Partition move starting from low address.
 */
#define EXT_PARTITION_TB_BOTTOM 0

/**
 * @ingroup hct_flash_partiton
 * Partition move starting from high address.
 */
#define EXT_PARTITION_TB_TOP 1

/**
 * @ingroup hct_flash_partiton
 * Partition move starting from low/high address determined automatically, used in multi-version scenarios.
 */
#define EXT_PARTITION_TB_AUTO 2

/**
 * @ingroup hct_flash_partiton
 * Partition entry ID.
 */
typedef enum {
    EXT_FLASH_PARTITON_BOOT = 0,   /**< Boot partition ID */
    EXT_FLASH_PARTITON_FACTORY_NV, /**< Factory NV working partition ID */
    EXT_FLASH_PARTITON_NV_BACKUP,  /**< Common NV backup partition ID */
    EXT_FLASH_PARTITON_KERNEL,     /**< Kernel running partition ID */

    EXT_FLASH_PARTITON_UPG_BACKUP,  /**< Upgrade backup partition ID */
    EXT_FLASH_PARTITON_NV_FILE,     /**< NV working partition ID */
    EXT_FLASH_PARTITON_CRASH_INFO,  /**< Crash log partition */
    EXT_FLASH_PARTITON_USER_INFO_1, /**< User partition 1 */

    EXT_FLASH_PARTITON_USER_INFO_2, /**< User partition 2 */
    EXT_FLASH_PARTITON_USER_INFO_3, /**< User partition 3 */
    EXT_FLASH_PARTITON_USER_INFO_4, /**< User partition 4 */
    EXT_FLASH_PARTITON_USER_INFO_5, /**< User partition 5 */
} ext_flash_partition_table_id;

/**
 * @ingroup hct_flash_partiton
 * Partition version information.
 */
typedef struct {
    td_u32 boot_support : 1; /**< Whether to support the replacement of the partition table in boot */
    td_u32 reserve : 10;     /**< Reserved */
    td_u32 ver_num : 5;      /**< Partition version.
                              * Function 1: Indicates the current partition version.
                              * Function 2: Determines whether the partition can be upgraded.
                              */
    td_u32 bitmap : 16;      /**< Version compatibility table.
                              * Function 1: Determines whether the partition can be upgraded.
                              */
} ext_partition_ver_info;

/**
 * @ingroup hct_flash_partiton
 * Log information about the partition status switchover.
 */
typedef struct {
    td_u32 finished_block : 12; /**< Number of moved blocks */
    td_u32 reserve : 12;        /**< Reserved */
    td_u32 id : 5;              /**< Partition ID */
    td_u32 move : 1;            /**< Whether to continue moving */
    td_u32 top1_or_bottom0 : 2; /**< Move starting from low address or high address. A possible value is
                                 * #EXT_PARTITION_TB_BOTTOM.
                                 */
} ext_pt_change_status;

/**
 * @ingroup hct_flash_partiton
 * Partition move control information.
 */
typedef struct {
    ext_partition_ver_info ver_support; /**< This version is compared during partition upgrade. If the version number
                                        * is greater than the current partition version number and the value of bitmap
                                        * in this field is not 0, the upgrade is allowed. */
    td_u32 temp_area_blk : 12;         /**< Log storage temporary area. */
    td_u32 total_num : 5;              /**< Number of moved items. */
    td_u32 reserve : 14;               /**< Reserved. */
    td_u32 status : 1;                 /**< Partition move status, internally controlled. */
} ext_pt_change_ctrl;

/**
 * @ingroup hct_flash_partiton
 * Partition move control details.
 */
typedef struct {
    td_u8 id : 5;              /**< Partition ID. */
    td_u8 reserve : 1;         /**< Reserved. */
    td_u8 top1_or_bottom0 : 2; /**< Move starting from high address or low address. */
} ext_pt_change_detail;

/**
 * @ingroup hct_flash_partiton
 * Log information about the flash partition replacement.
 */
typedef struct {
    ext_pt_change_ctrl ctrl;                           /**< Partition move control structure. */
    ext_pt_change_status table[EXT_FLASH_PARTITON_MAX]; /**< Partition table move log information. */
} ext_pt_change_log;

/**
 * @ingroup hct_flash_partiton
 * Flash partition management
 */
typedef struct {
    td_u32 addr_blk : 12; /**< Number of blocks corresponding to the low address of the flash partitions (unit: 4 KB).
                           */
    td_u32 size_blk : 12; /**< Number of blocks corresponding to the size of the flash partitions (unit: 4 KB). */
    td_u32 pad : 7;       /**< Reserved. */
    td_u32 dir : 1;       /**< Storage direction of the flash partitions 0: regular 1: reversed. */
} ext_flash_partition_info;

/**
 * @ingroup hct_flash_partiton
 * Flash partition table
 */
typedef struct {
    ext_partition_ver_info ver;                            /**< Flash partition version information. */
    ext_flash_partition_info table[EXT_FLASH_PARTITON_MAX]; /**< Flash partition entry information. */
    td_u32 reserve[2];                                    /**< Reserved.2 */
} ext_flash_partition_table;

/**
 * @ingroup hct_flash_partiton
 * Flash partition upgrade information
 */
typedef struct {
    ext_flash_partition_table new_info;  /**< New partition information. The partition can be upgraded only when the new
                                         * partition information is different from the old partition information. */
    ext_pt_change_ctrl change_ctrl_info; /**< Move control information. The partition upgrade can be performed only when
                                         * the version number in the partition control information is greater than the
                                         * current version number and the AND operation between the version
                                         * compatibility bit_map and the current version bitmap is not 0. */
    ext_pt_change_detail change_item[0]; /**< Moved item information. During the upgrade and replacement of the flash
                                         * partition, the move is performed in ascending order of the array subscripts.
                                         */
} ext_flash_partition_chage_info;

/**
 * @ingroup hct_flash_partiton
 * Partition version comparison result.
 */
typedef enum {
    EXT_PARTITION_VER_THE_SAME = 0, /**< Same as the local version. */
    EXT_PARTITION_VER_NEW,          /**< Later than the local version. */
    EXT_PARTITION_VER_ERR,          /**< The comparison is incorrect or the comparison is not supported. */
} ext_partition_ver;

/**
 * @ingroup  hct_flash_partiton
 * @brief  Initializes the flash partition table.
 *
 * @par Description:
 * Initializes the flash partition table.
 *
 * @attention None
 * @param   None
 *
 * @retval #0               Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_partition_table.h: Describes flash partition APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
td_u32 uapi_flash_partition_init(td_void);

/**
 * @ingroup  hct_flash_partiton
 * @brief  Obtains the flash partition table.
 *
 * @par Description:
 * Obtains the flash partition table.
 *
 * @attention None
 * @param  None
 *
 * @retval #ext_flash_partition_table*  Pointer to partition table information
 * @par Dependency:
 * @li soc_partition_table.h: Describes flash partition APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
ext_flash_partition_table *uapi_get_partition_table(td_void);

/**
 * @ingroup  hct_flash_partiton
 * @brief  Obtains the version of the current partition.
 *
 * @par Description:
 * Obtains the version of the current partition.
 *
 * @attention None
 * @param  ver [IN] Type #ext_partition_ver_info* Structure of the partition version information.
 *
 * @retval #0               Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_partition_table.h: Describes flash partition APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
td_u32 uapi_flash_partition_get_ver(ext_partition_ver_info *ver);

/**
 * @ingroup  hct_flash_partiton
 * @brief  Checks whether the partition can be replaced.
 *
 * @par Description:
 * Checks whether the partition can be replaced.
 *
 * @attention None
 * @param  ver [IN] Type #ext_partition_ver_info* Version information structure of the partition to be replaced.
 *
 * @retval #ext_partition_ver Comparison result.
 * @par Dependency:
 * @li soc_partition_table.h: Describes flash partition APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
ext_partition_ver uapi_flash_partition_check_ver(EXT_CONST ext_partition_ver_info *ver);

/**
 * @ingroup  hct_flash_partiton
 * @brief  Configures flash partition replacement.
 *
 * @par Description:
 * @li This API is used to configure the move sequence and new flash partition information.
 * @li When the first reset is performed after the API is called, the flash partition module moves the flash
 * partitions according to the move sequence configured by the user and logs the information about the new
 * flash partition to the flash memory.
 *
 * @attention
 * @li Before using this API, the flash partition module must be initialized by calling uapi_flash_partition_init.
 * @li The API does not check the partitioning reasonableness, which needs to be ensured by the user. For example:
 * If the common NV working partition is set to 4 KB, the board cannot start normally, but the flash partition
 * module does not check this.
 * @li The API does not check the partition move reasonableness, which needs to be ensured by the user. For example:
 * To move information A from partition 1 to partition 2, information B from partition 2 to partition 3,
 * information B should be moved before information A. Otherwise, information B will be overwritten.
 *
 * @param  change_info [IN] Type #ext_flash_partition_chage_info* Partition information (including new partition
 * information, partition move control information, and moved partition item information).
 * @param  size        [IN] Type #td_u16 Length of the partition information (unit: byte).
 *
 * @retval #0               Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_partition_table.h: Describes flash partition APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
td_u32 uapi_flash_partition_config(ext_flash_partition_chage_info *change_info, td_u16 size);

#ifdef __cplusplus
}
#endif
#endif /* __SOC_MDM_PARTITION_TABLE_H__ */
