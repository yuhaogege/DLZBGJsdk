/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Modify the drv directory, remove the first stage of the romization error,
 * delete the useless code to compile and adjust the dfx directory, remove the count
 * function driver part drv_dfx_sal.c code
 * Author: CompanyName
 * Create: 2012-04-04
 */
#include <soc_mdm.h>
#include "drv_dfx_cmn_inc.h"
#include "drv_dfx_sal.h"
td_u32 dfx_system_rst(td_u16 id, EXT_IN td_pvoid cmd_param, td_u16 cmd_param_size, td_u8 option)
{
    ext_unref_param(cmd_param);
    ext_unref_param(cmd_param_size);

    uapi_diag_send_ack_packet(id, option, "RESET NOW", (td_u16)strlen("RESET NOW"));
    uapi_sleep(500);

    uapi_sys_reboot(EXT_SYS_REBOOT_CAUSE_CMD);
    return EXT_ERR_SUCCESS;
}
