/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: SOC_NV_UPG.
 * Author: CompanyName
 * Create: 2018-10-31
 */
#ifndef SOC_UPG_NV_H
#define SOC_UPG_NV_H

/*
 * @ingroup upg
 * Upgrade report parameters configure the NV item ID.
 */
#define EXT_UPG_REPORT_INFO_NV_ID 0x73

/*
 * @ingroup upg
 * Upgrade parameters configure the NV item ID.
 */
#define EXT_UPG_CFG_NV_ID 0x74

/*
 * @ingroup upg
 * Delay waiting configuration: \n
 * @li The default value for the 3.2M band is: {{1,0}, 2,10,3}.
 * @li 500K band corresponds to the default value: {{1,0}, 2,10,3}.
 * @li 150K band corresponds to the default value: {{1,0}, 2,10,3}.
 */
typedef struct {
    /*
     * Upgrade restart or power failure After power-on,
     * the number of sites is less than the number of sites at the time of initial upgrade,
     * and whether the delay wait policy is enabled.A value of 0 means that the delay wait policy is not enabled,
     * and other values indicate that the policy is enabled.
     */
    td_u8 enable : 1;
    td_u8 pad : 7;
    td_u8 period; /* < Periodically check whether the station has entered the full timer period, unit: min. */
    /*
     * The number of timer cycles included in one round.
     * The number of sites did not change during the waiting period.
     * Then no longer wait for the delay. When set to 0, the default value of 10 is taken.
     */
    td_u8 timer_cnt;
    /*
     * delay waiting for the maximum number of rounds.
     * After reaching the maximum number of rounds, no longer wait.
     */
    td_u8 loop_cnt;
} ext_upg_delay_wait_cfg;

/*
 * @ingroup upg
 * Query site information configuration: \n
 * @li The default value for the 3.2M band is: {60, 2, 5}.
 * @li 500K band corresponds to the default value: {60, 2, 5}.
 * @li 150K band corresponds to the default value: {60, 2, 16}.
 */
typedef struct {
    td_u8 loop_max;       /* <Query the maximum number of rounds sent. */
    td_u8 loop_interval;  /* <round interval, unit: s. */
    td_u16 send_interval; /* < concurrent frame interval, unit: 100ms. */
} ext_upg_qry_info_cfg;

/*
 * @ingroup upg
 * Full network broadcast configuration: \n
 * @li The default value for the 3.2M band is: {100,3,6,1,5, {0,5},0}.
 * @li 500K band corresponds to the default value: {100,3,6,1,5, {0,5},0}.
 * @li 150K band corresponds to the default value: {100,3,6,4,16,{0,5},0}.
 */
typedef struct {
    td_u8 succ_ratio;          /* <Transport completed site percentage. */
    td_u8 exception_max_count; /* <Maximum number of EXCEPTIONs tolerated. */
    td_u8 data_loop_max;       /* <Maximum number of rounds for data transmission. */
    td_u8 data_send_interval;  /* <Transmission interval between packet and packet, unit: 100ms. */
    td_u8 query_send_interval; /* <Query frame transmission interval, unit: 100ms. */
    /*
     * Whether to query the switch for the first time, the default value is 0,
     * indicating that the first round is not queried.
     * A value of 1 indicates the first round of the query,
     * and other values indicate that the first round is not queried.
     */
    td_u8 query_switch : 1;
    /*
     * Maximum number of queries per round. The maximum allowed setting is 25.
     * When it exceeds 25, the maximum value is 25.
     */
    td_u8 query_count : 7;
    td_u16 pad;
} ext_upg_broadcast_cfg;

/*
 * @ingroup upg
 * Complete notification configuration on the whole network: \n
 * @li The default value for the 3.2M band is {20,10,10,5}.
 * @li 500K band corresponds to the default value: {20,10,10,5}.
 * @li 150K band corresponds to the default value: {20,10,10,8}.
 */
typedef struct {
    td_u8 delayed_for_send_restart; /* <CCO restarts after the restart command is sent, unit: s. */
    /* <How long after the STA receives the restart command, the delay is restarted, unit: s. */
    td_u8 delayed_for_recv_to_restart;
    /* <Send several rounds of commands. When the value is 0, only one round is sent. */
    td_u8 send_count;
    td_u8 send_interval; /* <Transmission interval between packet and packet, unit: 100ms. */
} ext_upg_affirm_cfg;

