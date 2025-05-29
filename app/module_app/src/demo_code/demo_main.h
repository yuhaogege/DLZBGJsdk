
#ifndef DEMO_MAIN_H
#define DEMO_MAIN_H

#include "soc_types.h"
#include "soc_errno.h"
#include "demo_gpio.h"
#include "demo_pwm.h"
#include "demo_uart.h"
#include "demo_plc.h"
#include "soc_mdm_task.h"
#include "stdio.h"

#define MICRO_PERI_MODULE_SLEEP_DURTION_MS   (1000)
#define MICRO_PERI_SRV_TASK_STACK_SIZE       (8*1024)
#define MICRO_PERI_SRV_TASK_PRIORITY         (25)

void demo_app_main(void);

#endif

