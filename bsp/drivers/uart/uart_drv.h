/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Adjust the directory structure to prepare for compiling the SDK. uart_drv.h code
 * Author: CompanyName
 * Create: 2012-07-14
 */
#ifndef __UART_DRV_H__
#define __UART_DRV_H__

#include <soc_mdm_uart.h>

#include "los_typedef.h"
#include "los_event.h"
#ifdef __cplusplus
extern "C" {
#endif

#define UART_RD_EVENT 0x1
#define UART_WD_EVENT 0x2
#define BUF_CIRCLED (1 << 0)
#define BUF_OVERFLOWED (1 << 1)
#define BUF_EMPTIED (1 << 2)

#define UART_FLG_RD_BLOCK (1 << 0)
#define UART_FLG_WD_BLOCK (1 << 1)

#define DATA_BIT_5 0x00
#define DATA_BIT_6 0x01
#define DATA_BIT_7 0x02
#define DATA_BIT_8 0x03
#define STOP_BIT_1 0x00
#define STOP_BIT_1P5 0x04
#define STOP_BIT_2 0x04
#define PARITY_NONE 0x00
#define PARITY_ODD 0x08
#define PARITY_EVEN 0x18
#define UART_DEBUG_INFO

#define GPIO_SEL_UART 0
#define GPIO_SEL_GPIO 1

#define UART_ERR_PARA_INVALID 1
#define UART_ERR_INIT_CIRC_FAILED 2
#define UART_ERR_START_FAILED 3
#define UART_ERR_IOCTL_FAILED 4
#define UART_ERR_PTR_NULL 5
#define UART_ERR_OPEN_AGAIN 6
#define UART_ERR_NOT_OPENED 7

typedef struct uart_circ_buf {
    unsigned int rp;
    unsigned int wp;
    unsigned int flags;
    char *data;
    unsigned int size;
} uart_circ_buf_t;

typedef enum uart_status {
    UART_STATE_NOT_OPENED = 0,
    UART_STATE_USEABLE
} uart_status;

typedef enum uart_mode {
    UART_232 = 0
} uart_mode;

typedef struct uart_attr {
    unsigned int baudrate;
    unsigned char data_bits;
    unsigned char stop_bits;
    unsigned char parity;
    unsigned char rsv;
} uart_attr_t;

typedef struct uart_driver_data uart_driver_data_t;

typedef struct uart_ops {
    int (*startup)(uart_driver_data_t *udd);
    void (*shutdown)(const uart_driver_data_t *udd);
    int (*start_tx)(uart_driver_data_t *udd, const char *buf, size_t count);
    int (*ioctl)(uart_driver_data_t *udd);
} uart_ops_t;

typedef int (*recv_notify)(uart_circ_buf_t *transfer, const char *buf, size_t count);
typedef int (*send_buf)(uart_circ_buf_t *transfer, char *buf, size_t count);

#ifdef UART_DEBUG_INFO

#define uart_set_err_no(err) (udd->uart_stat_info.uart_errno = (err))
#else
#define uart_set_err_no(err)
#endif

typedef struct {
    unsigned char data[EXT_UART_LAST_RECORD_BYTE_COUNT]; /* Stores the last data sent and received by the UART */
    unsigned int num;                                   /* The subscript of the last stored data */
} uart_recv_send_last_data_t;                           /* Record the last data sent and received by the UART */

typedef struct uart_drv_stat_info {
    unsigned int uart_errno;
    unsigned int recv_irq_cnt;
    unsigned int recv_irq_data_cnt;
    unsigned int recv_irq_data_suc_cnt;
    unsigned int recv_buf_overflow_cnt;
    unsigned int read_circ_cnt;
    unsigned int send_irq_cnt;
    unsigned int send_irq_data_cnt;
    unsigned int write_circ_cnt;

    unsigned int recv_irq_err_overrun;
    unsigned int recv_irq_err_parity;
    unsigned int recv_irq_err_frame;
    unsigned int recv_irq_err_break;
    unsigned int recv_irq_err_busy;
    unsigned int recv_irq_err_emptyfifo_cnt;

    uart_recv_send_last_data_t recv_last_context;
    uart_recv_send_last_data_t send_last_context;
} uart_drv_stat_info_t;

struct uart_driver_data {
#ifdef UART_DEBUG_INFO
    uart_drv_stat_info_t uart_stat_info;
#endif
    unsigned int num;
    uart_mode type;
    unsigned int phys_base;
    unsigned int irq_num;
    uart_circ_buf_t *rx_transfer;
    uart_circ_buf_t *tx_transfer;
    unsigned short flags;
    unsigned short usr_flag; /* Used for user layer to pass information for drive control */
    EVENT_CB_S uart_event;
    int count;
    int state;
    recv_notify rx_recv;
    send_buf tx_send;
    uart_ops_t *ops;
    uart_attr_t attr;
};

#define uart_error(msg...)                                 \
    do {                                                   \
        dprintf("\n");                                     \
        dprintf("<uart,err>:%s,%d: ", __func__, __LINE__); \
        dprintf(msg);                                      \
        dprintf("\n");                                     \
    } while (0)

void uart_tx_interrupt_enable(EXT_CONST uart_driver_data_t *udd);
int uart_circ_buf_empty(EXT_CONST uart_circ_buf_t *transfer);
unsigned int uart_init_circ_buf(uart_driver_data_t *udd, unsigned int rx_fifo_size, unsigned int tx_fifo_size);
void uart_deinit_circ_buf(uart_driver_data_t *udd);
int uart_read_circ_buf(uart_circ_buf_t *transfer, char *buf, size_t count);
int uart_write_circ_buf(uart_circ_buf_t *transfer, const char *buf, size_t count);
void uart_set_tx_mode(uart_driver_data_t *udd);
unsigned char uart_rx_fifo_num(EXT_CONST uart_driver_data_t *udd);
void uart_tx_interrupt_disable(EXT_CONST uart_driver_data_t *udd);
void uart_tf_interrupt_disable(EXT_CONST uart_driver_data_t *udd);
void uart_rx_interrupt_disable(EXT_CONST uart_driver_data_t *udd);

extern uart_ops_t g_uart_driver_uops;

#ifdef __cplusplus
}
#endif
#endif /* __UART_DRV_H__ */
