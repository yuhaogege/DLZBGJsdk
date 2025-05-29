/*
 * Copyright (c) CompanyNameMagicTag 2012-2018. All rights reserved.
 * Description: NDM related interfaces
 * Author: CompanyName
 * Create: 2012-07-13
 */

/** @defgroup ndm_group NDM service
 * @ingroup mid
 */
#ifndef __SOC_MDM_NDM_H__
#define __SOC_MDM_NDM_H__

#include <soc_list.h>
#include <soc_types.h>
#include "soc_mdm_types.h"
#include "soc_config.h"
#include "soc_mdm_mac.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup ndm_group
 * @brief Indicates that the frequency band is not specified and the NDC automatically searches for the frequency band.
 */
#define EXT_MDM_PLC_FREQ_AUTO 0xFF

/**
 * @ingroup ndm_group
 * @brief Indicates that the snid is not specified, and the NDC automatically searches for the snid.
 */
#define EXT_MDM_PLC_SNID_AUTO 0

/**
 * @ingroup ndm_group
 * @brief
 */
#define EXT_MDM_CHL_STATUS_DISCONNECT 2

/**
 * @ingroup ndm_group
 * @brief
 */
#define EXT_NDM_NDM_MAX_SEND_REQ 5

/**
 * @ingroup ndm_group
 * @brief Indicates that the NDC notifies the DBK to update or add the PLC network search information.
 */
#define EXT_NDC_SEARCH_CONN_IND_TYPE_NEW 0

/**
 * @ingroup ndm_group
 * @brief Indicates that the NDC reports all PLC network information.
 */
#define EXT_NDC_SEARCH_CONN_IND_TYPE_ALL 1

/**
 * @ingroup ndm_group
 * @brief Indicates that the NDC reports the current network search status.
 */
#define EXT_NDC_SEARCH_CONN_IND_TYPE_STATUS 2

/**
 * @ingroup ndm_group
 * Used for the PLC network search and connection initiated by the DBK.
 */
#define EXT_ND_NETWORK_SEARCH_CONN 0x3408
/**
 * @ingroup ndm_group
 * Ndm max tei.
 */
#define EXT_NDM_MAX_TEI 0xFFE
/**
 * @ingroup ndm_group
 * Ndm min tei.
 */
#define EXT_NDM_MIN_TEI 0xFEF

/**
 * @ingroup ndm_group
 * @brief Define the request connection mode.
 */
typedef enum {
    EXT_NDM_CONNECT_UNKNOWN_MODE = 0,
    EXT_NDM_CONNECT_NORMAL_CNNC_MODE = 1,
    EXT_NDM_CONNECT_SEIZE_CNNC_MODE,
    EXT_NDM_CONNECT_DISCONNECT_MODE,
    EXT_NDM_CONNECT_INVALID_MODE
} ext_ndm_connect_req_mode;

/**
 * @ingroup ndm_group
 * @brief Defines the reply result of the reply frame.
 */
typedef enum {
    EXT_NDC_CONNECT_RESULT_SUCCESS = 0, /**< Connection succeeded */
    EXT_NDC_CONNECT_RESULT_SEIZE,       /**< Use the forcible connection mode */
    EXT_NDC_CONNECT_RESULT_LOST_CNNC,   /**< The current connection is lost */
    EXT_NDC_CONNECT_RESULT_DISCONNCET   /**< Disconnect */
} ext_ndm_connect_rsp_result;

/**
 * @ingroup ndm_group
 * @brief Define the working status of the NDC channel.
 */
typedef enum {
    MAC_NDC_CH_WORK_STATUS_IDLE,       /**< Idle */
    MAC_NDC_CH_WORK_STATUS_CONNECTED,  /**< Connected */
    MAC_NDC_CH_WORK_STATUS_DISCONNECT, /**< Disconnected */
    MAC_NDC_CH_WORK_STATUS_CONNECTING, /**< The connection is being set up */
    MAC_NDC_CH_WORK_STATUS_SEARCH      /**< Network search status */
} mac_ndc_ch_work_status;

/**
 * @ingroup ndm_group
 * @brief  Definition of network search mode
 */
typedef enum {
    EXT_NDC_CHL_ACCESS_MODE_CCO = 0, /**< Indicates the CCO search network access mode. Only the CCO is searched and
                                     * connected (by default, the controller connects to the network after
                                     * power-on)
                                     */
    EXT_NDC_CHL_ACCESS_MODE_STA,     /**< Indicates that the STA searches for and connects to the STA */
    EXT_NDC_CHL_ACCESS_MODE_AUTO     /**< Indicates that the mode is not specified */
} ext_ndc_chl_access_mode;

