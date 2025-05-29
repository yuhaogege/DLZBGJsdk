/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: [New requirements for platform group] Added pwm secondary development interface pwm_drv.c code
 * Author: CompanyName
 * Create: 2012-12-18
 */
#ifndef PWM_DRV_H
#define PWM_DRV_H

#include "pwm_drv.h"
#include <soc_ft_nv.h>
#include <soc_mdm_ver.h>
#include <soc_mdm_isr.h>
#include <soc_mdm_sem.h>
#include <soc_stdlib.h>
#include <dw21_platform_rom.h>

EXT_CPP_START

pwm_ctx g_pwm_ctx = { 0 };

td_u32 uapi_pwm_init(ext_pwm_port port)
{
    td_u32 ret;
    pwm_ctl *pwm_ctrl = TD_NULL;

    if (pwm_check_port(port) != EXT_ERR_SUCCESS) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    pwm_ctrl = pwm_get_ctl(port);
    if (pwm_ctrl->is_init == TD_FALSE) {
        if (port == EXT_PWM_PORT_PWM0) {
            ret = uapi_sem_bcreate(&(pwm_ctrl->pwm_sem), "pwm0", EXT_SEM_ONE);
        } else {
            ret = uapi_sem_bcreate(&(pwm_ctrl->pwm_sem), "pwm1", EXT_SEM_ONE);
        }
        if (ret == EXT_ERR_SUCCESS) {
            pwm_ctrl->is_init = TD_TRUE;
        }
    } else {
        ret = EXT_ERR_INITILIZATION_ALREADY;
    }
    return ret;
}

td_u32 uapi_pwm_deinit(ext_pwm_port port)
{
    td_u32 ret;
    pwm_ctl *pwm_ctl_point = TD_NULL;

    if (pwm_check_port(port) != EXT_ERR_SUCCESS) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    pwm_ctl_point = pwm_get_ctl(port);
    if (pwm_ctl_point->is_init == TD_TRUE) {
        ret = uapi_sem_delete(pwm_ctl_point->pwm_sem);
        if (ret == EXT_ERR_SUCCESS) {
            (td_void)memset_s((void *)pwm_ctl_point, sizeof(pwm_ctl), 0, sizeof(pwm_ctl));
        }
    } else {
        ret = EXT_ERR_NO_OS_RES_INIT;
    }
    return ret;
}

__isr td_u32 uapi_pwm_set_clock(ext_pwm_clk_source clk_type)
{
    td_u32 int_val;
    td_u32 reg_val;

    if (clk_type >= PWM_CLK_MAX) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    int_val = uapi_int_lock();
    reg_val = UAPI_REG_READ_VAL32(DW21_CRG_REG_SC_PERI_CLKSEL_REG);
    if (clk_type == PWM_CLK_APB) {
        reg_val &= (~(1 << CRG_REG_PWM_CLK_SEL_OFFSET));
        UAPI_REG_WRITE32(DW21_CRG_REG_SC_PERI_CLKSEL_REG, reg_val);
    } else {
        reg_val |= (1 << CRG_REG_PWM_CLK_SEL_OFFSET);
        UAPI_REG_WRITE32(DW21_CRG_REG_SC_PERI_CLKSEL_REG, reg_val);
    }
    uapi_int_restore(int_val);
    return EXT_ERR_SUCCESS;
}

td_u32 uapi_pwm_start(ext_pwm_port port, td_u16 duty, td_u16 freq)
{
    td_u32 ret;

    if ((pwm_check_port(port) != EXT_ERR_SUCCESS) || (duty == 0) || (freq == 0)) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    if (pwm_is_init(port) == TD_FALSE) {
        return EXT_ERR_NO_OS_RES_INIT;
    }
    ret = pwm_lock(port);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }
    pwm_set_enable(port, TD_TRUE);
    pwm_set_freq(port, freq);
    pwm_set_duty(port, duty);
    pwm_take_effect(port);
    return pwm_unlock(port);
}

td_u32 uapi_pwm_stop(ext_pwm_port port)
{
    td_u32 ret;

    if (pwm_check_port(port) != EXT_ERR_SUCCESS) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    if (pwm_is_init(port) == TD_FALSE) {
        return EXT_ERR_NO_OS_RES_INIT;
    }

    ret = pwm_lock(port);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }
    pwm_set_enable(port, TD_FALSE);
    pwm_take_effect(port);
    return pwm_unlock(port);
}

/* *********************** The following is an internal function *************************************** */
td_u32 pwm_check_port(ext_pwm_port port)
{
    ext_ftm_chip_type chip_type = uapi_get_hw_chip_type();
    /* E chip only supports PWM1 module */
    if ((port >= EXT_PWM_PORT_MAX) || ((chip_type == EXT_FTM_CHIP_TYPE_21EV) && (port <= EXT_PWM_PORT_PWM0))) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    return EXT_ERR_SUCCESS;
}

