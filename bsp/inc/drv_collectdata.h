/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: collect data
 * Author: CompanyName
 * Create: 2012-07-14
 */

#ifndef DRV_COLLECT_DATA_H
#define DRV_COLLECT_DATA_H
#if defined PRODUCT_CFG_NDM_COLLECT_DATA
#include <soc_mdm_datacollect.h>
#define DRV_DC_UN_FIX_AGCGAIN ((td_s32)0xFFFFFFFF)

typedef enum {
    EXT_DRV_DATA_COLLECT_STEP_PREPARE = 0x1,
    EXT_DRV_DATA_COLLECT_STEP_START = 0x2,
    EXT_DRV_DATA_COLLECT_STEP_GET_RESULT = 0x4,
    /* The following special processing is performed: */
    EXT_DRV_DATA_COLLECT_STEP_FORCE_RESTORE = 0x8,
} ext_drv_data_collect_step;

/* Define the struct C_fix_aagc_gain */
struct c_fix_aagc_gain {
    unsigned int fix_aagc_gain : 8;
    unsigned int sunspec_fix_aagc_gain : 7;
    unsigned int reserved_1 : 1;
    unsigned int afe_pga_gain_rpt : 7;
    unsigned int reserved_2 : 9;
};
typedef struct {
    td_u32 afe_version;     // 40040000 01280900
    td_u32 reg_testmode;    // 40040004 00000000
    td_u32 reg_analog_wr00; // 40040010 A70058A3
    td_u32 reg_analog_wr04; // 40040014 DF78886C
    td_u32 reg_analog_wr08; // 40040018 8F5B1550
    td_u32 reg_analog_wr12; // 4004001c 08080822
    td_u32 reg_analog_wr20; // 40040020 22222208
    td_u32 reg_analog_wr24; // 40040024 22222222
    td_u32 reg_analog_wr28; // 40040028 22222222
    td_u32 reg_analog_wr32; // 4004002c 10365512
    td_u32 reg_analog_wr40; // 40040034 1806DD00
    td_u32 reg_analog_wr44; // 40040038 0024548F
    td_u32 reg_sc_afe_ctrl;
    td_u32 reg_sc_peri_ctrl;
} dc_afe_backup;

#define EXT_DRV_DATA_COLLECT_STEP_ALL \
    (EXT_DRV_DATA_COLLECT_STEP_PREPARE | EXT_DRV_DATA_COLLECT_STEP_START | EXT_DRV_DATA_COLLECT_STEP_GET_RESULT)
#define EXT_DRV_DATA_COLLECT_OVERFLOW (1 << 1)
#define EXT_DRV_DATA_COLLECT_ADDR_CYCLE_IND (1 << 17)
#define EXT_DRV_DATA_COLLECT_TEST_MODE_OFFSET (2)
#define EXT_DRV_DATA_COLLECT_TRIG_TEST_MODE_BASE (255)

td_u32 dc_check_triger_type(dc_trig_type_enum triger_type);
td_u32 dc_check_data_src(dc_trig_data_enum data_src);
td_u32 dc_check_agc_gain(td_s32 l_agc_gain);
td_u32 dc_check_fix_agc_gain(td_s32 l_agc_gain);
td_u32 dc_check_addr_and_len(uintptr_t start_addr, uintptr_t end_addr, td_u32 after_pointlen);
td_u32 dc_data_collect(EXT_IN dc_input_para_stru *dc_input, EXT_OUT ext_buf_s *s_buf, ext_drv_data_collect_step step);
td_void dc_data_collect_force_restore(td_void); /* restore */
#endif
#endif // __DRV_COLLECT_DATA_H__
