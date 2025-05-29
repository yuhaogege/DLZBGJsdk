/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Upgrade APIs, which are used to implement the upgrade function
 * Author: CompanyName
 * Create: 2018-10-31
 */
/**
 * @defgroup upg Upgrade.
 * @ingroup mid
 */

#ifndef __SOC_MDM_UPG_H__
#define __SOC_MDM_UPG_H__

#include <soc_nv_upg.h>
#include <soc_21_upg.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup upg
 * Upgrade status query request parameter, involving all upgrade information: \n
 * @li In the case of single-station upgrade, other information (for example, the length of the upgrade file received
 * by the communication module and the time required for upgrading the module) can also be queried besides the
 * current upgrade status.
 * @li In the case of network-wide upgrade, all the upgrade information of all modules on the network, or the upgrade
 * statistics of all modules can also be queried besides the current upgrade status.
 */
#define EXT_UPG_STATUS_REQ_OPT_NA 0

/**
 * @ingroup upg
 * Upgrade status query request parameter, involving the basic upgrade information. You can set this parameter to query
 * only the current upgrade status.
 */
#define EXT_UPG_STATUS_REQ_OPT_BS 1

/**
 * @ingroup upg
 * The upgrade is successful.
 */
#define EXT_UPG_STATUS_OK 0

/**
 * @ingroup upg
 * The upgrade is being verified.
 */
#define EXT_UPG_STATUS_END 1

/**
 * @ingroup upg
 * The upgrade status is unknown. Currently, the upgrade is not in progress.
 */
#define EXT_UPG_STATUS_UNKNOWN 8

/**
 * @ingroup upg
 * The upgrade on the entire network is being stopped.
 */
#define EXT_UPG_STATUS_STOPPING 16

/**
 * @ingroup upg
 * The upgrade file is being received.
 */
#define EXT_UPG_STATUS_PROCESS 0xF0

/**
 * @ingroup upg
 * The upgrade failed.
 */
#define EXT_UPG_STATUS_FAIL 0xFF

/**
 * @ingroup upg
 * The sta trans mmcu file max waitting time.
 */
#define EXT_UPG_DEVICE_PROTECT_TIME  300 /* int s */

/**
 * @ingroup upg
 * @brief This message ID is used to query the upgrade status.
 *
 * @par Description:
 * (1) Call uapi_upg_register_msg_notify to register the handling function of the response corresponding to
 * the message ID. \n
 * (2) Call uapi_upg_msg_send to send an upgrade status query request message to the upgrade module. \n
 * (3) After the processing of the upgrade module, call the handling function registered by the user to report
 * the processing result. \n
 * @attention    None
 * @param msg_id [IN] Message ID for querying the upgrade status. The value is #EXT_UPG_STATUS.
 * @param packet [IN] Upgrade status query request parameter (ext_upg_status_req): \n
 * @li When status_req.option is set to #EXT_UPG_STATUS_REQ_OPT_NA: The response message IDs for network-wide upgrade
 * are #EXT_UPG_STATUS and #EXT_UPG_STAT_INFO_IND (0x36C0), and the parameters are defined as ext_upg_status_ind and
 * ext_upg_stat_info_ind, respectively.
 * @li When status_req.option is set to #EXT_UPG_STATUS_REQ_OPT_NA: The response message ID for single-station upgrade
 * is EXT_UPG_STATUS, and the parameter is defined as ext_upg_status_ind.
 * @li When status_req.option is set to #EXT_UPG_STATUS_REQ_OPT_BS: The response message ID for upgrade is
 * EXT_UPG_STATUS_BS_IND (0x36C1), and the parameter is defined as ext_upg_status_bs_ind.
 * @param packet_size [IN] Size of the structure for the upgrade status query request parameter.
 * @retval msg_id Message ID for the upgrade status query.The value options are #EXT_UPG_STATUS,#EXT_UPG_STAT_INFO_IND,
 * and #EXT_UPG_STATUS_BS_IND.
 * @retval packet Response parameter for the upgrade status query. The value options are ext_upg_status_ind,
 * ext_upg_stat_info_ind, and ext_upg_status_bs_ind, corresponding to the values of msg_id.
 * @retval packet_size Size of the structure for the upgrade status query response parameter.
 * @par Dependency:
 * @li soc_mdm_upg.h: This file describes the external APIs for upgrade.
 * @see uapi_upg_register_msg_notify | uapi_upg_msg_send
 * @since DW21_V100R001C00
 */
#define EXT_UPG_STATUS 0x3660

