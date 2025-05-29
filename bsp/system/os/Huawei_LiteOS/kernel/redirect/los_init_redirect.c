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
#include "los_base.h"
#include "los_config.h"
#include "los_typedef.h"
#include "los_hwi.h"
#include "los_task.ph"
#include "los_memory.h"
#include "los_queue.ph"
#include <printf.h>
#include "los_memory.ph"
#include <securec.h>


#if (LOSCFG_PLATFORM_EXC == YES)
#include "los_exc.ph"
#endif

#if (LOSCFG_LIB_CONFIGURABLE == YES)
#include "los_build.h"
#include "los_memstat.ph"
#include "string.h"
#endif

#if (LOSCFG_BASE_MISC_TRACK_RAM == YES)
#include "los_track.h"
#endif

#if (LOSCFG_LIB_CONFIGURABLE == YES)
#if (LOSCFG_LIB_LIBC == YES)
extern int *errno_array;
extern CHAR LOS_BLOCK_START(errno);
extern CHAR LOS_BLOCK_END(errno);
#endif

extern TSK_MEM_USED_INFO *g_TskMemUsedInfo;
extern CHAR LOS_BLOCK_START(sysmemused);
extern CHAR LOS_BLOCK_END(sysmemused);
#endif


#if (LOSCFG_LIB_CONFIGURABLE == YES)
extern UINT32 g_uwOsSysClock;
extern UINT32 g_uwSemLimit;
extern UINT32 g_uwMuxLimit;
extern UINT32 g_uwQueueLimit;
extern UINT32 g_uwSwtmrLimit;
extern UINT32 g_uwTskLimit;
extern UINT32 g_uwMinusOneTickPerSecond;
extern UINT32 g_uwTskMinStkSize;
extern UINT32 g_uwTskIdleStkSize;
extern UINT32 g_uwTskDfltStkSize;
extern UINT32 g_uwTimeSliceTimeOut;
extern UINT32 g_uwWorkQueuePri;
extern UINT32 g_uwWorkQueueStackSize;
#endif

extern void platform_tick_handler(void);
extern void platform_get_cycle(UINT32 *puwCntHi, UINT32 *puwCntLo);
/*****************************************************************************
 Function    : on_register
 Description : Configuring the maximum number of tasks
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
LITE_OS_SEC_TEXT_INIT_REDIRECT void board_config(void)
{
    g_sys_mem_addr_end = (UINT32)hal_sectors_ram_start + hal_sectors_ram_size - 1;
}

static VOID err_handle(CHAR* file_name, UINT32 line_no, UINT32 error_no, UINT32 para_len, VOID* para)
{
    if ((error_no == OS_ERRNO_MEMCHECK_ERR)||
        (error_no == LOS_ERRNO_TSK_STKAREA_TOO_SMALL)||
        (error_no == LOS_ERRNO_TSK_STACK_POINTER_ERROR))
    {
        LOS_Panic((const char * )error_no, para);//LOS_Panic_Patch
    }
}

/*****************************************************************************
 Function    : on_register
 Description : Configuring the maximum number of tasks
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
LITE_OS_SEC_TEXT_INIT_REDIRECT static VOID on_register(VOID)
{
#if (LOSCFG_LIB_CONFIGURABLE == YES)
    g_uwTskLimit = LOSCFG_BASE_CORE_TSK_LIMIT_CONFIG;
    g_uwTskMaxNum = g_uwTskLimit + 1;

#if (LOSCFG_LIB_LIBC == YES)
    errno_array = (INT32 *)&LOS_BLOCK_START(errno);
    (VOID)memset_s(errno_array, (&LOS_BLOCK_END(errno) - &LOS_BLOCK_START(errno)),
        0, (&LOS_BLOCK_END(errno) - &LOS_BLOCK_START(errno)));
#endif

    g_TskMemUsedInfo = (TSK_MEM_USED_INFO *)&LOS_BLOCK_START(sysmemused);
    (VOID)memset_s(g_TskMemUsedInfo, (&LOS_BLOCK_END(sysmemused) - &LOS_BLOCK_START(sysmemused)),
        0, (&LOS_BLOCK_END(sysmemused) - &LOS_BLOCK_START(sysmemused)));

    g_uwOsSysClock = OS_SYS_CLOCK_CONFIG;
    g_uwMinusOneTickPerSecond = LOSCFG_BASE_CORE_TICK_PER_SECOND_CONFIG - 1; /* tick per sencond minus one */
    g_uwSemLimit = LOSCFG_BASE_IPC_SEM_LIMIT_CONFIG;
    g_uwMuxLimit = LOSCFG_BASE_IPC_MUX_LIMIT_CONFIG;
    g_uwQueueLimit = LOSCFG_BASE_IPC_QUEUE_LIMIT_CONFIG;
    g_uwSwtmrLimit = LOSCFG_BASE_CORE_SWTMR_LIMIT_CONFIG;
    g_uwTskMinStkSize = LOSCFG_BASE_CORE_TSK_MIN_STACK_SIZE_CONFIG;
    g_uwTskIdleStkSize = LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE_CONFIG;
    g_uwTskDfltStkSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE_CONFIG;
    g_uwTimeSliceTimeOut = LOSCFG_BASE_CORE_TIMESLICE_TIMEOUT_CONFIG;
    g_uwWorkQueuePri = LOSCFG_BASE_WORKQUEUE_TASK_PRIO_CONFIG;
    g_uwWorkQueueStackSize = LOSCFG_BASE_WORKQUEUE_TASK_STACK_SIZE_CONFIG;
    g_stUserErrFunc.pfnHook = (LOS_ERRORHANDLE_FUNC)err_handle;
