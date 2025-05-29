/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Dynamic loading.
 * @li During initialization, the user must select one from uapi_remap_32k_init, uapi_remap_48k_init, and
 * uapi_remap_64k_init based on the actual requirements. The choice should be made through static compilation
 * rather than dynamic hardware or other configurations. Otherwise, all these three sets of resources will be
 * linked to the program, causing resource waste. \n
 * Author: CompanyName
 * Create: 2018-10-31
 */

/**
 * @defgroup hct_remap Dynamic loading
 * @ingroup osa
 */

#ifndef __SOC_MDM_REMAP_H__
#define __SOC_MDM_REMAP_H__

#include <soc_types.h>
#include <soc_mdm_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define EXT_REMAP_SHFT_BIT 12
#define EXT_REMAP_BLOCK_SIZE 0x1000

typedef struct {
    td_u32 phy_entry;    /**< Internal use. */
    td_u32 virt;         /**< Internal use. */
    td_u64 swapin_count; /**< Internal use. */
    td_u64 read_cnt;     /**< Internal use. */
} ext_remap_vir_entry;

/**
 * @ingroup  hct_remap
 * @brief  Initializes the remap module based on the actual 32 KB memory.
 *
 * @par Description:
 * Initializes the remap module based on the actual 32 KB memory.
 *
 * @attention None
 *
 * @retval #EXT_ERR_SUCCESS  Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_remap.h: This file declares the API.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_remap_32k_init(td_void);

/**
 * @ingroup  hct_remap
 * @brief  Initializes the remap module based on the actual 48 KB memory.
 *
 * @par Description:
 * Initializes the remap module based on the actual 48 KB memory.
 *
 * @attention None
 *
 * @retval #EXT_ERR_SUCCESS  Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_remap.h: This file declares the API.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_remap_48k_init(td_void);

/**
 * @ingroup  hct_remap
 * @brief  Initializes the remap module based on the actual 64 KB memory.
 *
 * @par Description:
 * Initializes the remap module based on the actual 64 KB memory.
 *
 * @attention None
 *
 * @retval #EXT_ERR_SUCCESS  Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_remap.h: This file declares the API.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_remap_64k_init(td_void);

typedef td_void (*remap_debug_func)(td_u32 vir_addr, td_u32 phy_addr, td_u32 access_cnt);

/**
 * @ingroup  hct_remap
 * @brief  Registers the remap statistics feature.
 *
 * @par Description:
 * Registers the remap statistics feature.
 *
 * @attention None
 *
 * @param  pfn [IN] Type #remap_debug_func, Remap debug callback function.
 *
 * @retval #EXT_ERR_SUCCESS  Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_remap.h: This file declares the API.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_remap_register_debug_hook(remap_debug_func pfn);

td_u32 usr_remap_init(td_void);
td_u32 usr_remap_diag_init(td_void);

#ifdef __cplusplus
}
#endif
#endif /* __SOC_MDM_REMAP_H__ */