/**
 * @ingroup ndm_group
 * @brief  Definition of the command for searching for networks
 */
typedef enum {
    EXT_NDC_CHL_ACTION_SEARCH = 0,   /**< Indicates that the NDC searches only the network but does not connect to
                                     * the network. (The NDC automatically searches for the network after the NDC
                                     * is powered on or powered off in CCO mode.)
                                     */
    EXT_NDC_CHL_ACTION_SEARCH_QRY,   /**< Indicates that the network information (including the network search mode,
                                     * MAC address, IP address, and device name) is queried during network search.
                                     */
    EXT_NDC_CHL_ACTION_CONNECT,      /**< Indicates that the network is selected(connected) */
    EXT_NDC_CHL_ACTION_DISCONNECT,   /**< Indicates that the network is disconnected */
    EXT_NDC_CHL_ACTION_CLOSE,        /**< Indicates that the network search is disabled */
    EXT_NDC_CHL_ACTION_SEIZE_CONNECT /**< Force to connect */
} ext_ndc_chl_connect_action;

/**
 * @ingroup ndm_group
 * @brief  Network search request parameters
 */
typedef struct {
    ext_ndc_chl_access_mode access_mode; /**< Network search mode */
    td_u8 search_freq;                  /**< Network search frequency band */
    td_u8 action;                       /**< For details, see the definition of ext_ndc_chl_connect_action */
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN];     /**< MAC Address */
    td_u32 snid;                        /**< Network identifier */
} ext_mac_ndm_search_conn_req;

/**
 * @ingroup ndm_group
 * @brief Parameters of the network search command
 */
typedef struct {
    ext_mac_ndm_search_conn_req req; /**< For details, see the definition of #ext_mac_ndm_search_conn_req_s */
} ext_mac_ndm_report_net_info_req_st;

/**
 * @ingroup ndm_group
 * @brief Information about a single STA in the network search result report
 */
typedef struct {
    td_u8 connect : 1;
    td_u8 freq : 5;                 /**< Frequency band */
    td_u8 prod_type_sta_type : 1;   /**< STA products are used to distinguish the I-type meter and electric meter */
    td_u8 pad : 1;                  /**< Reserved  */
    td_u8 rsvd;                     /**< Reserved  */
    td_u8 mac[EXT_PLC_MAC_ADDR_LEN]; /**< MAC address */

    td_u32 snid;          /**< Network identifier */
    td_u32 discovery_cnt; /**< Number of times that the communication module is searched */

    td_u8 ipv4[EXT_IPV4_ADDR_LEN];    /**< IP address of the PLC network node */
    td_u8 dev_id[EXT_MDM_DEV_ID_LEN]; /**< Device ID of the network node */
    td_s8 channel_info;              /**< Channel quality information */
    td_u8 ndc_ver;                   /**< NDC Channel version number */

    td_u8 prod_type; /**< Product Version #ext_ftm_product_type */
    td_u8 rvsd[3];   /**< Reserved 3 bytes */
} ext_mac_ndm_multinetwork_info_st;

/**
 * @ingroup ndm_group
 * @brief Network searching result
 */
typedef struct {
    ext_ndc_chl_access_mode access_mode;      /**< Indicates the mode for the communication module of the transponder
                                              * to connect to the PLC network. */
    td_u8 ndc_status : 3;                    /**< Indicates the current state of the NDC. */
    td_u8 report_type : 3;                   /**< Report type, partial or all, or status update */
    td_u8 rvsd : 2;                          /**< Reserved */
    td_u8 cur_work_freq;                     /**< Indicates the current working network frequency band. */
    td_u8 node_num;                          /**< Indicates the number of searched networks. */
    td_u8 pad;                               /**< Reserved */
    td_u32 cur_snid;                         /**< Indicates the current network ID. */
    td_u32 err_code;                         /**< Indicates the network search/network connection error code. */
    td_u32 continue_time;                    /**< Duration of searching for the network, in seconds. */
    ext_mac_ndm_multinetwork_info_st node[0]; /**< network parameter. This parameter is valid only when ucNodeNum is
                                              * not 0. */
} ext_mac_ndm_report_net_info_st;

/**
 * @ingroup ndm_group
 * @brief Network searching result parameter
 */
typedef struct {
    ext_mac_ndm_report_net_info_st report; /**<  For details, see the definition of #ext_mac_ndm_report_net_info_st */
} ext_mac_ndm_report_net_info_ind_st;

