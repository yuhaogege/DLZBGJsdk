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

#ifndef _LOS_EXC_H
#define _LOS_EXC_H

#include "los_hwi.h"
#include "los_task.ph"
#include "los_queue.h"
#include "los_config.h"
#include "los_memcheck.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#define OS_EXC_IN_INIT                      0
#define OS_EXC_IN_TASK                      1
#define OS_EXC_IN_HWI                       2

#define OS_EXC_MAX_BUF_LEN                  25
#define OS_EXC_MAX_NEST_DEPTH               1

#define OS_NVIC_SHCSR                       0xE000ED24
#define OS_NVIC_CCR                         0xE000ED14

#define OS_NVIC_INT_ENABLE_SIZE             0x20
#define OS_NVIC_INT_PRI_SIZE                0xF0

#define OS_NVIC_INT_PEND_SIZE               OS_NVIC_INT_ACT_SIZE
#define OS_NVIC_INT_ACT_SIZE                OS_NVIC_INT_ENABLE_SIZE

#define OS_EXC_FLAG_NO_FLOAT                0x10000000
#define OS_EXC_FLAG_FAULTADDR_VALID         0x01
#define OS_EXC_FLAG_IN_HWI                  0x02

#define OS_EXC_IMPRECISE_ACCESS_ADDR        0xABABABAB

/**
 *@ingroup los_exc
 * the struct of register files
 *
 * description: the register files that saved when exception triggered
 *
 * notes:the following register with prefix 'uw'  correspond to the registers in the cpu  data sheet.
 */
typedef struct tagExcContext
{
    //handler save
#if ((defined (__FPU_PRESENT) && (__FPU_PRESENT == 1U)) && \
     (defined (__FPU_USED   ) && (__FPU_USED    == 1U))     )
    UINT32 S16;
    UINT32 S17;
    UINT32 S18;
    UINT32 S19;
    UINT32 S20;
    UINT32 S21;
    UINT32 S22;
    UINT32 S23;
    UINT32 S24;
    UINT32 S25;
    UINT32 S26;
    UINT32 S27;
    UINT32 S28;
    UINT32 S29;
    UINT32 S30;
    UINT32 S31;
#endif
    UINT32 uwR4;
    UINT32 uwR5;
    UINT32 uwR6;
    UINT32 uwR7;
    UINT32 uwR8;
    UINT32 uwR9;
    UINT32 uwR10;
    UINT32 uwR11;
    UINT32 uwPriMask;
    //auto save
    UINT32 uwSP;
    UINT32 uwR0;
    UINT32 uwR1;
    UINT32 uwR2;
    UINT32 uwR3;
    UINT32 uwR12;
    UINT32 uwLR;
    UINT32 uwPC;
    UINT32 uwxPSR;
#if ((defined (__FPU_PRESENT) && (__FPU_PRESENT == 1U)) && \
     (defined (__FPU_USED   ) && (__FPU_USED    == 1U))     )
    UINT32 S0;
    UINT32 S1;
    UINT32 S2;
    UINT32 S3;
    UINT32 S4;
    UINT32 S5;
    UINT32 S6;
    UINT32 S7;
    UINT32 S8;
    UINT32 S9;
    UINT32 S10;
    UINT32 S11;
    UINT32 S12;
    UINT32 S13;
    UINT32 S14;
    UINT32 S15;
    UINT32 FPSCR;
    UINT32 NO_NAME;
#endif
}EXC_CONTEXT_S;

typedef UINT32 (*EXC_INFO_SAVE_CALLBACK)(UINT32 , VOID* );
typedef VOID (* EXC_PROC_FUNC)(UINT32, EXC_CONTEXT_S *);
VOID osExcHandleEntry(UINT32 exc_type, UINT32 uwFaultAddr, UINT32 uwPid, EXC_CONTEXT_S *  pstExcBufAddr);

/**
 * @ingroup  los_hwi
 * @brief: Exception initialization.
 *
 * @par Description:
 * This API is used to configure the exception function vector table.
 *
 * @attention:
 * <ul><li>None.</li></ul>
 *
 *@param uwArraySize [IN] Memory size of exception.
 *
 * @retval: None
 * @par Dependency:
 * <ul><li>los_hwi.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 * @since Huawei LiteOS V100R001C00
 */
VOID osExcInit(UINT32 uwArraySize);

extern VOID osExcNMI(VOID);
extern VOID osExcHardFault(VOID);
extern VOID osExcMemFault(VOID);
extern VOID osExcBusFault(VOID);
extern VOID osExcUsageFault(VOID);
extern VOID osExcSvcCall(VOID);
extern VOID osBackTrace(VOID);
#if (LOSCFG_LIB_CONFIGURABLE == YES)
extern UINT8 *m_aucTaskArray;
#else
extern UINT8 m_aucTaskArray[];
#endif

/**
 *@ingroup los_exc
 *@brief Kernel panic function.
 *
 *@par Description:
 *Stack function that prints kernel panics.
 *@attention After this function is called and stack information is printed, the system will fail to respond.
 *@attention The input parameter can be NULL.
 *@param fmt [IN] Type #char* : variadic argument.
 *
 *@retval #None.
 *
 *@par Dependency:
 *los_exc.h: the header file that contains the API declaration.
 *@see None.
 *@since Huawei LiteOS V100R001C00
*/
void LOS_Panic(const char * fmt, ...);

void LOS_Panic_Patch(const char * fmt, ...);

/**
 *@ingroup los_exc
 *Cortex-M3 exception specific type: An error occurred when the bus status register was pushed onto the stack
 */
#define OS_EXC_BF_STKERR           1

