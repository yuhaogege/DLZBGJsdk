/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: dfx_sys dsid sdm.
 * Author: CompanyName
 * Create: 2021-1-27
 */

#ifndef DFX_SYS_DSID_SDM_H
#define DFX_SYS_DSID_SDM_H

#include "soc_phy_nv.h"
#include "soc_config.h"
#include "dfx_sys_sdm.h"

/* storage access id */
typedef enum {
    SDM_FLASH = 0,
    SDM_RAM
} ext_sdm_sa_id;

typedef struct {
    ext_sdm_sa_id said;  /* storage access id */
    ext_sdm_cmd_id scid; /* sdm command id. */
    td_u32 offset;      /* reserved */
    td_u32 num;
} ext_dsid_sdm_dump;

typedef struct {
    td_u32 id;
    td_u32 cnt;
    td_u32 local_time;
    td_u32 net_time;
    td_u32 status;
} ext_dsid_sdm_hrd;

typedef struct {
    td_u32 save_time;             /* deprecated */
    td_u32 flash_size;            /* flash size */
    td_u32 flash_addr;            /* flash basic addr */
    td_u32 sdm_save_buffer;       /* periodic storage structure pointer */
    td_u32 magic_num;             /* content magic word */
    td_u16 sys_cause;             /* deprecated */
    td_u16 sdm_content_arrtibute; /* sdm_content_attribute_e */
    td_u16 sdm_save_buffer_size;  /* periodic storage structure size */
    td_u16 per_block_size;        /* ulsdmperiodsavebuffersize or ulsdmperiodsavebuffersize+magic+logic */

    td_u16 current_write_block_idx; /* record the flash offset block number to be written. */
    td_u16 current_logic_block_idx; /* record the index number to be written. */
    td_u16 max_write_block_idx;     /* free block */
    td_u16 write_fail_times;

    td_u8 sdm_content_type; /* sdm_content_type_e content id */
    td_u8 sdm_blk_id;       /* share flashid, valid for shared flash */
    /*
     * a value of 1 indicates that the semaphore was created successfully,
     * and it is meaningful for partitions that support synchronous operations.
     */
    td_u8 created_sem : 1;
    td_u8 inited : 1; /* indicates that initialization is complete */
    td_u8 cur_stat_reserver : 6;
    td_u8 reserver_2;

    td_u32 sem_handle;    /* semaphore */
    td_u32 sem_wait_time; /* get semaphore timeout ms */
} sdm_eng_ctx;
typedef struct {
    td_u32 write_fail;
    td_u32 read_fail;
    td_u32 malloc_fail;
    td_u32 msg_send_fail_times;
    td_u32 msg_send_sucess_times;
    td_u32 msg_receive_times;
} sdm_eng_dsid_msg;
typedef struct {
    td_u32 tx_send_sof_cnt;    /* the number of sof packets sent by the sender */
    td_u32 tx_send_sack_cnt;   /* the number of sack messages sent by the sender */
    td_u32 tx_send_beacon_cnt; /* the number of beacon packets sent by the sender */
    /* total number of discarded packets due to reassembly of the reassembly module */
    td_u16 rsb_reassemble_fail_cnt;
    td_u16 rx_receive_coordinate_cnt; /* receiver receives the total number of coordinates */

    td_u32 rx_receive_beacon_cnt;        /* receiver receives the total number of beacons */
    td_u32 rx_receive_sof_cnt;           /* receiver receives the total number of sof */
    td_u32 rx_receive_sack_cnt;          /* receiver receives the total number of sacks */
    td_u32 rx_get_macbuffer_success_cnt; /* receiver gets the total number of cache successes */
    td_u32 rx_get_macbuffer_fail_cnt;    /* total number of cache failures received by the receiver */

    td_u16 rx_rcv_not_finished_cnt; /* receiver receives the total number of outstanding release buffers */
    td_u16 rx_discard_packet_cnt;   /* total number of packets discarded by the receiver */

    td_u32 rsb_receive_cnt;            /* total number of messages processed by the reassembly module */
    td_u32 rsb_deliver_cnt;            /* total number of messages distributed by the reassembly module */
    td_u32 cvg_receive_delivery_cnt;   /* total number of packets received by the aggregation module */
    td_u32 cvg_discard_unicast_cnt;    /* total number of unicast packets discarded by the aggregation module */
    td_u32 cvg_transmit_unicast_cnt;   /* total number of unicast packets forwarded by the aggregation module */
    td_u32 cvg_discard_broadcast_cnt;  /* total number of broadcast packets discarded by the aggregation module */
    td_u32 cvg_transmit_broadcast_cnt; /* total number of broadcast packets forwarded by the aggregation module */
    td_u32 cvg_deliver_app_cnt;        /* total number of app messages distributed by the aggregation module */
    td_u32 cvg_deliver_app_fail_cnt;   /* total number of failed app messages distributed by the aggregation module */

    td_u32 cvg_deliver_nm_cnt;      /* total number of nm packets distributed by the aggregation module */
    td_u32 cvg_deliver_nm_fail_cnt; /* total number of failed nm packet transmissions by the aggregation module */
    /* the aggregation module distributes the total number of unknown types of packets. */
    td_u32 cvg_deliver_invalid_type_cnt;
    td_u32 cvg_receive_transmit_cnt;      /* total number of packets received by the aggregation module */
    td_u32 cvg_receive_transmit_fail_cnt; /* total number of failed packets received by the aggregation module */
    td_u32 cvg_receive_nm_cnt;            /* total number of nm packets received by the aggregation module */
    td_u32 cvg_receive_app_cnt;           /* total number of app messages received by the aggregation module */
    /* the total number of failed processing errors received by the aggregation */
    td_u16 cvg_receive_app_no_buffer_cnt;
    /* module receiving app messages due to insufficient cache
     * total number of queue packet overflow failures received by the aggregation module */
    td_u16 cvg_receive_app_queue_overflow_cnt;

    td_u32 cvg_handle_cnt;          /* total number of packets processed by the aggregation module */
    td_u32 cvg_handle_transmit_cnt; /* the total number of packets forwarded by the aggregation module */
    td_u32 cvg_handle_nm_cnt;       /* total number of nm packets processed by the aggregation module */

    td_u16 cvg_handle_app_cnt; /* total number of app messages processed by the aggregation module */
    /* the aggregation module processes the total number of unknown types of packets. */
    td_u16 cvg_handle_invalid_cnt;

    td_u32 cvg_handle_fail_cnt;                /* total number of failed packets processed by the aggregation module */
    td_u32 cvg_handle_transmit_route_fail_cnt; /* the total number of failed routing attempts by the */
    /* aggregation module to process forwarded packets */
    td_u32 cvg_handle_nm_route_fail_cnt;  /* total number of route failures in the aggregation module nm packet */
    td_u32 cvg_handle_app_route_fail_cnt; /* total number of route failures in the aggregation module app packet */
    td_u32 cvg_segment_fail_cnt;          /* total number of aggregation module segmentation processing failures */

    td_u16 pad; /* total number of failures of the aggregation module segment to generate mac frames */
    /* total number of failed segmentation split macs in the aggregation module */
    td_u16 cvg_segment_split_mac_fail_cnt;

    td_u32 cvg_segment_set_fc_fail_cnt;    /* aggregate module segmentation sets the total number of fc failures */
    td_u32 cvg_segment_queue_overflow_cnt; /* total number of aggregate module segment queue full failures */
    td_u32 segment_sof_cnt;                /* total number of packets received by the segmentation module */
    /* the total number of discarded packets successfully sent by the segmentation module */
    td_u32 sgm_mpdu_send_success_cnt;
    /* total number of discarded packets by the segmentation module due to transmission failure */
    td_u32 sgm_mpdu_send_fail_cnt;
    /* the total number of discarded packets by the segmentation module due to packet timeout */
    td_u32 sgm_mpdu_send_timeout_cnt;
    /* total number of discarded packets by the segmentation module due to system soft restart */
    td_u32 sgm_mpdu_reset_discard_cnt;
} ext_mac_interface_statistics;