/**
 * @ingroup upg
 * @brief This message ID is used to notify trans file finish.
 *
 * @par Description:
 *   (1) Call uapi_upg_register_msg_notify to register the handling function of the response corresponding to
 *       the message ID. \n
 *   (2) After the processing of the upgrade module, call the handling function registered by the user to report
 *       the processing result. \n
 * @attention    None
 * @param msg_id [IN] Message ID for  notify trans file finish. The value is #EXT_UPG_TRANSMIT.
 * @param packet [IN] Upgrade transmit ind parameter (ext_upg_status_req): \n
 *   @li When sta slave the upg file finished and the notify is registered. the packet will be sent to user to deal.
         #ext_upg_transmit_finished_ind
 * @param packet_size [IN] Size of the structure for the upgrade transmit request parameter.
 * @retval msg_id Message ID for the upgrade transmit. The value options are #EXT_UPG_TRANSMIT, 
 *                and #EXT_UPG_STATUS_BS_IND.
 * @retval packet Response parameter for the upgrade transmit. The value options are ext_upg_transmit_finished_ind,
 * @retval packet_size Size of the structure for the upgrade transmit ind parameter.
 * @par Dependency:
 *   @li soc_mdm_upg.h: This file describes the external APIs for upgrade.
 * @see uapi_upg_register_msg_notify | uapi_upg_msg_send
 * @since DW21_V100R001C00
 */
#define EXT_UPG_TRANSMIT_FINISHED 0x3661

/**
 * @ingroup upg
 * @brief This message ID is used to start the upgrade.
 *
 * @par Description:
 * (1) Call uapi_upg_register_msg_notify to register the handling function of the response corresponding to
 * the message ID. \n
 * (2) Call uapi_upg_msg_send to send an upgrade start request message to the upgrade module. \n
 * (3) After the processing of the upgrade module, call the callback function registered by the user to report
 * the processing result. \n
 * @attention Before starting the upgrade, ensure that the upgrade file has been saved to the cache.
 * @param msg_id [IN] Message ID for the upgrade start. The value is #EXT_UPG_START.
 * @param packet [IN] Upgrade start request parameter (ext_upg_start_req)
 * @param   packet_size [IN]  Size of the structure for the upgrade start request parameter.
 * @retval  msg_id Message ID for the upgrade start. The value is #EXT_UPG_START.
 * @retval  packet Upgrade start response parameter. The corresponding structure is ext_upg_start_ind.
 * @retval  packet_size Size of the structure for the upgrade start response parameter.
 * @par Dependency:
 * @li soc_mdm_upg.h: This file describes the external APIs for upgrade.
 * @see uapi_upg_register_msg_notify | uapi_upg_msg_send
 * @since DW21_V100R001C00
 */
#define EXT_UPG_START 0x3662

/**
 * @ingroup upg
 * @brief This message ID is used to stop the upgrade.
 *
 * @par Description:
 * (1) Call uapi_upg_register_msg_notify to register the handling function of the response corresponding to
 * the message ID. \n
 * (2) Call uapi_upg_msg_send to send an upgrade start request message to the upgrade module. \n
 * (3) After the processing of the upgrade module, call the callback function registered by the user to report
 * the processing result. \n
 * @attention    None
 * @param msg_id [IN] Message ID for the upgrade stop. The value is #EXT_UPG_STOP.
 * @param   packet [IN]  Upgrade stop request parameter (ext_upg_stop_req)
 * @param   packet_size [IN]  Size of the structure for the upgrade stop request parameter
 * @retval  msg_id Message ID for the upgrade stop. The value is #EXT_UPG_STOP.
 * @retval  packet Upgrade stop response parameter. The corresponding structure is ext_upg_stop_ind.
 * @retval  packet_size Size of the structure for the upgrade stop response parameter
 * @par Dependency:
 * @li soc_mdm_upg.h: This file describes the external APIs for upgrade.
 * @see uapi_upg_register_msg_notify | uapi_upg_msg_send
 * @since DW21_V100R001C00
 */
#define EXT_UPG_STOP 0x3663

/**
 * @ingroup upg
 * @brief  This message ID is used to respond to the upgrade status statistics.
 *
 * @par Description:
 * (1) Call uapi_upg_register_msg_notify to register the handling function of the response corresponding to
 * the message ID. \n
 * (2) Call uapi_upg_msg_send to send an upgrade start request message to the upgrade module. \n
 * (3) After the processing of the upgrade module, call the callback function registered by the user to report
 * the processing result. \n
 * @attention    None
 * @param   msg_id [IN]  Message ID for querying the upgrade status. The value is #EXT_UPG_STATUS.
 * @param   packet [IN]  Upgrade status query request parameter (ext_upg_status_req):
 * When status_req.option is set to EXT_UPG_STATUS_REQ_OPT_NA: The response message ID of
 * the network-wide upgrade status statistics is EXT_UPG_STAT_INFO_IND, and the parameter is
 * defined as ext_upg_stat_info_ind.
 * @param   packet_size [IN]  Size of the structure for the upgrade status query request parameter
 * @retval  msg_id Message ID of the upgrade status statistics. The value is #EXT_UPG_STAT_INFO_IND.
 * @retval  packet Response parameter of the upgrade status statistics. The corresponding structure is
 * ext_upg_stat_info_ind.
 * @retval  packet_size Size of the structure for the upgrade status statistics response parameter.
 * @par Dependency:
 * @li soc_mdm_upg.h: This file describes the external APIs for upgrade.
 * @see uapi_upg_register_msg_notify | uapi_upg_msg_send
 * @since DW21_V100R001C00
 */
