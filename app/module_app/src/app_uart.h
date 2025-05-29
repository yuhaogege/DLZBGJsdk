/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: APP UART handle.
 */

#ifndef APP_UART_H
#define APP_UART_H

#include <soc_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define APP_UART_SLEEP_DURTION_MS 1000

td_u32 app_uart_init(td_void);
td_void app_uart_task_body(td_u32 param);
td_u32 app_uart_tx(td_pbyte data, td_u16 data_length);

#ifdef __cplusplus
}
#endif
#endif