/**
 *@ingroup los_exc
 *Cortex-M3 exception specific type: An error occurred when the bus status register was popped
 */
#define OS_EXC_BF_UNSTKERR         2

/**
 *@ingroup los_exc
 *Cortex-M3 exception specific type: Inaccurate data access violation with bus status registers
 */
#define OS_EXC_BF_IMPRECISERR      3

/**
 *@ingroup los_exc
 *Cortex-M3 exception specific type: Accurate data access violation with bus status registers
 */
#define OS_EXC_BF_PRECISERR        4

/**
 *@ingroup los_exc
 *Cortex-M3 exception specific type: Access violation with bus status registers' fetcing instruction
 */
#define OS_EXC_BF_IBUSERR          5

/**
 *@ingroup los_exc
 *Cortex-M3 exception specific type: An error occurred when the memory management status register was pushed onto the stack
 */
#define OS_EXC_MF_MSTKERR          6

/**
 *@ingroup los_exc
 *Cortex-M3 exception specific type: An error occurred when the memory management status register was popped
 */
#define OS_EXC_MF_MUNSTKERR        7

/**
 *@ingroup los_exc
 *Cortex-M3 exception specific type: data access violation with memory management status register
 */
#define OS_EXC_MF_DACCVIOL         8

/**
 *@ingroup los_exc
 *Cortex-M3 exception specific type: Access violation with memory management status registers' fetcing instruction
 */
#define OS_EXC_MF_IACCVIOL         9


/**
 *@ingroup los_exc
 *Cortex-M3 exception specific type: usage error, divisor is zero when dividing
 */
#define OS_EXC_UF_DIVBYZERO        10

/**
 *@ingroup los_exc
 *Cortex-M3 exception specific type: usage error, errors caused by unaligned access
 */
#define OS_EXC_UF_UNALIGNED        11

/**
 *@ingroup los_exc
 *Cortex-M3 exception specific type: usage error, attempt to exection instructions related with coprocessor
 */
#define OS_EXC_UF_NOCP             12

/**
 *@ingroup los_exc
 *Cortex-M3 exception specific type: usage error, attempt to illegally load EXC_RETURN to PC when the exception returns
 */
#define OS_EXC_UF_INVPC            13

/**
 *@ingroup los_exc
 *Cortex-M3 exception specific type: usage error, attempt to switch to AMR state
 */
#define OS_EXC_UF_INVSTATE         14

/**
 *@ingroup los_exc
 *Cortex-M3 exception specific type: usage error, the code of the executed instruction is undefined-Not decoding 
 */
#define OS_EXC_UF_UNDEFINSTR       15

/**
 *@ingroup los_exc
 *Cortex-M3 exception specific type: NMI interruption
 */

#define OS_EXC_CAUSE_NMI           16

/**
 *@ingroup los_exc
 *Cortex-M3 exception specific type:hard fault
 */
#define OS_EXC_CAUSE_HARDFAULT     17

/**
 *@ingroup los_exc
 *Cortex-M3 exception specific type: Task processing function exists
 */
#define OS_EXC_CAUSE_TASK_EXIT     18

/**
 *@ingroup los_exc
 *Cortex-M3 exception specific type: Fatal error
 */
#define OS_EXC_CAUSE_FATAL_ERR     19

/**
 *@ingroup los_exc
 *Cortex-M3 exception specific type: Hard fault caused by debug event
 */
#define OS_EXC_CAUSE_DEBUGEVT      20

/**
 *@ingroup los_exc
 *Cortex-M3 exception specific type: A hard fault that occues when taking a vector
 */
#define OS_EXC_CAUSE_VECTBL        21

/**
 *@ingroup los_exc
 *Cortex-M3 bus fault
 */
#define OS_EXC_CAUSE_BUSFAULT        22

/**
 *@ingroup los_exc
 */

/**
 *@ingroup los_exc
 *Cortex-M3 exception specific type: hard fault
 */
#define OS_EXC_CAUSE_FROM_SVC     23

#define OS_EXC_CAUSE_FROM_WATCH_DOG     24


#define OS_EXC_CAUSE_SVC            0xf0

/**
 *@ingroup los_exc
 * abnormal information structure
 *
 * Description: Exception information saved when an exception is triggered on the M4 platform
 *
 */
typedef struct tagExcInfo
{
    UINT16 usPhase;              /**< stage of exception occurrence: 0 denotes that an exception occurs in initialization, 1 denotes that an exception occurs in a task, 2 denotes that an exception occurs in an interrupt */
    UINT16 usType;               /**< Exception type, check the numbers 1-19 listed above when an exception occurs */
    UINT32 uwFaultAddr;          /**< If it is an exact address access error, it means that the address was incorrectly accessed when the exception occurred */
    UINT32 uwThrdPid;            /**< An exception occurred during the interrupt, indicating the interrupt number. An exception occurred in the task, indicating the task id, or 0xffffffff if it occurred during initialization */
    UINT16 usNestCnt;            /**< numbetr of abnormal nested function, currently only supports hook functions that execute registration when the exception is entered for the first time */
    UINT16 usReserved;           /**< reserved */
    EXC_CONTEXT_S * pstContext;  /**< Hardware context at the time of the exception of the automatic push floating-point register */
}EXC_INFO_S;

extern UINT32 g_uwCurNestCount;
extern UINT32 g_vuwIntCount;
extern EXC_INFO_S m_stExcInfo;

VOID osExcInfoDisplay(EXC_INFO_S *pstExc);

extern OS_TASK_SWITCH_INFO g_astTskSwitchInfo;
extern UINT8 g_uwExcTbl[32];

extern UINT32 LOS_ExcRegHook(EXC_PROC_FUNC pfnHook);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* _LOS_EXC_H */

