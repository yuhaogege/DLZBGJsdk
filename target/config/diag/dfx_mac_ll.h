/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: DFX interfaces of link layer
 */
#ifndef _DFX_MAC_LL_H_
#define _DFX_MAC_LL_H_

/* 5.1 Proactively report and send statistics (DBK, console) after the DFX periodic query */
typedef struct {
    td_u32 rts_cts_count;                      /* Number of sent rts frames */
    td_u32 sack_count;                         /* Number of sent SACK frames */
    td_u32 beacon_count;                       /* Number of sent beacon frames */
    td_u32 unicast_count;                      /* Number of unicast packets */
    td_u32 broadcast_count;                    /* Number of broadcast packets */
    td_u32 short_packet_count;                 /* Number of transmitted short packets */
    td_u32 pb72_packet_count;                  /* Number of transmitted PB72s */
    td_u32 collision_count;                    /* Number of sending conflicts */
    td_u32 tx_get_all_msdu_cnt;                /* Number of msdu packets successfully obtained by the sender */
    td_u32 tx_get_nm_cnt;                      /* Obtain the number of NMS packets. */
    td_u32 tx_get_app_cnt;                     /* obtains the number of APP packets. */
    td_u32 tx_get_upg_cnt;                     /* Obtain the number of upgrade packets. */
    td_u32 tx_send_msdu_success;               /* Total number of msdu packets successfully sent by the sender. */
    td_u32 tx_send_nm_success;                 /* successfully sends network management packets */
    td_u32 tx_send_app_success;                /* Number of times APP packets are successfully sent */
    td_u32 tx_send_upg_success;                /* Number of Upgrade Packets Successfully Sent */
    td_u32 tx_send_msdu_timeout;               /* Total number of timeout msdu packets sent by the sender. */
    td_u32 tx_send_nm_timeout;                 /* Indicates the number of timeouts for sending NMS packets. */
    td_u32 tx_send_app_timeout;                /* Number of times APP packets time out to be sent */
    td_u32 tx_send_upg_timeout;                /* Number of upgrade packet sending timeouts */
    td_u32 tx_send_msdu_fail;                  /* Total number of msdu packets that fail to be sent by the sender. */
    td_u32 tx_send_nm_fail;                    /* Number of times NMS packets fail to be sent */
    td_u32 tx_send_app_fail;                   /* Number of times APP packets fail to be sent */
    td_u32 tx_send_upg_fail;                   /* Number of Upgrade Packet Sending Failures */
    td_u32 tx_send_msdu_resend;                /* Number of times that the sender resends the msdu message */
    td_u32 tx_send_nm_resend;                  /* Number of retransmissions of network management packets */
    td_u32 tx_send_app_resend;                 /* Number of times APP packets are retransmitted */
    td_u32 tx_send_upg_resend;                 /* Number of retransmissions of upgrade packets */
    td_u32 sof_wait_sack_send_success;         /* Number of SACK SOF messages that are successfully sent */
    td_u32 sof_not_wait_sack_send_success;     /* Number of packets that are successfully
                                                  sent without waiting for SACK packets */
    td_u32 sof_resend_exclude_fix_resend;      /* Indicates the number of retransmitted packets,
                                                * excluding fixed retransmission packets.
                                                */
    td_u32 no_upg_slot_send;                   /* Number of upgrade packets sent without BCSMA timeslots */
    td_u32 tx_send_wait_end_interrupt_timeout; /* Timeout times of waiting for the transmission completion interrupt */
    td_u32 tx_state_abnormal_cnt;              /* Number of abnormal sending status */
} diag_cmd_query_tx_statics;
/* 5.2 Report the received statistics (DBK, console) after the DFX periodic query */
typedef struct {
    td_u32 fc_count;                    /* Number of FC interrupts (statistics in FC interrupts) */
    td_u32 fc_crc_success_count;        /* Count of FC check success in FC interrupt. */
    td_u32 fc_crc_err_count;            /* Number of FC check errors Count of FC interrupts
                                         * (If FCCS check errors do not trigger FC interrupts, the count is 0.
                                         */
    td_u32 fc_local_count;              /* Total number of local FCs. Beacon:sof:sack:sound:rts_cts: */
    td_u32 false_frame_count;           /* Number of virtual ports */
    td_u32 not_expect_dt_count;         /* number of unexpected frames (generated due to collision) */
    td_u32 beacon_count;                /* Number of local Beacon frames */
    td_u32 sof_count;                   /* Number of local SoF frames */
    td_u32 sack_count;                  /* Number of local SACK frames */
    td_u32 coordinate_frame_count;      /* Number of multi-network coordination frames */
    td_u32 service_fc_cnt;              /* Number of service extended FCs */
    td_u32 parallel_ok_cnt;             /* Number of parallel detection interrupts that are OK */
    td_u32 beacon_pb_crc_err_count;     /* Number of beacon pb verification failures */
    td_u32 beacon_pb_crc_success_count; /* Number of successful beacon pb verifications */
    td_u32 broadcast_count;             /* Number of local broadcast packets */
    td_u32 unicast_count;               /* Number of local unicast packets */
    td_u32 resend_count;                /* Number of locally retransmitted packets */
    td_u32 short_pb_crc_err_count;      /* Number of 136pb block CRC errors */
    td_u32 pb72_crc_err_count;          /* Number of 72pb block CRC errors */
    td_u32 long_pb_crc_err_count;       /* Number of 72pb block CRC errors */
    td_u32 short_pb_count;              /* Number of 136pb blocks PB block interrupt */
    td_u32 pb72_count;                  /* Number of 72pb blocks PB block interrupt */
    td_u32 long_pb_count;               /* Number of 72pb blocks PB block interrupt */
    td_u32 size_err_cnt;                /* Number of PB block size errors */
    td_u32 discard_packet_count;        /* Discard packets. */
    td_u32 no_pb_packet_count;          /* Indicates the number of packets that are not received completely and
                                         * have no PB blocks during FC frequency scanning.
                                         */
    td_u32 time_out_count;              /* Timeout count of packets in the receive queue */
    td_u32 alloc_mac_buff_fail_cnt;     /* Number of failures to apply for MAC buffers during packet receiving. */
    td_u16 input_rx_sof_queue_fail_cnt;
    td_u16 input_rx_beacon_queue_fail_cnt;
    td_u16 input_rx_other_queue_fail_cnt;
    td_u16 drop_pb_cnt;
} diag_cmd_query_rx_statics;
typedef struct {
    td_u32 rcv_first_pkt_time; /* Time when the first packet is received. Unit: ms. */
    td_u32 rx_lisr_count;      /* Number of times that the receiver interrupts are generated */
    td_u32 rx_hisr_count;      /* Number of responses to advanced interrupts of the receiver */
    td_u32 sack_success_count; /* Generate the number of SACK frames that are successfully received. */
    td_u32 sack_fail_count; /* Generate SACK frames whose content is the number of frames that fail to be received. */
    td_u32 pad3;            /* Time when the snid is set for the first time, in ms */
    td_u32 discard_sack_count; /* Number of discarded SACK frames */
    td_u32 pad2;
    td_u32 random_assoc_start_time; /* Start time of random association. (unit: ms) */
    td_u32 first_pkt_send_time;     /* Time when the first packet is sent */
    td_u32 send_assoc_req_cnt;
    td_u32 first_rcv_bcn_time;
    td_u32 first_rcv_bcn_fc_time; /* Time when the beacon FC is received for the first time */
    td_u32 pad1;
    td_u8 spc_atten_min[RX_STAT_NUM]; /* Minimum value of multi-network SPC attenuation */
    td_u8 spc_atten_max[RX_STAT_NUM]; /* maximum SPC attenuation of multiple networks */
} diag_cmd_query_rx_diagnostic;
typedef struct {
    td_u32 beacon_in_que_cnt;       /* Beacon Packet Enqueuing Counter */
    td_u32 sof_in_que_cnt;          /* Counter of SOF packets entering the sending queue */
    td_u32 fc_in_que_cnt;           /* Counter of FC packets entering the transmit queue. */
    td_u32 coordinate_in_que_cnt;   /* inter-network coordination packet queue counter */
    td_u32 bind_tdma_in_que_cnt;    /* Number of bound TDMA packets entering queues. */
    td_u32 beacon_out_que_cnt;      /* Counter for beacon packets leaving the sending queue */
    td_u32 sof_out_que_cnt;         /* Counter for SOF packets leaving the sending queue */
    td_u32 fc_out_que_cnt;          /* Counter for FC packets leaving the sending queue */
    td_u32 coordinate_out_que_cnt;  /* inter-network coordination packet outgoing transmit queue counter */
    td_u32 bind_tdma_out_que_cnt;   /* Count of bound TDMA packets leaving queues. */
    td_u32 beacon_timeout_cnt;      /* Beacon packet timeout counter in the sending queue */
    td_u32 sof_timeout_cnt;         /* SOf packet timeout counter in the transmit queue */
    td_u32 fc_timeout_cnt;          /* FC packet timeout counter in the transmit queue */
    td_u32 coordinate_timeout_cnt;  /* Inter-network coordination frame packet timeout counter in the transmit queue */
    td_u32 get_beacon_cnt;          /* obtains the beacon packet counter from the sending queue. */
    td_u32 get_sof_cnt;             /* Obtain the SOF packet counter from the transmit queue. */
    td_u32 get_fc_cnt;              /* Obtain the FC packet counter from the transmit queue. */
    td_u32 get_coordinate_cnt;      /* Obtain the coordination frame packet counter from the transmit queue. */
    td_u32 get_bind_tdma_cnt;       /* Obtain the number of bound TDMA packets. */
    td_u32 sof_overflow_cnt;        /* Overflow counter for SOF packets entering the transmit queue */
    td_u32 fc_overflow_cnt;         /* Overflow counter for FC packets entering the transmit queue */
    td_u32 beacon_overflow_cnt;     /* Beacon packet queue overflow counter */
    td_u32 coordinate_overflow_cnt; /* coordination packet overflow counter when packets enter the transmit queue. */
    td_u16 detect_bcn_timeout_cnt;  /* probing beacon timeout count */
    td_u16 detect_bcn_overflow_cnt; /* probe beacon overflow count */
    td_u16 direct_in_que_cnt;       /* Number of packets directly sent to queues. */
    td_u16 direct_timeout_cnt;      /* Timeout count of directly sending packets */
} diag_cmd_query_tx_queue_diagnostic;

