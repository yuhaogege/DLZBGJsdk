/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Upgrade base funciton - start.
 * Author: CompanyName
 * Create: 2018-10-31
 */

#include "upg_base_start.h"
#include "upg_base_prv.h"
#include "upg_base_nv_mode.h"

#include <soc_ft_nv.h>
#include <soc_mdm_flash.h>
#include <soc_upg_api.h>
#include <soc_stdlib.h>
#include <trans_file_slave.h>

#ifdef __cplusplus
extern "C" {
#endif

ext_upg_kernel_ctrl g_upg_kernel_ctrl;
EXT_CONST td_u32 g_remap_offset = sizeof(ext_start_tbl) + sizeof(ext_upg_secure_info);

td_u32 uapi_upg_real_start_prv(td_u32 tail_flash_addr)
{
    td_u32 ret;
    ext_upg_file_base_info upg_info = { 0 };
    ext_upg_kernel_ctrl *upg_ctrl = ext_upg_kerclt_ctrl();

    ret = uapi_upg_check_upg_file(TD_TRUE, tail_flash_addr, &upg_info);
    if (ret != EXT_ERR_SUCCESS) {
        upg_ctrl->dfx_err_type |= EXT_UPG_BASE_DFX_ERR_CHECK_UPG_FILE_FAIL;
        return ret;
    }

    return uapi_upg_nv_set_val(upg_info.file_type, upg_info.flash_addr, upg_info.flash_size, upg_info.crc);
}

td_u32 uapi_upg_write_magic(td_u32 upg_head_address, td_u32 upg_tail_address)
{
    ext_21_upg_tail_new new_tail = {0};

    new_tail.upg_magic[0] = EXT_UPG_DO_UPG_MAGIC; /* set value to offset 0. */
    new_tail.upg_magic[1] = EXT_UPG_DO_UPG_MAGIC; /* set value to offset 1. */
    new_tail.upg_magic[2] = EXT_UPG_DO_UPG_MAGIC; /* set value to offset 2. */
    new_tail.mode_magic = EXT_UPG_NEW_PROCESS_MAGIC;
    new_tail.head_end_magic = PRODUCT_CFG_UPG_FILE_END_SIGNATURE;
    new_tail.head_before_offset = upg_tail_address - upg_head_address;

    return uapi_flash_write((td_u32)(upg_tail_address - sizeof(ext_21_upg_tail_new)), sizeof(new_tail),
        (td_u8 *)&new_tail, TD_FALSE);
}

td_u32 uapi_upg_real_start(td_u32 upg_head_address, td_u32 upg_tail_address)
{
    td_u32 ret;

    ret = uapi_upg_write_magic(upg_head_address, upg_tail_address);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    ret = uapi_upg_real_start_prv(upg_tail_address);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    return EXT_ERR_SUCCESS;
}

td_u32 uapi_upg_real_stop(td_void)
{
    return uapi_upg_nv_clear_mode();
}

td_u32 uapi_upg_get_boot_status(td_void)
{
    ext_21_upg_tail_new new_tail = {0};

    (td_void)uapi_flash_read((td_u32)(uapi_trans_get_partition_end_addr() - sizeof(ext_21_upg_tail_new)),
        sizeof(new_tail), (td_u8 *)&new_tail);

    for (td_u32 loop = 0; loop < ext_array_count(new_tail.upg_magic); loop++) {
        if (new_tail.upg_magic[loop] == EXT_UPG_DO_UPG_MAGIC) {
            return EXT_ERR_FAILURE;
        }
    }

    return EXT_ERR_SUCCESS;
}

#ifdef __cplusplus
}
#endif
