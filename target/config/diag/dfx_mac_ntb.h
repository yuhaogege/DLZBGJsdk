/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: DFX interfaces of ntb
 */
#ifndef _DFX_MAC_NTB_H
#define _DFX_MAC_NTB_H

typedef struct {
    td_u32 snid : 24;                         /* Network ID */
    td_u32 bt : 2;                            /* Indicates whether the ID is a proxy ID. */
    td_u32 freq : 5;                          /* Frequency band */
    td_u32 is_rcv_pb : 1;                     /* Whether PB is received */
    td_u16 tei : 12;                          /* Device identifier */
    td_u16 level : 4;                         /* Level */
    td_s16 snr;                               /* Average SNR */
    td_u16 sync_status : 3;                   /* Synchronization status of the synchronization object */
    td_u16 freqerr_excpt_cnt_with_mac : 4;    /* frequency offset abnormality count */
    td_u16 freqerr_excpt_cnt_without_mac : 4; /* Number of frequency offset exceptions when there is no CCO_MAC */
    td_u16 pad : 5;                           /* Reserved */
    td_s16 freqerr_output;                    /* Configure the frequency offset. */
    td_u32 last_rcv_timestamp;                /* Latest time when a packet is received */
} diag_cmd_query_ntb_candi_sta_stru;

typedef struct {
    td_u32 is_config_freqerr : 1; /* Check whether frequency offset is configured. */
    td_u32 sync_mode : 2;         /* Synchronization mode */
    td_u32 freq : 5;              /* Receive frequency band */
    td_u32 sync_snid : 24;        /* synchronization network ID */
    td_u32 sync_target_idx : 2;   /* Index of the synchronization object.
                                   * For details about the value, see ntb_sync_target_idx_en
                                   */
    td_u32 pad : 30;              /* Reserved */
    diag_cmd_query_ntb_candi_sta_stru candidate_sta[DFX_NTB_SYNC_STA_CNT_MAX];
} diag_cmd_query_ntb_statistic;
#define NTB_RESET_REASON_CNT_MAX 10
typedef struct {
    td_u8 reset_reason; /* Reset reason */
    td_u8 pad1;         /* Reserved */
    td_s16 freq_err;    /* Frequency offset */
    td_u32 snid;        /* Network ID */
    td_u32 timestamp;   /* Reset time (unit: ms) */
} diag_cmd_query_ntb_reset_reason_entry_stru;
typedef struct {
    td_u32 reset_cnt;
    diag_cmd_query_ntb_reset_reason_entry_stru entry[NTB_RESET_REASON_CNT_MAX];
} diag_cmd_query_ntb_reset_reason;

#define NTB_MAX_FREQERR_CNT 36
typedef struct {
    td_u16 freqerr[NTB_MAX_FREQERR_CNT]; /* Number of frequency deviations within the range */
} diag_cmd_query_ntb_freqerr_stat;
#endif