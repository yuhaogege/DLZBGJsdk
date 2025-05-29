/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description:  serial_dw.h code
 * Author: CompanyName
 * Create: 2012-12-14
 */
#ifndef __SERIAL_DW_H__
#define __SERIAL_DW_H__

#define DW_UART_RBR 0x00 /* Receive buff. */
#define DW_UART_THR 0x00 /* Transmit Holding */
#define DW_UART_DLL 0x00 /* Divisor Latch Low */

#define DW_UART_DLH 0x04 /* Divisor Latch High */
#define DW_UART_IER 0x04 /* int enable */

#define DW_UART_IIR 0x08 /* interrupt indentification REG */
#define DW_UART_FCR 0x08 /* FIFO control */

#define DW_UART_LCR 0x0C /* Line control */
#define DW_UART_MCR 0x10 /* Modem control */

#define DW_UART_LSR 0x14 /* Line statue */

#define DW_UART_USR 0x7C /* Uart statues */

#define DW_UART_HTX 0xA4 /* Halt Tx */

#define DE_FIFO_DEPTH 0x10 /* 16bytes FIFO */

/*
 *
 * DW_UART_LCR bit field
 *
 */
#define DW_UART_DLAB (1 << 7)
#define DW_UART_BREAK (1 << 6)
#define DW_UART_EPS (1 << 4)
#define DW_UART_PEN (1 << 3)
#define DW_UART_STOP (1 << 2)
#define DW_UART_8BIT 0x03

void serial_init(unsigned int g_uart_baseaddr, unsigned int baud_rate);
void serial_exit(unsigned int g_uart_baseaddr);
void serial_putc(const char c);
void console_puthex(unsigned long h, unsigned char print_all); /* for debug */
void serial_puts(const char *s);
int serial_getc(void);
int serial_tstc(void);
#endif
