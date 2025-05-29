/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Modify the drv directory, remove the first stage of the romization error,
 * delete the useless code to compile and adjust the dfx directory, remove the count
 * function driver part drv_dfx_shell.h code
 * Author: CompanyName
 * Create: 2012-04-04
 */

#ifndef __DRV_DFX_SHELL_H__
#define __DRV_DFX_SHELL_H__
#include <soc_mdm_types.h>
#define SHELL_CMD_ARGC_MAX_COUNT 10
typedef struct {
    td_u32 argc;                             /* argv num */
    td_char *argv[SHELL_CMD_ARGC_MAX_COUNT]; /* argv list */
    td_char *req;                            /* buf point :storage req */
    td_char *out_line_buf;                   /* buf point :storage ind */
} ext_shell_cmd_pre_do_handle;

/* Functional Description: Pre-apply resources and convert long string parameters into multiple parameters */
ext_shell_cmd_pre_do_handle *diag_shell_cmd_prepare(EXT_CONST td_pvoid cmd_param, td_u32 cmd_param_size,
    td_u32 out_line_size);

td_void diag_shell_cmd_restore(ext_shell_cmd_pre_do_handle *handle);

#endif
