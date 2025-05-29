/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: implementation of error handling of system errors.
 * Author: CompanyName
 * Create: 2012-07-14
 */

#ifndef __SOC_SYSTEM_ERROR_H__
#define __SOC_SYSTEM_ERROR_H__
#include <soc_types.h>
#include <soc_mdm_crash.h>
EXT_START_HEADER

#define EXT_EXC_IN_ISR_STR "f:isr"
#define EXT_EXC_IN_TASK_STR "f:task"
#define EXT_EXC_SYS_REBOOT_STR "reboot"
#define EXT_EXC_WATCHDOG_STR "wdg"
#define EXT_EXC_RESERVE_STR "to def"
/* System exception processing entry (never return) */
/* The configuration of the serial port is consistent with the BOOT, and the crash information is written to Flash. */
extern td_u32 __SYSTEM_STACK_END__;
extern td_u32 __SYSTEM_STACK_BEGIN__;
extern unsigned int g_cur_interrupt_idx;
extern td_void osExcWatchDog(td_u32);
extern td_void uapi_syserr_save_cmn_info_step1(ext_syserr_id eid, td_u16 rid);
extern td_void uapi_syserr_save_cmn_info_step2(ext_syserr_id eid, td_u16 rid);
extern td_void uapi_syserr_save_fault_info_step1(ext_syserr_id eid, td_u16 rid);
extern td_void uapi_syserr_save_fault_info_step2(td_void);

EXT_END_HEADER
#endif
