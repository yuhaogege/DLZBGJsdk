/**
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 */
#ifndef __SOC_MDM_FLASH_ROM_H__
#define __SOC_MDM_FLASH_ROM_H__

#include <soc_types_rom.h>

#define EXT_FLASH_CMD_GET_INFO 1 /**< IOCTL command ID for obtaining the flash information */

typedef td_void (*flash_prepare_func)(td_void);
typedef td_void (*flash_restore_func)(td_void);
typedef struct {
    flash_prepare_func prepare_func;
    flash_restore_func restore_func;
} EXT_FLASH_REGISTER_S;
td_void uapi_flash_register(EXT_FLASH_REGISTER_S sfunc);

/**
 * @ingroup  hct_flash
 *
 * Flash information obtaining structure, used to describe the return structure of the command ID EXT_FLASH_CMD_GET_INFO
 */
typedef struct {
    td_char *name;      /**< Flash name, less than 10 bytes long */
    td_u32 total_size;  /**< Size of the flash memory (unit: byte) */
    td_u32 sector_size; /**< Flash block size (unit: byte) */
} ext_flash_info_s;

#endif