typedef struct {
    /* Channel statistics */
    td_u32 first_notify_time;    /* the first notification time stamp after the network is completed */
    td_u32 last_true_time;       /* the last time the channel changed to the available message timestamp */
    td_u32 last_false_time;      /* the last time the channel changed to the unavailable message timestamp */
    td_u32 chl_status;           /* current channel status */
    td_u16 chl_change_true;      /* channel availability */
    td_u16 chl_change_false;     /* channel unavailable times */
    td_u32 uart_rx_total_len[2]; /* serial port receive bytes (send message/process message), 2 count */
    /* the serial port receives the full number of frames (sends the message/process message), 2 count */
    td_u16 uart_rx_ok_num[2];    /* 2 count */
    td_u32 uart_rx_last_time[2]; /* last time the serial port received (send message/process message), 2 count */
    td_u32 uart_tx_total_len[2]; /* serial port number of bytes sent (send message/process message), 2 count */
    /* the number of successful frames sent by the serial port (sending message/processing message), 2 count */
    td_u16 uart_tx_ok_num[2];    /* 2 count */
    td_u32 uart_tx_last_time[2]; /* last time the serial port was sent (send message/process message), 2 count */
    /* number of failed serial port transmissions (sending message/processing message), 2 count */
    td_u16 uart_tx_fail_num[2]; /* 2 count */
    /* the reason why the serial port failed to send the last time (send message/process message), 2 count */
    td_u32 uart_tx_err_cause[2]; /* 2 count */
    td_u16 plc_rx_total_num[2];  /* plc receives the number of frames (sends a message/process message), 2 count */
    td_u32 plc_rx_last_time[2];  /* the last time the plc received (sending a message/processing message), 2 count */
    td_u16 plc_rx_last_id[2];    /* the last time the plc received the id (send message/process message), 2 count */
    /* the last time the plc received the packet length (send message/process message), 2 count */
    td_u32 plc_rx_last_len[2]; /* 2 count */
    /* the number of frames sent by the plc (sending a message/processing message), 2 count */
    td_u16 plc_tx_total_num[2];
    td_u32 plc_tx_last_time[2]; /* the last time the plc sent (send message/process message), 2 count */
    td_u16 plc_tx_last_id[2];   /* the last time the plc sent an id (send message/process message), 2 count */
    /* the last time the plc sent the packet length (send message/process message), 2 count */
    td_u32 plc_tx_last_len[2];
    td_u16 plc_tx_fail_num[2]; /* plc failed to send (send message/process message), 2 count */
    td_u32 plc_tx_fail_len[2]; /* plc sends the last failed packet length (send message/process message), 2 count */
    /* plc sends the last reason for failure (sending message/processing message), 2 count */
    td_u32 plc_tx_err_cause[2];
} ext_mrs_interface_statistics; /* mrs external interface statistics */


