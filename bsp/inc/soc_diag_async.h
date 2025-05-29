/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: AsyncDiagCmd
 * Author: CompanyName
 * Create: 2012-10-31
 */
#ifndef __SOC_DIAG_ASYNC_H__
#define __SOC_DIAG_ASYNC_H__
#include <soc_mdm.h>
#include <soc_types.h>
#include <soc_mdm_types.h>
#if defined PRODUCT_CFG_NDM_COLLECT_DATA
td_u32 dbk_send_signal(EXT_IN td_pvoid pCmdParam, td_u16 usCmdParamSize);

typedef enum {
    SAL_QUEUE_ASYNC_DIAG_MSG_CMD,
    SAL_QUEUE_ASYNC_DIAG_MSG_MAX,
} sal_queue_async_diag_msg_id;


EXT_EXTERN td_u32 uapi_diag_async_cmd(td_u16 usId, EXT_IN td_pvoid pCmdParam, td_u16 usCmdParamSize, td_u32 ulOption,
    uapi_diag_async_cmd_f func);
EXT_EXTERN td_void sal_diag_async_task_init(td_void);

#endif
#endif