#define EXT_UPG_STAT_INFO_IND 0x36C0

/**
 * @ingroup upg
 * @brief  This message ID is used to respond to the basic upgrade status information.
 *
 * @par Description:
 * (1) Call uapi_upg_register_msg_notify to register the handling function of the response corresponding to
 * the message ID. \n
 * (2) Call uapi_upg_msg_send to send an upgrade start request message to the upgrade module. \n
 * (3) After the processing of the upgrade module, call the callback function registered by the user to report
 * the processing result. \n
 * @attention    None
 * @param   msg_id [IN]  Message ID for querying the upgrade status. The value is #EXT_UPG_STATUS.
 * @param   packet [IN]  Upgrade status query request parameter (ext_upg_status_req):
 * When status_req.option is set to #EXT_UPG_STATUS_REQ_OPT_BS: The response message ID for
 * upgrade is EXT_UPG_STATUS_BS_IND (0x36C1), and the parameter is defined as ext_upg_status_bs_ind.
 * @param   packet_size [IN]  Size of the structure for the upgrade status query request parameter
 * @retval  msg_id Message ID of the basic upgrade status information. The value is EXT_UPG_STATUS_BS_IND.
 * @retval  packet Response parameter of the basic upgrade status information. The corresponding structure is
 * ext_upg_status_bs_ind, according to msg_id.
 * @retval  packet_size Size of the structure for the response parameter of the basic upgrade status information.
 * @par Dependency:
 * @li soc_mdm_upg.h: This file describes the external APIs for upgrade.
 * @see uapi_upg_register_msg_notify | uapi_upg_msg_send
 * @since DW21_V100R001C00
 */
#define EXT_UPG_STATUS_BS_IND 0x36C1

/**
 * @ingroup upg
 * Upgrade option: single CCO upgrade
 * @li Scenario: This feature applies only to the CCO. It is used for single CCO upgrade.
 * @li Usage: EXT_UPG_REQ_OPTION_CCO | EXT_UPG_VER_UPDATE_OPTION_START.
 */
#define EXT_UPG_REQ_OPTION_CCO 0x02

/**
 * @ingroup upg
 * Upgrade option: network-wide STA upgrade (excluding the CCO)
 * @li Scenario: This feature applies only to the CCO. It is used to upgrade STAs on the entire network.
 * @li Usage: EXT_UPG_REQ_OPTION_STA | EXT_UPG_VER_UPDATE_OPTION_START.
 */
#define EXT_UPG_REQ_OPTION_STA 0x04

/**
 * @ingroup upg
 * Upgrade option: network-wide DEVICE upgrade (excluding the CCO)
 * @li Scenario: This feature applies only to the CCO. It is used to upgrade DEVICEs on the entire network.
 * @li Usage: EXT_UPG_REQ_OPTION_STA | EXT_UPG_VER_UPDATE_OPTION_START.  
 */
#define EXT_UPG_REQ_OPTION_DEVICE 0x100

/**
 * @ingroup upg
 * Upgrade option: single-station upgrade
 * @li Scenario: It is applicable to all product forms, and is used for single-station upgrade.
 * @li Usage: EXT_UPG_REQ_OPTION_UNICAST | EXT_UPG_VER_UPDATE_OPTION_START.
 */
#define EXT_UPG_REQ_OPTION_UNICAST 0x10

/**
 * @ingroup upg
 * Upgrade option£ºlist upgrade.
 * @li scenes£ºLimited to CCO. Used to upgrade the STA list of the entire network.
 * @li usage£ºEXT_UPG_REQ_OPTION_LIST | EXT_UPG_VER_UPDATE_OPTION_START¡£
 */
#define EXT_UPG_REQ_OPTION_LIST 0x0200

/**
 * @ingroup upg
 * Upgrade option: upgrade start
 * Usage: EXT_UPG_REQ_ | EXT_UPG_VER_UPDATE_OPTION_START.
 */
#define EXT_UPG_VER_UPDATE_OPTION_START 0x20

/**
 * @ingroup upg
 * Invalid value, indicating that the upgrade is not network-wide.
 */
#define EXT_UPG_UPGRADE_NA 0x00

/**
 * @ingroup upg
 * STA upgrade in network-wide mode
 */
#define EXT_UPG_UPGRADE_ALL 0x01

/**
 * @ingroup upg
 * Upgrade the STA in the entire network list mode.
 */
#define EXT_UPG_UPGRADE_LIST 0x02

