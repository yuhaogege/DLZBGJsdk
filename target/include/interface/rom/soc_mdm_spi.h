/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * @defgroup hct_spi SPI
 * @ingroup drivers
 */

#ifndef __SOC_MDM_SPI_H__
#define __SOC_MDM_SPI_H__
#include <soc_types_rom.h>
#include <soc_mdm_types_rom.h>

#define REG_SPI0_BASE_ADDR 0x40035000 // Base address of the SPI controller
#define REG_SPI1_BASE_ADDR 0x4003e000 // Base address of the SPI controller

/*
 * @ingroup hct_spi
 *
 * Communication polarity
 */
typedef enum {
    EXT_SPI_CFG_CLOCK_CPOL_0, /* < Polarity 0 */
    EXT_SPI_CFG_CLOCK_CPOL_1, /* < Polarity 1 */
} ext_spi_cfg_clock_cpol_e;

/*
 * @ingroup hct_spi
 *
 * Communication phase
 */
typedef enum {
    EXT_SPI_CFG_CLOCK_CPHA_0, /* < Phase 0 */
    EXT_SPI_CFG_CLOCK_CPHA_1, /* < Phase 1 */
} ext_spi_cfg_clock_cpha_e;

/*
 * @ingroup hct_spi
 *
 * Communication protocol type
 */
typedef enum {
    EXT_SPI_CFG_FRAM_MODE_MOTOROLA,  /* < Motorola protocol */
    EXT_SPI_CFG_FRAM_MODE_TI,        /* < Texas Instruments protocol */
    EXT_SPI_CFG_FRAM_MODE_MICROWIRE, /* < Microwire protocol */
} ext_spi_cfg_fram_mode_e;

/*
 * @ingroup hct_spi
 *
 * Communication bit width, that is, number of valid bits in each frame
 */
typedef enum {
    EXT_SPI_CFG_DATA_WIDTH_E_4BIT = 0x3, /* < The bit width is 4 bits. */
    EXT_SPI_CFG_DATA_WIDTH_E_5BIT,       /* < The bit width is 5 bits. */
    EXT_SPI_CFG_DATA_WIDTH_E_6BIT,       /* < The bit width is 6 bits. */
    EXT_SPI_CFG_DATA_WIDTH_E_7BIT,       /* < The bit width is 7 bits. */
    EXT_SPI_CFG_DATA_WIDTH_E_8BIT,       /* < The bit width is 8 bits. */
    EXT_SPI_CFG_DATA_WIDTH_E_9BIT,       /* < The bit width is 9 bits. */
    EXT_SPI_CFG_DATA_WIDTH_E_10BIT,      /* < The bit width is 10 bits. */
    EXT_SPI_CFG_DATA_WIDTH_E_11BIT,      /* < The bit width is 11 bits. */
    EXT_SPI_CFG_DATA_WIDTH_E_12BIT,      /* < The bit width is 12 bits. */
    EXT_SPI_CFG_DATA_WIDTH_E_13BIT,      /* < The bit width is 13 bits. */
    EXT_SPI_CFG_DATA_WIDTH_E_14BIT,      /* < The bit width is 14 bits. */
    EXT_SPI_CFG_DATA_WIDTH_E_15BIT,      /* < The bit width is 15 bits. */
    EXT_SPI_CFG_DATA_WIDTH_E_16BIT,      /* < The bit width is 16 bits. */
} ext_spi_cfg_data_width_e;

/*
 * @ingroup hct_spi
 *
 * Communication parameter: big-endian and little-endian transmission of each frame
 */
typedef enum {
    EXT_SPI_CFG_ENDIAN_LITTLE, /* < Little-endian transmission */
    EXT_SPI_CFG_ENDIAN_BIG,    /* < Big-endian transmission */
} ext_spi_cfg_endian_e;

/*
 * @ingroup hct_spi
 *
 * Communication parameter: Mircrowire frame wait enable
 */
typedef enum {
    EXT_SPI_CFG_WAIT_EN_DISABLE, /* < Wait disable */
    EXT_SPI_CFG_WAIT_EN_ENABLE,  /* < Wait enable */
} ext_spi_cfg_wait_en_e;

