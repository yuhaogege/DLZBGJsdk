/*
 * Copyright (c) CompanyNameMagicTag 2020-2021. All rights reserved.
 * Description: APIs for the secondary development interfaces related to the topology.
 * Note: Change History
 *     Version              Date         DefectNum           Description
 *     V100R001C00SPC016    2020-12-28   NA                  NA
 */

/** @defgroup network  Channel
 * @ingroup plc
 */
#ifndef __SOC_MDM_TOPO_H__
#define __SOC_MDM_TOPO_H__

#include <soc_types.h>

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @ingroup network
 * @brief The role of the current module is unknown.
 */
#define EXT_MDM_PLC_ROLE_UNKNOWN 0
/**
 * @ingroup network
 * @brief The current station is a STA module.
 */
#define EXT_MDM_PLC_ROLE_STA 1
/**
 * @ingroup network
 * @brief The current station is a proxy module.
 */
#define EXT_MDM_PLC_ROLE_PROXY 2
/**
 * @ingroup network
 * @brief The current station is a CCO module.
 */
#define EXT_MDM_PLC_ROLE_CCO 4

/**
 * @ingroup network
 * @brief Maximum number of nodes in the topology
 * To support small and large-scale topologies, the maximum topology scale can be configured by uapi_topo_set_max_tei().
 * uapi_topo_get_max_tei() is used to obtain the topology scale.
 */
#define EXT_MDM_NM_TOPO_NODE_NUM_MAX (uapi_topo_get_max_tei())
/**
 * @ingroup network
 * @brief Maximum station number in one PLC network
 */
#define EXT_TOPO_AVAILABLE_MAX_TEI 4096

/**
 * @ingroup network
 * @brief Maximum number of version information indexes supported by the topology
 */
#define EXT_MDM_NM_STA_VER_INFO_NUM_MAX 8

/**
 * @ingroup network
 * @brief When the version information index table is full, it indicates the index value of 11.
 */
#define EXT_MDM_NM_STA_VER_INDEX_V12 (EXT_MDM_NM_STA_VER_INFO_NUM_MAX + 1)

/**
 * @ingroup network
 * @brief When the version information index table is full, it indicates the index value of 21.
 */
#define EXT_MDM_NM_STA_VER_INDEX_V21 (EXT_MDM_NM_STA_VER_INDEX_V12 + 1)
/**
 * @ingroup network
 * @brief When the version information index table is full, it indicates the index value of 61.
 */
#define EXT_MDM_NM_STA_VER_INDEX_V61 (EXT_MDM_NM_STA_VER_INDEX_V21 + 1)
/**
 * @ingroup network
 * @brief When the version information index table is full, it indicates the index value of 62.
 */
#define EXT_MDM_NM_STA_VER_INDEX_V62 (EXT_MDM_NM_STA_VER_INDEX_V61 + 1)

/**
 * @ingroup network
 * @brief #Invalid value of sta_ver_index in ext_mdm_nm_topo_node_info
 */
#define EXT_MDM_NM_STA_VER_INFO_INDEX_INVALID 15

/**
 * @ingroup network
 * @brief Obtains the number of directly-connected subnodes of a node.
 *
 * @par  Description:
 *            Obtains the number of directly-connected subnodes of a node.
 *
 * @attention The product type is CCO.
 * @param  tei  [IN] Type #td_u16 Station TEI
 *
 * @retval Number of sub-stations of a node
 * @par Dependency:
 *            @li soc_mdm_topo.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u16 uapi_get_child_station_num(td_u16 tei);

/**
 * @ingroup network
 * @brief Topology status Value range: [0, 31] The value range is further divided into 0, 1, 2, 3, [4, 11],
 *       and [12, 19], which correspond to six values of WORKING_STATE. For [4, 11] and
 *       [12, 19], the WORKING_STATE value can be obtained using the formula_state macro.
 */
#define formula_state(v) (3 + (((v) + 4) >> 3))

/**
 * @ingroup network
 * @brief STA working state
 */
typedef enum enum_working_state {
    STATE_NOT_USED = 0x00,     /**< The node is offline. */
    STATE_LEAVE = 0x01,        /**< The node goes offline. */
    STATE_JOIN = 0x02,         /**< The node has just joined the network. */
    STATE_DISCOVERING_END = 0x03, /**< The discovering state ends. */
    STATE_DISCOVERING = 0x04,  /**< The node is in discovering state. */
    STATE_HEARTBERT_CHK = 0x05 /**< The node is in heartbeat detection state. */
} working_state;

