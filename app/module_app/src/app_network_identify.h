/*
 * Copyright (c) CompanyNameMagicTag 2019-2020. All rights reserved.
 * Description: decleration of self identify functions
 */

#ifndef _APP_NETWORK_IDENTIFY_H_
#define _APP_NETWORK_IDENTIFY_H_
#include "dfx_app.h"
#include "soc_mdm_mac.h"

#define APP_NI_MAX_QRY_CLCYE           5      /* Maximum query round */
#define APP_WAIT_RESPOND_MAX_TIME      6000   /* unit : ms */
#define APP_PER_STA_USE_BIT_NUM        2      /* 0bit: is same network, 1bit: is cco receive respond */
#define APP_NI_CCO_TEI                 1
#define APP_START_QRY_TEI              2      /* Start sta TEI */
#define APP_NI_WAIT_NEW_STA_MIN_TIME   1      /* Minimum waiting time for a site to access the network.Unit: minute. */
#define APP_DEFAULT_WAIT_STA_TIME      10
#define APP_NI_MS_PER_MIN              60000
#define is_sta_tei_valid(tei)          ((tei) >= APP_START_QRY_TEI && (tei) < EXT_MDM_NM_TOPO_NODE_NUM_MAX)
#define APP_COUNT_ROLLBACK             0xFFFFFFFF
/* Maximum number of concurrent connections. The number of concurrent connections
   increases by one, A timer ID needs to be added. */
#define APP_NI_MAX_PARAL_NUM           5
#define APP_NODE_NUM_ONE               1
typedef enum {
    APP_NI_INIT_STATE = 0,      /* Initialization state */
    APP_NI_IDLE_STATE,          /* Idle state  */
    APP_NI_QUERY_STATE,         /* Query status  */
    APP_NI_END_QRY_STATE,       /* End query status */
    APP_NI_STOP_STATE,          /* Stop status  */
    APP_NI_MAX_STATE
} app_ni_state;

typedef enum {
    APP_NI_WL_INVALID = 0,      /* Initialization state */
    APP_NI_WL_UNUSED = 1,
    APP_NI_WL_EFFECTIVE = 2,
} app_ni_white_list_state;

typedef struct {
    td_u8 mac_addr[EXT_PLC_MAC_ADDR_LEN];  /* The site waiting for the current timer */
    td_u16 pad;
} app_parall_timer_info;

typedef struct {
    td_u8   cur_qry_cycle;         /* Current query round */
    td_bool cur_state;             /* Current status */
    td_u8   current_parallel_num;  /* Number of current concurrent connections. */
    td_u8   wait_new_sta_join_time; /* Waiting for new site join time */

    td_u16  max_tei;
    td_bool white_list_state;
    td_u8   total_parallel_num;    /* Maximum number of concurrent NV configuration,range:[1,5] */

    td_u16  frame_index;           /* Send index */
    td_u16  next_qry_sta_tei;      /* TEI of the next site to be queried */
    td_u32  time_diff_nv;          /* Power-on time difference */

    td_u8   *sta_qry_info;               /* Save query result index */
    ext_mdm_nm_topo_node_info *topo_list; /* This command is used to obtain the topo structure index */
    app_parall_timer_info *timer_ctrl;   /* Concurrency timer control */

    td_bool white_list_switch;
    td_u8 pad[3]; /* 3 is pad num */
} app_network_identify_ctrl;

td_u32 app_ni_init(td_void);
td_u32 app_ni_set_identify_state(td_bool is_enable);
td_void app_ni_cco_formed_notify(td_void);
td_void app_ni_new_sta_join_notify(td_u16 tei);
td_void app_ni_restart_querying_sta_poweron_time(td_void);
td_void app_ni_wait_sta_rsp_timeout(td_u32 timer_id);
td_void app_ni_handle_wait_sta_join_timeout(td_void);
td_u32 app_ni_handle_rcv_sta_rsp(td_pbyte data, td_u16 data_length, td_pbyte src_addr, td_u8 addr_len);
td_u32 app_on_msg_ni_send_frame(ext_sys_queue_msg *msg);
diag_cmd_ni_dfx_info *app_ni_get_dfx_info(td_void);
td_void app_ni_update_dfx_info(td_void);
td_void app_ni_open_white_list_switch(td_void);
#endif
