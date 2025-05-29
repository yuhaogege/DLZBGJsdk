
#ifndef DEMO_PWM_H
#define DEMO_PWM_H

#include "soc_mdm_io.h"
#include "soc_mdm_pwm.h"
#include "stdio.h"
void demo_pwm_init(void);
void demo_pwm_output(td_u32 index, td_u32 pwmduty);
#endif

