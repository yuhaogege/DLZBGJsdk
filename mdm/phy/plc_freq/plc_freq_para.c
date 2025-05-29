/*
 * Copyright (c) CompanyNameMagicTag 2017-2020. All rights reserved.
 * Description: Supported frequency parameters
 */

#include "phy_freq_cfg.h"
#include "soc_types_rom.h"

td_u32 g_plc_freq_param[FREQ_CFG_NUM * ARRAY_LEN] = PHY_FREQ_CFG_ST;
td_u8 g_plc_freq_cfg_num = FREQ_CFG_NUM;

/* frequency parameters by default */
td_u32 g_default_freq_param[ARRAY_LEN] = {
    0x322e3306, 0x6564204d, 0x6c756166, 0x74, 0x4d352e30, 0x372e332d, 0x47535f4d, 0x0, 0x60, 0x839715, 0x0,
    0xc00, 0x0, 0x235, 0x235, 0x4, 0x3f0406, 0x56363515, 0x97787757, 0x401, 0x30405, 0x8080303, 0x3030a0a,
    0xa0a0808, 0x640014, 0x8, 0x12c, 0x20301, 0x3fac, 0x28, 0xffffffff, 0x0
};

__hot td_u32 *plc_get_freq_cfg_param(td_void)
{
    return (g_plc_freq_cfg_num == 0) ? g_default_freq_param : g_plc_freq_param;
}

__hot td_u8 plc_get_freq_cfg_num(td_void)
{
    return (g_plc_freq_cfg_num == 0) ? 0x01 : FREQ_CFG_NUM;
}
