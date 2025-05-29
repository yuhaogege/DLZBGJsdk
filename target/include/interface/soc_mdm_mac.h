/*
 * Copyright (c) CompanyNameMagicTag 2012-2018. All rights reserved.
 * Description: APIs for the secondary development of the PLC protocol stack
 * Author: CompanyName
 * Create: 2012-07-13
 */

/** @defgroup network  Channel
 * @ingroup plc
 */

#ifndef SOC_MDM_MAC_H
#define SOC_MDM_MAC_H

#include <soc_list.h>
#include <soc_types.h>
#include "soc_mdm_types.h"
#include "soc_config.h"
#include "soc_mdm_topo.h"
#include "soc_mdm_tf.h"
#include "soc_mdm_fast_send.h"
#include "soc_mdm_plc_security.h"
#include "soc_mdm_star_net.h"
#ifdef __cplusplus
extern "C" {
#endif
#define bytes_align_4(s) (((s) + 3) & 0xfffffffc)

/**
 * @ingroup network
 * @brief Length of the management ID of association requests
 */
#define MANAGEMENT_ID_SIZE 24

/**
 * @ingroup network
 * @brief MAC address entry structure
 */
typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN]; /**< MAC address */
    td_u8 pad[2];                   /**< Reserved 2 bytes */
} ext_mac_entry;

/**
 * @ingroup network
 * @brief  MAC address source: app
 */
#define EXT_MDM_APP_MAC 0

/**
 * @ingroup network
 * @brief  MAC address source: NV
 */
#define EXT_MDM_NV_MAC 1

/**
 * @ingroup network
 * @brief MAC type adding
 */
typedef struct {
    td_u8 list_num;           /**< Count of mac_list. Value range: 1-6. */
    td_u8 clear_flag;         /**< The value #TD_TRUE indicates that the backup MAC address list is cleared. */
    td_u8 type;               /**< MAC type #EXT_MDM_APP_MAC: from the app; #EXT_MDM_NV_MAC: from NV */
    td_u8 reserved;           /**< Reserved */
    ext_mac_entry mac_list[0]; /**< MAC address entry structure */
} ext_mac_attr;


/**
 * @ingroup network
 * Return code for the whitelist change reasons
 */
typedef enum {
    EXT_MAC_WHITE_LIST_CHANGE_REASON_UNKNOWN = 0x00,       /**< Unknown reason */
    EXT_MAC_WHITE_LIST_CHANGE_REASON_DBK_SWITCH_AUTH_OFF,  /**< Switch of the whitelist modification on the DBK
                                                           * GUI: off
                                                           */
    EXT_MAC_WHITE_LIST_CHANGE_REASON_DBK_SWITCH_AUTH_ON,   /**< Switch of the whitelist modification on the DBK
                                                           * GUI: on
                                                           */
    EXT_MAC_WHITE_LIST_CHANGE_REASON_DBK_SWITCH_METER_OFF, /**< Switch of the whitelist modification on the DBK
                                                           * GUI: off
                                                           */
    EXT_MAC_WHITE_LIST_CHANGE_REASON_DBK_SWITCH_METER_ON,  /**< Switch of the whitelist modification on the DBK
                                                           * GUI: on
                                                           */
    EXT_MAC_WHITE_LIST_CHANGE_REASON_DBK_NV,               /**< NV of the modified NV item */
    EXT_MAC_WHITE_LIST_CHANGE_REASON_DBK_BASE_LIB,         /**< Basic information library changed */
    EXT_MAC_WHITE_LIST_TF_IDENTIFY,                        /**< Transformer district identification */
    EXT_MAC_WHITE_LIST_CHANGE_REASON_DBK_SWITCH_MODE,      /**< Mode of whitelist switching on the DBK GUI */

    EXT_MAC_WHITE_LIST_CHANGE_REASON_MAC_REQ = 0x1f,   /**< MAC address request */
    EXT_MAC_WHITE_LIST_CHANGE_REASON_APP_INIT,         /**< App initialization table archive */
    EXT_MAC_WHITE_LIST_CHANGE_REASON_APP_START_SEARCH, /**< Table searching start */
    EXT_MAC_WHITE_LIST_CHANGE_REASON_APP_STOP_SEARCH,  /**< Table searching stop */
    EXT_MAC_WHITE_LIST_CHANGE_REASON_APP_CON,          /**< Concentrator change table archive */
    EXT_MAC_WHITE_LIST_CHANGE_REASON_CLT_JOIN_NET,     /**< The collector stations that attempt to access
                                                       * networks without using table addresses are rejected
                                                       * and added to the whitelist.
                                                       */
    EXT_MAC_WHITE_LIST_CHANGE_REASON_TEST_MODE,        /**< The whitelist is closed for the version to be
                                                       * sent for check.
                                                       */
    EXT_MAC_WHITE_LIST_CHANGE_REASON_MAX = 0x3f        /**< Max. value for the return code */
} ext_mac_white_list_change_reason;

/**
 * @ingroup network
 * MAC address entry
 */
typedef struct {
    td_u16 list_num;          /**< MAC address entry */
    td_u16 pad;               /**< Reserved */
    ext_mac_entry mac_list[0]; /**< MAC address entry structure */
} ext_mac_valid_mac_list;

/**
 * @ingroup network
 * @brief  Sets a backup MAC address.
 *
 * @par  Description:
 * Sets a backup MAC address. If a station with multiple backup MAC addresses attempts to access the network
 * and the CCO rejects such request, the STAs will attempt to get the station online by traversing the backup
 * MAC addresses.
 * @attention  STA/NDM
 * @attention  When the product type is STA, a unique MAC address is added to the backup MAC address table of the STA
 * each time.
 * The addresses included in #ext_mac_entry of the input parameter #ext_mac_attr* cannot be all 0x00 or 0xFF.
 * The caller should ensure that list_num in #ext_mac_entry indicates the number of elements in the array,
 * which matches the length of the actual array mac_list.
 * Up to six backup MAC addresses can be set. If clear_flag is set to TD_TRUE, the new backup MAC address
 * overwrites the original backup MAC address.
 * @attention  When the product type is NDM, only the first MAC address in the list is used. "\x00\x00\x00\x00\x00\x00",
 * "\xFF\xFF\xFF\xFF\xFF\xFF", and "\x00\xFF\xFF\xFF\xFF\xFF" are not supported.
 * @attention  If the MAC address carried in mac_attr is backed up successfully, the success message "#EXT_ERR_SUCCESS"
 * is returned. Otherwise, the failure message "#EXT_ERR_INVALID_PARAMETER" is returned.
 * @param  mac_attr [IN] Type #ext_mac_attr* MAC address information
 *
 * @retval #EXT_ERR_SUCCESS   Success
 * @retval #EXT_ERR_INVALID_PARAMETER   Failure
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_set_backup_mac_addr(EXT_CONST EXT_IN ext_mac_attr * mac_attr);

/**
 * @ingroup network
 * @brief  Sets the CCO MAC address.
 *
 * @par  Description:
 * Sets the CCO MAC address.
 *
 * @attention  When the product type is CCO,
 * the following MAC addresses cannot be used:
 * 0x00: 0x00: 0x00: 0x00: 0x00: 0x00, 0xff: 0xff: 0xff: 0xff: 0xff: 0xff, 0x00: 0xff: 0xff: 0xff: 0xff:
 * 0xff. After the setting is successful, soft reset is performed on the CCO and the re-networking is
 * performed. The setting does not take effect after the hard restart.
 * @param  mac_addr [IN] Type #td_u8 PLC MAC address
 *
 * @retval ret      Type #td_u32
 * @li EXT_ERR_SUCCESS: The configuration is successful.
 * @li #EXT_ERR_INVALID_PARAMETER: The input parameter is unavailable.
 * @li For details about the return values, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_set_cco_mac_addr(EXT_CONST td_u8 mac_addr[EXT_PLC_MAC_ADDR_LEN]);

/**
 * @ingroup network
 * @brief  Writes the MAC address of the PLC to the NV in the factory partition.
 *
 * @par   Description:
 * Writes the MAC address of the PLC to the NV in the factory partition.
 *
 * @attention  The following MAC addresses cannot be used:
 * 0x00: 0x00: 0x00: 0x00: 0x00: 0x00, 0xff: 0xff: 0xff: 0xff: 0xff: 0xff, 0x00: 0xff: 0xff: 0xff: 0xff:
 * 0xff. After the setting is successful, only the MAC address is written to the NV in the factory
 * partition.
 * The MAC address can be used for PLC networking only after hard restart. You can query the MAC address
 * using uapi_get_local_mac_addr().
 * @param  plc_mac [IN] Type #td_u8 PLC MAC address
 *
 * @retval   #EXT_ERR_SUCCESS The PLC_MAC address is set successfully.
 * @retval   #EXT_ERR_FAILURE The PLC_MAC address fails to be set.
 * @retval   #EXT_ERR_INVALID_PARAMETER The input parameter is unavailable.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_set_plc_mac(EXT_CONST td_u8 plc_mac[EXT_PLC_MAC_ADDR_LEN]);

/**
 * @ingroup network
 * @brief Specifies whether the STA is allowed to associate with the CCO only after the app sets a MAC address.
 *
 * @par  Description:
 * Specifies whether the STA is allowed to associate with the CCO only after the app sets a MAC address.
 *
 * @attention This API is called by the STA. This function is disabled by default, which can be only called by the STA
 * during initialization. Dynamic calling cannot ensure the validity of settings.
 * @param  is_assoc_by_app_mac [IN] Type #td_bool, #TD_TRUE: If this function is enabled, the STA can be associated with
 * the CCO only after the app calls uapi_set_backup_mac_addr() to set a valid MAC address.
 * #TD_FALSE: If this function is disabled and the app does not specify a MAC address within the specified
 * period, the STA uses the default MAC address to associate with the CCO.
 * @retval   ret Type #td_void
 *
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_set_assoc_by_app_mac(td_bool is_assoc_by_app_mac);

/**
 * @ingroup network
 * @brief   Sets the STA type.
 *
 * @par   Description:
 * Sets the STA type.
 *
 * @attention The product type is STA.
 * @param  sta_type [IN] Type #td_u8 STA product type
 *
 * @retval #EXT_ERR_SUCCESS     Success
 * @retval #EXT_ERR_NOT_SUPPORT The device does not support this setting.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_set_sta_product_type(td_u8 sta_type);

/**
 * @ingroup network
 * @brief  Obtains the CCO MAC address.
 *
 * @par   Description:
 * Obtains the CCO MAC address.
 *
 * @attention The product type is STA.
 * The CCO MAC address can be queried only after the STA accesses the network.
 * @param  mac                       [IN/ OUT] Type #td_u8 CCO MAC address obtaining
 *
 * @retval #EXT_ERR_SUCCESS           The CCO MAC address is obtained successfully.
 * @retval #EXT_ERR_FAILURE           The CCO MAC address fails to be obtained.
 * @retval #EXT_ERR_INVALID_PARAMETER The input parameter is unavailable.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_get_cco_mac_addr(EXT_INOUT td_u8 mac[EXT_PLC_MAC_ADDR_LEN]);

/**
 * @ingroup network
 * @brief  Queries the operating frequency band.
 *
 * @par  Description:
 * Queries the operating frequency band.
 *
 * @attention When the API is used on the STA, if multiple frequency band blind detections exist, the API obtains the
 * blind detection frequency band of the STA when the query is performed. Only after the STA accesses
 * the network, the API can obtain the final network operating frequency band.
 * @param  None
 *
 * @retval   Operating frequency band Band 3 indicates 76K-145K_IS, and band 6 indicates 500K-3.7M_SG. Other values are
 * not defined currently.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u8 uapi_get_whole_net_work_freq(td_void);

/**
 * @ingroup network
 * @brief  Sets the network-wide operating frequency band.
 *
 * @par  Description:
 * Sets the network-wide operating frequency band.
 *
 * @attention The product type is CCO. If the frequency band after the switchover is different from the original
 * operating frequency band, soft reset will be performed on the CCO and the re-networking is performed.
 * @param   freq        [IN] Type #td_u8, 3: 76K-145KHz_IS, 6: 0.5-3.7MHz_SG (Other values are not described again.
 * The frequency bands supported by blind detection in the NV item 0x55 can be configured using this API.)
 * @param   change_time [IN] Type #td_u32 Network-wide frequency band switching delay (unit: s) If the value is less
 * than 10s, the default value defined by the whole_network_change_freq_time item in the NV item 0x55 is used.
 *
 * @retval  #EXT_ERR_SUCCESS: success
 * @retval  #EXT_ERR_FAILURE: failure
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_set_whole_net_work_freq(td_u8 freq, td_u32 change_time);

/**
 * @ingroup network
 * @brief  Sets a network ID.
 *
 * @par  Description:
 * Sets a network ID.
 *
 * @attention The product type is CCO.
 * @param  nid   [IN] Type #td_u32: Network ID. Value range: [16, 16777215]
 *
 * @retval #EXT_ERR_SUCCESS Success
 * @retval #EXT_ERR_INVALID_PARAMETER The input parameter is invalid. For details about the return value definitions,
 * see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_set_my_nid(td_u32 nid);

/**
 * @ingroup network
 * @brief  Obtains a network ID.
 *
 * @par  Description:
 * Obtains a network ID.
 *
 * @attention If the station is not connected to the network, the network ID is 0.
 * @param  None
 *
 * @retval   Network ID. Value range: [16, 16777215]
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_get_my_nid(td_void);

/**
 * @ingroup network
 * @brief V100 protocol version
 */
