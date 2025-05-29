/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: dfx_sys dsid app def.
 * Author: CompanyName
 * Create: 2021-1-27
 */

#ifndef DFX_SYS_DSID_APP_DEF_H
#define DFX_SYS_DSID_APP_DEF_H

/* ***************************************************************************** */
/* MRS TF system message definition */
/* ***************************************************************************** */
#define EXT_DSID_APP_MRS_TF_STA_PLC_SEND 0xA601
#define EXT_DSID_APP_MRS_TF_STA_PLC_RECV 0xA602
#define EXT_DSID_APP_MRS_TF_CCO_PLC_START_SEND 0xA603
#define EXT_DSID_APP_MRS_TF_CCO_PLC_QUERY_SEND 0xA604
#define EXT_DSID_APP_MRS_TF_CCO_PLC_STOP_SEND 0xA605
#define EXT_DSID_APP_MRS_TF_CCO_PLC_RECV 0xA606
#define EXT_DSID_APP_MRS_TF_CCO_UART_SEND 0xA607
/* ***************************************************************************** */
/* Structure definition */
/* ***************************************************************************** */
#define METER_ADDR_LEN 6  /* Table address length */
#define MAX_METER_NUM 32  /* The maximum number of table addresses */
#define MAX_QUEUE_NUM 3   /* The maximum number of queue elements */
#define MAX_BUF_LEN 64    /* Maximum buffer length */
#define MAX_LAST_NUM 16   /* Maximum number of history */
#define MAX_LAST_NUM_1 8  /* Maximum number of history 1 */
#define MAX_LAST_NUM_2 4  /* Maximum number of history 2 */
#define MAX_RM_POOL_NUM 2 /* Maximum number of table buffer pools */

typedef struct {
    td_u8 addr[METER_ADDR_LEN]; /* Table address */
} meter_addr;                   /* Table address */

typedef struct {
    td_u32 rx_total_len; /* total number of bytes received */
    td_u32 rx_last_time; /* last received time */
    td_u32 rx_ok_len;    /* received the correct number of bytes (parsed to full frame) */
    td_u32 rx_err_len;   /* number of bytes received (the number of bytes discarded when parsing a full frame failed) */
    td_u32 rx_ok_num;    /* number of complete frames received */
    td_u32 rx_err_num;   /* number of error frames received */
    td_u32 rx_err_cause; /* receive the last error reason */
    td_u32 rx_buf_tail;  /* end of buffer content */
    /* last received content (loop override -rx_buf_tail is the tail of the current overlay) */
    td_u8 rx_buf[MAX_BUF_LEN];
    td_u32 tx_total_len; /* total number of bytes sent */
    td_u32 tx_last_time; /* last time sent */
    td_u32 tx_ok_len;    /* number of bytes sent successfully */
    /* the number of bytes that failed to be sent (status after calling the dms interface to return a failure) */
    td_u32 tx_fail_len;
    td_u32 tx_total_num; /* total number of transmitted frames */
    td_u32 tx_ok_num;    /* number of frames successfully sent */
    /* the number of frames that failed to be sent (status after calling the dms interface to return a failure) */
    td_u32 tx_fail_num;
    td_u32 tx_err_cause;       /* the reason for the last failed transmission */
    td_u32 tx_last_len;        /* length of data sent in the previous frame */
    td_u8 tx_buf[MAX_BUF_LEN]; /* data content sent in the previous frame */
} uart_chl_inf_s;              /* uart channel statistics */

#endif

