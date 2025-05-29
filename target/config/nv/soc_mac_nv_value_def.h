/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: NV-Related IDs and Structure Definitions
 */
/* NV ID */
#ifndef NV_SOC_MAC_NV_VALUE_DEF_H
#define NV_SOC_MAC_NV_VALUE_DEF_H

#include "soc_types.h"
#include "soc_phy_nv.h"

typedef td_u16 id_diag_config;

#define ID_DIAG_CMD_CONFIG_IF 0x40                       /* Frame interval parameters */
#define ID_DIAG_CMD_CONFIG_TIMEOUT_ON_PRI 0x41           /* Packet Expiration Time */
#define ID_DIAG_CMD_CONFIG_BUFFER_PERCENT_ON_PRI 0x42    /* Size of the sending queue */
#define ID_DIAG_CMD_CONFIG_LED_NV 0x43                   /* Light control module parameters */
#define ID_DIAG_CMD_NDM_PIPE_NV 0x44                     /* Reader channel parameters */
#define ID_DIAG_CMD_CONFIG_ASSOC_CONFIG 0x45             /* Associated parameters */
#define ID_DIAG_CMD_CONFIG_PHASE_ATTR 0x46               /* Phase module parameters */
#define ID_DIAG_CMD_CONFIG_TDMA_CONFIG 0x47              /* TDMA scheduling parameters */
#define ID_DIAG_CMD_CONFIG_ROUTE_ATTRIB 0x48             /* Route evaluation parameters */
#define ID_DIAG_CMD_CONFIG_TX_CSMA_ALGORITHM 0x49        /* CSMA backoff parameters */
#define ID_DIAG_CMD_CONFIG_TX_SOF_TMI_POLICY 0x50        /* TMI sending policy parameters */
#define ID_DIAG_CMD_CONFIG_BEACON_TMI 0x51               /* Beacon frame TMI selection parameter */
#define ID_DIAG_CMD_CONFIG_DFX_CONFIG 0x52               /* Maintainability and Testability Switch Parameters */
#define ID_DIAG_CMD_BEACON_SLOT_CONFIG 0x53              /* Beacon timeslot parameters */
#define ID_DIAG_CMD_FILTER_SWITCH_CONFIG 0x54            /* Packet filtering switch parameters */
#define ID_DIAG_CMD_CONFIG_STA_BLIND_CHECK 0x55          /* Frequency band blind detection parameters */
#define ID_DIAG_CMD_CONFIG_NDM_CONNECT_PARA 0x56         /* Reader connection parameters */
#define ID_DIAG_CMD_CONFIG_RT_CYCLE_SELF_ADAPT_PARA 0x57 /* Routing period parameters */
#define ID_DIAG_CMD_STA_GPIO_NV 0x58                     /* GPIO pin configuration parameters and key node \
                                                          * positioning parameters of the STA              \
                                                          */
#define ID_DIAG_CMD_CONFIG_MULTI_EXCEPTION_NV 0x59       /* Multi-network exception detection parameters */
#define ID_DIAG_CMD_CONFIG_COMM_RATE 0x60         /* Communication rate reporting parameters */
#define ID_DIAG_CMD_CONFIG_DUTY_PARA_CONFIG 0x62         /* Duty cycle control parameter (for test) */
#define ID_DIAG_CMD_CONFIG_EMC_PARA 0x63                 /* EMC certification parameters */
#define ID_DIAG_CMD_NTB_PARA 0x64                        /* V200-NTB parameters */
#define ID_DIAG_CMD_CONFIG_COORDINATE_CONFIG 0x65        /* Multi-network coordination parameters */
#define ID_DIAG_CMD_CONFIG_ROUTE_PROXY 0x66              /* Static Route Configuration */
#define ID_DIAG_CMD_MULTI_NETWORK_ASSOC_CONFIG 0x67      /* Multi-network association parameters */
#define ID_DIAG_CMD_TF_CONFIG 0x68                       /* Transformer district identification NV */
#define ID_DIAG_CMD_P2P_CTRL_CONFIG 0x69                 /* P2P Configuration */
#define ID_DIAG_CMD_CFG_PI_CONFIG 0x70                   /* phase identification parameter */
#define ID_DIAG_CMD_SECURITY_CONFIG 0x71                 /* Protocol security parameters */
/* Maximum NV. The value must be changed accordingly. Otherwise, the registration fails. */
#define ID_DIAG_CMD_MAC_NV_MAX ID_DIAG_CMD_SECURITY_CONFIG

/* NV setting */
#define SNR_TRANS_CLASS_NUM 8    /* Number of transmission SNR layers */
#define SNR_COLLECT_CLASS_NUM 16 /* Count the number of SNR levels. */

