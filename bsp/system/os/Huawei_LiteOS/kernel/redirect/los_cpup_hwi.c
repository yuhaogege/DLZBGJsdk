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
#include "string.h"
#include "los_task.ph"
#include "los_memory.ph"
#include "los_cpup.ph"
#include "los_cpup_hwi.h"
#include "los_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#if (LOSCFG_BASE_CORE_CPUP_RAM == YES)

extern UINT32 g_uwTimeInIrqPerTskSwitch;
extern OS_CPUP_S*    g_pstCpup;
extern UINT32        g_uwTskMaxNum;
extern UINT16        g_usCpupInitFlg;

#if(LOSCFG_BASE_CORE_CPUP_HWI_RAM == YES)
UINT64 g_time_start = 0;
#endif

extern UINT64 osGetCpuCycle(VOID);
extern VOID osTskCycleEnd(VOID);
extern VOID osTskCycleStart(VOID);

#if(LOSCFG_BASE_CORE_CPUP_HWI_RAM == YES)
LITE_OS_SEC_TEXT_MINOR_REDIRECT VOID os_cpup_lrq_start(VOID)
{
    g_time_start = osGetCpuCycle();
}

LITE_OS_SEC_TEXT_MINOR_REDIRECT VOID os_cpup_lrq_end(UINT32 uwID)
{
    UINT64 time_end = osGetCpuCycle();
    g_pstCpup[g_uwTskMaxNum + uwID].uwID = uwID;
    g_pstCpup[g_uwTskMaxNum + uwID].usStatus = OS_TASK_STATUS_RUNNING;

    if (time_end <= g_time_start)
    {
        g_uwTimeInIrqPerTskSwitch += (OS_SYS_CLOCK / LOSCFG_BASE_CORE_TICK_PER_SECOND - g_time_start + time_end);
        g_pstCpup[g_uwTskMaxNum + uwID].ullAllTime += (OS_SYS_CLOCK / LOSCFG_BASE_CORE_TICK_PER_SECOND -
            g_time_start + time_end);
    }
    else
    {
        g_uwTimeInIrqPerTskSwitch += (time_end - g_time_start);
        g_pstCpup[g_uwTskMaxNum + uwID].ullAllTime += (time_end - g_time_start);
    }
}
#endif


#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


