/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Upload SAR-ADC/PGA driver code. Pga_drv.c code
 * Author: CompanyName
 * Create: 2012-12-11
 */
#include "pga_drv.h"
#include <soc_stdlib.h>
#include <soc_mdm_task.h>

pga_ctl g_pga_ctl = {
    0,
};

td_u32 uapi_pga_init(void)
{
    reg_analog_wr44 pga_cfg_reg = {
        0,
    };
    reg_analog_wr40 pga_cfg_clk_gate = {
        0,
    };
    td_u32 ret = EXT_ERR_SUCCESS;

    uapi_task_pri_elevate();

    if (g_pga_ctl.is_init == TD_FALSE) {
        ret = uapi_sem_bcreate(&g_pga_ctl.pga_sem, "pga", EXT_SEM_ONE);
        if (ret == EXT_ERR_SUCCESS) {
            g_pga_ctl.is_init = TD_TRUE;
        } else {
            uapi_task_pri_resume();
            return ret;
        }
    }
    uapi_task_pri_resume();

    /* Configure initial value */
    pga_lock();
    REG_FORMAT_UAPI_U32(pga_cfg_reg) = com_bbp_reg_rd(REG_ANALOG_WR44);
    pga_cfg_reg.pd_pga0 = 0x1; /* powerdown */
    pga_cfg_reg.pd_pga1 = 0x1; /* powerdown */
    com_bbp_reg_wr(REG_ANALOG_WR44, &pga_cfg_reg);

    /* Other bits are configured to 0 */
    REG_FORMAT_UAPI_U32(pga_cfg_clk_gate) = com_bbp_reg_rd(REG_ANALOG_WR40);
    pga_cfg_clk_gate.clk_gate = 0x1;
    pga_cfg_clk_gate.buf_clk_sel = 0x0;
    pga_cfg_clk_gate.buf_clk_rst = 0x0;
    pga_cfg_clk_gate.buf_pd_clk = 0x0;
    pga_cfg_clk_gate.pga_clk_sel = 0x3;
    pga_cfg_clk_gate.pga_clk_rst = 0x0;
    pga_cfg_clk_gate.pga_pd_clk = 0x0;
    com_bbp_reg_wr(REG_ANALOG_WR40, &pga_cfg_clk_gate);

    pga_unlock();

    return ret;
}

td_u32 uapi_pga_amplify(EXT_CONST ext_pga_cfg *pga_cfg)
{
    reg_analog_wr44 pga_cfg_reg = {
        0,
    };

    if (pga_cfg == TD_NULL) {
        return EXT_ERR_FAILURE;
    }
    if ((td_u32)pga_cfg->pga_idx & EN_PGA_0) {
        if ((pga_cfg->pga0_gain == EN_PGA_GAIN_NA) || (pga_cfg->pga0_lp == EN_PGA_LP_NA)) {
            return EXT_ERR_FAILURE;
        }
    }
    if ((td_u32)pga_cfg->pga_idx & EN_PGA_1) {
        if ((pga_cfg->pga1_gain == EN_PGA_GAIN_NA) || (pga_cfg->pga1_lp == EN_PGA_LP_NA)) {
            return EXT_ERR_FAILURE;
        }
    }

    uapi_task_pri_elevate();
    if (g_pga_ctl.is_init == TD_FALSE) {
        uapi_task_pri_resume();
        return EXT_ERR_FAILURE;
    }
    uapi_task_pri_resume();

    pga_lock();
    REG_FORMAT_UAPI_U32(pga_cfg_reg) = com_bbp_reg_rd(REG_ANALOG_WR44);
    if ((td_u32)pga_cfg->pga_idx & EN_PGA_0) {
        pga_cfg_reg.pga0_gain = pga_cfg->pga0_gain;
        pga_cfg_reg.pga0_lp = pga_cfg->pga0_lp;
        pga_cfg_reg.pga_vcm_sel = pga_cfg->pga_vcm_sel;
        pga_cfg_reg.pd_pga0 = 0x0; /* Operating mode */
    }
    if ((td_u32)pga_cfg->pga_idx & EN_PGA_1) {
        pga_cfg_reg.pga1_gain = pga_cfg->pga1_gain;
        pga_cfg_reg.pga1_lp = pga_cfg->pga1_lp;
        pga_cfg_reg.pga_vcm_sel = pga_cfg->pga_vcm_sel;
        pga_cfg_reg.pd_pga1 = 0x0; /* Operating mode */
    }
    com_bbp_reg_wr(REG_ANALOG_WR44, &pga_cfg_reg);
    pga_unlock();

    return EXT_ERR_SUCCESS;
}

td_u32 uapi_pga_deinit(void)
{
    reg_analog_wr44 pga_cfg_reg = {
        0,
    };
    td_u32 ret = EXT_ERR_SUCCESS;

    uapi_task_pri_elevate();
    if (g_pga_ctl.is_init == TD_FALSE) {
        uapi_task_pri_resume();
        return EXT_ERR_FAILURE;
    }
    uapi_task_pri_resume();

    pga_lock();
    /* Restore initial value */
    REG_FORMAT_UAPI_U32(pga_cfg_reg) = com_bbp_reg_rd(REG_ANALOG_WR44);
    pga_cfg_reg.pd_pga0 = 0x1; /* powerdown */
    pga_cfg_reg.pd_pga1 = 0x1; /* powerdown */
    pga_cfg_reg.pga_vcm_sel = EN_PGA_VCM_SEL_0_4V;
    pga_cfg_reg.pga0_lp = EN_PGA_LP_NA;
    pga_cfg_reg.pga1_lp = EN_PGA_LP_NA;
    pga_cfg_reg.pga0_gain = EN_PGA_GAIN_25;
    pga_cfg_reg.pga1_gain = EN_PGA_GAIN_25;
    com_bbp_reg_wr(REG_ANALOG_WR44, &pga_cfg_reg);
    pga_unlock();

    if (g_pga_ctl.is_init == TD_TRUE) {
        ret = uapi_sem_delete(g_pga_ctl.pga_sem);
        if (ret == EXT_ERR_SUCCESS) {
            (td_void)memset_s((void *)&g_pga_ctl, sizeof(g_pga_ctl), 0x0, sizeof(g_pga_ctl));
        }
    }
    return ret;
}
