/*
 * Copyright (c) CompanyNameMagicTag 2018-2018. All rights reserved.
 * Description: SAL reset handle.
 * Author: CompanyName
 * Create: 2018-12-15
 */

#ifndef __SAL_RESET_H__
#define __SAL_RESET_H__

#include <soc_mdm_crash.h>

EXT_CPP_START

td_void uapi_sys_reset_init(td_void);

td_u32 sal_reset_monitor(td_u32 cur_sec);

extern td_void uapi_syserr_save_cmn_info_step1(ext_syserr_id eid, td_u16 rid);
extern td_void uapi_syserr_save_cmn_info_step2(ext_syserr_id eid, td_u16 rid);

EXT_CPP_END
#endif /* __SAL_RESET_H__ */
