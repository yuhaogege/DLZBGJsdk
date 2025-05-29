/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: dfx_sys dsid.
 * Author: CompanyName
 * Create: 2021-1-27
 */

#ifndef __DFX_SYS_DSID_H__
#define __DFX_SYS_DSID_H__

#define EXT_DSID_MIN 0xA000
#define EXT_DSID_MAX 0xF000
#define EXT_STAT_ID_NUM_MAX (EXT_DSID_MAX - EXT_DFX_STAT_ID_BASE_SYS)

/* ************************************************ */
/* SAL statistic definition */
/* ************************************************ */
/* range [A000, A050) */
/* ************************************************ */
/* ************************************************ */
/* DRV statistic definition */
/* ************************************************ */
/* range [0xA050, 0xA150) */
/* ************************************************ */
#define EXT_DSID_DRV_XXX 0xA050        /* EXT_DSID_DRV_XXX_S */
#define EXT_DSID_DRV_FLASH_INFO 0xA051 /* FLASH Statistics */
#define EXT_DSID_DRV_UART0_INFO 0xA052 /* UART0 statistics */
#define EXT_DSID_DRV_UART1_INFO 0xA053 /* UART1 Statistics */
#define EXT_DSID_DRV_SPI_INFO 0xA054   /* SPI Statistics */
#define EXT_DSID_DRV_IR_INFO 0xA055    /* IR statistics */
#define EXT_DSID_DRV_HRT_INFO 0xA057   /* HRT Statistics */
#define EXT_DSID_DRV_ETH_INFO 0xA058   /* ETH Statistics */
#define EXT_DSID_DRV_RF_ERR 0xA059     /* RF GPIO SPI Statistics */

/* Structure definition */
typedef struct {
    ext_obj64 obj_cnt;
} ext_dsid_drv_xxx;
typedef struct sfc_op {
    td_u8 uciftype;
    td_u8 uccmd;
    td_u8 ucdummy;
    td_u8 pad;
    td_u32 ulsize;
    td_u32 ulclock;
} sfc_operation;
typedef struct sfc_dfx_info {
    td_u32 pname;
    td_u32 ulchipselect;
    td_u32 ulchipsize;
    td_u32 ulerasesize;
    td_u32 piobase;
    td_u32 uladdrcycle;
    sfc_operation read[1];
    sfc_operation write[1];
    sfc_operation erase[8]; /* 8 count */
} ext_dsid_drv_flash_info;

typedef struct {
    td_u32 read;
    td_u32 write;
    td_u32 int_read;
    td_u32 int_write;
    td_u32 err_overrun;
    td_u32 err_parity;
    td_u32 err_frame;
    td_u32 err_break;
    td_u32 err_busy;
    td_u32 tx_time_us;             /* system time value when sending */
    td_u32 tx_timer_us;            /* Set value of the timer when sending */
    td_u32 tx2rx_max_us;           /* The longest time to send and receive */
    td_u32 tx2rx_max_us_timestamp; /* Sending the longest transmission time */
} ext_dsid_drv_uart_info;

typedef struct {
    td_u32 old_addr; /* dbk preemption: preempted ip address */
    td_u32 new_addr; /* dbk preemption: preempt ip address */
    td_u32 second;   /* dbk preemption: time (s) */
} ext_dsid_sal_dbk_grab_connect_info;
#define EXT_DSID_SAL_DBK_GRAB_CONNECT_INFO_COUNT 3

typedef struct spi_init_info {
    td_u32 baudrate;  /* baud rate */
    td_u32 framemode; /* transfer mode */
    td_u32 datawidth; /* data width */
    td_u32 spo;       /* spiclkout phase in moto's spi mode */
    td_u32 sph;       /* spiclkout polarity in moto's spi mode */
} ext_spi_dev_attr;
typedef struct spi_dfx_info {
    td_u32 id;
    td_u32 int_vector;
    ext_spi_dev_attr spi_attr;
    td_u32 buf_size;
    td_u32 reg_base;
    td_u32 rxbuf_p;
    td_u32 rxbuf_read;
    td_u32 rxbuf_write;
    td_s32 rxbuf_status;
    td_u32 txbuf;
    td_u32 txbuf_read;
    td_u32 txbuf_write;
    td_s32 txbuf_status;
    td_u32 err_buf_full;
    td_u32 err_runout;
} ext_dsid_drv_spi_info;

