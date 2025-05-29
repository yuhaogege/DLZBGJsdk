#include "stdlib.h"
#include "string.h"
#include "los_base.h"
#include "los_config.h"
#include "los_typedef.h"
#include "los_hwi.h"
#include "los_task.ph"
#include "los_sem.h"
#include "los_event.h"
#include "los_memory.h"
#include "los_queue.ph"
#include "los_track.h"
#include "los_exc.h"
#include "los_runstop.h"
#include <stdio.h>

#include "dw21_platform.h"
#include "uart.h"
#include "serial_dw.h"
#include "soc_mdm_clock.h"
#include "fpb_lib.h"
#include "printf.h"
#include "workqueue.h"
#include <soc_mdm_flash.h>
#include <soc_mdm_timecnt.h>
#include <soc_mdm_crash.h>
#include <securec.h>
#include <soc_mdm_mem.h>
#include <soc_mdm_isr.h>
#include <soc_lowpower.h>
#ifdef SUPPORT_DEEP_SLEEP
#include <soc_timecnt.h>
#endif
#include <soc_mdm_symtab.h>

extern UINT32 g_uwSysClock;
extern int uart_puts(char *ptr, unsigned int len, void *state);
extern td_void uapi_app_init(td_void);
extern void board_config(void);
extern UINT32 LOS_HwiCreate_Patch( HWI_HANDLE_T  uwHwiNum,
                           HWI_PRIOR_T   usHwiPrio,
                           HWI_MODE_T    usMode,
                           HWI_PROC_FUNC pfnHandler,
                           HWI_ARG_T     uwArg
                           );

#ifdef USE_REMAP
extern td_u32 swap_init(void);
#endif

td_void app_init(void);

#if (LOSCFG_BASE_MISC_TRACK_RAM == YES)
void track_config(void)
{
    LOS_TrackRegHook(osTrackhook);
    LOS_CurUsRegHook((USER_CUR_US_FUNC)SYMTAB_TIME_GETUS);
}
#endif
extern unsigned usr_remap_init(void);
unsigned uapi_event_init(unsigned char max_event_cnt, void* p_event_space);

LITE_OS_SEC_TEXT_INIT_REDIRECT td_void app_init(void)
{
    
    uapi_event_init(4, TD_NULL);
#ifdef USE_REMAP
    uapi_flash_init();
    usr_remap_init();
    *(volatile unsigned int *)(0x2040000);
    *(volatile unsigned int *)(0x2041000);
    *(volatile unsigned int *)(0x2042000);
    *(volatile unsigned int *)(0x2043000);
    *(volatile unsigned int *)(0x2044000);
    *(volatile unsigned int *)(0x2045000);
    *(volatile unsigned int *)(0x2046000);
    *(volatile unsigned int *)(0x2047000);
#endif

    uapi_app_init();
}

extern td_void patch_init(td_void);
extern void hal_sectors_ram_patch(void);

LITE_OS_SEC_TEXT_INIT_REDIRECT void board_resource_init(void)
{
    hal_sectors_ram_patch();
}

LITE_OS_SEC_TEXT_INIT_REDIRECT int main(void)
{
    UINT32 result;

    uapi_uart_set_clock(UART_CLK_APB);
    app_io_init();

    serial_init (UART0_BASE, 115200);
    serial_init (UART1_BASE, 115200);
    register_log_hook(uart_puts);
    board_resource_init();
    los_printf("%s\r\n",KERNEL_START);

     /* The code before FPB initialization does not support patching. If the previous code needs patching, you need to compile the FPB module as a resident area */
    patch_init();

    board_config();
#if (LOSCFG_BASE_MISC_TRACK_RAM == YES)
    track_config();
#endif

    result = LOS_KernelInit();
    g_uwSysClock = OS_SYS_CLOCK;

#if !defined DW21TEST
    (VOID)init_default_workqueue(10);   /* after testing, 257 sta large agent environments of flash two lines are 2 */
#endif

#if !defined DW21TEST
    (VOID)app_init();
    #ifdef SUPPORT_DEEP_SLEEP
    uapi_low_accuracy_time_counter_enable();
    #endif
	LOS_RunStopCallBackRegister(uapi_lowpower_handle);	
#endif

    if (result == LOS_OK)
    {
        (void)LOS_Start();/*start liteos, waiting for task switch*/
    }
    else
    {
        los_printf("LOS_KernelInit failed\r\n");
    }
    while(1);
    /* Replace the dots (...) with your own code.  */
    return 0;
}