typedef struct {
    td_u32 tmi6_send_cnt;                /* TMI6 packet sending times */
    td_u32 tmi4_send_cnt;                /* TMI4 packet sending times */
    td_u32 tmi1_send_cnt;                /* TMI1 packet sending times */
    td_u32 tmi0_send_cnt;                /* TMI0 packet sending times */
    td_u32 tmi9_send_cnt;                /* TMI9 packet sending times */
    td_u32 tmi11_send_cnt;               /* TMI11 packet sending times */
    td_u32 tmi12_send_cnt;               /* TMI12 packet sending times */
    td_u32 tmi13_send_cnt;               /* TMI13 packet sending times */
    td_u32 tmi14_send_cnt;               /* TMI14 packet sending times */
    td_u32 send_fail_in_sending_cnt;     /* Times of resending failure due to sending */
    td_u32 backoff_fail_cnt;             /* Number of backoff failures */
    td_u32 send_event_cnt;               /* Number of events received by the transmitter */
    td_u32 handle_event_cnt;             /* Number of events processed by the sender */
    td_u32 tdma_period_refreshed_cnt;    /* Number of timeslot update times */
    td_u32 mpdu_queue_in_cnt;            /* Number of packet enqueuing events */
    td_u32 tx_en_send_fail_cnt;          /* Indicates the number of PHY transmission failures. */
    td_u32 channel_busy_cnt;             /* Number of times that the channel becomes busy before transmission */
    td_u32 tmi6_send_fail_cnt;           /* Number of TMI6 packet sending failures */
    td_u32 tmi4_send_fail_cnt;           /* Number of TMI4 packet sending failures */
    td_u32 tmi1_send_fail_cnt;           /* Number of TMI1 packet sending failures */
    td_u32 tmi0_send_fail_cnt;           /* Number of TMI0 packet sending failures */
    td_u32 tmi9_send_fail_cnt;           /* Number of TMI9 packet sending failures */
    td_u32 tmi11_send_fail_cnt;          /* Number of TMI11 packet sending failures */
    td_u32 tmi12_send_fail_cnt;          /* Number of TMI12 packet sending failures */
    td_u32 tmi13_send_fail_cnt;          /* Number of TMI13 packet sending failures */
    td_u32 tmi14_send_fail_cnt;          /* Number of TMI14 packet sending failures */
    td_u32 tx_send_bea_fail_cnt;         /* Number of beacon sending failures due to insufficient timeslots,
                                          * excluding the number of times that the channel is busy
                                          */
    td_u32 tx_send_bea_fail_by_busy_cnt; /* Number of beacon sending failures due to busy channels */
    td_u32 send_sof_pb_to_phy_fail_cnt;  /* Indicates the number of encryption packets that
                                            fail to be written into the PHY. */
} diag_cmd_query_tx_diagnostic;
typedef struct {
    td_u32 tx_retranfer_packet_count;  /* Number of packets forwarded by the aggregator to the sender */
    td_u32 tx_network_packet_count;    /* Number of NMS packets that the sends to the sender */
    td_u32 tx_app_packet_count;        /* Number of application packets that the sends to the sender */
    td_u32 tx_ndc_packet_count;        /* Number of NDC packets sent from the aggregator to the sender. */
    td_u32 tx_unknown_packet_count;    /* The aggregator receives an unknown packet to be sent. */
    td_u32 rx_retranfer_packet_count;  /* Number of forwarded packets received by
                                         the convergence device from the receiver */
    td_u32 rx_network_packet_count;    /* Number of NM packets received by the convergence device from the receiver */
    td_u32 rx_app_packet_count;        /* Number of app packets received by the aggregator from the receiver */
    td_u32 rx_unknown_packet_count;    /* Number of unknown packets received by the concentrator from the receiver */
    td_u32 rx_upg_packet_count;        /* Number of upgrade packets received by the concentrator from the receiver */
    td_u32 rx_ndc_packet_count;        /* This object indicates the number of
                                        * NDC packets received by the concentrator from the receiver.
                                        */
    td_u32 unicast_discard_count;      /* Number of discarded unicast packets: packets from other sites */
    td_u32 unicast_retransfer_count;   /* Number of forwarded unicast packets */
    td_u32 broadcast_discard_count;    /* Number of discarded broadcast packets */
    td_u32 broadcast_retransfer_count; /* Number of forwarded broadcast packets */
    td_u32 broadcast_retransfer_get_buf_fail_count; /* Number of broadcast packets that fail to apply
                                                     * for buffers before being forwarded
                                                     */
    td_u32 query_route_fail_count;                  /* Route query fails. */
    td_u32 get_tei_by_mac_fail_count;    /* Number of times that the fails to obtain the TEI through the MAC address */
    td_u32 app_dequeue_buf_fail_count;   /* Number of application layer packets that fail to enter the queue */
    td_u16 app_pk_msdu_size_err_count;   /* Number of abnormal application layer packet size parameters */
    td_u16 set_app_send_info_fail_count; /* sets the number of times that application layer packets fail to be sent. */
    td_u16 app_vlantag_err_count;        /* number of packets with abnormal priorities at the application layer. */
    td_u16 send_pk_to_app_fail_count;    /* Number of packets that fail to be sent to the application layer */
    td_u32 rx_app_pkt_unencrypt_err_cnt; /* The protocol security is enabled, and plaintext packets
                                            are received from the app. */
    td_u32 set_stas_commu_odtei_count;   /* Set the number of destination TEIs of the subnode communication. */
    td_u32 rx_retransfer_stas_commu_packet_count; /* Number of communication packets received by the aggregator from
                                                   * the receiver and forwarded by the subnode
                                                   */
    td_u32 rx_retransfer_stas_commu_unicast_cnt;  /* Number of communication unicast packets
                                                     of the forwarding subnode */
} diag_cmd_query_cvg_statics_pk;

