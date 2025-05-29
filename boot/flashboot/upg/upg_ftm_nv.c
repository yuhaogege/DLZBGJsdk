/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: upg from nv
 * Author: CompanyName
 * Create: 2018-10-31
 */

#include "upg_ftm_nv.h"

/* whether upgrade dependence on nv determinated by kernel. use this message for patch when meet the problem following.
 * problem decription: module reset by half of upgrading when upgrade file conform to certain performance.
 * recurrent circulation of upgrading.
 * ( performance:  like hupg head version is drfferent from inner image version, this exception cause RSA reset)
 * OR not manipulate factory NV.  manipulate other space according to factory nv option.
 */
td_bool boot_upg_is_do_upg_from_nv(td_void)
{
    td_u32 ret;
    ext_nv_ftm_upg_extension upg_nv = { 0 };

    ret = uapi_factory_nv_read(EXT_NV_FTM_UPG_EXTENSION_ID, &upg_nv, sizeof(upg_nv));
    if (ret != EXT_ERR_SUCCESS) {
        return TD_TRUE;
    }

    if (upg_nv.magic != EXT_UPG_NV_MAGIC) {
        return TD_TRUE;
    }

    if (upg_nv.cur_times > upg_nv.dst_times) {
        /* upgrading dependence on nv, and try to upgrade failed for several times */
        return TD_FALSE;
    }

    upg_nv.cur_times++;

    ret = uapi_factory_nv_write(EXT_NV_FTM_UPG_EXTENSION_ID, &upg_nv, sizeof(upg_nv));
    if (ret != EXT_ERR_SUCCESS) {
        return TD_TRUE;
    }

    return TD_TRUE;
}
