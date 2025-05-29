/*
 * Copyright (c) CompanyNameMagicTag 2012-2018. All rights reserved.
 * Description: Program gain amplifier (PGA)
 * Author: CompanyName
 * Create: 2012-07-16
 */

/* @defgroup hct_pga Program gain amplifier (PGA)
 * @ingroup drivers
 */

#ifndef __SOC_MDM_PGA_H__
#define __SOC_MDM_PGA_H__

#include <soc_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup hct_pga
 * PGA index
 */
typedef enum {
    EN_PGA_0 = 0x1, /**< PGA0 */
    EN_PGA_1 = 0x2  /**< PGA1 */
} ext_pga_idx_en;

/**
 * @ingroup hct_pga
 * PGA  Low pass break frequency control
 */
typedef enum {
    EN_PGA_LP_NA = 0x0,   /**< Use this value if the PGA is not configured. */
    EN_PGA_LP_150K = 0x1, /**< 150 kHz */
    EN_PGA_LP_300K = 0x2, /**< 300 kHz */
    EN_PGA_LP_600K = 0x3, /**< 600 kHz */
} ext_pga_lp_en;

/**
 * @ingroup hct_pga
 * PGA  ÔöÒæ¿ØÖÆ±¶Êý
 */
typedef enum {
    EN_PGA_GAIN_25 = 0x0, /**< 25x gain amplification */
    EN_PGA_GAIN_35 = 0x1, /**< 35x gain amplification */
    EN_PGA_GAIN_NA        /**< Use this value if the PGA is not configured. */
} ext_pga_gain_en;

/**
 * @ingroup hct_pga
 * PGA VCM voltage control
 */
typedef enum {
    EN_PGA_VCM_SEL_0_6V = 0x0, /**< 0.6 V */
    EN_PGA_VCM_SEL_0_4V = 0x1, /**< 0.4 V (default) */
    EN_PGA_VCM_SEL_0_3V = 0x2, /**< 0.3 V */
} ext_pga_vcm_sel_en;

/**
 * @ingroup hct_pga
 * PGA configuration description
 */
typedef struct {
    ext_pga_idx_en pga_idx;         /**< PGA index. If you want to configure more than one PGA at a time, you can OR
                                    * the PGA index.
                                    */
    ext_pga_lp_en pga0_lp;          /**< PGA 0 low pass break frequency control */
    ext_pga_lp_en pga1_lp;          /**< PGA 1 low pass break frequency control */
    ext_pga_gain_en pga0_gain;      /**< PGA 0 gain control multiplier */
    ext_pga_gain_en pga1_gain;      /**< PGA 1 gain control multiplier */
    ext_pga_vcm_sel_en pga_vcm_sel; /**< PGA VCM voltage control */
} ext_pga_cfg;

/**
 * @ingroup  hct_pga
 * @brief   Initializes the PGA.
 *
 * @par Description:
 * Initializes the PGA.
 *
 * @attention None
 * @param  None
 *
 * @retval #EXT_ERR_SUCCESS  Success
 * @retval #EXT_ERR_FAILURE  Failure
 * @par Dependency:
 * @li soc_mdm_pga.h: Describe PGA APIs.
 * @see  uapi_pga_deinit
 * @since DW21_V100R001C00
 */
td_u32 uapi_pga_init(void);

/**
 * @ingroup  hct_pga
 * @brief   Sets the PGA parameters and enables the PGA.
 *
 * @par Description:
 * Sets the PGA parameters and enables the PGA.
 *
 * @attention None
 * @param  pga_cfg             [IN] Type #ext_pga_cfg * Input PGA gain parameters
 *
 * @retval #EXT_ERR_SUCCESS  Success
 * @retval #EXT_ERR_FAILURE  Failure
 * @par Dependency:
 * @li soc_mdm_pga.h: Describe PGA APIs.
 * @see  uapi_pga_init | uapi_pga_deinit
 * @since DW21_V100R001C00
 */
td_u32 uapi_pga_amplify(EXT_CONST ext_pga_cfg *pga_cfg);

/**
 * @ingroup  hct_pga
 * @brief   Deinitializes the PGA.
 *
 * @par Description:
 * Deinitializes the PGA.
 *
 * @attention None
 * @param  None
 *
 * @retval #EXT_ERR_SUCCESS  Success
 * @retval # Other values  Failure
 * @par Dependency:
 * @li soc_mdm_pga.h: Describe PGA APIs.
 * @see  uapi_pga_init
 * @since DW21_V100R001C00
 */
td_u32 uapi_pga_deinit(void);

#ifdef __cplusplus
}
#endif
#endif
