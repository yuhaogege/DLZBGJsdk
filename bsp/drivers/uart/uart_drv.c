/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Adjust the directory structure to prepare for compiling the SDK. uart_drv.c code
 * Author: CompanyName
 * Create: 2012-07-14
 */

#include "uart_drv.h"
#include "uart.h"
#include "soc_mdm_isr.h"
#include "errno.h"

#include <soc_stdlib.h>
#include <los_hwi.h>
#include <los_base.h>
#include <los_memory.h>
#include <soc_mdm_time.h>
#include <soc_mdm_mem.h>
#include <dw21_platform_rom.h>

#define UART_RBR 0x00 /* Receive buff. */
#define UART_THR 0x00 /* Transmit Holding */
#define UART_DLL 0x00 /* Divisor Latch Low */
#define UART_DLH 0x04 /* Divisor Latch High */
#define UART_IER 0x04 /* int enable */
#define UART_IIR 0x08 /* interrupt indentification REG */
#define UART_FCR 0x08 /* FIFO control */
#define UART_LCR 0x0C /* Line control */
#define UART_MCR 0x10 /* Line control */
#define UART_LSR 0x14 /* Line statue */
#define UART_USR 0x7C /* Uart statues */
#define UART_TFL 0x80
#define UART_RFL 0x84
#define UART_HTX 0xA4 /* Halt Tx */

#define UART_FCR_RXFIFO_ONEBYTE 0
#define UART_FCR_RXFIFO_QUARTER (1 << 6)
#define UART_FCR_RXFIFO_HALF (2 << 6)
#define UART_FCR_RXFIFO_2TOFULL (3 << 6)
#define UART_FCR_TXFIFO_EMPTY 0
#define UART_FCR_TXFIFO_2BYTE (1 << 4)
#define UART_FCR_TXFIFO_QUARTER (2 << 4)
#define UART_FCR_TXFIFO_HALF (3 << 4)
#define UART_FCR_RESET_TXFIFO (1 << 2)
#define UART_FCR_RESET_RXFIFO (1 << 1)
#define UART_FCR_ENABLE_FIFO (1 << 0)

#define FCR_CONFIG_VAL                                                                                 \
    (UART_FCR_RXFIFO_QUARTER | UART_FCR_TXFIFO_2BYTE | UART_FCR_RESET_TXFIFO | UART_FCR_RESET_RXFIFO | \
        UART_FCR_ENABLE_FIFO)

#define UART_FIFO_TX_SIZE 16
#define UART_IER_TX (0x01 << 1)
#define UART_IER_RX (0x01 << 0)
#define UART_IER_TF (0x01 << 8) /* Tx finish */
#define UART_IER_MASK 0x18F

#define UART_IIR_RX_TIMEOUT (0x03 << 2)
#define UART_IIR_RX (0x01 << 2)
#define UART_IIR_TX (0x01 << 1)
#define UART_IIR_TF 0xE /* Tx finish */

#define UART_USR_BUSY (0x01 << 0)
#define UART_USR_TXF_NF (0x01 << 1)
#define UART_USR_TXF_E (0x01 << 2)
#define UART_USR_RXF_NE (0X01 << 3)

#define UART_LSR_OE 0x02 /* overrun error */
#define UART_LSR_PE 0x04 /* parity error */
#define UART_LSR_FE 0x08 /* framing error */
#define UART_LSR_BI 0x10 /* break interrupt */

int g_irq_send_count = 0;

#define ext_uart_buffer_lock(x) uapi_int_lock()
#define ext_uart_buffer_unlock(x, val) uapi_int_restore(val)

int uart_circ_buf_empty(EXT_CONST uart_circ_buf_t *transfer)
{
    if (transfer->flags & BUF_CIRCLED) {
        return 0;
    }

    if (transfer->wp == transfer->rp) {
        return 1;
    } else {
        return 0;
    }
}