#define EXT_MDM_PTCL_VER_100 0x01

/**
 * @ingroup network
 * @brief State Grid protocol version number
 */
#define EXT_MDM_PTCL_VER_SG 0x40

/**
 * @ingroup network
 * @brief Version number of the 1901.1 protocol
 */
#define EXT_MDM_PTCL_VER_1901 0x60

/**
 * @ingroup network
 * @brief  Obtains the protocol version.
 *
 * @par   Description:
 * Obtains the protocol version.
 *
 * @attention None
 * @param  None
 *
 * @retval   protocol_ver [IN] Type #td_u16. For details, see #EXT_MDM_PTCL_VER_100.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see EXT_MDM_PTCL_VER_100 definition in the soc_mdm_mac.h file
 * @since DW21_V100R001C00
 */
td_u16 uapi_get_protocol_ver(td_void);
/**
 * @ingroup network
 * @brief  Indicates the callback function type that the MAC layer instructs to the app layer.
 *
 * @par  Description:
 * Indicates the callback function type that the MAC layer instructs to the app layer.
 *
 * @attention None
 * @param  Event     [IN]  Type #td_u32 MAC event type
 * @param  EventInfo [OUT] Type #td_pvoid Event information, which is transferred during registration and sent back
 * during callback.
 *
 * @retval   None
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
typedef td_void (*mac_event_notify_fun)(td_u32 Event, td_pvoid EventInfo);

/**
 * @ingroup network
 * @brief  Information about the event report node at the MAC layer
 */
typedef struct {
    EXT_LIST node;                             /**< Pointer to the linked list of the list_node event */
    mac_event_notify_fun mac_event_notify_if; /**< Callback function of the MAC event notification */
    td_u32 mac_event;                         /**< MAC event */
} ext_mac_event_notify_node;

/**
 * @ingroup network
 * @brief  MAC event at the CCO end: The first networking is complete.
 */
#define MAC_FRIST_NETWORK_FORMED (1 << 0)

/**
 * @ingroup network
 * @brief  MAC event at the CCO end: Route optimization is complete.
 */
#define MAC_ROUTE_OPTIMAL_COMPLETION (1 << 1)

/**
 * @ingroup network
 * @brief  MAC event at the CCO end: The networking is complete not for the first time.
 */
#define MAC_NETWORK_FORMED (1 << 2)

/**
 * @ingroup network
 * @brief  MAC event at the CCO end: Re-networking is performed.
 */
#define MAC_RE_NETWORKING (1 << 3)

/**
 * @ingroup network
 * @brief  MAC event at the CCO end: The frequency band is switched.
 */
#define MAC_FREQ_SWITCH (1 << 4)

/**
 * @ingroup network
 * @brief  MAC event at the CCO end: The node joins the network.
 */
#define MAC_STATION_JOIN (1 << 5)

/**
 * @ingroup network
 * @brief  MAC event at the CCO end: The node goes offline.
 */
#define MAC_STATION_LEAVE (1 << 6)

/**
 * @ingroup network
 * @brief  MAC event at the CCO end: A proxy change occurs on the node.
 */
#define MAC_STATION_PROXY_CHANGE (1 << 7)
/**
 * @ingroup network
 * @brief  MAC event at the CCO end: Security negotiation result report.
 */
#define MAC_SECURITY_NEGOTIATION_REPORT (1 << 8) /* Left shift 8 bits */
/**
 * @ingroup network
 * @brief  The station determines the causes of going offline and re-networking.
 */
typedef enum {
    RCV_BEACON_FAILED = 1, /**< The STA fails to receive the beacon (a complete heartbeat period). */
    CHOSE_PROXY_FAIL,      /**< Information about the proxy station fails to be received (four routing
                            * periods). */
    SERIAL_NUM_CHANGE,     /**< The networking serial number changes. */
    RCV_LEAVE_IND,         /**< An offline indication packet is received. */
    CCO_MAC_CHANGE,        /**< The MAC address of the CCO changes. */

    SAME_NEIGHBOR_SNID,          /**< Used by the CCO. The SNID of the neighbor network is the same as the SNID
                                  * of the local network. */
    SET_FREQ_MODE,               /**< The STA receives the command for directly changing the frequency band in
                                  * the NV item. (This operation is not recommended.) */
    SET_MAC_ADDR,                /**< The STA receives the command for changing the MAC address. */
    SET_WHITE_LIST,              /**< The CCO restarts when it removes 140 stations using the whitelist. */
    PROXY_CHANGE_DISCOVERY_NODE, /**< The STA detects that the proxy identity of the local station is changed to
                                  * the discovery station. */

    NOT_IN_WRITER = 11,     /**< Stations are removed using the whitelist: 11. */
    DBK_SET_CCO_SNID,       /**< The CCO SNID is changed by running the DBK command. */
    PCO_MAX_LEVEL,          /**< The station level exceeds the maximum. */
    INTERFACE_SET_CCO_SNID, /**< The API for the secondary development is used to set the CCO SNID. */
    NOT_IN_BACKUP_MAC_LIST, /**< The MAC address of the local station is not in the backup MAC address
                             * list. */

    STA_SET_PRODUCT_TYPE,       /**< The product type of the station is different. */
    NDM_METER_ACCESS,           /**< The meter test of the network diagnose monitor (NDM) is forcibly connected. */
    NDM_METER_CHECK_DISCONNECT, /**< The meter test of the NDM is disconnected. */
    PLC_SWITCH_TO_RF,           /**< The PLC network mode is switched to the RF mode. */
    RF_SWITCH_TO_PLC,           /**< The RF network mode is switched to the PLC mode. */

    TF_IDENTIFY_RESET = 21,           /**< The transformer district identification is restarted. */
    TF_IDENTIFY_CHECK_SUPPORT_RESET,  /**< If the STA detection supports the transformer district identification and
                                       * an association request is initiated, soft restart is performed on the STA. */
    NM_ENTER_NETWORK_TEST_MODE_RESET, /**< The MAC layer enters the network test mode. */
    NM_EXIT_NETWORK_TEST_MODE_RESET,  /**< The MAC layer exits the network test mode. */
    NM_RCV_NETWORK_CONFLICT_PK,       /**< A network conflict packet is received. */

    STA_EXIT_NON_ASSOCIATE = 26, /**< The STA exits the non-networking state. */
    BBPLC_SWITCH_TO_SCPLC,       /**< The broadband PLC network mode is switched to the single-carrier PLC
                                  * mode. */
    SCPLC_SWITCH_TO_BBPLC,       /**< The single-carrier PLC network mode is switched to the broadband PLC
                                  * network mode. */
    ENTER_PHY_TEST_MODE,         /**< The State Grid network test system enters the physical layer test mode. */
    EXIT_PHY_TEST_MODE,          /**< The State Grid test system postpones the physical layer test mode. */

    NETWORK_MODE_SWITCH = 31,     /**< The network mode is switched. */
    NETWORK_PTCL_SWITCH,          /**< The network protocol is switched. */
    REJOIN_CALL_FROM_APP_LAYER,   /**< The app layer calls the API for re-accessing the network. */
    CHANGE_LOCK_NETWORK,          /**< The locked network is changed and short-term convergence is performed. */
    DEVICE_POWER_FAILURE_RECOVER, /**< The power supply is recovered. */

    STA_NOT_RCV_LEAVE_IND = 36, /**< In energy mode, the STA proactively goes offline when the unlocked network
                                 * is fully loaded. */
    CHANGE_LOCK_NETWORK_LONG,   /**< The locked network is changed and long-term convergence is performed. */
    CHANGE_TO_BELONG_NETWORK,   /**< The current network is exited, and the manually specified home network is
                                 * dissociated. */
    CHANGE_NOT_JOIN_LOCK_NET,   /**< The joined network is not locked. */
    CHANGE_LOCK_NETWORK_DBK,    /**< The network is locked by running the DBK command. */

    MAC_REJOIN_REASON_INVALID = 0xff /**< Cause of the invalid soft restart */
} ext_mdm_mac_rejoin_reason;

/**
 * @ingroup network
 * @brief  Re-networking event information
 */
typedef struct {
    td_u8 reason; /**< Re-networking reason */
    td_u8 pad[3]; /**< Reserved 3 bytes */
} ext_mac_renetworking_info;

/**
 * @ingroup network
 * @brief  Node joining event information
 */
typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN]; /**< Node MAC address */
    td_u16 tei;                     /**< Node TEI */

    td_u8 user_defined_info; /**< only effective in frequency three */
    td_u8 pad[3];            /**< Reserved 3 bytes */
} ext_mac_station_join_info;

/**
 * @ingroup network
 * @brief  Node offline event information
 */
typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN]; /**< Node MAC address */
    td_u16 tei;                     /**< Node TEI */
    td_u8 leave_reason;             /**< Offline reason */
    td_u8 pad[3];                   /**< Reserved 3 bytes */
} ext_mac_station_leave_info;

/**
 * @ingroup network
 * @brief  Node proxy change event information
 */
typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN]; /**< Node MAC address */
    td_u16 tei;                     /**< Node TEI */
} ext_mac_station_proxy_change_info;

/**
 * @ingroup network
 * @brief  MAC event at the STA end: The frequency band is switched.
 */
#define MAC_STA_FREQ_SWITCH (1 << 0)

/**
 * @ingroup network
 * @brief  MAC event at the STA end: The network is accessed.
 */
#define MAC_STA_ACCESS_NETWORK (1 << 1)

/**
 * @ingroup network
 * @brief  MAC event at the STA end: The access to the network fails.
 */
#define MAC_STA_ACCESS_NETWORK_FAIL (1 << 2) /* Left shift 2 bits */

/**
 * @ingroup network
 * @brief  MAC event at the STA end: The access to the network is successful.
 */
#define MAC_STA_ACCESS_NETWORK_SUCCESS (1 << 3) /* Left shift 3 bits */

/**
 * @ingroup network
 * @brief  MAC event at the STA end: The networking is complete.
 */
#define MAC_STA_NETWORK_FORMED (1 << 4) /* Left shift 4 bits */

/**
 * @ingroup network
 * @brief  MAC event at the STA end: A node goes offline.
 */
#define MAC_STA_OFFLINE (1 << 5) /* Left shift 5 bits */

/**
 * @ingroup network
 * @brief  MAC event at the STA end: The node agent is changed.
 */
#define MAC_STA_PROXY_CHANGE (1 << 6) /* Left shift 6 bits */

/**
 * @ingroup network
 * @brief  MAC event at the STA end: The node role is changed.
 */
#define MAC_STA_ROLE_CHANGE (1 << 7) /* Left shift 7 bits */

/**
 * @ingroup network
 * @brief  MAC event at the STA end: The node level is changed.
 */
#define MAC_STA_LEVEL_CHANGE (1 << 8) /* Left shift 8 bits */
/**
 * @ingroup network
 * @brief  MAC event at the STA end: Security negotiation result report.
 */
#define MAC_STA_SECURITY_NEGOTIATION_REPORT (1 << 9) /* Left shift 9 bits */
/**
 * @ingroup network
 * @brief  MAC event at the STA end: Security negotiation result report.
 */
#define MAC_STA_END_MESH_STATE (1 << 10) /* Left shift 9 bits */
/**
 * @ingroup network
 * @brief  Frequency band change reason
 */
typedef enum {
    CHANGE_BY_INIT = (1 << 0),        /**< Initialization */
    CHANGE_BY_NV_LOCAL = (1 << 1),    /**< The NV item is modified locally. */
    CHANGE_BY_NV_NOTIFY = (1 << 2),   /**< The CCO instructs frequency band modification, left shift 2 bits */
    CHANGE_BY_BLIND = (1 << 3),       /**< Blind detection, left shift 3 bits  */
    CHANGE_BY_FREQ_NOTIFY = (1 << 4), /**< Frequency band selection frame, left shift 4 bits  */
    CHANGE_BY_DBK = (1 << 5),         /**< Left shift 5 bits, the configuration window is modified by running the DBK
                                           command */
    CHANGE_BY_FOLLOW = (1 << 6),      /**< Left shift 6 bits, The copy controller follows the frequency band
                                           switching */
    CHANGE_BY_NDM_METER = (1 << 7),   /**< Left shift 7 bits, The frequency band is switched during testing. */
    CHANGE_BY_NDM_FL = (1 << 8),      /**< Left shift 8 bits, switched for the fault locating of the copy controller
                                           collector. */
    CHANGE_BY_RF_MULTI_NETWORK = (1 << 9), /**< Left shift 9 bits, The frequency band is switched in RF mode. */
    CHANGE_BY_TF_IDENTIFY = (1 << 10),     /* Left shift 10 bits,  switched for the identification of the transformer
                                              district. */
    CHANGE_BY_PTPC = (1 << 11),            /**< Left shift 11 bits, The frequency band is switched for point-to-point
                                                communication */
    CHANGE_BY_SC = (1 << 12),              /**< Left shift 12 bits, The frequency band is switched for the single
                                                carrier. */
} ext_mdm_freq_switch_reason;

