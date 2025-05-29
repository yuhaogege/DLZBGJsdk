/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Module initialization.
 * Author: CompanyName
 * Create: 2018-10-31
 */

/**
 * @defgroup hct_init Module initialization
 * @ingroup system
 */

#ifndef __SOC_MDM_INIT_H__
#define __SOC_MDM_INIT_H__

#include <soc_types.h>
#include <soc_mdm_types.h>
#include <soc_ft_nv.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup  hct_init
 * @brief  Obtains the product MAC address of the communication module.
 *
 * @par Description:
 * Obtain the product MAC address (corresponding to the MAC address configured in the NV item of the factory
 * partition) of the communication module.
 *
 * @attention The configuration API is uapi_set_plc_mac().
 * @param  aucMac[EXT_PLC_MAC_ADDR_LEN] [OUT] Type #td_u8 MAC address of the module.
 *
 * @retval ret [OUT] Type #td_u32.
 * @li EXT_ERR_SUCCESS: The configuration is successful.
 * @li For details about the return values, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_init.h: Describes the init APIs.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_get_local_mac_addr(td_u8 mac[EXT_PLC_MAC_ADDR_LEN], td_u32 mac_size);

/**
 * @ingroup  hct_init
 * @brief  Obtains the product form of the communication module.
 * @par Description:
 * Obtains the product form of the communication module.
 * @attention None
 * @param None
 * @retval ret [OUT] Type #ext_ftm_product_type For details, see soc_ft_nv.h.
 * @par Dependency:
 * @li soc_mdm_init.h: Describes the init APIs.
 * @see None
 * @since DW21_V100R001C00
 */
ext_ftm_product_type uapi_get_product_type(td_void);

#define uapi_is_product_cco(type) ((type >= EXT_FTM_PRODUCT_TYPE_C01) && (type <= EXT_FTM_PRODUCT_TYPE_C60))
#define uapi_is_product_sta(type) ((type >= EXT_FTM_PRODUCT_TYPE_M101) && (type <= EXT_FTM_PRODUCT_TYPE_M8191))
#define uapi_is_product_ndm(type) ((type >= EXT_FTM_PRODUCT_TYPE_D61) && (type <= EXT_FTM_PRODUCT_TYPE_D100))

/**
 * @ingroup  hct_init
 * @brief  Initializes the board.
 *
 * @par Description:
 * Initializes the board.
 *
 * @attention None
 * @param  None
 * @retval ret [OUT] Type #td_u32.
 * @li EXT_ERR_SUCCESS: The configuration is successful.
 * @li For details about the return values, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_init.h: Describes the init APIs.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_board_init(td_void);

/**
 * @ingroup  hct_init
 * @brief  Enables or disables the recording of reset times.
 *
 * @par Description:
 * @li Enables or disables the recording of reset times. When the function of recording the reset times is enabled,
 * the NV is recorded at 30s after the board is powered on, and the number of reset times is recorded normally.
 * @li When the number of reset times is updated, the flash memory needs to be written. Generally, when the system
 * frequently powers on and off and power-on NV write greatly hurts the flash service life. In this case, set
 * enable to TD_FALSE and check whether the service function is affected.
 *
 * @attention
 * @li This function is enabled by default.
 * @li It should be called in the initialization function of the app layer.
 * @li You are advised to disable this function only when the system is frequently powered on and off.
 * @retval None
 * @par Dependency:
 * @li soc_mdm_init.h: Describes the init APIs.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_enable_reset_times_save(td_bool enable);

#ifdef __cplusplus
}
#endif
#endif /* __SOC_MDM_INIT_H__ */