/**
 * @ingroup upg
 * Version number of the structure for the upgrade status query response The corresponding structure is
 * upg_status_ind_entry.
 */
#define EXT_UPG_STATUS_IND_ENTRY_VER 1

/**
 * @ingroup upg
 * List upgrade bitmap size.
 */
#define UPG_LIST_BITMAP_LEN 128

/**
 * @ingroup upg
 * @brief  Callback function corresponding to the message ID for the user registered upgrade
 *
 * @par Description:
 * Callback function corresponding to the message ID for the user registered upgrade
 * @attention    None
 * @param msg_id [IN] message ID.  Message ID
 * @param   packet [IN]  Upgrade response parameter
 * @param   packet_size [IN]  Size of the structure for the upgrade response parameter
 * @retval #EXT_ERR_SUCCESS Success
 * @retval # Other values. Failure. For details, see the error code definitions.
 * @par Dependency:
 * @li soc_mdm_upg.h: This file describes the external APIs for upgrade.
 * @see None
 * @since DW21_V100R001C00
 */
typedef td_u32 (*ext_upg_proc_msg_fun)(td_u32 msg_id, td_pbyte packet, td_u16 packet_size);

/**
 * @ingroup upg
 * @brief Callback function registered after the completion of upgrade file transfer
 *
 * @par Description:
 * Callback function registered after the completion of upgrade file transfer
 * @attention This feature applies only to the CCO.
 * @param  None
 * @retval None
 * @par Dependency:
 * @li soc_mdm_upg.h: This file describes the external APIs for upgrade.
 * @see None
 * @since DW21_V100R001C00
 */
typedef td_void (*ext_upg_affirm_notify)(td_void *param);

/**
 * @ingroup upg
 * Upgrade status query request parameter
 */
typedef struct {
    td_u32 option;      /**< Type of the upgrade status information query. If the value is #EXT_UPG_STATUS_REQ_OPT_BS,
                         * only the basic upgrade status information is involved. If this parameter is set to other
                         * values, all upgrade information is queried.
                         */
    td_u32 blk_per_pkt; /**< Query parameter, not used. */
} upg_status_req;

/**
 * @ingroup upg
 * Upgrade start request.
 */
typedef struct {
    td_u32 option;             /**< Upgrade option, which must use #EXT_UPG_VER_UPDATE_OPTION_START. It can be ORed with
                                * following values: \n
                                * @li EXT_UPG_REQ_OPTION_CCO indicates that only the CCO is upgraded, and all STAs are
                                * not upgraded. \n
                                * @li EXT_UPG_REQ_OPTION_STA indicates that all STAs are upgraded while the CCO is not
                                * upgraded. \n
                                * @li EXT_UPG_REQ_OPTION_UNICAST indicates the single-station upgrade. That is, only
                                * the module that receives the upgrade request is upgraded, and the upgrade file
                                * is not sent to other modules.
                                */
    td_u32 upg_id;             /**< Upgrade ID, which must be unique during the upgrade. */
    td_u32 file_len;           /**< Total length of the upgrade file (unit: byte). */
    td_u32 blk_num;            /**< The value is fixed at 1. */
    td_u32 upg_time_window;    /**< Time window between the upgrade start and the upgrade end (unit: min). If
                                * this parameter is set to 0, the default value 360 min is used. */
    td_u32 file_crc;           /**< CRC value of the entire upgrade file, calculated by calling uapi_crc32
                                * (the initial CRC value is 0).
                                */
    ext_nv_ftm_upg_cfg upg_cfg; /**< Upgrade parameter structure. The default value is 0. */
} upg_start_req;

/**
 * @ingroup upg
 * List upgrade structure version number.
 */
typedef enum {
    EXT_UPG_LIST_VER0 = 0, /**< version 0 */
    EXT_UPG_LIST_VER1 = 1, /**< version 1 */
} ext_upg_list_ver;

/**
 * @ingroup upg
 * List upgrade request information.
 */
typedef struct {
    td_u8 stru_ver;                        /**< Version number, this version is EXT_UPG_LIST_VER1. */
    td_u8 stru_size;                       /**< Structure size. */
    td_u16 sta_num;                        /**< Number of stations. */
    td_u32 upg_id;                         /**< Upgrade ID. */
    td_u8 tei_bitmap[UPG_LIST_BITMAP_LEN]; /**< tei bitmap: 1:need to upgrade. */
} upg_sta_list_req;

/**
 * @ingroup upg
 * Upgrade start response
 */
typedef struct {
    td_u32 rc;                      /**< Result code: EXT_ERR_SUCCESS indicates success,
                                        while other values indicate failure. */
    td_u32 upg_id;                  /**< Upgrade ID */
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN]; /**< MAC address of the communication module (MAC address of the PLC module). */
    td_u8 pad[2];                   /**< Reseved 2 bytes. */
} upg_start_ind;