/*
 * @ingroup hct_spi
 *
 * Communication parameter: loopback mode
 */
typedef enum {
    EXT_SPI_CFG_LBM_DISABLE, /* < Loopback disable */
    EXT_SPI_CFG_LBM_ENABLE,  /* < Loopback enable */
} ext_spi_cfg_lbm_en_e;

/*
 * @ingroup hct_spi
 *
 * Communication parameter: master and slave devices
 */
typedef enum {
    EXT_SPI_CFG_ROLE_MASTER, /* < Master device */
    EXT_SPI_CFG_ROLE_SLAVE,  /* < Slave device */
} ext_spi_cfg_role_e;

/*
 *
 * The comment is deleted and the code is to be rectified.
 */
typedef enum {
    EXT_SPI_ID_0 = 0,
    EXT_SPI_ID_1,
} ext_spi_id_e;

/*
 * @ingroup hct_spi
 *
 * Communication parameter: SPI TX FIFO threshold. Here, the byte refers to a special TX FIFO unit rather
 * than the byte unit which is generally considered to be equal to 8 bits.
 */
typedef enum {
    EXT_SPI_TX_FIFO_WATER_LINE_1,  /* < The TX FIFO threshold is 1 byte. */
    EXT_SPI_TX_FIFO_WATER_LINE_4,  /* < The TX FIFO threshold is 4 bytes. */
    EXT_SPI_TX_FIFO_WATER_LINE_8,  /* < The TX FIFO threshold is 8 bytes. */
    EXT_SPI_TX_FIFO_WATER_LINE_16, /* < The TX FIFO threshold is 16 bytes. */
    EXT_SPI_TX_FIFO_WATER_LINE_32, /* < The TX FIFO threshold is 32 bytes. */
    EXT_SPI_TX_FIFO_WATER_LINE_48, /* < The TX FIFO threshold is 48 bytes. */
    EXT_SPI_TX_FIFO_WATER_LINE_56, /* < The TX FIFO threshold is 56 bytes. */
    EXT_SPI_TX_FIFO_WATER_LINE_64, /* < The TX FIFO threshold is 64 bytes. */
} ext_spi_tx_fifo_water_line_e;

/*
 * @ingroup hct_spi
 *
 * Communication parameter: SPI RX FIFO threshold. Here, the byte refers to a special TX FIFO unit
 * rather than the byte unit which is generally considered to be equal to 8 bits.
 */
typedef enum {
    EXT_SPI_RX_FIFO_WATER_LINE_RESERVE, /* < Reserved */
    EXT_SPI_RX_FIFO_WATER_LINE_62,      /* < The RX FIFO threshold is 62 bytes. */
    EXT_SPI_RX_FIFO_WATER_LINE_48,      /* < The RX FIFO threshold is 48 bytes. */
    EXT_SPI_RX_FIFO_WATER_LINE_32,      /* < The RX FIFO threshold is 32 bytes. */
    EXT_SPI_RX_FIFO_WATER_LINE_16,      /* < The RX FIFO threshold is 16 bytes. */
    EXT_SPI_RX_FIFO_WATER_LINE_8,       /* < The RX FIFO threshold is 8 bytes. */
    EXT_SPI_RX_FIFO_WATER_LINE_4,       /* < The RX FIFO threshold is 4 bytes. */
    EXT_SPI_RX_FIFO_WATER_LINE_1,       /* < The RX FIFO threshold is 1 byte. */
} ext_spi_rx_fifo_water_line_e;

/*
 * @ingroup hct_spi
 *
 * Communication parameter: SPI software buffer type
 */
typedef enum {
    EXT_SPI_SOFT_BUF_DATA_WIDTH_1_BYTES, /* < SPI communication bit width EXT_SPI_CFG_DATA_WIDTH_E_4BIT
                                                to EXT_SPI_CFG_DATA_WIDTH_E_8BIT */
    EXT_SPI_SOFT_BUF_DATA_WIDTH_2_BYTES, /* < SPI communication bit width EXT_SPI_CFG_DATA_WIDTH_E_9BIT
                                                to EXT_SPI_CFG_DATA_WIDTH_E_16BIT */
} ext_spi_soft_buf_data_width_e;

