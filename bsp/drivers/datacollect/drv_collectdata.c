/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: add data collect
 * Create: 2012-09-10
 */
#include <soc_types.h>
#include <soc_mdm_types.h>

#if defined PRODUCT_CFG_NDM_COLLECT_DATA
#include <dfx_phy.h>
#include <drv_collectdata.h>
#include <mac_inf.h>
#include <sal_common.h>
#include <soc_lowpower.h>
#include <soc_diag_async.h>

#define IO_EXP_NUM_BPF_SHORT IO_EXP_IO_NUM_03

#define REG_DC_AGC_SET 0x40020534
#define REG_DC_AGC_VAL 0x40020538

#define DC_BASE_ADDR 0x40020e00
#define DC_CONTROL (DC_BASE_ADDR + 0x80)
#define DC_RX_STATUS (DC_BASE_ADDR + 0x84)
#define DC_RX_START_ADDR (DC_BASE_ADDR + 0x88)
#define DC_RX_END_ADDR (DC_BASE_ADDR + 0x8c)
#define DC_RX_CURR_ADDR (DC_BASE_ADDR + 0x90)
#define DC_TX_STATUS (DC_BASE_ADDR + 0x94)
#define DC_TX_START_ADDR (DC_BASE_ADDR + 0x98)
#define DC_TX_END_ADDR (DC_BASE_ADDR + 0x9c)
#define DC_TX_CURR_ADDR (DC_BASE_ADDR + 0xa0)
#define DC_DELAY_COUNT (DC_BASE_ADDR + 0xac)
#define DC_CAPTURE_CTRL (DC_BASE_ADDR + 0xb0)
#define DC_TRIGGER_MASK (DC_BASE_ADDR + 0xb4)
#define DC_TRIGGER_DATA (DC_BASE_ADDR + 0xb8)

#define CAPTURE_CTRL_REG_LADDR_BIT_ST 0

#define CAPTURE_CTRL_REG_TRI_MODE_BIT_ST 16

#define CAPTURE_CTRL_REG_TRI_TYPE_BIT_ST 18

#define CAPTURE_CTRL_REG_VALID_CTRL_BIT_ST 21

#define CAPTURE_CTRL_REG_TRI_SEL_BIT_ST 24

#define CAPTURE_CTRL_TRI_MODE_NOT_TRI 0
#define CAPTURE_CTRL_TRI_MODE_SOFT_TRI 1
#define CAPTURE_CTRL_TRI_MODE_BUS_TRI 2
#define CAPTURE_CTRL_TRI_MODE_MASK_TRI 3

#define CAPTURE_CTRL_TRI_TYPE_LOW_LEVEL 0
#define CAPTURE_CTRL_TRI_TYPE_RISE_EDGE 1
#define CAPTURE_CTRL_TRI_TYPE_FALL_EDGE 2
#define CAPTURE_CTRL_TRI_TYPE_HIGH_LEVEL 3
td_u32 g_force_stop_dc = 0;
td_u32 dc_check_triger_type(dc_trig_type_enum triger_type)
{
    if ((triger_type >= EN_TRIG_TYPE_MAX) && (triger_type != EN_TRIG_TYPE_MASK_DATA)) {
        return EXT_ERR_FAILURE;
    } else {
        return EXT_ERR_SUCCESS;
    }
}

td_u32 dc_check_data_src(dc_trig_data_enum data_src)
{
    if (data_src >= EN_TRIG_DATA_MAX) {
        return EXT_ERR_FAILURE;
    } else {
        return EXT_ERR_SUCCESS;
    }
}

td_u32 dc_check_agc_gain(td_s32 l_agc_gain)
{
    if (l_agc_gain == DRV_DC_UN_FIX_AGCGAIN) {
        return EXT_ERR_SUCCESS;
    }
    if (l_agc_gain < -18 || l_agc_gain > 42 || (((td_u32)l_agc_gain & 0x1) != 0x0)) { // check agc -18 ~ 42
        return EXT_ERR_FAILURE;
    } else {
        return EXT_ERR_SUCCESS;
    }
}

td_u32 dc_check_fix_agc_gain(td_s32 l_agc_gain)
{
    if (l_agc_gain < -18 || l_agc_gain > 42 || (((td_u32)l_agc_gain & 0x1) != 0x0)) { // check agc -18 ~ 42
        return EXT_ERR_FAILURE;
    } else {
        return EXT_ERR_SUCCESS;
    }
}