/* End-to-end interface level including MRS-IP-MAC-PHY */

typedef struct {
    ext_dsid_sdm_hrd hrd;

    /* * ip packet received, segment count, sent to phy count */
    ext_mac_interface_statistics mac_hl_info; /* mac first-level statistics, focusing on access */
} ext_dsid_sdm_chl;

typedef struct {
    ext_dsid_sdm_hrd hrd;
    td_u32 rx_fc_cnt;
    td_u32 rx_fc_crc_success_cnt;
    td_u32 rx_fc_crc_err_cnt;
    td_u32 rx_false_frame;
    td_u32 rx_beacon_fc_recv_cnt;
    td_u32 rx_beacon_fc_discard_cnt;
    td_u32 rx_beacon_pb_crc_err_cnt;
    td_u32 rx_beacon_pb_crc_success_cnt;
    td_u32 rx_sof_broadcast_cnt;
    td_u32 rx_sof_unicast_cnt;
    td_u32 rx_sof_resend_cnt;
    td_u32 rx_sof_136_pb_crc_err_cnt;
    td_u32 rx_sof_136_pb_crc_success_cnt;
} ext_dsid_sdm_rx;

typedef struct {
    ext_dsid_sdm_hrd hrd;

    td_u32 tx_sof_unicast_cnt;
    td_u32 tx_sof_broadcast_cnt;
    td_u32 tx_sof_resend_cnt;
    td_u32 tx_sof_136_pb_cnt;
    td_u32 tx_sof_520_pb_cnt;
    td_u32 tx_tdma_refresh_evt_cnt;
    td_u32 tx_mpdu_queued_in_evt_cnt;
    td_u32 tx_get_slot_fail_cnt;
    td_u32 tx_start_hr_timer_fail_cnt;

    td_u32 tx_get_all_msdu_cnt;            /* total number of messages received by the sender */
    td_u32 sof_wait_sack_send_success;     /* wait for sack sof to send successfully */
    td_u32 sof_not_wait_sack_send_success; /* no need to wait for the number of successful sack packets sent */
    /* the number of retransmissions of the message, excluding fixed retransmissions */
    td_u32 sof_resend_exclude_fix_resend;

    td_u8 switch_to_rf_by_blink_check_cnt; /* number of times due to blind detection switching to rf mode */
    td_u8 network_mode_switch_cmd_cnt;     /* network mode switching command switching times */
    td_u8 pad[2];                          /* pad 2 byte */
} ext_dsid_sdm_tx;

