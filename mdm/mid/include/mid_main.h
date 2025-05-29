/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Middle layer - main.
 * Author: CompanyName
 * Create: 2018-10-31
 */

#ifndef __MID_MAIN_H__
#define __MID_MAIN_H__

#include <soc_mdm.h>
#include <soc_mid.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MID_QUEUE_NUM 32
#define MID_TASK_STASK_SIZE 1024
#define MID_TASK_PRIORITY 21

typedef enum {
    MID_APP_MSG_ID_NONE,
    MID_APP_MSG_ID_DIAG,
    MID_APP_MSG_ID_PLC,
} mid_app_msg_id;

typedef struct {
    td_u32 task_id;
    td_u32 queue_id;
} mid_app_ctx;

mid_app_ctx *mid_get_ctx(td_void);

#ifdef __cplusplus
}
#endif
#endif /* __MID_MAIN_H__ */
