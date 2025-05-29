/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Modify the drv directory, remove the first stage of the romization error,
 * delete the useless code to compile and adjust the dfx directory, remove the count
 * function driver part drv_dfx_uart.c code
 * Author: CompanyName
 * Create: 2012-04-04
 */

#include "drv_dfx_uart.h"
#include "soc_mdm_uart.h"

#include <dfx_uart.h>
#include <soc_types.h>
#include <soc_mdm_types.h>
#include <soc_mdm_diag.h>


td_u32 dfx_get_uart_stat(td_u16 id, EXT_IN td_pvoid cmd_param, td_u16 cmd_param_size, td_u8 option)
{
    ext_unref_param(cmd_param_size);

    ext_diag_uart_stat_cmd *cmd_param_point = (ext_diag_uart_stat_cmd *)cmd_param;
    ext_diag_uart_stat_ind uart_info = { 0 };
    td_u32 ret = EXT_ERR_INVALID_PARAMETER;

    uart_info.ret = EXT_ERR_INVALID_PARAMETER;
    if (cmd_param_point->port < EXT_UART_MAX) {
        uart_info.ret = uapi_uart_dfx_get_stat((ext_uart_port)(cmd_param_point->port),
            &(uart_info.cfg),
            &(uart_info.stat));
        ret = EXT_ERR_SUCCESS;
    }


    uapi_diag_report_packet(id, (td_u8)option, (td_u8 *)(&uart_info), sizeof(ext_diag_uart_stat_ind), TD_TRUE);

    return ret;
}