__init unsigned int uart_init_circ_buf(uart_driver_data_t *udd, unsigned int rx_buf_size, unsigned int tx_buf_size)
{
    if ((rx_buf_size == 0) || (tx_buf_size == 0)) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    udd->rx_transfer = (uart_circ_buf_t *)uapi_malloc(EXT_MOD_ID_UART, sizeof(uart_circ_buf_t));
    if (udd->rx_transfer == NULL) {
        goto END;
    }
    (td_void)memset_s((td_void *)udd->rx_transfer, sizeof(uart_circ_buf_t), 0, sizeof(uart_circ_buf_t));
    udd->rx_transfer->data = (char *)uapi_malloc(EXT_MOD_ID_UART, rx_buf_size);
    if (udd->rx_transfer->data == NULL) {
        goto FREE_RX_TRANSFER;
    }
    udd->rx_transfer->size = rx_buf_size;

    udd->tx_transfer = (uart_circ_buf_t *)uapi_malloc(EXT_MOD_ID_UART, sizeof(uart_circ_buf_t));
    if (udd->tx_transfer == NULL) {
        goto FREE_RX_CIRC_BUF;
    }
    (td_void)memset_s((td_void *)udd->tx_transfer, sizeof(uart_circ_buf_t), 0, sizeof(uart_circ_buf_t));
    udd->tx_transfer->data = (char *)uapi_malloc(EXT_MOD_ID_UART, tx_buf_size);
    if (udd->tx_transfer->data == NULL) {
        goto FREE_TX_CIRC_BUF;
    }
    udd->tx_transfer->size = tx_buf_size;

    return EXT_ERR_SUCCESS;

FREE_TX_CIRC_BUF:
    uapi_free(EXT_MOD_ID_UART, (VOID *)udd->tx_transfer);
    udd->tx_transfer = TD_NULL;

FREE_RX_CIRC_BUF:
    uapi_free(EXT_MOD_ID_UART, (VOID *)udd->rx_transfer->data);
    udd->rx_transfer->data = TD_NULL;
FREE_RX_TRANSFER:
    uapi_free(EXT_MOD_ID_UART, (VOID *)udd->rx_transfer);
    udd->rx_transfer = TD_NULL;
END:
    return EXT_ERR_FAILURE;
}

__init void uart_deinit_circ_buf(uart_driver_data_t *udd)
{
    if (udd->rx_transfer != TD_NULL) {
        if (udd->rx_transfer->data != TD_NULL) {
            uapi_free(EXT_MOD_ID_UART, udd->rx_transfer->data);
            udd->rx_transfer->data = TD_NULL;
        }

        uapi_free(EXT_MOD_ID_UART, udd->rx_transfer);
        udd->rx_transfer = TD_NULL;
    }

    if (udd->tx_transfer != TD_NULL) {
        if (udd->tx_transfer->data != TD_NULL) {
            uapi_free(EXT_MOD_ID_UART, udd->tx_transfer->data);
            udd->tx_transfer->data = TD_NULL;
        }

        uapi_free(EXT_MOD_ID_UART, udd->tx_transfer);
        udd->tx_transfer = TD_NULL;
    }
}

int uart_read_circ_buf(uart_circ_buf_t *transfer, char *buf, size_t count)
{
    if (transfer == NULL) {
        return -EFAULT;
    }
    td_u32 last_int_val = ext_uart_buffer_lock(transfer);
    unsigned int wp = transfer->wp;
    unsigned int rp = transfer->rp;
    unsigned int flags = transfer->flags;
    ext_uart_buffer_unlock(transfer, last_int_val);
    unsigned long data = (unsigned long)(uintptr_t)transfer->data;
    unsigned int buf_size = transfer->size;
    if ((flags & BUF_CIRCLED) == 0) {
        if (count >= (wp - rp)) {
            count = wp - rp;
        }
        if (count > 0 && memcpy_s(buf, count, (void *)((uintptr_t)data + rp), count) != EOK) {
            return -EFAULT;
        }
        transfer->rp += count;
        return (int)count;
    } else {
        if (count < (buf_size - rp)) {
            if (count > 0 && memcpy_s(buf, count, (void *)((uintptr_t)data + rp), count) != EOK) {
                return -EFAULT;
            }
            transfer->rp += count;
            return (int)count;
        } else {
            unsigned int copy_size = buf_size - rp;
            unsigned int left_size = count - copy_size;
            if (copy_size > 0 && memcpy_s(buf, copy_size, (void *)((uintptr_t)data + rp), copy_size) != EOK) {
                return -EFAULT;
            }
            rp = 0;
            if (left_size > wp) {
                left_size = wp;
            }
            if (left_size > 0 &&
                memcpy_s((void *)(buf + copy_size), left_size, (void *)((uintptr_t)data + rp), left_size) != EOK) {
                return -EFAULT;
            }
            last_int_val = ext_uart_buffer_lock(transfer);
            transfer->rp = left_size;
            transfer->flags &= ~BUF_CIRCLED;
            ext_uart_buffer_unlock(transfer, last_int_val);
            return (int)(copy_size + left_size);
        }
    }
}

