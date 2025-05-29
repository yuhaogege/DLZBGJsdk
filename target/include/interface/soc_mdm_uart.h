/**
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description:  UART Port APIs.
 * Author: CompanyName
 * Create: 2018-10-31
 */
/**
 * @defgroup hct_uart UART Port
 * @ingroup drivers
 */

#ifndef SOC_MDM_UART_H
#define SOC_MDM_UART_H

#include <soc_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup hct_uart
 * UART serial number.
 */
typedef enum {
    EXT_UART_0,  /**< Physical port number 0. */
    EXT_UART_1,  /**< Physical port number 1. */
    EXT_UART_MAX /**< Maximum physical port number, which cannot be used. */
} ext_uart_port;

/**
 * @ingroup hct_uart
 * UART port configuration parameters.
 */
typedef struct {
    td_u32 baud_rate; /**< Baud rate. */
    td_u32 data_bits; /**< Data bit. The value can be 6 bits, 7 bits, or 8 bits. */
    td_u32 stop_bits; /**< Stop bit. 1: 1 stop bit; 2: 2 stop bit. */
    td_u32 parity;    /**< Parity bit.
                       * 0: No check is performed.
                       * 1: Odd parity check is performed;
                       * 2: Even parity check is performed.
                       */
} ext_uart_init_type;

#define EXT_UART_CFG_SET_ATTR 0x101
#define EXT_UART_CFG_RD_BLOCK 0x102
#define EXT_UART_CFG_WD_BLOCK 0x103
#define EXT_UART_CFG_GET_ATTR 0x104

#define EXT_UART_RD_BLOCK 1
#define EXT_UART_RD_NONBLOCK 0
#define EXT_UART_WD_BLOCK 1
#define EXT_UART_WD_NONBLOCK 0

#define EXT_UART_STOP_BIT_1 0x00   /**< UART Stop bit setting (register value): 1 stop bit. */
#define EXT_UART_STOP_BIT_1P5 0x04 /**< UART Stop bit setting (register value): 1.5 stop bits.   \
                                   * The setting value is the same as 2 stop bits.               \
                                   * When the data length is 5 bits, it indicates 1.5 stop bits. \
                                   */
#define EXT_UART_STOP_BIT_2 0x04   /**< UART Stop bit setting (register value): 2 stop bits. */

#define EXT_UART_PARITY_NONE 0x00 /**< UART Parity check setting (register value): no check. */
#define EXT_UART_PARITY_ODD 0x08  /**< UART Parity check setting (register value): odd parity check. */
#define EXT_UART_PARITY_EVEN 0x18 /**< UART Parity check setting (register value): even parity check. */

typedef struct {
    td_u32 baudrate; /**< Baud rate. */
    td_u8 data_bits; /**< Number of data bits. The value can be 6 bits, 7 bits, or 8 bits. */
    td_u8 stop_bits; /**< Number of stop bits (register configuration). */
    td_u8 parity;    /**< Parity check flag. */
    td_u8 reserved;  /**< Reserved. */
} ext_uart_attr;

/**
 * @ingroup hct_uart
 * UART maintenance and testing statistics parameters.
 * Number of bytes sent and received at the end of the UART maintenance and testing.
 */
#define EXT_UART_LAST_RECORD_BYTE_COUNT 32

/**
 * @ingroup hct_uart
 * UART maintenance and testing statistics.
 */
typedef struct {
    td_u32 rx_irq_cnt;               /**< Number of RX interrupts. */
    td_u32 rx_data_cnt;              /**< Number of bytes received in the RX interrupt. */
    td_u32 rx_data_suc_cnt;          /**< Number of data bytes that are successfully received in
                                      * the RX interrupt (that is, data after the software buffer
                                      * overflow is removed from rx_data_cnt).
                                      */
    td_u32 rx_data_buf_overflow_cnt; /**< Number of times that software buffer overflow occurs in
                                      * the RX interrupt.
                                      */
    td_u32 tx_irq_cnt;               /**< Number of TX interrupts. */
    td_u32 tx_data_cnt;              /**< Number of bytes transmitted in the TX interrupt. */

    td_u32 err_overrun; /**< Number of RX overflow errors. */
    td_u32 err_parity;  /**< Number of RX parity check errors. */
    td_u32 err_frame;   /**< Number of RX frame errors. */
    td_u32 err_break;   /**< Number of RX break errors. */
    td_u32 err_busy;    /**< Number of RX busy errors. */

    td_u32 last_rx_sn;                                  /**< ID of the last received data. */
    td_u8 last_rx_data[EXT_UART_LAST_RECORD_BYTE_COUNT]; /**< RX buffer. */

    td_u32 last_tx_sn;                                  /**< ID of the last transmitted data. */
    td_u8 last_tx_data[EXT_UART_LAST_RECORD_BYTE_COUNT]; /**< TX buffer. */
} ext_uart_dfx_stat;

/**
 * @ingroup  hct_uart
 * @brief  Opens the UART.
 *
 * @par Description:
 * Opens the specified UART based on the configured operating mode.
 *
 * @attention None
 * @param  port [IN] Type #ext_uart_port UART port number.
 *
 * @retval #EXT_ERR_SUCCESS  Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_uart.h: This file describes UART APIs.
 * @see  uapi_uart_close
 * @since DW21_V100R001C00
 */
td_u32 uapi_uart_open(ext_uart_port port);

