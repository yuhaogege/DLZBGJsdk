/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: serial dw serial_dw.c code
 * Author: CompanyName
 * Create: 2012-12-14
 */
#include "serial_dw.h"
#include <soc_types.h>
#include <dw21_platform_rom.h>

#define UART_LOOPBACK_ENABLE (1 << 4)
#define to_write(addr, val) (*(volatile unsigned int *)(addr) = (val))

/*
 * IntegratorCP has two UARTs, use the first one, at 38400-8-N-1
 * Versatile PB has four UARTs.
 */
static void dw_putc(unsigned int g_uart_baseaddr, char c);
static int dw_getc(unsigned int g_uart_baseaddr);
static int dw_tstc(unsigned int g_uart_baseaddr);

/*
 * function: wait untill UART idle by reading out(throw) all fifo data
 */
void dw_wait_idle(unsigned int g_uart_baseaddr)
{
    while ((UAPI_REG_READ_VAL32(g_uart_baseaddr + DW_UART_USR) & 0x01u) != 0u) {
        if ((UAPI_REG_READ_VAL32(g_uart_baseaddr + DW_UART_LSR) & 0x01u) != 0u) {
            UAPI_REG_READ_VAL32(g_uart_baseaddr + DW_UART_RBR);
        }
    }
}

/*
 * Before and after need add serial_rx_remap interface Protect rx from generating busy
 */
void serial_init(unsigned int g_uart_baseaddr, unsigned int baud_rate)
{
    unsigned int data;
    unsigned int temp;
    unsigned int divider;
    unsigned int divider_h, divider_l;
    /* disable FIFO */
    UAPI_REG_WRITE32(g_uart_baseaddr + DW_UART_FCR, 0);

    /* config uart is ahb clock src , CONFIG_UART_CLOCK */
    temp = UAPI_REG_READ_VAL32(DW21_CRG_REG_SC_PERI_CLKSEL_REG);
    /* Using 150m crossover */
    temp &= (~(unsigned int)(1 << CRG_REG_UART_CLK_SEL_OFFSET));
    UAPI_REG_WRITE32(DW21_CRG_REG_SC_PERI_CLKSEL_REG, temp);

    /* enable UART0 FIFO */
    if (g_uart_baseaddr == EXT_UART0_REG_BASE) {
        UAPI_REG_WRITE32((uintptr_t)g_uart_baseaddr + DW_UART_FCR, 0x7);
    }

    /* enable interrupt */
    if (g_uart_baseaddr == EXT_UART1_REG_BASE) {
        to_write((uintptr_t)g_uart_baseaddr + DW_UART_IER, 5);
    } else {
        to_write((uintptr_t)g_uart_baseaddr + DW_UART_IER, 0);
    }

#ifdef PRODUCT_CFG_UART_INIT_NEW
    /* loopback mode */
    temp = UAPI_REG_READ_VAL32(g_uart_baseaddr + DW_UART_MCR);
    UAPI_REG_WRITE32((uintptr_t)g_uart_baseaddr + DW_UART_MCR, temp | UART_LOOPBACK_ENABLE);
#endif

    dw_wait_idle(g_uart_baseaddr);

    UAPI_REG_WRITE32((uintptr_t)g_uart_baseaddr + DW_UART_LCR, 0x80);

    /* Caculate devide */
    temp = 16 * baud_rate; /* Baud rate * 16 */
    divider = CONFIG_UART_CLOCK / temp;
    divider_h = ((divider) & 0xffff0000) >> 16; /* Take 16 bits high and 16 bits lower */
    divider_l = ((divider) & 0x0000ffff);
    UAPI_REG_WRITE32((uintptr_t)g_uart_baseaddr + DW_UART_DLL, divider_l);
    UAPI_REG_WRITE32((uintptr_t)g_uart_baseaddr + DW_UART_DLH, divider_h);

    dw_wait_idle(g_uart_baseaddr);

    data = 0x0;
    UAPI_REG_WRITE32((uintptr_t)g_uart_baseaddr + DW_UART_LCR, data); /* disable DLL and DLH */

    dw_wait_idle(g_uart_baseaddr);

#ifdef PRODUCT_CFG_UART_INIT_NEW
    /* loopback mode */
    temp = UAPI_REG_READ_VAL32(g_uart_baseaddr + DW_UART_MCR);
    UAPI_REG_WRITE32((uintptr_t)g_uart_baseaddr + DW_UART_MCR, temp & (~UART_LOOPBACK_ENABLE));
#endif
    data = 0x1b; /* 8bit data, 1bit stop,even parity */
    /* 8bit data, 1bit stop,no parity */
    UAPI_REG_WRITE32((uintptr_t)g_uart_baseaddr + DW_UART_LCR, data);
}

