/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Upload SAR-ADC/PGA  driver code  adc_drv.c  code
 * Author: CompanyName
 * Create: 2012-12-11
 */
#include "adc_drv.h"
#include <soc_stdlib.h>
#include <soc_mdm_isr.h>
#include <soc_mdm_adc.h>
#include <soc_mdm_time.h>
sar_adc_ctl g_sar_adc_ctl = {
    0,
};

td_void uapi_sar_adc_vref_cfg(ext_adc_vref_en vref)
{
    reg_analog_adc_wr40 reg_wr40 = { 0 };
    td_u32 int_value;

    int_value = uapi_int_lock();

    REG_FORMAT_UAPI_U32(reg_wr40) = com_bbp_reg_rd(REG_ANALOG_WR40);
    reg_wr40.sar_vref_switch = vref;
    com_bbp_reg_wr(REG_ANALOG_WR40, &reg_wr40);

    uapi_int_restore(int_value);
}

td_void uapi_sar_adc_noiseshaping_cfg(ext_adc_noiseshaping_en noiseshaping_pd)
{
    reg_analog_adc_wr40 reg_wr40 = { 0 };
    td_u32 int_value;

    int_value = uapi_int_lock();

    REG_FORMAT_UAPI_U32(reg_wr40) = com_bbp_reg_rd(REG_ANALOG_WR40);
    reg_wr40.sar_noiseshaping_pd = noiseshaping_pd;
    com_bbp_reg_wr(REG_ANALOG_WR40, &reg_wr40);

    uapi_int_restore(int_value);
}

td_u32 uapi_sar_adc_start(EXT_CONST ext_adc_channel_index_en channel_idx)
{
    reg_analog_adcwr7 reg_adcwr7 = { 0 };
    reg_analog_adcwr0 reg_adcwr0 = { 0 };
    reg_analog_adcwr9 reg_adcwr9 = { 0 };
    td_u32 int_value;

    switch (channel_idx) {
        case EN_CHANNEL_1:
            break;
        case EN_CHANNEL_2:
            break;
        case EN_CHANNEL_3:
            break;
        case EN_CHANNEL_4:
            break;
        case EN_CHANNEL_5:
            break;
        case EN_CHANNEL_6:
            break;
        default:
            return EXT_ERR_FAILURE;
    }

    /* First, stop and restart collecting again. */
    uapi_sar_adc_stop();

    int_value = uapi_int_lock();
    g_sar_adc_ctl.channel_idx = channel_idx;
    g_sar_adc_ctl.is_start = TD_TRUE;

    /* start up cic+filter fuction */
    REG_FORMAT_UAPI_U32(reg_adcwr7) = com_bbp_reg_rd(REG_ANALOG_ADCWR7);
    reg_adcwr7.cic_fir_bypass = 0x0;
    com_bbp_reg_wr(REG_ANALOG_ADCWR7, &reg_adcwr7);

    /* cs continuous mode, Lock with channel number, must be a single channel */
    REG_FORMAT_UAPI_U32(reg_adcwr0) = com_bbp_reg_rd(REG_ANALOG_ADCWR0);
    reg_adcwr0.cs_series = 1; /* Start cs continuous mode */
    reg_adcwr0.sel_num = channel_idx;
    reg_adcwr0.sel_num_ctrl = 0; /* Count from 0 */
    reg_adcwr0.sw_adc_pd = 0;    /* The ADC works according to the CS state, and the default is the powerdown state. */
    com_bbp_reg_wr(REG_ANALOG_ADCWR0, &reg_adcwr0);

    /* Start the conversion, first configure the pull low, then configure it to high to take effect. */
    REG_FORMAT_UAPI_U32(reg_adcwr9) = com_bbp_reg_rd(REG_ANALOG_ADCWR9);
    reg_adcwr9.adc_work_en = 0;
    com_bbp_reg_wr(REG_ANALOG_ADCWR9, &reg_adcwr9);
    REG_FORMAT_UAPI_U32(reg_adcwr9) = com_bbp_reg_rd(REG_ANALOG_ADCWR9);
    reg_adcwr9.adc_work_en = 1;
    com_bbp_reg_wr(REG_ANALOG_ADCWR9, &reg_adcwr9);

    /* According to the test results, the first time need to wait 100us to stabilize */
    uapi_udelay(100);

    uapi_int_restore(int_value);

    return EXT_ERR_SUCCESS;
}

__hot td_u16 uapi_sar_adc_get_data(td_void)
{
    reg_analog_adcr adc_value = { 0 };
    td_u16 ret;
    td_u32 int_value;

    int_value = uapi_int_lock();
    if (g_sar_adc_ctl.is_start == TD_FALSE) {
        ret = (td_u16)EXT_ERR_FAILURE;
        uapi_int_restore(int_value);
        return ret;
    }

    switch (g_sar_adc_ctl.channel_idx) {
        case EN_CHANNEL_1:
            REG_FORMAT_UAPI_U32(adc_value) = com_bbp_reg_rd(REG_ANALOG_ADCR00);
            ret = adc_value.adc_ch0_data_sync;
            break;
        case EN_CHANNEL_2:
            REG_FORMAT_UAPI_U32(adc_value) = com_bbp_reg_rd(REG_ANALOG_ADCR00);
            ret = adc_value.adc_ch1_data_sync;
            break;
        case EN_CHANNEL_3:
            REG_FORMAT_UAPI_U32(adc_value) = com_bbp_reg_rd(REG_ANALOG_ADCR01);
            ret = adc_value.adc_ch0_data_sync;
            break;
        case EN_CHANNEL_4:
            REG_FORMAT_UAPI_U32(adc_value) = com_bbp_reg_rd(REG_ANALOG_ADCR01);
            ret = adc_value.adc_ch1_data_sync;
            break;
        case EN_CHANNEL_5:
            REG_FORMAT_UAPI_U32(adc_value) = com_bbp_reg_rd(REG_ANALOG_ADCR02);
            ret = adc_value.adc_ch0_data_sync;
            break;
        case EN_CHANNEL_6:
            REG_FORMAT_UAPI_U32(adc_value) = com_bbp_reg_rd(REG_ANALOG_ADCR02);
            ret = adc_value.adc_ch1_data_sync;
            break;
        default:
            ret = (td_u16)EXT_ERR_FAILURE;
    }

    uapi_int_restore(int_value);
    return ret;
}

__hot td_void uapi_sar_adc_stop(td_void)
{
    reg_analog_adcwr0 reg_adcwr0 = { 0 };
    reg_analog_adcwr9 reg_adcwr9 = { 0 };
    td_u32 int_value;

    int_value = uapi_int_lock();
    /* Stop conversion */
    REG_FORMAT_UAPI_U32(reg_adcwr9) = com_bbp_reg_rd(REG_ANALOG_ADCWR9);
    reg_adcwr9.adc_work_en = 0;
    com_bbp_reg_wr(REG_ANALOG_ADCWR9, &reg_adcwr9);

    REG_FORMAT_UAPI_U32(reg_adcwr0) = com_bbp_reg_rd(REG_ANALOG_ADCWR0);
    reg_adcwr0.sw_adc_pd = 1;
    com_bbp_reg_wr(REG_ANALOG_ADCWR0, &reg_adcwr0);
    g_sar_adc_ctl.is_start = TD_FALSE;
    (td_void)memset_s(&g_sar_adc_ctl, sizeof(g_sar_adc_ctl), 0x0, sizeof(g_sar_adc_ctl));
    uapi_int_restore(int_value);
}
