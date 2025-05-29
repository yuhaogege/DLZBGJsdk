/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Adjust the directory structure to prepare for compiling the SDK. uart.c code
 * Author: CompanyName
 * Create: 2012-07-14
 */

#include "uart.h"
#include "los_event.h"
#include "string.h"
#include "soc_mdm_uart.h"
#include "los_hwi.h"
#include "uart_drv.h"

#include <soc_stdlib.h>
#include <soc_lowpower.h>
#include <dw21_platform_rom.h>

static uart_driver_data_t g_uart_0 = {
    .num = UART0,
    .phys_base = EXT_UART0_REG_BASE,
    .irq_num = Uart0_IRQn,
    .rx_transfer = TD_NULL,
    .tx_transfer = TD_NULL,
    .rx_recv = uart_write_circ_buf,
    .tx_send = uart_read_circ_buf,
    .flags = UART_FLG_RD_BLOCK | UART_FLG_WD_BLOCK,
    .count = 0,
    .state = UART_STATE_NOT_OPENED,
    .ops = &g_uart_driver_uops,
    .attr = UART0_ATTR_DEFAULT,
};

static uart_driver_data_t g_uart_1 = {
    .num = UART1,
    .phys_base = EXT_UART1_REG_BASE,
    .irq_num = Uart1_IRQn,
    .rx_transfer = TD_NULL,
    .tx_transfer = TD_NULL,
    .rx_recv = uart_write_circ_buf,
    .tx_send = uart_read_circ_buf,
    .flags = UART_FLG_RD_BLOCK | UART_FLG_WD_BLOCK,
    .count = 0,
    .state = UART_STATE_NOT_OPENED,
    .ops = &g_uart_driver_uops,
    .attr = UART_ATTR_DEFAULT,
};

uart_driver_data_t *g_udd_g[UART_NUM] = { &g_uart_0, &g_uart_1 };

EXT_PRV td_bool check_commit_to_sleep(td_u32 uart_index)
{
    uart_driver_data_t *udd = TD_NULL;

    udd = (uart_driver_data_t *)g_udd_g[uart_index];
    if (udd->state == UART_STATE_USEABLE) {
        if (uart_rx_fifo_num(udd) != 0) {
            return TD_FALSE;
        }
    }

    return TD_TRUE;
}

__init uart_driver_data_t *uart_open(int uart_index, uart_mode mode)
{
    unsigned int ret;
    uart_driver_data_t *udd = TD_NULL;
    /* Compare with UART_232, not direct use != , Convenient to modify only the lower boundary */
    if (uart_index < 0 || uart_index >= UART_NUM || mode < UART_232 || mode > UART_232) {
        return TD_NULL;
    }
    udd = (uart_driver_data_t *)g_udd_g[uart_index];

    if (udd->state == UART_STATE_NOT_OPENED) {
        ext_lowpower_check_id check_;

        ret = uart_init_circ_buf(udd, CONFIG_RX_BUF_SIZE, CONFIG_TX_BUF_SIZE);
        if (ret != EXT_ERR_SUCCESS) {
            uart_set_err_no(UART_ERR_INIT_CIRC_FAILED);
            return TD_NULL;
        }
        ret = LOS_EventInit(&udd->uart_event);
        if (ret != EXT_ERR_SUCCESS) {
            uart_deinit_circ_buf(udd);
            return TD_NULL;
        }
        udd->type = mode;

        if (!udd->ops->startup || udd->ops->startup(udd)) {
            uart_deinit_circ_buf(udd);
            uart_set_err_no(UART_ERR_START_FAILED);
            return TD_NULL;
        }
        check_.deep = TD_TRUE;
        check_.light = TD_TRUE;
        if (uart_index == 0) {
            check_.id = EXT_LOWPOWER_ID_UART0;
        } else {
            check_.id = EXT_LOWPOWER_ID_UART1;
        }
        uapi_lowpower_register_checkfunc(check_, check_commit_to_sleep, (unsigned int)uart_index);
        udd->state = UART_STATE_USEABLE;
    } else {
        if (mode != udd->type) {
            if (udd->ops->startup != TD_NULL) {
                udd->type = mode;
                udd->ops->startup(udd);
            }
        }
    }

    return udd;
}

unsigned int uart_read(uart_driver_data_t *udd, char *buf, size_t count)
{
    int len;

    if (udd == TD_NULL || udd->rx_transfer == TD_NULL) {
        return EXT_ERR_FAILURE;
    }
    if ((buf == TD_NULL) || (count == 0)) {
        uart_set_err_no(UART_ERR_PARA_INVALID);
        return EXT_ERR_FAILURE;
    }

    if (udd->state != UART_STATE_USEABLE) {
        uart_set_err_no(UART_ERR_NOT_OPENED);
        return EXT_ERR_FAILURE;
    }

    do {
        len = uart_read_circ_buf(udd->rx_transfer, buf, count);
#ifdef UART_DEBUG_INFO
        udd->uart_stat_info.read_circ_cnt += (unsigned int)len;
#endif
        if (len == 0) {
            if (udd->flags & UART_FLG_RD_BLOCK) {
                (void)LOS_EventRead(&udd->uart_event, UART_RD_EVENT, LOS_WAITMODE_OR | LOS_WAITMODE_CLR,
                    LOS_WAIT_FOREVER);
            } else {
                break;
            }
        }
    } while (len == 0);

    return (td_u32)len;
}