/**
 * @ingroup  hct_uart
 * @brief  Reads data.
 *
 * @par Description:
 * Reads the data received by the UART. The block mode is used by default.
 *
 * @attention This API must be used after the uapi_uart_open function is called.
 * @param  port     [IN]  Type #ext_uart_port UART port number.
 * @param  data     [OUT] Type #td_u8* Start address of the data to be read.
 * @param  data_len [IN]  Type #td_u32 Number of bytes to be read.
 *
 * @retval #>=0 Number of bytes that are actually read.
 * @retval #< 0 Data read error.
 * @par Dependency:
 * @li soc_mdm_uart.h: This file describes UART APIs.
 * @see  uapi_uart_write
 * @since DW21_V100R001C00
 */
td_s32 uapi_uart_read(ext_uart_port port, td_u8 *data, td_u32 data_len);

/**
 * @ingroup  hct_uart
 * @brief  Writes data.
 *
 * @par Description:
 * Writes the data to be sent to the UART. The block mode is used by default.
 *
 * @attention This API must be used after the uapi_uart_open function is called.
 * @param  port     [IN] Type #ext_uart_port UART port number.
 * @param  data     [IN] Type #td_u8* Start address of the data to be written.
 * @param  data_len [IN] Type #td_u32 Number of bytes to be written.
 *
 * @retval #>=0 Number of bytes to be sent.
 * @retval #< 0 Data send failure.
 * @par Dependency:
 * @li soc_mdm_uart.h: This file describes UART APIs.
 * @see  uapi_uart_read
 * @since DW21_V100R001C00
 */
td_s32 uapi_uart_write(ext_uart_port port, EXT_CONST td_u8 *data, td_u32 data_len);

/**
 * @ingroup hct_uart
 * ioctl parameter.
 * Sets the communication parameters of the UART port.
 */
#define UART_CFG_SET_ATTR 0x101

/**
 * @ingroup hct_uart
 * ioctl parameter.
 * Sets the blocking read of the UART port.
 */
#define UART_CFG_RD_BLOCK 0x102

/**
 * @ingroup hct_uart
 * ioctl parameter.
 * Sets the blocking write of the UART port.
 */
#define UART_CFG_WD_BLOCK 0x103

/**
 * @ingroup hct_uart
 * ioctl parameter.
 * Obtains the communication parameters of the UART port.
 */
#define UART_CFG_GET_ATTR 0x104

/**
 * @ingroup  hct_uart
 * @brief  Functions related to UART control.
 *
 * @par Description:
 * Functions related to UART configuration.
 *
 * @attention This API must be used after the uapi_uart_open function is called.
 * @param  port    [IN]     Type #ext_uart_port UART port number.
 * @param  request [IN]     Type #td_u32 Command type. The value is UART_CFG_XXX.
 * @param  attr    [IN/OUT] Type #uintptr_t Parameter used together with the command type.
 * @li UART_CFG_SET_ATTR£ºSets the attributes such as the baud rate of the UART port.
 * attr is the pointer that points to the ext_uart_init_type_s structure.
 * @li UART_CFG_GET_ATTR£ºObtains the attributes such as the baud rate of the UART port.
 * attr is the pointer that points to the ext_uart_init_type_s structure.
 * @li UART_CFG_RD_BLOCK£ºSets the blocking or non-blocking read mode of the UART port.
 * attr is the pointer that points to td_u32. The value of td_u32 is
 * UART_RD_BLOCK or UART_RD_NONBLOCK.
 * @li UART_CFG_WD_BLOCK£ºSets the blocking or non-blocking write mode of the UART port.
 * attr is the pointer that points to td_u32. The value of td_u32 is
 * UART_WD_BLOCK or UART_WD_NONBLOCK.
 * @retval #EXT_ERR_SUCCESS  Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_uart.h: This file describes UART APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
td_u32 uapi_uart_ioctl(ext_uart_port port, td_u32 request, uintptr_t attr);

/**
 * @ingroup  hct_uart
 * @brief  Closes the UART.
 *
 * @par Description:
 * Closes the UART.
 *
 * @attention This API is used together with uapi_uart_open.
 * @param  port    [IN] Type #ext_uart_port UART port number.
 *
 * @retval #EXT_ERR_SUCCESS  Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_uart.h: This file describes UART APIs.
 * @see  uapi_uart_open
 * @since DW21_V100R001C00
 */
td_u32 uapi_uart_close(ext_uart_port port);

/**
 * @ingroup  hct_uart
 * @brief  UART maintenance and test function.
 *
 * @par Description:
 * Obtains UART parameters and statistics for maintenance and test.
 *
 * @attention Access the UART port directly. There is no permission control.
 * The UART port can be used only for maintenance and testing.
 * @param  port    [IN]  Type #ext_uart_port Number of the UART port whose maintenance and test information is to
 * be obtained.
 * @param  cfg     [OUT] Type #ext_uart_init_type* UART configuration information that is obtained.
 * @param  stat    [OUT] Type #ext_uart_dfx_stat* UART statistics information that is obtained.
 *
 * @retval #EXT_ERR_SUCCESS  Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_uart.h: This file describes UART APIs.
 * @see  uapi_uart_open
 * @since DW21_V100R001C00
 */
td_u32 uapi_uart_dfx_get_stat(ext_uart_port port, ext_uart_init_type *cfg, ext_uart_dfx_stat *stat);

#ifdef __cplusplus
}
#endif
#endif /* SOC_MDM_UART_H */
