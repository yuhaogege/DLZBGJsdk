/*
 * Copyright (c) CompanyNameMagicTag 2019-2019. All rights reserved.
 * Description: tsensor driver implementatioin.
 * Author: CompanyName
 * Create: 2019-06-03
 */

#include "tsensor.h"
#include <soc_mdm_tsensor.h>
#include <soc_mdm_time.h>
#include <dw21_platform_rom.h>

typedef struct {
    td_u64 start_tick;
    td_bool tsensor_start;
    td_u8 pad[7]; /* reserved 7B */
} ext_tsensor_start_s;

ext_tsensor_start_s g_tsensor_state;

td_u32 uapi_tsensor_start(ext_tsensor_mode mode, td_u32 ms)
{
    td_u32 tmp_val = 0;

    if (mode >= EXT_TSENSOR_MODE_MAX) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    if (ms % 2 != 0) { /* 2 */
        return EXT_ERR_INVALID_PARAMETER;
    }
    ms = ms / 2; /* 2 */

    /* stop tsensor */
    uapi_tsensor_stop();
    UAPI_REG_READ32(EXT_TSENSOR_REG_BASE + TSENSOR_TEMPEN_CTL, tmp_val);
    tmp_val &= 0xFFFF00FF;
    tmp_val |= (td_u32)ms << 8; /* 8 */
    UAPI_REG_WRITE32(EXT_TSENSOR_REG_BASE + TSENSOR_TEMPEN_CTL, tmp_val);

    if (mode == EXT_TSENSOR_MODE_16_POINTS_SINGLE) {
        ext_reg_setbit(EXT_TSENSOR_REG_BASE + TSENSOR_TEMPEN_CTL, TSENSOR_TEMPEN_CTL_SINGLE);
    } else if (mode == EXT_TSENSOR_MODE_16_POINTS_LOOP) {
        ext_reg_setbit(EXT_TSENSOR_REG_BASE + TSENSOR_TEMPEN_CTL, TSENSOR_TEMPEN_CTL_LOOP);
    } else if (mode == EXT_TSENSOR_MODE_1_POINT_LOOP) {
        ext_reg_setbit(EXT_TSENSOR_REG_BASE + TSENSOR_TEMPEN_CTL, TSENSOR_TEMPEN_CTL_LOOP);
        ext_reg_setbit(EXT_TSENSOR_REG_BASE + TSENSOR_TEMPEN_CTL, TSENSOR_TEMPEN_CTL_CNT16_BYPASS);
    }
    g_tsensor_state.start_tick = uapi_get_tick64();
    g_tsensor_state.tsensor_start = TD_TRUE;

    return EXT_ERR_SUCCESS;
}

EXT_PRV td_float uapi_tsensor_code_to_temperature(td_u16 code)
{
    return (td_float)((code - TSENSOR_LOW_CODE_VALUE) * TSENSOR_TEMPERATURE_D_VALUE /
                      (td_float)TSENSOR_CODE_D_VALUE +
                      TSENSOR_TEMPERATURE_LOW);
}

EXT_PRV td_u16 uapi_tsensor_temperature_to_code(td_float temp)
{
    if (temp < TSENSOR_TEMPERATURE_LOW || temp > TSENSOR_TEMPERATURE_HIGH) {
        return EXT_TSENSOR_INVALID_CODE;
    }
    return (td_u16)((temp - TSENSOR_TEMPERATURE_LOW) * (td_float)TSENSOR_CODE_D_VALUE / TSENSOR_TEMPERATURE_D_VALUE) +
           TSENSOR_LOW_CODE_VALUE;
}

td_u32 uapi_tsensor_set_outtemp_threshold(td_float low_temp, td_float high_temp)
{
    if ((low_temp < TSENSOR_TEMPERATURE_LOW) || (high_temp > TSENSOR_TEMPERATURE_HIGH)
        || (low_temp > high_temp)) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    /* write temp code low and high limit */
    td_u16 high_temp_code = (td_u16)uapi_tsensor_temperature_to_code(high_temp);
    td_u16 low_temp_code = (td_u16)uapi_tsensor_temperature_to_code(low_temp);
    td_u32 tmp_val = (td_u32)(high_temp_code << 16) + (td_u32)low_temp_code; /* 16 */
    UAPI_REG_WRITE32(EXT_TSENSOR_REG_BASE + TSENSOR_LIMIT_CTL, tmp_val);

    return EXT_ERR_SUCCESS;
}

td_u32 uapi_tsensor_read_temperature(td_float *temperature, ext_tsensor_temp num)
{
    td_u32 temperature_code = 0;

    if (temperature == TD_NULL || num >= EXT_TSENSOR_TEMP_MAX) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    if (g_tsensor_state.tsensor_start == TD_FALSE) {
        return EXT_ERR_FAILURE;
    }

    if (uapi_get_tick64() - g_tsensor_state.start_tick < TSENSOR_DLY_20MS) {
        return EXT_ERR_FAILURE;
    }

    UAPI_REG_READ32(EXT_TSENSOR_REG_BASE + TSENSOR_TEMP0 + ((td_u32)num << 2), temperature_code); /* 2 */
    temperature_code &= 0x000003FF;
    *temperature = uapi_tsensor_code_to_temperature((td_u16)temperature_code);

    return EXT_ERR_SUCCESS;
}

td_void uapi_tsensor_stop(td_void)
{
    /* disable tsensor */
    ext_reg_clrbit(EXT_TSENSOR_REG_BASE + TSENSOR_TEMPEN_CTL, TSENSOR_TEMPEN_CTL_LOOP);
    ext_reg_clrbit(EXT_TSENSOR_REG_BASE + TSENSOR_TEMPEN_CTL, TSENSOR_TEMPEN_CTL_SINGLE);
    ext_reg_clrbit(EXT_TSENSOR_REG_BASE + TSENSOR_TEMPEN_CTL, TSENSOR_TEMPEN_CTL_CNT16_BYPASS);
    g_tsensor_state.tsensor_start = TD_FALSE;

    return;
}
