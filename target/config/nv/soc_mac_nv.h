/*
 * copyright (c) CompanyNameMagicTag 2012-2018. All rights reserved.
 * Description: NV-Related IDs and Structure Definitions
 * Author: CompanyName
 * Create: 2012-07-16
 */

#ifndef MAC_NV_H
#define MAC_NV_H

#include "soc_types.h"
#include "soc_mac_nv_value_def.h"
#include "soc_phy_nv.h"

typedef struct {
    td_u16 evaluate_phase_bpc_or_cnt_th; /* Assessment period: BPC */
    td_u16 boot_evaluate_fail_th;        /* Maximum number of evaluation failures
                                          * due to the failure to receive a three-phase beacon frame
                                          */

    td_u16 evaluate_cmm_diff;     /* Compare error values based on the receive rate. */
    td_u16 evaluate_channel_diff; /* Compare error values based on channel capacity. */
} diag_cmd_config_phase_evaluate_attr_stru;

typedef struct {
    td_u8 cco_phase_cnt;                /* Number of CCO phases (a maximum of three phases are supported) */
    td_u8 default_phase;                /* Default phase (used in CCO single-phase mode) */
    td_u8 rx_phase_mode;                /* Phase reception mode: 0. 3-phase independent reception;
                                         * 1. Three-phase simultaneous reception
                                         */
    td_u8 dynamic_adjust_resend_enable; /* Enable the function of dynamically adjusting the number of
                                         * retransmission times. This function is enabled by default.
                                         */
    td_u8 max_resend_cnt;               /* Max. Retransmissions */
    td_u8 fixed_resend_cnt;             /* Fixed Retransmission Times */
    td_u8 pad[2];                       /* Reserved 2 bytes */

    diag_cmd_config_phase_evaluate_attr_stru phs_evaluate_para;
} diag_cmd_config_phase_attr_stru;

/*
 * 1.31 TDMA Configuration
 */
typedef struct _diag_cmd_config_tdma_config_stru_ {
    td_u32 max_beacon_period; /* Indicates the maximum beacon period, in ms. */
    td_u32 min_beacon_period; /* Indicates the minimum beacon period, in ms. */

    td_u16 percent_beacon_period;    /* Ratio of CSMA to Beacon Timeslots */
    td_u16 alloc_period_athead_time; /* Time when a beacon frame is created in advance */

    td_u8 one_bea_duration;                    /* Beacon Timeslot Length */
    td_u8 central_beacon_num;                  /* Number of Central Beacon Frames */
    td_u8 no_report_backup_proxy_chance;       /* Number of times that all candidate proxies
                                                * increase the beacon timeslot
                                                */
    td_u8 start_alloc_first_level_cnf_bcn_cnt; /* Beacon rounds to be confirmed
                                                * before the next round of networking
                                                */

    td_u8 comm_rate;                               /* Communication rate pre-value in the association phase */
    td_u8 min_discover_beacon_sum;                 /* Min. Discovery Beacon Timeslots */
    td_u8 net_forming_discover_beacon_state_count; /* Number of rounds of intensive beacon frame sending */
    td_u8 net_forming_no_report_sta_chance_count;  /* Number of Beacon Timeslot Rounds
                                                    * in the Dense Association Phase
                                                    */

    td_u16 min_phase_duration;              /* Minimum CSMA length of each phase */
    td_bool pad2;                           /* Enable Level-1 Escalation Mode */
    td_u8 alloc_dis_bea_cnt_for_rt_one_sta; /* Number of Beacon Timeslot Rounds During Fast Optimization */

    td_u16 min_network_packet_cp; /* Minimum number of network management timeslots when bound timeslots exist. */
    td_u16 max_bind_cp;

    td_u16 pad4;
    td_u8 pad5;
    td_bool equip_mode_flag; /* Whether the tooling mode is used */

    td_u16 max_proxy_count;     /* Maximum number of proxies when the number of sites is less than 500.
                                 * The default value is 50.
                                 */
    td_u16 max_proxy_count_max; /* Maximum number of proxies when the number of sites exceeds 500.
                                 * The default value is 100.
                                 */

    td_u16 pad6;                       /* Reserved bit */
    td_bool tdma_alloc_dfx_switch;     /* TDMA Timeslot Allocation Maintainability and Testability Switch */
    td_u8 tdma_join_count_per_periods; /* Number of sites that can be added in each round
                                        * in the dense association phase
                                        */

    td_u8 tdma_pk_duration;        /* Duration for sending a packet */
    td_u8 performance_test_switch; /* Indicates whether to enable TDMA scheduling in test mode.
                                    * In test mode, timeslot separation is not performed and is not used.
                                    */
    td_u16 min_csma_duration;      /* Minimum CSMA Timeslot */
} diag_cmd_config_tdma_config_stru;

