/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Modify the drv directory, remove the first phase of the rom error,
 * delete the useless code to compile and adjust the dfx directory, remove the
 * count function driver part drv_dfx.c code
 * Author: CompanyName
 * Create: 2012-04-04
 */
#include "drv_dfx_cmn_inc.h"
#include "drv_dfx_os_info.h"
#include "drv_dfx_sal.h"
#include "drv_dfx_uart.h"

#include <diag_cmd_id.h>
#include <dfx_upg_base.h>
#include <dfx_diag_cmd_id.h>
#include <dfx_low_power.h>
#if defined PRODUCT_CFG_NDM_COLLECT_DATA
#include "drv_dfx_dc.h"
#endif
#ifdef PRODUCT_CFG_MEM_OPERATE
#include "drv_dfx_mem.h"
#endif

EXT_PRV EXT_CONST ext_diag_cmd_reg_obj g_drv_dfx_cmd_tbl[] = {
#ifdef PRODUCT_CFG_MEM_OPERATE
    { ID_DIAG_CMD_DRV_MEM_READ, ID_DIAG_CMD_DRV_MEM_READ, mem_read },
    { ID_DIAG_CMD_DRV_MEM_WRITE, ID_DIAG_CMD_DRV_MEM_WRITE, mem_write },
#endif
    { ID_DIAG_CMD_DRV_SYSTEM_RST, ID_DIAG_CMD_DRV_SYSTEM_RST, dfx_system_rst },
    { ID_DIAG_CMD_NEW_MEM_SHOW,   ID_DIAG_CMD_NEW_MEM_SHOW,   mem_info_show },
    { ID_DIAG_CMD_RESOURCE_SHOW,  ID_DIAG_CMD_RESOURCE_SHOW,  os_resource_show },
#ifdef TASK_DEBUG
#if TASK_DEBUG
    { ID_DIAG_CMD_TASK_SHOW, ID_DIAG_CMD_TASK_SHOW, os_task_show },
#endif
#endif
    { ID_DIAG_CMD_GET_UART_STAT, ID_DIAG_CMD_GET_UART_STAT, dfx_get_uart_stat },
    { ID_DIAG_CMD_GET_LP_STAT, ID_DIAG_CMD_GET_LP_STAT, dfx_get_lowpower_stat },
#if defined PRODUCT_CFG_NDM_COLLECT_DATA
    {ID_DIAG_CMD_DRV_UPLOAD_DATA, ID_DIAG_CMD_DRV_UPLOAD_DATA, upload_data},
    {ID_DIAG_CMD_DRV_UPLOAD_DATA_START, ID_DIAG_CMD_DRV_UPLOAD_DATA_STOP, upload_data_config},
    {ID_DIAG_CMD_DRV_RECEIVE_DATA_START, ID_DIAG_CMD_DRV_RECEIVE_DATA_STOP, receive_data_config},
#endif
};

td_u32 uapi_drv_dfx_init(td_void)
{
    td_u32 ret = EXT_ERR_SUCCESS;
    ret += uapi_diag_register_cmd(uapi_diag_reg_cmd_tbl(g_drv_dfx_cmd_tbl));
    return ret;
}