typedef struct {
    ext_dsid_sdm_hrd hrd;

    td_u32 bcn_slot_table_update_cnt;
    td_u32 bcn_last_update_slot_table_time;
    td_u32 bcn_last_update_slot_table_bpc;
    td_u32 bcn_last_rcv_beacon_time;
    td_u32 bcn_last_rcv_beacon_bpc;
    td_u32 bcn_discard_beacon_slot_cnt;
    td_u32 bcn_discard_bind_csma_slot_cnt;
    td_u32 bcn_discard_csma_slot_cnt;
    td_u32 bcn_discard_tdma_slot_cnt;
    td_u32 bcn_give_beacon_slot_cnt;
    td_u32 bcn_give_bind_csma_slot_cnt;
    td_u32 bcn_give_csma_slot_cnt;
    td_u32 bcn_give_tdma_slot_cnt;
    td_u32 bcn_get_repeat_slot_cnt;
    td_u32 bcn_give_oneself_slot_cnt;
    td_u32 tx_get_slot_cnt;
    td_u32 tx_get_slot_fail_cnt;
    td_u32 phs_evaluate_a_cnt;
    td_u32 phs_evaluate_b_cnt;
    td_u32 phs_evaluate_c_cnt;
} ext_dsid_sdm_bcn;

typedef struct _dfx_switch_freq_info_ {
    td_u16 freq_total_cnt;
    td_u16 reason;
    td_u32 last_time; /* Duration of this band */
} dfx_switch_freq_info;

typedef struct _dfx_switch_snid_info_ {
    td_u16 snid_total_cnt;
    td_u16 status;
    td_u32 fc_success_cnt;
    td_u32 beacon_pb_success_cnt;
} dfx_switch_snid_info;