typedef struct {
    ext_obj64 obj_cnt;
    td_u32 rx_cnt;     /* number of received packets */
    td_u32 tx_cnt;     /* number of packets sent */
    td_u32 tx_err_cnt; /* send failure count */
    td_u16 buf_total;  /* total number of buffer */
    td_u16 buf_used;   /* current number of uses */
} ext_dsid_drv_eth_info;

typedef struct {
    ext_obj64 obj_cnt;
    td_u32 timer_add_cnt;     /* add the number of timer */
    td_u32 timer_del_cnt;     /* delete the number of timer */
    td_u32 timer_timeout_cnt; /* number of teimrs due */
} ext_dsid_drv_hrt_info;

/* ************************************************ ***************************** */
/* MAC statistic definition */
/* ************************************************ ***************************** */
/* range [A250, A450) */
/* 1, networking service message [0xA250~0xA270) */
/* 2, route [0xA270~0xA290) */
/* 3, sender [0xA290~0xA2B0) */
/* 4, segmenter [0xA2B0~0xA2D0) */
/* 5, PHY [0xA2D0~0xA2F0) */
/* 6, reorganizer [0xA2F0~0xA310) */
/* 7, beacon [0xA310~0xA330) */
/* 8, conv [0xA330~0xA350) */
/* 9, RX [0xA350~0xA370) */
/* 10, NTB [0xA370~0xA390) */
/* 11, the copy control [0xA390~0xA3A0) */
/* 12, secondary development of the control unit [0xA3A0~0xA3B0) */
/* 13, DFX [0xA3B0~0xA3C0) */
/* 14, PLC_IP [0xA3C0~0xA3D0) */
/* 17 Reserved [0xA3D0~0xA450) */
/* ************************************************ ***************************** */
/* ************************************************ ***************************** */
/* Note that the definition of the ID number must be defined in accordance with the documentation requirements. */
/* ID: EXT_DSID_XXX_XXX */
/* Corresponding structure: EXT_DSID_XXX_XXX_S */
/* ************************************************ ***************************** */
/* PHY statistic definition */
#define EXT_DSID_PHY_OFFSET 0xA2D0
#define EXT_DSID_PHY_TX_ERR_ID (EXT_DSID_PHY_OFFSET + 0x00)

/* Structure definition */
/* The measurable count variable inside L1COMMON */
typedef struct {
    ext_obj64 obj_cnt;
    td_u32 nv_id_error;
    td_u32 afe_error;
    td_u32 nv_read_error;
    td_u32 get_hw_id_error;  /* get the number of hardware id failures */
    td_u32 power_step_error; /* configure the number of digital transmit power errors */
    td_u32 last_hw_id;       /* hardware id obtained last time */

    td_u16 malloc_err_cnt; /* memory analysis failure count */
    td_u16 pad1;
} ext_dsid_phy_l1_common_err;

/* The measurable variable inside TX */
typedef struct {
    td_u32 freq_err_cnt;  /* band number error count */
    td_u32 freq_same_cnt; /* parallel detection frequency band consistent, number of unswitched */

    td_u32 freq_change_cnt; /* band switching times */
    td_u8 cur_freq;
    td_bool is_in_silence;          /* the sender is in a silent state */
    td_u16 set_freq_in_sending_cnt; /* set the frequency band during transmission */

    td_u32 plc_tx_not_allow_cnt; /* the number of times plc is not allowed to be sent */

    td_u32 plc_tx_success_cnt;
    td_u32 spc_tx_success_cnt;

    td_u32 plc_tx_timer_success_cnt;
} ext_dsid_phy_tx_err;

/* ticker module statistics */
#define NDM_STATISTIC_HEAD_OFFSET 0xA390
#define EXT_DSID_MAC_NDM_STAT_INFO_ID (NDM_STATISTIC_HEAD_OFFSET + 0x02)   /* Error information for the controller */
#define EXT_DSID_MAC_NDM_REMOTE_INFO_ID (NDM_STATISTIC_HEAD_OFFSET + 0x03) /* remote channel information */

