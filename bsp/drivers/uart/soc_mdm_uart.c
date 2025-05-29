/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Adjust the directory structure to prepare for compiling the SDK. soc_mdm_uart.c code
 * Author: CompanyName
 * Create: 2012-07-14
 */
#include "soc_mdm_uart.h"
#include "uart.h"
#include "soc_mdm_types.h"
#include "uart_drv.h"

#include <soc_stdlib.h>

#define UART_FD_BASE 0
#define UART_NUMBER 1

__init td_u32 uapi_uart_open(ext_uart_port port)
{
    uart_driver_data_t *udd = TD_NULL;
    udd = uart_open(port, (uart_mode)0);
    if (udd == TD_NULL) {
        return EXT_ERR_FAILURE;
    }

    return EXT_ERR_SUCCESS;
}

td_s32 uapi_uart_read(ext_uart_port port, td_u8 *data, td_u32 data_len)
{
    td_u32 port_num = (td_u32)port;

    if (port_num > UART_NUMBER) {
        return (td_s32)EXT_ERR_FAILURE;
    }

    return (td_s32)uart_read(g_udd_g[port_num], (td_char *)data, data_len);
}

td_s32 uapi_uart_write(ext_uart_port port, EXT_CONST td_u8 *data, td_u32 data_len)
{
    td_u32 port_num = (td_u32)port;

    if (port_num > UART_NUMBER) {
        return (td_s32)EXT_ERR_FAILURE;
    }

    return (td_s32)uart_write(g_udd_g[port_num], (td_char *)data, data_len);
}

__init td_u32 uapi_uart_ioctl(ext_uart_port port, td_u32 request, uintptr_t attr)
{
    td_u32 port_num = (td_u32)port;
    ext_uart_init_type *cfg = (ext_uart_init_type *)attr;
    uart_driver_data_t *udd = TD_NULL;
    ext_uart_attr uart_attr = { 0 };

    if (port_num > UART_NUMBER) {
        return EXT_ERR_FAILURE;
    }
    udd = g_udd_g[port_num];

    if (request == UART_CFG_SET_ATTR) {
        uart_attr.baudrate = cfg->baud_rate;
        uart_attr.data_bits = (td_u8)cfg->data_bits;
        uart_attr.parity = EXT_UART_PARITY_NONE;
        uart_attr.stop_bits = EXT_UART_STOP_BIT_1;

        if (cfg->parity == 1) {
            uart_attr.parity = EXT_UART_PARITY_ODD;
        } else if (cfg->parity == 2) { /* 2: Even parity */
            uart_attr.parity = EXT_UART_PARITY_EVEN;
        }
        if (cfg->stop_bits == 2) { /* 2: 2bit stop bit */
            uart_attr.stop_bits = EXT_UART_STOP_BIT_2;
        }

        return uart_ioctl(udd, (td_s32)request, (td_u32)(uintptr_t)&uart_attr);
    } else if (request == UART_CFG_GET_ATTR) {
        td_u32 ret = uart_ioctl(g_udd_g[port_num], (td_s32)request, (uintptr_t)&uart_attr);
        if (ret == EXT_ERR_SUCCESS) {
            cfg->baud_rate = uart_attr.baudrate;
            cfg->data_bits = (td_u32)uart_attr.data_bits;
            if (uart_attr.parity == EXT_UART_PARITY_NONE) {
                cfg->parity = 0;
            } else if (uart_attr.parity == EXT_UART_PARITY_ODD) {
                cfg->parity = 1;
            } else if (uart_attr.parity == EXT_UART_PARITY_EVEN) {
                cfg->parity = 2; /* 2: Even parity */
            }

            if (uart_attr.stop_bits == EXT_UART_STOP_BIT_1) {
                cfg->stop_bits = 1;
            } else if (uart_attr.stop_bits == EXT_UART_STOP_BIT_2) {
                cfg->stop_bits = 2; /* *< Stop bit. 1:1bit stop bit; 2: 2bit stop bit */
            }
        }
        return ret;
    } else {
        return uart_ioctl(udd, (td_s32)request, attr);
    }
}

__init td_u32 uapi_uart_close(ext_uart_port port)
{
    td_u32 port_num = (td_u32)port;

    if (port_num > UART_NUMBER) {
        return EXT_ERR_FAILURE;
    }

    return uart_close(g_udd_g[port_num]);
}

__init td_u32 uapi_uart_dfx_get_stat(ext_uart_port port, ext_uart_init_type *cfg, ext_uart_dfx_stat *stat)
{
    td_u32 ret;

    if (port > UART_NUMBER || cfg == TD_NULL || stat == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    uart_attr_t uart_attr = { 0 };

    ret = uart_ioctl(g_udd_g[port], UART_CFG_GET_ATTR, (uintptr_t)&uart_attr);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    cfg->baud_rate = uart_attr.baudrate;
    cfg->data_bits = uart_attr.data_bits;
    cfg->parity = 0;
    cfg->stop_bits = 1;

    if (uart_attr.parity == EXT_UART_PARITY_ODD) {
        cfg->parity = 1;
    } else if (uart_attr.parity == EXT_UART_PARITY_EVEN) {
        cfg->parity = 2; /* 2: Even parity */
    }

    if (uart_attr.stop_bits == EXT_UART_STOP_BIT_2) {
        cfg->stop_bits = 2; /* 2: Even parity */
    }

    uart_drv_stat_info_t *stat_inf = &(g_udd_g[port]->uart_stat_info);

    stat->rx_irq_cnt = stat_inf->recv_irq_cnt;
    stat->rx_data_cnt = stat_inf->recv_irq_data_cnt;
    stat->rx_data_suc_cnt = stat_inf->recv_irq_data_suc_cnt;
    stat->rx_data_buf_overflow_cnt = stat_inf->recv_buf_overflow_cnt;
    stat->tx_irq_cnt = stat_inf->send_irq_cnt;
    stat->tx_data_cnt = stat_inf->send_irq_data_cnt;

    stat->err_overrun = stat_inf->recv_irq_err_overrun;
    stat->err_parity = stat_inf->recv_irq_err_parity;
    stat->err_frame = stat_inf->recv_irq_err_frame;
    stat->err_break = stat_inf->recv_irq_err_break;
    stat->err_busy = stat_inf->recv_irq_err_busy;

    (td_void)memcpy_s(stat->last_rx_data, sizeof(stat->last_rx_data), stat_inf->recv_last_context.data,
        EXT_UART_LAST_RECORD_BYTE_COUNT);
    stat->last_rx_sn = stat_inf->recv_last_context.num;

    (td_void)memcpy_s(stat->last_tx_data, sizeof(stat->last_tx_data), stat_inf->send_last_context.data,
        EXT_UART_LAST_RECORD_BYTE_COUNT);
    stat->last_tx_sn = stat_inf->send_last_context.num;

    return EXT_ERR_SUCCESS;
}
