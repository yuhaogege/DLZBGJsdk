/*
 * Copyright (c) CompanyNameMagicTag 2012-2018. All rights reserved.
 * Description: APP NV structure define.
 * Author: CompanyName
 * Create: 2012-12-15
 */

#ifndef __APP_NV_H__
#define __APP_NV_H__

#include "soc_types.h"

/* Device address information */
#define ID_NV_APP_DEV_ADDR_INF 0x8a
/* Remote upgrade information */
#define ID_NV_APP_UPG_INF 0x8d
/* Virtual table */
#define ID_NV_APP_SIMU_METER 0xa0
/* Transformer district identification, which is valid only for CCO. */
#define ID_NV_APP_MRS_CCO_TF 0xa1
/* Transformer district identification, which is valid only for STAs. */
#define ID_NV_APP_MRS_STA_TF 0xa2

#define ID_NV_APP_MOUDULE_CCO_IDENTIFY 0xb1
typedef struct {
    td_u8 dev_addr[EXT_PLC_MAC_ADDR_LEN]; /* *< Device Address */
    td_u8 rsv[10];                       /* Reserved 10 bytes */
} app_dev_addr_inf;

typedef struct {
    td_u32 filc_crc; /* *< Upgrade ID */
    td_u8 file_attr; /* *< Upgrade type. The options are as follows: 0: default; 1: local upgrade; 2: remote upgrade. */
    td_u8 rsv[3];    /* Reserved 3 bytes */
} app_upg_inf;

typedef struct {
    td_u8 simu_meter;
    td_u8 simu_evt;
    td_u8 simu_evt_delay;
    td_u8 rsv;
} mrs_simu_meter_inf;

typedef struct {
    td_u32 identify_duration; /* Recognition duration, in minutes,default:120 */
    td_u32 report_duration;   /* Duration for reporting concentrators and querying STAs, in minutes,default:60 */

    td_u16 start_cmd_interval; /* Indicates the interval for sending the command
                                * for starting to search for meters. Unit: s,default:180
                                */
    td_u16 stop_cmd_interval;  /* Indicates the interval for sending the command for
                                * stopping searching for meters. Unit: s,default:60
                                */

    td_u16 delay_white_list_duration; /* Interval for enabling the whitelist after a delay, in seconds,default:1200 */
    td_u16 query_sta_interval;        /* Interval for querying STA identification results, in seconds,default:2 */

    td_u16 refrash_topo_interval; /* Interval for refreshing the topology, in seconds,default:120 */
    td_u16 query_sta_max_way;     /* Number of STAs to be queried at the same time. The default value is 3. */

    td_u8 query_sta_timeout;      /* This object indicates the timeout period of the STA,
                                    in seconds. The default value is 12. */
    td_u8 query_max_cnt;          /* Maximum number of query times for each STA
                                   * when the STA fails to be queried. The default value is 50.
                                   */
    td_u8 identify_duration_rate; /* Percentage of user input time spent identifying duration (default: 67%) */
    td_u8 pad2;
} mrs_cco_tf_inf;

typedef struct {
    td_u16 tf_check_period;               /* Self-check period of the STA's meter searching status,
                                            in seconds,defaut:60 */
    td_u16 tf_stop_while_not_receive;     /* Duration in which the transformer district identification
            function is disabled if the sta does not receive the startup command. The unit is second,defaut:1800 */
    td_u16 tf_unlock_while_leave_network; /* Duration for unlocking a subscriber who is deregistered.
                                            The unit is second. The default value is 900. */
    td_u16 tf_start_interval; /* Interval between two consecutive times that the transformer district identification
                               * function is enabled for the STA, in seconds. The default value is 600.
                               */
} mrs_sta_tf_inf;

/* After the NV is modified, restart it for the modification to take effect */
typedef struct {
    td_u8 is_network_idf_enable; /* Automatic networking identification flag */
    td_u8 wait_new_sta_max_time; /* Waiting for the STA to access the network.Unit: minute.  */
    td_u8 parallel_num;          /* aximum number of concurrent connections,range: [1,5] */
    td_u8 rvsd;
    /* Specifies the maximum power-on time difference.
       It is used to differentiate the local network from the non-local network.The unit is millisecond */
    td_u32 time_diff;
} module_cco_identify_inf;
#endif /* __APP_NV_H__ */