/**
 * @ingroup network
 * Value of the node offline state
 */
#define STATE_LEAVE_INIT 1

/**
 * @ingroup network
 * Minimum value of the node discovering state
 */
#define STATE_DISCOVERING_MIN 4

/**
 * @ingroup network
 * Maximum value of the node discovering state
 */
#define STATE_DISCOVERING_MAX 11

/**
 * @ingroup network
 * Initial value of the node heartbeat detection state
 */
#define STATE_HEARTBERT_INIT_VALUE 12

/**
 * @ingroup network
 * Maximum value of the node heartbeat detection state
 */
#define STATE_HEARTBERT_MAX 19

/**
 * @ingroup network
 * Phase identification control information
 */
typedef struct {
    td_u8 phase;                /**< Identified phase */
    td_u8 state : 7;            /**< Phase identification status */
    td_u8 is_have_identify : 1; /**< Whether phase identification is performed */
    td_u8 timeout_cnt;          /**< Identification timeout times */
    td_u8 fail_cnt;             /**< Number of identification failures (the response is received but the phase is
                                 * not identified)
                                 */
} ext_mdm_nm_topo_phase_identify_st;

/**
 * @ingroup network
 * Obtains information of stations in the topology.
 */
typedef struct {
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN]; /**< MAC address */
    td_u16 product_type : 5;    /**< Product type. For details, see CCO:EXT_ND_TYPE_CCO, STA:EXT_ND_TYPE_STA */
    td_u16 sta_ver_index : 4;   /**< Index of stored version information. The value of
                                 * EXT_MDM_NM_STA_VER_INFO_INDEX_INVALID is invalid. Its meaning of can be
                                 * obtained by calling uapi_get_topo_ver_info_by_index().
                                 */
    td_u16 phase : 6;           /**< Evaluates the communication phase. */
    td_u16 heartbeat_count : 1; /**< Indicates whether the heartbeat of the station is received in the current
                                 * heartbeat detection period. 0: not received; 1: received
                                 */
    td_u16 level : 4;  /**< Level: 0-15 */
    td_u16 proxy_tei : 12; /**< TEI of the proxy station */
    td_u8 ability : 3; /**< Station role \ n
                        * CCO: #EXT_MDM_PLC_ROLE_CCO. \ n
                        * Proxy: #EXT_MDM_PLC_ROLE_PROXY. \ n
                        * STA: #EXT_MDM_PLC_ROLE_STA. \ n
                        * Unknown: EXT_MDM_PLC_ROLE_UNKNOWN.
                        */
    td_u8 state : 5;   /**< Node status. Value range: 0-19, corresponding to six statuses */
    td_u8 end_sequence; /**< Storage sequence number. This parameter indicates the E2E storage serial number of the
                         * CCO.
                         */
} ext_mdm_nm_topo_node_info;

/**
 * @ingroup network
 */
#define EXT_TP_INVALID_PARAMETER 0
/**
 * @ingroup network
 */
#define EXT_TP_NOT_USED          1
/**
 * @ingroup network
 */
#define EXT_TP_IN_USE            2
/**
 * @ingroup network
 */
#define EXT_TP_IN_USE_BUT_NO_HT  3