td_u32 dc_check_addr_and_len(uintptr_t start_addr, uintptr_t end_addr, td_u32 after_pointlen)
{
    if (start_addr >= end_addr) {
        return EXT_ERR_FAILURE;
    }

    if (((start_addr & 0x3) != 0) || ((end_addr & 0x3) != 0) || ((after_pointlen & 0x3) != 0) ||
        (after_pointlen == 0)) {
        return EXT_ERR_FAILURE;
    }

    /*
     * Confirm with the chip staff that the collecting length after the trigger point cannot
     * be greater than collecting interval. If the user is configured incorrectly, the software will report an error.
     * CNComments:
     * 与芯片人员确认触发点后的采集长度不可能大于采数区间，
     * 如果使用人员配置错误软件报错.
     */
    if ((end_addr - start_addr) < after_pointlen) {
        return EXT_ERR_FAILURE;
    }

    return EXT_ERR_SUCCESS;
}

EXT_PRV td_u32 dc_check_para(dc_input_para_stru *dc_input)
{
    if (dc_input == TD_NULL) {
        return EXT_ERR_FAILURE;
    }
    if (EXT_ERR_SUCCESS != dc_check_addr_and_len(dc_input->start_addr, dc_input->end_addr, dc_input->after_point_len)) {
        return EXT_ERR_FAILURE;
    }
    if (EXT_ERR_SUCCESS != dc_check_triger_type(dc_input->triger_type)) {
        return EXT_ERR_FAILURE;
    }
    if (EXT_ERR_SUCCESS != dc_check_data_src(dc_input->data_src)) {
        return EXT_ERR_FAILURE;
    }

    if (EXT_ERR_SUCCESS != dc_check_agc_gain(dc_input->agc_gain)) {
        return EXT_ERR_FAILURE;
    }

    return EXT_ERR_SUCCESS;
}
EXT_PRV td_void dc_data_config_triger_type(EXT_IN dc_input_para_stru *dc_input)
{
    dc_trig_data_enum data_src_temp = dc_input->data_src;
    td_u32 ctrl_reg_val = 0;
    td_u32 ctrl_tri_type = 0;
    td_u32 ctrl_tri_mode = 0;
    td_u32 ctrl_tri_sel = 0;
    td_u32 ctrl_valid_ctrl = 0;
    if ((EN_TRIG_INCREMENTAL_SEQUENCE == data_src_temp) || (EN_TRIG_DESCENDING_SEQUENCE == data_src_temp)) {
        data_src_temp = EN_TRIG_AD_12_BIT;
    }
    if ((EN_TRIG_LOGIC_32_BIT_LOGIC == data_src_temp) || (EN_TRIG_LOGIC_16_HBIT_LOGIC == data_src_temp)) {
        data_src_temp = EN_TRIG_LOGIC_16_LBIT_LOGIC;
    }
    // 6.configure trigger type and data source
    if (EN_TRIG_TYPE_NOISE == dc_input->triger_type) {
        ctrl_tri_sel = 0xff;                             // fill in 0xff for data source when collecting noise
        ctrl_tri_type = CAPTURE_CTRL_TRI_TYPE_LOW_LEVEL; // no care
        ctrl_tri_mode = CAPTURE_CTRL_TRI_MODE_NOT_TRI;   // no care
    } else if (EN_TRIG_TYPE_MASK_DATA == dc_input->triger_type) {
        ctrl_tri_sel = 0;                                // no care
        ctrl_tri_type = CAPTURE_CTRL_TRI_TYPE_LOW_LEVEL; // no care
        ctrl_tri_mode = CAPTURE_CTRL_TRI_MODE_MASK_TRI;
    } else {
        ctrl_tri_sel = dc_input->triger_type; // no care
        ctrl_tri_type = CAPTURE_CTRL_TRI_TYPE_HIGH_LEVEL;
        ctrl_tri_mode = CAPTURE_CTRL_TRI_MODE_BUS_TRI;
    }
    ctrl_valid_ctrl = 1;

    ctrl_reg_val |= (ctrl_tri_sel << CAPTURE_CTRL_REG_TRI_SEL_BIT_ST);
    ctrl_reg_val |= (ctrl_valid_ctrl << CAPTURE_CTRL_REG_VALID_CTRL_BIT_ST);
    ctrl_reg_val |= (ctrl_tri_type << CAPTURE_CTRL_REG_TRI_TYPE_BIT_ST);
    ctrl_reg_val |= (ctrl_tri_mode << CAPTURE_CTRL_REG_TRI_MODE_BIT_ST);
    ctrl_reg_val = ctrl_reg_val | ((unsigned int)data_src_temp << CAPTURE_CTRL_REG_LADDR_BIT_ST);
    if (EN_TRIG_TYPE_NOISE == dc_input->triger_type) {
         /* configure data source as AD; blind collecting; not trigger */
        UAPI_REG_WRITE32(DC_CAPTURE_CTRL, ctrl_reg_val);
    } else {
        /* collect pDcInput->ulAfterPointlen size after triggering */
        UAPI_REG_WRITE32(DC_DELAY_COUNT, dc_input->after_point_len);
        UAPI_REG_WRITE32(DC_CAPTURE_CTRL, ctrl_reg_val); /* Set to high level trigger */
    }
}
EXT_PRV td_void dc_data_collect_prepare(EXT_IN dc_input_para_stru *dc_input, dc_afe_backup *afe_backup)
{
    ext_unref_param(afe_backup);

    /* clear data collection space */
    (td_void)memset_s((td_void *)dc_input->start_addr, (dc_input->end_addr - dc_input->start_addr), 0xff,
        (dc_input->end_addr - dc_input->start_addr));

    // 2.PHY mutual exclusion
    if (dc_input->data_src != EN_TRIG_DA_12_BIT) {
        drv_set_collect_data_flag(COLLECT_DATA_ENABLE);
    }
    // wait(1s), ensure that the transmitter completes the current transmission
    // hardware filter generate noise in a moment by pass. needs to wait for a while
    if (dc_input->wait_envok_time) {
        uapi_sleep(dc_input->wait_envok_time);
    }
    UAPI_REG_WRITE32(0x40020dd8, 0x12C0018);

    // PHY TX enble
    UAPI_REG_WRITE32(0x40020d28, 1);
    // 4.configure AGC gain
    if ((dc_input->agc_gain) != DRV_DC_UN_FIX_AGCGAIN) { // means whether AGC is automatic
        td_u32 temp_value = 0;
        struct c_fix_aagc_gain *fix_aagc_gain = (struct c_fix_aagc_gain *)&temp_value;
        fix_aagc_gain->fix_aagc_gain = (td_u32)dc_input->agc_gain;
        UAPI_REG_WRITE32(REG_DC_AGC_VAL, temp_value);
        UAPI_REG_WRITE32(REG_DC_AGC_SET, TD_FALSE);
        UAPI_REG_WRITE32(REG_DC_AGC_SET, TD_TRUE);
    } else {
        UAPI_REG_WRITE32(REG_DC_AGC_SET, TD_FALSE);
    }
    // 5.configure start and end address of data collection
    UAPI_REG_WRITE32(DC_RX_START_ADDR, dc_input->start_addr); // start address of data collection
    UAPI_REG_WRITE32(DC_RX_END_ADDR, dc_input->end_addr);     // end address of data collection
    dc_data_config_triger_type(dc_input);
}

