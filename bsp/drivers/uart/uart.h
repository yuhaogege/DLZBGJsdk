/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Adjust the directory structure to prepare for compiling the SDK. uart.h code
 * Author: CompanyName
 * Create: 2012-07-14
 */

#ifndef __UART_H__
#define __UART_H__

#include "uart_drv.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CONFIG_MAX_BAUDRATE 921600 /* max baud rate */

#define UART_NUM 2
#define UART0 0
#define UART1 1

#define UART0_BASE EXT_UART0_REG_BASE
#define UART1_BASE EXT_UART1_REG_BASE

#define CONFIG_RX_BUF_SIZE 0x0200
#define CONFIG_TX_BUF_SIZE 0x0200
#define UART_CIRC_BUF_ENABLE
#define UART_ATTR_DEFAULT                  \
    {                                      \
        115200, 8, STOP_BIT_1, PARITY_EVEN \
    }
#define UART0_ATTR_DEFAULT                 \
    {                                      \
        115200, 8, STOP_BIT_1, PARITY_EVEN \
    }

/* Read Block: */
#define UART_RD_BLOCK 1
#define UART_RD_NONBLOCK 0

/* Write Block: */
#define UART_WD_BLOCK 1
#define UART_WD_NONBLOCK 0

uart_driver_data_t *uart_open(int uart_index, uart_mode uart_mode);
unsigned int uart_read(uart_driver_data_t *udd, char *buf, size_t count);
unsigned int uart_write(uart_driver_data_t *udd, const char *buf, size_t count);
unsigned int uart_ioctl(uart_driver_data_t *udd, int cmd, unsigned long arg);
unsigned int uart_close(uart_driver_data_t *udd);

extern uart_driver_data_t *g_udd_g[UART_NUM];
#ifdef __cplusplus
}
#endif
#endif