typedef struct _dfx_rt_info_ {
    td_u16 rt_discover_station_cnt;        /* number of sites found */
    td_u16 rt_near_station_cnt;            /* find the number of sites closer to cco */
    td_u16 rt_upper_comm_rate_station_cnt; /* number of communication rate upper limit sites */
    /* number of intermediate stations with upper and lower communication rates */
    td_u16 rt_middle_comm_rate_station_cnt;
    td_u16 rt_lower_comm_rate_station_cnt; /* number of stations with lower communication rate */
    td_u16 rt_rcv_discover_pk_cnt;         /* receive the number of discovery lists (1 window) */
    td_u16 rt_rcv_bea_cnt;                 /* number of received beacons (1 window) */
    td_u8 rt_proxy_up_comm_rate;           /* uplink communication rate with proxy site */
    td_u8 rt_proxy_down_comm_rate;         /* downstream communication rate with proxy site */
    td_u16 rt_rcv_proxy_beacon_cnt;        /* receive proxy station beacon frames (1 window) */
    td_u16 rt_rcv_proxy_discover_pk_cnt;   /* number of receiving proxy site discovery lists (1 window) */
    td_u8 proxy_mac[EXT_PLC_MAC_ADDR_LEN];
    td_u8 chose_proxy_mac[EXT_PLC_MAC_ADDR_LEN];
    td_u16 change_proxy_reason;
    td_bool proxy_is_static;
    td_u8 rsvd;
} dfx_rt_info;

#define DFX_RT_INDEX_MAX 10

/* Proxy change reason bit */
enum {
    PROXY_RATE_IS_LOW = 1 << 0,
    PROXY_RATE_IS_MIDDLE = 1 << 1,
    PROXY_RATE_IS_UPPER = 1 << 2,
    PROXY_IS_FAR_TO_CCO = 1 << 3,
    MULTI_PROXY_COMMRATE_BAD = 1 << 4,
    HALF_WINDOW_COMM_BAD = 1 << 5,
    NEW_PROXY_LEVEL_IS_SMALL = 1 << 6,
    NEW_PROXY_IS_STA = 1 << 7,
    NEW_PROXY_RATE_IS_BIG = 1 << 8,
    CHANGE_TO_STATIC_PROXY = 1 << 9,
};

