/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Private interface referenced in the modem
 */

#ifndef _MDM_INF_H__
#define _MDM_INF_H__

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************/
#if defined(PRODUCT_CFG_SUPPORT_UPG)
/* Redefine the interface for obtaining the version. */
typedef enum tag_drv_obj {
    EXT_DRV_OBJ_NULL,

    EXT_DRV_OBJ_CODE,
    EXT_DRV_OBJ_NV, /* The function is discarded but cannot be deleted to avoid compatibility issues. */
    EXT_DRV_OBJ_BOOT,

    /* Extended Object Content */
    /* Flash memory area for STA upgrade stored on the CCO */
    EXT_DRV_OBJ_UPGFILE_BACKUP,

    /* Device upgrade file */
    EXT_DRV_OBJ_DEVICE,

    /* Upgrade files of other vendors */
    EXT_DRV_OBJ_OTHER,

    EXT_DRV_OBJ_UNKNOWN
} ext_drv_obj;

#endif

#ifdef __cplusplus
}
#endif

#endif /* _MDM_INF_H__ */