typedef struct _diag_cmd_config_coordinate_config_stru_ {
    td_u16 max_backup_sys_time;    /* Maximum backoff duration */
    td_u16 max_same_snid_err_time; /* Maximum duration in which the same network code is allowed */

    td_u16 send_pk_len_for_notify;     /* Interval for sending coordination packets
                                        * in a Beacon period other than the Beacon period of the CCO
                                        */
    td_u16 send_pk_len_for_coordinate; /* indicates the period of sending coordination packets
                                        * in the Beacon period of the CCO.
                                        */

    td_u16 check_cannot_comm_limit;      /* Threshold of disconnection, in the unit of system time. */
    td_u16 listen_time_before_first_bea; /* Indicates the duration for monitoring the network bandwidth
                                          * of the neighbor after the device is powered on. Unit: ms.
                                          */

    td_bool open_coordinate_network; /* Multi-network coordination switch */
    td_u8 network_evaluation_enable; /* Indicates whether to enable the preferred SNID function. 0: no */
    td_u8 continue_try_assoc_th;     /* Number of consecutive association attempts
                                      * (In the scenario where the uplink and downlink are asymmetric,
                                      * the association time is delayed to the end after the number of
                                      * consecutive association attempts on a single network reaches
                                      * the value of this parameter.)
                                      * Maximum value)
                                      */
    td_u8 max_assoc_time;            /* Maximum association duration
                                      * (for the scenario where the uplink and downlink are asymmetric
                                      * and the maximum association duration of a single network)
                                      */

    td_bool cco_snid_coordinate_switch; /* CCO network code coordination switch */
    td_bool sta_snid_coordinate_switch; /* STA network code coordination switch */
    td_bool conflict_detect_switch;     /* Conflict detection switch */
    td_bool monitor_associate_flag;     /* Listening association mode flag */

    td_u16 coordinate_backoff_time;      /* Multi-network conflict backoff duration */
    td_u16 bcn_send_period_for_star_net; /* Beacon sending period of the star network, in ms */

    td_u16 listen_time_for_white;   /* Monitoring timing time base on white list. Unit: s. */
    td_u16 exception_by_cco_mac_th; /* CCO MAC address exception threshold */
    td_u16 report_interval;         /* Report interval */
    td_u16 exception_interval;      /* Exception interval */
    td_u16 pad4;
    td_u16 pad;
} diag_cmd_config_coordinate_config_stru;

/*
2.34  CSMA Backoff Algorithm of the Transmitter
 */