/*
 * @ingroup upg
 * Start upgrading frame configuration: \n
 * @li The default value for the 3.2M band is: {5, 1, {5, 1, 1}}.
 * @li 500K band corresponds to the default value: {5, 1, {5, 1, 1}}.
 * @li 150K band corresponds to the default value: {8,10,{5,1,1}}.
 */
typedef struct {
    td_u8 bc_send_interval; /* <Transmission interval between broadcast packet and packet, unit: 100ms. */
    /* Transmission interval between unicast packets and packets, in units of 100ms. */
    td_u8 send_interval;
    /*
     * Broadcast transmission starts to upgrade the number of frame rounds.
     * When the value is 0, only one round is sent.
     */
    td_u16 bc_loop : 4;
    /* How long does it take to send data after the start of the broadcast send start frame, unit: s. */
    td_u16 bc_delayed : 6;
    td_u16 delayed : 6; /* <How long does it take to send a start frame to send data in unicast: unit: s. */
} ext_upg_notify_start_cfg;

/*
 * @ingroup upg
 * Concurrent unicast configuration: \n
 * @li The default value for the 3.2M band is: {100,10,10,1, 5,{1,3},0}.
 * @li 500K band corresponds to the default value: {100,10,10,1, 5,{1,3},0}.
 * @li 150K band corresponds to the default value: {100,10,10,4,16,{1,3},0}.
 */
typedef struct {
    td_u8 succ_ratio;          /* <Transport completed site percentage. */
    td_u8 exception_max_count; /* <Maximum number of EXCEPTIONs tolerated. */
    /*
     * The maximum number of rounds for data transmission.
     * When this number is reached, the exception is incremented by 1.
     * The exception is set when the exception reaches exception_max_count.
     */
    td_u8 data_loop_max;
    td_u8 data_send_interval;  /* <Transmission interval between packet and packet, unit: 100ms. */
    td_u8 query_send_interval; /* <Query frame transmission interval, unit: 100ms. */
    /*
     * Whether to query the switch for the first time, the default value is 1,
     * indicating the first round of the query. Taking other values means that the first round is not queried.
     */
    td_u8 query_switch : 1;
    /*
     * Maximum number of queries per round. The maximum allowed setting is 25.
     * When it exceeds 25, it is actually treated as 25.
     */
    td_u8 query_count : 7; /* bit(7) */
    td_u16 pad;
} ext_upg_unicast_cfg;

/*
 * @ingroup upg
 * Upgrade parameters.
 */
typedef struct {
    /*
     * Whether to use the switch of the whole network broadcast policy, the default value is 1,
     * indicating that the policy is turned on. A value of 0 means that the policy is turned off.
     */
    td_u8 bc_switch : 1;
    /*
     * Whether to use the switch of the concurrent unicast policy, the default value is 1,
     * indicating that the policy is turned on. A value of 0 means that the policy is turned off.
     */
    td_u8 switch_code : 1;
    /* <The number of concurrent channels when unicast is concurrent.
     * Default: 1. When set to 0, the default value is taken.
     */
    td_u8 max_ways : 6; /* bit(6) */
    /* < Waiting time after sending a round of query upgrade status message, unit: s, default value: 1. */
    td_u8 query_loop_interval;
    /* <Time to wait after sending a round of data completion, unit: s, default value: 1. */
    td_u8 data_loop_interval;
    td_u8 switch_schedule_delay;   /* <policy scheduling switch time, unit: 100ms, default: 10. */
    ext_upg_broadcast_cfg bc;       /* <Network broadcast policy configuration. */
    ext_upg_unicast_cfg uc;         /* <Concurrent unicast policy configuration. */
    ext_upg_notify_start_cfg start; /* <Start upgrade frame send configuration. */
    ext_upg_affirm_cfg affirm;      /* <The whole network completes the notification sending configuration. */
    ext_upg_delay_wait_cfg wait;    /* <delay waiting for configuration. */
    ext_upg_qry_info_cfg qry;       /* <Query the site information frame transmission configuration. */
} ext_upg_cfg;

/*
 * @ingroup upg
 * Upgrade parameter configuration NV item structure: cfg[0] is configured with 3.2M band parameters,
 * cfg[1] is configured with 500K band parameters, and cfg[2] is configured with 150K band parameters.
 */
typedef struct {
    ext_upg_cfg cfg[3]; /* 3 count */
} ext_upg_cfg_nv;

typedef struct {
    td_u16 num_per_pack;
    td_u16 interval;
} upg_report_para;

typedef struct {
    upg_report_para status[2]; /* 2 count */
} ext_upg_report_para_nv;

#endif /* __SOC_UPG_NV_H__ */
