/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Phase recognition requirements development raw_timer_pga.c code
 * Author: CompanyName
 * Create: 2012-04-08
 */
#include <drv_dfx.h>
#include <soc_mdm_diag.h>
#include <soc_mdm_task.h>
#include <soc_mdm_isr.h>
#include <soc_mdm_timer.h>

__hot td_void uapi_usr_timer_stop(td_void)
{
    unsigned int val;

    uapi_irq_disable(TIMER_IRQNR);

    val = timer_readl(TIMER_CONTROL_REG);
    val &= ~TIMER_ENABLE_BIT;
    timer_writrel(TIMER_CONTROL_REG, val);

    val = timer_readl(TIMER_CONTROL_REG);
    val |= TIMER_INTEN_BIT;
    timer_writrel(TIMER_CONTROL_REG, val);
}

td_void uapi_usr_timer_start(td_u32 time_out_us, lisr_proc_func routine)
{
    unsigned int val;
    td_u32 to_clk = 0;
    if (uint_2_multiply(time_out_us, TIMER_CLOCK_BASE, &to_clk) == TD_FALSE) {
        return;
    }

    uapi_irq_request(TIMER_IRQNR, 0, routine, 0);
    uapi_irq_enable(TIMER_IRQNR);

    /* ************** enable INT************************** */
    val = timer_readl(TIMER_CONTROL_REG);
    val &= ~TIMER_INTEN_BIT;
    timer_writrel(TIMER_CONTROL_REG, val);

    /* ************** disable TIMER*********************** */
    val = timer_readl(TIMER_CONTROL_REG);
    val &= ~TIMER_ENABLE_BIT;
    timer_writrel(TIMER_CONTROL_REG, val);

    /* ************** setting srart count******************** */
    timer_writrel(TIMER_LOADCOUNT_REG, to_clk);

    /* ************** setting running mode****************** */
    val = timer_readl(TIMER_CONTROL_REG);
    val |= TIMER_PERIOD_BIT;
    timer_writrel(TIMER_CONTROL_REG, val);

    /* ************** enable TIMER, start timing************************ */
    val = timer_readl(TIMER_CONTROL_REG);
    val |= TIMER_ENABLE_BIT;
    timer_writrel(TIMER_CONTROL_REG, val);
}

__hot td_void uapi_usr_timer_clear(td_void)
{
    timer_readl(TIMER_EOI_REG);
}
