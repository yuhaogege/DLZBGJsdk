/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Version management.
 * Author: CompanyName
 * Create: 2018-10-31
 */
/**
 * @defgroup hct_ver  Version management
 * @ingroup system
 */

#ifndef __SOC_MDM_VER_H__
#define __SOC_MDM_VER_H__

#include <soc_types.h>
#include <soc_mdm_types.h>
#include <soc_ft_nv.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup  hct_ver
 * Chip version information.
 */
typedef enum {
    EXT_CHIP_VERSION_UNKNOW = 0, /**< The version is not supported currently. */
    EXT_CHIP_VERSION_V100,       /**< DW21 V100 version. */
} ext_hw_chip_ver;

/**
 * @ingroup  hct_ver
 * User version information
 */
typedef struct {
    td_u16 sw_ver;        /**< Software version number in BCD format. */
    td_u8 boot_ver;       /**< Bootloader version. */
    td_u8 year;           /**< Version date (year), in BIN format. */
    td_u8 month;          /**< Version date (month), in BIN format. */
    td_u8 day;            /**< Version date (day), in BIN format. */
    td_char chip_code[2]; /**< Chip code, in ASCII format.2 count  */
    td_char manu_code[2]; /**< Manufacturer code, in ASCII format.2count  */
    td_u8 pad[2];         /* pad 2 */
} ext_mdm_user_info;

/**
 * @ingroup  hct_ver
 * Chip type.
 */
typedef enum {
    EXT_FTM_CHIP_TYPE_UNKOWN = 0, /**< Incorrect version. */
    EXT_FTM_CHIP_TYPE_21EV,     /**< E version. */
    EXT_FTM_CHIP_TYPE_21SV,     /**< E version. */
} ext_ftm_chip_type;

/* Active reset condition. */
#define EXT_RST_STA_SECURE_TIME (75 * 24 * 60 * 60) /**< Default value: 75 days. */

/* Delay definition Unit: ms. */
#define EXT_RST_DELAY_USR_TIME_MAX (5 * 60 * 1000) /**< 5min */
#define EXT_RST_DELAY_WRT_NET_SERIAL 10000         /**< 10s */
#define EXT_RST_DELAY_WRT_SNID 10000               /**< 10s */
#define EXT_RST_DELAY_WRT_RNG 10000                /**< 10s */

/**
 * @ingroup  hct_ver
 * @brief  Obtains the hardware chip version number.
 *
 * @par Description:
 * (1) Obtains the hardware chip version number.
 *
 * @attention None
 *
 * @retval #ext_hw_chip_ver Enumeration of the hardware chip version number.
 * @par Dependency:
 * @li soc_mdm_ver.h:  This file describes version APIs.
 * @see None
 * @since DW21_V100R001C00
 */
ext_hw_chip_ver uapi_get_hw_chip_ver(td_void);

/**
 * @ingroup  hct_ver
 * @brief  Obtains the hardware chip type.
 *
 * @par Description:
 * (1) Obtains the hardware chip type.
 *
 * @attention None
 *
 * @retval #ext_ftm_chip_type  Enumeration of the hardware chip type.
 * @par Dependency:
 * @li soc_mdm_ver.h:  This file describes version APIs.
 * @see None
 * @since DW21_V100R001C00
 */
ext_ftm_chip_type uapi_get_hw_chip_type(td_void);

/**
 * @ingroup  hct_ver
 * @brief  Obtains the product version information.
 *
 * @par Description:
 * Obtains the product version information, including the chip name and version number in string format.
 * @attention None
 *
 * @retval #ext_product_info* Product version information, including the chip name and version number, in string format.
 *
 * @par Dependency:
 * @li soc_mdm_ver.h:  This file describes version APIs.
 * @see None
 * @since DW21_V100R001C00
 */
EXT_CONST ext_product_info *uapi_get_product_info(td_void);

/**
 * @ingroup  hct_ver
 * @brief  Obtains the boot version number.
 *
 * @par Description:
 * Obtains the boot version number.
 * @attention None
 * @retval #td_u8 Boot version number.
 *
 * @par Dependency:
 * @li soc_mdm_ver.h:  This file describes version APIs.
 * @see None
 * @since DW21_V100R001C00
 */
td_u8 uapi_get_boot_ver(td_void);

/**
 * @ingroup  hct_ver
 * @brief  Obtains the user version information.
 *
 * @par Description:
 * Obtains the user version information.
 * @attention None
 *
 * @param user_info [OUT] Type #ext_mdm_user_info* User version information. The default value is generated based on the
 * version information configured in Socbuilder.
 * For example,Vendor Code is AB,manu_code[0]==B manu_code[1]==A.Chip Code is CD,chip_code[0]==D chip_code[1]==C.
 * @retval #EXT_ERR_SUCCESS  Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_ver.h:  This file describes version APIs.
 * @see  uapi_set_user_info
 * @since DW21_V100R001C00
 */
td_u32 uapi_get_user_info(ext_mdm_user_info *user_info);

/**
 * @ingroup  hct_ver
 * @brief  Sets the user version information.
 *
 * @par Description:
 * Sets the user version information.
 * @attention None
 *
 * @param user_info [IN] Type #ext_mdm_user_info* User version information.
 * For example,Vendor Code is AB,manu_code[0]==B manu_code[1]==A.Chip Code is CD,chip_code[0]==D chip_code[1]==C.
 *
 * @retval #EXT_ERR_SUCCESS  Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 *
 * @par Dependency:
 * @li soc_mdm_ver.h:  This file describes version APIs.
 * @see  uapi_set_user_info.
 * @since DW21_V100R001C00
 */
td_u32 uapi_set_user_info(EXT_CONST ext_mdm_user_info *user_info);

/**
 * @ingroup  hct_ver
 * @brief  Obtains SDK version information.
 *
 * @par Description:
 * Obtains SDK version information.
 * @attention None
 * @retval #td_char* SDK version information string.
 *
 * @par Dependency:
 * @li soc_mdm_ver.h:  This file describes version APIs.
 * @see None
 * @since DW21_V100R001C00
 */
EXT_CONST td_char *uapi_get_sdk_version(td_void);

#ifdef __cplusplus
}
#endif
#endif /* __SOC_MDM_VER_H__ */