/**
 * @ingroup network
 * @brief  MAC event information at the STA end: frequency band change information
 */
typedef struct {
    td_u8 freq;          /**< Frequency band No */
    td_u8 swtich_reason; /**< Frequency band change reason */
    td_u8 pad[2];        /**< Reserved 2 bytes */
} ext_mac_sta_event_freq_switch_info;

/**
 * @ingroup network
 * @brief  Network access event at STA side
 */
typedef struct {
    td_u32 access_nid; /**< Network NID */
    td_u8 access_freq; /**< Network frequency band */
    td_u8 pad[3];      /**< Reserved 3 bytes */
} ext_mac_sta_event_access_network_info;

/**
 * @ingroup network
 * @brief  Association failure cause(defined in protocol, shoult not be edited.)
 */
typedef enum {
    ASSOC_SUCCESS = 0,                 /**< Association succeeded. */
    ASSOC_ERR_NOT_WHITER = 1,          /**< Not in the whitelist */
    ASSOC_ERR_BLACKER = 2,             /**< In the blacklist */
    ASSOC_ERR_TOO_MUCH_STA = 3,        /**< The number of stations to be added exceeds the upper limit. */
    ASSOC_ERR_NOT_SET_WHITER_LIST = 4, /**< The whitelist is not set. */
    ASSOC_ERR_TOO_MUCH_PCO = 5,        /**< The number of proxy stations exceeds the upper limit. */
    ASSOC_ERR_TOO_MUCH_CHILD = 6,      /**< The number of sub-stations exceeds the upper limit. */
    ASSOC_ERR_NO_RETURN = 7,           /**< No reply, V100 protocol. */
    ASSOC_ERR_REPEAT_MAC = 8,          /**< Duplicate MAC address. */
    ASSOC_ERR_PCO_MAX_LEVEL = 9,       /**< The number of topology levels exceeds the upper limit. */
    ASSOC_RIGHT_ALLOC_TEI_BEFORE = 10, /**< The station that has accessed the network requests to access the network
                                            again. */
    ASSOC_ERR_JOIN_MY_CHILD = 11,      /**< A new station attempts to access the network by using its sub-node as a
                                            proxy. */
    ASSOC_ERR_HAVE_CIRCLE = 12,        /**< A loop exists in the network topology. */
    ASSOC_ERR_UNKNOW = 13              /**< An error occurs at the CCO end, with the reason unknown. */
} ext_mdm_assoc_err_reason;

/**
 * @ingroup network
 * @brief  MAC event information at the STA end: Accessing the network fails.
 */
typedef struct {
    td_u32 access_nid;                  /**< Network NID */
    td_u8 access_freq;                  /**< Network frequency band */
    td_u8 cco_mac[EXT_PLC_MAC_ADDR_LEN]; /**< CCO MAC address */
    td_u8 fail_reason;                  /**< Association failure cause */
} ext_mac_sta_event_access_fail_info;

/**
 * @ingroup network
 * @brief  MAC event information at the STA end: Accessing the network succeeds.
 */
typedef struct {
    td_u32 access_nid;                  /**< Network NID */
    td_u8 access_freq;                  /**< Network frequency band */
    td_u8 cco_mac[EXT_PLC_MAC_ADDR_LEN]; /**< CCO MAC address */
    td_u8 pad;                          /**< Reserved */
} ext_mac_sta_event_access_success_info;

/**
 * @ingroup network
 * @brief  MAC event information at the STA end: The node goes offline.
 */
typedef struct {
    td_u8 leave_reason; /**< Offline reason */
    td_u8 pad[3];       /**< Reserved 3 bytes */
} ext_mac_sta_event_leave_info;

/**
 * @ingroup network
 * @brief  MAC event information at the STA end: The node agent is changed.
 */
typedef struct {
    td_u16 old_proxy_tei; /**< Old proxy's TEI */
    td_u16 new_proxy_tei; /**< New proxy's TEI */
} ext_mac_sta_event_proxy_change_info;

/**
 * @ingroup network
 * @brief  Node role: CCO
 */
#define IS_CCO EXT_MDM_PLC_ROLE_CCO

/**
 * @ingroup network
 * @brief  Node role: proxy node
 */
#define IS_PROXY EXT_MDM_PLC_ROLE_PROXY

/**
 * @ingroup network
 * @brief  Node role: discovery node
 */
#define IS_STA EXT_MDM_PLC_ROLE_STA
/**
 * @ingroup network
 * @brief  Node role: NDM
 */
#define IS_NDM 0xFE

/**
 * @ingroup network
 * @brief  MAC event information at the STA end: The node role is changed.
 */
typedef struct {
    td_u8 role;   /**< Node role */
    td_u8 pad[3]; /**< Reserved 3 bytes */
} ext_mac_sta_event_role_change_info;

/**
 * @ingroup network
 * @brief  MAC event information at the STA end: The node level is changed.
 */
typedef struct {
    td_u8 level;  /**< Node level */
    td_u8 pad[3]; /**< Reserved 3 bytes */
} ext_mac_sta_event_level_change_info;

/**
 * @ingroup network
 * @brief  Registers the callback function that notifies the app of the MAC event.
 *
 * @par  Description:
 * Registers the callback function that notifies the app of the MAC event.
 *
 * @attention This API runs only on the CCO or STA. A maximum of 32 callback functions can be registered.
 * @param  mac_event               [IN] Type #td_u32 MAC or STA event such as #MAC_STA_FREQ_SWITCH, and CCO event such
 * as #MAC_FRIST_NETWORK_FORMED
 * @param  mac_event_call_back_fun [IN] Type #mac_event_notify_fun Callback function that notifies the app of the MAC
 * event
 *
 * @retval #EXT_ERR_SUCCESS           The adding is successful.
 * @retval #EXT_ERR_INVALID_PARAMETER The pointer to the input parameter is empty.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_register_mac_event_call_back(td_u32 mac_event, mac_event_notify_fun mac_event_call_back_fun);

/**
 * @ingroup network
 * @brief  Sets the soft restart for the network layer of the station.
 *
 * @par  Description:
 * Sets the soft restart for the network layer of the station. After the configuration on the CCO is
 * successful, all stations on the network go offline and re-networking is performed. After the configuration
 * on the STA is successful, the station goes offline and accesses the network again.
 *
 * @attention When this API is called on the CCO, the flash memory is written. The calling frequency must be controlled.
 * Otherwise, the service life of the flash memory is shortened.
 * @param  None
 *
 * @retval   #EXT_ERR_SUCCESS Success
 * @retval   Other values.   Failure. For details about the return value definitions, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_rejoin_network(td_void);

/**
 * @ingroup network
 * @brief  Authentication infomation of an STA
 */
typedef struct {
    td_u32 nid;                         /**< NID to be certificated */
    td_u8 freq;                         /**< Frequency band to be certified */
    td_u8 cco_mac[EXT_PLC_MAC_ADDR_LEN]; /**< CCO MAC address to be certified */
    td_u8 pad;
} ext_sta_auth_network_info;

/**
 * @ingroup network
 * @brief  Specifies whether the nodes on an unlocked network proactively go offline when the network is fully loaded.
 *
 * @par  Description:
 * Specifies whether the nodes on an unlocked network proactively go offline when the network is fully loaded.
 *
 * @attention
 * @li The product type is STA.
 * @li Before enabling this function, ensure that the station on the local network has locked the network
 * @li by using uapi_set_lock_assoc_cco_mac_switch() and uapi_register_lock_assoc_cco_mac_func(). Otherwise,
 * the STA automatically goes offline.
 * @param   is_offline_by_unlock_enable [IN] Type #td_bool Whether the function is enabled
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_set_offline_by_unlock_switch(td_bool is_offline_by_unlock_enable);

/**
 * @ingroup network
 * @brief  Sets a timeout period for the networking on the CCO.
 *
 * @par  Description:
 * Sets a timeout period for the networking on the CCO.
 * @li When the timeout period expires and there are stations online, the networking is forcibly set to the
 * completion state.
 * @li The timeout interval must be greater than  60000 ms.
 *
 * @attention The product type is CCO.
 * @param   is_net_state_form_enable  [IN] Type #td_bool Whether the function is enabled
 * @param   net_state_form_timeout    [IN] Type #td_u32 Timeout interval (unit: ms). The value must be greater than
 * 60000 ms.
 *
 * @retval  #EXT_ERR_SUCCESS           Success.
 * @retval  #EXT_ERR_INVALID_PARAMETER Parameter error.
 * @retval  #EXT_ERR_BUSY              Repeated startup is not allowed.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_set_network_state_form_switch(td_bool is_net_state_form_enable, td_u32 net_state_form_timeout);

/**
 * @ingroup network
 * @brief Configures a beacon period.
 *
 * @par   Description:
 * Configures a beacon period.
 *
 * @attention The product type is CCO. This interface does not support star networking.
 * @param  min_period       [IN] Type #td_u32 Minimum beacon period. The value cannot be less than 1000 (unit: ms).
 * @param  max_period       [IN] Type #td_u32 Maximum beacon period. The value cannot be greater than 10000 (unit: ms).
 *
 * @retval #EXT_ERR_SUCCESS Success
 * @retval #EXT_ERR_FAILURE Failure
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_set_tdma_period(td_u32 min_period, td_u32 max_period);

/**
 * @ingroup network
 * @brief Obtains the length of the current beacon period.
 *
 * @par   Description:
 * Obtains the length of the current beacon period.
 *
 * @attention None
 * @param  None
 *
 * @retval Length of the current beacon period (unit: 100 us)
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_get_tdma_period_len(td_void);

/**
 * @ingroup network
 * @brief Disables the networking operation and the transmission of the central beacon.
 *
 * @par   Description:
 * Set the CCO non-networking mode, that is, the CCO does not send the central beacon.
 *
 * @attention The product type is CCO.
 * @param  is_stop_networking [IN] Type #td_bool CCO not involved in networking
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_set_stop_networking(td_bool is_stop_networking);

/**
 * @ingroup network
 * @brief Specifies whether to bind CSMA timeslots.
 *
 * @par   Description:
 * Specifies whether to bind CSMA timeslots.
 *
 * @attention The product type is CCO.
 * @param  csma_period_flag          [IN] Type #td_bool Whether to start binding timeslots. #TD_TRUE: enabled.
 * #TD_FALSE: disabled.
 * @param  lid                       [IN] Type #td_u8 ID of the bound service link. Currently, this parameter is
 * applicable only to the link IDs of upgrade services. lid =  2.
 *
 * @retval #EXT_ERR_SUCCESS           Operation succeeded.
 * @retval #EXT_ERR_INVALID_PARAMETER The input parameter is invalid.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_set_bind_csma_period(td_bool csma_period_flag, td_u8 lid);

/**
 * @ingroup network
 * @brief Configures multiple heartbeat packets to be sent within a routing period.
 *
 * @par   Description:
 * Configures multiple heartbeat packets to be sent within a routing period.
 *
 * @attention
 * @li The product type is STA.
 * @li When the STA is in non-networking mode, the configuration parameters take effect when the STA accesses
 * the network next time.
 * @param  heartbeat_packet_num [IN] Type #td_u8 Number of heartbeat packets sent in a routing period Value range:
 * [4, 8].
 *
 * @retval #EXT_ERR_SUCCESS      Success
 * @retval Other values.        Failure
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_set_heartbeat_packet_num_per_cycle(td_u8 heartbeat_packet_num);

/**
 * @ingroup network
 * @brief Configures several heartbeat periods to determine whether a node is offline.
 *
 * @par   Description:
 * Configures several heartbeat periods to determine whether a node is offline.
 *
 * @attention The product type is CCO.
 * @param  offline_heartbeat_cycle [IN] Type #td_u8 Number of heartbeat periods for determining whether a node is
 * offline. Value range: [1, 4]
 *
 * @retval #EXT_ERR_SUCCESS         Success
 * @retval Other values.           Failure
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_set_offline_heartbeat_cycle(td_u8 offline_heartbeat_cycle);

/**
 * @ingroup network
 * @brief Configures whether to coordinate the NID.
 *
 * @par   Description:
 * Configures whether to coordinate the NID.
 *
 * @attention
 * @li The product type is CCO.
 * @li The control scope includes inter-network coordination frames and NID coordination triggered by
 * multi-network collision packets.
 * @param  nid_coordinate_switch [IN] Type #td_bool Whether to coordinate the NID
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_set_nid_coordinate_switch(td_bool nid_coordinate_switch);

/**
 * @ingroup network
 * @brief Configures multi-network timeslot coordination.
 *
 * @par   Description:
 * Configures multi-network timeslot coordination.
 *
 * @attention The product type is CCO.
 * @param  bandwidth_coordinate_switch [IN] Type #td_bool Multi-network timeslot coordination
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_set_bandwidth_coordinate_switch(td_bool bandwidth_coordinate_switch);

/**
 * @ingroup network
 * @brief Maximum number of neighbor networks
 */