/**
 * @ingroup network
 * @brief  Obtains the network status of a node in the topology.
 *
 * @par   Description:
 *           Obtains the network status of a node in the topology.
 *
 * @attention This function takes effect only for CCO products.
 * @param  tei         [IN] Type #td_u16 Node TEI
 *
 * @retval #EXT_TP_PARAM_INVALID        Incorrect parameters.
 * @retval #EXT_TP_NOT_USED             The node is offline and is not used.
 * @retval #EXT_TP_IN_USE               The node is online.
 * @retval #EXT_TP_IN_USE_BUT_NO_HT     The node is online but does not receive heartbeat messages.
 * @par Dependency:
 *           @li soc_mdm_topo.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_topo_get_state_by_tei(td_u16 tei);
/**
 * @ingroup network
 * @brief  Queries the attributes of a network station.
 *
 * @par  Description: Queries the attributes of a network station.
 *
 * @attention  The product type is CCO. This function takes effect only for CCO products.
 * @param  mac                      [IN]  Type #td_u8* Query of the MAC address of the target station
 * @param  entry                    [OUT] Type #ext_mdm_nm_topo_node_info* Information of the site corresponding to the
 *                                  specified MAC address. For details, see #ext_mdm_nm_topo_node_info.
 *
 * @retval #EXT_ERR_SUCCESS           Success
 * @retval #EXT_ERR_INVALID_PARAMETER The input pointer parameter is null.
 * @retval #EXT_ERR_NOT_EXIST         The current product model is not supported.
 * @par Dependency:
 *           @li soc_mdm_topo.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_query_mac_attr(EXT_IN td_u8 * mac, ext_mdm_nm_topo_node_info * entry);

/**
 * @ingroup network
 * @brief Sets the topology scale and maximum TEI.
 *
 * @par  Description: Sets the topology scale and maximum TEI.
 * @attention The precautions are as follows:
 *         @li When uapi_topo_set_max_tei() is used to configure the topology scale, it must be called earlier than
 *         uapi_topo_get_max_tei().
 *         @li If the topology scale is not set, the calling of uapi_topo_get_max_tei() will trigger the topology scale
 *         to be set to the default value.
 *         @li Once the topology is set to the default value,calling uapi_topo_set_max_tei() again does not take effect.
 *         @li uapi_topo_set_max_tei() The non-zero value only configured for the first time takes effect.
 *         @li Therefore, you need to call uapi_topo_set_max_tei() to configure the topology scale as early as possible,
 *         @li before to call uapi_mac_init().
 *         @li The input parameter max_tei is not limited, but whether the allocation is successful depends on the
 *         overall RAM. Select a proper value as required.
 *         @li The number of stations(CCO and STA) in topo is equal to max_tei.
 * @param  max_tei       [IN] Type #td_u16 Configured topology scale
 * @retval  ret Type #td_u16
 *           @li #0 : Invalid input parameter
 *           @li others : Configuration success, return configured parameter.
 * @par Dependency:
 *           @li soc_mdm_topo.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u16 uapi_topo_set_max_tei(td_u16 max_tei);

/**
 * @ingroup network
 * @brief Obtains the specified topology scale (maximum TEI).
 *
 * @par  Description:
 *           Obtains the specified topology scale (maximum TEI).
 * @attention The precautions are as follows:
 *           @li If the topology scale is not set, calling uapi_topo_get_max_tei() will trigger the topology scale to be
 *           set to the default value. The default value is returned.
 * @param  None
 * @retval Specified maximum topology scale (MAX_TEI)
 * @par Dependency:
 *           @li soc_mdm_topo.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u16 uapi_topo_get_max_tei(void);

/**
 * @ingroup network
 * @brief Obtains information of a node in the topology.
 *
 * @par  Description:
 *           Obtains information of a node in the topology.
 *
 * @attention This function takes effect only on CCO products. Pay attention to the following points:
 *           @li Whether topo_info is TD_NULL depends on the return value.
 *           @li The content pointed to by topo_info is the original information of each node in the topology. It can be
 *           read only and cannot be written.
 *           @li The content is upgraded in real-time based on the online status, role, and hierarchy of the station.
 *           You need to consider the impact of asynchronous read and write on the topology information.
 *           @li The space size that topo_info points to is the value of
 *           EXT_MDM_NM_TOPO_NODE_NUM_MAX * sizeof(ext_mdm_nm_topo_node_info), in bytes.
 *           @li Whether a station is online depends on the value of in_use (TD_TRUE indicates online while TD_FALSE
 *           indicates offline).
 *           @li childsum indicates the number of sub-nodes of the proxy station. The value is invalid and 0 for the
 *           CCO.
 * @param  topo_info [IN] Type #ext_mdm_nm_topo_node_info** Topology storage address pointer obtained by the application
 *           layer
 *
 * @retval None
 * @par Dependency:
 *           @li soc_mdm_topo.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_get_topo_info(ext_mdm_nm_topo_node_info **topo_info);

/**
 * @ingroup network
 * @brief Obtain the number of online stations (including CCO and STA) in the topology.
 *
 * @par  Description:
 *           Obtain the number of online stations (including CCO and STA) in the topology.
 *
 * @attention This function takes effect only for CCO products.
 * @param  None
 *
 * @retval Total number of nodes in the topology
 * @par Dependency:
 *           @li soc_mdm_topo.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u16 uapi_get_topo_node_num(td_void);

/**
 * @ingroup network
 * @brief Obtains the version information based on the version index in the topology.
 *
 * @par  Description:
 *           Obtains the version information based on the version index in the topology.
 *
 * @attention This function takes effect only for CCO products.
 * @param  index           [IN] Type #td_u8 The value is the same as that of sta_ver_index in ext_mdm_nm_topo_node_info.
 *                         (When the value is greater than or equal to
 *                         @li EXT_MDM_NM_STA_VER_INFO_NUM_MAX and is not EXT_MDM_NM_STA_VER_INFO_INDEX_INVALID, the
 *                         version information table is full.)
 * @param  out_info        [OUT] Type #ext_sys_status_info* Output version information
 *
 * @retval #EXT_ERR_SUCCESS Success
 * @retval #EXT_ERR_FULL    The station version information table is full. There is no complete version information
 *                         about the station. It is valid only when out_info is greater than usChipVer.
 * @retval #EXT_ERR_FAILURE Failure
 * @par Dependency:
 *           @li soc_mdm_topo.h: Describes the external APIs of the link layer.
 * @see Definition of #ext_sys_status_info in the soc_mdm_types.h file
 * @since DW21_V100R001C00
 */
