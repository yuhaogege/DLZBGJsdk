/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: APP main entry.
 */

#ifndef APP_MAIN_H
#define APP_MAIN_H

#include "app_res.h"
#include "soc_types.h"
#include "soc_mdm_timer.h"

#ifdef __cplusplus
extern "C" {
#endif

#define APP_DATA_BUF_SIZE           512
#define APP_QUEUE_SIZE              16
#define APP_UART_TASK_STACK_SIZE    1024
#define APP_UART_TASK_PRIORITY      25
#define APP_SRV_TASK_STACK_SIZE     1024
#define APP_SRV_TASK_PRIORITY       25
#define APP_MODULE_SLEEP_DURTION_MS 1000
#define APP_LED_STEP                16
#define APP_LED_STEP1               4
#define APP_LED_STEP2               8
#define APP_LED_STEP3               12
#define APP_TL_QUEUE_SIZE           10

typedef struct {
    td_u16 rx_length;
    td_u16 reserved;
    td_u8 buffer[APP_DATA_BUF_SIZE];
} app_data_buf;

typedef struct {
    td_u32 queue_id;
    td_u32 sem_id[APP_SEM_ID_MAX];
    ext_timer_handle_s timer_handle[APP_TIMER_ID_MAX];
} app_res_cfg;

typedef struct {
    app_res_cfg res;
    app_data_buf buffer;
} app_srv_context;

app_srv_context *app_get_srv_context(td_void);

#ifdef __cplusplus
}
#endif
#endif