#define EXT_MAX_NEIGHBOR_NET_TOTAL_NUM 15

/**
 * @ingroup network
 * @brief Neighbor network infomation entry
 */
typedef struct {
    td_u32 nid;             /**< ID of the neighbor network */
    td_u32 band_width;      /**< Neighbor network bandwidth */
    td_bool is_single_comm; /**< Whether it is unidirectional communication */
    td_u8 pad[3];           /**< 3 bytes reserved */
} ext_mdm_neighbor_net_entry;

/**
 * @ingroup network
 * @brief Neighbor network infomation
 */
typedef struct {
    td_u16 neighbor_net_num;                                        /**< Number of neighbor networks */
    td_u16 pad;                                                     /**< Reserved */
    ext_mdm_neighbor_net_entry entry[EXT_MAX_NEIGHBOR_NET_TOTAL_NUM]; /**< Neighbor network information array */
} ext_mdm_neighbor_net_info;

/**
 * @ingroup network
 * @brief  Obtains neighbor network information.
 *
 * @par   Description:
 * Obtains neighbor network information.
 *
 * @attention None
 * @param  neighbor_net_info         [IN/ OUT] Type #ext_mdm_neighbor_net_info* Stored neighbor network information
 *
 * @retval #EXT_ERR_SUCCESS           Success
 * @retval #EXT_ERR_FAILURE           Failure
 * @retval #EXT_ERR_INVALID_PARAMETER The input parameter is unavailable.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_get_neighbor_net_info(ext_mdm_neighbor_net_info *neighbor_net_info);

/**
 * @ingroup network
 * @brief  Obtains the number of neighbor networks (excluding the local network).
 *
 * @par   Description:
 * Obtains the number of neighbor networks (excluding the local network).
 *
 * @attention None
 * @param  None
 *
 * @retval #td_u32           Number of neighbor networks obtained, excluding the local network
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @since DW21_V100R001C00
 */
td_u32 uapi_get_neighbor_net_num(td_void);
/**
 * @ingroup network
 * @brief   Sets the whitelist switch.
 *
 * @par  Description:
 * Sets the whitelist switch.
 *
 * @attention The product type is CCO.
 * @param  is_on          [IN] Type #td_bool Whether the whitelist is enabled 0: disable; 1: enable
 * @param  is_permanent   [IN] Type #td_bool Whether the configuration takes effect upon the next restart 0: no. 1: yes
 * @param  reason         [IN] Type #ext_mac_white_list_change_reason Whitelist change reason code. For details,
 * see the definition.
 *
 * @retval #EXT_ERR_SUCCESS Success
 * @retval #EXT_ERR_FAILURE Failure
 * @retval Other values.      For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_set_white_list_switch(td_bool is_on, td_bool is_permanent, ext_mac_white_list_change_reason reason);

/**
 * @ingroup network
 * @brief   Obtains the whitelist switch status.
 *
 * @par  Description:
 * Obtains the whitelist switch status.
 *
 * @attention The product type is CCO.
 * @param  is_on         [OUT] Type #td_bool* Whether the whitelist is enabled. #TD_FALSE: disabled; #TD_TRUE: enabled
 *
 * @retval #EXT_ERR_SUCCESS Success
 * @retval #EXT_ERR_FAILURE Failure
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_get_white_list_switch(td_bool *is_on);

/**
 * @ingroup network
 * @brief  Sets the network whitelist.
 *
 * @par  Description:
 * Sets the network whitelist.
 *
 * @attention  The product type is CCO.
 * The caller must ensure that list_num in #ext_mac_valid_mac_list indicates the number of array elements,
 * which matches the length of the actual array mac_list.
 * The new whitelist overwrites the existing whitelist. Therefore, you need to consider whether to import the
 * original whitelist to the new whitelist before the setting.
 * @param  list   [IN] Type #ext_mac_valid_mac_list* Configured MAC address list in the whitelist
 * @param  reason [IN] Type #ext_mac_white_list_change_reason Reason why the whitelist is modified
 *
 * @retval  None
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see ext_mac_valid_mac_list definition
 * @since DW21_V100R001C00
 */
td_void uapi_set_valid_mac_list(EXT_CONST ext_mac_valid_mac_list *list, ext_mac_white_list_change_reason reason);

/**
 * @ingroup network
 * @brief  Specifies whether to enable the network access authentication function at the application layer.
 *
 * @par  Description:
 * Configures the application layer authentication function.
 *
 * @attention The product type is CCO.
 * @param   is_app_auth [IN] Type #td_bool Whether the function is enabled
 * @li #TD_TRUE: The authentication function is enabled at the application layer.
 * @li #TD_FALSE: The authentication function is disabled at the application layer.
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_set_app_auth(td_bool is_app_auth);

/**
 * @ingroup network
 * @brief  Specifies the number of stations to be authenticated by the whitelist at the application layer.
 *
 * @par  Description:
 * Specifies the number of stations to be authenticated by the whitelist at the application layer.
 *
 * @attention The product type is CCO.
 * @param   white_list_num [IN] Type #td_u16 Number of stations in the whitelist Value range: 0-65535
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_set_app_white_list_num(td_u16 white_list_num);

/**
 * @ingroup network
 * @brief  Sets the whitelist at the application layer to determine whether to allow stations to access the network.
 *
 * @par  Description:
 * Sets the whitelist at the application layer to determine whether to allow stations to access the network.
 *
 * @attention The product type is CCO.
 * @param   is_allow_assoc [IN] Type #td_bool Whether the network access is allowed
 * @li #TD_TRUE: allowed
 * @li #TD_FALSE: not allowed
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_set_allow_assoc(td_bool is_allow_assoc);

/**
 * @ingroup network
 * @brief  Obtains the flag indicating whether the application layer allows stations to access the network.
 *
 * @par  Description:
 * Obtains the flag indicating whether the application layer allows stations to access the network.
 *
 * @attention The product type is CCO.
 * @param   None
 *
 * @retval #TD_TRUE  Stations are allowed to join the network.
 * @retval #TD_FALSE Stations are not allowed to join the network.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_bool uapi_get_allow_assoc(td_void);

/**
 * @ingroup network
 * @brief  Specifies whether to carry the network full load flag in the beacon at the application layer.
 *
 * @par  Description:
 * Specifies whether to carry the network full load flag in the beacon at the application layer.
 *
 * @attention The product type is CCO.
 * @param   is_full_load [IN] Type #td_bool Whether to enable the network full load flag
 * @li #TD_TRUE: enabled
 * @li #TD_FALSE: disabled
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_set_full_load_assoc_switch(td_bool is_full_load);

/**
 * @ingroup network
 * @brief  Allows the app layer to set whether the network whitelist carried in the beacon is empty.
 *
 * @par  Description:
 * Sets the flag indicating whether the network whitelist carried in the beacon is empty at the application
 * layer. If the carried whitelist is empty, the station association is not allowed.
 *
 * @attention The product type is CCO.
 * @param   is_white_list_assoc_enable [IN] Type #td_bool Flag indicating whether the network whitelist carried in the
 * beacon is empty
 * @li #TD_TRUE: enabled
 * @li #TD_FALSE: disabled
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_set_white_list_assoc_switch(td_bool is_white_list_assoc_enable);

/**
 * @ingroup network
 * @brief  Obtains the number of packets received from the CCO at the application layer.
 *
 * @par  Description:
 *           Obtains the number of packets received from the CCO at the application layer.
 *
 * @attention The product type is STA.
 * @param   None
 *
 * @retval Value Number of packets from the CCO
 * @par Dependency:
 *           @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_get_cco_packet_cnt(td_void);

/**
 * @ingroup network
 * @brief  Locks the current network.
 *
 * @par  Description:
 * Locks the current network.
 *
 * @attention The product type is STA. This API is used to lock the network by locking the MAC address of the CCO.
 * @param   lock_duration [IN] Type #td_u32 Lock duration (unit: s).
 *
 * @retval  #EXT_ERR_SUCCESS The message indicating the successful setting is sent.
 * @retval  Other values indicate that the setting message fails to be sent. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_lock_network(td_u32 lock_duration);

/**
 * @ingroup network
 * Lock information
 */
typedef struct {
    td_u8 lock_status; /**< Lock status. For details, see #ext_lock_network_status. */
    td_u8 pad1;
    td_u8 cco_mac[EXT_PLC_MAC_ADDR_LEN]; /**< MAC address of the CCO on the locked network */
    td_u32 lock_begin_time;             /**< Lock start time (unit: s) */
    td_u32 lock_duration;               /**< Lock duration (unit: s) */
    td_u32 leave_time_len;              /**< Duration since the exit from the locked network (unit: s) */
} ext_mdm_lock_info;

/**
 * @ingroup network
 * Lock status
 */
typedef enum {
    EXT_UNLOCK_NETWORK_STATUS = 0, /**< The network status is not locked. */
    EXT_LOCK_CCO_MAC_STATUS,       /**< The MAC address status of the CCO is locked. */
} ext_lock_network_status;