/*
 * @ingroup  hct_spi
 * @brief  Type of the SPI callback function
 *
 * @par Description:
 * Type of the SPI callback function
 *
 * @attention None
 * @param  data [IN] Type #td_u32, callback input parameter
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_spi.h: This file describes the SPI APIs.
 * @see None
 * @since DW21_V100R001C00
 */
typedef td_void (*spi_usr_func)(td_u32 data);

/*
 * @ingroup hct_spi
 *
 * Data communication parameter
 */
typedef struct {
    td_u16 data_width : 4; /* < Data bit width. For details about the value, see ext_spi_cfg_data_width_e. */
    td_u16 fram_mode : 2;  /* < Data protocol. For details about the value, see ext_spi_cfg_fram_mode_e. */
    td_u16 spo : 1;        /* < Polarity. For details about the value, see ext_spi_cfg_clock_cpol_e. */
    td_u16 sph : 1;        /* < Phase. For details about the value, see ext_spi_cfg_clock_cpha_e. */
    td_u16 scr : 8;        /* < Clock rate: one of the factors that determine the SPI clock */
    /* < Whether the loopback mode is enabled. For details about the value, see EXT_SPI_CFG_LBM_ENABLE. */
    td_u16 loop_back : 1;
    td_u16 reserver_1 : 1; /* < Reserved */
    td_u16 b_slave : 1;    /* < Master/slave mode. For details about the value, see ext_spi_cfg_role_e. */
    td_u16 reserver_2 : 1;
    td_u16 endian : 1; /* < Big and little endian. For details, see ext_spi_cfg_endian_e. */
    td_u16 reserver_3 : 3;
    td_u16 wait_val : 7; /* < Wait clock cycle between write and read: used for the Microwire protocol */
    /* < Wait enable: used for the Microwire protocol. For details about the value, see ext_spi_cfg_wait_en_e */
    td_u16 wait_en : 1;

    td_u16 cpsdvsr; /* < Clock rate: one of the factors that determine the SPI clock */
    /* < TX data when the SPI is used as the master device and data is received in simplex mode */
    td_u16 fix_data_val;
    spi_usr_func prepare_func; /* < User preparation function before data communication */
    spi_usr_func restore_func; /* < User restoration function after data communication */
} ext_spi_device_usr_cfg_s;

/*
 * @ingroup hct_spi
 *
 * Structure used internally in the SPI module. Initialize it to 0.
 */
typedef struct {
    td_u16 cr0;                /* < Used internally in the SPI module */
    td_u16 cr1;                /* < Used internally in the SPI module */
    td_u16 cpsdvsr;            /* < Used internally in the SPI module */
    td_u16 fix_data_val;       /* < Used internally in the SPI module */
    spi_usr_func prepare_func; /* < Used internally in the SPI module */
    spi_usr_func restore_func; /* < Used internally in the SPI module */
} ext_spi_device_inner_cfg_s;

/*
 * @ingroup hct_spi
 *
 * Structure used internally in the SPI module, indicating the software handle description
 */
typedef struct {
    ext_spi_device_inner_cfg_s usr_cfg; /* < Used internally in the SPI module */
    /*
     * < Used internally in the SPI module,
     * indicating whether the software handle is available
     */
    td_bool b_used;
    td_u8 hw_spi_id; /* < Used internally in the SPI module, indicating the hardware handle ID */
    td_u8 opt_id;    /* < Used internally in the SPI module, indicating the software handle ID */
    /*
     * < Used internally in the SPI module, indicating whether it is the first time to
     * use the read/write interface after the handle is opened
     */
    td_bool b_first;
} ext_spi_sw_dev_s;

/*
 * @ingroup hct_spi
 *
 * Buffer structure used internally in the SPI module
 */
typedef struct {
    td_pvoid p_buf;      /* < Buffer pointer */
    td_u16 write_pos;    /* < Current write position */
    td_u16 read_pos;     /* < Current read position */
    td_u16 total_len;    /* < Total buffer length (unit: byte) */
    td_u16 cur_used_len; /* < Current buffer length (unit: byte) */
} ext_spi_sfot_buf;