typedef struct _diag_cmd_config_cmsa_backoff_pol_ {
    td_u16 default_node_count; /* Default number of surrounding sites == 10 */
    td_u16 default_slot;       /* ==100 */
    /* (*TX_CSMA_BACKOFF_BC_DEFAULT_SLOT_MULTI)ŁŹ */
    /* The value set here is multiplied by TX_CSMA_BACKOFF_BC_DEFAULT_SLOT_MULTI. */
    td_u8 fixed_bc;       /* Fixed backoff number for the highest priority = 4 */
    td_u8 min_node_count; /* Maximum number of sites in a small-scale conflict domain = 2 */
    td_u8 sm_scope_slot;  /* Slot value used when the surrounding sites are in a small range. = 120 */
    td_u8 sm_scope_bc;    /* Backoff value used when the surrounding is in a small range. =7 */

    td_u8 bg_scope_slot; /* Used around more than TX_CSMA_BACKOFF_BC_TABLE_ITEMS */
    td_u8 bg_scope_bc;   /* It is used for the BC when the number of surroundings
                          * exceeds TX_CSMA_BACKOFF_BC_TABLE_ITEMS.
                          */
    td_u8 bc_ratio_500_k;
    td_u8 bc_ratio_150_k;

    td_u8 bc_table[TX_CSMA_BACKOFF_BC_TABLE_ITEMS];
    td_u8 dbc_table[TX_CSMA_BACKOFF_DBC_TABLE_ITEMS];
    td_u8 bfs_table[TX_CSMA_BACKOFF_BFC_TABLE_ITEMS];
} diag_cmd_config_cmsa_backoff_algorithm3_stru;

typedef struct _diag_cmd_config_tx_csma_algorithm_ {
    td_u16 base_slot_150k; /* Unit: ms */
    td_u16 base_slot_500k; /* Unit: us */
    td_u16 wind_min;
    td_u16 wind_max;

    td_u16 backoff_time_150k_500k; /* The unit is us. */
    td_u16 collision_end;

    td_bool is_150k_500k_backoff_enable;
    td_bool is_preamble_interrupt_enable;
    td_u8 sta_slot_multi;
    td_u8 is_backoff_enable; /* New backoff policy */

    diag_cmd_config_cmsa_backoff_algorithm3_stru alg3;
} diag_cmd_config_tx_csma_algorithm_stru;

typedef struct _diag_cmd_config_assoc_config_ {
    td_u8 mac_auth_enable;              /* Start MAC authentication. */
    td_u8 heart_check_mode;              /* Set heartbeat detection mode */
    td_u8 no_sta_join_max_time_for_spc; /* Time required for determining that no site is
                                         * added during network identification, in seconds.
                                         */
    td_u8 fast_assoc_cnt_max;           /* Maximum number of fast association attempts.
                                         * If the number of fast association attempts exceeds the value of
                                         * this parameter and the STA does not access the network,
                                         * the STA enters the slow association state.
                                         */

    td_u16 pad2;
    td_u16 max_child_sum; /* Maximum number of substations */

    td_u8 no_sta_join_max_time;              /* Indicates the time required for the network status to change to
                                              * the networking completion state after the last site is added.
                                              * Unit: s
                                              */
    td_u8 assoc_gather_switch;               /* Correlation Indicator Summary Switch */
    td_u16 min_change_proxy_req_resend_time; /* Minimum time for waiting for the proxy change reply */

    td_u8 assoc_req_try_max;        /* Maximum number of association requests */
    td_u8 wait_gather_ind_time;     /* waiting time for generating summary packets, in seconds */
    td_u8 change_proxy_req_try_max; /* Maximum number of proxy change requests */
    td_u8 ind_resend_max_count;     /* Number of ind packet retransmissions */

    td_u8 slow_assoc_timeout;              /* Period of the slow association (unit: s) */
    td_u8 network_evaluation_time;         /* Evaluate the network time (unit: s) */
    td_u8 wait_assoc_ind_timeout_max_150k; /* Timeout interval for waiting for the association indication
                                            * at the 150 k/500 k frequency band, in seconds.
                                            */
    td_u8 random_assoc_array_num;          /* Number of randomly associated groups */

    td_u32 wait_for_mac_calculation_timeout; /* Time for calculating the table address */

    td_u8 wait_assoc_ind_timeout;     /* Time for waiting for the association indication packet, in seconds. */
    td_u8 max_assoc_req;              /* Maximum number of requests */
    td_bool random_assoc_enable;      /* Random association switch */
    td_u8 wait_assoc_ind_timeout_min; /* Minimum duration for broadband to wait for the association indication,
                                       * in seconds.
                                       */

    td_u16 assoc_not_in_whiter;  /* Duration for initiating an association request again after an association
                                  * failure because the STA is not in the whitelist
                                  */
    td_u16 assoc_not_set_whiter; /* Duration for initiating an association request again after an association
                                  * failure because no whitelist is configured on the network
                                  */

    td_u16 assoc_too_much_sta; /* Association Request Delay After Too Many Network Sites Fail to Be Associated */
    td_u16 assoc_too_much_pco; /* Duration for initiating an association request again after too many association
                                * failures of the proxy station
                                */

    td_u16 assoc_too_much_child; /* Duration for initiating an association request again after
                                  * too many association failures of a subsite
                                  */
    td_u16 assoc_too_much_level; /* Association Request Delay After Association Failure
                                  * Due to Layer Exceeding Maximum Limit
                                  */

    td_u16 assoc_repeat_mac; /* indicates the duration for initiating an association request
                              * again after a repeated MAC address association fails.
                              */
    td_u16 random_assoc_time_not_150k;

    td_u16 random_assoc_time_max_150k;     /* Maximum time for starting association randomly (ms) */
    td_u8 wait_assoc_ind_timeout_min_150k; /* Indicates the minimum timeout time for waiting for the
                                            * association indication in the 150 k/500 k frequency band. Unit: s.
                                            */
    td_u8 wait_gather_ind_time_150k;       /* Time for waiting for the generation of the association summary
                                            * packet at the 150 k/500 k frequency band (unit: 100 ms)
                                            */

    td_u16 white_leave_sta_th;     /* Threshold of the maximum number of sites that can be kicked out of the
                                    * whitelist at a time. If the threshold is exceeded, the CCO performs a
                                    * soft restart.
                                    */
    td_u16 white_table_notify_num; /* Number of offline notifications sent when whitelisted sites are
                                    * removed from the whitelist
                                    */
    td_u16 cco_send_leave_pk_time; /* Interval at which the CCO sends delay offline indication packets */
    td_u16 delay_leave_time_len;   /* STA restart delay */

    td_u8 heartbeat_leave_cycle_th;             /* Heartbeat detection offline period threshold */
    td_u8 heartbeat_report_cycle_th;            /* This parameter specifies the threshold of the site reporting
                                                 * period in the heartbeat detection. This parameter is not used.
                                                 */
    td_u8 route_cylce_per_heartbeat_cycle;      /* Number of routing periods included in the heartbeat period */
    td_u8 heartbeat_pk_cnt_per_heartbeat_cycle; /* Number of heartbeat packets sent in each heartbeat period */
} diag_cmd_config_assoc_config_stru;

