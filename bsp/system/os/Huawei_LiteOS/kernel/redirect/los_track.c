/*----------------------------------------------------------------------------
 * Copyright (c) <2013-2015>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/
#include <securec.h>
#include "los_track.h"
#include "los_memory.ph"
#include "los_sys.ph"
#include "los_tick.ph"
#include "string.h"
#include "los_hwi.h"
#include "los_task.ph"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#if (LOSCFG_BASE_MISC_TRACK_RAM == YES)

#define LOS_TRACK_US_PER_TICK         (OS_SYS_US_PER_SECOND/LOSCFG_BASE_CORE_TICK_PER_SECOND)
#define LOS_TRACK_MGR_SIZE sizeof(TRACK_MGR_S)
UINT16   g_track_mask = 0;
static TRACK_MGR_S     *g_pst_track_mgr = NULL;
USER_CUR_US_FUNC g_curr_user_get_hook = NULL;

/*****************************************************************************
Function   : osTrackhook
Description: Defalut track hook function
Input      :  usTrackType :The  type of track item.
            uwTrackId   :The   id  of track item.
               uwUserData: The  user data of track item.
               uwEntry:      The  entry of function .
Output     : None
Return     : None
*****************************************************************************/
LITE_OS_SEC_TEXT_INIT_REDIRECT VOID osTrackhook(UINT16 usTrackType, UINT16 uwTrackId, UINT32 uwUserData, UINT32 uwEntry)
{
    UINT16 index;
    TRACK_ITEM_S *track_run_item;
    UINT32 uv_save;
    if (usTrackType & g_track_mask)
    {
        uv_save = LOS_IntLock();
        index = g_pst_track_mgr->usCurIndex;
        track_run_item = &g_pst_track_mgr->pstTrackItems[index];
        track_run_item->usTrackType = usTrackType;
        track_run_item->usData0 = uwTrackId;
        if ((!uwEntry)&&(usTrackType&TRACK_TASK_FLAG))
        {
            LOS_TASK_CB *pst_task_cb;
            pst_task_cb = OS_TCB_FROM_TID(uwTrackId);
            track_run_item->uwData1 = ((TSK_CONTEXT_S*)(pst_task_cb->pStackPointer))->uwPC;
            track_run_item->uwEntry = (UINT32)(pst_task_cb->pfnTaskEntry);
        }
        else
        {
            track_run_item->uwData1 = uwUserData;
            track_run_item->uwEntry = uwEntry;
        }
        if (g_curr_user_get_hook != NULL)
        {
            (VOID)g_curr_user_get_hook(&(track_run_item->uwCurTime));
        }
        else
        {
            track_run_item->uwCurTime = g_ullTickCount * LOS_TRACK_US_PER_TICK;
        }
        g_pst_track_mgr->usCurIndex++;
        if (g_pst_track_mgr->usCurIndex >= g_pst_track_mgr->usMaxRecordCount)
        {
            g_pst_track_mgr->usCurIndex = 0;
        }
        LOS_IntRestore(uv_save);
    }
}


LITE_OS_SEC_TEXT_INIT_REDIRECT UINT32 LOS_CurUsRegHook(USER_CUR_US_FUNC pfnHook)
{
    UINTPTR uv_save;
    uv_save = LOS_IntLock();
    g_curr_user_get_hook = pfnHook;
    LOS_IntRestore(uv_save);
    return LOS_OK;
}

LITE_OS_SEC_TEXT_INIT_REDIRECT UINT32 LOS_TrackRegHook(TRACK_PROC_HOOK pfnHook)
{
    UINTPTR uv_save;
    (VOID)LOS_TrackDelete();
    uv_save = LOS_IntLock();
    g_pTrackHook = pfnHook;
    LOS_IntRestore(uv_save);
    return LOS_OK;
}

/*****************************************************************************
Function   : LOS_TrackInit
Description: Initializes memory of track manager and track items
Input      :  usMaxRecordCount:The  max track count of track items.
Output     : None
Return     : LOS_OK on success or error code on failure
*****************************************************************************/
LITE_OS_SEC_TEXT_INIT_REDIRECT UINT32 LOS_TrackInit(UINT16 usMaxRecordCount)
{
    UINT32 size;
    VOID *buffer = NULL;
    UINT32 uv_save;

    if (usMaxRecordCount > LOSCFG_BASE_MISC_TRACK_MAX_COUNT)
    {
        return LOS_ERRNO_TRACK_TOO_LARGE_COUNT;
    }

    if (usMaxRecordCount == 0)
    {
        return LOS_ERRNO_TRACK_PARA_ISZERO;
    }

    uv_save = LOS_IntLock();
    if (g_pst_track_mgr != NULL)
    {
        LOS_IntRestore(uv_save);
        return LOS_ERRNO_TRACK_RE_INIT;
    }
    size = LOS_TRACK_MGR_SIZE + sizeof(TRACK_ITEM_S) * usMaxRecordCount;

    buffer = LOS_MemAlloc(m_aucSysMem0, size);
    if (buffer == NULL)
    {
        LOS_IntRestore(uv_save);
        return LOS_ERRNO_TRACK_NO_MEMORY;
    }
    (VOID)memset_s(buffer, size, 0, size);
    g_pst_track_mgr = (TRACK_MGR_S *)buffer;
    g_pst_track_mgr->pstTrackItems = (TRACK_ITEM_S *)((UINT8 *)buffer + LOS_TRACK_MGR_SIZE);
    g_pst_track_mgr->usMaxRecordCount = usMaxRecordCount;
    g_pst_track_mgr->usCurIndex = 0;
    LOS_IntRestore(uv_save);
    return LOS_OK;
}


