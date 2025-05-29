/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Flash protection APIs.
 * @li The flash chip register is configured so that the flash chip does not respond to the SPI erase and write
 * operations.
 * @li Both non-volatile and volatile flash protection registers are supported. When the status of the volatile
 * register conflicts with that of the non-volatile register, the status of the volatile register prevails.
 * @li Generally, the non-volatile register is set to the full-chip protection mode. When the system is rebooted,
 * the flash memory is in the full-chip protection state by default.
 * @li When a partition in the flash memory needs to be erased, the flash memory is de-protected by using the
 * volatile register (implemented inside the driver). After the operation is complete, the volatile register
 * restores to the protection state.
 * @li During version upgrade, to prevent the target version from not supporting flash de-protection, full-chip
 * de-protection is performed (non-volatile and volatile protection registers). Full-chip de-protection needs
 * to be called by the upgrade module proactively. \n
 * Author: CompanyName
 * Create: 2018-10-31
 */

/**
 * @defgroup hct_flash_protect  FLASH protection
 * @ingroup drivers
 */

#ifndef __SOC_MDM_FLASH_PROTECT_H__
#define __SOC_MDM_FLASH_PROTECT_H__

#include <soc_types.h>
#include <soc_mdm_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup  hct_flash_protect
 * @brief  Initializes the flash protection.
 *
 * @par Description:
 * Initializes the flash protection.
 *
 * @attention This API can be called in the initialization phase only once.
 * @param  type [IN] Type #ext_flash_protect_type Protection type.
 *
 * @retval #EXT_ERR_SUCCESS  Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_flash_protect.h: Declares the API.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_flash_protect_init(ext_flash_protect_type type);

/**
 * @ingroup  hct_flash_protect
 * @brief  Enables flash protection.
 *
 * @par Description:
 * Enables flash protection.
 *
 * @attention This API cannot be called in an interrupt handling function.
 * @param  enable [IN] Type #td_bool TD_TRUE: flash protection enabled; TD_FALSE: flash protection disabled.
 *
 * @retval #EXT_ERR_SUCCESS  Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_flash_protect.h: Declares the API.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_flash_protect_enable(td_bool enable);

/**
 * @ingroup  hct_flash_protect
 * @brief  Sets the flash protection partition (the configuration is volatile after power-down).
 *
 * @par Description:
 * Sets the flash protection partition based on the current operation address.
 * The configuration is volatile after power-down
 * but the operation efficiency is high and there is no limit on the number of operations.
 *
 * @attention This API cannot be called in an interrupt handling function.
 * @param  ep_addr [IN] Type #td_u32 Address for erasing or writing to the flash memory.
 * @li If the value of ep_addr exceeds the actual chip size, the entire chip is protected.
 * @li If the value of ep_addr is not an integer multiple of 4 KB, 4 KB alignment is performed
 * downwards.
 * @param  timeout [IN] Type #td_u32 Validity period of the configuration.
 * @li If this parameter is set to 0, the setting takes effect immediately.
 * @li If this parameter is set to #PROTECT_TIMEOUT_AUTO, the default timeout value of the
 * protection type is used.
 * @li If this parameter is set to other values, the value is used as the timeout period.
 * (Unit: ms).
 * @li The callback function is executed after timeout (implemented internally and can be
 * modified by the user).
 *
 * @retval #EXT_ERR_SUCCESS  Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_flash_protect.h: Declares the API.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_flash_volatile_protect_config(td_u32 ep_addr, td_u32 timeout);

/**
 * @ingroup  hct_flash_protect
 * @brief  Sets the flash protection partition (the configuration is non-volatile after power-down).
 *
 * @par Description:
 * Sets the flash protection partition based on the current operation address. The configuration is
 * non-volatile after power-down but the operation efficiency is low and the operation times are limited.
 *
 * @attention This API cannot be called in an interrupt handling function.
 * @param  ep_addr [IN] Type #td_u32 Address for erasing or writing to the flash memory.
 * @li If the value of ep_addr exceeds the actual chip size, the entire chip is protected.
 * @li If the value of ep_addr is not an integer multiple of 4 KB, 4 KB alignment is performed
 * downwards.
 * @param  timeout [IN] Type #td_u32 Validity period of the configuration.
 * @li If this parameter is set to 0, the setting takes effect immediately.
 * @li If this parameter is set to #PROTECT_TIMEOUT_AUTO, the default timeout value of the
 * protection type is used.
 * @li If this parameter is set to other values, the value is used as the timeout period.
 * (Unit: ms).
 * @li The callback function is executed after timeout (implemented internally and can be
 * modified by the user).
 * @retval #EXT_ERR_SUCCESS  Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_flash_protect.h: Declares the API.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_flash_protect_config(td_u32 ep_addr, td_u32 timeout);

#ifdef __cplusplus
}
#endif
#endif /* __SOC_MDM_FLASH_PROTECT_H__ */