td_void pwm_set_enable(ext_pwm_port port, td_bool flag)
{
    td_u32 reg_val;

    /* Enable */
    if (flag == TD_TRUE) {
        if (port == EXT_PWM_PORT_PWM0) {
            reg_val = UAPI_REG_READ_VAL32(DW21_PWM_EN_REG);
            reg_val |= (1 << PWM_REG_PWM_EN_OFFSET);
            UAPI_REG_WRITE32(DW21_PWM_EN_REG, reg_val);
        } else if (port == EXT_PWM_PORT_PWM1) {
            reg_val = UAPI_REG_READ_VAL32(DW21_PWM1_EN_REG);
            reg_val |= (1 << PWM1_REG_PWM_EN_OFFSET);
            UAPI_REG_WRITE32(DW21_PWM1_EN_REG, reg_val);
        }
        return;
    }

    /* Disenable */
    if (port == EXT_PWM_PORT_PWM0) {
        reg_val = UAPI_REG_READ_VAL32(DW21_PWM_EN_REG);
        reg_val &= (~(1 << PWM_REG_PWM_EN_OFFSET));
        UAPI_REG_WRITE32(DW21_PWM_EN_REG, reg_val);
    } else if (port == EXT_PWM_PORT_PWM1) {
        reg_val = UAPI_REG_READ_VAL32(DW21_PWM1_EN_REG);
        reg_val &= (~(1 << PWM1_REG_PWM_EN_OFFSET));
        UAPI_REG_WRITE32(DW21_PWM1_EN_REG, reg_val);
    }
}

td_void pwm_set_freq(ext_pwm_port port, td_u16 freq)
{
    td_u32 reg_val;

    if (port == EXT_PWM_PORT_PWM0) {
        reg_val = UAPI_REG_READ_VAL32(DW21_PWM_FREQ_REG);
        reg_val &= (~PWM_FREQ_MASK);
        reg_val |= (td_u32)freq;
        UAPI_REG_WRITE32(DW21_PWM_FREQ_REG, reg_val);
    } else {
        reg_val = UAPI_REG_READ_VAL32(DW21_PWM1_FREQ_REG);
        reg_val &= (~PWM_FREQ_MASK);
        reg_val |= (td_u32)freq;
        UAPI_REG_WRITE32(DW21_PWM1_FREQ_REG, reg_val);
    }
}

td_void pwm_set_duty(ext_pwm_port port, td_u16 duty)
{
    td_u32 reg_val;

    if (port == EXT_PWM_PORT_PWM0) {
        reg_val = UAPI_REG_READ_VAL32(DW21_PWM_DUTY_REG);
        reg_val &= (~PWM_DUTY_MASK);
        reg_val |= (td_u32)duty;
        UAPI_REG_WRITE32(DW21_PWM_DUTY_REG, reg_val);
    } else {
        reg_val = UAPI_REG_READ_VAL32(DW21_PWM1_DUTY_REG);
        reg_val &= (~PWM_DUTY_MASK);
        reg_val |= (td_u32)duty;
        UAPI_REG_WRITE32(DW21_PWM1_DUTY_REG, reg_val);
    }
}

td_void pwm_take_effect(ext_pwm_port port)
{
    td_u32 reg_val;

    if (port == EXT_PWM_PORT_PWM0) {
        reg_val = UAPI_REG_READ_VAL32(DW21_PWM_START_REG);
        reg_val |= (1 << PWM_REG_PWM_START_OFFSET);
        UAPI_REG_WRITE32(DW21_PWM_START_REG, reg_val);
    } else {
        reg_val = UAPI_REG_READ_VAL32(DW21_PWM1_START_REG);
        reg_val |= (1 << PWM1_REG_PWM_START_OFFSET);
        UAPI_REG_WRITE32(DW21_PWM1_START_REG, reg_val);
    }
}

td_bool pwm_is_init(ext_pwm_port port)
{
    return g_pwm_ctx.pwm[port].is_init;
}

td_u32 pwm_lock(ext_pwm_port port)
{
    pwm_ctl *pwm_ctrl = pwm_get_ctl(port);
    return uapi_sem_wait(pwm_ctrl->pwm_sem, EXT_SYS_WAIT_FOREVER);
}

td_u32 pwm_unlock(ext_pwm_port port)
{
    pwm_ctl *pwm_ctrl = pwm_get_ctl(port);
    return uapi_sem_signal(pwm_ctrl->pwm_sem);
}

pwm_ctl *pwm_get_ctl(ext_pwm_port port)
{
    return &(g_pwm_ctx.pwm[port]);
}

EXT_CPP_END

#endif