/*****************************************************************************
Function   : LOS_TrackStart
Description: Start  recording the track
Input      : usTrackMask:The  mask of information needed to track.
Output     : None
Return     : LOS_OK on success or error code on failure
*****************************************************************************/
LITE_OS_SEC_TEXT_INIT_REDIRECT UINT32 LOS_TrackStart(UINT16 usTrackMask)
{
    UINT32 uv_save;
    uv_save = LOS_IntLock();
    if (g_pst_track_mgr == NULL)
    {
        LOS_IntRestore(uv_save);
        return LOS_ERRNO_TRACK_NO_INIT;
    }
    g_track_mask = usTrackMask;
    LOS_IntRestore(uv_save);
    return LOS_OK;
}


/*****************************************************************************
Function   : LOS_TrackStop
Description: Stop recording track
Input      : None
Output     : None
Return     : usMaskSave:Save the mask of information needed to track.
*****************************************************************************/
LITE_OS_SEC_TEXT_INIT_REDIRECT UINT16 LOS_TrackStop(VOID)
{
    UINT32 uv_save;
    UINT16 us_mask_save;
    us_mask_save = g_track_mask;
    uv_save = LOS_IntLock();
    g_track_mask = 0;
    LOS_IntRestore(uv_save);
    return us_mask_save;
}


/*****************************************************************************
Function   : LOS_TrackInfoGet
Description: Get track manager info
Input      : None
Output     : pInfo:Pointer to the track manager information structure to be obtained.
Return     : LOS_OK on success or error code on failure
*****************************************************************************/
LITE_OS_SEC_TEXT_INIT_REDIRECT UINT32 LOS_TrackInfoGet(TRACK_MGR_S *pInfo)
{
    UINT32 uv_save;
    if (pInfo == NULL)
    {
        return LOS_ERRNO_TRACK_PTR_NULL;
    }
    uv_save = LOS_IntLock();
    if (g_pst_track_mgr == NULL)
    {
        LOS_IntRestore(uv_save);
        return LOS_ERRNO_TRACK_NO_INIT;
    }

    pInfo->usCurIndex = g_pst_track_mgr->usCurIndex;
    pInfo->usMaxRecordCount = g_pst_track_mgr->usMaxRecordCount;
    pInfo->pstTrackItems = g_pst_track_mgr->pstTrackItems;
    LOS_IntRestore(uv_save);
    return LOS_OK;
}

/*****************************************************************************
Function   : LOS_TrackItemGetById
Description: Get track item info by index
Input      :  usIndex:The index of track items.
Output     : pItemInfo:Pointer to the track item information structure to be obtained.
Return     : LOS_OK on success or error code on failure
*****************************************************************************/
LITE_OS_SEC_TEXT_INIT_REDIRECT UINT32 LOS_TrackItemGetById(UINT16 usIndex, TRACK_ITEM_S *pItemInfo)
{
    UINT32 uv_save;
    TRACK_ITEM_S *track_item = NULL;
    if (pItemInfo == NULL)
    {
        return LOS_ERRNO_TRACK_PTR_NULL;
    }

    uv_save = LOS_IntLock();
    if (g_pst_track_mgr == NULL)
    {
        LOS_IntRestore(uv_save);
        return LOS_ERRNO_TRACK_NO_INIT;
    }

    if (usIndex >= g_pst_track_mgr->usMaxRecordCount)
    {
        LOS_IntRestore(uv_save);
        return LOS_ERRNO_TRACK_INVALID_PARAM;
    }

    track_item = &g_pst_track_mgr->pstTrackItems[usIndex];
    if (memcpy_s(pItemInfo, sizeof(TRACK_ITEM_S), track_item, sizeof(TRACK_ITEM_S)) != EOK) {
        LOS_IntRestore(uv_save);
        return LOS_NOK;
    }
    LOS_IntRestore(uv_save);
    return LOS_OK;
}


/*****************************************************************************
Function   : LOS_TrackDelete
Description: Delete the track
Input      : None
Output     : None
Return     : LOS_OK on success or error code on failure
*****************************************************************************/
LITE_OS_SEC_TEXT_INIT_REDIRECT UINT32 LOS_TrackDelete(VOID)
{
    UINT32 result;
    UINT32 uv_save;
    uv_save = LOS_IntLock();
    if (g_pst_track_mgr == NULL)
    {
        LOS_IntRestore(uv_save);
        return LOS_ERRNO_TRACK_NO_INIT;
    }
    g_track_mask = 0;
    result = LOS_MemFree(m_aucSysMem0, (VOID *)g_pst_track_mgr);
    if (result != LOS_OK)
    {
        LOS_IntRestore(uv_save);
        return result;
    }
    g_pst_track_mgr = NULL;
    LOS_IntRestore(uv_save);
    return LOS_OK;
}



#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