typedef struct {
    td_u16 select_assoc_timeout;
    td_u16 fc_state_timeout;
    td_u16 beacon_state_timeout;
    td_u16 assoc_state_timeout;

    td_u8 neighbor_atten_th; /* neighbor network signal attenuation threshold */
    td_u8 neighbor_atten_en;
    td_u16 bcn_state_timeout_150k_with_security;

    td_u32 assoc_belong_lock_duration; /* Lock duration when the home network is associated, in seconds.
                                        * The default value is 180.
                                        */
    td_u32 join_belong_lock_duration;  /* Lock duration when a subscriber joins the home network,
                                        * in seconds. The default value is 0xFFFFFFFF.
                                        */
    td_u32 neighbor_auth_time; /* ľĽÎť: Ăë */
    td_u16 pad2[2];  /* 2ąŁÁôÎť */
} diag_cmd_config_multi_network_stru;

typedef struct _diag_cmd_config_tx_sof_tmi_policy_ {
    td_u32 default_72_sg : 4;     /* State Grid protocol PB72 retransmission replacement TMI disabled TMI, 14 */
    td_u32 default_136_sg : 4;    /* PB136 retransmission replaces the TMI disabled TMI, 4 */
    td_u32 default_520 : 4;       /* PB520 retransmission replaces the TMI disabled TMI, 0 */
    td_u32 default_72_p1901 : 4;  /* When the 1901 protocol is used, the default TMI used by the 72 blocks is 12. */
    td_u32 default_136_p1901 : 4; /* When the 1901 protocol is used, the default TMI used by 136 blocks is 4. */
    td_u32 resend_72_enable : 1;  /* PB136 Retransmission Replace TMI Switch, 1 */
    td_u32 resend_136_enable : 1; /* PB136 Retransmission Replace TMI Switch, 1 */
    td_u32 resend_520_enable : 1; /* PB520 Retransmission Change TMI Switch, 1 */
    td_u32 pad : 1;
    td_u32 tmi_72_cnt : 4;     /* The default value is 2. */
    td_u32 tmi_136_cnt_sg : 4; /* The default value is 2. */

    td_u32 tmi_520_cnt : 4;         /* The default value is 2. */
    td_u32 tmi_136_cnt_p1901 : 4;   /* The default value is 2. */
    td_u32 broadcast_72_sg : 4;     /* The default value is 14. */
    td_u32 broadcast_136_sg : 4;    /* The default value is 4. */
    td_u32 broadcast_520_sg : 4;    /* The default value is 9. */
    td_u32 broadcast_72_p1901 : 4;  /* The default value is 12. */
    td_u32 broadcast_136_p1901 : 4; /* The default value is 4. */
    td_u32 broadcast_520_p1901 : 4; /* The default value is 9. */

    td_u8 resend_72_sg[2];        /* 2 State Grid agreements pb72tmi, [13,14] */
    td_u8 resend_136_sg[2];       /* 2 national network protocols [6, 4] */
    td_u8 resend_520[2];          /* Two 520 TMIs [0, 9] */
    td_u8 resend_72_p1901[2];     /* 2 1901-compliant pb72tmi, [11,12] */
    td_u8 resend_136_p1901[2];    /* 2 1901 protocols [6, 4] */
    td_u8 tmi_auto_adjust_enable; /* Automatic adjustment of the SoF retransmission TMI, 1 */
    td_u8 degrade_resend_cnt;     /* Default value: 4 */

    td_u32 recover_tmi_period; /* Restore the default tmi timer period. 300000 */

    td_u8 broadcast_72_short : 4;
    td_u8 broadcast_136_short : 4;
    td_u8 broadcast_520_short : 4;
    td_u8 pad1 : 4;
    td_u8 add_bc_send_flag;
    td_u8 bc_send_limit;
} diag_cmd_config_tx_sof_tmi_policy_stru;

