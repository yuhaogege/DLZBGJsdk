/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * defgroup: hct_i2c  I2C.
 * ingroup drivers.
 */

#ifndef __SOC_MDM_I2C_H__
#define __SOC_MDM_I2C_H__

#include <soc_types_rom.h>

// I2C Interface
#define I2C_RATE_DEFAULT (100000)

typedef td_void (*i2c_reset_func)(td_void);
typedef td_void (*i2c_prepare_func)(td_void);
typedef td_void (*i2c_restore_func)(td_void);

/**
 * @ingroup hct_i2c
 *
 * I2C callback function.
 */
typedef struct {
    /*
     * < This function is called back when the communication with the slave device is abnormal.
     * This function can be used to cancel the suspension operation of the slave device.
     */
    i2c_reset_func reset_func;
    /*
     * < This function is called back before the I2C read/write operation to implement
     * the preparations before the I2C operation.
     */
    i2c_prepare_func prepare_func;
    /*
     * < After the I2C read/write operation is performed, this function
     * is called back to implement the recovery after the I2C operation.
     */
    i2c_restore_func restore_func;
} ext_i2c_func_s;

/**
 * @ingroup hct_i2c
 *
 * I2C TX/RX data descriptor
 */
typedef struct {
    td_u8 *p_send_buf;    /**< Data TX pointer. The user needs to ensure that no null pointer is transferred. */
    td_u32 send_len;      /**< Length of sent data (unit: byte) */
    td_u8 *p_receive_buf; /**< Data RX pointer. The user needs to ensure that no null pointer is transferred. */
    td_u32 receive_len;   /**< Length of received data (unit: byte) */
} ext_i2c_data_s;

/**
 * @ingroup  hct_i2c
 * @brief  I2C data TX and RX
 *
 * @par Description:
 * The I2C sends data to the slave device and then receives data from the slave device.
 *
 * @attention Multi-tasking is not supported.
 * @param  device_addr [IN] Type #td_u8 The device address of the I2C device is shifted leftward by one bit.
 * The least significant bit is set based on the RX and TX conditions.
 * @param  p_i2c_data  [IN] Type #ext_i2c_data_s* Descriptor of the data to be received.
 * The data TX pointer and data RX pointer of the structure body cannot be empty.
 *
 * @retval #0          Success
 * @retval #Other values        Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_i2c.h: Declares the API.
 * @see  uapi_i2c_send|uapi_i2c_receive
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_i2c_sendreceive(td_u8 device_addr, ext_i2c_data_s *p_i2c_data);

/**
 * @ingroup  hct_i2c
 * @brief  I2C data TX
 *
 * @par Description:
 * I2C data TX
 *
 * @attention Multi-tasking is not supported.
 * @param  device_addr [IN] Type #td_u8 The device address of the I2C device is shifted leftward by one bit.
 * The least significant bit is set based on the RX and TX conditions.
 * @param  p_i2c_data  [IN] Type #ext_i2c_data_s* Descriptor of the data to be sent
 *
 * @retval #0          Success
 * @retval #Other values        Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_i2c.h: Declares the API.
 * @see  uapi_i2c_sendreceive|uapi_i2c_receive
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_i2c_send(td_u8 device_addr, ext_i2c_data_s *p_i2c_data);

/**
 * @ingroup  hct_i2c
 * @brief  I2C data RX
 *
 * @par Description:
 * I2C data RX
 *
 * @attention Multi-tasking is not supported.
 * @param  device_addr [IN] Type #td_u8 The device address of the I2C device is shifted leftward by one bit.
 * The least significant bit is set based on the RX and TX conditions.
 * @param  p_i2c_data  [IN] Type #ext_i2c_data_s* Descriptor of the data to be received
 *
 * @retval #0          Success
 * @retval #Other values        Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_i2c.h: Declares the API.
 * @see  uapi_i2c_send|uapi_i2c_sendreceive
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_i2c_receive(td_u8 device_addr, ext_i2c_data_s *p_i2c_data);

/**
 * @ingroup  hct_i2c
 * @brief  Initializes the I2C controller.
 *
 * @par Description:
 * Initializes the I2C controller.
 *
 * @attention None
 * @param  baudrate [IN] Type #td_u32 I2C baud rate
 *
 * @retval #0      Success
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_i2c.h: Declares the API.
 * @see  uapi_i2c_deinit
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_i2c_init(td_u32 baudrate);

/**
 * @ingroup  hct_i2c
 * @brief  Exits the I2C module.
 *
 * @par Description:
 * Exits the I2C module.
 *
 * @attention This API is called after uapi_i2c_init is called.
 * @param  None
 *
 * @retval #0      Success
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_i2c.h: Declares the API.
 * @see  uapi_i2c_init
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_i2c_deinit(td_void);

/**
 * @ingroup  hct_i2c
 * @brief  Registers the I2C callback function.
 *
 * @par Description:
 * Registers the I2C callback function, for extension.
 *
 * @attention The input parameter is ext_i2c_func_s instead of ext_i2c_func_s*.
 * @param  pfn     [IN] Type #ext_i2c_func_s Callback function
 *
 * @retval #0      Success
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_i2c.h: Declares the API.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_void uapi_i2c_register_reset_bus_func(ext_i2c_func_s pfn);

#endif