unsigned char uart_rx_fifo_num(EXT_CONST uart_driver_data_t *udd)
{
    return (unsigned char)UAPI_REG_READ_VAL32((uintptr_t)udd->phys_base + UART_RFL);
}

int uart_write_circ_buf(uart_circ_buf_t *transfer, const char *buf, size_t count)
{
    if (transfer == NULL) {
        return -EFAULT;
    }
    td_u32 last_int_val = ext_uart_buffer_lock(transfer);
    unsigned int wp = transfer->wp;
    unsigned int rp = transfer->rp;
    unsigned int flags = transfer->flags;
    ext_uart_buffer_unlock(transfer, last_int_val);
    unsigned long data = (unsigned long)(uintptr_t)transfer->data;
    unsigned int buf_size = transfer->size;
    if ((flags & BUF_CIRCLED) == 0) {
        if (count < (buf_size - wp)) {
            if (count > 0 && memcpy_s((void *)((uintptr_t)data + wp), count, buf, count) != EOK) {
                return -EFAULT;
            }
            transfer->wp += count;
            return (int)count;
        } else {
            unsigned int copy_size = buf_size - wp;
            unsigned int left_size = count - copy_size;
            if (copy_size > 0 && memcpy_s((void *)((uintptr_t)data + wp), copy_size, buf, copy_size) != EOK) {
                return -EFAULT;
            }
            wp = 0;
            if (left_size > rp) {
                /* overflowed. some new data will be discarded */
                left_size = rp;
            }
            if (left_size > 0 &&
                memcpy_s((void *)((uintptr_t)data + wp), left_size, (void *)(buf + copy_size), left_size) != EOK) {
                return -EFAULT;
            }
            last_int_val = ext_uart_buffer_lock(transfer);
            transfer->wp = left_size;
            transfer->flags |= BUF_CIRCLED;
            ext_uart_buffer_unlock(transfer, last_int_val);
            return (int)(copy_size + left_size);
        }
    } else {
        if (count > (rp - wp)) {
            /* overflowed. some new data will be discarded */
            count = rp - wp;
        }
        if (count > 0 && memcpy_s((void *)((uintptr_t)data + wp), count, buf, count) != EOK) {
            return -EFAULT;
        }
        transfer->wp += count;
        return (int)count;
    }
}

void uart_tx_interrupt_disable(EXT_CONST uart_driver_data_t *udd)
{
    unsigned int tx_status = UAPI_REG_READ_VAL32(udd->phys_base + UART_IER);
    UAPI_REG_WRITE32((uintptr_t)udd->phys_base + UART_IER, (~UART_IER_TX) & tx_status);
}

void uart_tx_interrupt_enable(EXT_CONST uart_driver_data_t *udd)
{
    unsigned int tx_status = UAPI_REG_READ_VAL32(udd->phys_base + UART_IER);
    UAPI_REG_WRITE32((uintptr_t)udd->phys_base + UART_IER, (unsigned short)(UART_IER_TX | tx_status));
}

void uart_rx_interrupt_disable(EXT_CONST uart_driver_data_t *udd)
{
    unsigned int tx_status = UAPI_REG_READ_VAL32(udd->phys_base + UART_IER);
    UAPI_REG_WRITE32((uintptr_t)udd->phys_base + UART_IER, ~(UART_IER_RX)&tx_status);
}

EXT_PRV void uart_rx_interrupt_enable(EXT_CONST uart_driver_data_t *udd)
{
    unsigned int tx_status = UAPI_REG_READ_VAL32(udd->phys_base + UART_IER);
    UAPI_REG_WRITE32((uintptr_t)udd->phys_base + UART_IER, UART_IER_RX | tx_status);
}

void uart_tf_interrupt_disable(EXT_CONST uart_driver_data_t *udd)
{
    unsigned int tx_status = UAPI_REG_READ_VAL32((uintptr_t)udd->phys_base + UART_IER);
    UAPI_REG_WRITE32((uintptr_t)udd->phys_base + UART_IER, ~(UART_IER_TF)&tx_status);
}
EXT_PRV void uart_tf_interrupt_enable(EXT_CONST uart_driver_data_t *udd)
{
    unsigned int tx_status = UAPI_REG_READ_VAL32((uintptr_t)udd->phys_base + UART_IER);
    UAPI_REG_WRITE32((uintptr_t)udd->phys_base + UART_IER, UART_IER_TF | tx_status);
}