typedef struct _diag_cmd_config_bcn_beacon_tmi_ {
    td_u8 payload520_tmi_sg : 4;
    td_u8 payload136_tmi : 4;
    td_u8 payload72_tmi_sg : 4;
    td_u8 payload520_tmi_p1901 : 4;
    td_u8 payload72_tmi_p1901 : 4;
    td_u8 pad1 : 4;
    td_u8 pad;
} diag_cmd_config_bcn_beacon_tmi_stru;

typedef struct _diag_cmd_recode_rx_stat_config_stru_ {
    td_u8 dfx_rx_report_switch;
    td_u8 dfx_tx_report_switch;
    td_u8 is_rcv_nm_pkt_rpt : 1;
    td_u8 is_send_nm_pkt_rpt : 1;
    td_u8 pad2 : 6;
    td_u8 rcv_fc_switch;

    td_u16 multi_switch;
    td_u8 is_print_dis;
    td_u8 pad3;
} diag_cmd_recode_rx_stat_config_stru;

typedef struct {
    diag_cmd_recode_rx_stat_config_stru recode_rx_stat_policy;
} diag_cmd_config_dfx_config_stru;

typedef struct _diag_cmd_config_beacon_nv_ {
    td_u8 alloc_slot_oneself_flag; /* Whether to generate timeslots for data transmission when no timeslot
                                    * is available
                                    */
    td_u8 cco_update_beacon_slot_flag;
    td_u8 ptcl_detect_bcn_send_cnt; /* number of times that each protocol probe beacon is sent */
    td_u8 ptcl_detect_bcn_send_mode;

    td_u32 alloc_slot_time_th; /* Indicates the period after which a timeslot is generated if no beacon is received.
                                * Unit: ms.
                                */
    td_u32 alloc_slot_len;     /* Time slot duration (unit: us) */
    td_u32 protect_cp_len;
    td_u32 csma_slot_slice_th;      /* Minimum CSMA time segment unit. The unit is SLICE_SLOT_UNIT (10 ms). */
    td_u32 alloc_slot_len_for_star; /* Indicates the duration of timeslots generated on a star network. Unit: us. */
} diag_cmd_config_beacon_nv_stru;

