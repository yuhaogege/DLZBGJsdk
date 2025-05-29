/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Upload SAR-ADC/PGA driver code¡£adc_drv.h code
 * Author: CompanyName
 * Create: 2012-12-11
 */
#ifndef __ADC_DRV_H__
#define __ADC_DRV_H__

#include <soc_mdm_adc.h>
#include <soc_mdm_types.h>
#include <soc_mdm_isr.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define EXT_SAR_ADC_REG_BASE 0x40040000

#define REG_ANALOG_WR40 (EXT_SAR_ADC_REG_BASE + 0x34)
#define REG_ANALOG_ADCWR0 (EXT_SAR_ADC_REG_BASE + 0x40)
#define REG_ANALOG_ADCWR7 (EXT_SAR_ADC_REG_BASE + 0x5c)
#define REG_ANALOG_ADCWR9 (EXT_SAR_ADC_REG_BASE + 0x64)

#define REG_ANALOG_ADCR00 (EXT_SAR_ADC_REG_BASE + 0x44)
#define REG_ANALOG_ADCR01 (EXT_SAR_ADC_REG_BASE + 0x48)
#define REG_ANALOG_ADCR02 (EXT_SAR_ADC_REG_BASE + 0x6C)
#define REG_ANALOG_ADCW

typedef struct {
    td_u32 resv1 : 7;
    td_u32 clk_gate : 1;

    td_u32 buf_clk_sel : 2;
    td_u32 buf_clk_rst : 1;
    td_u32 buf_pd_clk : 1;
    td_u32 pga_clk_sel : 2;
    td_u32 pga_clk_rst : 1;
    td_u32 pga_pd_clk : 1;

    td_u32 sar_noiseshaping_pd : 1;
    td_u32 sar_fir_gain : 4;
    td_u32 sar_cs_mode : 2;
    td_u32 sar_cs_ctrl : 1;

    td_u32 sar_pulse_pd : 1;
    td_u32 sar_ctrl_pd : 2;
    td_u32 sar_vref_switch : 1;
    td_u32 sar_clk_selct : 2;
    td_u32 sar_pga_debug : 1;
    td_u32 pga_bypass : 1;
} reg_analog_adc_wr40;

/* REG_ANALOG_ADCWR7 */
typedef struct {
    td_u32 fir2_coeff4 : 12;
    td_u32 cic_fir_bypass : 1;
    td_u32 rsvd : 19;
} reg_analog_adcwr7;

/* REG_ANALOG_ADCWR0 */
typedef struct {
    td_u32 rsvd1 : 4;
    td_u32 sw_adc_pd : 1;
    td_u32 rsvd2 : 3;

    td_u32 cs_series : 1;
    td_u32 rsvd3 : 3;
    td_u32 sel_num_ctrl : 3;
    td_u32 rsvd4 : 1;

    td_u32 sel_num : 6;
    td_u32 rsvd5 : 10;
} reg_analog_adcwr0;

typedef struct {
    td_u32 adc_work_en : 1;
    td_u32 rsvd : 31;
} reg_analog_adcwr9;

/* Output data report register */
typedef struct {
    td_u32 rsvd0 : 1;
    td_u32 adc_ch0_data_sync : 12;
    td_u32 rsvd1 : 4;
    td_u32 adc_ch1_data_sync : 12;
    td_u32 rsvd2 : 3;
} reg_analog_adcr;

#define MAC_CHANNEL_NUM 6 /* Number of channels supported */

typedef struct {
    td_u32 channel_idx;
    td_bool is_start;
    td_u8 resv[3]; /* Reserved 3 bytes */
} sar_adc_ctl;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
