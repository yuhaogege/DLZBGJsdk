/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: FLASH driver APIs
 * Author: CompanyName
 * Create: 2018-10-31
 */
/**
 * @defgroup hct_flash  FLASH driver APIs.
 * @ingroup drivers
 */

#ifndef SOC_MDM_FLASH_H
#define SOC_MDM_FLASH_H

#include <soc_types.h>
#include <soc_mdm_flash_rom.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup hct_flash
 * Default timeout value for enabling flash protection.
 * The timeout value varies according to the protection type (internally adjustable).
 */
#define PROTECT_TIMEOUT_AUTO                                                              \
    (0xFFFFFFFF) /**< The timeout value corresponding to a specific type is encapsulated \
                  * by the internal code and can be adjusted by the user.                 \
                  */

/**
 * @ingroup hct_flash
 * Flash protection type enumeration
 */
typedef enum {
    EXT_FLASH_PROTECT_TYPE_1 = 0, /**< This type supports the 0x50 command, that is, both the volatile and
                                  * non-volatile operating status registers are supported. */
    EXT_FLASH_PROTECT_TYPE_2,     /**< This type does not support the 0x50 command, that is, the volatile operating
                                  * status register is not supported. */
    EXT_FLASH_PROTECT_NONE,       /**< This type does not support protection. */
    EXT_FLASH_PROTECT_TYPE_MAX,
} ext_flash_protect_type;

/**
 * @ingroup  hct_flash
 * @brief  Initializes the flash device.
 *
 * @par Description:
 * Initializes the flash device.
 *
 * @attention Initialize the flash module during system boot.
 * @param  None
 *
 * @retval #0               Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_flash.h£ºFLASH driver APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
td_u32 uapi_flash_init(td_void);

/**
 * @ingroup  hct_flash
 * @brief  Reads the flash data to the specified cache.
 *
 * @par Description:
 * Reads the flash data to the specified cache.
 *
 * @attention This API cannot be called in an interrupt handling function.
 * @param  addr             [IN] Type #td_u32 Offset of the flash address.
 * @param  size             [IN] Type #td_u32 Read length (unit: byte).
 * @param  data             [OUT] Type #td_u8* Destination cache address.
 *
 * @retval #0               Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_flash.h£ºFLASH driver APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
td_u32 uapi_flash_read(EXT_CONST td_u32 addr, EXT_CONST td_u32 size, td_u8 *data);

/**
 * @ingroup  hct_flash
 * @brief  Writes data to the specified flash partition.
 *
 * @par Description:
 * Writes data to the specified flash partition.
 *
 * @attention
 * @li Restriction protection for the relative address of the flash memory.
 * @li The number of flash erase times must comply with the device data sheet.
 * @li Determine whether to erase the flash before the write based on the actual control scenario.
 * @li This API cannot be called in an interrupt handling function.
 * @li do_erase can be set to TD_TRUE only when the read back check space is configured during flash initialization.
 * @param  addr     [IN] Type #td_u32 Offset address for writing data to the flash memory.
 * @param  size     [IN] Type #td_u32 Length of the data to be written (unit: byte).
 * @param  data     [IN] Type #td_u8* Cache address of the data to be written.
 * @param  do_erase [IN] Type #td_bool  TD_FALSE: Write data to the flash memory directly. TD_TRUE: Erase the sector
 * space before write. The user data is written to the user operation space and the historical
 * data is written back to other spaces.
 *
 * @retval #0               Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_flash.h£ºFLASH driver APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
td_u32 uapi_flash_write(EXT_CONST td_u32 addr, td_u32 size, EXT_CONST td_u8 *data, td_bool do_erase);

/**
 * @ingroup  hct_flash
 * @brief  Erases the data in the specified flash partition.
 *
 * @par Description:
 * Erases the data in the specified flash partition.
 *
 * @attention
 * @li When erasing the flash memory, pay attention to the restriction on the relative address of the flash memory.
 * @li The number of flash erase times must comply with the device data sheet.
 * @li This API cannot be called in an interrupt handling function.
 * @param  addr [IN] Type #td_u32 Address offset of the flash memory to be erased.
 * @param  size [IN] Type #td_u32 Length of the data to be erased (unit: byte). The value must be a multiple of 4 KB.
 *
 * @retval #0               Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_flash.h£ºFLASH driver APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
td_u32 uapi_flash_erase(EXT_CONST td_u32 addr, EXT_CONST td_u32 size);

/**
 * @ingroup  hct_flash
 * @brief  Sets or reads flash information.
 *
 * @par Description:
 * Sets or reads flash information. The supported functions are as follows:
 * @li The command ID is EXT_FLASH_CMD_GET_INFO, and the data structure is ext_flash_info_s, which is used to obtain
 * the basic information about the flash memory.
 *
 * @attention When the command ID is EXT_FLASH_CMD_GET_INFO, the spi_flash_basic_info_probe function needs to be called
 * to register the current flash model when the flash is initialized.
 * @param  cmd             [IN]     Type #td_u16 Command ID
 * @param  data            [IN/OUT] Type #td_void* Information set or obtained.
 *
 * @retval #0               Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_flash.h£ºFLASH driver APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
td_u32 uapi_flash_ioctl(EXT_IN td_u16 cmd, EXT_INOUT td_void *data);

/* Internal APIs */
td_u32 uapi_flash_erase_in(EXT_CONST td_u32 addr, EXT_CONST td_u32 size);
td_u32 uapi_flash_write_in(EXT_CONST td_u32 addr, td_u32 size, EXT_CONST td_u8 *data, td_bool do_erase);
td_u32 uapi_flash_read_in(EXT_CONST td_u32 addr, EXT_CONST td_u32 size, td_u8 *data);
td_u32 uapi_flash_ioctl_in(EXT_IN td_u16 cmd, EXT_INOUT td_void *data);

#ifdef __cplusplus
}
#endif
#endif /* SOC_MDM_FLASH_H */