EXT_PRV td_u32 uart_interrupt_disable(EXT_CONST uart_driver_data_t *udd)
{
    unsigned int int_val = UAPI_REG_READ_VAL32((uintptr_t)udd->phys_base + UART_IER);
    UAPI_REG_WRITE32((uintptr_t)udd->phys_base + UART_IER, int_val & (~UART_IER_MASK));
    return int_val;
}

EXT_PRV void uart_interrupt_restore(EXT_CONST uart_driver_data_t *udd, td_u32 int_val)
{
    UAPI_REG_WRITE32((uintptr_t)udd->phys_base + UART_IER, int_val);
}

EXT_PRV void get_recv_irq_err(unsigned int int_status, uart_drv_stat_info_t *uart_stat_info)
{
    if ((int_status & UART_LSR_OE) != 0) {
        (uart_stat_info->recv_irq_err_overrun)++;
    }

    if ((int_status & UART_LSR_PE) != 0) {
        (uart_stat_info->recv_irq_err_parity)++;
    }

    if ((int_status & UART_LSR_FE) != 0) {
        (uart_stat_info->recv_irq_err_frame)++;
    }

    if ((int_status & UART_LSR_BI) != 0) {
        (uart_stat_info->recv_irq_err_break)++;
    }
}
EXT_PRV void uart_drv_iir_tx_proc(uart_driver_data_t *udd, char *buf, td_u32 len)
{
    ext_unref_param(len);
    if (udd->tx_transfer == TD_NULL || udd->ops == TD_NULL) {
        return;
    }
    if (uart_circ_buf_empty(udd->tx_transfer) == TD_TRUE) {
        uart_tx_interrupt_disable(udd);
        uart_tf_interrupt_enable(udd);
    } else {
        unsigned int tfl = UAPI_REG_READ_VAL32(udd->phys_base + UART_TFL);
        EXT_CONST unsigned int tx_fifo_cnt = UART_FIFO_TX_SIZE - tfl;
        unsigned int count = (unsigned int)udd->tx_send(udd->tx_transfer, buf, tx_fifo_cnt);
        if (udd->ops->start_tx) {
            (void)udd->ops->start_tx(udd, buf, count);
        }
        if ((udd->flags & UART_FLG_WD_BLOCK) != 0) {
            (void)LOS_EventWrite(&udd->uart_event, UART_WD_EVENT);
        }
    }
#ifdef UART_DEBUG_INFO
    udd->uart_stat_info.send_irq_cnt++;
#endif
}
#define BUFF_SIZE 64

EXT_PRV void uart_drv_irq_stat_count(uart_driver_data_t *udd, unsigned int count, unsigned int rcv_suc_cnt)
{
    udd->uart_stat_info.recv_irq_cnt++;
    udd->uart_stat_info.recv_irq_data_cnt += count;
    udd->uart_stat_info.recv_irq_data_suc_cnt += rcv_suc_cnt;
    if (rcv_suc_cnt < count) {
        udd->uart_stat_info.recv_buf_overflow_cnt++;
    }
}

EXT_PRV void uart_drv_irq(td_u32 data)
{
    char buf[BUFF_SIZE] = { 0 };
    unsigned int count = 0;
    unsigned int rcv_suc_cnt = 0;
    uintptr_t data_t = (uintptr_t)data;
    uart_driver_data_t *udd = (uart_driver_data_t *)data_t;
    if (udd == NULL || udd->rx_transfer == TD_NULL) {
        return;
    }
    uart_drv_stat_info_t *uart_stat_info = &(udd->uart_stat_info);
    unsigned int status = UAPI_REG_READ_VAL32(udd->phys_base + UART_IIR) & 0xf;
    if ((status == UART_IIR_RX_TIMEOUT) || (status == UART_IIR_RX)) {
        get_recv_irq_err(UAPI_REG_READ_VAL32(udd->phys_base + UART_LSR), uart_stat_info);
        do {
            unsigned int usr = UAPI_REG_READ_VAL32(udd->phys_base + UART_USR);
            /* if uart hardware rx fifo is empty, go out of circle */
            if (((usr & UART_USR_RXF_NE) == 0) && (count == 0)) {
                UAPI_REG_READ_VAL32((td_u32)(udd->phys_base + (td_u32)UART_RBR));
                (udd->uart_stat_info.recv_irq_err_emptyfifo_cnt)++;
            }
            if ((usr & UART_USR_RXF_NE) == 0) {
                break;
            }
            buf[count] = (char)UAPI_REG_READ_VAL32(udd->phys_base + UART_RBR);
            if (udd->uart_stat_info.recv_last_context.num < EXT_UART_LAST_RECORD_BYTE_COUNT) {
                udd->uart_stat_info.recv_last_context.data[udd->uart_stat_info.recv_last_context.num] =
                    (unsigned char)buf[count];
            }
            udd->uart_stat_info.recv_last_context.num++;
            udd->uart_stat_info.recv_last_context.num &= (EXT_UART_LAST_RECORD_BYTE_COUNT - 1);
            count++;
        } while (count < BUFF_SIZE);
        if (count >= 1) {
            rcv_suc_cnt = (unsigned int)udd->rx_recv(udd->rx_transfer, buf, count);
            if ((udd->flags & UART_FLG_RD_BLOCK) != 0) {
                LOS_EventWrite(&udd->uart_event, UART_RD_EVENT);
            }
        }
#ifdef UART_DEBUG_INFO
        uart_drv_irq_stat_count(udd, count, rcv_suc_cnt);
#endif
    }
    if (status == UART_IIR_TX) {
        /* disable uart tx fifo interrupt */
        uart_drv_iir_tx_proc(udd, buf, sizeof(buf));
    } else if (status == UART_IIR_TF) {
        uart_tf_interrupt_disable(udd);
    }
}