typedef struct _diag_cmd_config_filter_switch_nv_stru_ {
    td_u32 filter_switch; /* Packet filtering switch */

    td_bool is_freq_filter_enable;
    td_u8 filter_timeout;
    td_u8 pad[2]; /* Reserved 2 bytes */
} diag_cmd_config_filter_switch_nv_stru;

typedef struct _diag_cmd_config_blind_check_nv_stru_ {
    td_bool blind_switch;
    td_u8 pad[3]; /* Reserved 3 bytes */

    td_u8 freq[8];                  /* 8 blind checks supported frequency bands */
    td_u8 blind_freq_check_time[8]; /* Eight periods corresponding to the blind detection frequency band,
                                     * corresponding to freq (unit: s)
                                     */

    td_u32 whole_network_change_freq_time; /* Indicates the interval after which the CCO notifies
                                            * the STA of the frequency band change. Unit: s
                                            */
} diag_cmd_config_blind_check_nv_stru;

#define MAX_SELECTED_ID_NUM 4
/* NV item of the NMR controller */
typedef struct _diag_cmd_config_ndm_connect_para_stru_ {
    td_u16 ndm_tei;
    td_u16 tl_big_packet_size; /* Indicates the maximum size of data that can be transmitted remotely at a time.
                                * Unit: byte.
                                */
    td_u8 work_mode;
    td_u8 report_search_result_period; /* Report the network search result. The unit is second. */
    td_u8 search_net_short_timeout;
    td_u8 result_expire_check_period;

    td_u8 cco_freq_scan_period; /* CCO frequency band listening period (unit: second) */
    td_u8 sta_freq_scan_period; /* CCO frequency band listening period (unit: second) */
    td_u8 ndm_wait_respond_max_time;
    td_u8 chl_pkt_use_highest_lid_time;

    td_u16 send_connect_req_period; /* Unit: ms */
    td_u16 heartbeat_period;

    td_u16 rpt_check_timer_cycle;
    td_u8 chl_tl_send_num; /* Number of send */
    td_u8 chl_tl_send_window;

    td_u32 chl_tl_retry_timeout;  /* Retry timeout period (unit: ms) */
    td_u32 chl_tl_rcv_timeout;    /* Receive timeout period (unit: ms) */
    td_u16 chl_tl_snd_interval;   /* Send interval (unit: ms) */
    td_u16 chl_tl_delay_ack_time; /* ACK message delay (unit: ms) */
    td_u8 not_join_net_resend_cnt;
    td_u8 is_auto_selected;
    td_u16 default_id;
    td_u16 selected_id_list[MAX_SELECTED_ID_NUM];
} diag_cmd_config_ndm_connect_para_stru;

typedef struct _diag_cmd_config_sta_gpio_set_para_stru_ {
    td_u8 tx_app_switch; /* 0 indicates that the GPIO pin is set to high when the MAC packet is transmitted,
                          * and 1 indicates that the GPIO pin is set to high when the APP packet is transmitted.
                          */
    td_u8 pad;
    td_u16 message_send_length; /* Time limit of STA pin pull-up (unit: us) */

    td_u32 pad1;
    td_u16 pad2;
    td_u16 pad3;
} diag_cmd_config_sta_gpio_set_para_stru;

