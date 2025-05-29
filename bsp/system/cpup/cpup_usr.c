/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: 1.Modify the cpu occupancy statistics code. cpup_usr.c code
 * Author: CompanyName
 * Create: 2012-02-21
 */
#include "cpup_usr.h"
#include <soc_types.h>
#include <soc_mdm_types.h>
#include <los_cpup.ph>
#include "cpup_inner.h"

/*
 * CPU usage, statistics DIAG command supports the maximum number of reported tasks and interrupts,
 * When the value is less than the number of active tasks + interrupts in the activity,
 * it will result in incomplete statistics.
 */
#define EXT_CPUP_DIAG_DFX_REPORT_COUNT 20

__init td_u32 usr_cpup_init(td_void)
{
    osCpupInit(TD_NULL); /* Initialize CPU usage statistics */
    /* Initialize the CPU usage statistics DIAG test command, do not use the DIAG diagnostic tool scenario to delete */
    usr_cpup_dfx_init(EXT_CPUP_DIAG_DFX_REPORT_COUNT);
    return EXT_ERR_SUCCESS;
}