#define WRITE_LCR_TIMEOUT 1000000
#define LCR_DLAB 0x80
EXT_PRV void writelcr(EXT_CONST uart_driver_data_t *udd, unsigned int val)
{
    unsigned int usr = 0;
    unsigned int lcr;
    unsigned int timeout_cnt = 0;
    unsigned int i = 0;

    lcr = UAPI_REG_READ_VAL32((uintptr_t)udd->phys_base + UART_LCR);
    while (lcr != val) {
        if (timeout_cnt++ >= WRITE_LCR_TIMEOUT) {
            return;
        }
        usr = UAPI_REG_READ_VAL32((uintptr_t)udd->phys_base + UART_USR);
        while (usr & UART_USR_BUSY) {
            uapi_udelay(1);
            if (timeout_cnt++ >= WRITE_LCR_TIMEOUT) {
                return;
            }
            lcr = UAPI_REG_READ_VAL32((uintptr_t)udd->phys_base + UART_LCR);
            for (i = 0; i < UART_FIFO_TX_SIZE + 1 && !(lcr & LCR_DLAB); i++) {
                (void)UAPI_REG_READ_VAL32((uintptr_t)udd->phys_base + UART_RBR);
            }

            if ((lcr & LCR_DLAB)) {
                /* reset fifo */
                UAPI_REG_WRITE32((uintptr_t)udd->phys_base + UART_FCR, FCR_CONFIG_VAL);
            }

            usr = UAPI_REG_READ_VAL32((uintptr_t)udd->phys_base + UART_USR);
        }

        UAPI_REG_WRITE32((uintptr_t)udd->phys_base + UART_LCR, val);
        lcr = UAPI_REG_READ_VAL32((uintptr_t)udd->phys_base + UART_LCR);
    }
}

__init static void uart_drv_set_attr(EXT_CONST uart_driver_data_t *udd)
{
    unsigned int baudrate;
    unsigned int lcr = 0;
    unsigned int mcr;

    unsigned int divider;
    unsigned char divider_h;
    unsigned char divider_l;

    unsigned int uv_int_save;

    /* Caculate devide */
    baudrate= uapi_min(CONFIG_MAX_BAUDRATE, udd->attr.baudrate);
    baudrate = 16 * baudrate;                      /* Calculate the baud rate and multiply by 16 */
    divider = ((g_cfg_uart_clock * 10) / baudrate + 5) / 10; /* Serial clock *10/baud rate + 5 */

    divider_h = ((divider) & 0xff00) >> 8; /* Exchange high and low 8 */
    divider_l = ((divider) & 0x00ff);

    if (udd->attr.data_bits == 5) { /* data bit, 5bit */
        lcr |= (DATA_BIT_5);
    } else if (udd->attr.data_bits == 6) { /* data 6bit */
        lcr |= (DATA_BIT_6);
    } else if (udd->attr.data_bits == 7) { /* data 7bit */
        lcr |= (DATA_BIT_7);
    } else { /* data 8bit */
        lcr |= (DATA_BIT_8);
    }

    if (udd->attr.stop_bits == 1) {
        lcr |= (STOP_BIT_1);
    } else if (udd->attr.stop_bits == STOP_BIT_1P5 || udd->attr.stop_bits == STOP_BIT_2) {
        lcr |= (STOP_BIT_2);
    }

    if (udd->attr.parity == PARITY_NONE) {
        lcr |= (PARITY_NONE);
    } else if (udd->attr.parity == PARITY_ODD) {
        lcr |= (PARITY_ODD);
    } else if (udd->attr.parity == PARITY_EVEN) {
        lcr |= (PARITY_EVEN);
    }

    uv_int_save = uart_interrupt_disable(udd);

    /* loopback mode */
    mcr = UAPI_REG_READ_VAL32((uintptr_t)udd->phys_base + UART_MCR);
    UAPI_REG_WRITE32((uintptr_t)udd->phys_base + UART_MCR, mcr | (1 << 4)); /* get 4 bit */

    writelcr(udd, 0x80);

    /* configure DLL and DLH */
    UAPI_REG_WRITE32((uintptr_t)udd->phys_base + UART_DLL, divider_l);
    UAPI_REG_WRITE32((uintptr_t)udd->phys_base + UART_DLH, divider_h);

    writelcr(udd, lcr);

    mcr = UAPI_REG_READ_VAL32((uintptr_t)udd->phys_base + UART_MCR);
    UAPI_REG_WRITE32((uintptr_t)udd->phys_base + UART_MCR, mcr & (~(1 << 4))); /* get low 4 bit */

    uart_interrupt_restore(udd, uv_int_save);
}

