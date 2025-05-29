/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: This file is the system measurable package interface adapter file,
 *              including dead machine storage / task status view / timer status view
 *              / message queue / Memory information view
 */

#ifndef __SAL_OS_DFX_H__
#define __SAL_OS_DFX_H__
#include <soc_types.h>
#include <soc_mdm_crash.h>

EXT_START_HEADER

td_u32 uapi_get_track_info(syserr_track_info *info);
td_u32 uapi_get_cur_task_id(td_void);
td_u32 uapi_get_new_task_id(td_void);
td_void uapi_get_new_sp_info(td_u32 *sp, td_u32 *stack_top, td_u32 *stack_bottom);

EXT_END_HEADER
#endif /* __SAL_OS_DFX_H__ */