typedef struct {
    td_u32 rsb_handle_pk_cnt;               /* Number of packets processed by the reassembler */
    td_u32 msdu_size_err_cnt;               /* Indicates the number of packets with invalid sizes. */
    td_u32 rsb_icv_rcv_cnt;                 /* Indicates the number of packets that pass the MAC check. */
    td_u32 rsb_not_my_pk_mac_cnt;           /* indicates the number of non-local site packets,
                                             * which are filtered by MAC address.
                                             */
    td_u32 rsb_not_my_pk_tei_cnt;           /* indicates the number of non-local site packets,
                                                which are filtered by dtei. */
    td_u32 rsb_not_my_pk_type_cnt;          /* number of packets from other sites, filtered by type */
    td_u32 rsb_filter_repeat_pk_cnt;        /* Number of repeated packets filtered by packet */
    td_u32 rsb_filter_repeat_pk_by_mac_cnt; /* number of filtered MAC addresses. */
    td_u32 rsb_filter_ciu_pk_cnt;           /* ciu filtering */
    td_u32 rsb_filter_rcv_pk_cnt;           /* Indicates the number of normal packets that are not filtered out. */
    td_u32 rsb_deliver_pk_fail_cnt;         /* Number of packets that the reassembler fails to distribute */
    td_u32 rsb_deliver_pk_cnt;              /* Number of packets distributed by the reassembler */
    td_u32 rsb_list_pk_cnt;                 /* Number of packets in the reassembler linked list */
    td_u32 rsb_list_pk_max_cnt;             /* peak number of packets in the link table of the reassembler. */
    td_u32 rsb_check_rt_err_cnt;
    td_u16 rsb_monitor_nid_cnt;
    td_u16 rsb_monitor_nid_success_cnt;
    td_u16 rsb_without_white_list_cnt;
    td_u16 pad;
} diag_cmd_query_rsb_statics_pk;
typedef struct {
    td_u8 all_queue_pk_cnt;            /* Total number of packets in a segment queue */
    td_u8 queue0_pk_cnt;               /* Number of packets in queue 0 */
    td_u8 queue1_pk_cnt;               /* Number of packets in queue 1 */
    td_u8 queue2_pk_cnt;               /* Number of packets in queue 2 */
    td_u8 queue3_pk_cnt;               /* Indicates the number of packets in queue 3. */
    td_u8 upg_queue_cnt;               /* Number of packets in the upgrade queue */
    td_u16 msdu_len_err_cnt;           /* msdu length error */
    td_u32 queue0_overflow_cnt;        /* Overflow times of queue 0 */
    td_u32 queue1_overflow_cnt;        /* Number of times that queue 1 overflows */
    td_u32 queue2_overflow_cnt;        /* Number of times that queue 2 overflows */
    td_u32 queue3_overflow_cnt;        /* Number of overflows of queue 3 */
    td_u32 upg_queue_overflow_cnt;     /* Number of upgrade packet queue overflows */
    td_u32 segment_sof_cnt;            /* Number of times that the SOF frame is segmented */
    td_u32 beacon_enqueue_cnt;         /* Number of times that beacon frames are added to the segment queue */
    td_u32 segment_split_mac_fail_cnt; /* Number of packet fragmentation errors */
    td_u32 segment_queue_overflow_cnt; /* Number of times that packets enter the queue overflows */
    td_u32 send_event_to_tx_cnt;       /* Number of times that packets enter the queue */
} diag_cmd_query_seg_statics_pk_stru;
typedef struct {
    td_u32 bcn_soft_crc_err_cnt;          /* Number of Beacon Frame Software Check Failures */
    td_u32 sof_crc_err_cnt;               /* Number of MAC check failures of SOF frames */
    td_u16 rcv_neighbor_identify_bcn_cnt; /* receives the identification beacon count of the neighbor network
                                           * with large attenuation.
                                           */
    td_u16 rcv_neighbor_fc_cnt;           /* Number of non-beacon frames that are received with large
                                           * attenuation and are determined as neighboring networks
                                           */
    td_u8 rcv_atten;                      /* Receive attenuation value */
    td_u8 analyze_pkt_err_cnt;            /* An error occurred when parsing the beacon content. */
    td_u8 my_net_big_atten; /* Attenuation of the received signal that is greater than the crosstalk threshold
                             * on the local network
                             */
    td_u8 my_net_big_atten_invalid_cnt; /* maximum attenuation invalid times */
    td_u32 send_bcn_to_nm_cnt;          /* Number of packets sent to the NMS */
    td_u16 parse_bcn_entry_err_cnt;     /* The fails to parse the beacon entry. */
    td_u16 bcn_belen_err_cnt;           /* Beacon entry length error */
    td_u32 rcv_my_bcn_cnt;              /* Number of beacons received from the local network */
    td_u32 my_net_big_atten_cnt;        /* Indicates the number of received attenuations that are greater than
                                         * the crosstalk threshold on the local network.
                                         */
} diag_cmd_query_fc_pb_fault;