/* uops */
__init static int uart_drv_startup(uart_driver_data_t *udd)
{
    unsigned int ret;
    unsigned int fcr;

    /* disable interrupt */
    UAPI_REG_WRITE32((uintptr_t)udd->phys_base + UART_IER, 0);

    /* set baudrate,data_bit,stop_bit,parity */
    uart_drv_set_attr(udd);

    /* reset and enable FIFO */
    fcr = FCR_CONFIG_VAL;
    UAPI_REG_WRITE32((uintptr_t)udd->phys_base + UART_FCR, fcr);

    /* creat interrupt function for uart */
    ret = uapi_irq_request(udd->irq_num, 3, (lisr_proc_func)uart_drv_irq, (uintptr_t)udd); /* priority 3 */
    if (ret == 0) {
        uart_rx_interrupt_enable(udd);
        uart_tf_interrupt_disable(udd);
        uapi_irq_enable((td_u32)udd->irq_num);
    }

    return (td_s32)ret;
}

__init static void uart_drv_shutdown(const struct uart_driver_data *udd)
{
    uart_tf_interrupt_disable(udd);
    uart_tx_interrupt_disable(udd);
    uart_rx_interrupt_disable(udd);
    uapi_irq_disable((td_u32)udd->irq_num);

    LOS_HwiDelete(udd->irq_num);
}

EXT_PRV int uart_drv_start_tx(struct uart_driver_data *udd, const char *buf, size_t count)
{
    unsigned int idx = 0;

    if (count > UART_FIFO_TX_SIZE) {
        return (int)EXT_ERR_FAILURE;
    }

    /* if tx fifo is empty, write data to fifo */
    while (idx < count) {
        UAPI_REG_WRITE32((uintptr_t)udd->phys_base + UART_THR, (unsigned)(td_u8)buf[idx]);

        if (udd->uart_stat_info.send_last_context.num < EXT_UART_LAST_RECORD_BYTE_COUNT) {
            udd->uart_stat_info.send_last_context.data[udd->uart_stat_info.send_last_context.num] =
                (unsigned char)buf[idx];
        }
        udd->uart_stat_info.send_last_context.num++;
        udd->uart_stat_info.send_last_context.num &= (EXT_UART_LAST_RECORD_BYTE_COUNT - 1);

        idx++;
    }
#ifdef UART_DEBUG_INFO
    udd->uart_stat_info.send_irq_data_cnt += count;
#endif
    return (int)count;
}

__init static int uart_drv_ioctl(uart_driver_data_t *udd)
{
    unsigned int fcr;
    /* wait for send finish */
    uart_drv_set_attr(udd);

    /* reset and enable FIFO */
    fcr = FCR_CONFIG_VAL;
    UAPI_REG_WRITE32(udd->phys_base + UART_FCR, fcr);

    uart_rx_interrupt_enable(udd);
    return 0;
}

uart_ops_t g_uart_driver_uops = {
    .startup = uart_drv_startup,
    .shutdown = uart_drv_shutdown,
    .start_tx = uart_drv_start_tx,
    .ioctl = uart_drv_ioctl,
};