/* dfx module statistics */
#define DFX_STATISTIC_HEAD_OFFSET 0xA3B0
#define EXT_DSID_MAC_DFX_STAT_INFO_ID (DFX_STATISTIC_HEAD_OFFSET + 0x01) /* dfx module business process statistics */

/* Business statistics of the controller */
typedef struct {
    td_u16 tx_chl_type_proxy_path_cnt;
    td_u16 tx_chl_type_no_proxy_path_cnt;

    td_u16 tx_chl_type_unkown_cnt;
    td_u16 rx_chl_type_proxy_path_cnt;

    td_u16 rx_chl_type_no_proxy_path_cnt;
    td_u16 rx_chl_duplicate_pk_cnt; /* received duplicate messages */

    td_u16 rx_chl_type_unkown_cnt;
    td_u16 mac_send_message_unknown_id; /* unknown id */

    td_u16 tx_remote_req_success_cnt; /* number of remote requests sent */
    td_u16 tx_remote_req_fail_cnt;    /* number of remote requests sent */
    td_u16 pad;
    td_u16 rx_remote_rsp_cnt;  /* number of remote responses received */
    td_u16 dbk_remote_req_cnt; /* number of remote requests sent by dbk */
    td_u16 dbk_remote_rsp_cnt; /* number of remote responses received by dbk */

    td_u16 dbk_cmd_callback_ndm_notify_msg_fail_cnt;
    td_u16 dbk_cmd_callback_para_err_cnt;
    td_u16 dbk_cmd_callback_impl_cnt;
    td_u16 dbk_cmd_callback_impl_mac_buffer_fail_cnt;
    td_u16 dbk_cmd_callback_impl_packet_frame_fail_cnt;
    td_u16 mac_send_message_set_white_list_cnt;

    td_u16 mac_send_message_cnt;
    td_u16 mac_send_message_msg_fail_cnt;
    td_u16 mac_send_message_msg_unknown_cnt;
    td_u16 nv_changed_notify_cnt; /* nv refresh callbacks */
    /* 14 */
    td_u32 rx_remote_chl_pkt_cnt;
    td_u32 drop_remote_chl_pkt_cnt;
    td_u32 local_handle_remote_chl_pkt_cnt;
    td_u16 retrans_pkt_from_req_success_cnt;
    td_u16 retrans_pkt_from_req_fail_cnt;
    td_u16 retrans_pkt_from_rsp_success_cnt;
    td_u16 retrans_pkt_from_rsp_fail_cnt;
    /* create the number of copy controller channel connection request packets */
    td_u32 create_remote_connect_req_cnt;
    /* create the number of copy control channel connection response messages */
    td_u32 create_remote_connect_rsp_cnt;
    td_u32 create_remote_heartbeat_cnt; /* create the number of heartbeat packets for the tracker channel */
    td_u32 rcv_remote_connect_req_cnt;  /* receive the number of caller channel connection request packets */
    /* receive the number of copy control channel connection response messages */
    td_u32 rcv_remote_connect_rsp_cnt;
    /* receive the number of heartbeat packets from the remote control channel */
    td_u32 rcv_remote_heartbeat_cnt;
    td_u32 create_chl_long_head_pkt_cnt;
} ext_dsid_mac_ndm_stat_info;