/**
 * @ingroup ndm_group
 * @brief Format of the P2P service packet.
 */
typedef struct {
    td_u8 src_mac[EXT_PLC_MAC_ADDR_LEN];  /**< MAC address of the message Sender */
    td_u8 dest_mac[EXT_PLC_MAC_ADDR_LEN]; /**< MAC address of the message receiver */

    td_u8 max_resend_count; /**< Maximum number of times for sending packets. If this parameter is not
                             * set, the default value 5 is used by default.
                             */
    td_u8 lid;              /**< Packet priority */
    td_u8 port_num;         /**< Port number. The value is #EXT_DMS_CHL_MAC_PORT_APP */
    td_u8 ctrl;             /**< Control word. For details, see EXT_DMS_CHL_TX_CTRL_XXX. The maximum
                             * value is #EXT_DMS_CHL_TX_CTRL_MAX
                             */
    td_u16 id;              /**< Frame ID, which is customized by the user */
    td_u16 data_size;       /**< Size of the data to be sent */
    td_u8 *data;            /**< Data to be sent */

    td_u8 is_active_report; /**< The flag of sta's auto push data.#TD_TRUE:is active push */
    td_u8 reserved[3];      /**< 3 reserved byte number */
} ext_p2p_mac_frame_ctrl;

/**
 * @ingroup ndm_group
 * @brief P2P service packet sending interface
 *
 * @par  Description:
 * P2P service packet sending interface.This interface adds the STA's conditional active push
 * P2P data function. On the STA, whether to actively push data is determined according to the
 * value of is_active_report. When is_active_report is TRUE, the STA traverses the 1901 protocol
 * band and sends a handshake frame to search for the CIU device when it doesn't join the network.
 * When the handshake with the CIU device is successful, the P2P packet is sent. The STA will record
 * the latest search push information (receive the target site handshake response frame). If data is
 * pushed to a recorded site on the same frequency band, the STA will send the data directly. To
 * ensure that the normal networking functions of STAs are not affected, some restrictions are added here:
 * 1. STA on the network only sends P2P data directly in this frequency band. It will not switchs
 * frequency bands and will not send handshake frames.
 * 2. When not connected to the network, the STA works in the frequency band connected to the CIU or when
 * the handshake with the CIU succeeds, the STA actively pushes the data without first sending a
 * handshake frame, and directly sends the data. In other P1901.1 protocol bands, STA needs to send
 * handshake frames for searching. The STA search interval is an integer multiple of 60 seconds (the
 * integer multiple is the current number of P1901.1 protocol frequency bands).
 * 3. The STA sends handshake frames by default in a round of search: the number of handshake frames sent
 * in each frequency band is 6, the interval between handshake frames is 1 second.
 *
 * @attention The interface is used to send P2P service packets. All service packets are sent in local broadcast mode.
 * @attention Range of data size that can be sent is (0, 476].
 * @attention Invoked by NDM/STA
 * @attention dest_mac cannot use the following MAC addresses:
 * 0x00: 0x00: 0x00: 0x00: 0x00: 0x00,
 * 0xff: 0xff: 0xff: 0xff: 0xff: 0xff.
 * @param  frame_ctrl [IN] type #ext_p2p_mac_frame_ctrl* : service packets
 * @param  frame_ctrl_len [IN] type #td_u8 : sizeof(ext_p2p_mac_frame_ctrl)
 *
 * @retval   ret type #td_u32
 * @li EXT_ERR_INVALID_PARAMETER: Invalid input parameter.
 * @li EXT_ERR_MALLOC_FAILUE: (CIU/STA)Memory allocation error
 * @li EXT_ERR_P2P_IN_LINKING: (CIU/STA)Start the search.
 * @li EXT_ERR_P2P_IN_SEARCHING: (CIU/STA)The search is in progress. New search cannot be started.
 * @li EXT_ERR_NOT_SUPPORT: CIU is not working in p2p communication mode.
 * @li EXT_ERR_P2P_PUSH_FREQUENTLY: STA pushes too frequently.
 * @li EXT_ERR_BUSY : (CIU/STA)is sending P2P data which needs to wait for a while.
 * @li EXT_ERR_SUCCESS : (STA) send packet successful.
 * @par Dependency:
 * @li soc_mdm_ndm.h : Describes the external APIs of the link layer.
 * @see None.
 * @since DW21_V100R001C00
 */
td_u32 uapi_p2p_chl_tx(EXT_CONST ext_p2p_mac_frame_ctrl *frame_ctrl, td_u8 frame_ctrl_len);

