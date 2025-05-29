/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Upgrade base funciton - nv mode.
 */

#ifndef __UPG_BASE_PRV_H__
#define __UPG_BASE_PRV_H__

#include "trans_file_host.h"
#include "trans_file_slave.h"

#include <dfx_upg_base.h>

#ifdef __cplusplus
extern "C" {
#endif

extern ext_upg_kernel_ctrl g_upg_kernel_ctrl;
#define ext_upg_kerclt_ctrl() ((ext_upg_kernel_ctrl *)(&g_upg_kernel_ctrl))
#define ext_upg_kernel_trans_ctrl() ((ext_trans_file_ctrl *)(&g_upg_kernel_ctrl.trans_ctrl))

#ifdef __cplusplus
}
#endif
#endif /* __UPG_BASE_PRV_H__ */