#define SNR_FAST_ACCURATE_TH_NUM 4        /* Number of high precision thresholds for fast convergence */
#define SNR_FAST_ACCURATE_AVG_PECISION 10 /* Accuracy of the average SNR of the high precision \
                                           * threshold for fast convergence                    \
                                           */

typedef enum {
    MAC_IFS_FREQ_TRY_ORDER_OTHER_INDEX = 0,
    MAC_IFS_FREQ_TRY_ORDER2_INDEX,
    MAC_IFS_FREQ_TRY_ORDER3_INDEX,
    MAC_IFS_FREQ_TRY_MAX_INDEX = 3,
} diag_mac_ifs_cfg_index;

#define TX_CSMA_BACKOFF_BC_TABLE_ITEMS 20
#define TX_CSMA_BACKOFF_DBC_TABLE_ITEMS 8
#define TX_CSMA_BACKOFF_BFC_TABLE_ITEMS 8
#define TX_CSMA_BACKOFF_DBC_MAX_TIME 20 /* Empirical values need to be obtained through tests. */
#define TX_CSMA_BACKOFF_BC_DEFAULT_SLOT_MULTI 5
#define TX_CSMA_BACKOFF_BC_CCO_SLOT_MULTI 2

#ifdef SAL_HAVE_FEATURE_UT
#define ID_DIAG_CMD_MAC_WHITE_ITEM_MAX 150
#else
/*
 * 2048. The DBK uses this value, which is associated with
 * EXT_MAC_VALID_MAC_LIST_MAX and EXT_BIDB_ITEM_NUM_MAX.
 */
#define ID_DIAG_CMD_MAC_WHITE_ITEM_MAX 64

typedef struct {
    td_u8 static_proxy_mac[EXT_PLC_MAC_ADDR_LEN]; /* Specifies the static relay MAC address. */
    td_bool my_static_proxy_exist;               /* Whether to set a specified static trunk. */
    td_bool to_be_static_repeater;               /* Indicates whether the local site is
                                                  * a static regeneration site.
                                                  */
} diag_cmd_config_route_proxy_stru;

/* *
 * 1.4    Setting the timeout period of packets with different priorities
 */
typedef struct {
    td_u32 timeout : 24; /* Timeout */
    td_u32 priorty : 8;  /* Priority */
} diag_cmd_config_timeout_on_pri_entry_stru;

#define ID_DIAG_CMD_CONFIG_TIMEOUT_ON_PRI_ENTRY_COUNT 10

typedef struct _diag_cmd_config_timeout_on_pri_stru_ {
    td_u8 num;    /* Set the number of items (n). */
    td_u8 pad[3]; /* Reserved 3 bytes */
    /* Setting item entity */
    diag_cmd_config_timeout_on_pri_entry_stru entry[ID_DIAG_CMD_CONFIG_TIMEOUT_ON_PRI_ENTRY_COUNT];
} diag_cmd_config_timeout_on_pri_stru;

/* *
 *
 * 1.5    Ratio of buffers of different priorities
 *
 */
typedef struct {
    td_u8 priorty;        /* Queue Priority */
    td_u8 cco_queue_size; /* Limit on the number of packets in a CCO queue */
    td_u8 sta_queue_size; /* Limit on the number of packets in a STA queue */
    td_u8 pad;
} diag_cmd_config_buffer_percent_on_pri_entry_stru;

#define ID_DIAG_CMD_CONFIG_BUFFER_PERCENT_ON_PRI_ENTRY_COUNT 10

typedef struct _diag_cmd_config_buffer_percent_on_pri_stru_ {
    td_u8 num;
    td_u8 pad[3]; /* Reserved 3 bytes */
    diag_cmd_config_buffer_percent_on_pri_entry_stru entry[ID_DIAG_CMD_CONFIG_BUFFER_PERCENT_ON_PRI_ENTRY_COUNT];
} diag_cmd_config_buffer_percent_on_pri_stru;

/* *
 *
 * 1.8    Setting the MAC address whitelist (for CCO and association requests)
 *
 */
