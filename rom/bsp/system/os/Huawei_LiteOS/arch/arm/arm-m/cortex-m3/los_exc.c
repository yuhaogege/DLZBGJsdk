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
#include "stdarg.h"
#include "los_exc.inc"
#include "los_sys.ph"
#include "los_base.ph"
#include "los_task.ph"
#ifdef LOSCFG_UNWIND_BACKTRACE
#include "backtrace_common.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

LITE_OS_SEC_RODATA UINT8 g_uwExcTbl[32] = {
                        0, //31bit
                        0, //30bit
                        0, //29bit
                        0, //28bit
                        0, //27bit
                        0, //26bit
                        OS_EXC_UF_DIVBYZERO, //25bit 除法错误---用法错误
                        OS_EXC_UF_UNALIGNED,//24bit 未对齐访问---用法错误
                        0,//23bit
                        0,//22bit
                        0,//21bit
                        0,//20bit
                        OS_EXC_UF_NOCP, //19bit试图执行协处理器相关指令---用法错误
                        OS_EXC_UF_INVPC, //18bit在异常返回时试图非法地加载 EXC_RETURN到 PC---用法错误
                        OS_EXC_UF_INVSTATE, //17bit试图切入 ARM 状态---用法错误
                        OS_EXC_UF_UNDEFINSTR,//16bit执行的指令其编码是未定义的---用法错误
                        0,//15bit
                        0,//14bit
                        0,//13bit
                        OS_EXC_BF_STKERR,//12bit入栈时发生错误--busfault
                        OS_EXC_BF_UNSTKERR,//11bit出栈时发生错误---busfault
                        OS_EXC_BF_IMPRECISERR,//10bit不精确的数据访问违例---busfault
                        OS_EXC_BF_PRECISERR,//9bit精确的数据访问违例---busfault
                        OS_EXC_BF_IBUSERR,//8bit取指时的访问违例---busfault
                        0,//7bit 
                        0,//6bit
                        0,//5bit
                        OS_EXC_MF_MSTKERR,//4bit入栈时发生错误---mpufault
                        OS_EXC_MF_MUNSTKERR,//3bit出栈时发生错误---mpufault
                        0,//2bit 
                        OS_EXC_MF_DACCVIOL,//1bit数据访问违例----mpufault
                        OS_EXC_MF_IACCVIOL //0bit取指访问违例----mpufault
                       };

extern int _dprintf(const char *fmt, va_list ap);

LITE_OS_SEC_TEXT __attribute__ ((noinline))void LOS_Panic(const char * fmt, ...)
{
    va_list ap;
    va_start(ap,fmt);
    (void)_dprintf(fmt, ap);
    va_end(ap);
    __asm__ __volatile__("swi 0");
}/*lint !e438*/

/*lint -e528*/
LITE_OS_SEC_TEXT static UINT32 osGetSP(VOID)
{
    UINT32 uwSpSave;

    __asm__ __volatile__(
        "mov %0, SP"
        : "=r"(uwSpSave));

    return uwSpSave; /*lint !e530*/
}
/*lint -e528*/

LITE_OS_SEC_TEXT VOID osBackTrace(VOID)
{
#ifdef LOSCFG_UNWIND_BACKTRACE
    EXC_CONTEXT_S stContext = {0};
    stContext.uwSP = osGetSP() - 4 /*current_stack_pointer*/;
    stContext.uwLR = (unsigned long)__builtin_return_address(0);
    stContext.uwPC = (unsigned long)osBackTrace;

    osUnwindBacktrace(&stContext);
#endif
}

LITE_OS_SEC_DATA_INIT EXC_PROC_FUNC g_pExcHook = NULL;
/*****************************************************************************
Function   : LOS_ExcRegHook
Description: register user hook function of EXC
Input   : pfnHook --- user hook function
Output  : None
Return  : None
Other   : None
*****************************************************************************/
LITE_OS_SEC_TEXT_INIT UINT32 LOS_ExcRegHook(EXC_PROC_FUNC pfnHook)
{
    UINTPTR uvIntSave;
    uvIntSave = LOS_IntLock();
    g_pExcHook = pfnHook;
    LOS_IntRestore(uvIntSave);
    return LOS_OK;
}

#if 0
__attribute__ ((section(".hot.text"))) __attribute__ ((noinline))void LOS_Panic_Patch(const char * fmt, ...)
{

    /*
        r0:0x0220553c "///////[LOS_MemFreeNodeCheck] Task ID %d in pre node is invalid!////////\n"
        r1:uwTaskID


        r0:0x02205588 "\r\n[LOS_MemFreeNodeCheck] Task ID %d in pre node  is not created!\n"
        r1:uwTaskID


        r0:0x022055cc "[LOS_MemFreeNodeCheck] cur node: 0x%x\n""pre node: 0x%x\n""pre node was allocated by task:%s\n"
        r1:pstNode
        r2:pstNode->pstPreNode
        r3:pstTaskCB->pcTaskName

        
        不执行 
        r0:0x02205754 "Task ID %d in pre node is invalid!\n"
        r1:uwTaskID

        
        不执行 
        r0:0x02205778 "\r\nTask ID %d in pre node  is not created!\n"
        r1:uwTaskID

        
        不执行 
        r0:0x022057a4 "cur node: 0x%x\n""pre node: 0x%x\n""pre node was allocated by task:%s\n"
        r1:pstTmpNode
        r2:pstPreNode
        r3:pstTaskCB->pcTaskName


        r0:0x02205918 "[%u]stack-protector: Kernel stack is corrupted in: %p\n"
        r1:100
        r2:__builtin_return_address(0)



        r0:0x02000106 LOS_MemIntegrityCheck中LOS_ErrHandle
        r1:pstTmpNode


        r0:0x02000218 osTaskSwitchCheck中第一个LOS_ErrHandle
        r1:&g_stLosTask.pstRunTask->uwTaskID

        r0:0x02000223 osTaskSwitchCheck中第二个LOS_ErrHandle
        r2:&g_stLosTask.pstNewTask->uwTaskID
    */

    __asm__ __volatile__("swi 1");
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