/**
 * @ingroup network
 * @brief  Obtains the current network locking information of a station.
 *
 * @par  Description:
 * Obtains the current network locking information of a station.
 *
 * @attention The product type is STA.
 * @param   lock_info [OUT] Type #ext_mdm_lock_info* Current network locking information of the station
 *
 * @retval  #EXT_ERR_SUCCESS Success
 * @retval  #EXT_ERR_INVALID_PARAMETER The pointer to the input parameter is null.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_get_lock_info(ext_mdm_lock_info *lock_info);

/**
 * @ingroup network
 * @brief  Unlocks the network of the station.
 *
 * @par  Description:
 * Unlocks the network of the station.
 *
 * @attention The product type is STA.
 * @param   None
 *
 * @retval  #EXT_ERR_SUCCESS The unlock message is sent to the MAC layer successfully.
 * @retval  Other values indicate that the unlock message fails to be sent. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_unlock_network(td_void);

/**
 * @ingroup network
 * @brief    Registers the function at the STA end, which checks whether an association request for the CCO MAC address
 * is allowed.
 *
 * @par  Description:
 * Registers the function at the STA end, which checks whether an association request for the CCO MAC address
 * is allowed.
 *
 * @attention The product type is STA, which applies only to the V100 frequency band.
 * @param  cco_mac   [IN] Type #td_u8 MAC address of the CCO
 *
 * @retval #TD_TRUE  The association request is allowed for the CCO MAC address.
 * @retval #TD_FALSE The association request is not allowed for the CCO MAC address.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
typedef td_bool (*ext_mdm_handle_lock_assoc_cco_mac_fun)(EXT_CONST td_u8 cco_mac[EXT_PLC_MAC_ADDR_LEN]);

/**
 * @ingroup network
 * @brief  Registers the function at the STA end, which checks whether an association request for the CCO MAC address is
 *         allowed.
 *
 * @par  Description:
 *         The STA registration function uses the input CCO MAC address to determine whether the association is allowed.
 *         The association is allowed only when conditions are met.
 *
 * @attention The product type is STA.
 * @param   func                      [IN] Type #ext_mdm_handle_lock_assoc_cco_mac_fun Whether an association request is
 *         allowed to be performed on the CCO_MAC
 *
 * @retval  #EXT_ERR_SUCCESS           Registration succeeded.
 * @retval  #EXT_ERR_NOT_SUPPORT       The function is not supported currently.
 * @retval  #EXT_ERR_INVALID_PARAMETER The registration parameter is incorrect.
 * @par Dependency:
             @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_register_lock_assoc_cco_mac_func(ext_mdm_handle_lock_assoc_cco_mac_fun func);

/**
 * @ingroup network
 * @brief  Sets whether the check of the association request of the CCO MAC address is enabled on the STA.
 *
 * @par  Description:
 * Sets whether the check of the association request of the CCO MAC address is enabled on the STA.
 *
 * @attention The product type is STA.
 * @param is_set_switch   [IN] Type #td_bool Whether to enable the check of the association request of the CCO MAC
 * address on the STA
 * @li #TD_TRUE: enabled
 * @li #TD_FALSE: disabled
 *
 * @retval #EXT_ERR_SUCCESS Success
 * @retval Other values    Failure
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_set_lock_assoc_cco_mac_switch(td_bool is_set_switch);

/**
 * @ingroup network
 * @brief   Obtains the number of packets with the specified priority in the sending queue.
 *
 * @par  Description:
 * Obtains the number of packets with the specified priority in the sending queue.
 *
 * @attention None
 * @param  lid  [IN] Type #td_u8 Number of packets with a specified priority in the sending queue. Value range: [0, 254]
 *
 * @retval Value Number of packets with a specified priority in the sending queue
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u8 uapi_get_tx_queue_packet_num(EXT_IN td_u8 lid);

/**
 * @ingroup network
 * @brief   Specifies the maximum number of hops for broadcast or proxy broadcast packets on the entire network.
 *
 * @par  Description:
 * Specifies the maximum number of hops for broadcast or proxy broadcast packets on the entire network.
 *
 * @attention This API setting takes effect only for network-wide broadcast and proxy broadcast packets, but does not
 * take effect for local broadcast and unicast packets.
 * @param  max_hop_cnt     [IN] Type #td_u8 Maximum number of hops of packets. Value range: [1, 15]
 *
 * @retval #EXT_ERR_SUCCESS Success
 * @retval #EXT_ERR_FAILURE Failure
 * @retval Other values.      For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_set_max_hopcnt(td_u8 max_hop_cnt);

/**
 * @ingroup network
 * @brief   Sets the packet timeout period.
 *
 * @par  Description:
 * Sets the packet timeout period.
 *
 * @attention The timeout periods of packets with the following three priorities can be modified: 0, 2, and 6. You can
 * modify the timeout periods of the packets with the priority of 0, 2, and 6.
 * @attention At most five priorities can be added. If a priority does not exist, the system automatically adds it.
 * A maximum of five priorities can be added.
 * @attention The default size of each new queue is 12. You can call uapi_set_tx_queue_size_by_lid to change the value
 * to another valid value.
 *
 * @param  queue_priority    [IN] Type #td_u8 Packet priority. Value range: [0, 254]
 * @param  timeout           [IN] Type #td_u16 Packet timeout period (unit: ms). Value range: (0, 30000]
 *
 * @retval   #EXT_ERR_SUCCESS Success
 * @retval   #EXT_ERR_FAILURE Failure
 * @retval   Other values.      For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_set_packet_timeout(td_u8 queue_priority, td_u16 timeout);

/**
 * @ingroup network
 * @brief   Specifies the maximum number of times for sending unicast packets.
 *
 * @par  Description:
 * Specifies the maximum number of times for sending unicast packets.
 *
 * @attention None
 * @param  max_resend_cnt [IN] Type #td_u8 Maximum number of times for sending unicast packets. Value range: [1, 10]
 *
 * @retval #EXT_ERR_SUCCESS Success
 * @retval #EXT_ERR_FAILURE Failure
 * @retval Other values.      For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_set_unicast_max_resendcnt(td_u8 max_resend_cnt);

/**
 * @ingroup network
 * @brief   Specifies the maximum number of times for sending broadcast packets.
 *
 * @par  Description:
 * Specifies the maximum number of times for sending broadcast packets.
 *
 * @attention None
 * @param  max_resend_cnt [IN] Type #td_u8 Maximum number of times for sending broadcast packets. Value range: [1, 10]
 *
 * @retval #EXT_ERR_SUCCESS Success
 * @retval #EXT_ERR_FAILURE Failure
 * @retval Other values.      For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_set_broadcast_max_resendcnt(td_u8 max_resend_cnt);

/**
 * @ingroup network
 * @brief   Sets the size of the SOF frame queue by priority.
 *
 * @par  Description:
 * Sets the size of the SOF frame queue by priority.
 *
 * @attention The timeout periods of packets with the following three priorities can be modified: 0, 2, and 6. The size
 * of the sending queue whose priority is 0, 2, or 6 can be modified. After power-off, the modification of
 * the three priorities does not take effect.
 * @attention At most five priorities can be added. If a priority does not exist, the system automatically adds it. A
 * maximum of five priorities can be added. If more than five priorities are added, a failure message is
 * returned.
 * @attention The default packet timeout period of the new queue is 6000(unit:ms). You can call uapi_set_packet_timeout
 * to modify the timeout period.
 *
 * @param  lid             [IN] Type #td_u8 Packet priority. Value range: [0, 254]
 * @param  queue_size      [IN] Type #td_u8 Size of the sending queue with the specified priority. Value range: (0, 30].
 *
 * @retval #EXT_ERR_SUCCESS Success
 * @retval #EXT_ERR_FAILURE Failure
 * @retval Other values.      For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_set_tx_queue_size_by_lid(td_u8 lid, td_u8 queue_size);

/**
 * @ingroup network
 * @brief   Sets the size of the queue for receiving SOF frames.
 *
 * @par  Description:
 * Sets the size of the queue for receiving SOF frames.
 *
 * @attention None
 * @param  rx_queue_size [IN] Type #td_u8 Size of the receive queue. Value range: (0, 30]
 *
 * @retval #EXT_ERR_SUCCESS Success
 * @retval #EXT_ERR_FAILURE Failure
 * @retval Other values.      For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_set_rx_queue_size(td_u8 rx_queue_size);

/**
 * @ingroup network
 * Defines the FC receive information.
 */
typedef struct {
    td_u8 dt;    /**< Frame type */
    td_u8 freq;  /**< Frequency Band */
    td_u16 stei; /**< Source TEI address */

    td_u16 dtei;       /**< Destination TEI address */
    td_s8 snr;         /**< Average SNR */
    td_u8 attenuation; /**< Channel attenuation */

    td_u32 nid; /**< Network ID */

    td_u16 preamle_sat_num; /**< Preamble symbol saturation rate */
    td_u8 phase;
    td_u8 pad;
} ext_mdm_rcv_fc_info;

/**
 * @ingroup network
 * @brief  Obtains the registration function of the FC receiving information.
 *
 * @par  Description:
 * Obtains the registration function of the FC receiving information.
 *
 * @attention None
 * @param  rcv_fc_info [IN] Type #ext_mdm_rcv_fc_info* FC receive information
 *
 * @retval   None
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see ext_mdm_rcv_fc_info definition
 * @since DW21_V100R001C00
 */
typedef td_void (*mac_monitor_rcv_fc_info_fun)(EXT_CONST ext_mdm_rcv_fc_info * rcv_fc_info);

/**
 * @ingroup network
 * @brief Registers an API for obtaining FC receive information.
 *
 * @par  Description:
 * Registers an API for obtaining FC receive information.
 *
 * @attention None
 * @param  monitor_rcv_fc_info_if [IN] Type #mac_monitor_rcv_fc_info_fun for obtaining the FC receive information
 *
 * @retval #EXT_ERR_SUCCESS        Success
 * @retval #EXT_ERR_INVALID_PARAMETER   The delivered parameter is null.
 * @retval #EXT_ERR_FAILURE     The number of registered APIs reaches the upper limit (5).
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_register_monitor_rcv_fc_info_if(mac_monitor_rcv_fc_info_fun monitor_rcv_fc_info_if);

/**
 * @ingroup network
 * Defines MSDU information.
 */
typedef struct {
    td_u16 ostei; /**< Original source TEI address of the MSDU */
    td_u16 odtei; /**< Original destination TEI address of MSDU */
    td_u16 stei;  /**< Source TEI address of the MSDU */
    td_u16 dtei;  /**< Destination TEI address of the MSDU */
} ext_mac_msdu_info;

/**
 * @ingroup network
 * @brief  Obtains the registration function of MSDU information.
 *
 * @par  Description:
 * Obtains the registration function of MSDU information.
 *
 * @attention None
 * @param  msdu_info [IN] Type #ext_mac_msdu_info* MSDU information
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
typedef td_void (*mac_rcv_msdu_info_fun)(ext_mac_msdu_info *msdu_info);

/**
 * @ingroup network
 * @brief Registers the API for obtaining MSDU information.
 *
 * @par  Description:
 * Registers the API for obtaining MSDU information.
 *
 * @attention None
 * @param  rcv_msdu_info_if [IN] Type #mac_rcv_msdu_info_fun API for obtaining MSDU information
 *
 * @retval #EXT_ERR_SUCCESS  Success
 * @retval #EXT_ERR_INVALID_PARAMETER  The delivered parameter is null. A parameter error is returned.
 * @retval Other values.       For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_register_rcv_msdu_info_if(mac_rcv_msdu_info_fun rcv_msdu_info_if);

/**
 * @ingroup network
 * @brief Delivers the user-defined time for the MAC transmitter mechanism to stop running.
 *
 * @par  Description:
 * Delivers the user-defined time for the MAC transmitter mechanism to stop running.
 * The restrictions are as follows:
 * When the fast sending function is enabled and the immediate sending policy #EXT_MAC_FAST_SEND_PREEMPT is
 * enabled, this API can be called to stop the MAC transmitter.
 * This API cannot be used in other scenarios.
 *
 * @attention None
 * @param  disable_time [IN] Type #td_u32 Delivered time for stop the MAC transmitter (unit: ms). The valid range is
 * (0, 60000].
 *
 * @retval   ret Type #td_u32
 * @li #EXT_ERR_SUCCESS: The startup is successful.
 * @li #EXT_ERR_INVALID_PARAMETER: Invalid parameter.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_set_disable_sending(td_u32 disable_time);

/**
 * @ingroup network
 * @brief Enables the MAC transmitter.
 *
 * @par  Description:
 * Enables the MAC transmitter.
 *
 * @attention None
 * @param  None
 *
 * @retval   None
 *
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_set_enable_sending(td_void);

/**
 * @ingroup network
 * @brief Initializes the EMC function.
 *
 * @par  Description:
 * The EMC function includes static frequency avoidance, dynamic frequency avoidance, and dynamic power control.
 *
 * @attention None
 * @param  None
 *
 * @retval   None
 *
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_emc_init(td_void);

/**
 * @ingroup network
 * @brief Initializes the MAC module.
 *
 * @par  Description:
 * Initializes the MAC module.
 *
 * @attention This API is a MAC-layer resource initialization function and cannot be called multiple times.
 * @param  None
 *
 * @retval #EXT_ERR_SUCCESS  Success
 * @retval #EXT_ERR_FAILURE  Failure
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_mac_init(td_void);

/**
 * @ingroup network
 * @brief  TX parameter of the PLC channel
 */
typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN]; /**< Node MAC address */
    td_u8 product_type;             /**< Node product type */
    td_u8 mac_type;                 /**< Type of the node MAC address */
} ext_mdm_sta_auth_info;