EXT_PRV td_void dc_data_collect_start(EXT_IN dc_input_para_stru *dc_input)
{
    // 7.start collection
    if (dc_input->data_src == EN_TRIG_FFT_ORDER_32_BIT || (dc_input->data_src == EN_TRIG_FFT_DISORDER_32_BIT) ||
        (dc_input->data_src == EN_TRIG_IMP_18_BIT) || (dc_input->data_src == EN_TRIG_LOGIC_32_BIT_LOGIC)) {
        if (EN_TRIG_TYPE_NOISE == dc_input->triger_type) {
            // start collect, stop automatically when length is enough. data: 32bits
            UAPI_REG_WRITE32(DC_CONTROL, 0x011);
        } else {
            // start collect, stop at collect ulAfterPointlen after the trigger point
            UAPI_REG_WRITE32(DC_CONTROL, 0x001);
        }
    } else if (dc_input->data_src == EN_TRIG_LOGIC_16_HBIT_LOGIC) {
        if (EN_TRIG_TYPE_NOISE == dc_input->triger_type) {
            // start collect, stop automatically when length is enough. data: 32bits
            UAPI_REG_WRITE32(DC_CONTROL, 0x211);
        } else {
            // start collect, stop at collect ulAfterPointlen after the trigger point
            UAPI_REG_WRITE32(DC_CONTROL, 0x201);
        }
    } else if (dc_input->data_src == EN_TRIG_INCREMENTAL_SEQUENCE ||
        (dc_input->data_src == EN_TRIG_DESCENDING_SEQUENCE)) {
        if (EN_TRIG_TYPE_NOISE == dc_input->triger_type) {
            UAPI_REG_WRITE32(DC_CONTROL,
                (0x111 | ((td_u32)(dc_input->data_src - EXT_DRV_DATA_COLLECT_TRIG_TEST_MODE_BASE) <<
                EXT_DRV_DATA_COLLECT_TEST_MODE_OFFSET)));
        } else {
            UAPI_REG_WRITE32(DC_CONTROL,
                (0x101 | ((td_u32)(dc_input->data_src - EXT_DRV_DATA_COLLECT_TRIG_TEST_MODE_BASE) <<
                EXT_DRV_DATA_COLLECT_TEST_MODE_OFFSET)));
        }
    } else {
        if (EN_TRIG_TYPE_NOISE == dc_input->triger_type) {
            /* start collect, stop automatically when length is enough.  low 16bits: valid, high 16bits: 0 */
            UAPI_REG_WRITE32(DC_CONTROL, 0x111);
        } else {
            /* start collect, stop at collect ulAfterPointlen after the trigger point */
            UAPI_REG_WRITE32(DC_CONTROL, 0x101);
        }
    }
}