typedef struct _diag_cmd_config_route_attrib_stru_ {
    /*
     * This parameter is not used currently.
     * Use the adaptive evaluation period parameter.
     */
    td_u16 rt_evaluate_cycle_time_len;
    td_bool is_evaluate_proxy_before_join_chl; /* Enable Proxy Evaluation Before Network Access
                                                * Based on Channel Quality
                                                */
    td_u8 rt_down_comm_rate_basic_limit;       /* Basic threshold of the downlink communication rate */

    td_u8 rt_up_comm_rate_basic_limit; /* Basic threshold of the uplink communication rate */
    td_u8 rt_choose_proxy_upper_limit; /* Max. Proxy Selection Communication Rate */
    td_u8 rt_choose_proxy_lower_limit; /* Lower limit of agent selection communication rate */
    td_u8 rt_continue_better_th;       /* The agent selection is continuously better than the change threshold. */

    td_u8 rt_high_diff;             /* Upper limit change threshold */
    td_u8 rt_middle_diff;           /* Threshold for changing the upper and lower thresholds */
    td_u8 rt_low_diff;              /* Lower Change Threshold */
    td_u8 is_path_table_dfx_enable; /* Path Beacon Table Maintenance Switch */

    td_u32 rt_proxy_far_near_alg_switch : 1; /* Proxy Layer Near- and Far-End Identification Algorithm Switch */
    td_u32 rt_stat_beacon_switch : 1;        /* Use Beacon Frames as the Basis for
                                              * Communication Rate Statistics
                                              */
    td_u32 rt_far_or_near_alg_swith : 1;     /* Algorithm Switch for Identifying Near- and Far-End Layers */
    td_u32 rt_lower_level_switch : 1;        /* You can select a lower-layer discovery
                                              * site as the candidate proxy switch.
                                              */
    td_u32 rt_channel_info_switch : 1;       /* Channel Capacity Used as Entry Condition for Proxy Evaluation */
    /*
     * Indicates whether to disable the function of
     * limiting the uplink communication rate.
     */
    td_u32 rt_up_comm_rate_redundancy_switch : 1;
    /*
     * The site with a low communication rate is
     * not selected as the candidate proxy switch.
     */
    td_u32 rt_less_than_lower_limit_switch : 1;
    /*
     * When the proxy communication rate is higher than the
     * upper limit, you can select a site with a higher
     * communication rate as the proxy switch.
     */
    td_u32 rt_more_than_upper_limit_switch : 1;
    td_u32 rt_mult_path_switch : 1;      /* Multipath Evaluation Switch */
    td_u32 rt_get_multi_path_switch : 1; /* Obtaining the Multipathing Switch */
    td_u32 rt_snr_diff : 8;
    td_u32 pad_1 : 14;

    td_bool rt_convergence_flag; /* Route Aggregation Switch Flag */
    /*
     * Minimum period for sending beacons intensively and
     * collecting communication rate data quickly, in seconds.
     */
    td_u8 min_first_evaluate_period;
    td_u8 rt_proxy_send_discovery_pk_len; /* Interval for the agent to send the discovery list, in seconds. */
    td_u8 rt_sta_send_discovery_pk_len;   /* Interval at which a common site sends the discovery list (unit: s) */

    td_u8 slide_window_weight[4]; /* 4 Window Weight */

    td_bool is_star_disc_enable;         /* Display Enable Flag in Star Network Discovery List */
    td_u8 enforcing_proxy_change_enable; /* Whether forcible proxy change is enabled */
    td_u16 enforcing_proxy_change_cycle; /* Specifies the forcible proxy change period, in minutes. */
} diag_cmd_config_route_attrib_stru;

#define RT_CYCLE_PARA_CNT 6

typedef struct _diag_cmd_config_rt_cycle_seif_adaptive_attrib_str_ {
    td_u16 station_cnt;        /* Number of sites */
    td_u16 evaluate_cycle_len; /* Indicates the evaluation period, in seconds. */

    td_u8 proxy_send_dk_cycle_len; /* This object indicates the frequency at which the proxy
                                    * station sends discovery list packets, in seconds.
                                    */
    td_u8 sta_send_dk_cycle_len;   /* Interval at which the discovery site sends discovery list packets,
                                    * in seconds.
                                    */
    td_u8 pad[2];                  /* Reserved 2 bytes */
} diag_cmd_config_rt_cycle_seif_adaptive_attrib_str;

typedef struct _diag_cmd_config_route_adaptive_attrib_stru_ {
    /*
     * Number of sites on the network. The value 0 indicates that parameters
     * are dynamically selected based on the number of sites on the network.
     */
    td_u16 rt_policy;
    td_u16 adaptive_redundance_para; /* Periodic adaptive parameter redundancy,
                                      * preventing periodic parameter instability
                                      */
    td_u8 multi_net_flag;
    td_u8 pad[3]; /* Reserved 3 bytes */
    diag_cmd_config_rt_cycle_seif_adaptive_attrib_str cycle_self_adaptive[RT_CYCLE_PARA_CNT];
} diag_cmd_config_route_adaptive_attrib_stru;

