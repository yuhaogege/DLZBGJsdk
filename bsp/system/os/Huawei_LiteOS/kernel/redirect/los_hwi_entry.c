#include "los_hwi.h"

#if (LOSCFG_BASE_MISC_TRACK_RAM ==YES)
#include "los_track.h"
#endif

volatile UINT32 g_vuw_act;

#if (LOSCFG_BASE_CORE_CPUP_HWI_RAM == YES)
extern VOID os_cpup_lrq_start(VOID);
extern VOID os_cpup_lrq_end(UINT32);
#endif
unsigned int g_cur_interrupt_idx=0;



/*****************************************************************************
 Function    : osInterrupt
 Description : Hardware interrupt entry function
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
LITE_OS_SEC_TEXT_REDIRECT VOID  osInterrupt(VOID)
{
    UINT32 index;
    UINT32 save;

#if(LOSCFG_KERNEL_RUNSTOP == YES)
    SCB->SCR &= (UINT32)~((UINT32)SCB_SCR_SLEEPDEEP_Msk);
#endif

    save = LOS_IntLock();
    g_vuwIntCount++;
    LOS_IntRestore(save);

    index = osIntNumGet();
    
    g_cur_interrupt_idx = index-OS_SYS_VECTOR_CNT;

#if (LOSCFG_BASE_CORE_CPUP_HWI_RAM == YES)
    if (index >= (OS_SYS_VECTOR_CNT - 1))
    {
        os_cpup_lrq_start();
    }
#endif

#if (LOSCFG_BASE_MISC_TRACK_RAM == YES)
    if (g_pTrackHook != NULL )
    {
        g_pTrackHook(TRAP_INT, (UINT16)index, 0, 0);
    }
#endif

    g_vuw_act = NVIC->IABR[0];
#if (OS_HWI_WITH_ARG == YES)
    if (m_pstHwiSlaveForm[index].pfnHandler!=0)
    {
        m_pstHwiSlaveForm[index].pfnHandler((UINT32)(VOID*)m_pstHwiSlaveForm[index].pParm);
    }
#else
    if (m_pstHwiSlaveForm[index] !=0)
    {
        m_pstHwiSlaveForm[index]();
    }
#endif

    save = LOS_IntLock();
    g_vuwIntCount--;
    
#if (LOSCFG_BASE_CORE_CPUP_HWI_RAM == YES)
    if (index >= (OS_SYS_VECTOR_CNT - 1))
    {
        os_cpup_lrq_end(index-(OS_SYS_VECTOR_CNT - 1));
    }
#endif

    LOS_IntRestore(save);

}