EXT_PRV td_u32 dc_data_collect_wait_result(EXT_IN dc_input_para_stru *dc_input, td_u32 *will_wrie_addr,
    td_bool *over_turn)
{
    td_u32 time_out_ms = dc_input->wait_result_timeout;
    td_u32 cur_time_ms = 0;
    td_u32 temp_reg_val = 0;
    g_force_stop_dc = 0;
    for (;;) {
        UAPI_REG_READ32(DC_RX_STATUS, temp_reg_val);
        if (temp_reg_val & 0x1) {
            break;
        }

        uapi_sleep(10);
        cur_time_ms += 10;
        if (cur_time_ms >= time_out_ms || g_force_stop_dc) {
            // timeout
            UAPI_REG_WRITE32(DC_CONTROL, 0x0); // stop collecting
            g_force_stop_dc = 0;
            return EXT_ERR_TIMEOUT;
        }
    }
    g_force_stop_dc = 0;

    // fifo overflow
    UAPI_REG_READ32(DC_RX_STATUS, temp_reg_val);
    if (temp_reg_val & EXT_DRV_DATA_COLLECT_OVERFLOW) {
        return EXT_ERR_COLLECT_DATA_OVERFLOW_ERR;
    }

    // memory address to be written next
    UAPI_REG_READ32(DC_RX_CURR_ADDR, *will_wrie_addr);

    UAPI_REG_READ32(DC_RX_STATUS, temp_reg_val);
    if ((temp_reg_val & EXT_DRV_DATA_COLLECT_ADDR_CYCLE_IND) || (*will_wrie_addr == dc_input->start_addr)) {
        *over_turn = TD_TRUE;
    } else {
        *over_turn = TD_FALSE;
    }
    return EXT_ERR_SUCCESS;
}

// return collection result
EXT_PRV td_u32 dc_data_collect_get_result(EXT_IN dc_input_para_stru *dc_input, EXT_OUT ext_buf_s *s_buf,
    td_u32 will_write_addr, td_bool over_turn)
{
    if (will_write_addr < dc_input->start_addr || will_write_addr >= dc_input->end_addr) {
        return EXT_ERR_INVALID_RAM_ADDR;
    }

    // all data is valid if flipped or just full
    if (over_turn) {
        // first block:  make sure the first block has content by judging the illegal address above
        s_buf->buf_addr[0] = will_write_addr;
        s_buf->buf_size[0] = dc_input->end_addr - will_write_addr;

        // second block: fill 0 if there is no content
        if (will_write_addr == dc_input->start_addr) {
            s_buf->buf_addr[1] = 0;
            s_buf->buf_size[1] = 0;
        } else {
            s_buf->buf_addr[1] = dc_input->start_addr;
            s_buf->buf_size[1] = will_write_addr - dc_input->start_addr;
        }
    } else {
        s_buf->buf_addr[0] = dc_input->start_addr;
        s_buf->buf_size[0] = dc_input->end_addr - dc_input->start_addr;
        s_buf->buf_addr[1] = 0;
        s_buf->buf_size[1] = 0;
    }
    return EXT_ERR_SUCCESS;
}

