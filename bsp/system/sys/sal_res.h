/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Implementation of sal module phase 1 init.
 * Author: CompanyName
 * Create: 2012-07-14
 */

#ifndef __SAL_RES_H__
#define __SAL_RES_H__

#include <soc_types.h>
#include <soc_mdm_types.h>

EXT_START_HEADER

typedef struct {
    td_u8 sys_mode;
    td_u8 rsv[3]; /* reserve 3 byte */
    ext_sys_status_info status_info;
} sal_ctx_stru;

sal_ctx_stru *get_sal_ctx(td_void);

EXT_END_HEADER
#endif /* __SAL_RES_H__ */
