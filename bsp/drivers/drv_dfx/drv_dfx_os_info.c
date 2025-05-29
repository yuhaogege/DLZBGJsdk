/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Modify the drv directory, remove the first stage of the romization error,
 * delete the useless code to compile and adjust the dfx directory, remove the count
 * function driver part drv_dfx_os_info.c code
 * Author: CompanyName
 * Create: 2012-04-04
 */
#include "drv_dfx_os_info.h"
#include "drv_dfx_cmn_inc.h"

#include <soc_mdm_mem.h>
#include <soc_mdm_os_stat.h>
#include <soc_mdm_task.h>
#include <soc_stdlib.h>
#ifdef __MEM_HEAP_STAT_PATCH__
td_u32 g_heap_bsp_size;
td_u32 g_heap_mac_size;
td_u32 g_heap_app_size;
#endif

td_u32 mem_info_show(td_u16 id, EXT_IN td_pvoid cmd_param, td_u16 cmd_param_size, td_u8 option)
{
    ext_unref_param(cmd_param);
    ext_unref_param(cmd_param_size);

    dfx_sys_mem_info sys_mem_info = { 0 };

    uapi_mem_get_sys_info(&sys_mem_info.sys_mem_info);

    sys_mem_info.reset_handler_size = (uintptr_t)&__reset_handler_size__;
    sys_mem_info.rom_data_size = (uintptr_t)&__rom_data_size__;
    sys_mem_info.rom_bss_size = (uintptr_t)&__rom_bss_size__;
    sys_mem_info.permanent_text_size = (uintptr_t)&__permanent_size__;
    sys_mem_info.bss_size = (uintptr_t)&__bss_size__;
    sys_mem_info.data_size = (uintptr_t)&__data_size__;
    sys_mem_info.dyn_size = (uintptr_t)&__dyn_size__;
    sys_mem_info.sys_stack_size = (uintptr_t)&__SYSTEM_STACK_SIZE__;
    sys_mem_info.heap_size = (uintptr_t)&__HEAP_SIZE__;
#ifdef __MEM_HEAP_STAT_PATCH__
    sys_mem_info.bsp_heap_size = g_heap_bsp_size;
    sys_mem_info.mac_heap_size = g_heap_mac_size;
    sys_mem_info.app_heap_size = g_heap_app_size;
#endif

    uapi_diag_report_packet((td_u32)id, (td_u8)option, (td_pbyte)&sys_mem_info, sizeof(sys_mem_info), TD_TRUE);
    return EXT_ERR_SUCCESS;
}
td_u32 os_resource_show(td_u16 id, EXT_IN td_pvoid cmd_param, td_u16 cmd_param_size, td_u8 option)
{
    ext_unref_param(cmd_param);
    ext_unref_param(cmd_param_size);

    ext_os_resource_use_stat_s os_resource = { 0 };

    uapi_os_get_resource_status(&os_resource);
    uapi_diag_report_packet((td_u32)id, (td_u8)option, (td_pbyte)&os_resource, sizeof(os_resource), TD_TRUE);
    return EXT_ERR_SUCCESS;
}

#ifdef TASK_DEBUG
#if TASK_DEBUG
EXT_EXTERN td_u32 g_uwTskMaxNum;
#define OS_TASK_STATUS_UNUSED 0x0001
#define PRINT_BUFF_SIZE 200
td_u32 os_task_show(td_u16 id, EXT_IN td_pvoid cmd_param, td_u16 cmd_param_size, td_u8 option)
{
    ext_unref_param(cmd_param);
    ext_unref_param(cmd_param_size);
    ext_task_info_s *ptask_info = TD_NULL;
    td_char *printline = TD_NULL;
    td_u32 ret = EXT_ERR_FAILURE;
    td_u32 i = 0;

    ptask_info = (ext_task_info_s *)uapi_malloc(EXT_MOD_ID_SAL_DFX, sizeof(ext_task_info_s));
    if (ptask_info == TD_NULL) {
        uapi_diag_report_packet(id, (td_u8)option, (td_pbyte)g_str_ind_not_enough_memory,
            (td_u16)(strlen(g_str_ind_not_enough_memory) + 1), TD_TRUE);
        ret = EXT_ERR_MALLOC_FAILUE;
        goto MALLOC_FAIL_1;
    }

    printline = (td_char *)uapi_malloc(EXT_MOD_ID_SAL_DFX, PRINT_BUFF_SIZE);
    if (printline == TD_NULL) {
        uapi_diag_report_packet(id, (td_u8)option, (td_pbyte)g_str_ind_not_enough_memory,
            (td_u16)(strlen(g_str_ind_not_enough_memory) + 1), TD_TRUE);
        ret = EXT_ERR_MALLOC_FAILUE;
        goto MALLOC_FAIL_2;
    }

    for (i = 0; i < g_uwTskMaxNum; i++) {
        (td_void)memset_s(ptask_info, sizeof(ext_task_info_s), '0', sizeof(ext_task_info_s));
        (td_void)memset_s(printline, PRINT_BUFF_SIZE, '0', PRINT_BUFF_SIZE);

        ret = uapi_task_get_info(i, ptask_info);
        if (ret == EXT_ERR_SUCCESS) {
            int ret1 = -1;
            ret1 = sprintf_s(printline, PRINT_BUFF_SIZE, "%s, id: %d, status: %hd, pri: %hd; stack(size: 0x%x,\
                cur_size: 0x%x, peak_size: 0x%x, top: 0x%x, bottom: 0x%x)\r\n",
                ptask_info->name, ptask_info->id, ptask_info->status, ptask_info->priority, ptask_info->stack_size,
                ptask_info->curr_used, ptask_info->peak_used, ptask_info->top_of_stack, ptask_info->bottom_of_stack);
            if (ret1 >= 0) {
                uapi_diag_report_packet((td_u32)id, (td_u8)option, (td_pbyte)printline, PRINT_BUFF_SIZE, TD_TRUE);
            }
        }
    }
    ret = EXT_ERR_SUCCESS;
    uapi_free(EXT_MOD_ID_SAL_DFX, printline);
MALLOC_FAIL_2:
    uapi_free(EXT_MOD_ID_SAL_DFX, ptask_info);
MALLOC_FAIL_1:
    return ret;
}
#endif
#endif