typedef struct {
    td_u8 max_buff_num;
    td_u8 rx_queue_depth;
    td_u8 per_cor_num_bcn;
    td_u8 min_bcn_num;

    td_u8 save_proxy_info_rt_cnt;
    td_u8 opti_policy;
    td_u8 min_discovering_num;
    td_u8 wait_proxy_time; /* 等待代理发送信标时间,单位: 10s */

    td_u8 cco_check_heart_time; /* 心跳检测时间, 单位: 10s */
    td_u8 pco_rpt_heart_time;  /* PCO跳上报时间, 单位: 10s */
    td_u8 sta_send_heart_min_time; /* STA心跳上报最小间隔, 单位: 10s */
    td_u8 mesh_mode_time; /* 入网后判断MESH模式时间, 单位: 10s */

    td_u8 lock_net_mode; /* 锁定网络模式 */
    td_u8 select_assoc_timeout; /* 单位: 10s */
    td_u8 select_fc_timeout; /* 单位: 10s */
    td_u8 select_beacon_timeout; /* 单位: 10s */

    td_u8 assoc_state_timeout; /* 单位: 10s */
    td_u8 sta_offline_by_no_beacon_time; /* 单位: 10s */
    td_u8 pad[2]; /* 2 是长度 */
} diag_cmd_config_network_opti_params;

/* Set the communication rate reporting */
typedef struct {
    td_u8 report_switch; /* Report switch */
    td_u8 report_period; /* Report period (report_period: report once in each evaluation period) */
    td_u8 report_num;
    td_u8 pad;
    diag_cmd_config_network_opti_params opti_policy;
} diag_cmd_config_comm_rate_opti;

typedef struct {
    td_bool mac_ctrl_enable; /* Indicates whether to enable the duty cycle control policy of the MAC module.
                              * It is used for debugging.
                              */
    td_bool duty_enable;     /* Duty cycle policy enable */
    td_u8 duty_percent;      /* Duty cycle size */
    td_u8 pad;

    td_u32 max_sendlength; /* Maximum duration for sending packets, in ms */

    td_bool period1_duty_percent_enable; /* duty cycle statistics in period 1 */
    td_bool period2_duty_percent_enable; /* duty cycle statistics in period 2 */
    td_bool period3_duty_percent_enable; /* duty cycle statistics in period 3 */
    td_u8 pad1;

    td_u32 duty_period1; /* Length of period 1 (unit: ms) */
    td_u32 duty_period2; /* Period 2 length (unit: ms) */
    td_u32 duty_period3; /* Period 3 length (unit: ms) */
} diag_cmd_config_mac_duty_ctrl_para_stru;

typedef struct {
    td_u8 pad1[2];                        /* Reserved 2 bytes */
    td_u8 sync_timeout_th;                /* Object synchronization timeout interval 150s */
    td_u8 is_use_all_sta_sync_target : 1; /* whether to use all-STA synchronization object 1 */
    td_u8 is_neighbor_enable : 1;
    td_u8 pad : 6;
} diag_cmd_sel_sync_sta_policy_nv_stru;

typedef struct {
    td_u8 p2p_search_freq_times; /* search round */
    td_u8 p2p_connect_mode;      /* Connection mode */
    td_u8 send_req_max_num;      /* Maximum number of search frames transmitted on each frequency band */
    td_u8 is_show_p2p_log;       /* whether to display the P2P log information */

    td_u16 p2p_req_period; /* Search frame interval: ms */
    td_u16 rvsd;
} diag_cmd_config_ciu_ctrl_nv_stru;

typedef struct {
    td_u8 snr_collect_period;  /* Data collection period, in minutes. The default value is 15. */
    td_u8 snr_unit_fc_cnt_abs; /* Absolute threshold: indicates the threshold of each unit of time.
                                * The default value is 1.
                                */
    td_s8 snr_avg_th;          /* SNR average value threshold. The default value is -2. */
    td_u8 snr_unit_duration;   /* Unit time length, in seconds. The default value is 30. */

    td_u16 attenuation_th;    /* Attenuation threshold. When the channel attenuation is lower than this threshold,
                               * the preamble saturation threshold is not checked. The default value is 10.
                               */
    td_u8 fast_lock_duration; /* Fast network convergence: lock time after each network switchover,
                               * in minutes. The default value is 5.
                               */
    td_u8 pad;

    td_u8 fast_accurate_rounds[SNR_FAST_ACCURATE_TH_NUM]; /* Indicates the threshold of the number of periodic rounds.
                                                           * The default value is {1,2,3,4}.
                                                           */
    td_u8 fast_accurate_avgs[SNR_FAST_ACCURATE_TH_NUM];   /* SNR average value threshold.
                                                           * The unit is 0.1 dB. The default value is {3.2,1.6,0.8,0}.
                                                           */

    td_u16 pream_sta_num_th; /* Threshold for the number of saturable preamble symbols.
                              * If the number of SNRs exceeds the threshold,
                              * the excessive SNRs are directly discarded. The default value is 80.
                              */
    td_u8 alpha_numerator;   /* Average snr filter numerator. The default value is 1. */
    td_u8 alpha_denominator; /* Denominator of the average snr filter. The default value is 16. */
} diag_cmd_config_tf_stru;

