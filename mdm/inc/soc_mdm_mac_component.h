/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Mac component interface
 */
#ifndef __SOC_MDM_MAC_COMPONENT_H__
#define __SOC_MDM_MAC_COMPONENT_H__

#include "soc_types.h"
#include "soc_mdm_msg.h"
#include "soc_list.h"
#include "dfx_interface.h"

typedef struct {
    td_u16 cmd_id;
    td_u16 cmd_param_size;
    td_pvoid cmd_param;
    td_u32 option;
} ext_cmd_req_para_st;

typedef td_u32 (*mc_get_stat_fun)(td_void *ind_payload, td_u16 ind_size,
    EXT_CONST ext_cmd_req_para_st *req_para);
typedef td_void (*mac_component_init_fun)(td_void);
typedef td_void (*mac_component_reinit_fun)(td_void);
typedef td_u32 (*mac_component_handle_msg_fun)(EXT_CONST ext_sys_queue_msg *queue_msg);
typedef td_u32 (*mac_component_refresh_nv_fun)(td_u16 nv_id);
typedef td_u32 (*mac_component_get_stat_fun)(td_u16 cmd_id, mc_get_stat_fun *get_stat_if, td_u16 *ind_size);
typedef td_void (*mac_component_periodic_proc_fun)(td_void);

typedef struct {
    EXT_LIST node;
    td_u32 component_id; /* Identifier of a component. */

    mac_component_init_fun mac_cp_init_if;                   /* Component initialization interface */
    mac_component_reinit_fun mac_cp_reinit_if;               /* Component re-initialization interface */
    mac_component_handle_msg_fun mac_cp_handle_msg_if;       /* Interface for processing component messages */
    mac_component_refresh_nv_fun mac_cp_refresh_nv_if;       /* This interface is used to update NV items of components.
                                                              * The NV items need to be read and updated. */
    mac_component_get_stat_fun mac_cp_get_stat_if;           /* Interface for components to obtain
                                                              * maintainability and testability */
    mac_component_periodic_proc_fun mac_cp_periodic_proc_if; /* Interface for periodically executing components */
} ext_mac_component_info_st;

/*
 * Phase identification (PI) component definition
 * Component ID = 1001
 * Component message ID range:[10001, 10050]
 */
#define MC_PI_ID     1001
#define MC_PI_MSG_ID 10001

/*
 * Transport layer component definition,TL(transport layer)
 * Component ID = 1002
 * Component message ID range:[10051, 10100]
 */
#define MC_TL_ID           1002
#define MC_TL_MSG_ID       10051
#define MC_TL_SEND_MSG_ID  10052
#define MC_TL_TIMER_MSG_ID (MC_TL_MSG_ID + 1) /* Transport layer timeout timer */

/*
 * Definition of the power failure reporting component,PF(power failure)
 * Component ID = 1003
 * Component message ID range:[10101, 10150]
 */
#define MC_PF_ID     1003
#define MC_PF_MSG_ID 10101

/*
 * CIU component definition
 * Component ID = 1004
 * Component message ID range:[10151, 10200]
 */
#define MC_P2P_ID                     1004
#define MC_P2P_MSG_ID                 10151
#define MC_P2P_MSG_BASE_CYCL_TIMER    (MC_P2P_MSG_ID + 1)  /* P2P communication periodic timer */
#define MC_P2P_MSG_RCV_RSP            (MC_P2P_MSG_ID + 2)  /* receives the search respond frame. */
#define MC_P2P_MSG_RCV_REQ            (MC_P2P_MSG_ID + 3)  /* receives the search frame req. */

/*
 * Definition of the TF component
 * Component ID = 1005
 * Component message ID range:[10201, 10250]
 */
#define MC_TF_ID                               1005
#define MC_TF_MSG_ID                           10201
#define MC_TF_MSG_ID_SNR_COLLECT_CYCLE_TIMEOUT (MC_TF_MSG_ID + 1) /* The data statistics period expires. */
#define MC_TF_MSG_ID_SNR_RESTART_COLLECT_CYCLE (MC_TF_MSG_ID + 2) /* Restart identification period */
#define MC_TF_MSG_ID_SET_TF_MODE               (MC_TF_MSG_ID + 3) /* Setting the transformer district
                                                                     identification mode */
#define MC_TF_MSG_ID_CHECK_FC_BCN_TIMEOUT      (MC_TF_MSG_ID + 4) /* Timer for the neighbor fc
                                                              * beacon to receive information */

/*
 * SPC_IDEFTIFY Component Definition
 * Component ID = 1006
 * Component message ID range:[10261, 10300]
 */
#define MC_SPC_IDEFTIFY_ID                        1006
#define MC_SPC_IDEFTIFY_MSG_ID                    10261
#define MC_SPC_IDEFTIFY_MSG_ID_RCV_SPC_DATA       (MC_SPC_IDEFTIFY_MSG_ID + 1) /* The SPC data is received. */
#define MC_SPC_IDEFTIFY_MSG_ID_START_SPC          (MC_SPC_IDEFTIFY_MSG_ID + 2) /* Start identification */
#define MC_SPC_IDEFTIFY_MSG_ID_SEND_EVENT         (MC_SPC_IDEFTIFY_MSG_ID + 3) /* The SPS sends an event. */
#define MC_SPC_IDEFTIFY_MSG_ID_RCV_EVENT          (MC_SPC_IDEFTIFY_MSG_ID + 4) /* The SPS receives the event. */
#define MC_SPC_IDEFTIFY_MSG_ID_TIMER_EVENT        (MC_SPC_IDEFTIFY_MSG_ID + 5) /* SPC timer event */
#define MC_SPC_IDEFTIFY_MSG_ID_RETRANSMIT_TIMEOUT (MC_SPC_IDEFTIFY_MSG_ID + 6) /* SPC timer event */

/*
 * Definition of the mesh route component
 * Component ID = 1007
 * Component message ID range:[10301, 10350]
 */
#define MC_MESH_RT_ID                        1007
#define MC_MESH_RT_MSG_ID                    10301
#define NM_MESH_RT_CHECK_MSG                 (MC_MESH_RT_MSG_ID + 1)
#define NM_MESH_RT_REQ_MSG                   (MC_MESH_RT_MSG_ID + 2)
#define NM_MESH_SET_MONITOR_MSG              (MC_MESH_RT_MSG_ID + 3)
/*
 * Definition of the lock network component
 * Component ID = 1008
 * Component message ID range:[10351, 10400]
 */
#define MC_LOCK_NETWOK_ID                     1008
#define MC_LOCK_NETWOK_MSG_ID                 10351

td_void uapi_mac_add_component(EXT_CONST ext_mac_component_info_st *component_info);
td_u32 uapi_mc_send_msg(EXT_CONST ext_sys_queue_msg *msg, td_u32 timeout);
#endif
