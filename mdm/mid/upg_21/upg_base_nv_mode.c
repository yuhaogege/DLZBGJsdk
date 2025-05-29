/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Upgrade base funciton - nv mode.
 */

#include "upg_base_nv_mode.h"
#include <soc_mdm_types.h>
#include <soc_ft_nv.h>
#include <soc_mdm_nv.h>
#include <soc_21_upg.h>
#include "upg_base_start.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    td_u32 start_flash_addr;
    td_u32 start_flash_size;
    td_u32 crc;
    td_u8 start_upg_mode;
    td_u8 pad[3]; /* pad 3 bytes */
} ext_upg_base_mode;

ext_upg_base_mode g_upg_base_mode = { 0, 0, 0, 0, { 0 } };

td_u32 uapi_upg_nv_init_mode(td_void)
{
    td_u32 ret;
    ext_upg_mode_nv upg_nv = { 0 };

    ret = uapi_nv_read(EXT_NV_FTM_UPG, &upg_nv, sizeof(upg_nv));
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    g_upg_base_mode.start_upg_mode = upg_nv.upg_mode;
    g_upg_base_mode.start_flash_addr = upg_nv.flash_addr;
    if ((upg_nv.do_upgrade == TD_TRUE) &&
        (uapi_upg_get_boot_status() != EXT_ERR_SUCCESS)) {
        g_upg_base_mode.start_flash_size = 0;
        g_upg_base_mode.crc = 0;
    } else {
        g_upg_base_mode.start_flash_size = upg_nv.flash_size;
        g_upg_base_mode.crc = upg_nv.crc;
    }

    return EXT_ERR_SUCCESS;
}

td_u32 uapi_upg_nv_clear_mode(td_void)
{
    td_u32 ret;
    ext_upg_mode_nv upg_nv = { 0 };

    ret = uapi_nv_read(EXT_NV_FTM_UPG, &upg_nv, sizeof(upg_nv));
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    upg_nv.upg_mode = EXT_21_UPG_TYPE_NONE;
    upg_nv.do_upgrade = 0;
    return uapi_nv_write(EXT_NV_FTM_UPG, &upg_nv, sizeof(upg_nv));
}

td_u32 uapi_upg_nv_set_val(td_u8 upg_mode, td_u32 flash_addr, td_u32 flash_size, td_u32 crc)
{
    td_u32 ret;
    ext_upg_mode_nv upg_nv = { 0 };

    ret = uapi_nv_read(EXT_NV_FTM_UPG, &upg_nv, sizeof(upg_nv));
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    upg_nv.upg_mode = upg_mode;
    upg_nv.do_upgrade = TD_TRUE;
    upg_nv.flash_addr = flash_addr;
    upg_nv.flash_size = flash_size;
    upg_nv.crc = crc;

    return uapi_nv_write(EXT_NV_FTM_UPG, &upg_nv, sizeof(upg_nv));
}

td_u32 uapi_upg_get_start_upg_mode(td_u8 *start_upg_mode)
{
    *start_upg_mode = g_upg_base_mode.start_upg_mode;
    return EXT_ERR_SUCCESS;
}

td_u32 uapi_upg_restart_get_file_info(td_u32 *flash_addr, td_u32 *flash_size, td_u32 *crc)
{
    *flash_addr = g_upg_base_mode.start_flash_addr;
    *flash_size = g_upg_base_mode.start_flash_size;
    *crc = g_upg_base_mode.crc;

    return EXT_ERR_SUCCESS;
}

#ifdef __cplusplus
}
#endif