/**
 * @ingroup upg
 * Upgrade stop request
 */
typedef struct {
    td_u32 upg_id; /**< Upgrade ID. The source of the upgrade stop request command ID must match that of the upgrade
                    * start request command ID. Otherwise, the board does not respond to the upgrade stop.
                    * When the value of ulUpgId for request stopping upgrade is 0, the board responds to the upgrade
                    * stop request unconditionally.
                    */
    td_u16 option; /**< The value is fixed at 0. */
    td_u16 reserved;
} upg_stop_req;

/**
 * @ingroup upg
 * Upgrade stop response
 */
typedef struct {
    td_u32 rc;     /**< Result code: EXT_ERR_SUCCESS indicates success, while other values indicate failure.
                    * For details, see the error code definitions.
                    */
    td_u32 upg_id; /**< Upgrade ID, which must be unique during the upgrade. */
} upg_stop_ind;

/**
 * @ingroup upg
 * Upgrade file information in the upgrade start parameters
 */
typedef struct {
    td_u16 check_sum;     /**< The value is fixed at 0. */
    td_u16 file_blk_size; /**< The value is fixed at 0. */
    td_u32 upg_id;        /**< Upgrade ID. */
    td_u16 req_sn;        /**< The value is fixed at 0. */
    td_u16 idx;           /**< The value is fixed at 1. */
    td_u8 file_blk[0];    /**< The value is fixed at 0. */
} upg_transmit_blk_req;

/**
 * @ingroup upg
 * Structure of the STA upgrade status information
 */
typedef struct {
    td_u16 last_blk_idx;  /**< Index number of the last continuous and valid upgrade file block. */
    td_u8 error_code : 3; /**< Error code that occurs during the upgrade: 3 bit */
    td_u8 ratio : 5;      /**< Receive file ratio: 5 bit. */
    /* Current upgrade status:
     * EXT_UPG_STATUS_OK indicates that the upgrade is successful. \ n
     * EXT_UPG_STATUS_END indicates that the upgrade is being verified. \ n
     * EXT_UPG_STATUS_FAIL indicates that the upgrade fails. \n
     * EXT_UPG_STATUS_PROCESS indicates that the upgrade is in progress. \n
     * EXT_UPG_STATUS_UNKNOWN indicates that the status is unknown.
     */
    td_u8 upg_status;
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN]; /**< MAC address of the communication module (MAC address of the PLC module). */
    td_u16 upg_end_time;            /**< Time for module upgrade. */
} upg_status_ind_entry;

/**
 * @ingroup upg
 * Upgrade status query and response
 */
typedef struct {
    td_u32 upg_id;    /**< ID of the upgrade process. The ID must be unique during the upgrade. */
    td_u32 blk_size;  /**< The value is fixed at 300. */
    td_u32 file_size; /**< Size of the upgrade file (unit: byte). */
    td_u16 num;       /**< Number of valid entries. */
    td_u8 pad;
    td_u8 stru_ver;                /**< Version number of the structure. */
    upg_status_ind_entry entry[0]; /**< STA upgrade status information. */
} upg_status_ind;

/**
 * @ingroup upg
 * Response to the statistics about the network-wide upgrade status query
 */
typedef struct {
    td_u32 upg_id;        /**< ID of the network-wide upgrade. */
    td_u16 ok_num;        /**< Total number of STAs successfully upgraded on the entire network. */
    td_u16 complete_num;  /**< Total number of STAs being verified on the entire network. */
    td_u16 fail_num;      /**< Total number of STAs that fail to be upgraded on the entire network. */
    td_u16 process_num;   /**< Total number of STAs with the upgrade files being transferred on the entire network. */
    td_u8 upg_status;     /**< Current network-wide upgrade status: #EXT_UPG_STATUS_OK indicates that the network-wide
                           * upgrade is successful. #EXT_UPG_STATUS_END indicates that the network-wide upgrade is
                           * complete. #EXT_UPG_STATUS_PROCESS indicates that the network-wide upgrade is in progress.
                           */
    td_u8 upg_option : 2; /**< Upgrade mode: #EXT_UPG_UPGRADE_NA indicates not available. #EXT_UPG_UPGRADE_ALL
                           * indicates network-wide STA upgrade.
                           */
    td_u8 pad1 : 6;       /**< Reserved. */
    td_u8 pad2[2];        /**< Reserved 2 bytes. */
} upg_stat_info_ind;

/**
 * @ingroup upg
 * Upgrade status request
 */
typedef struct {
    upg_status_req status_req; /**< Status query request. */
} ext_upg_status_req;

/**
 * @ingroup upg
 * Upgrade status response
 */
typedef struct {
    upg_status_ind status_ind; /**< Response to the query of network-wide upgrade or single-station upgrade status. */
} ext_upg_status_ind;

/**
 * @ingroup upg
 * Response to the upgrade status statistics
 */
