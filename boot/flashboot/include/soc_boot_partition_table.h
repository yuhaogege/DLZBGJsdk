/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: FLASH partition management
 */

/* *@defgroup hct_flash_partiton FLASH partition management
 * @ingroup system
 */
#ifndef __SOC_BOOT_PARTITION_TABLE_H__
#define __SOC_BOOT_PARTITION_TABLE_H__
#include <soc_types.h>
#include <boot_partition_table.h>
/* *
 * @ingroup hct_flash_partiton
 * details for part move control
 */
typedef struct {
    td_u8 id : 5;              /* *< partition ID */
    td_u8 reserve : 1;         /* *< reserved */
    td_u8 top1_or_bottom0 : 2; /* *< move start address(lower address OR higher address) */
} ext_pt_change_detail;

/* *
 * @ingroup  hct_flash_partiton
 * @brief  get Flash partition list
 *
 * @par description:
 * get Flash partition list
 *
 * @attention :nothing
 * @param :nothing
 *
 * @retval #ext_flash_partition_table_s  poiniter of partition list info
 * @par dependency:
 * @li soc_partition_table.h: using the file for describing interfaces about flash partition
 * @see :nothing
 * @since DW21_V100R001C00
 */
ext_flash_partition_table *uapi_get_partition_table(td_void);

#endif
