/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Implements system measurable package interface.
 * Author: CompanyName
 * Create: 2012-04-20
 */

#include <sal_os_dfx.h>
#include <los_track.h>
#include <los_task.h>
#include <los_task.ph>

#include <soc_stdlib.h>

__isr td_u32 uapi_get_track_info(syserr_track_info *info)
{
    td_u32 ret;
    TRACK_MGR_S track_msg;

    if (info == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    ret = LOS_TrackInfoGet(&track_msg);
    if (ret != LOS_OK) {
        return EXT_ERR_SYSTEM_CALL_ERROR;
    }

    info->current_item = track_msg.usCurIndex;
    info->item_cnt = track_msg.usMaxRecordCount;
    for (td_u32 i = 0; i < EXT_EXC_TRACE_DEPTH; i++) {
        ret = (td_u32)memcpy_s(&(info->item[i]), sizeof(syserr_track_item), (const void *)(track_msg.pstTrackItems + i),
            sizeof(TRACK_ITEM_S));
        if (ret != EXT_ERR_SUCCESS) {
            return EXT_ERR_FAILURE;
        }
    }
    return EXT_ERR_SUCCESS;
}

__isr td_u32 uapi_get_cur_task_id(td_void)
{
    return LOS_CurTaskIDGet();
}

__isr td_u32 uapi_get_new_task_id(td_void)
{
    return LOS_NewTaskIDGet();
}

__isr td_void uapi_get_new_sp_info(td_u32 *sp, td_u32 *stack_top, td_u32 *stack_bottom)
{
    if ((sp == TD_NULL) ||
        (stack_top == TD_NULL) ||
        (stack_bottom == TD_NULL) ||
        (g_stLosTask.pstNewTask == TD_NULL)) {
        return;
    }
    *sp = (td_u32)(uintptr_t)(g_stLosTask.pstNewTask->pStackPointer);
    *stack_top = g_stLosTask.pstNewTask->uwTopOfStack;
    *stack_bottom = g_stLosTask.pstNewTask->uwTopOfStack + g_stLosTask.pstNewTask->uwStackSize;
}