unsigned int uart_write(uart_driver_data_t *udd, const char *buf, size_t count)
{
    int len = 0;
    int left;
    /* if uart is not in useable, report error */
    if (udd == TD_NULL || udd->tx_transfer == TD_NULL) {
        return EXT_ERR_FAILURE;
    }

    if ((buf == TD_NULL) || (count == 0)) {
        uart_set_err_no(UART_ERR_PARA_INVALID);
        return EXT_ERR_FAILURE;
    }
    left = (int)count;
    if (udd->state != UART_STATE_USEABLE) {
        uart_set_err_no(UART_ERR_NOT_OPENED);
        return EXT_ERR_FAILURE;
    }

#ifdef UART_CIRC_BUF_ENABLE
    do {
        len += uart_write_circ_buf(udd->tx_transfer, (char *)&buf[len], (unsigned int)left);
        if (len < (int)count) {
            left = (int)count - len;

            if (udd->flags & UART_FLG_WD_BLOCK) {
                uart_tx_interrupt_enable(udd);
                (void)LOS_EventRead(&udd->uart_event, UART_WD_EVENT, LOS_WAITMODE_OR | LOS_WAITMODE_CLR,
                    LOS_WAIT_FOREVER);
            } else {
                break;
            }
        } else {
            break;
        }
    } while (left > 0);
    uart_tx_interrupt_enable(udd);
#else
    if (udd->ops->start_tx) {
        len = udd->ops->start_tx(udd, buf, count);
    }
#endif

#ifdef UART_DEBUG_INFO
    udd->uart_stat_info.write_circ_cnt += (unsigned int)len;
#endif

    return (td_u32)len;
}
__init bool uart_attr_valued(EXT_CONST uart_attr_t *attr)
{
    if (attr && (attr->baudrate <= CONFIG_MAX_BAUDRATE) && (attr->baudrate > 0) && attr->data_bits <= 8 &&
        attr->data_bits >= 5 && /* Data bit 8bit or 5bit */
        (attr->parity == PARITY_NONE || attr->parity == PARITY_ODD || attr->parity == PARITY_EVEN) &&
        (attr->stop_bits == STOP_BIT_1 || attr->stop_bits == STOP_BIT_1P5 || attr->stop_bits == STOP_BIT_2)) {
        return TD_TRUE;
    }
    return TD_FALSE;
}

#define set_err_code(type)     \
    do {                       \
        uart_set_err_no(type); \
        ret = EXT_ERR_FAILURE;  \
    } while (0)

__init unsigned int uart_ioctl(uart_driver_data_t *udd, int cmd, unsigned long arg)
{
    uart_attr_t *attr = TD_NULL;
    unsigned int ret = EXT_ERR_SUCCESS;

    if (udd->state != UART_STATE_USEABLE) {
        uart_set_err_no(UART_ERR_NOT_OPENED);
        return EXT_ERR_FAILURE;
    }

    switch (cmd) {
        case UART_CFG_SET_ATTR:
            attr = (uart_attr_t *)(uintptr_t)arg;
            if (uart_attr_valued(attr) == TD_TRUE) {
                (td_void)memcpy_s(&udd->attr, sizeof(udd->attr), attr, sizeof(uart_attr_t));
                if ((udd->ops == TD_NULL) || (udd->ops->ioctl == TD_NULL) || udd->ops->ioctl(udd)) {
                    set_err_code(UART_ERR_IOCTL_FAILED);
                }
            } else {
                set_err_code(UART_ERR_PARA_INVALID);
            }
            break;
        case UART_CFG_RD_BLOCK:
            if (arg == UART_RD_BLOCK) {
                udd->flags |= UART_FLG_RD_BLOCK;
            } else if (arg == UART_RD_NONBLOCK) {
                udd->flags &= ~UART_FLG_RD_BLOCK;
                (void)LOS_EventWrite(&udd->uart_event, UART_RD_EVENT);
            }
            break;
        case UART_CFG_WD_BLOCK:
            if (arg == UART_WD_BLOCK) {
                udd->flags |= UART_FLG_WD_BLOCK;
            } else if (arg == UART_WD_NONBLOCK) {
                udd->flags &= ~UART_FLG_WD_BLOCK;
                (void)LOS_EventWrite(&udd->uart_event, UART_WD_EVENT);
            }
            break;
        case UART_CFG_GET_ATTR:
            if (memcpy_s((uart_attr_t *)(uintptr_t)arg, sizeof(uart_attr_t), &udd->attr, sizeof(udd->attr)) != EOK) {
                break;
            }
            break;
        default:
            set_err_code(UART_ERR_PARA_INVALID);
            break;
    }
    return ret;
}

__init unsigned int uart_close(uart_driver_data_t *udd)
{
    if (udd == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    if (udd->state != UART_STATE_USEABLE) {
        uart_set_err_no(UART_ERR_NOT_OPENED);
        return EXT_ERR_FAILURE;
    }

    uart_tf_interrupt_disable(udd);
    uart_rx_interrupt_disable(udd);
    uart_tx_interrupt_disable(udd);

    return EXT_ERR_SUCCESS;
}
