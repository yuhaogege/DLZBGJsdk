
#ifndef DEMO_UART_H
#define DEMO_UART_H

#include "soc_mdm_uart.h"
#include "soc_mdm_io.h"
#include "soc_errno_rom.h"
#include <stdio.h>
#include <string.h>


void demo_uart_init(void);
td_void demo_uart_recv_task(td_void);
td_u32 demo_uart_tx_data(char *data, td_u16 data_length);
#endif

