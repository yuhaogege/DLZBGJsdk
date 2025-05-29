#include "los_hwi.h"
#include "los_tick.ph"
#include "soc_timer.h"
#include <soc_mdm_symtab.h>

extern UINT32 g_int_enable_status;
#define RELOAD_VALUE_25M    (EXT_XTAL_CLOCK_BASE * 1000000  / LOSCFG_BASE_CORE_TICK_PER_SECOND)
LITE_OS_SEC_TEXT_REDIRECT void platform_tick_handler(void)
{
    timer_type* timer = (timer_type*)TIMER5_REG_BASE;
    int intval = timer->eoi;
    int reload_val = timer->load_count;
  
    (td_void)intval;
    if (reload_val != RELOAD_VALUE_25M)
    {
        timer->ctrl_reg = 0x0;
        timer->load_count = RELOAD_VALUE_25M;
        timer->ctrl_reg |= TIMER_INT_UNMASK<<2; /* left(2) */
        timer->ctrl_reg |= TIMER_MODE_CYCLE<<1;
        timer->ctrl_reg |= 0x1;
    }
}
/*获取64位表示的circle数*/
LITE_OS_SEC_TEXT_INIT_REDIRECT void platform_get_cycle(UINT32 *puw_cnt_hi, UINT32 *puw_cnt_low)
{

    timer_type* timer = (timer_type*)TIMER5_REG_BASE;

    UINT64 ull_sw_tick;
    UINT64 ull_cycle;
    UINT32 uw_hw_cycle;
    ull_sw_tick = g_ullTickCount;
    timer->ctrl_reg |= (1 << 3); /* left(3) */
    while((timer->ctrl_reg)&(1 << 3)){}; /* left(3) */
    uw_hw_cycle = timer->current_value;
    /*tick has come, but may interrupt environment, not counting the Tick interrupt response, to do +1 */
    if ((timer->int_status == 1))
    {
        timer->ctrl_reg |= (1 << 3);
        while((timer->ctrl_reg)&(1 << 3)){}; /* left(3) */
        uw_hw_cycle = timer->current_value;
        ull_sw_tick++;
    }
    uw_hw_cycle = uw_hw_cycle / EXT_XTAL_CLOCK_BASE * (OS_SYS_CLOCK / 1000000); /* Unit conversion(1000000) */

    ull_cycle = (((ull_sw_tick) * g_uwCyclesPerTick) + (g_uwCyclesPerTick - uw_hw_cycle));

    *puw_cnt_hi = ull_cycle >> 32; /* right(32) */
    *puw_cnt_low = ull_cycle & 0xFFFFFFFFU;

}

LITE_OS_SEC_TEXT_INIT_REDIRECT UINT32 os_timer_init(VOID)
{
    UINT32 result;
    UINT32 reload_value;
    timer_type* timer = (timer_type*)TIMER5_REG_BASE;
    if (0 == LOSCFG_BASE_CORE_TICK_PER_SECOND)/*lint !e506 !e774*/
    {
        return LOS_ERRNO_TICK_CFG_INVALID;
    }
    g_uwCyclesPerTick = OS_SYS_CLOCK / LOSCFG_BASE_CORE_TICK_PER_SECOND;
    reload_value = EXT_XTAL_CLOCK_BASE * 1000000 / LOSCFG_BASE_CORE_TICK_PER_SECOND;
    g_ullTickCount = 0;

    timer->ctrl_reg = 0x00000000;
    timer->load_count = reload_value;
    timer->ctrl_reg |= TIMER_MODE_CYCLE<<1;
    timer->ctrl_reg |= TIMER_INT_UNMASK<<2; /* left(2) */
    result = LOS_HwiCreate(Timer5_IRQn, 2, 0, (HWI_PROC_FUNC)SYMTAB_TICK_HANDLER, 0); /* priority(2) */
    g_int_enable_status |= 1<<Timer5_IRQn;
    if (result != LOS_OK)
    {
        los_printf("os_timer_init FAILED!\r\n");
        return result;
    }
    timer->ctrl_reg |= 0x00000001;
    return LOS_OK;
}


