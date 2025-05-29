/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Modify the drv directory, remove the first stage of the romization error,
 * delete the useless code to compile and adjust the dfx directory, remove the count
 * function driver part drv_dfx_str.c code
 * Author: CompanyName
 * Create: 2012-04-04
 */
#include "drv_dfx_str.h"
#include "drv_dfx_cmn_inc.h"
#include "soc_const_str.h"

EXT_CONST td_char g_str_ind_not_enough_memory[] = "mem exausted.\n";
EXT_CONST td_char g_str_ind_param_wrong[] = "para num wrong.\n";
EXT_CONST td_char g_str_ind_success[] = "success.\n";
EXT_CONST td_char g_str_ind_format_err_code[] = "cmd id %d errcode=%d\n";