/* Reader channel parameter configuration */
typedef struct {
    td_u16 fragment_sleep_time;   /* Time of waiting for the failure to obtain the MacBuffer of the packet data */
    td_u8 fragment_resend_cnt;    /* Maximum number of retransmissions when
                                   * the packet data fails to obtain the MACBuffer
                                   */
    td_u8 fc_pkt_num_max_per_req; /* Maximum number of response packets in each request
                                   * in the flow control channel.
                                   */

    td_s16 proxy_snr_threshold;      /* Indicates the average SNR threshold of the proxy site used by the NCR. */
    td_u16 fc_rsp_wait_timer_period; /* Maximum waiting time for response timeout */

    td_u32 fc_req_wait_timer_period; /* Maximum processing time of a request */

    td_u8 fc_send_times_per_req;  /* Number of times that each request packet is sent */
    td_u8 fc_send_times_per_rsp;  /* Number of times that each response packet is sent */
    td_u8 ndm_remote_send_switch; /* Indicates whether to enable the non-IP communication on the NCR.
                                   * 1: enabled; 0: disabled.
                                   */
    td_u8 pad;
} diag_cmd_config_ndm_pipe_nv_stru;

/*
 * Frame interval configuration
 */
typedef struct _diag_cmd_config_if_stru_ {
    td_u16 pifs[MAC_IFS_FREQ_TRY_MAX_INDEX]; /* Unit: us */
    td_u16 rifs[MAC_IFS_FREQ_TRY_MAX_INDEX]; /* Unit: us */
    td_u16 cifs[MAC_IFS_FREQ_TRY_MAX_INDEX]; /* Unit: us */
    td_u16 bifs[MAC_IFS_FREQ_TRY_MAX_INDEX]; /* Unit: us */

    td_u16 eifs[FREQ_MAX_CNT]; /* Unit: ms */

    td_u16 ack_delay_durtion; /* Indicates the time for responding to the SACK packet.
                               * The default value is 60 us.Unit: us
                               */
    td_u16 time_redundancy;   /* Unit: us */

    td_u8 slot_dfx_switch; /* Timeslot Maintenance Switch */
    td_u8 pad1;
    td_u16 mesh_delay;
} diag_cmd_config_if_stru;

typedef struct {
    td_u16 nbi_start;
    td_u16 nbi_end;
} diag_cmd_config_avoid_freq_entry;

typedef struct {
    td_bool emc_switch; /* EMC authentication switch */
    td_u8 report_static_cyc;
    td_u8 avoid_freq_num;         /* Number of frequency bands for dynamic avoidance */
    td_u8 avoid_freq_mask_extend; /* Number of extended subcarriers for dynamic frequency avoidance */
    /*
     * After the frequency interference disappears,
     * the avoidance duration needs to be extended. The unit is second.
     */
    td_u16 freq_avoid_extend_time;
    td_u16 power_control_pk_cycle; /* Power control packet period, in seconds. */

    td_u16 emc_periodic_detect_cycle;  /* EMC detection period, in milliseconds. */
    td_u8 send_fail_th_power_recovery; /* Threshold for packet transmission failures during power recovery */
    td_u8 power_adjust_step;           /* Power Adjustment Step */

    td_u8 max_channel_dec_thresh;  /* Maximum channel attenuation */
    td_u8 min_channel_dec_thresh;  /* Maximum channel attenuation */
    td_u8 white_noise_detect_th;   /* White Noise Scenario Threshold */
    td_u8 is_enable_lab_nbi_judge; /* Whether to enable the NBI judgment threshold in the lab */

    td_u8 fft_num_th;
    td_u8 power_control_nv_switch;         /* Power Control Switch */
    td_u8 send_fail_recovery_power_switch; /* Power Control Triggered by Transmission Failure */
    td_u8 noise_pow_target;                /* noise target power */

    td_u8 powerloading_merge_th; /* Slot combination threshold */
    td_u8 lab_nbi_judge_th;      /* Threshold for the number of consecutive
                                  * confirmation times in the NBI lab scenario
                                  */
    td_u8 lab_nbi_max;           /* NBI Threshold for Determining a Lab Scenario */
    td_u8 default_afe_power;     /* Default AFE power in EMC mode: 8 */

    td_u32 test_pk_cycle;

    td_u16 test_pk_size;
    td_u8 test_pk_num;     /* Indicates the number of test packets sent per second. */
    td_u8 avoid_detect_th; /* Dynamic Frequency Avoidance Detection Times Threshold */

    td_u16 sat_num_th;
    td_u16 pad1;

    diag_cmd_config_avoid_freq_entry freq[5]; /* frequency band entry is of 5 items */
} diag_cmd_config_emc_para_stru;
#endif
#endif
