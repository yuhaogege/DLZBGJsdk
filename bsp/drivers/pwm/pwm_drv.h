/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: [New requirements for platform group] Add pwm secondary development interface pwm_drv.h code
 * Author: CompanyName
 * Create: 2012-12-18
 */

#ifndef __PWM_DRV_H__
#define __PWM_DRV_H__

#include <soc_mdm_pwm.h>

EXT_CPP_START

#define PWM_DUTY_MASK 0x0000FFFF
#define PWM_FREQ_MASK 0x0000FFFF

#define PWM_REG_PWM_EN_OFFSET 0
#define PWM_REG_PWM_START_OFFSET 0

#define PWM1_REG_PWM_EN_OFFSET 0
#define PWM1_REG_PWM_START_OFFSET 0

#define DW21_PWM_EN_REG (EXT_PWM_REG_BASE + 0x0)
#define DW21_PWM_START_REG (EXT_PWM_REG_BASE + 0x4)
#define DW21_PWM_FREQ_REG (EXT_PWM_REG_BASE + 0x8)
#define DW21_PWM_DUTY_REG (EXT_PWM_REG_BASE + 0xC)

#define DW21_PWM1_EN_REG (EXT_PWM1_REG_BASE + 0x0)
#define DW21_PWM1_START_REG (EXT_PWM1_REG_BASE + 0x4)
#define DW21_PWM1_FREQ_REG (EXT_PWM1_REG_BASE + 0x8)
#define DW21_PWM1_DUTY_REG (EXT_PWM1_REG_BASE + 0xC)

typedef struct {
    td_bool is_init;
    td_u8 reserved[3]; /* keep 3 bytes for alignment */
    td_u32 pwm_sem;
} pwm_ctl;

typedef struct {
    pwm_ctl pwm[EXT_PWM_PORT_MAX];
} pwm_ctx;

static td_u32 pwm_check_port(ext_pwm_port port);
static td_void pwm_set_enable(ext_pwm_port port, td_bool flag);
static td_void pwm_set_freq(ext_pwm_port port, td_u16 freq);
static td_void pwm_set_duty(ext_pwm_port port, td_u16 duty);
static td_void pwm_take_effect(ext_pwm_port port);
static td_bool pwm_is_init(ext_pwm_port port);
static td_u32 pwm_lock(ext_pwm_port port);
static td_u32 pwm_unlock(ext_pwm_port port);
static pwm_ctl *pwm_get_ctl(ext_pwm_port port);

EXT_CPP_END
#endif