typedef struct {
    upg_stat_info_ind stat_info; /**< Response to the query of network-wide upgrade statistics. */
} ext_upg_stat_info_ind;

/**
 * @ingroup upg
 * Response to the basic upgrade status information
 */
typedef struct {
    td_u32 upg_id;     /**< Upgrade ID. */
    td_u8 upg_status;  /**< Upgrade status:
                        * #EXT_UPG_STATUS_OK indicates that the upgrade is successful.
                        * #EXT_UPG_STATUS_FAIL indicates that the upgrade fails.
                        * #EXT_UPG_STATUS_END indicates the upgrade is being verified.
                        * #EXT_UPG_STATUS_PROCESS indicates that the upgrade is in progress.
                        * #EXT_UPG_STATUS_UNKNOWN indicates that the status is unknown, and currently
                        * the upgrade is not in progress.
                        */
    td_u8 reserved[3]; /**< Reserved 3 bytes. */
} ext_upg_status_bs_ind;

/**
 * @ingroup upg
 * Upgrade start request
 */
typedef struct {
    upg_start_req cfg;         /**< Upgrade start information. */
    upg_sta_list_req upg_list; /**< list upgrade information. */
    upg_transmit_blk_req file; /**< Upgrade file information. */
} ext_upg_start_req;

/**
 * @ingroup upg
 * Upgrade start response
 */
typedef struct {
    td_u32 rc; /**< Execution result of the upgrade start: EXT_ERR_SUCCESS indicates that the upgrade is successfully
                * started. Other values indicate that the upgrade fails to be started. For details, see the error code
                * definitions.
                */
} ext_upg_start_ind;

/**
 * @ingroup upg
 * Upgrade stop request
 */
typedef struct {
    upg_stop_req stop_req; /**< Upgrade stop request. */
} ext_upg_stop_req;

/**
 * @ingroup upg
 * Upgrade stop response
 */
typedef struct {
    upg_stop_ind stop_ind; /**< Upgrade stop response. */
} ext_upg_stop_ind;

/**
 * @ingroup upg
 * Upgrade trans response
 */
typedef struct {
    td_u32 upg_id; /**< Current upgrade id */
    td_u32 crc; /**< Crc of the slaved file */
    td_u32 file_len; /**< file len of the slaved file */
    td_u32 flash_offset; /**< flash_addr of the slaved file */
} ext_upg_transmit_finished_ind;

/**
 * @ingroup upg
 * @brief  Initializes the upgrade module.
 *
 * @par Description:
 * Initializes the upgrade module.
 * @attention
 * @li This API needs to be called in the app startup process. For details, see section 1.3 "Demo Overview" in
 * the DW21 V100 API Development Guide.
 * @li This API is an upgrade resource initialization function and cannot be called multiple times.
 * @param  None
 * @retval #EXT_ERR_SUCCESS Success
 * @retval #Other values. Failure. For details, see the error code definitions.
 * @par Dependency:
 * @li soc_mdm_upg.h: This file describes the external APIs for upgrade.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_upg_init(td_void);
/**
 * @ingroup upg
 * @brief  set the upgrade data send interval.
 * @par Description:
 * This function is used to set the interval of data send.
 * @param  interval [IN] data send interval.
 * @retval #EXT_ERR_SUCCESS Success
 * @retval #Other values. Failure. For details, see the error code definitions.
 * @par Dependency:
 * @li soc_mdm_upg.h: This file describes the external APIs for upgrade.
 * @see None
 */
td_u32 uapi_set_upg_send_interval(td_u8 interval);