typedef struct {
    td_u32 snd_ntb;
    td_u16 odtei;        /* Destination site of the message */
    td_u16 ostei;        /* Source site of packets */
    td_u16 dtei;         /* Next hop */
    td_u16 packet_index; /* Packet sequence number */
    td_u16 msdu_size;    /* Packet size */
    td_u16 packet_type;  /* Packet type: upgrade packet, application layer packet,
                            network management packet, and IP packet */
    td_u8 result;        /* Sending result: sending success, sending failure,
                            packet expiration, and packet retransmission */
    td_u8 lid;
    td_u8 pad1;
    td_u8 broadcast_flag;
    td_u16 ptcl_ver;
    td_u16 pad;
} diag_cmd_tx_report;
typedef struct {
    td_u32 rcv_ntb;
    td_u16 ostei;       /* Source address of the packet */
    td_u16 odtei;       /* Packet destination address */
    td_u16 dtei;        /* Next hop address of the packet */
    td_u16 stei;        /* Packet sending address */
    td_u16 msdu_size;   /* Packet size */
    td_u16 packet_type; /* Packet type */
    td_u16 packet_index;
    td_u8 lid;
    td_u8 broadcast_flag;
    td_u16 ptcl_ver;
    td_u16 pad;
} diag_cmd_rx_report;
typedef struct {
    td_u32 ntb; /* NTB when the packet is received */
    td_u32 snid;
    td_u8 freq;
    td_u8 phy_detect_ver;
    td_u8 ptcl_ver;
    td_u8 dt;
    td_u16 stei;
    td_u16 dtei;
    td_u8 tmi : 4;
    td_u8 eks : 2; /* Encrytoin Key Select */
    td_u8 pad : 2;
    td_u8 phs;
    td_u8 fc_crc;
    td_u8 crc;
    td_u8 lid;
    td_u8 pream_fft_cnt;
    td_u8 attenuation;
    td_s8 snr;

    td_u8 fc_ver;
    td_u8 pad_1[3]; /* resverd 3 bytes */

    td_u16 preamle_sat_num;
    td_u8 discard_payload;
    td_u8 is_reply_sack;
    td_u32 fc[4]; /* fc length is of 4 words */
} diag_cmd_rcv_info_report;
typedef struct {
    td_u16 pad;
    td_u16 size;
    td_u32 addr[130]; /* LONG_PB_SIZE / 4 = 130 bytes */
} diag_cmd_rcv_pb_report;
typedef struct {
    td_u8 dt;
    td_u8 tmi;
    td_u16 freq;
    td_u32 snd_ntb;
    td_u32 bpc;
} diag_cmd_snd_info_report;


