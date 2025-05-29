/*
 * Copyright (c) CompanyNameMagicTag 2012-2018. All rights reserved.
 * Description: Phy NV definitions
 * Author: CompanyName
 * Create: 2012-07-16
 */

#ifndef PHY_NV_21_H
#define PHY_NV_21_H

#include "soc_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Base value of the changed ID of the NV item */
/* Base value of the changed ID of the NV item */
#define DIAG_CMD_TONEMASK_NV_STRU_ID 0x020
#define DIAG_CMD_PHY_MAIN_PARAM_NV_STRU_ID 0x021
#define DIAG_CMD_NTB_CALC_NV_ID 0x022
#define DIAG_CMD_PHY_STUB_NV_STRU_ID 0x023
#define DIAG_CMD_DIGITAL_PWR_NV_STRU_ID 0x024
#define DIAG_CMD_AFE_PWR_NV_STRU_ID 0x025
#define DIAG_CMD_SPC_NV_STRU_ID 0x02d
#define DIAG_CMD_MAX_NV_ID DIAG_CMD_SPC_NV_STRU_ID

#define MAX_FREQ_NUM_V100 9 /* Number of V100 frequency bands (7) */
#define FREQ_MAX_CNT 8

#define MAX_TONEMASK_NUM 8
#define NBI_START_STOP_CNT 3
#define FREQ_BAND_CNT 3
#define FREQ_INFO_LEN 15

/* Data Structure Definition */
typedef struct {
    td_u8 param1;
    td_u8 param2;
    td_u8 param3;
    td_u8 param4;
} freq_self_detect_mask_stru;

typedef struct {
    td_u32 param1 : 8;
    td_u32 param2 : 1;
    td_u32 param3 : 8;
    td_u32 param4 : 6;
    td_u32 param5 : 3;
    td_u32 param6 : 6;

    td_u32 param7 : 2;
    td_u32 param8 : 7;
    td_u32 param9 : 7;
    td_u32 param10 : 7;
    td_u32 param11 : 7;
    td_u32 param12 : 1;
    td_u32 param13 : 1;

    freq_self_detect_mask_stru param14[4]; /* 4 items in mask entry */

    td_u32 param15 : 1;
    td_u32 param16 : 7;
    td_s32 param17 : 7;
    td_u32 param18 : 14;
    td_u32 param19 : 3;

    td_u32 param20 : 5;
    td_u32 param21 : 6;
    td_u32 param22 : 1;
    td_u32 param23 : 20;

    td_u32 param24 : 14;
    td_u32 param25 : 14;
    td_u32 param26 : 4;

    td_u32 param27 : 14;
    td_u32 param28 : 5;
    td_u32 param29 : 1;
    td_u32 param30 : 5;
    td_u32 param31 : 7;

    td_u32 param33 : 8;
    td_u32 param34 : 8;
    td_u32 param35 : 8;
    td_u32 param36 : 8;

    td_u32 param37;

    td_u8 param38 : 1;
    td_u8 param39 : 1;
    td_u8 param40 : 1;
    td_u8 param41 : 4;
    td_u8 param42 : 1;
    td_u8 param43[NBI_START_STOP_CNT];

    td_u8 param44[NBI_START_STOP_CNT];
    td_u8 param45;

    td_u16 param46[NBI_START_STOP_CNT];
    td_u16 param47 : 8;
    td_u16 param48 : 8;

    td_u8 param49;
    td_u8 param50;
    td_s8 param51;
    td_s8 param52;

    td_u16 param53 : 12;
    td_u16 param54 : 3;
    td_u16 param55 : 1;
    td_u8 param56;
    td_u8 param57;

    td_u8 param58;
    td_u8 param59;
    td_u16 param60;

    td_u32 param61 : 1;
    td_u32 param62 : 1;
    td_u32 param63 : 1;
    td_u32 param64 : 8;
    td_u32 param65 : 21;
} diag_cmd_phy_main_param_nv_stru;

typedef struct {
    td_u32 param[MAX_TONEMASK_NUM];
} diag_cmd_tonemask_nv_mask_stru; /* 32 bytes in total */

typedef struct {
    td_u16 param1;
    td_u8 param2;
    td_u8 param3;

    td_u32 param4;
    td_u16 param5;
    td_u16 param6;
} diag_cmd_ntb_calc_nv_mask_stru; /* 30 bytes in total */