td_u32 uapi_get_topo_ver_info_by_index(td_u8 index, ext_sys_status_info *out_info);

/**
 * @ingroup network
 * @brief   Re-authenticates nodes in the network topology based on the whitelist and removes stations that are not in
 *          the whitelist.
 *
 * @par  Description:
 *          Re-authenticates nodes in the network topology based on the whitelist and removes stations that are not in
 *          the whitelist.
 *
 * @attention The product type is CCO.
 * @param  None
 *
 * @retval #EXT_ERR_SUCCESS A success message is returned.
 * @retval #EXT_ERR_FAILURE Failure
 * @par Dependency:
 *           @li soc_mdm_topo.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_reauth_sta_in_topo(td_void);

/**
 * @ingroup network
 */
typedef struct {
    td_u16 sta_num;     /** < Number of STAs removed */
    td_u16 reserved;    /** < Reserved */
    td_u16 tei_list[0]; /** < TEI of the removed STAs */
} ext_mdm_tei_list;

/**
 * @ingroup network
 * @brief  Specifies a batch of stations to go offline at the application layer.
 *
 * @par  Description:
 *         Specifies a batch of stations to go offline at the application layer.
 *
 * @attention The product type is CCO.
 *         The caller must ensure that the value of sta_num in #ext_mdm_tei_list indicates the number of array elements,
 *         which matches the length of the actual array tei_list.
 * @param  tei_list                  [IN] Type #ext_mdm_tei_list* Set of offline stations
 *
 * @retval #EXT_ERR_INVALID_PARAMETER The input parameter is invalid.
 * @retval #EXT_ERR_SUCCESS           The STA is successfully removed.
 * @par Dependency:
 *         @li soc_mdm_topo.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_set_sta_offline_by_teilist(ext_mdm_tei_list *tei_list);

/**
 * @ingroup network
 * @brief  Specifies a single STA to go offline at the application layer.
 *
 * @par  Description:
 *           Specifies a single STA to go offline at the application layer.
 *
 * @attention The product type is CCO.
 * @param   sta_mac[EXT_PLC_MAC_ADDR_LEN] [IN] Type #td_u8 Offline station MAC address
 *
 * @retval  #EXT_ERR_INVALID_PARAMETER      The input parameter is invalid.
 * @retval  #EXT_ERR_SUCCESS                The STA is successfully set to go offline.
 * @par Dependency:
 *           @li soc_mdm_topo.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_set_sta_offline(td_u8 sta_mac[EXT_PLC_MAC_ADDR_LEN]);
/**
 * @ingroup network
 * @brief  Specifies the topology change status when the PLC channel is available.
 *         Networking is complete.
 */
#define EXT_MDM_MAC_NETWORK_FORMED       1