/* V200 multi-network receiving statistics */
typedef struct {
    td_u32 snid[RX_STAT_NUM];
    td_u32 bcn_fc[RX_STAT_NUM];
    td_u32 sof_fc[RX_STAT_NUM];
    td_u32 first_rcv_time[RX_STAT_NUM];
    td_u32 last_rcv_time[RX_STAT_NUM];
    td_u8 rcv_cco_atten[RX_STAT_NUM];
    td_u8 rcv_cco_atten_min[RX_STAT_NUM];
    td_s8 rcv_cco_snr[RX_STAT_NUM];
    td_u8 rcv_atten_min[RX_STAT_NUM];
    td_u8 rcv_atten_max[RX_STAT_NUM];
    td_u16 store_overflow_cnt; /* Storage overflow count */
    td_u16 pad1;
    td_u32 store_overflow_snid; /* indicates the SN ID of the last record that overflows. */
} diag_cmd_multi_network_rcv_v200;

typedef enum _tx_status_ {
    PK_GET = 0,
    TX_SUCCESS,
    TX_TIMEOUT,
    TX_FAIL,
    TX_RESEND
} tx_status;

typedef struct {
    td_u32 get_slot_cnt;        /* Obtain the timeslot count counter. */
    td_u32 sta_slot_update_cnt; /* STA update timeslot count */
    td_u32 cco_slot_update_cnt; /* CCO update timeslot count */
    td_u32 updata_event_cnt;    /* Timeslot update event counter */
    td_u32 no_slot_cnt;         /* No timeslot count */
    td_u32 repeat_slot_cnt;     /* Count of the repeatedly used timeslots */
    td_u32 get_slot_fail_cnt;   /* Indicates the count of timeslot obtaining failures. */
} diag_cmd_query_slot_diagnostic;
/* Message definition of the sender module */
typedef struct {
    td_u32 cur_ntb;
    td_u32 src_mod;
    td_u32 event_group;
    td_u32 cur_event;
} ext_dmid_mac_tx_receive_event;
typedef struct {
    td_u32 cur_ntb;
    td_u16 dtei;
    td_u16 rsvd;
    td_u32 duration;
    td_u32 result;
} ext_dmid_mac_tx_rcts_packet_generate;
typedef struct {
    td_u32 cur_ntb;
    td_u8 mpdu_type;
    td_u8 discard_flag;
    td_u8 phase;
    td_u8 exist_flag;
} ext_dmid_mac_tx_beacon_mpdu_info;
typedef struct {
    td_u8 mpdu_type;
    td_u8 send_flag;
    td_u8 discard_flag; /* Discard flag of the MPDU packet; */
    td_u8 count;        /* Number of PB blocks; */
    td_u8 phase;        /* Phase information; */
    td_u8 bcf;          /* Broadcast flag bit of the packet; */
    td_u16 mpdu_index;  /* MPDU index value; */
    td_u16 size;        /* PB block size; */
    td_u16 ctei;        /* Terminal equipment identifier of the cross-connect station; */
    td_u16 fc_size;     /* Frame header size of the packet; */
    td_u16 odtei;       /* Identifier of the destination terminal device; */
    td_u32 expire_time; /* Packet expiration time; */
    td_u8 lid;
    td_u8 encrypt_flag;
    td_u8 rsvd;
    td_u8 mpdu_queue_phase; /* Phase information of the queue that unicast packets enter in multi-phase mode; */
    td_u8 cur_resend;
    td_u8 total_resend;
} ext_dmid_mac_tx_mpdu_info;
typedef struct {
    td_u32 ntb;
    td_s32 avg_snr;
    td_u8 dt;
    td_u8 expect_dt;
    td_u16 stei;
    td_u16 dtei;
    td_u8 snid;
    td_u8 freq;
} ext_dmid_mac_rx_rcv_fc_info;
typedef struct {
    td_bool decode_result;
    td_bool crc_result;
    td_u8 snid;
    td_u8 pad;
    td_u16 stei;
    td_u16 pad2;
    td_u32 bpc;
} ext_dmid_mac_rx_beacon_status;
typedef struct {
    td_bool decode_result;
    td_bool crc_result;
    td_u8 snid;
    td_u8 pad;
    td_u16 stei;
    td_u16 pad2;
    td_u32 bpc;
} ext_dmid_mac_rx_sof_status;
/* Packets of the NMR controller fail to enter the segment queue. */
typedef struct {
    td_u8 pkd_idx; /* Packet number */
    td_u8 pad[3];  /* Reserved 3 bytes */
} ext_dmid_mac_dcm_seg_queue_in_fail_info;
/* The NCR successfully sends packets. */
typedef struct {
    td_u8 pkd_idx; /* Packet number */
    td_u8 pad[3];  /* Reserved 3 bytes */
} ext_dmid_mac_dcm_tx_success_info;
/* The NMR reader fails to send packets. */
typedef struct {
    td_u8 pkd_idx; /* Packet number */
    td_u8 pad[3];  /* Reserved 3 bytes */
} ext_dmid_mac_dcm_tx_fail_info;
/* Sending the NCR packet times out. */
typedef struct {
    td_u8 pkd_idx; /* Packet number */
    td_u8 pad[3];  /* Reserved 3 bytes */
} ext_dmid_mac_dcm_tx_timeout_info;
/* Packet reassembly of the NCR times out. */
typedef struct {
    td_u8 pkd_idx; /* Packet number */
    td_u8 pad[3];  /* Reserved 3 bytes */
} ext_dmid_mac_dcm_rsb_timeout_info;
#endif