__init void serial_exit(unsigned int g_uart_baseaddr)
{
    int i;
    /* Disenable UART0 FIFO */
    UAPI_REG_WRITE32((uintptr_t)g_uart_baseaddr + DW_UART_FCR, 0x6);

    for (i = 0; i < DE_FIFO_DEPTH; i++) {
        UAPI_REG_READ_VAL32((uintptr_t)g_uart_baseaddr + DW_UART_RBR);
    }
}

void serial_putc(const char c)
{
#ifdef PRODUCT_CFG_PRINT_UART0
    dw_putc(EXT_UART0_REG_BASE, c);
#else
    dw_putc(EXT_UART1_REG_BASE, c);
#endif
}

void serial_puts(const char *s)
{
    const char *p = s;
    while (*p != '\0') {
        serial_putc(*p++);
    }
}
int serial_getc(void)
{
    return dw_getc(EXT_UART1_REG_BASE);
}

int serial_tstc(void)
{
    return dw_tstc(EXT_UART1_REG_BASE);
}

/* send */
EXT_PRV void dw_putc(unsigned int g_uart_baseaddr, char c)
{
    /* Wait until THRE is empyt */
    while ((UAPI_REG_READ_VAL32(g_uart_baseaddr + DW_UART_LSR) & 0x20) != 0x20) { }

    /* send one char */
    UAPI_REG_WRITE32(g_uart_baseaddr + DW_UART_THR, (unsigned)(int)c);
}

/* receive */
EXT_PRV int dw_getc(unsigned int g_uart_baseaddr)
{
    unsigned int data;
    unsigned int err;

    /* Wait until data is ready */
    while ((UAPI_REG_READ_VAL32(g_uart_baseaddr + DW_UART_LSR) & 0x01) != 0x01) { }
    /* receive one char */
    data = UAPI_REG_READ_VAL32(g_uart_baseaddr + DW_UART_RBR);

    /* Check for an error flag,read clear Reg */
    err = (UAPI_REG_READ_VAL32(g_uart_baseaddr + DW_UART_LSR) & 0x1E);
    if (err != 0) {
        return -1;
    }
    return (int)data;
}

EXT_PRV int dw_tstc(unsigned int g_uart_baseaddr)
{
    return (UAPI_REG_READ_VAL32(g_uart_baseaddr + DW_UART_LSR) & 0x01);
}

/* debug code */
void console_puthex(unsigned long h, unsigned char print_all)
{
    int i;
    char c;
    char mark = 0;
    unsigned long h_temp = h;

    for (i = 0; i < 8; i++) { /* exec 8 times */
        c = (h_temp >> 28) & 0x0F; /* right move 28 bit get 4 bit */

        if (c >= 0xa) {
            c = (char)(((int)c - 0xa) + (int)'A');
        } else {
            c = (char)((int)c + (int)'0');
        }

        if (print_all != 0) {
            serial_putc(c);
            h_temp = h_temp << 4; /* get low 4 bit */
            continue;
        }
        /* If not is last one and all num */
        if ((mark == 0) && (i != 7)) { /* i max is 7 */
            if (c != '0') {
                mark = 1;
                serial_putc(c);
            }
        } else {
            serial_putc(c);
        }

        h_temp = h_temp << 4; /* get low 4 bit */
    }
}
