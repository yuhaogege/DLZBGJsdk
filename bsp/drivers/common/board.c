/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Adjust directory structure, Prepare to compile sdk, board.c code
 * Author: CompanyName
 * Create: 2012-07-14
 */

#include "board.h"
#include <soc_mdm.h>

EXT_CPP_START

EXT_PRV ext_ftm_product_type g_product_type = EXT_FTM_PRODUCT_TYPE_UNKOWN;
EXT_PRV td_u8 g_local_mac[EXT_PLC_MAC_ADDR_LEN] = { 0 };

#define CHIP_VERSION_MASK 0x0000FFF0
#define CHIP_VERSION_V100 0x100
#define CHIP_VERSION_SHIFT_LEFT_COUNT 4
/* Get the chip version */
ext_hw_chip_ver uapi_get_hw_chip_ver(td_void)
{
    td_u32 chip_info = 0;

    UAPI_REG_READ32(DW21_SYSCTRL_SC_VERSION_ID_REG, chip_info);
    chip_info = (chip_info & CHIP_VERSION_MASK) >> CHIP_VERSION_SHIFT_LEFT_COUNT;
    if (chip_info == CHIP_VERSION_V100) {
        return EXT_CHIP_VERSION_V100;
    } else {
        return EXT_CHIP_VERSION_UNKNOW;
    }
}

/* Get the chip type */
ext_ftm_chip_type uapi_get_hw_chip_type(td_void)
{
    td_u32 asic_ver = 0;

    UAPI_REG_READ32(DW21_SYSCTRL_SC_VERSION_ID_REG, asic_ver);

    if (EXT_GET_BIT_I(asic_ver, 1)) {
        return (ext_ftm_chip_type)EXT_FTM_CHIP_TYPE_21SV;
    } else {
        return (ext_ftm_chip_type)EXT_FTM_CHIP_TYPE_21EV;
    }
}

__init td_u32 uapi_board_init(td_void)
{
    ext_nv_ftm_product_id nv;
    td_u32 ret;

    (td_void)memset_s(&nv, sizeof(nv), 0, sizeof(nv));
    ret = uapi_factory_nv_read(EXT_NV_FTM_PRODUCT_ID, &nv, sizeof(nv));
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    g_product_type = nv.product_type;
    (td_void)memcpy_s(g_local_mac, sizeof(g_local_mac), nv.plc_mac, sizeof(nv.plc_mac));

    return ret;
}

__init td_u32 uapi_get_local_mac_addr(td_u8 mac[EXT_PLC_MAC_ADDR_LEN], td_u32 mac_size)
{
    if (mac != TD_NULL) {
        (td_void)memcpy_s(mac, mac_size, g_local_mac, sizeof(g_local_mac));
    }

    return EXT_ERR_SUCCESS;
}

__isr ext_ftm_product_type uapi_get_product_type(td_void)
{
    return g_product_type;
}

EXT_CPP_END
