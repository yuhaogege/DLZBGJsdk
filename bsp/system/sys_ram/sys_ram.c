/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: sys_ram.c
 */
#include <soc_mdm.h>
#include <los_builddef.h>
#include <hal_sectors.h>

extern void *__HEAP_SIZE__;
extern void *__HEAP_BEGIN__;
extern void *__SYSTEM_STACK_END__;
extern void *__RAM_SIZE__;
extern void *__RAM_BEGIN__;

#ifdef LOSCFG_LIB_CONFIGURABLE
#if (LOSCFG_LIB_CONFIGURABLE == YES)
typedef struct {
    td_u32  uw_mem_used;
} tsk_mem_used_info;

LITE_OS_SEC_BSS_MINOR tsk_mem_used_info *g_tsk_mem_used_info;
__attribute__((section(".sysmemused")))tsk_mem_used_info g_tsk_mem_used_info_0;

LITE_OS_SEC_BSS int *errno_array;
__attribute__((section(".errno"))) int errno_array_0;
#endif
#endif

__hot void hal_sectors_ram_patch(void)
{
    hal_sectors_ram_heap_size = ((UINT32)(void*)&__HEAP_SIZE__);
    hal_sectors_ram_heap_start = ((void*)&__HEAP_BEGIN__);
    hal_sectors_stack_end = ((void*)&__SYSTEM_STACK_END__);
    hal_sectors_ram_size = ((UINT32)(void*)&__RAM_SIZE__);
    hal_sectors_ram_start = ((void*)&__RAM_BEGIN__);
}