/*
 * @ingroup hct_spi
 *
 * Hardware description used internally in the SPI module
 */
typedef struct {
    td_u32 reg_base;        /* < SPI register base address */
    td_u32 sem_id;          /* < SPI mutually exclusive semaphores */
    td_u32 event_id;        /* < SPI read/write synchronization event ID */
    ext_spi_sfot_buf tx_buf; /* < SPI write buffer */
    ext_spi_sfot_buf rx_buf; /* < SPI read buffer */

    td_u8 irq_num;     /* < SPI interrupt ID */
    td_u8 last_opt_id; /* < Software handle ID corresponding to the SPI enable configuration */

    td_u16 b_useable : 1;       /* < Availability */
    td_u16 data_width_byte : 1; /* < For details about the software buffer type, see ext_spi_soft_buf_data_width_e. */
    td_u16 b_operationing : 1;  /* < Reserved. The value must be 0. */
    td_u16 rx_fifo_line : 3;    /* < RX FIFO threshold */
    td_u16 tx_fifo_line : 3;    /* < TX FIFO threshold */
    td_u16 pad : 7;             /* < Reserved */
} ext_spi_hw_dev_s;

/*
 * @ingroup hct_spi
 *
 * Control structure of the SPI module
 */
typedef struct {
    td_u8 hw_device_count;      /* < Number of SPI hardware devices */
    td_u8 sw_device_count;      /* < Number of SPI software handles */
    ext_spi_hw_dev_s *p_hw_devs; /* < Hardware description used internally in the SPI module */
    ext_spi_sw_dev_s *p_sw_devs; /* < Software description used internally in the SPI module */
} ext_spi_ctrl_s;

/*
 *
 * This API is used internally and supports secondary development.
 */
typedef enum {
    EXT_SPI_OPT_SET_CFG = 0x1,
    EXT_SPI_OPT_ENABLE_SPI = 0x2,    /* SPI enable before operation */
    EXT_SPI_OPT_DISABLE_SPI = 0x4,   /* SPI disable after operation */
    EXT_SPI_OPT_TASKED_SIGNAL = 0x8, /* Internal status. The API is unavailable for the user. */
    /*
     * Operation option for the master device to receive data in
     * half-duplex mode: Select this option when the master
     * device receives data in half-duplex mode.
     */
    EXT_SPI_OPT_SEND_FIX_DATA = 0x10,
    /*
     * Operation option for the master device to send data in half-duplex mode:
     * Select this option when the master device sends data in half-duplex mode.
     */
    EXT_SPI_OPT_RCV_FIX_DATA = 0x20,
    /*
     * Operation option for sending data: whether the operation
     * semaphore (permission) needs to be obtained before the operation
     */
    EXT_SPI_OPT_WAIT_SIGNAL = 0x40,
    /*
     * Operation option for sending data: whether the semaphore
     * (permission) needs to be released after the operation
     */
    EXT_SPI_OPT_FREE_SIGNAL = 0x80,
} EXT_SPI_OPT_E;

/*
 *
 * This API is used internally and supports secondary development.
 */
#define EXT_EVENT_BIT_RX_DATA 0x1          // RX data synchronization event
#define EXT_EVENT_BIT_TX_DATA 0x2          // TX data synchronization event
#define EXT_EVENT_BIT_RX_DATA_TIME_OUT 0x4 // RX data synchronization event