typedef struct {
    ext_dsid_sdm_hrd hrd;

    td_u32 switch_freq_last_systime;                     /* last cut band system time */
    dfx_switch_freq_info switch_freq_info[FREQ_MAX_CNT]; /* band switching record */

    td_u8 freq_now;                  /* current band */
    td_u8 freq_last;                 /* last band */
    td_u16 last_change_reason;       /* last time the band was switched */
    td_u32 switch_snid_last_systime; /* last time switching snid system time */

    dfx_switch_snid_info switch_snid_info[15]; /* snid switch record 15 count */

    td_u32 join_first_systime; /* first association request system time */
    td_u32 join_last_systime;  /* network access time */

    td_u16 not_bigger_enough_bpc_cnt; /* no larger bpc to associate requests */
    td_u16 rcv_bea_period_count;      /* receive beacons for several beacon periods */
    td_u16 chose_proxy_fail_cnt;      /* select agent failures */
    td_u16 assoc_req_cnt;             /* number of association requests */
    td_u16 cco_reject_cnt;            /* number of cco rejections */
    td_u16 cco_reject_reason;         /* reason for cco rejection */

    td_u32 mac_reset_cnt; /* mac soft restart times */

    td_u16 join_not_white_cnt;      /* not whitelisted requests */
    td_u16 join_level_overflow_cnt; /* too many times */
    td_u16 join_proxy_overflow_cnt; /* too many agents */
    td_u16 join_sta_overflow_cnt;   /* too many subsites */
    td_u16 join_total_overflow_cnt; /* too many totals */
    td_u16 join_repeat_mac_cnt;     /* repeat mac times */
    td_u16 join_illege_proxy_cnt;   /* the number of times the agent does not exist */
    td_u16 join_end_sequence_err;   /* end_sequence error count */

    td_u32 cco_recv_req_cnt; /* total number of cco requests received */

    diag_cmd_soft_reset soft_reset;

    td_u32 nm_handle_assoc_req_cnt;
    td_u32 nm_handle_assoc_cnf_cnt;
    td_u32 nm_create_assoc_ind_01_cnt;
    td_u32 nm_create_change_proxy_cnf_cnt;
    td_u32 nm_handle_change_proxy_cnf_cnt;
    td_u32 dfx_ndm_handle_dbk_cmd_callback_cnt;
    td_u32 dfx_ndm_pipe_output_cnt;
    td_u32 dfx_ndm_packet_frame_copy_payload_list_cnt;
    td_u32 dfx_ndm_handle_packet_cnt;
    td_u32 dfx_ndm_pipe_send_ack_back_cnt;
    td_u32 dfx_ndm_pipe_fragment_output_cnt;
    td_u32 dfx_ndm_fill_reply_payload_cnt;
    td_u32 nm_write_nv_fail;

    td_u8 cco_mac[EXT_PLC_MAC_ADDR_LEN]; /* lock the network's cco mac address */
    td_u8 lock_status;                  /* lock status */
    td_u8 lock_status_identify;         /* lock status under the station identification function */

    td_u32 lock_begin_time; /* lock start time */
    td_u32 lock_duration;   /* lock duration */
    td_u32 reject_time;     /* lock network rejection time */

    td_u16 lock_cnt;                  /* number of locks */
    td_u16 reject_cnt;                /* lock network rejections */
    td_u16 lock_timeout_cnt;          /* lock timeout at this time */
    td_u16 lock_fail_by_not_join_cnt; /* failed to join the network lock */
    td_u16 send_msg_fail_cnt;         /* message failed to be sent */
    td_u16 unlock_cnt_by_identify;    /* number of unlocking times for station identification (measurement) */

    td_u8 lock_freq_identify;                    /* lock network band number */
    td_u8 lock_snid_identify;                    /* lock the network number */
    td_u8 cco_mac_identify[EXT_PLC_MAC_ADDR_LEN]; /* the zone identifies the cco mac address of the locked network */

    td_u32 lock_begin_time_by_identify;        /* lock start time, unit s */
    td_u32 lock_freqsnid_duration_by_identify; /* lock duration, unit s */
    td_u32 lock_ccomac_duration_by_identify;   /* lock cco mac duration, unit s */
    td_u32 unlock_time_by_identify;            /* unlock time (measurement) */
    /* the zone identifies the locked band and the number of network numbers (measurement) */
    td_u16 lock_freqsnid_cnt_by_identify;
    td_u16 lock_ccomac_cnt_by_identify; /* station identification lock cco mac times (measurement) */

    td_u32 lock_freq : 8;  /* lock the band of the network */
    td_u32 lock_snid : 24; /* lock the snid of the network */
} ext_dsid_sdm_nm;

typedef struct {
    ext_dsid_sdm_hrd hrd;

    dfx_rt_info rt_dfx_info[DFX_RT_INDEX_MAX];
    td_u8 rt_dfx_index;
    td_u8 rsvd2[2]; /* pad 2 byte */
    td_u8 my_static_proxy_exist;
    td_u8 static_proxy_mac[EXT_PLC_MAC_ADDR_LEN];
    td_u16 rt_fast_proxy_change_cnt;   /* quickly trigger route change times */
    td_u16 rt_window_proxy_change_cnt; /* total window changes */
    td_u16 lock_static_proxy_cnt;
    td_u16 unlock_static_proxy_cnt;
    td_u16 choose_static_proxy_fail_cnt;
} ext_dsid_sdm_rt;

typedef struct {
    ext_dsid_sdm_hrd hrd;
} ext_dsid_sdm_mr;

typedef struct {
    ext_dsid_sdm_hrd hrd;
} ext_dsid_sdm_boot;

typedef struct {
    ext_dsid_sdm_hrd hrd;
} ext_dsid_sdm_evt_mac;

typedef struct {
    ext_dsid_sdm_hrd hrd;
} ext_dsid_sdm_evt_mr;

#endif /* DFX_SYS_DSID_SDM_H */