/* Phase identification NV item */
typedef struct {
    td_bool is_pi_enable;         /* Phase identification enable. TD_TRUE */
    td_u8 ntb_query_parallel_num; /* Number of concurrent sites for NTB query, 3 */
    td_u8 ntb_query_turn_cnt_max; /* Maximum number of NTB query rounds. The value is 5. */
    td_u8 identify_fail_cnt_max;  /* Maximum number of phase identification failures.
                                   * The value ranges from 10 to 15.
                                   */

    td_u16 parallel_query_timeout; /* Parallel query timeout interval. The unit is ms and the value is 12000. */
    td_u16 check_over_timeout;     /* Timer for confirming the end of phase identification.
                                    * The unit is s and the value is 300.
                                    */

    td_u8 phase_cnt_th_min;          /* Minimum threshold for phase identification statistics, 15 */
    td_u8 phase_cnt_other_th_min;    /* Minimum threshold of the current phase when other phases are not zero. 10 */
    td_u8 phase_cnt_other_th;        /* threshold of other phases when other phases are not 0, 2 */
    td_u8 phase_cnt_no_other_th_min; /* minimum threshold of the current phase when other phases are 0 */

    td_bool is_report_zero_cross_ntb_req; /* switch for reporting zero-crossing query packets, TD_TRUE */
    td_bool is_report_zero_cross_ntb_rsp; /* Whether to report zero-response packets (TD_TRUE) */
    td_bool is_report_zero_cross_ntb_cco; /* Reference zero crossing NTB 1 of the reported CCO */
    td_u8 pad;

    td_u16 ntb_report_timer_period; /* Interval for the CCO to report cached NTBs, in seconds. */
    td_u16 cco_period_th;           /* CCO period judgment threshold (unit: ms) */
    td_u16 zc_collect_buff_num;     /* sampling buffer size */
    td_u8 zc_compare_num;           /* Number of zero-crossing points used to compare sizes */
    td_u8 pad1[5];                  /* pad(5) */
} diag_cmd_config_pi_config;

typedef struct {
    td_bool is_security_enable;
    td_bool is_random_negotiation;
    td_u16 rand_negotiation_time_max; /* unit: ms */

    td_u8 fast_negotiate_max_cnt;
    td_u8 slow_negotiate_timeout;        /* unit: s */
    td_u8 wait_server_hello_timeout;     /* unit: s */
    td_u8 wait_server_hello_timeout_min; /* unit: s */

    td_u8 wait_server_hello_timeout_max_150k;
    td_u8 set_nek_parallel_num;    /* maximum SECURITY_PARALLEL_NOTIFY_MAX_CNT */
    td_u8 parallel_notify_timeout; /* unit: s */
    td_u8 parallel_notify_turn_max_cnt;

    td_u32 nek_timeout; /* unit: s */
    td_u16 delay_update_nek_time;
    td_u16 negotiate_timeout; /* unit: s */

    td_u8 negotiatning_max_num;
    td_u8 negotiate_timeout_check_period;  /* unit: s */
    td_u16 send_interval_min_150k;         /* unit: ms */
    td_u32 rand_negotiation_time_max_150k; /* unit: ms */
    td_u32 first_nek_timeout_150k;         /* unit: s */
} diag_cmd_security_nv_stru;

#endif /* MAC_NV_H */