/*
 * @ingroup  hct_spi
 * @brief TX interface for the SPI slave mode
 *
 * @par Description:
 * TX interface for the SPI slave mode
 *
 * @attention None
 * @param  fd               [IN]  Type #td_u32, device handle
 * @param  p_send_data      [IN]  Type #td_pvoid, TX data pointer
 * @param  send_len         [IN]  Type #td_u32, length of the target data to be sent (unit: byte)
 * @param  p_real_send_len  [OUT] Type #td_u32*, length of the data that is actually successfully sent (unit: byte)
 * @param  usr_data         [IN]  Type #td_u32, parameter which is transferred to the pre-registered user preparation
 * function (spi_usr_func prepare_func) and user recovery function (spi_usr_func restore_func). The callback function is
 * configured by the input parameters prepare_func and restore_func of the uapi_spi_config interface.
 * @param  time_out_ms      [IN]  Type #td_u32, wait timeout period (unit: ms). This period refers to the continuous
 * blocking duration of a certain operation, rather than the total operation time.
 * The wait operation refers to waiting for the semaphore, waiting for the
 * chip to send data, or waiting for the chip to receive data.
 *
 * @retval #0               Success
 * @retval #Other values             Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_spi.h: This file describes the SPI APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 ext_spi_slave_send_data(EXT_IN td_u32 fd, EXT_IN td_pvoid p_send_data, EXT_IN td_u32 send_len,
    td_u32 *p_real_send_len, td_u32 usr_data, td_u32 time_out_ms);
#define ext_spi_slave_send_data(fd, p_send_data, send_len, p_real_send_len, usr_data, time_out_ms) \
    uapi_spi_send_data(fd, p_send_data, send_len, p_real_send_len, usr_data, time_out_ms,           \
        EXT_SPI_OPT_ENABLE_SPI | EXT_SPI_OPT_SET_CFG | EXT_SPI_OPT_WAIT_SIGNAL | EXT_SPI_OPT_FREE_SIGNAL)

/*
 * @ingroup  hct_spi
 * @brief RX interface for the SPI slave mode
 *
 * @par Description:
 * RX interface for the SPI slave mode
 *
 * @attention None
 * @param  fd              [IN]  Type #td_u32, device handle
 * @param  p_rcv_data      [OUT] Type #td_pvoid, RX data pointer
 * @param  rcv_len         [IN]  Type #td_u32, length of the target data to be received (unit: byte)
 * @param  p_real_rcv_len  [IN]  Type #td_u32*, length of the data that is actually received (unit: byte)
 * @param  usr_data        [IN]  Type #td_u32, parameter which is transferred to the pre-registered user preparation
 * function (spi_usr_func prepare_func) and user recovery function (spi_usr_func restore_func). The callback function is
 * configured by the input parameters prepare_func and restore_func of the uapi_spi_config interface.
 * @param  time_out_ms     [IN]  Type #td_u32, wait timeout period (unit: ms). This period refers to the continuous
 * blocking duration of a certain operation, rather than the total operation time. The wait operation refers to waiting
 * for the semaphore, waiting for the chip to send data, or waiting for the chip to receive data.
 *
 * @retval #0              Success
 * @retval #Other values            Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_spi.h: This file describes the SPI APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 ext_spi_slave_receive_data(EXT_IN td_u32 fd, EXT_IN td_pvoid p_rcv_data, EXT_IN td_u32 rcv_len,
    td_u32 *p_real_rcv_len, td_u32 usr_data, td_u32 time_out_ms);
#define ext_spi_slave_receive_data(fd, p_rcv_data, rcv_len, p_real_rcv_len, usr_data, time_out_ms) \
    uapi_spi_receive_data(fd, p_rcv_data, rcv_len, p_real_rcv_len, usr_data, time_out_ms,           \
        EXT_SPI_OPT_ENABLE_SPI | EXT_SPI_OPT_SET_CFG)

/*
 * @ingroup  hct_spi
 * @brief Half-duplex TX interface for the SPI master mode
 *
 * @par Description:
 * Half-duplex TX interface for the SPI master mode
 *
 * @attention None
 * @param  fd               [IN] Type #td_u32, device handle
 * @param  p_send_data      [IN] Type #td_pvoid, TX data pointer
 * @param  send_len         [IN] Type #td_u32, length of the target data to be sent (unit: byte)
 * @param  p_real_send_len  [OUT] Type #td_u32*, length of the data that is actually sent (unit: byte)
 * @param  usr_data         [IN]  Type #td_u32, parameter which is transferred to the pre-registered user preparation
 * function (spi_usr_func prepare_func) and user recovery function (spi_usr_func restore_func). The callback function
 * is configured by the input parameters prepare_func and restore_func of the uapi_spi_config interface.
 * @param  time_out_ms      [IN]  Type #td_u32, wait timeout period (unit: ms). This period refers to the continuous
 * blocking duration of a certain operation, rather than the total operation time.
 * The wait operation refers to waiting for the semaphore, waiting for the chip to
 * send data, or waiting for the chip to receive data.
 *
 * @retval #0               Success
 * @retval #Other values             Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_spi.h: This file describes the SPI APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 ext_spi_host_half_send_data(EXT_IN td_u32 fd, EXT_IN td_pvoid p_send_data, EXT_IN td_u32 send_len,
    td_u32 *p_real_send_len, td_u32 usr_data, td_u32 time_out_ms);
#define ext_spi_host_half_send_data(fd, p_send_data, send_len, p_real_send_len, usr_data, time_out_ms)         \
    uapi_spi_host_send_recieve_data(fd, p_send_data, TD_NULL, send_len, p_real_send_len, usr_data, time_out_ms, \
        EXT_SPI_OPT_ENABLE_SPI | EXT_SPI_OPT_DISABLE_SPI | EXT_SPI_OPT_SET_CFG | EXT_SPI_OPT_WAIT_SIGNAL |        \
        EXT_SPI_OPT_FREE_SIGNAL | EXT_SPI_OPT_RCV_FIX_DATA)

/*
 * @ingroup  hct_spi
 * @brief Half-duplex RX interface for the SPI master mode
 *
 * @par Description:
 * Half-duplex RX interface for the SPI master mode
 *
 * @attention None
 * @param  fd              [IN]  Type #td_u32, device handle
 * @param  p_rcv_data      [OUT] Type #td_pvoid, RX data pointer
 * @param  rcv_len         [IN]  Type #td_u32, length of the target data to be received (unit: byte)
 * @param  p_real_rcv_len  [IN]  Type #td_u32*, length of the data that is actually received (unit: byte)
 * @param  usr_data        [IN]  Type #td_u32, parameter which is transferred to the pre-registered user preparation
 * function (spi_usr_func prepare_func) and user recovery function (spi_usr_func restore_func). The callback function
 * is configured by the input parameters prepare_func and restore_func of the uapi_spi_config interface.
 * @param  time_out_ms      [IN]  Type #td_u32, wait timeout period (unit: ms). This period refers to the continuous
 * blocking duration of a certain operation, rather than the total operation time.
 * The wait operation refers to waiting for the semaphore, waiting for the chip to send
 * data, or waiting for the chip to receive data.
 *
 * @retval #0              Success
 * @retval #Other values            Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_spi.h: This file describes the SPI APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 ext_spi_host_half_receive_data(EXT_IN td_u32 fd, td_pvoid p_rcv_data, EXT_IN td_u32 rcv_len,
    td_u32 *p_real_rcv_len, td_u32 usr_data, td_u32 time_out_ms);
#define ext_spi_host_half_receive_data(fd, p_rcv_data, rcv_len, p_real_rcv_len, usr_data, time_out_ms)      \
    uapi_spi_host_send_recieve_data(fd, TD_NULL, p_rcv_data, rcv_len, p_real_rcv_len, usr_data, time_out_ms, \
        EXT_SPI_OPT_ENABLE_SPI | EXT_SPI_OPT_DISABLE_SPI | EXT_SPI_OPT_SET_CFG | EXT_SPI_OPT_WAIT_SIGNAL |     \
        EXT_SPI_OPT_FREE_SIGNAL | EXT_SPI_OPT_SEND_FIX_DATA)

/*
 * @ingroup  hct_spi
 * @brief Full-duplex TX/RX interface for the SPI master mode
 *
 * @par Description:
 * Full-duplex TX/RX interface for the SPI master mode
 *
 * @attention None
 * @param  fd               [IN]  Type #td_u32, device handle
 * @param  p_send_data      [IN]  Type #td_pvoid, TX data pointer
 * @param  p_rcv_data       [OUT] Type #td_pvoid, RX data pointer
 * @param  data_len         [IN]  Type #td_u32, length of the target data to be sent and received (unit: byte)
 * @param  p_real_data_len  [IN]  Type #td_u32*, length of the data that is actually sent and received (unit: byte)
 * @param  usr_data         [IN]  Type #td_u32, parameter which is transferred to the pre-registered user preparation
 * function (spi_usr_func prepare_func) and user recovery function (spi_usr_func restore_func). The callback function is
 * configured by the input parameters prepare_func and restore_func of the uapi_spi_config interface.
 * @param  time_out_ms      [IN]  Type #td_u32, wait timeout period (unit: ms). This period refers to the continuous
 * blocking duration of a certain operation, rather than the total operation time. The wait operation refers to waiting
 * for the semaphore, waiting for the chip to send data, or waiting for the chip to receive data.
 *
 * @retval #0               Success
 * @retval #Other values             Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_spi.h: This file describes the SPI APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 ext_spi_host_send_and_receive_data(EXT_IN td_u32 fd, EXT_IN td_pvoid p_send_data, td_pvoid p_rcv_data,
    EXT_IN td_u32 data_len, td_u32 *p_real_data_len, td_u32 usr_data, td_u32 time_out_ms);
#define ext_spi_host_send_and_receive_data(fd, p_send_data, p_rcv_data, data_len, p_real_data_len, usr_data,      \
    time_out_ms)                                                                                                 \
    uapi_spi_host_send_recieve_data(fd, p_send_data, p_rcv_data, data_len, p_real_data_len, usr_data, time_out_ms, \
        EXT_SPI_OPT_ENABLE_SPI | EXT_SPI_OPT_DISABLE_SPI | EXT_SPI_OPT_SET_CFG | EXT_SPI_OPT_WAIT_SIGNAL |           \
        EXT_SPI_OPT_FREE_SIGNAL)

EXT_EXTERN td_u32 uapi_spi_send_data(EXT_IN td_u32 fd, EXT_IN td_pvoid p_send_data, EXT_IN td_u32 send_len,
    td_u32 *p_real_send_len, td_u32 usr_data, td_u32 time_out_ms, td_u32 options);
EXT_EXTERN td_u32 uapi_spi_receive_data(EXT_IN td_u32 fd, EXT_IN td_pvoid p_rcv_data, EXT_IN td_u32 rcv_len,
    td_u32 *p_real_rcv_len, td_u32 usr_data, td_u32 time_out_ms, td_u32 options);
EXT_EXTERN td_u32 uapi_spi_send_recieve_data(EXT_IN td_u32 fd, EXT_IN td_pvoid p_send_data, td_pvoid p_rcv_data,
    EXT_IN td_u32 data_len, td_u32 *p_real_data_len, td_u32 usr_data, td_u32 time_out_ms, td_u32 options);
EXT_EXTERN td_u32 uapi_spi_host_send_recieve_data(EXT_IN td_u32 fd, EXT_IN td_pvoid p_send_data, td_pvoid p_rcv_data,
    EXT_IN td_u32 data_len, td_u32 *p_real_data_len, td_u32 usr_data, td_u32 time_out_ms, td_u32 options);

/*
 * @ingroup  hct_spi
 * @brief Configures the SPI parameter.
 *
 * @par Description:
 * Configures the SPI parameter.
 *
 * @attention None
 * @param  fd       [IN] Type #td_u32, device handle
 * @param  pUsrArg  [IN] Type #ext_spi_device_usr_cfg_s, SPI parameter
 *
 * @retval #0       Success
 * @retval #Other values     Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_spi.h: This file describes the SPI APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_spi_config(EXT_IN td_u32 fd, EXT_IN ext_spi_device_usr_cfg_s *pUsrArg);

/*
 * @ingroup  hct_spi
 * @brief Disables the SPI device.
 *
 * @par Description:
 * Disables the SPI device.
 *
 * @attention None
 * @param  fd       [IN] Type #td_u32, device handle
 *
 * @retval #0       Success
 * @retval #Other values     Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_spi.h: This file describes the SPI APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_spi_close(td_u32 fd);

/*
 * @ingroup  hct_spi
 * @brief Enables the SPI device to obtain the device handle.
 *
 * @par Description:
 * Enables the SPI device to obtain the device handle.
 *
 * @attention None
 * @param  spi_hw_id  [IN] Type #td_u8, SPI device ID, indicating ext_spi_ctrl_s.p_hw_devs.[spi_hw_id]
 * @param  p_fd       [OUT] Type #td_u32, device handle
 *
 * @retval #0         Success
 * @retval #Other values       Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_spi.h: This file describes the SPI APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_spi_open(td_u8 spi_hw_id, EXT_OUT td_u32 *p_fd);

/*
 * @ingroup  hct_spi
 * @brief  Sets the SPI FIFO threshold.
 *
 * @par Description:
 * Sets the SPI FIFO threshold.
 *
 * @attention None
 * @param  spi_hw_id     [IN] Type #td_u8, SPI device ID, indicating ext_spi_ctrl_s.p_hw_devs.[spi_hw_id]
 * @param  tx_fifo_line  [IN] Type #ext_spi_tx_fifo_water_line_e, SPI TX FIFO threshold
 * @param  rx_fifo_line  [IN] Type #ext_spi_rx_fifo_water_line_e, SPI RX FIFO threshold
 *
 * @retval #0            Success.
 * @retval #Other values          Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_spi.h: This file describes the SPI APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_spi_set_water_line(td_u8 spi_hw_id, ext_spi_tx_fifo_water_line_e tx_fifo_line,
    ext_spi_rx_fifo_water_line_e rx_fifo_line);

/*
 * @ingroup  hct_spi
 * @brief  Initializes the SPI module.
 *
 * @par Description:
 * Initializes the SPI module.
 *
 * @attention To initialize the SPI module, the user needs to perform the initial configuration on the SPI information
 * as follows:
 * @li Clear p_spi_ctrl to 0.
 * @li Specify the number of used hardware devices, that is, assign a value to g_test_spi_ctrl.hw_device_count.
 * @li Specify the hardware resource space to be used, that is, assign a value to g_test_spi_ctrl.p_hw_devs.
 * The parameter p_hw_devs points to the ext_spi_hw_dev_s array space.
 * @li Specify the number of software handles (maximum number of concurrent callers), that is,
 * assign a value to g_test_spi_ctrl.sw_device_count.
 * @li Specify the software handle space, that is, assign a value to g_test_spi_ctrl.p_sw_devs.
 * The parameter p_sw_devs points to the ext_spi_sw_dev_s array space.
 * @li Configure the base address of the nth hardware resource, that is, assign a value to
 * g_test_spi_hw_device[n].reg_base.
 * @li Configure the interrupt ID of the nth hardware resource, that is, assign a value to
 * g_test_spi_hw_device[n].irq_num.
 * @li Configure the buffer type of the nth hardware resource, that is, assign a value to
 * g_test_spi_hw_device[n].data_width_byte.
 * @li Configure the size of the TX buffer of the nth hardware resource, that is, assign a value to
 * g_test_spi_hw_device[n].tx_buf.total_len, which indicates the size of the TX buffer.
 * @li Configure the TX buffer space of the nth hardware resource, that is, assign a value to
 * g_test_spi_hw_device[n].tx_buf.p_buf. The TX buffer space is provided by the user. The TX buffer size is equal
 * to tx_buf.total_len*data_width_byte. When data_width_byte is EXT_SPI_SOFT_BUF_DATA_WIDTH_2_BYTES,
 * the space address needs to be 2-byte aligned.
 * @li Configure the RX buffer size of the nth hardware resource, that is, assign a value to
 * g_test_spi_hw_device[n].rx_buf.total_len, which indicates the size of the RX buffer.
 * @li Configure the RX buffer space of the nth hardware resource, that is, assign a value to
 * g_test_spi_hw_device[n].rx_buf.p_buf. The RX buffer space is provided by the user. The RX buffer size is equal to
 * rx_buf.total_len*data_width_byte. When data_width_byte is EXT_SPI_SOFT_BUF_DATA_WIDTH_2_BYTES, the space
 * address needs to be 2-byte aligned.
 * @param  p_spi_ctrl  [IN] Type #ext_spi_ctrl_s, global resource used by the SPI module. The space
 * is provided by the user and cannot be released.
 *
 * @retval #0          Success
 * @retval #Other values        Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_spi.h: This file describes the SPI APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_spi_init(ext_spi_ctrl_s *p_spi_ctrl);

#endif
