/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Project line  :  Platform And Key Technologies Development
 * Department    :  CAD Development Department
 * Version       :  1.0
 * Date          :  2017/10/29
 * Description   :  The description of xxx project
 * Others        :  Generated automatically by nManager V4.2
 * History       :  xxx 2018/05/18 10:44:45 Create file
 */

#ifndef __STC_C_UNION_DEFINE_H__
#define __STC_C_UNION_DEFINE_H__

/* Define the union U_STC_CTRL_REG */
struct stc_ctrl_reg {
    unsigned int stc_rx_en : 1;         /* [0] */
    unsigned int stc_tx_en : 1;         /* [1] */
    unsigned int self_test_mode : 2;    /* [3:2] */
    unsigned int loop_rx_disable : 1;   /* [4] */
    unsigned int overflow_stop_en : 1;  /* [5] */
    unsigned int loop_tx_disable : 1;   /* [6] */
    unsigned int underflow_stop_en : 1; /* [7] */
    unsigned int data_width : 2;        /* [9:8] */
    unsigned int rsv_0 : 22;            /* [31:10] */
};

/* Define the union U_STC_RX_STATUS_REG */
struct stc_rx_status_reg {
    unsigned int rx_end : 1;         /* [0] */
    unsigned int overflow : 1;       /* [1] */
    unsigned int rx_single_end : 1;  /* [2] */
    unsigned int trace_end : 1;      /* [3] */
    unsigned int max_cnt_rx : 13;    /* [16:4] */
    unsigned int addr_cycle_ind : 1; /* [17] */
    unsigned int rsv_1 : 14;         /* [31:18] */
};

/* Define the union U_STC_RX_START_ADDR_REG */
struct stc_rx_start_addr_reg {
    unsigned int rx_start_addr : 32; /* [31:0] */
};

/* Define the union U_STC_RX_END_ADDR_REG */
struct stc_rx_end_addr_reg {
    unsigned int rx_end_addr : 32; /* [31:0] */
};

/* Define the union U_STC_RX_CURR_ADDR_REG */
struct stc_rx_curr_addr_reg {
    unsigned int current_rx_address : 32; /* [31:0] */
};

/* Define the union U_STC_TX_STATUS_REG */
struct stc_tx_status_reg {
    unsigned int tx_end : 1;        /* [0] */
    unsigned int underflow : 1;     /* [1] */
    unsigned int tx_single_end : 1; /* [2] */
    unsigned int tx_test_error : 1; /* [3] */
    unsigned int rsv_2 : 28;        /* [31:4] */
};

/* Define the union U_STC_TX_START_ADDR_REG */
struct stc_tx_start_addr_reg {
    unsigned int tx_start_addr : 32; /* [31:0] */
};

/* Define the union U_STC_TX_END_ADDR_REG */
struct stc_tx_end_addr_reg {
    unsigned int tx_end_addr : 32; /* [31:0] */
};

/* Define the union U_STC_TX_CURR_ADDR_REG */
struct stc_tx_curr_addr_reg {
    unsigned int current_tx_address : 32; /* [31:0] */
};

/* Define the union U_STC_DELAY_COUNT_REG */
struct stc_delay_count_reg {
    unsigned int delay_count : 32; /* [31:0] */
};

/* Define the union U_STC_CAP_CTRL_REG */
struct stc_cap_ctrl_reg {
    unsigned int laddr : 8;           /* [7:0] */
    unsigned int gaddr : 6;           /* [13:8] */
    unsigned int rsv_3 : 2;           /* [15:14] */
    unsigned int trigger_mode : 2;    /* [17:16] */
    unsigned int trigger_type : 2;    /* [19:18] */
    unsigned int trace_ctrl : 1;      /* [20] */
    unsigned int valid_ctrl : 1;      /* [21] */
    unsigned int rsv_4 : 2;           /* [23:22] */
    unsigned int stc_trigger_sel : 8; /* [31:24] */
};

/* Define the union U_STC_TRIG_MASK_REG */
struct stc_trig_mask_reg {
    unsigned int trigger_mask : 32; /* [31:0] */
};

/* Define the union U_STC_TRIG_DATA_REG */
struct stc_trig_data_reg {
    unsigned int trigger_data : 32; /* [31:0] */
};

/* Define the union U_STC_TRIG_ADDR */
struct stc_trig_addr {
    unsigned int trigger_curr_addr : 32; /* [31:0] */
};

/* Define the union U_PKT_GAP_THRES */
struct pkt_gap_thres {
    unsigned int pkt_gap_thres : 32; /* [31:0] */
};

/* Define the union U_HADDRM */
struct haddrm {
    unsigned int haddrm : 32; /* [31:0] */
};

/* Define the union U_DFX_REG_CTRL */
struct dfx_reg_ctrl {
    unsigned int cap_en : 1;        /* [0] */
    unsigned int dfx_self_test : 1; /* [1] */
    unsigned int rsv_5 : 30;        /* [31:2] */
};

/* Define the union U_DFX_REG_CAP_CTRL */
struct dfx_reg_cap_ctrl {
    unsigned int dfx_laddr : 2;        /* [1:0] */
    unsigned int dfx_trigger_type : 2; /* [3:2] */
    unsigned int dfx_trigger_sel : 3;  /* [6:4] */
    unsigned int rsv_6 : 25;           /* [31:7] */
};

/* Define the union U_DFX_REG_DELAY_COUNT */
struct dfx_reg_delay_count {
    unsigned int dfx_delay_count : 32; /* [31:0] */
};

/* Define the union U_DFX_REG_PREAM_PAR */
struct dfx_reg_pream_par {
    unsigned int trig_pream_par : 8; /* [7:0] */
    unsigned int dfx_overflow : 1;   /* [8] */
    unsigned int dfx_trace_end : 1;  /* [9] */
    unsigned int rsv_7 : 22;         /* [31:10] */
};

#endif // __STC_C_UNION_DEFINE_H__