/**
 * @ingroup network
 * @brief  Defines the multi-domain authentication API type that the application layer registers with the protocol stack
 *
 * @par  Description:
 * Defines the multi-domain authentication API type that the application layer registers with the protocol stack
 *
 * @attention This API is used by the STA.
 * @param  is_add_field_info [IN] Type #td_bool Flag indicating that the CCO carries domain information
 * @li #TD_TRUE : The CCO carries domain information.
 * @li #TD_FALSE : The CCO does not contain domain information.
 * @param  product_field [IN] Type #td_u32 CCO domain information
 *
 * @retval  ret Type #td_bool
 * @li #TD_TRUE : The domain information is consistent.
 * @li #TD_FALSE : The domain information is inconsistent.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
typedef td_bool (*sta_auth_cco_by_field_type_fun)(td_bool is_add_field_info, td_u32 product_field);

/**
 * @ingroup network
 * @brief Indicates the user-delivered API for registering domain type authentication.
 *
 * @par  Description:
 * Indicates the user-delivered API for registering domain type authentication.
 *
 * @attention This API is called by the STA.
 * @param  cb [IN] Type #sta_auth_cco_by_field_type_fun Registration callback function delivered by the user
 * @retval   ret Type #td_u32
 * @li #EXT_ERR_SUCCESS: Registration succeeded
 * @li #EXT_ERR_INVALID_PARAMETER: Invalid parameter.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_register_sta_auth_cco_callback(sta_auth_cco_by_field_type_fun cb);

/**
 * @ingroup network
 * @brief Sets the management ID.
 * @param  source_mangement_id [IN] Type #td_u8* Management ID information
 * @par  Description:
 * Set the management ID when the station is running properly. You need to set the management ID again after
 * the station is powered off.
 *
 * @attention If the CCO registers the callback function for managing ID authentication, you must call this function to
 * set the management ID of the station. Otherwise, the management ID authentication function may fail.
 * The management ID information carried by different STAs can be identified.
 * @attention CCO/ This API is called by the STA.
 * @attention If the management ID is not set, the default value is 0. The input parameter cannot be a null pointer.
 * @retval   ret Type #td_u32
 * @li #EXT_ERR_SUCCESS: success
 * @li #EXT_ERR_INVALID_PARAMETER: Invalid parameter.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_set_runtime_management_id(EXT_CONST td_u8 source_mangement_id[MANAGEMENT_ID_SIZE]);

/**
 * @ingroup network
 * @brief Obtains the set management ID when the station is running properly.
 * @param  runtime_management_id  [IN/ OUT] type #td_u8* : Used to save the read management ID address
 * @par  Description:
 * Obtains the set management ID when the station is running properly.
 *
 * @attention This API is used on the CCO/STA. runtime_management_id is the start address of a continuous 24-byte
 * memory. The caller is responsible for allocating the memory.
 * @retval   ret Type #td_u32
 * @li #EXT_ERR_SUCCESS: Obtaining succeeded.
 * @li #EXT_ERR_INVALID_PARAMETER: Parameter error.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_get_runtime_management_id(EXT_INOUT td_u8 runtime_management_id[MANAGEMENT_ID_SIZE]);

/**
 * @ingroup network
 * @brief Set the information about the domain.
 * @param  network_type [IN] Type #td_u32 Domain type
 * @par  Description:
 * This API is used to initialize domain information.
 *
 * @attention This API is used on the CCO/STA. The value 0 is considered as an invalid input parameter.
 * @retval   ret Type #td_u32
 * @li #EXT_ERR_SUCCESS: Setting succeeded.
 * @li #EXT_ERR_INVALID_PARAMETER: Parameter error.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_set_product_field_type(td_u32 network_type);

/**
 * @ingroup network
 * @brief Obtains information about the domain.
 *
 * @par  Description:
 * Obtains information about the domain.
 *
 * @attention This API is used on the CCO/STA.
 * @retval   ret Type #td_u32: domain type
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_get_product_field_type(td_void);

/**
 * @ingroup network
 * @brief Sets the domain authentication switch.
 *
 * @par  Description:
 * Sets the domain authentication switch.
 *
 * @attention This API is used on the CCO/STA.
 * @param  enable [IN] Type td_bool #TD_TRUE : enabled #TD_FALSE : disabled
 * @retval   ret Type #td_void
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_set_auth_field_type_switch(td_bool enable);

/**
 * @ingroup network
 * @brief  Defines the type of the interface that authenticates station information (excluding the management ID
 * information), which is registered by the application layer with the protocol stack.
 *
 * @par  Description:
 * Defines the type of the interface that authenticates station information (excluding the management ID
 * information), which is registered by the application layer with the protocol stack.
 *
 * @attention None
 * @param  sta_info [IN] Type #ext_mdm_sta_auth_info: Station information
 * @retval  ret Type #td_bool
 * @li #TD_TRUE : Management ID authentication succeeded.
 * @li #TD_FALSE : Failed to authenticate the management ID.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
typedef td_bool (*ext_mdm_sta_auth_func)(ext_mdm_sta_auth_info *sta_info);

/**
 * @ingroup network
 * @brief  Defines the authentication function for station management ID information, which is registered by the
 * application layer with the protocol stack.
 *
 * @par  Description:
 * Defines the authentication function for station management ID information, which is registered by the
 * application layer with the protocol stack.
 *
 * @attention This callback function is called only by the State Grid protocol. Other protocols do not call this
 * function.
 * @param  sta_id [IN] Type #td_u8* Station management ID
 * @param  id_len [IN] Type #td_u8* Length of station management ID
 *
 * @retval  ret Type #td_bool
 * @li #TD_TRUE : Management ID authentication succeeded.
 * @li #TD_FALSE : Failed to authenticate the management ID.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
typedef td_bool (*ext_mdm_cco_auth_sta_id_func)(td_u8 sta_id[MANAGEMENT_ID_SIZE], td_u8 id_len);

/**
 * @ingroup network
 * @brief Allows users to register STA authentication information at the CCO end.
 *
 * @par  Description:
 * Allows users to register STA authentication information at the CCO end.
 *
 * @attention This API is called by the CCO. The registration interface does not distinguish the protocol version, but
 * the registered callback function is used only in the State Grid protocol instead of in other protocols.
 * @param  cb [IN] Type #ext_mdm_cco_auth_sta_id_func Registration callback function delivered by the user
 * @retval   ret Type #td_u32
 * @li #EXT_ERR_SUCCESS: Registration succeeded
 * @li #EXT_ERR_INVALID_PARAMETER: Invalid parameter.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_register_cco_auth_management_id_callback(ext_mdm_cco_auth_sta_id_func cb);

/**
 * @ingroup network
 * @brief   Registers the authentication MAC address callback function.
 *
 * @par  Description:
 * Registers the authentication MAC address callback function.
 *
 * @attention
 * @li The product type is CCO.
 * @li To enable this function, call uapi_set_white_list_switch() to disable the MDM whitelist authentication
 * function.
 * @param  auth_func [IN] Type #ext_mdm_sta_auth_func Authentication MAC address callback function. If the value is
 * #TD_NULL, it indicates that the registration is canceled.
 * @retval None
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_register_sta_auth_callback(ext_mdm_sta_auth_func auth_func);

/**
 * @ingroup  network
 * @brief Obtains the number of online stations on the current network (excluding the CCO).
 * @par  Description:
 * Obtains the number of online stations on the current network (excluding the CCO).
 * @attention The product type is CCO.
 * @retval   ret Type #td_u16 Number of stations
 * @par Dependency:
 * < ul>< li>soc_mdm_mac.h: Describes the external interfaces of the link layer. </ li></ ul>
 * @see None
 * @since DW21_V100R001C00
 */
td_u16 uapi_get_current_online_sta_num(td_void);

/**
 * @ingroup network
 * Obtains the structure of the whitelist request information.
 */
typedef struct {
    /*
     * Maximun number of white list items reported from MDM to APP upon one request. Report only one
     * white list item if item_num equals to 0;
     */
    td_u16 item_num;
    td_u16 reserved;
} ext_mdm_nm_white_list_qry_req;

/**
 * @ingroup network
 * Whitelist item
 */
typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN]; /**< Station MAC address */
    td_u16 reserved;
} ext_mdm_nm_white_list_item;

/**
 * @ingroup network
 * Sets the whitelist information structure.
 */
typedef struct {
    td_u16 total_num;                   /**< Total number of whitelists */
    td_u16 item_num;                    /**< Count of valid stItem */
    ext_mdm_nm_white_list_item items[0]; /**< Whitelist item */
} ext_mdm_nm_white_list_qry_ind;

/**
 * @ingroup network
 * Defines whether to enable or disable the function.
 */
#define EXT_MDM_ENABLE 1  /**< Enable */
#define EXT_MDM_DISABLE 0 /**< Disable */

/**
 * @ingroup  network
 * @brief  Sets the beacon duration.
 *
 * @par   Description:
 * Sets the beacon duration.
 *
 * @attention The product type is CCO.
 * @param  bcn_duration [IN] Type #td_u8 Beacon duration (unit: ms). The valid range is [15, 30].
 *
 * @retval ret         [IN] Type #td_u32
 * @li EXT_ERR_SUCCESS: success
 * @li EXT_ERR_FAILURE: failure
 * @li For details about the return values, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_set_beacon_duration(td_u8 bcn_duration);

/**
 * @ingroup  network
 * @brief  Sets the multi-network coordination switch.
 *
 * @par   Description:
 * Sets the multi-network coordination switch.
 *
 * @attention This API is called by the CCO.
 * @param  multinetwork_coordinate_switch [IN] Type #td_bool
 * @li TD_TRUE: Enabled.
 * @li TD_FALSE: Disabled.
 *
 *
 * @retval ret [IN] Type #td_u32
 * @li EXT_ERR_SUCCESS Success
 * @li EXT_ERR_FAILURE Failure
 * @li For details about the return values, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_set_multi_network_coordinate_switch(td_bool multinetwork_coordinate_switch);

/**
 * @ingroup  network
 * @brief  Obtains the multi-network coordination switch.
 *
 * @par   Description:
 * Obtains the multi-network coordination switch.
 *
 * @attention This API is called by the CCO.
 * @retval ret [IN] Type #td_bool
 * @li TD_TRUE Multi-network coordination is enabled.
 * @li TD_FALSE Multi-network coordination is disabled.
 * @li For details about the return values, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_bool uapi_get_multi_network_coordinate_switch(td_void);

/**
 * @ingroup  network
 * @brief  Configures the multi-network monitoring time.
 *
 * @par   Description:
 * Configures the multi-network monitoring time.
 * @param  multinetwork_listen_time [IN] Type #td_u16 unit: ms
 * @attention This API is called by the CCO. If the input parameter value is less than 100, the default value 100 is
 * used.
 * @retval ret  [IN] Type #td_u16
 * @li EXT_ERR_SUCCESS: Success
 * @li EXT_ERR_FAILURE: Failure
 * @li For details about the return values, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_set_multi_network_listen_time(td_u16 multinetwork_listen_time);

/**
 * @ingroup  network
 * @brief  Configures the network priority function switch.
 *
 * @par   Description:
 * Configures the network priority function switch.
 * @param  network_evaluation_switch [IN] Type #td_bool
 * @li TD_TRUE: Enabled.
 * @li TD_FALSE: Disabled.
 *
 * @attention This API is called by the STA.
 * @retval ret                          [IN] Type #td_u32
 * @li EXT_ERR_SUCCESS: Success
 * @li EXT_ERR_FAILURE: Failure
 * @li For details about the return values, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_set_network_evaluation_switch(td_bool network_evaluation_switch);

/**
 * @ingroup  network
 * @brief   Checks whether packets can be sent.
 *
 * @par   Description:
 * Checks whether packets can be sent.
 *
 * @attention None
 * @param  macbuffer_lid [IN] Type #td_u8 Serial number of the software queue
 * @retval ret           [IN] Type #td_bool
 * @li TD_TRUE: Packets can be sent.
 * @li TD_FALSE: Packets cannot be sent.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_bool uapi_query_mac_send_status(td_u8 macbuffer_lid);

/**
 * @ingroup  network
 * @brief  Sets whether to enable the silence state.
 *
 * @par   Description:
 * Sets whether to enable the silence state.
 *
 * @attention None
 * @param  status         [IN] Type #td_u8
 * @li EXT_MDM_ENABLE(1): Enables the silence state.
 * @li EXT_MDM_DISABLE(0): Disables the silence state.
 *
 * @retval ret Type #td_u32
 * @li EXT_ERR_SUCCESS: success
 * @li EXT_ERR_INVALID_PARAMETER: The input parameter is invalid.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_set_send_silence_status(td_u8 status);

/**
 * @ingroup network
 * @brief Refreshes the time at which the CCO packet is received.
 *
 * @par  Description:
 * Refreshes the time at which the CCO packet is received.
 *
 * @attention This API can be called only on the STA.
 * @param  None
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_refresh_rcv_cco_pkt_time(td_void);

/**
 * @ingroup network
 * @brief Queries the channel attenuation of a specified TEI.
 *
 * @par  Description:
 * Queries the channel attenuation of a specified TEI.
 *
 * @attention The caller must ensure the validity of the input TEI.
 *
 * @param  tei [IN] Type #td_u16: Queries the TEI of a station.
 *
 * @retval ret Type #td_u8
 * @li 127: Invalid attenuation value. The channel attenuation of a station is not obtained or the input
 * parameter is invalid.
 * @li Other values: Indicates the valid attenuation value.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u8 uapi_get_rcv_atten_by_tei(td_u16 tei);

/**
 * @ingroup network
 * @brief Queries the SNR of a specified TEI.
 *
 * @par  Description:
 * Queries the SNR of a specified TEI.
 *
 * @attention The caller must ensure the validity of the input TEI.
 *
 * @param  tei [IN] Type #td_u16: Queries the TEI of a station.
 *
 * @retval ret Type #td_u8
 * @li 127: Invalid SNR value. The station SNR is not obtained or the input parameter is invalid.
 * @li Other values: Indicates the valid SNR value.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u8 uapi_get_snr_by_tei(td_u16 tei);

/**
 * @ingroup network
 * @brief  Initializes the station logic phase identification function.
 *
 * @par  Description:
 * Initializes the station logic phase identification function.
 *
 * @attention This API is called when the app is initialized and after uapi_mac_init is called.
 * @param  None
 *
 * @retval   None
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_phase_evaluate_enable(td_void);

/**
 * @ingroup network
 * @brief  Initializes the NTB synchronization function.
 *
 * @par  Description:
 * Initializes the NTB synchronization function.
 *
 * @attention None
 * @param  None
 *
 * @retval   None
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_ntb_init(td_void);

/**
 * @ingroup network
 * @brief Load IEEE1901.1 protocal and SG protocol during startup
 *
 * @par  Description:
 * Load IEEE1901.1 protocal and SG protocol during startup
 *
 * @attention None
 * @param  None
 *
 * @retval  None
 *
 * @par Dependency:
 * @li soc_mdm_mac.h : Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_load_sg_and_1901_protocol(td_void);
/**
 * @ingroup network
 * @brief Load IEEE1901.1 protocal during startup
 *
 * @par  Description:
 * Load IEEE1901.1 protocal during startup
 *
 * @attention None
 * @param  None
 *
 * @retval  None
 *
 * @par Dependency:
 * @li soc_mdm_mac.h : Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_load_1901_protocol(td_void);
/**
 * @ingroup network
 * size of received bitmap
 */
#define POWER_FAILURE_BITMAP_LEN ((EXT_MDM_NM_TOPO_NODE_NUM_MAX / 8) + 1)

/**
 * @ingroup  network
 * @brief    Initializes the protocol stack component.
 *
 * @par  Description:
 * Initializes the protocol stack component.
 *
 * @attention
 * @li This API can be called only once.
 * @param  None
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs for phase identification.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_mac_cp_init(td_void);

/**
 * @ingroup network
 * @brief Enable communication between STAs feature.
 *
 * @par   Description:
 * Enable communication between STAs feature.
 *
 * @attention
 * @li CCO/STA
 *
 * @param is_stas_commu [IN] type #td_bool, input parameter indicates whether feature is enabled.
 * @li TD_TRUE :enable
 * @li TD_FALSE:disable
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_set_stas_commu(td_bool is_stas_commu);

/**
 * @ingroup network
 * @brief Set the communication alert threshold.
 *
 * @par   Description:
 * Set the communication alert threshold.
 *
 * @attention The product type is CCO or STA.Channel attenuation parameter setting is not allowed to be less than 80db,
 * Average SNR parameter setting is not allowed to be less than 1,communication rate parameter setting
 * is not allowed to be less than 10.
 * If the parameter settings are incorrect, execute default parameter (80,1,10).
 *
 * @param chl_atenuation [IN] type #td_u8: Set the Minimum Threshold of Channel Attenuation.The lower threshold is 80.
 *
 * @param snr [IN] type #td_s8: Set the Minimum Threshold of Average SNR.The lower threshold is 1.
 *
 * @param comm_rate [IN] type #td_u8: Set the Minimum Threshold of communication rate.The lower threshold is 10.
 *
 * @retval ret Type #td_u32
 * @li EXT_ERR_SUCCESS: success
 * @li EXT_ERR_INVALID_PARAMETER: The input parameter is invalid.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_set_comm_lower_limit(td_u8 chl_atenuation, td_s8 snr, td_u8 comm_rate);

/**
 * @ingroup network
 * @brief  Defines the abnormal event notify type.
 */
