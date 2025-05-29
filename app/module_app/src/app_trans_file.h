/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: Transmit file process.
 */

#ifndef APP_TRANS_FILE_H
#define APP_TRANS_FILE_H

#include "app_proto_proc.h"
#include "soc_mdm_msg.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TRANS_FILE_STAGE_IDLE  0x00
#define TRANS_FILE_STAGE_LOCAL 0x01
#define TRANS_FILE_STAGE_UPG   0x02
#define UPG_STOP_ENABLE        1
#define UPG_STOP_DISABLE       0
#define UPG_MSG_SEND_CNT       2
#define UPG_MSG_PARAM_INDEX0   0
#define UPG_MSG_PARAM_INDEX1   1
#define UPG_MSG_PARAM_INDEX2   2
#define UPG_MSG_PARAM_INDEX3   3
#define UPG_PAD_LEN1           2

typedef struct {
    td_u16 success;  /* Indicates the total number of upgrade files that have been successfully uploaded. */
    td_u16 complete; /* Indicates the total number of upgrade files that are successfully uploaded. */
    td_u16 failed;   /* Total number of upgrade failures. */
    td_u16 process;  /* Indicates the total number of upgrade files that are being transferred. */
} trans_file_stat;

typedef struct {
    td_u8 file_attr; /* File attribute */
    td_u8 stage;     /* Upgrade Phase */
    td_u8 clear_flag : 1;
    td_u8 clear_resp : 1;
    td_u8 allow_stop : 1;
    td_u8 wait_chl : 1;
    td_u8 result : 4;
    td_u8 stop_times;

    td_u16 seq;
    td_u16 seg_num;

    td_u16 seg_total; /* Total number of segments */
    td_u16 seg_size;  /* Part size */

    td_u32 file_len;  /* File length */
    td_u32 file_crc;  /* File CRC check */
    td_u32 timeout;   /* Upgrade Timeout Interval */

    td_u8 status_msg_cnt;
    td_u8 retry;
    td_u8 rsv[UPG_PAD_LEN1];

    trans_file_stat stat;
} trans_file_context;

trans_file_context *get_trans_file_context(td_void);
td_u32 trans_file_init(td_void);
td_u32 trans_file_clear(td_void);
td_u32 trans_file_start(EXT_CONST proto_start_trans_file_req *req);
td_u32 trans_file_data(EXT_CONST proto_trans_file_data_req *req);
td_void trans_file_set_seq(td_u16 seq);
td_void trans_file_set_state(td_u8 state);
td_void trans_file_on_timer_stop_retry(td_void);
td_void trans_file_on_timer_frame(td_void);
td_void trans_file_on_timer_status_query(td_void);
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
td_void trans_file_poweron_active_query(td_void);
td_void trans_file_on_timer_query_active(td_void);
td_void trans_file_on_timer_plc_available(td_void);
td_void trans_file_on_plc_chl_available(td_void);
#endif
td_void trans_file_on_timer_query_info(td_void);
td_u32 app_on_message_trans_file(EXT_CONST ext_sys_queue_msg *msg);

#ifdef __cplusplus
}
#endif
#endif