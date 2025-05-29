/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Implements the active reset function and the delayed reset function.
 * Author: CompanyName
 * Create: 2012-05-17
 */

#include "sal_monitor.h"
#include <soc_mdm_watchdog.h>
#include <soc_mdm_time.h>
#include <soc_sal.h>
#include "sal_reset.h"

EXT_CPP_START

td_void sal_monitor(td_void)
{
    static td_u32 last_sec = 0;
    td_u32 cur_sec = uapi_get_seconds();
    /* Put in the IDLE task, should be called every second. */
    if (cur_sec - last_sec >= EXT_MONITOR_INTERVAL_SEC) {
        uapi_watchdog_feed();
        sal_reset_monitor(cur_sec);
        last_sec = cur_sec;
    }
}

EXT_CPP_END
