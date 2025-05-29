/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: 1.Modify the cpu occupancy statistics code. cpup_diag_dfx.c code
 * Author: CompanyName
 * Create: 2012-02-21
 */
#include "cpup_diag_dfx.h"
#include <soc_types.h>
#include <soc_mdm_types.h>
#include <soc_mdm_mem.h>
#include <soc_stdlib.h>
#include <soc_mdm_diag.h>
#include <dfx_diag_cmd_id.h>
#include <soc_mdm_cpup.h>
#include <soc_mdm_task.h>
#include <dfx_cpup.h>

td_u32 g_cpup_report_cnt;

EXT_PRV td_u32 cpup_dfx_diag_cmd(td_u16 id, EXT_IN td_pvoid cmd_param, td_u16 cmd_param_size, td_u8 option)
{
    td_u32 ret = EXT_ERR_SUCCESS;
    td_char printline[100] = { 0 };
    ext_dfx_diag_cpup_cmd *cmd = cmd_param;
    ext_unref_param(cmd_param_size);
    if (cmd->option & 0x1) {
        uapi_cpup_reset_usage();
    }
    ext_cpup_item *cpup_items = uapi_malloc(EXT_MOD_ID_CPUP_DFX, (td_u32)(sizeof(ext_cpup_item) * g_cpup_report_cnt));
    if (cpup_items == TD_NULL) {
        if (snprintf_s(printline, sizeof(printline), sizeof(printline) - 1, "cmd id %d errcode=%d\n", id,
            EXT_ERR_NOT_ENOUGH_MEMORY) < 0) {
            goto END;
        }
        uapi_diag_report_packet(id, option, (td_pbyte)printline, (td_u16)strlen(printline), TD_TRUE);
        goto END;
    }
    if (uapi_cpup_get_usage(g_cpup_report_cnt, cpup_items) != EXT_ERR_SUCCESS) {
        if (snprintf_s(printline, sizeof(printline), sizeof(printline) - 1, "cmd id %d errcode=%d\n", id, ret) < 0) {
            goto END;
        }
        uapi_diag_report_packet(id, option, (td_pbyte)printline, (td_u16)strlen(printline), TD_TRUE);
        goto END;
    }
    for (td_u32 i = 0; i < g_cpup_report_cnt; i++) {
        ext_cpup_item *item = &cpup_items[i];
        ext_task_info_s info = { 0 };
        if (item->valid == 0) {
            break;
        }
        td_u32 low_time = (td_u32)item->cpu_time;
        td_u32 high_time = (td_u32)(item->cpu_time >> 32); // right(32)
        if (item->task) {
            ret = uapi_task_get_info(item->id, &info);
            if (ret == EXT_ERR_SUCCESS && snprintf_s(printline, sizeof(printline), sizeof(printline) - 1,
                "[%s][pe\rmillage=%d][cpu_time=0x%08x%08x]\n", info.name, item->permillage, high_time, low_time) >= 0) {
                uapi_diag_report_packet(id, option, (td_pbyte)printline, (td_u16)strlen(printline), TD_TRUE);
            }
        } else {
            snprintf_s(printline, sizeof(printline), sizeof(printline) - 1,
                "[isr%d][permillage=%d][cpu_time=0x%08x%08x]\n", item->id, item->permillage, high_time, low_time);
            uapi_diag_report_packet(id, option, (td_pbyte)printline, (td_u16)strlen(printline), TD_TRUE);
        }
    }
END:
    if (cpup_items != TD_NULL) {
        uapi_free(EXT_MOD_ID_CPUP_DFX, cpup_items);
    }
    return ret;
}

EXT_PRV EXT_CONST ext_diag_cmd_reg_obj g_cpu_dfx_tbl[] = {
    { ID_DIAG_CMD_DRV_OS_CPUP, ID_DIAG_CMD_DRV_OS_CPUP, cpup_dfx_diag_cmd },
};

__init td_u32 usr_cpup_dfx_init(td_u8 report_count)
{
    g_cpup_report_cnt = report_count;
    return uapi_diag_register_cmd(g_cpu_dfx_tbl, sizeof(g_cpu_dfx_tbl) / sizeof(g_cpu_dfx_tbl[0]));
}
