/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: UartStruct
 * Author: CompanyName
 * Create: 2018-10-31
 */

#ifndef __DFX_UART_H__
#define __DFX_UART_H__
#include <soc_types.h>
#include <soc_mdm_types.h>
#include <soc_mdm_uart.h>

typedef struct {
    td_u32 ret;
    ext_uart_init_type cfg;
    ext_uart_dfx_stat stat;
} ext_diag_uart_stat_ind; /* send */

typedef struct {
    td_u32 port;         /* serial port number */
} ext_diag_uart_stat_cmd; /* issue */

#endif
