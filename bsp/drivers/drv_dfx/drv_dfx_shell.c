/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Modify the drv directory, remove the first stage of the romization error,
 * delete the useless code to compile and adjust the dfx directory, remove the count
 * function driver part drv_dfx_shell.c code
 * Author: CompanyName
 * Create: 2012-04-04
 */
#include <soc_mdm.h>
#include "drv_dfx_shell.h"

EXT_PRV td_u32 str_to_arg(td_char *str, td_u16 max_str_len, td_u32 *argc, td_char *argv[], td_u32 max_argc)
{
    td_u32 i;
    td_bool pree_blank = TD_TRUE;
    td_u32 argc_object = 0;

    if (max_str_len == 0) {
        *argc = 0;
        return EXT_ERR_SUCCESS;
    }
    str[max_str_len - 1] = '\0';

    for (i = 0; i < max_str_len; i++) {
        /* cmd end */
        if (str[i] == '\r' || str[i] == '\n' || str[i] == '\0') {
            str[i] = '\0';
            break;
        }

        if (str[i] == ' ' || str[i] == '\t') {
            str[i] = '\0';
            pree_blank = TD_TRUE;

            if (argc_object >= max_argc) { /* string end and arg max */
                break;
            }
        } else {
            /* string start */
            if (pree_blank && argc_object < max_argc) {
                argv[argc_object] = &(str[i]);
                argc_object++;
            }
            pree_blank = TD_FALSE;
        }
    }
    *argc = argc_object;

    return EXT_ERR_SUCCESS;
}

ext_shell_cmd_pre_do_handle *diag_shell_cmd_prepare(EXT_CONST td_pvoid cmd_param, td_u32 cmd_param_size,
    td_u32 out_line_size)
{
    td_char *out_line_buf = TD_NULL;
    ext_shell_cmd_pre_do_handle *handle = TD_NULL;
    td_pvoid req = TD_NULL;

    if ((cmd_param == NULL) || (cmd_param_size == 0)) {
        return TD_NULL;
    }
    handle = (ext_shell_cmd_pre_do_handle *)uapi_malloc(EXT_MOD_ID_DRV_DFX, sizeof(ext_shell_cmd_pre_do_handle));
    if (handle == TD_NULL) {
        return TD_NULL;
    }
    (td_void)memset_s((td_void *)handle, sizeof(ext_shell_cmd_pre_do_handle), 0, sizeof(ext_shell_cmd_pre_do_handle));

    if (out_line_size) {
        out_line_buf = (td_char *)uapi_malloc(EXT_MOD_ID_DRV_DFX, out_line_size);
        if (out_line_buf == TD_NULL) {
            uapi_free(EXT_MOD_ID_DRV_DFX, handle);
            return TD_NULL;
        }
        (td_void)memset_s((td_void *)out_line_buf, out_line_size, 0, out_line_size);
    }

    if (cmd_param_size) {
        req = (td_pvoid)uapi_malloc(EXT_MOD_ID_DRV_DFX, cmd_param_size);
        if (req == TD_NULL) {
            uapi_free(EXT_MOD_ID_DRV_DFX, out_line_buf);
            uapi_free(EXT_MOD_ID_DRV_DFX, handle);
            return TD_NULL;
        }
        (td_void)memcpy_s(req, cmd_param_size, cmd_param, cmd_param_size);
    }

    handle->out_line_buf = out_line_buf;
    handle->req = (td_char *)req;
    if (EXT_ERR_SUCCESS !=
        str_to_arg((td_char *)req, (td_u16)cmd_param_size, &(handle->argc), handle->argv, SHELL_CMD_ARGC_MAX_COUNT)) {
        uapi_free(EXT_MOD_ID_DRV_DFX, req);
        uapi_free(EXT_MOD_ID_DRV_DFX, out_line_buf);
        uapi_free(EXT_MOD_ID_DRV_DFX, handle);
        return TD_NULL;
    }

    return handle;
}

td_void diag_shell_cmd_restore(ext_shell_cmd_pre_do_handle *handle)
{
    uapi_free(EXT_MOD_ID_DRV_DFX, handle->out_line_buf);
    uapi_free(EXT_MOD_ID_DRV_DFX, handle->req);
    uapi_free(EXT_MOD_ID_DRV_DFX, handle);
    return;
}