/**
 * @ingroup upg
 * @brief  Obtains the upgrade cache.
 *
 * @par Description:
 * This function is used to obtain the upgrade cache for storing upgrade files.
 * @attention
 * @li If the upgrade cache is successfully obtained by calling this API, call uapi_upg_release_cache to release
 * the upgrade cache after the upgrade is stopped (by calling #EXT_UPG_STOP).
 * @li Ensure that the input parameters are the same as those during upgrade start. Otherwise, the upgrade will fail.
 * @param  file_type [IN] Upgrade file type. The reference value is #upg_trans_file_type.
 * @param  file_len  [IN] Length of the upgrade file (unit: byte). For details about the maximum length of
 * an upgrade file, see the description of "Upgrade backup partition" in the DW21 V100 API
 * Development Guide.
 * @retval #EXT_ERR_SUCCESS Success
 * @retval #Other values. Failure. For details, see the error code definitions.
 * @par Dependency:
 * @li soc_mdm_upg.h: This file describes the external APIs for upgrade.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_upg_obtain_cache(td_u32 file_type, td_u32 file_len);

/**
 * @ingroup upg
 * @brief  Releases the upgrade cache.
 *
 * @par   Description:
             Releases the upgrade cache requested by uapi_upg_obtain_cache.
 * @attention
 *   @li After calling #EXT_UPG_START to start the upgrade, call this API to release the upgrade cache after the upgrade
 *       is stopped.
 *   @li After obtaining the upgrade cache, call this API to release the upgrade cache before stopping the upgrade
 *       which has been started by calling #EXT_UPG_START.
 * @param  None
 * @retval None
 * @par Dependency:
 *   @li soc_mdm_upg.h: This file describes the external APIs for upgrade.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_upg_release_cache(td_void);

/**
 * @ingroup upg
 * @brief Writes the upgrade data to the flash cache.
 *
 * @par Description:
 * This function is used to write upgrade files to the upgrade cache.
 * @attention
 * @li Before sending an upgrade message, call this API to write the upgrade files to the flash memory.
 * @li Before calling this API, call uapi_upg_obtain_cache to obtain the upgrade cache.
 * @param  offset  [IN] Offset address relative to the start address of the upgrade cache.
 * @param  buf     [IN] Pointer to the upgrade data package.
 * @param  buf_len [IN] Length of the upgrade data package (unit: byte).
 * @retval #EXT_ERR_SUCCESS Success
 * @retval #Other values. Failure. For details, see the error code definitions.
 * @par Dependency:
 * @li soc_mdm_upg.h: This file describes the external APIs for upgrade.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_upg_mem_write(td_u32 offset, EXT_CONST td_u8 *buf, td_u32 buf_len);

/**
 * @ingroup upg
 * @brief Reads upgrade data from the flash cache.
 *
 * @par Description:
 * This function is used to read upgrade data from the upgrade cache.
 * @attention
 * @li This API is not supported when the operation permission on the upgrade cache is not obtained or the upgrade
 * is not in progress.
 * @li Ensure that the actual buffer size is the same as the value of buf_len.
 * @param  offset  [IN] Offset address relative to the start address of the upgrade cache.
 * @param  buf [IN/OUT] Pointer to the upgrade data package.
 * @param  buf_len [IN] Length of the upgrade data package (unit: byte).
 * @retval #EXT_ERR_SUCCESS Success
 * @retval #Other values. Failure. For details, see the error code definitions.
 * @par Dependency:
 * @li soc_mdm_upg.h: This file describes the external APIs for upgrade.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_upg_mem_read(td_u32 offset, td_u8 *buf, td_u32 buf_len);

/**
 * @ingroup upg
 * @brief  Handles registration and upgrade messages.
 *
 * @par Description:
 * Handles registration and upgrade messages.
 * @attention Ensure that this API is called before the upgrade process is started.
 * @param  msg_id [IN] Message ID.
 * @param  cb     [IN] Callback function registered by the user.
 * @retval #EXT_ERR_SUCCESS Success
 * @retval #Other values. Failure. For details, see the error code definitions.
 * @par Dependency:
 * @li soc_mdm_upg.h: This file describes the external APIs for upgrade.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_upg_register_msg_notify(td_u32 msg_id, ext_upg_proc_msg_fun cb);

/**
 * @ingroup upg
 * @brief  Upgrade message sending API
 *
 * @par Description:
 * Upgrade message sending API
 * @attention  None
 * @param  msg_id      [IN] message ID. Message ID.
 * @param  packet      [IN] Content of the sent data packet.
 * @param  packet_size [IN] Size of the sent data packet.
 * @retval  #EXT_ERR_SUCCESS Success
 * @retval  #Other values. Failure. For details, see the error code definitions.
 * @par Dependency:
 * @li soc_mdm_upg.h: This file describes the external APIs for upgrade.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_upg_msg_send(td_u32 msg_id, td_pbyte packet, td_u32 packet_size);

/**
 * @ingroup upg
 * @brief  Registers the API for notifying the completion of upgrade file transfer.
 *
 * @par Description:
 * This API is used for registering the API for notifying the completion of upgrade file transfer from the CCO to
 * STAs in the case of network-wide upgrade.
 * @attention  This feature applies only to the CCO.
 * @param  cb    [IN] API for notifying the completion of upgrade file transfer
 * @param  param [IN] Callback parameter registered by the user
 * @retval #EXT_ERR_SUCCESS Success.
 * @retval #Other values Failure
 * @par Dependency:
 * @li soc_mdm_upg.h: This file describes the external APIs for upgrade.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_upg_register_affirm_notify(ext_upg_affirm_notify cb, td_void *param);

/**
 * @ingroup upg
 * @brief Obtains the upgrade configuration parameter.
 *
 * @par   Description:
 * This function is used to obtain the upgrade configuration parameter.
 * @attention
 * @li This feature applies only to the CCO.
 * @li The upgrade configuration parameters corresponding to the current frequency band should be obtained.
 * For example, if the current frequency band is 3.2 MHz, the upgrade configuration parameter for the 3.2 MHz
 * frequency band is obtained.
 * @param  cfg [IN/OUT] Upgrade configuration parameter
 * @retval #EXT_ERR_SUCCESS Success
 * @retval #Other values. Failure. For details, see the error code definitions.
 * @par Dependency:
 * @li soc_mdm_upg.h: This file describes the external APIs for upgrade.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_upg_get_cfg(ext_upg_cfg *cfg);

/**
 * @ingroup upg
 * @brief  Sets the upgrade configuration parameter.
 *
 * @par Description:
 * This function is used to set the upgrade configuration parameter.
 * @attention
 * @li This feature applies only to the CCO.
 * @li The upgrade configuration parameter corresponding to the current frequency band should be set. For example,
 * if the current frequency band is 3.2 MHz, the upgrade configuration parameter for the 3.2 MHz frequency band
 * is set.
 * @li This function needs to be called in the task initialization API at the app layer. The specific parameter is
 * matched with the running program. If the user does not call this API, the default parameter is used.
 * @li The latency waiting configuration applies to the scenario where the CCO is restarted after upgrade or
 * the power is resumed after power-off. In this scenario, the number of STAs after the networking is complete
 * is less than the number of STAs during upgrade start.
 * @li STA information query configuration applies to the scenario where the CCO is restarted after upgrade and
 * the networking is complete.
 * @li Except the latency waiting configuration and STA information query configuration, other configurations
 * apply to the data distribution process before the CCO is restarted after upgrade.
 * @li When a time-related parameter is set to 0, the default value 10 ms is used.
 * @li Do not disable the broadcast policy and unicast policy at the same time. Otherwise, data distribution will
 * not be performed and the upgrade will fail.
 * @param  cfg [IN] Upgrade configuration parameter
 * @retval #EXT_ERR_SUCCESS Success
 * @retval #Other values. Failure. For details, see the error code definitions.
 * @par Dependency:
 * @li soc_mdm_upg.h: This file describes the external APIs for upgrade.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_upg_set_cfg(EXT_CONST ext_upg_cfg *cfg);

/**
 * @ingroup upg
 * @brief  Set the list to upgrade the cache information.
 *
 * @par Description:
 * Set the list to upgrade the cache information.
 * @attention
 * @li Use only on the CCO side.
 * @li Must be called when the list upgrade function is required, otherwise the list upgrade function cannot be used.
 * @li This interface needs to be called before calling the uapi_upg_init interface,and does not support repeated calls.
 * @param  addr_blk [IN]Cache low address.unit: 4KB.
 * @param  size_blk [IN]Cache size.unit: 4KB.
 * @retval #EXT_ERR_SUCCESS The setting is successful.
 * @retval #Other values:failed to be set. For details, see the error code definition.
 * @par Dependency:
 * @li soc_mdm_upg.h: This file describes the external APIs for upgrade.
 * @see None.
 * @since DW21_V100R001C00
 */