/* The statistics of the remote channel of the remote controller */
typedef struct {
    td_u32 fragment_output_success_cnt;
    td_u32 fragment_output_fail_cnt;

    td_u32 pipe_fragment_output_cnt;
    td_u32 pipe_fragment_output_malloc_fail_cnt;
    td_u32 pipe_fragment_output_msg_fail_cnt;
    td_u32 pipe_output_cnt;

    td_u32 pipe_output_mac_buffer_fail_cnt;
    td_u32 pipe_output_success_cnt;
    td_u32 pipe_output_fail_cnt;
    td_u32 pk_parse_transf_target_addr_fail_cnt;

    td_u32 pk_dump_pk_to_cvg_success_cnt;
    td_u32 dump_pk_to_cvg_fail_cnt;

    td_u32 put_pkt_in_tl_success_cnt;
    td_u32 put_pkt_in_tl_fail_cnt;
    td_u32 tl_pkt_bad_data_cnt;
    td_u32 rcv_chl_tl_pkt_cnt;
    td_u32 not_my_chl_tl_pkt_cnt;
    td_u32 handle_rcv_tl_pkt_fail_cnt;
    td_u32 rsb_tl_pkt_success_cnt;

    td_u16 cmd_id;
    td_u16 cmd_id_map[3]; /* 3 is length */
    td_u32 clear_memory_msg_count;
    td_u32 cur_malloc_count;
    td_u32 cur_free_count;
    td_u16 exception_pc_pkt_cnt;
    td_u16 rpt_pkt_to_pc_cnt;
} ext_dsid_mac_ndm_remote_info;

/* dfx module process statistics */
typedef struct {
    td_u32 send_msg_err_cnt;
    td_u16 malloc_err_cnt;
    td_u16 mfree_err_cnt;

    td_u32 query_request_cnt; /* dbk query statistics */
    td_u32 ndc_msg_cnt;
    td_u32 msg_nv_refresh_notify_cnt;
    td_u32 msg_dfx_ctrl_pk_cnt;
    td_u32 msg_dfx_ndm_notify_cnt;

    td_u32 msg_dfx_query_notify_cnt;
    td_u32 ndm_chl_rcv_pkt; /* number of ndm channel packets received from the nms */
    td_u32 msg_unknown_cnt;
    td_u32 topo_success_cnt;

    td_u32 discover_list_success_cnt;
    td_u32 discover_list_sm_success_cnt;
    td_u32 router_table_success_cnt;
    td_u16 other_query_success_cnt;
    td_u16 other_query_fail_cnt;

    td_u16 topo_fail_cnt;
    td_u16 discover_list_fail_cnt;
    td_u16 discover_list_sm_fail_cnt;
    td_u16 router_table_fail_cnt;

    td_u16 phase_identify_success_cnt;
    td_u16 phase_identify_fail_cnt;

    td_u16 violence_sta_qry_success_cnt;
    td_u16 violence_sta_qry_fail_cnt;

    td_u16 all_sta_info_success_cnt;
    td_u16 all_sta_info_fail_cnt;

    td_u16 topo_sm_sta_info_success_cnt;
    td_u16 topo_sm_sta_info_fail_cnt;
    td_u16 topo_sm_ver_info_success_cnt;
    td_u16 topo_sm_ver_info_fail_cnt;

    td_u16 register_cmd_id_success_cnt;
    td_u16 register_cmd_id_fail_cnt;

    td_u16 query_register_cmd_id_success_cnt;
    td_u16 cover_cmd_id_cnt;
} ext_dsid_mac_dfx_stat_info;

/* ***************************************************************************** */
/* UPG statistic definition */
/* ***************************************************************************** */
#define EXT_DSID_UPG_CCO_SND_STAT 0xA502   /* ext_dsid_upg_cco_snd_info */
#define EXT_DSID_UPG_STA_BLK_STAT 0xA503   /* ext_dsid_upg_sta_start_blk */
#define EXT_DSID_UPG_CCO_RCV_STAT 0xA504   /* ext_dsid_upg_cco_rcv_info */
#define EXT_DSID_UPG_CCO_NET_STAT 0xA505   /* ext_dsid_upg_network_info */
#define EXT_DSID_UPG_STA_STOP_STAT 0xA506  /* ext_dsid_upg_sta_status_stop */
#define EXT_DSID_UPG_STA_REPLY_STAT 0xA508 /* ext_dsid_upg_sta_reply */
#define EXT_DSID_UPG_CCO_QUERY_INFO 0xA512 /* ext_dsid_upg_cco_query_info */
#define EXT_DSID_UPG_STA_QUERY_INFO 0xA513 /* ext_dsid_upg_sta_query_info */

#endif /* __DFX_SYS_DSID_H__ */