EXT_PRV td_void dc_data_aagc_restore(td_void)
{
    diag_cmd_phy_main_param_nv_stru phy_nv;
    uapi_nv_read(DIAG_CMD_PHY_MAIN_PARAM_NV_STRU_ID, (td_pvoid)&phy_nv, sizeof(diag_cmd_phy_main_param_nv_stru));
    UAPI_REG_WRITE32(REG_DC_AGC_SET, (td_u32)phy_nv.param15); // restore operation
    UAPI_REG_WRITE32(REG_DC_AGC_VAL, (td_u32)phy_nv.param16); // restore operation
}

EXT_PRV td_void dc_data_collect_restore(EXT_IN dc_input_para_stru *dc_input) // restore
{
    dc_data_aagc_restore();
    if (dc_input->data_src != EN_TRIG_DA_12_BIT) {
        drv_set_collect_data_flag(COLLECT_DATA_DISABLE);
    }
}

td_void dc_data_collect_force_restore() // restore
{
    dc_data_aagc_restore();
    drv_set_collect_data_flag(COLLECT_DATA_DISABLE);
}

// noise collection
td_u32 dc_data_collect(EXT_IN dc_input_para_stru *dc_input, EXT_OUT ext_buf_s *s_buf, ext_drv_data_collect_step step)
{
    td_u32 ret = EXT_ERR_SUCCESS;
    td_u32 will_write_addr = 0;
    td_bool over_turn = TD_FALSE;
    dc_afe_backup afe_backup = { 0 };

    // 1.check input
    /* judge the validity of user input parameters */
    if (EXT_ERR_SUCCESS != dc_check_para(dc_input)) {
        return EXT_ERR_FAILURE;
    }
    uapi_lowpower_forbid(EXT_LOWPOWER_ID_DC, EXT_SLEEP_LIGHT);
    uapi_lowpower_forbid(EXT_LOWPOWER_ID_DC, EXT_SLEEP_DEEP);

    // ready to collect
    if (EXT_DRV_DATA_COLLECT_STEP_PREPARE & (td_u32)step) {
        dc_data_collect_prepare(dc_input, &afe_backup);
    }


    // start collection
    if (EXT_DRV_DATA_COLLECT_STEP_START & (td_u32)step) {
        (td_void)dc_data_collect_start(dc_input);
    }

    // waiting for data collection result
    if (EXT_DRV_DATA_COLLECT_STEP_GET_RESULT & (td_u32)step) {
        ret = dc_data_collect_wait_result(dc_input, &will_write_addr, &over_turn);
        if (ret != EXT_ERR_SUCCESS) {
            dc_data_collect_restore(dc_input);
            uapi_lowpower_unforbid(EXT_LOWPOWER_ID_DC, EXT_SLEEP_LIGHT, 0);
            uapi_lowpower_unforbid(EXT_LOWPOWER_ID_DC, EXT_SLEEP_DEEP, 0);
            return ret;
        }
        ret = dc_data_collect_get_result(dc_input, s_buf, will_write_addr, over_turn);
        if (ret != EXT_ERR_SUCCESS) {
            dc_data_collect_restore(dc_input);
            uapi_lowpower_unforbid(EXT_LOWPOWER_ID_DC, EXT_SLEEP_LIGHT, 0);
            uapi_lowpower_unforbid(EXT_LOWPOWER_ID_DC, EXT_SLEEP_DEEP, 0);
            return ret;
        }

        dc_data_collect_restore(dc_input);
    }

    // restore default registers and MAC configuration
    if (EXT_DRV_DATA_COLLECT_STEP_FORCE_RESTORE & (td_u32)step) {
        dc_data_collect_force_restore();
    }
    uapi_lowpower_unforbid(EXT_LOWPOWER_ID_DC, EXT_SLEEP_LIGHT, 0);
    uapi_lowpower_unforbid(EXT_LOWPOWER_ID_DC, EXT_SLEEP_DEEP, 0);
    return ret;
}
#endif
