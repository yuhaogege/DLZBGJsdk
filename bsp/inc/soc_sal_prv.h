/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Adjust the directory structure to prepare for compiling the SDK. soc_sal_prv.h code
 * Author: CompanyName
 * Create: 2012-07-14
 */

#ifndef SOC_SAL_PRV_H
#define SOC_SAL_PRV_H

#include <soc_mdm_diag.h>
#include "soc_mdm_nv.h"
/* Function name: UAPI_SYS_FlashIsBadBlock */
/* Function description: Check if the specified number of Flash blocks are bad blocks (V1 version is not supported) */
/* Parameter Description: */
/* ulSectorSn [in] : Flash block number */
/* return value: */
/* TD_TRUE : indicates a bad block */
/* TD_FALSE : indicates no bad blocks */
EXT_EAPI td_bool UAPI_SYS_FlashIsBadBlock(td_u32 sector_address);
#if !defined(PRODUCT_CFG_FEATURE_UT)
EXT_EAPI td_u32 uapi_sys_control_task_timer_prv(td_u32 timer_id, td_u32 period, td_bool start, td_bool app_layer);
EXT_EAPI td_u32 UAPI_DIAG_ReportSysEventPrv(td_u16 sys_evt_id, td_u8 option);
EXT_EAPI td_u32 UAPI_DIAG_ReportSysCriticalEventPrv(td_u32 evt_id, td_u32 error);
EXT_EAPI td_u32 uapi_diag_report_buffer_by_hex_prv(td_u32 id, td_u8 *puc_buffer, td_u32 len, td_pcstr file,
    td_u32 line_no);
EXT_EAPI td_u32 uapi_diag_is_printf_enable_prv(td_u32 id);
#elif defined(PRODUCT_CFG_FEATURE_UT) && !defined(PRODUCT_CFG_HAVE_CUSTOM_UT_STUB)
EXT_PRVL td_u32 uapi_sys_control_task_timer_prv(td_u32 timer_id, td_u32 period, td_bool start, td_bool app_layer)
{
    return EXT_ERR_NOT_SUPPORT;
}
EXT_PRVL td_u32 uapi_nvm_write_prv(td_u16 id, EXT_CONST td_pvoid item_data, td_u16 item_data_len)
{
    return EXT_ERR_NOT_SUPPORT;
}
EXT_PRVL td_u32 uapi_nvm_get_item_length_prv(td_u16 id, EXT_OUT td_u16 *pus_item_length)
{
    return EXT_ERR_NOT_SUPPORT;
}
EXT_PRVL td_u32 uapi_nvm_read_prv(td_u16 id, EXT_OUT td_pvoid item_data, td_u16 item_data_len,
    td_bool if_fail_use_default)
{
    return EXT_ERR_NOT_SUPPORT;
}
EXT_PRVL td_u32 uapi_nvm_write_factory_prv(td_u16 id, EXT_CONST td_pvoid data, td_u16 data_len)
{
    return EXT_ERR_NOT_SUPPORT;
}
EXT_PRVL td_u32 uapi_nvm_register_changed_noitfy_proc_prv(td_u16 min_id, td_u16 max_id,
    ext_nvm_changed_notify_f changed)
{
    return EXT_ERR_NOT_SUPPORT;
}
EXT_PRVL td_u32 uapi_dms_set_forwarding_prv(td_u16 port_num, td_bool forwarding)
{
    return EXT_ERR_NOT_SUPPORT;
}
EXT_PRVL td_bool uapi_dms_is_forwarding_prv(td_u16 port_num)
{
    return TD_FALSE;
}
EXT_PRVL td_u32 uapi_diag_report_buffer_by_hex_prv(td_u32 id, td_u8 *puc_buffer, td_u32 len, td_pcstr file,
    td_u32 line_no)
{
    return EXT_ERR_NOT_SUPPORT;
}
EXT_PRVL td_u32 uapi_diag_is_printf_enable_prv(td_u32 id)
{
    return EXT_ERR_NOT_SUPPORT;
}
#endif
#include <soc_sal_nv.h>

/* **************************************************************************** */
#endif /* SOC_SAL_PRV_H */
