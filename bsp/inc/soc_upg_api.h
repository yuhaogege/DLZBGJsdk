/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Upgrade the underlying interface is not generic enough and
 * is no longer available as a generic secondary development interface.
 * Used only for internal use of the upgrade module. soc_upg_api.h code
 * Author: CompanyName
 * Create: 2012-12-12
 */
#ifndef __SOC_UPG_API_H__
#define __SOC_UPG_API_H__
#include <soc_types.h>
#include <soc_mdm_types.h>
#include <soc_21_upg.h>

typedef struct {
    td_u32 flash_addr; /* < the flash start address where the file is located */
    td_u32 flash_size; /* < file size */
    td_u32 crc;        /* < file crc */
    td_u8 file_type;   /* < file type, see ext_21_upg_type_e */
    td_u8 pad[3];      /* < reserved 3 bytes */
} ext_upg_file_base_info;

/* *
 * @ingroup upg
 * @brief Powers up the upgrade status.
 *
 * @par Description:
 * Power on initialization upgrade status.
 *
 * @attention None.
 *
 * @retval #0 Success.
 * @retval #!0 failed. See soc_errno.h for details.
 * @par Dependency:
 * @li soc_upg_api.h: The file is used to describe the upgrade interface.
 * @see None.
 * @since DW21_V100R001C00
 */
td_u32 uapi_upg_base_init(td_void);
/* *
 * @ingroup upg
 * @brief starts the upgrade.
 *
 * @par Description:
 * Start the upgrade, it is the record upgrade mode and upgrade address.
 *
 * @attention None.
 * @li The current upgrade mode does not affect the actual upgrade results,
 * and the upgrade mode record function is reserved only for possible extensions in the future.
 *
 * @retval #0 Success.
 * @retval #!0 failed. See soc_errno.h for details.
 * @par Dependency:
 * @li soc_upg_api.h: The file is used to describe the upgrade interface.
 * @see None.
 * @since DW21_V100R001C00
 */
td_u32 uapi_upg_real_start(td_u32 upg_head_address, td_u32 upg_tail_address);

/* *
 * @ingroup upg
 * @brief stop upgrading
 *
 * @par Description:
 * Stop the upgrade, actually clear the upgrade mode
 *
 * @attention None.
 * @li The current upgrade mode does not affect the actual upgrade results,
 * and the upgrade mode record function is reserved only for possible extensions in the future.
 *
 * @retval #0 Success.
 * @retval #!0failed. See soc_errno.h for details.
 * @par Dependency:
 * @li soc_upg_api.h: The file is used to describe the timer related interface.
 * @see None.
 * @since DW21_V100R001C00
 */
td_u32 uapi_upg_real_stop(td_void);

/* *
 * @ingroup upg
 * @brief verifies that the upgrade file header is correct.
 *
 * @par Description:
 * Verify the correctness of the upgrade file header
 *
 * @attention None.
 * @param upg_head: head pointer
 * @param head_size: Header size, when the header size is smaller than sizeof(uapi_21_upg_head_s),
 * only the contents of the head_size size are verified, and the uncontained part is processed correctly.
 *
 * @retval #0 Success.
 * @retval #!0failed. See soc_errno.h for details.
 * @par Dependency:
 * @li soc_upg_api.h: file is used to describe the upgrade related interface
 * @see None.
 * @since DW21_V100R001C00
 */
td_u32 uapi_upg_check_head(const ext_21_upg_head *upg_head, td_u32 head_size);

/* *
 * @ingroup upg
 * @brief verifies that the upgrade file header is correct.
 *
 * @par Description:
 * Verify that the upgrade file header is correct.
 *
 * @attention None.
 * @param end_addr [IN] Type #td_bool,TD_TRUE:flash_addr is the tail address
 * of the upgrade file. TD_FALSE: flash_addr is the upgrade file header address.
 * @param flahs_addr [IN] type #td_u32, the FLASH location where the upgrade file is located
 * @param upg_info [OUT] type #ext_upg_file_base_info_s, upgrade file basic information
 *
 * @retval #0 Success.
 * @retval #!0 failed. See soc_errno.h for details.
 * @par Dependency:
 * @li soc_upg_api.h: file is used to describe the upgrade related interface
 * @see None.
 * @since DW21_V100R001C00
 */
td_u32 uapi_upg_check_upg_file(td_bool end_addr, td_u32 flash_addr, ext_upg_file_base_info *upg_info);

#endif