/**
 * @ingroup ndm_group
 * @brief  When the app receives the p2p packet, it invokes this interface to indicate that the p2p packet has been
 * received.
 *
 * @par  Description:
 * After receiving the p2p packet, the app invokes this interface to indicate that the p2p packet has been
 * received.
 *
 * @attention After the app identifies the p2p packet, it needs to invoke this interface to inform the MAC layer that
 * the packet has been received.
 * @attention Invoked by the NDM/STA
 * @param osa_mac [IN] type #td_u8*, source MAC address of packets
 * @param mac_len [IN] type #td_u8,mac address length
 * @retval ret type #td_u32.
 * @li EXT_ERR_SUCCESS: The notification is successful.
 * @li EXT_ERR_INVALID_PARAMETER: Invalid input parameter.
 *
 * @par Dependency:
 * @li soc_mdm_mac.h. The file is used to describe the external interfaces of the link layer.
 * @see None.
 * @since DW21_V100R001C00
 */
td_u32 uapi_app_received_p2p_pkt(EXT_IN EXT_CONST td_u8 *osa_mac, EXT_IN td_u8 mac_len);

/**
 * @ingroup ndm_group
 * @brief Load the information about the components related to the P2P communication.
 *
 * @par description:
 * Load P2P communication component information.
 *
 * @attention Invoked by the NDM/STA. It can be invoked only once.
 * @param None
 *
 * @retval ret type #ext_mac_component_info_st
 *
 * @par dependency:
 * @li soc_mdm_mac.h: The file is used to describe the external interfaces of the link layer.
 * @see None.
 * @since DW21_V100R001C00
 */
td_void uapi_load_p2p_component_p1901(td_void);

/**
 * @ingroup ndm_group
 * @brief
 * Set whether the CIU communicate with dynamic MAC address.
 * @par description:
 * Run the following command to check whether the CIU has no MAC address.
 *
 * @attention When set #TD_TRUE, CIU automatically uses the MAC address of the target to be
 * connected as its own MAC address.
 * @attention When set #TD_FALSE, CIU uses fixed MAC address communication, the caller must ensure
 * that the MAC address of the NDM is
 * valid and unique.In addition,the identification and processing flow of fixed MAC communication
 * for CIU should be added at the application layer of receiver.
 * @attention Fixed MAC address and dynamic MAC address cannot be used at the same time.
 * @attention Invoked by the NDM
 * @param is_no_mac [IN] type #td_bool:
 * @li TD_TRUE: Dynamic MAC address. Use the MAC address of the P2P communication target site as its own MAC (except
 * for wildcard MAC addresses)
 * @li TD_FALSE: Fixed MAC address. Use CIU's own MAC address
 * @retval ret type #td_u32.
 * @li EXT_ERR_SUCCESS: The setting is successful.
 * @li # Other return values fail to be set.
 *
 * @par dependency:
 * @li soc_mdm_mac.h: The file is used to describe the external interfaces of the link layer.
 * @see None.
 * @since DW21_V100R001C00
 */
td_u32 uapi_p2p_set_ciu_no_mac_state_p1901(EXT_IN td_bool is_no_mac);

/**
 * @ingroup ndm_group
 * brief the type of ndm's work mode
 */
typedef enum {
    EXT_NDM_UNKNOWN_WORK_MODE = 0,      /**< Unknown mode */
    EXT_NDM_CIU_MODE,                   /**< CIU mode */
    EXT_NDM_SEARCH_NET_MODE,            /**< Network search mode */
    EXT_NDM_CIU_MODE_SWITCH_COM,        /**< CIU mode which needs switching com type */
    EXT_NDM_SEARCH_NET_MODE_SWITCH_COM, /**< Network search mode which needs switching com type */
    EXT_NDM_MONITOR_MODE,
} ext_ndm_work_mode_type;

/**
 * @ingroup ndm_group
 * @brief  Registration and initialization of the transponder channel function (P1901.1 protocol)
 *
 * @par description:
 * Register and initialize the function of the transponder channel.
 * @par dependency:
 * @li soc_mdm_ndm.h: The file is used to describe the external interfaces of the link layer.
 * @see None.
 * @since DW21_V100R001C00
 */
td_u32 uapi_register_ndm_chl_p1901(td_void);

/**
 * @ingroup ndm_group
 * @brief  Registration and initialization of the transponder channel function (SG protocol)
 *
 * @par description:
 * Register and initialize the function of the transponder channel.
 * @par dependency:
 * @li soc_mdm_ndm.h: The file is used to describe the external interfaces of the link layer.
 * @see None.
 * @since DW21_V100R001C00
 */
