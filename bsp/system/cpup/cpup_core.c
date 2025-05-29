/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: 1.Modify the cpu occupancy statistics code. cpup_core.c code
 * Author: CompanyName
 * Create: 2012-02-21
 */
#include "cpup_core.h"
#include <soc_types.h>
#include <soc_mdm_types.h>
#include <soc_mdm_isr.h>
#include <soc_stdlib.h>
#include <los_hwi.h>
#include <los_task.ph>
#include <los_cpup.ph>
#include <soc_mdm_cpup.h>

EXT_EXTERN td_void osTskCycleStart(td_void);
EXT_EXTERN td_void osTskCycleEnd(td_void);
EXT_EXTERN td_u64 osGetCpuCycle(td_void);

EXT_EXTERN td_u16 g_usCpupInitFlg;

__isr td_u32 uapi_cpup_get_usage(td_u32 array_count, ext_cpup_item *cpup_items)
{
    td_u32 int_value;
    td_u64 total_time = 0;
    td_u32 i, j;

    if (g_usCpupInitFlg == 0) {
        return EXT_ERR_CPUP_NOT_INIT;
    }

    if (array_count == 0 || cpup_items == TD_NULL) {
        return EXT_ERR_CPUP_INVALID_PARAM;
    }

    memset_s(cpup_items, (td_u32)(array_count * sizeof(ext_cpup_item)), 0,
        (td_u32)(array_count * sizeof(ext_cpup_item)));

    int_value = uapi_int_lock();
    osTskCycleEnd();

    for (i = 0, j = 0; i < (g_uwTskMaxNum + OS_HWI_MAX_NUM + 1); i++) {
        if (g_pstCpup[i].ullAllTime == 0) {
            /* Already release task lose time, calculating total time may be not accurate */
            continue;
        }

        total_time += g_pstCpup[i].ullAllTime;

        if (j < array_count) {
            ext_cpup_item *item = &cpup_items[j];
            if (i < g_uwTskMaxNum) {
                item->task = TD_TRUE;
                item->id = (td_u8)i;
            } else if (i == g_uwTskMaxNum) {
                item->task = TD_FALSE;
                item->id = 0xFF;
            } else {
                item->task = TD_FALSE;
                item->id = (td_u8)(i - g_uwTskMaxNum - 1);
            }
            item->valid = TD_TRUE;
            item->cpu_time = g_pstCpup[i].ullAllTime;
        }
        j++;
    }

    for (j = 0; j < array_count; j++) {
        ext_cpup_item *item = &cpup_items[j];
        if (item->valid && total_time != 0) {
            item->permillage = (td_u32)((LOS_CPUP_PRECISION * item->cpu_time) / total_time);
        }
    }

    osTskCycleStart();
    uapi_int_restore(int_value);

    return EXT_ERR_SUCCESS;
}

/* ****************************************************************************
Function   : LOS_CpupReset
Description: reset data of CPU usage
Input      : None
Return     : None
**************************************************************************** */
__isr td_void uapi_cpup_reset_usage(td_void)
{
    td_u32 max_num = g_uwTskMaxNum;

    if (g_pstCpup == TD_NULL) {
        return;
    }

    max_num += OS_HWI_MAX_NUM + 1;
    for (td_u32 i = 0; i < max_num; i++) {
        g_pstCpup[i].ullStartTime = osGetCpuCycle();
        g_pstCpup[i].ullAllTime = 0;
    }
}
