/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Restore OSM/OSSMdrv_dfx_mem.c code
 * Author: CompanyName
 * Create: 2012-06-27
 */
#include "drv_dfx_mem.h"
#include "drv_dfx_cmn_inc.h"
#include "los_task.h"
#include "soc_mdm_hrtimer.h"
#include "soc_stdlib.h"

#ifdef PRODUCT_CFG_MEM_OPERATE
#define MEM_READ_MAX_NUM 0x32

EXT_PRV td_u32 mem_read_make_ind(td_u32 address, td_u32 word_num, td_char *report_buf, td_u32 reoprt_buf_len)
{
    td_char temp_str[20] = { 0 }; /* Temp String Length 20 */
    td_u32 line_len;
    td_u32 val;
    td_u32 i;
    if ((report_buf == NULL) || (reoprt_buf_len == 0)) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    while (word_num > 0) {
        int ret = sprintf_s(temp_str, sizeof(temp_str), "0x%08x  ", address);
        if (ret == -1) {
            return EXT_ERR_MEMCPY_FAIL;
        }
        if (strcat_s(report_buf, reoprt_buf_len, temp_str) != EOK) {
            return EXT_ERR_MEMCPY_FAIL;
        }

        line_len = (word_num >= 4) ? 4 : word_num; /* Length of Line Less Than 4 */
        for (i = 0; i < line_len; i++) {
            if (strcat_s(report_buf, reoprt_buf_len, "0x") != EOK) {
                return EXT_ERR_MEMCPY_FAIL;
            }

            val = *(td_u32 *)(uintptr_t)address;

            ret = sprintf_s(temp_str, sizeof(temp_str), "%08x  ", val);
            if (ret == -1) {
                return EXT_ERR_MEMCPY_FAIL;
            }

            if (strcat_s(report_buf, reoprt_buf_len, temp_str) != EOK) {
                return EXT_ERR_MEMCPY_FAIL;
            }

            address += 0x4;
        }
        word_num -= line_len;
        if (strcat_s(report_buf, reoprt_buf_len, "\r\n") != EOK) {
            return EXT_ERR_MEMCPY_FAIL;
        }
    }
    return EXT_ERR_SUCCESS;
}

td_u32 mem_read(td_u16 id, EXT_IN td_pvoid cmd_param, td_u16 cmd_param_size, td_u8 option)
{
    td_u32 address;
    td_u32 word_num;
    ext_shell_cmd_pre_do_handle *handle = TD_NULL;
    td_u32 argc;
    td_char **argv;
    td_char *report_buf = TD_NULL;
    const td_u32 reoprt_buf_len = 1024;

    if ((cmd_param == NULL) || (cmd_param_size == 0)) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    handle = diag_shell_cmd_prepare(cmd_param, cmd_param_size, reoprt_buf_len);
    if (handle == TD_NULL) {
        uapi_diag_report_packet(id, (td_u8)option, (td_pbyte)g_str_ind_not_enough_memory,
            (td_u16)strlen(g_str_ind_not_enough_memory) + 1, TD_TRUE);
        return EXT_ERR_SUCCESS;
    }
    argc = handle->argc;
    argv = handle->argv;
    report_buf = handle->out_line_buf;

    if (argc != 2) {
        if (sprintf_s(report_buf, reoprt_buf_len, "%s", g_str_ind_param_wrong) == -1) {
            goto END;
        }
        goto END;
    }

    address = strtoul(argv[0], (char **)NULL, 16);
    word_num = strtoul(argv[1], (char **)NULL, 16);

    if (word_num > MEM_READ_MAX_NUM || word_num == 0 || 0 != (address & 0x3)) {
        errno_t ret = sprintf_s(report_buf, reoprt_buf_len,
            "len can't >0x32,len can't = 0;len can't = 0;addr should 4 bytes align\n");
        if (ret == -1) {
            goto END;
        }
        goto END;
    }

    errno_t ret = memcpy_s(report_buf, reoprt_buf_len, "\r\n", sizeof("\r\n"));
    if (ret != EOK) {
        goto END;
    }
    mem_read_make_ind(address, word_num, report_buf, reoprt_buf_len);

END:
    report_buf[reoprt_buf_len - 1] = 0;
    uapi_diag_report_packet(id, (td_u8)option, (td_pbyte)report_buf, (td_u16)strlen(report_buf) + 1, TD_TRUE);
    diag_shell_cmd_restore(handle);

    return EXT_ERR_SUCCESS;
}

td_u32 mem_write(td_u16 id, EXT_IN td_pvoid cmd_param, td_u16 cmd_param_size, td_u8 option)
{
    uintptr_t address;
    td_u32 VALUE;

    ext_shell_cmd_pre_do_handle *handle = TD_NULL;
    td_u32 argc;
    td_char **argv;

    if ((cmd_param == NULL) || (cmd_param_size == 0)) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    handle = diag_shell_cmd_prepare(cmd_param, cmd_param_size, 0);
    if (handle == TD_NULL) {
        uapi_diag_report_packet(id, (td_u8)option, (td_pbyte)g_str_ind_not_enough_memory,
            (td_u16)(strlen(g_str_ind_not_enough_memory) + 1), TD_TRUE);
        return EXT_ERR_SUCCESS;
    }
    argc = handle->argc;
    argv = handle->argv;

    if (argc != 2) {
        uapi_diag_report_packet(id, (td_u8)option, (td_pbyte)g_str_ind_param_wrong,
            (td_u16)(strlen(g_str_ind_param_wrong) + 1), TD_TRUE);
        return EXT_ERR_SUCCESS;
    }

    address = strtoul(argv[0], (char **)NULL, 16);
    VALUE = strtoul(argv[1], (char **)NULL, 16);

    if (0 != (address & 0x3)) {
        uapi_diag_report_packet(id, (td_u8)option, (td_pbyte) "inval addr.\n", (td_u16)(strlen("inval addr.\n") + 1),
            TD_TRUE);
        diag_shell_cmd_restore(handle);
        return EXT_ERR_SUCCESS;
    }

    *(volatile td_u32 *)(uintptr_t)(address) = (td_u32)VALUE;

    uapi_diag_report_packet(id, (td_u8)option, (td_pbyte)g_str_ind_success, (td_u16)(strlen(g_str_ind_success) + 1),
        TD_TRUE);
    diag_shell_cmd_restore(handle);
    return EXT_ERR_SUCCESS;
}
#endif