typedef enum {
    ABNORMAL_SNAPSHOT = 0x00,   /**< The value of abnormal_type is 0. This event is reported when a STA that
    is offline due to an exception,joins the network again.The possible causes are as follows: #CHOSE_PROXY_FAIL,
    #RCV_BEACON_FAILED, #PCO_MAX_LEVEL */
    ABNORMAL_SNR_EVENT = 0x01,   /**< If bit[0] is 1, it indicates that average snr is abnormal. */
    ABNORMAL_CHL_ATTN_EVENT = 0x02,   /**< If bit[1] is 1, it indicates that channel attenuation is abnormal. */
    ABNORMAL_COMMU_RATE_EVENT = 0x04,  /**< If bit[2] is 1, it indicates that communication rate is abnormal. */
} ext_mdm_abnormal_notify_type;

/**
 * @ingroup network
 * Defines the Abnormal Data information.
 */
typedef struct {
    td_u8 abnormal_type;  /**< Abnormal  Dimension Type */
    td_u8 chl_atenuation; /**< Channel Attenuation */
    td_s8 snr;            /**< Average SNR */
    td_u8 comm_rate;

    td_u16 tei;       /**< Abnormal Node TEI */
    td_u16 proxy_tei; /**< Abnormal Node's Proxy TEI */

    td_bool is_online;
    td_u8 offline_reason; /**< Offline reason,refer to the structure of ext_mdm_mac_rejoin_reason */
    td_u8 pad[2];         /**< Reserved 2 bytes */

    td_u8 abnormal_mac[EXT_PLC_MAC_ADDR_LEN];       /**< Abnormal Node MAC address */
    td_u8 abnormal_proxy_mac[EXT_PLC_MAC_ADDR_LEN]; /**< Abnormal Node Proxy MAC address */
} ext_report_abnormal_data;

/**
 * @ingroup network
 * @brief Indicates the callback function type for reporting abnormal communication event.
 *
 * @par  Description:
 * Indicates the callback function type for reporting abnormal communication event.
 *
 * @attention None
 * @param  data [IN] Type #ext_report_abnormal_data Report of the data to the user.
 *
 * @retval   None
 *
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
typedef td_void (*ext_mdm_abnormal_notify_callback)(ext_report_abnormal_data *data);

/**
 * @ingroup network
 * @brief Register the callback function for reporting abnormal communication event.
 *
 * @par  Description:
 * Register the callback function for reporting abnormal communication event.
 *
 * @attention The product type is CCO or STA.
 * @param  cb [IN] Type #ext_mdm_abnormal_notify_callback Registration callback function delivered by the user
 *
 * @retval   ret Type #td_u32
 * @li #EXT_ERR_SUCCESS: The startup is successful.
 * @li #EXT_ERR_INVALID_PARAMETER: Invalid parameter.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_register_abnormal_comm_event_report(ext_mdm_abnormal_notify_callback cb);

/**
 * @ingroup network
 * @brief  Defines the packet parsing status.
 */
typedef enum {
    PBB_CORRECT_TYPE, /**< The received PB is correct. */
    PBB_ERROR_TYPE,   /**< Failed to receive the PB. */
    PBB_NOTHING_TYPE, /**< Only the FC has no PB. */
    PBB_MAX_TYPE
} ext_mdm_capture_packet_pbb_type;

/**
 * @ingroup network
 * @brief  The user delivers the interface for registering the callback function for capturing packets.
 *
 * @par  Description:
 * The user delivers the interface for registering the callback function for capturing packets.
 *
 * @attention None
 * @param  state [IN] Type ext_mdm_capture_packet_pbb_type.
 * @param  fc [IN] Type #td_u8*,For storing the report FC, the space pointed by the pointer is not less than 16 bytes.
 * @param  payload [IN] Type #td_u8*, For storing the report payload.
 * @param  pb_size [IN] Type #td_u16, paylaod size.
 *
 * @retval ret Type #td_u32
 * @li EXT_ERR_SUCCESS: success
 * @li For details about the return values, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
typedef td_u32 (*ext_mdm_capture_fun)(ext_mdm_capture_packet_pbb_type state, EXT_CONST td_u8 *fc,
    EXT_CONST td_u8 *payload, td_u16 pb_size);

/**
 * @ingroup network
 * @brief  Register the callback function for capturing packets.
 *
 * @par   Description:
 * Register the callback function for capturing packets.
 *
 * @attention None.
 * @param fun [IN] Type #ext_mdm_capture_fun, Register the callback function.
 *
 * @retval None
 *
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_register_capture_packet_function(ext_mdm_capture_fun fun);

/**
 * @ingroup network
 * @brief  No packet capture mode.
 */
#define EXT_MAC_NO_CAPTURE_MODE 0
/**
 * @ingroup network
 * @brief  Packet capture mode of a single payload
 */
#define EXT_MAC_CAPTURE_SINGLE_PB_MODE 1
/**
 * @ingroup network
 * @brief  Single payload Loop Mode
 */
#define EXT_MAC_INTERCEPT_LOOP_MODE 2
/**
 * @ingroup network
 * @brief  Single payload interception mode
 */
#define EXT_MAC_INTERCEPT_SINGLE_PB_MODE 3

/**
 * @ingroup network
 * @brief Set the packet capture mode.
 *
 * @par  Description:
 * Set the packet capture mode.
 *
 * @attention None
 * @param  mode [IN] Type #td_u8
 * @li #EXT_MAC_NO_CAPTURE_MODE: Indicates that no packet capture mode is used.
 * @li #EXT_MAC_CAPTURE_SINGLE_PB_MODE: Indicates the packet capture mode of a single Payload.
 * @li #EXT_MAC_INTERCEPT_LOOP_MODE: Indicates the loopback test mode of a single Payload.
 * @li #EXT_MAC_INTERCEPT_SINGLE_PB_MODE: Indicates the interception mode of a single Payload.
 *
 * @retval   ret Type #td_u32
 * @li #EXT_ERR_SUCCESS: The startup is successful.
 * @li #EXT_ERR_FAILURE: Invalid parameter.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_mdm_set_capture_mode(td_u8 mode);

/**
 * @ingroup network
 * @brief Obtains the packet capture mode.
 *
 * @par  Description:
 * Obtains the packet capture mode.
 *
 * @attention None.
 * @param  None.
 *
 * @retval   ret Type #td_u8
 * @li #EXT_MAC_NO_CAPTURE_MODE: Indicates that no packet capture mode is used.
 * @li #EXT_MAC_CAPTURE_SINGLE_PB_MODE: Indicates the packet capture mode of a single Payload.
 * @li #EXT_MAC_INTERCEPT_LOOP_MODE: Indicates the loopback test mode of a single Payload.
 * @li #EXT_MAC_INTERCEPT_SINGLE_PB_MODE: Indicates the interception mode of a single Payload.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u8 uapi_mdm_get_capture_mode(td_void);

typedef enum {
    PHYSICAL_TEST_MODE_NONE = 0,
    PHYSICAL_TEST_MODE_APP_TO_COM = 1,      /**< Forwards the application layer packets to the application layer
                                             * serial channel test mode. */
    PHYSICAL_TEST_MODE_APP_TO_PLC = 2,      /**< Forwards the application layer to the carrier channel test mode */
    PHYSICAL_TEST_MODE_APP_TO_COM_LOOP = 3, /**< Enter the physical layer transparent transmission test mode to the
                                             * serial port channel,and exit until the "test mode duration" */
    PHYSICAL_TEST_MODE_APP_TO_PLC_LOOP = 4, /**< Enter the physical layer return test mode, automatically reply to the
                                             * carrier channel, and exit the mode until the "test mode duration" */
    PHYSICAL_TEST_MODE_EXIT = 5,            /**< Enter the MAC layer transparent transmission test mode, transparently
                                             * transmit the MSDU of the received paceket to the serial channel
                                             * and exit the test mode until the "test mode duration"  */
    PHYSICAL_TEST_MODE_SET_FREQ = 6,        /**< Perform band switching operation */
    PHYSICAL_TEST_MODE_SET_TONE_MASK_MODE = 7, /**< Perform the ToneMask configuration operation */
} ext_mdm_physical_test_mode;

/**
 * @ingroup network
 * @brief  Test mode callback function definition .
 *
 * @par  Description:
 *         Test mode callback function definition .
 *
 * @attention None
 * @param  test_duration [IN] Type # td_u16  Testing Times
 * @param  pre_mode [IN] Type #ext_mdm_physical_test_mode.
 *
 * @retval None.
 * @par Dependency:
 *            @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
typedef td_void (*ext_mdm_set_test_mode_callback)(td_u16 test_duration, td_u8 pre_mode);

/**
 * @ingroup network
 * @brief  Register interface functions of different test modes .
 *
 * @par  Description:
 *         Register interface functions of different test modes .
 *
 * @attention None
 * @param  mode_type [IN] Type #ext_mdm_physical_test_mode.
 * @param  fun [IN] Type # ext_mdm_set_test_mode_callback
 *
 * @retval None.
 * @par Dependency:
 *            @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_mdm_register_test_mode(ext_mdm_physical_test_mode mode_type, ext_mdm_set_test_mode_callback fun);

/**
 * @ingroup network
 * @brief Setting the Physical Layer Test Mode.
 *
 * @par  Description:
 * Setting the Physical Layer Test Mode.
 * @attention Execute #uapi_mdm_exit_phy_test_mode when exiting test mode.
 * @param  mode  [IN] Type #ext_mdm_physical_test_mode
 * @li #PHYSICAL_TEST_MODE_APP_TO_COM_LOOP: refer to the structure of ext_mdm_physical_test_mode.
 * @li #PHYSICAL_TEST_MODE_APP_TO_PLC_LOOP: refer to the structure of ext_mdm_physical_test_mode.
 * @li #PHYSICAL_TEST_MODE_EXIT: refer to the structure of ext_mdm_physical_test_mode.
 *
 * @retval ret #td_u32,
 * @li #EXT_ERR_SUCCESS: Enter the physical layer test state success.
 * @li #EXT_ERR_NOT_SUPPORT: Don't need to enter the physical layer test state for this mode.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_mdm_enter_phy_test_mode(ext_mdm_physical_test_mode mode);

/**
 * @ingroup network
 * @brief Exit the physical layer test mode.
 *
 * @par  Description:
 * Exit the physical layer test mode.
 * @attention When this API is called in test mode, it automatically reboot
 *   and The cause of reset is #EXT_SYS_REBOOT_CAUSE_USR4_EXIT_PHY_TEST.
 * @param  None.
 * @retval None.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_mdm_exit_phy_test_mode(td_void);

/**
 * @ingroup network
 * @brief  To obtain the physical layer test mode
 *
 * @par  Description:
 * To obtain the physical layer test mode
 * @attention None.
 * @param  None.
 * @retval ret Type #td_u8,the physical layer test mode,refer to the structure of ext_mdm_physical_test_mode.
 *
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u8 uapi_mdm_get_phy_test_mode(td_void);

/**
 * @ingroup network
 * @brief  Capturing MSDU Content Information.
 */
typedef struct {
    td_u16 protocol_ver; /**< MSDU protocol type */
    td_u16 msdu_size;    /**< MSDU length */

    td_u8 *msdu_data; /**< MSDU Content cache address */
} ext_capture_msdu_info;