td_u32 uapi_upg_list_set_cache_info(EXT_IN td_u32 addr_blk, EXT_IN td_u32 size_blk);

/**
 * @ingroup upg
 * @brief  get result of check files.
 *
 * @par Description:
 * check if the upgrade file is sta, used only upgrade sta from cco.
 * @par Dependency:
 * @li soc_mdm_upg.h: This file describes the external APIs for upgrade.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_upg_head_check_res(td_void);

/**
 * @ingroup upg
 * @brief  check single upgrade packet
 *
 * @par Description:
 * check if the first package of single upgrade matches.
 * @par Dependency:
 * @li soc_mdm_upg.h: This file describes the external APIs for upgrade.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_local_upg_head_check_res(td_void);

/**
 * @ingroup upg
 * @brief  get whether allow NDM to upgrade CCO and STA status
 *
 * @par Description:
 * @par Dependency:
 * @li soc_mdm_upg.h: This file describes the external APIs for upgrade.
 * @see None
 * @since DW21_V100R001C00
 */
td_bool uapi_upg_get_ndm_upg_dev_status(td_void);

/**
 * @ingroup upg
 * @brief  set whether allow NDM to upgrade CCO and STA
 *
 * @par Description:
 * allow_ndm_upg_dev: true: allowed; false: disabled.
 * @retval:true: allowed; false: disabled
 * @par Dependency:
 * @li soc_mdm_upg.h: This file describes the external APIs for upgrade.
 * @see None
 * @since DW21_V100R001C00
*/
td_void uapi_upg_set_ndm_upg_dev_status(td_bool allow_ndm_upg_dev);

/**
* @ingroup upg
* @brief   register DIAG command
*
* @par Description:register whether allow DBK to upgrade STA
* @par Dependency:
* @li soc_mdm_upg.h: This file describes the external APIs for upgrade.
* @see None
* @since DW21_V100R001C00
*/
td_u32 uapi_upg_sta_cmd_init(td_void);
#ifdef __cplusplus
}
#endif
#endif /* __SOC_MDM_UPG_H__ */