td_u32 uapi_register_ndm_chl_psg(td_void);
/**
 * @ingroup ndm_group
 * @brief Indicates the callback function type for judging point to point packet by destination mac address.
 *
 * @par  Description:
 * Indicates the callback function type for judging whether receive point to point packet by destination mac address.
 *
 * @attention The product type is STA or NDM.
 * @param  dest_mac [IN] Type #td_u8*: destination mac address.
 * @param  dest_mac_len [IN] Type #td_u8: destination mac address length.
 * @retval ret type #td_bool.
 * @li TD_TRUE: receive this point to point packet.
 * @li TD_FALSE: drop this packet
 *
 * @par Dependency:
 * @li soc_mdm_ndm.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
typedef td_bool (*ext_mdm_is_rcv_ciu_pkt_by_dest_mac)(EXT_CONST td_u8 *dest_mac, td_u8 dest_mac_len);

/**
 * @ingroup ndm_group
 * @brief Register the callback function for judging point to point packet by destination mac address.
 *
 * @par  Description:
 * Register the callback function for for judging whether receive point to point packet by destination mac address.
 *
 * @attention The product type is STA or NDM.
 * @param  cb [IN] Type #ext_mdm_is_rcv_ciu_pkt_by_dest_mac: callback function.
 * @retval ret type #td_u32.
 * @li EXT_ERR_INVALID_PARAMETER: return this value when cb is null.
 * @li EXT_ERR_SUCCESS: registration success
 *
 * @par Dependency:
 * @li soc_mdm_ndm.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_register_judge_is_rcv_p2p_pkt_by_dest_mac(ext_mdm_is_rcv_ciu_pkt_by_dest_mac cb);
/**
 * @ingroup ndm_group
 * brief the type of event type
 */
typedef enum {
    P2P_SHARK_HANDS_START = 0, /**< Start handshake.  */
    P2P_SHARK_HANDS_FAIL,      /**< Handshake failure. */
    P2P_SHARK_HANDS_SUCCESS,   /**< Handshake success. */
    P2P_SEND_PACKET,           /**< Data was sent successfully. */
} ext_p2p_event_report_code;
/**
 * @ingroup ndm_group
 * brief the type of report information
 */
typedef struct {
    ext_p2p_event_report_code code; /**< Event reporting code  */

    td_u16 frame_id; /**< frame id, the value is equal to id in ext_p2p_mac_frame_ctrl */
    td_u16 rsved_1;  /**<  reserved 2 bytes */

    td_u8 current_frequency; /**< Current frequency band  */
    td_u8 rsved_2;           /**<  reserved 1 bytes */

    td_u8 dest_mac[EXT_PLC_MAC_ADDR_LEN]; /**< Destination site MAC address  */
} ext_p2p_event_info;
/**
 * @ingroup ndm_group
 * @brief Indicates the callback function type for processing reported data.
 *
 * @par  Description:
 * Indicates the callback function type for processing reported data.
 *
 * @attention The product type is STA or NDM.
 * @param  report_info [IN] Type #ext_p2p_event_info*: data to be reported.
 * @retval none.
 *
 * @par Dependency:
 * @li soc_mdm_ndm.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
typedef td_void (*ext_p2p_event_report_callback)(ext_p2p_event_info *report_info);
/**
 * @ingroup ndm_group
 * @brief Register the callback function for processing reported data.
 *
 * @par  Description:
 * Register the callback function for processing reported data.
 *
 * @attention The product type is STA or NDM.
 * @param  callback [IN] Type #ext_p2p_event_report_callback: function name.
 * @retval ret type #td_u32.
 * @li EXT_ERR_INVALID_PARAMETER: callback is null pointer.
 * @li EXT_ERR_SUCCESS: registration success.
 *
 * @par Dependency:
 * @li soc_mdm_ndm.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_register_p2p_event_report(ext_p2p_event_report_callback callback);
/**
 * @ingroup ndm_group
 * @brief Register the callback function for processing the handshake and heartbeat
 * FC sent from the NDM in the case of point-to-point communication.
 *
 * @par  Description: Register the callback function for processing the handshake and heartbeat
 * FC sent from the NDM in the case of point-to-point communication.
 *
 * @attention The product type is STA.
 * @param  None.
 * @retval None.
 * @par Dependency: soc_mdm_ndm.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_mdm_register_process_p2p_fc_function(td_void);
#ifdef __cplusplus
}
#endif

#endif