/**
 * @ingroup network
 * @brief  Capturing MSDU Content Registration Functions.
 *
 * @par  Description:
 * Capturing MSDU Content Registration Functions.
 *
 * @attention None
 * @param  data [IN] Type #ext_capture_msdu_info.
 *
 * @retval None.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
typedef td_void (*ext_mac_capture_msdu_fun)(EXT_CONST ext_capture_msdu_info *data);

/**
 * @ingroup network
 * @brief  Register the interface function for capturing MSDU content.
 *
 * @par   Description:
 * Register the interface function for capturing MSDU content.
 *
 * @attention None.
 * @param fun [IN] Type #ext_mac_capture_msdu_fun,Register the callback function.
 *
 * @retval None.
 *
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_register_capture_msdu_function(ext_mac_capture_msdu_fun fun);

/**
 * @ingroup network
 * @brief  Obtains the MSDU packet interception switch.
 *
 * @par   Description:
 * Obtains the MSDU packet interception switch.
 *
 * @attention None.
 * @param None.
 *
 * @retval ret Type #td_bool
 * @li TD_TRUE: Intercept MSDU packets.
 * @li TD_FALSE: Allow MSDU packets to be sent.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_bool uapi_get_intercept_msdu_enable(td_void);

/**
 * @ingroup network
 * @brief  This command is used to enable or disable the function of intercepting MSDU packets.
 *
 * @par   Description:
 * This command is used to enable or disable the function of intercepting MSDU packets.
 *
 * @attention None.
 * @param is_intercept_msdu [IN] Type #td_bool
 * @li TD_TRUE: Set Intercept MSDU packets.
 * @li TD_FALSE: Allow MSDU packets to be sent.
 * @retval None.
 *
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_set_intercept_msdu_enable(td_bool is_intercept_msdu);

/**
 * @ingroup network
 * @brief Set the working frequency band.
 *
 * @par  Description:
 * Set the working frequency band.Currently, only the 2.5-5.7 frequency band of the 1901 protocol is supported.
 *
 * @attention This parameter can be used only in the test system. If the frequency band is set, the nv value will
 * not be changed, and the restart will become invalid.
 * @param  freq  [IN] Type #td_u8,if this parameter is set to 1, the frequency band is 2.5-5.7m.
 *
 * @retval   ret Type #td_u8
 * @li #EXT_ERR_SUCCESS: The startup is successful.
 *
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_mdm_set_work_freq(td_u8 freq);
/**
 * @ingroup network
 * @brief  the type of querying app's version and name callback function.
 *
 * @par  Description:
 * the type of querying app's version and name callback function.
 *
 * @attention None
 * @param  ver_info [OUT] Type #td_pbyte, query data.
 * @param  ver_info_size [IN] Type #td_u16, size of parameter ver_info.
 *
 * @retval td_u32.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
typedef td_u32 (*ext_mdm_query_app_ver)(EXT_OUT td_pbyte ver_info, EXT_IN EXT_CONST td_u16 ver_info_size);

/**
 * @ingroup network
 * @brief Register querying app's version and  name callback function.
 * @par  Description:
 * Register querying app's version and  name callback function.
 * @param  cb  [IN] Type #ext_mdm_query_app_ver, callback function name.
 * @retval   ret Type #td_u32
 * @li #EXT_ERR_SUCCESS.
 * @li #EXT_ERR_INVALID_PARAMETER: FAIL.cb is TD_NULL.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_diag_register_query_app_ver_cmd_callback(ext_mdm_query_app_ver cb);


/**
 * @ingroup network
 * @brief Callback function of sta module soft restart reporting app.
 * @par  Description:
 * Callback function of sta module soft restart reporting app.
 * @attention The product type is CCO or STA.
 * @param  rejoin_reason [IN] Type #td_u8,refer to the structure of ext_mdm_mac_rejoin_reason.
 * @retval ret Type #td_u32
 * @li EXT_ERR_SUCCESS: success
 * @li For details about the return values, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
typedef td_u32 (*ext_mdm_pf_rejoin_event_callback)(td_u8 rejoin_reason);

/**
 * @ingroup network
 * @brief Register power failure report rejoin event interface.
 * @par  Description:
 * Register power failure report rejoin event interface.
 * @attention The product type is CCO or STA.
 * @param  fun [IN] Type #ext_mdm_pf_rejoin_event_callback, Register the callback function.
 * @retval None.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_register_power_failure_rejoin_event_report(ext_mdm_pf_rejoin_event_callback fun);

/**
 * @ingroup network
 * @brief Callback function for monitoring result reporting.
 * @par  Description:
 * Callback function for monitoring result reporting.
 * @attention The product type is CCO.
 * @param  nid_addr [IN] Type #td_u32, monitored NID .
 * @param  result [IN] Type # td_u8 , monitored result, 0:success, 1:failure, Monitoring NID is invalid when
 * the result is a failure
 * @retval None.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
typedef td_void (*ext_mdm_monitor_nid_result_callback)(td_u32 nid, td_u8 result);

/**
 * @ingroup network
 * @brief Registration monitoring NID result reporting interface .
 * @par  Description:
 * Registration monitoring NID result reporting interface .
 * @attention The product type is CCO.
 * @param  fun [IN] Type #ext_mdm_monitor_nid_result_callback, Register the callback function.
 * @retval None.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_register_monitor_nid_report(ext_mdm_monitor_nid_result_callback fun);

typedef enum {
    MONITOR_SETING_SUCCESS = 0, /* Setting monitoring succeeded */
    MONITOR_TIMEOUT_INVALID,    /* Invalid setting monitoring time */
    MONITOR_WITHER_LISH_EMPTY,  /* White list is empty */
} ext_mdm_set_monitor_result_type;


/**
 * @ingroup network
 * @brief Setting the interface function of CCO monitoring access mode .
 * @par  Description:
 * Setting the interface function of CCO monitoring access mode .
 * @attention The product type is CCO,CCO will restart after setting successfully .
 * @param  timeout [IN] Type #td_u16, Monitoring timeout, Unit is second.
 * @param  is_white [IN] Type #is_white
 * @li TD_TRUE:Whitelist comparison monitoring
 * @li TD_FALSE:No white list to monitor whether the neighbor network NID conflicts with the current NID .
 * @retval ret type [IN] Type #ext_mdm_set_monitor_result_type.
 *
 * @par Dependency: soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
ext_mdm_set_monitor_result_type uapi_mdm_set_monitor_mode(td_u16 timeout, td_bool is_white);

/**
 * @ingroup network
 * @brief Setting up the interface of mesh network locking NID .
 * @par  Description:
 * Setting up the interface of mesh network locking NID .
 * @attention The product type is STA.
 * @param  lock_flag [IN] Type #td_bool
 * @li TD_TRUE:Mesh network lock NID
 * @li TD_FALSE:The mesh network does not lock the NID and clears the previously saved NID .
 * @retval ret type [IN] Type #td_u32.
 * @li EXT_ERR_SUCCESS: success
 * @li For details about the return values, see soc_errno.h.
 * @par Dependency: soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_mdm_set_mesh_lock_nid(td_bool lock_flag);

/**
* @ingroup network
* Maximum number of MAC addresses of sites that are rejected
*/
#define EXT_REJECT_JOIN_NET_STA_NUM_MAX     16

/**
 * @ingroup network
 * Parameters for querying the MAC address list of the sites that are denied access
 */
typedef struct {
    td_u8  mac_num;               /**< Number of rejected MAC addresses >*/
    td_u8  pad[3];                /**< 3:Reserved >*/
    td_u8  mac[EXT_REJECT_JOIN_NET_STA_NUM_MAX][EXT_PLC_MAC_ADDR_LEN];   /**< List of MAC addresses of sites
                                                                            that are denied access */
} ext_mac_reject_list_info;
/**
* @ingroup network
* @brief Interface for Querying the MAC Address List of the Sites That Are Rejected to Access the Network
* @par Description: Interface for Querying the MAC Address List of the Sites That Are Rejected to Access the Network.
* @attention The product type is CCO.
* @param mac_list [IN]/[OUT] type #ext_mac_reject_list_info *, MAC address entry rejection list
* @param input_size [IN] type #td_u16, The memory space allocated to mac_list must be greater than or equal to the
*   size of #ext_mac_reject_list_info.
* @retval   ret type #td_u32:
*            @li EXT_ERR_SUCCESS: success
*            @li For details about the return values, see soc_errno.h.
* @par Dependency:
*           @li soc_mdm_mac.h: Describes the external APIs of the link layer.
* @see None
* @since DW21_V100R001C00
*/
td_u32 uapi_mdm_mac_get_reject_list(ext_mac_reject_list_info *mac_list, td_u16 input_size);

/**
* @ingroup network
* @brief Obtains the number of sites in the discovery list.
* @par  Description:
*  Obtains the number of sites in the discovery list.The interface can be used together with
* #uapi_mdm_query_discovery_list.After obtaining the number of sites in the discovery list, query
* the site information in the discovery list through the interface.
* @retval ret type #td_u16: number of sites in the discovery list.
* @par Dependency:
*           @li soc_mdm_mac.h: Describes the external APIs of the link layer.
* @see None
* @since DW21_V100R001C00
*/
td_u16 uapi_mdm_get_sta_num_in_discover_list(td_void);

/**
 * @ingroup network
 * Information about a single site in the discovery list
 */
typedef struct {
    td_u16 tei; /**< tei of this STA */
    td_u16 proxy_tei; /**< proxy tei of this STA */

    td_u8 atten; /**< average attenuation of received packets from this STA  */
    td_s8 snr; /**< average SNR of received packets from this STA */
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN]; /**< MAC address of this STA */

    td_u8 down_comm_rate; /**< downlink communication success rate of this STA */
    td_u8 up_comm_rate; /**< Uplink  communication success rate of this STA */
    td_u8 role; /**< role of this STA */
    td_u8 level; /**< level of this STA */
} ext_mdm_discover_list_station_info;
/**
* @ingroup  network
* @brief  Querying the Site Discovery List
* @par  Description:
*  Querying the Site Discovery List.The interface can be used together with #uapi_mdm_get_sta_num_in_discover_list.
* @param discover_stas [OUT] type #ext_mdm_discover_list_station_info *, Information about multiple sites in the
*   discovery list. The memory used by discover_stas is applied for and released by the app.
* @param input_size [IN] type #td_u16, actual length of the memory pointing to discover_stas.
* @retval   ret type #td_u32:
*            @li EXT_ERR_SUCCESS: success
*            @li For details about the return values, see soc_errno.h.
* @par Dependency:
*           @li soc_mdm_mac.h: Describes the external APIs of the link layer.
* @see None
* @since DW21_V100R001C00
*/
td_u32 uapi_mdm_query_discovery_list(ext_mdm_discover_list_station_info *discover_stas, td_u16 data_length);
/**
 * @ingroup network
 * @brief Register mesh routing components.
 * @par  Description:
 *       Register mesh routing components.
 * @attention The product type is STA.
 * @param  None.
 * @retval None.
 * @par Dependency:
 *           @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_mdm_mesh_rt_component(td_void);

/**
 * @ingroup network
 * @brief Enabling the smart configuration network.
 * @par  Description: Enabling the smart configuration network.
 * @attention The product type is STA. This function can be disabled at any time. However, enable this function only
 * @li after the system is powered on and initialized or when sta has joined the CCO network.
 * @li If this parameter is set to ON,
 * @param  is_on  [IN] type #td_bool, TD_TRUE is ON, TD_FALSE is OFF.The setting takes effect in real time
 * @li and will be stored in the flash memory.
 * @retval ret type #td_u32:.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_mdm_set_smart_select_open_state(td_bool is_on);

/**
 * @ingroup network
 * @brief To get the Smart Select Network state.
 * @par  Description:
 * To get the Smart Select Network state.
 * @attention The product type is STA.
 * @param  None.
 * @retval ret type #td_bool: TD_TRUE,The smart select is enabled;
 * @li TD_FALSE: The smart select is disabled.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_bool uapi_mdm_get_smart_select_open_state(td_void);

/**
 * @ingroup network
 * @brief Initializing the Smart Select Network Module.
 * @par  Description: STA supports smart configuration after initialization. After this function is enabled,
 * STAs may be associated with the network selectively.
 * @attention The product type is STA.
 * @retval   ret type #td_u32: #EXT_ERR_SUCCESS: success
 * @li For details about the return values, see soc_errno.h.
 * @par Dependency:
 * @li  soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_mdm_smart_select_net_register(td_void);
/**
 * @ingroup network
 * @brief Set Upgrade Use Binding CSMA Width
 * @par  Description: Set Upgrade Use Binding CSMA Width. This parameter is invalid after a power failure.
 * @attention The product type is CCO.
 * @param  width_ms  [IN] type #td_u16, Binding CSMA Width, Range [500,6000],unit: ms
 * @retval   ret type #td_u32: #EXT_ERR_SUCCESS: success
 * @li For details about the return values, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_mdm_set_bind_csma_width(td_u16 width_ms);
/**
 * @ingroup network
 * @brief Load and init lock network components by SNR
 * @par  Description: Load and init lock network components by SNR
 * @attention The product type includes CCO and STA
 * @retval   ret type #td_u32: #EXT_ERR_SUCCESS: success
 * @li For details about the return values, see soc_errno.h.
 * @par Dependency:
* @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_load_config_belong_network_component(td_void);
/**
 * @ingroup network
 * @brief set route period.
 * @par  Description:
 * set route period.
 * @attention The product type is CCO
 * @param  route_period [IN] Type #td_u16, route length, unit :s.
 * @param  is_effect_now [IN] Type #td_bool, whether effect now .
 * @retval #EXT_ERR_SUCCESS : success, other value refer soc_errno.h
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_mdm_set_route_period(td_u16 route_period, td_bool is_effect_now);
/**
 * @ingroup network
 * @brief get mesh mode
 * @par  Description:
 * get mesh mode
 * @attention The product type is STA
 * @retval #TD_TRUE : in mesh, #TD_FALSE: not mesh
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_bool uapi_mdm_get_is_mesh_mode(td_void);
#ifdef __cplusplus
}
#endif
#endif