/**
 * @ingroup network
 * @brief  Specifies the topology change status when the PLC channel is available.
 *         Optimization is complete.
 */
#define EXT_MDM_MAC_NETWORK_EVALUATE_END 2

/**
 * @ingroup network
 * @brief  Specifies the topology change status when the PLC channel is available.
 *         The topology is changed
 */
#define EXT_MDM_MAC_TOPO_CHANGE          3

/**
 * @ingroup network
 * @brief  Defines the topology change status structure when the PLC channel is available.
 */
typedef struct {
    td_u8 status;       /**< Topology change status when the PLC channel is available */
    td_u8 reserved[3];  /**< Reserved 3 bytes */
} ext_mdm_topo_status_chg_entry;

/**
 * @ingroup  topo_info
 * @brief   Defines the type of the notification function for the network topology change when the channel is available.
 *
 * @par   Description:
 *          Defines the type of the notification function for the network topology change when the channel is available.
 *
 * @attention None
 * @param  fun [IN] Type #ext_mdm_topo_status_chg_entry* Channel availability parameter. For details, see the definition
 *         of ext_mdm_topo_status_chg_entry.
 * @retval   None
 * @par Dependency:
 *           @li soc_mdm_topo.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
typedef td_void (*ext_mdm_report_topo_status_chg_fun)(ext_mdm_topo_status_chg_entry *);

/**
 * @ingroup  topo_info
 * @brief Registers the network topology change notification when the registration channel is available.
 *
 * @par  Description:
 *        Registers the network topology change notification when the registration channel is available.
 *
 * @attention This API is used by the CCO.
 * @param  fun [IN] Type #ext_mdm_report_topo_status_chg_fun
 *
 * @retval ret [IN] Type #td_u32
 *           @li EXT_ERR_SUCCESS: Success
 *           @li EXT_ERR_FAILURE: Failure
 * @par Dependency:
 *           @li soc_mdm_topo.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_register_report_topo_status_chg(ext_mdm_report_topo_status_chg_fun fun);

/**
 * @ingroup network
 * @brief  Clears the detected version information about a chip at the application layer.
 *
 * @par  Description:
 *           Clears the detected version information about a chip at the application layer.
 *
 * @attention The product type is CCO.
 * @param  None
 *
 * @retval  None
 * @par Dependency:
 *           @li soc_mdm_topo.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_clear_chip_detect_result(td_void);

/**
 * @ingroup network
 * Chip detection result
 */
typedef enum {
    EXT_MDM_CHIP_VER_UNKOWN = 0x00,     /**< The station is not detected. */
    EXT_MDM_CHIP_VER_11 = 0x01, /**< If bit[0] is 1, it indicates that 11 is detected. */
    EXT_MDM_CHIP_VER_12 = 0x02, /**< If bit[1] is 1, it indicates that 12 is detected. */
    EXT_MDM_CHIP_VER_21 = 0x04, /**< If bit[2] is 1, it indicates that 21 is detected. */
    EXT_MDM_CHIP_VER_61 = 0x08, /**< If bit[4] is 1, it indicates that 61 is detected. */
    EXT_MDM_CHIP_VER_62 = 0x10, /**< If bit[5] is 1, it indicates that 62 is detected. */
} ext_mdm_chip_ver_bit_offset;

