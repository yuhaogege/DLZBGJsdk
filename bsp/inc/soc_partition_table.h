/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Split the FLASH partition interface. soc_partition_table.h code
 * Author: CompanyName
 * Create: 2012-12-25
 */

#ifndef SOC_PARTITION_TABLE_H
#define SOC_PARTITION_TABLE_H
#include <soc_types.h>
#include <soc_mdm_types.h>
#include <soc_mdm_partition_table.h>

/* *
 * @ingroup hct_flash
 *
 * Reset mode after partition replacement.
 */
typedef enum {
    EXT_PARTITON_SOFT_RESET = 0, /* <soft reset, that is, the internal partition information of each module is reset */
    EXT_PARTITON_HARD_RESET,     /* <hard reset, chip restart */
    EXT_PARTITON_NO_RESET,       /* <Do not reset, business control */
} ext_partition_reset;

/* *
 * @ingroup hct_flash
 * @brief Flash partition table moved reset.
 *
 * @par Description:
 * Move the flash partition according to the partition information configuration.
 *
 * @attention You need to call uapi_flash_partition_init before using it.
 * @param reset [IN] Type #ext_partition_reset_e, restart mode after partition information is updated.
 *
 * @retval #0 Success.
 * @retval #!0 failed. See soc_errno.h for details.
 * @par Dependency:
 * @li soc_partition_table.h: The file is used to describe the Flash partition related interface.
 * @see None.
 * @since DW21_V100R001C00
 */
td_u32 uapi_flash_partition_refresh(ext_partition_reset reset);

#endif