typedef struct {
    td_u8 param1[FREQ_MAX_CNT];
} diag_cmd_digital_pwr_nv_mask_stru; /* 8 bytes in total */

typedef struct {
    td_u8 param1;
    td_u8 param2[3];             /* Reserved 3Bytes */
} diag_cmd_afe_pwr_nv_mask_stru; /* 4 bytes in total */

typedef struct {
    td_u8 power_adjust[FREQ_MAX_CNT]; /* Indicates the adjusted transmit power. Value range:  -40 to +26.
                                       * The configured value is the corresponding complement.
                                       */
} diag_cmd_digital_pwr_nv_stru;       /* 8 bytes in total */

typedef struct {
    td_u8 mpw_afe_gain;
    td_u8 pad[3];           /* Reserved 3Bytes */
} diag_cmd_afe_pwr_nv_stru; /* 4 bytes in total */

typedef struct {
    td_u8 param1;
    td_u8 param2;
    td_u8 param3;
    td_u8 param4;

    td_u8 param5;
    td_u8 param6;
    td_u8 param7;
    td_u8 param8;

    td_u8 param9;
    td_u8 param10;
    td_u8 param11;
    td_u8 param12;

    td_u8 param13;
    td_u8 param14;
    td_u16 param15;

    td_u8 param16;
    td_u8 param17;
    td_u8 param18;
    td_u8 param19;
    td_u32 param20;
    td_u32 param21;

    td_u32 param22;
    td_u32 param23;
    td_u16 param24;
    td_u8 param25;
    td_u8 param26;
} diag_cmd_phy_stub_nv_mask_stru; /* 36 bytes in total */

typedef struct {
    td_u32 param_w0 : 1;
    td_u32 param_w1 : 1;
    td_u32 param_w2 : 1;
    td_u32 param_w3 : 1;
    td_u32 param_w4 : 1;
    td_u32 param_w5 : 1;
    td_u32 param_w6 : 1;
    td_u32 param_w7 : 1;
    td_u32 param_w8 : 1;
    td_u32 param_w9 : 1;
    td_u32 param_w10 : 1;
    td_u32 reserve : 21;
} param_w_stru;

typedef struct {
    td_u16 spc_cycle_num;
    td_bool is_rcv_win_enable; /* Indicates whether the receive window is enabled. */
    td_u8 spc_rxlpf_mul2_en;

    td_u8 spc_rx_dec51200_mul2_en;
    td_u8 spc_rx_dec10240_mul2_en;
    td_u8 spc_rx_dec2048_mul2_en;
    td_u8 spc_rx_dec512_mul2_en;

    td_u8 spc_rx_dec128_mul2_en;
    td_u8 spc_rx_dec32_mul2_en;
    td_u8 spc_rx_dec8_mul2_en;
    td_u8 tx_space_en;

    td_u8 tx_mark_en;
    td_bool is_spc_enable;
    td_u8 tx_right_shift;
    td_u8 tx6p25shift_num;

    td_u8 tx_up_convert_freq;
    td_u8 peakdisttoler;
    td_u8 syn_cc_window_en;
    td_u8 sync_win_advance;

    td_u8 smallhalfwinmin;
    td_u8 smallhalfwinmax;
    td_u8 largehalfwin;
    td_u8 absscale;

    td_u8 sumscale;
    td_u8 valley_scale;
    td_u8 plc_chl_timeout; /* Timeout interval for switching to the PLC mode, in seconds. */
    td_u8 gold_code_rep_num;

    td_u8 gold_thres_large;
    td_u8 gold_thres_small;
    td_u8 cont_lost_sync_thres;
    td_u8 total_lost_sync_thres;

    td_u16 clear_identify_result_time_max; /* Maximum time for clearing the recognition result, in seconds. */
    td_u16 gold_cycle_num;
    /*
     * Whether the function of ending the identification status
     * upon receiving a network identification request is enabled
     */
    td_bool is_single_rcv_to_over_enable;
    td_bool is_sync_atten_enable; /* Indicates whether the attenuation threshold policy is enabled
                                   * during synchronization.
                                   */
    td_u8 neighbor_net_atten_thres;
    td_u8 is_lock_snid_disable;

    param_w_stru param_w;
} diag_cmd_spc_nv_stru; /* 32 bytes in total */

#ifdef __cplusplus
}
#endif

#endif