/**
 * @ingroup network
 * @brief  Obtains the chip version information detected by the CCO at the application layer.
 *
 * @par  Description:
 *         Obtains the chip version information detected by the CCO at the application layer.
 *
 * @attention The product type is CCO.
 * @param   None
 *
 * @retval Value Each bit indicates whether a chip version exists. For details, see the definition in the enumeration
 *         value of #ext_mdm_chip_ver_bit_offset.
 * @par Dependency:
 *           @li soc_mdm_topo.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_get_chip_detect_result(td_void);

/**
 * @ingroup network
 * @brief  Obtains the TEI of a node.
 *
 * @par  Description:
 *            Obtains the TEI of a node.
 *
 * @attention The TEI is 0 when the station is not connected to the network.
 * @param  None
 *
 * @retval   Node TEI. Value range: [1, EXT_MDM_NM_TOPO_NODE_NUM_MAX]
 * @par Dependency:
 *            @li soc_mdm_topo.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u16 uapi_get_my_tei(td_void);
/**
 * @ingroup network
 * @brief  Obtains the proxy TEI of a node.
 *
 * @par  Description:
 *            Obtains the proxy TEI of a node.
 *
 * @param  None
 *
 * @retval  proxy Node TEI. Value range: [1, EXT_MDM_NM_TOPO_NODE_NUM_MAX]
 * @par Dependency:
 *            @li soc_mdm_topo.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u16 uapi_get_proxy_tei(td_void);
/**
 * @ingroup network
 * @brief   Obtains the MAC address of a node.
 *
 * @par  Description:
 *          Obtain the MAC address of a node (the MAC address used by the station to communicate with the PLC network).
 *
 * @attention None
 * @param  mac                       [OUT] Type #td_u8 Station MAC address
 * @param  mac_size                  [IN] Type #td_u32 Size of station MAC address
 *
 * @retval #EXT_ERR_INVALID_PARAMETER The input parameter is invalid.
 * @retval #EXT_ERR_SUCCESS           Success
 * @par Dependency:
 *            @li soc_mdm_topo.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_get_my_mac(td_u8 mac[EXT_PLC_MAC_ADDR_LEN], td_u32 mac_size);

/**
 * @ingroup network
 * @brief   Get TEI by MAC address .
 *
 * @par  Description:
 *          Get TEI by MAC address.
 *
 * @attention The product type is CCO.
 * @param  sta_mac                       [IN] Type #td_u8 Station MAC address
 * @param  sta_tei                  [OUT] Type #td_u16 * address of tei
 *
 * @retval #EXT_ERR_INVALID_PARAMETER The input parameter is invalid.
 * @retval #EXT_ERR_SUCCESS           Success
 * @par Dependency:
 *            @li soc_mdm_topo.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_get_tei_by_mac(td_u8 sta_mac[EXT_PLC_MAC_ADDR_LEN], td_u16 *sta_tei);

/**
 * @ingroup network
 * @brief  Obtains the level of a node.
 *
 * @par  Description:
 *            Obtains the level of a node.
 *
 * @attention When the station is not connected to the network, the level is 0.
 * @param  None
 *
 * @retval   Node level. Value range: [0,15]. The value 0 indicates the level when the STA does not join the network.
 * @par Dependency:
 *            @li soc_mdm_topo.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u8 uapi_get_my_level(td_void);

/**
 * @ingroup network
 * @brief Obtains the role of a node.
 *
 * @par  Description:
 *            Obtains the role of a node.
 *
 * @attention None
 * @param  None
 *
 * @retval #IS_STA    Discovery station
 * @retval #IS_PROXY  Proxy station
 * @retval #IS_CCO    CCO
 * @par Dependency:
 *            @li soc_mdm_topo.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u8 uapi_get_my_role(td_void);

/**
 * @ingroup network
 * @brief Get network serial number
 *
 * @par  Description:
 *        Get network serial number
 *
 * @attention The product type is CCO or STA.
 * @param  None.
 * @retval ret Type #td_u8   form serial num
 * @par Dependency:
 *           @li soc_mdm_topo.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u8 uapi_mdm_get_form_serial_num(td_void);

/**
 * @ingroup network
 * Defines the networking status.
 */
typedef enum {
    FORMING_STATUS = 1, /**< netorking */
    EVALUATE_STAUS,     /**< optimization */
    EVALUATE_END,       /**< Networking and optimization are completed. */
} ext_mdm_network_status;

/**
 * @ingroup  network
 * @brief Obtains the current network status.
 *
 * @par   Description:
 * Obtains the current network status.
 *
 * @attention This API can be called only on the CCO.
 * @param  status   [IN] Type #ext_mdm_network_status
 * @li FORMING_STATUS(1): under networking...
 * @li EVALUATE_STAUS(2): under optimization...
 * @li EVALUATE_END(3): The networking and optimization are completed.
 *
 * @retval ret       [IN] Type #td_u32
 * @li EXT_ERR_SUCCESS: The object parameter settings are synchronized successfully.
 * @li EXT_ERR_INVALID_PARAMETER: The input parameter of the pointer is empty.
 * @par Dependency:
 * @li soc_mdm_mac.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_get_current_network_status(ext_mdm_network_status *status);

#ifdef __cplusplus
}
#endif
#endif