#else
    g_uwTskMaxNum = LOSCFG_BASE_CORE_TSK_LIMIT + 1; /* Reserved 1 for IDLE */
#endif
    g_pIntEntry = (HWI_PROC_FUNC)osInterrupt;
    g_pTickHandler = (PLATFORM_TICK_HANDLER)platform_tick_handler;
    g_pCycleGetFunc = (PLATFORM_GET_CYCLE_FUNC)platform_get_cycle;
    return;
}

/*****************************************************************************
 Function    : LOS_Start
 Description : Task start function
 Input       : None
 Output      : None
 Return      : LOS_OK on success or error code on failure
 *****************************************************************************/
LITE_OS_SEC_TEXT_INIT_REDIRECT UINT32 LOS_Start(VOID)
{
    UINT32 result;
#if (LOSCFG_BASE_CORE_TICK_HW_TIME == NO)
    result = osTickStart();
#else
    extern int os_timer_init(void);
    result = (UINT32)os_timer_init();
#endif
    if (result != LOS_OK)
    {
        PRINT_ERR("Liteos system timer init error\n");
        return result;
    }
    LOS_StartToRun();
    return result;
}

LITE_OS_SEC_TEXT_INIT_REDIRECT VOID osExcInit(UINT32 uwArraySize)
{
    m_pstHwiForm[-14 + OS_SYS_VECTOR_CNT] = (HWI_PROC_FUNC)osExcNMI;
    m_pstHwiForm[-13 + OS_SYS_VECTOR_CNT] = (HWI_PROC_FUNC)osExcHardFault;
    m_pstHwiForm[-5 + OS_SYS_VECTOR_CNT] =  (HWI_PROC_FUNC)osExcSvcCall;

#if (LOSCFG_SAVE_EXC_INFO == YES)
    g_uwArraySize = uwArraySize;

#if (LOSCFG_LIB_CONFIGURABLE == YES)
    m_aucTaskArray =  (UINT8 *)LOS_MemAlloc(m_aucSysMem0, uwArraySize);
    if(NULL == m_aucTaskArray)
    {
        PRINT_ERR("osExcInit error\n");
    }
#endif
    osExcRegister(OS_EXC_TYPE_NVIC, (EXC_INFO_SAVE_CALLBACK)osExcSaveIntStatus, NULL);
#endif
}


/*****************************************************************************
 Function    : LOS_KernelInit
 Description : System kernel initialization function, configure all system modules
 Input       : None
 Output      : None
 Return      : LOS_OK on success or error code on failure
 *****************************************************************************/
 
extern TSKSWITCHHOOK g_pfnTskSwitchHook;

VOID task_switch_check_new(VOID)
{
    g_uwCurNestCount++;
    osTaskSwitchCheck();
    g_uwCurNestCount--;
}

LITE_OS_SEC_TEXT_INIT_REDIRECT UINT32 LOS_KernelInit(VOID)
{
    UINT32 result;

    on_register();

    m_aucSysMem0 = OS_SYS_MEM_ADDR;
    result = osMemSystemInit();
    if (result != LOS_OK)
    {
        PRINT_ERR("osMemSystemInit error %d\n", result);/*lint !e515*/
        return result;
    }

#if (LOSCFG_PLATFORM_HWI == YES)
    {
        osHwiInit();
    }
#endif

#if (LOSCFG_PLATFORM_EXC == YES)
    {
        osExcInit(MAX_EXC_MEM_SIZE);
    }
#endif

    result =osTaskInit(NULL);
    if (result != LOS_OK)
    {
        PRINT_ERR("osTaskInit error\n");
        return result;
    }

#if (LOSCFG_BASE_CORE_TSK_MONITOR == YES)
    {
        osTaskMonInit();
        g_pfnTskSwitchHook = task_switch_check_new;
    }
#endif


#if (LOSCFG_BASE_IPC_SEM == YES)
    {
        result = osSemInit(NULL);
        if (result != LOS_OK)
        {
            return result;
        }
    }
#endif

#if (LOSCFG_BASE_IPC_MUX == YES)
    {
        result = osMuxInit(NULL);
        if (result != LOS_OK)
        {
            return result;
        }
    }
#endif

#if (LOSCFG_BASE_IPC_QUEUE == YES)
    {
        result = osQueueInit(NULL);
        if (result != LOS_OK)
        {
            PRINT_ERR("osQueueInit error\n");
            return result;
        }
    }
#endif

#if (LOSCFG_BASE_CORE_SWTMR == YES)
    {
        result = osSwTmrInit(NULL);
        if (result != LOS_OK)
        {
            PRINT_ERR("osSwTmrInit error\n");
            return result;
        }
    }
#endif

#if (LOSCFG_BASE_MISC_TRACK_RAM == YES)
    {
        result = LOS_TrackInit(LOSCFG_BASE_MISC_TRACK_COUNT_CONFIG);
        if (result != LOS_OK)
        {
            PRINT_ERR("LOS_TrackInit error\n");
            return result;
        }
        (VOID)LOS_TrackStart(0xFFFF);
    }
#endif

#if(LOSCFG_BASE_CORE_TIMESLICE == YES)
    osTimesliceInit();
#endif

    result = osIdleTaskCreate();
    if (result != LOS_OK)
    {
        return result;
    }

#if (LOSCFG_TEST == YES)
    result = los_TestInit();
    if (result != LOS_OK)
    {
        return result;
    }
#endif

    return LOS_OK;
}

