/*
 * Copyright (c) CompanyNameMagicTag 2019-2019. All rights reserved.
 * Description: This file is used when linking,
 * and is used to assign values to global variables used by the board code
 * Author: CompanyName
 * Create: 2019-10-31
 */

#include <soc_types.h>
#include <soc_mdm_types.h>
#include <soc_product_config.h>
#include <hb_magic.h>
#include <soc_hb_ver.h>

EXT_CONST ext_product_info g_product_info = {
    PRODUCT_CFG_FULL_VERSION_STR,
    PRODUCT_CFG_BUILD_DATE,
    PRODUCT_CFG_BUILD_TIME
};

EXT_CONST ext_ue_product_ver g_product_version = {
    PRODUCT_CFG_VERSION_V,   /* V part */
    PRODUCT_CFG_VERSION_R,   /* R part */
    PRODUCT_CFG_VERSION_C,   /* C part */
    PRODUCT_CFG_VERSION_B,   /* B part */
    PRODUCT_CFG_VERSION_SPC, /* SPC part */
    { 0 }
};

td_char *g_creation_date = PRODUCT_CFG_BUILD_DATE;

EXT_CONST td_u32 g_ver_magic = PRODUCT_CFG_BUILD_MAGIC;

td_u32 uapi_ger_ver_magic(void)
{
    return (td_u32)g_ver_magic;
}

