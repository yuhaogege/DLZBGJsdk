/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: LowPowerStat
 * Author: CompanyName
 * Create: 2012-10-31
 */
#ifndef __DFX_LOW_POWER_H__
#define __DFX_LOW_POWER_H__
#include <soc_types.h>
#include <soc_mdm_types.h>
#include <soc_lowpower.h>

typedef struct {
    td_u32 int2wakeup; /* Interrupts that allow wakeups */
    td_u32 int2backup; /* interrupt configuration backup */

    td_u32 light : 1;  /* Enables light sleep */
    td_u32 deep : 1;   /* Enable deep sleep */
    td_u32 inited : 1; /* Initialization flag */
    td_u32 ver : 13;
    td_u32 delay_s : 16; /* Low power startup wait time, s is the unit */
    td_u32 timestamp;    /* timestamp */

    td_u32 total_count_forbid; /* Total number of sleeps rejected, only reference */
    td_u16 count_light;        /* The total number of light sleeps, only reference */
    td_u16 count_deep;         /* The total number of deep sleeps, only reference */

    td_u32 vote_light; /* Light sleep voting, 1 bit per id */
    td_u32 vote_deep;  /* Deep sleep voting, 1 bit per id */
    /* Each id counts the number of refusals to sleep, together with the depth of the statistics */
    td_u16 count_forbid[EXT_LOWPOWER_ID_LIMIT];
    /* Each id wakes up statistics, together with the depth of the statistics */
    td_u16 count_wakeup[EXT_LOWPOWER_ID_LIMIT];

    td_u32 func[EXT_LOWPOWER_CHECK_FUNC_LIMIT]; /* Monitors whether the array of sleep function pointers is allowed */
    td_u32 para[EXT_LOWPOWER_CHECK_FUNC_LIMIT]; /* Monitors whether the sleep function parameter array is allowed */

    td_u8 check_id[EXT_LOWPOWER_CHECK_FUNC_LIMIT]; /* Monitors whether sleep id arrays are allowed */

    td_u32 under_tick_threshold;   /* Below time threshold count statistics */
    td_u16 under_handle_threshold; /* Below the processing threshold count */
    td_u8 unforbid_delay_id_light; /* Undo the no-sleep delay id */
    td_u8 unforbid_delay_id_deep;  /* Undo the no-sleep delay id */

    td_u32 unforbid_delay_tick_light[2]; /* Undo the sleep delay tick, 2 count */
    td_u32 unforbid_delay_tick_deep[2];  /* Undo the sleep delay tick, 2 count */
    td_u32 time_lightsleep[2];           /* Total time of light sleep, 2 count */
    td_u32 time_deepsleep[2];            /* Total time of deep sleep, 2 count */
    td_u32 time_wakeup[2];               /* Total time to wake up, 2 count */
    td_u32 current_tick[2];              /* 2 count */
} ext_lowpower_status_cmd;

td_u32 dfx_get_lowpower_stat(td_u16 id, EXT_IN td_pvoid cmd_param, td_u16 cmd_param_size, td_u8 option);

#endif
