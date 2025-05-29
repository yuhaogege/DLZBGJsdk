/*
 * Copyright (c) CompanyNameMagicTag 2020-2021. All rights reserved.
 * Description: APIs for the secondary development interfaces related to spc.
 * Note: Change History
 *     Version              Date         DefectNum           Description
 *     V100R001C00SPC016    2020-12-28   NA                  NA
 */

/** @defgroup network  Channel
 * @ingroup plc
 */
#ifndef SOC_MDM_STAR_NET_H
#define SOC_MDM_STAR_NET_H

#include <soc_types.h>
#include <soc_mdm_plc_security.h>
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup network
 * @brief Sets STA offline detection on a star network.
 *
 * @par  Description:
 *           Sets STA offline detection on a star network.
 *
 * @attention This API can be called only on the STA. The offline detection time must be set based on the transmission
 *            density of CCO packets, and its validity must be ensured. By default, this function is enabled.
 *            The default offline detection time is 120s. When the offline detection time is set to 0, the default
 *            detection time is used.
 *
 * @param  is_offline_check_off [IN] Type #td_bool, #TD_TRUE: Offline detection is disabled. #TD_FALSE: Offline
 *         detection is enabled.
 * @param   check_rcv_cco_time [IN] Type #td_u16: Sets the detection time (unit: s). If no packets are received from
 *         the CCO within the specified period, the STA goes offline.
 *
 * @retval None
 * @par Dependency:
 *           @li soc_mdm_star_net.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_set_sta_offline_check_for_star(td_bool is_offline_check_off, td_u16 check_rcv_cco_time);

/**
 * @ingroup network
 * @brief Sets an interval for sending Beacon frames when an app on a star network does not send packets.
 *
 * @par  Description:
 *           Sets an interval for sending Beacon frames when an app on a star network does not send packets.
 *
 * @attention This API can be called only on the CCO. The minimum interval is 1000 ms. If the specified minimum interval
 *            is less than 1000 ms, 1000 ms is used. If the minimum interval is not set, the default value (15000 ms) is
 *            used.
 *            If the app does not send packets after the specified interval plus 5000 ms, the beacon frames will be sent
 *            periodically. The specified value must be smaller than  the value of check_rcv_cco_pkt_time set by
 *            uapi_set_sta_offline_check_for_star().
 *            Otherwise, the STA may be offline due to no heartbeat messages.
 *            The setting takes effect only after the policy is restarted at the MDM layer next time.
 *
 * @param  bcn_period [IN] Type #td_u16: beacon sending period (unit: ms)
 *
 * @retval None
 * @par Dependency:
 *           @li soc_mdm_star_net.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_set_bcn_period_without_app_pkt_for_star(td_u16 bcn_period);

/**
 * @ingroup network
 * @brief Sets the carrier sense backoff function of a star network.
 *
 * @par  Description:
 *          Sets the carrier sense backoff function of a star network.
 *
 * @attention None
 *
 * @param  is_use_backoff [IN] Type #td_bool, #TD_TRUE: The carrier sense backoff function is enabled. #TD_FALSE:
 *         The carrier sense backoff function is disabled.
 *
 * @retval None
 * @par Dependency:
 *          @li soc_mdm_star_net.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_set_app_backoff_switch_for_star(td_bool is_use_backoff);

/**
 * @ingroup network
 * spc network identification startup parameter
 */
typedef struct {
    td_u8 send_notice_num;      /**< Number of times the network identification start notification is sent */
    td_u8 send_seq_num;         /**< Number of times the network identification sequence is sent */
    /*
        * Whether to enable the function of receiving the identification start
        * notification by the station that accesses the neighbor network
       */
    td_bool is_neighbor_rcv_enable;
    td_u8 pad;                  /**< Reserved */
} ext_mdm_spc_net_identify_start_para;

/**
 * @ingroup network
 * @brief load spc identify component
 *
 * @par  Description:
 *        load spc identify component in order to be able to join the right network during networking.
 *        before use this interface, should enable the spc identify featrue.
 *
 * @attention CCO/STA
 *            @li Invoke this interface before using other spc interfaces, shall be invoked after uapi_mac_cp_init.
 * @param   None
 *
 * @retval  None
 * @par None
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_load_spc_identify_component(td_void);

/**
 * @ingroup network
 * @brief  Starts the spc network identification.
 *
 * @par   Description:
 *           Starts the spc network identification.
 *
 * @attention Starts the spc network identification. This API takes effect only when the spc identification
 *            function is enabled.
 *
 * @param  para   [IN] Type #ext_mdm_spc_net_identify_start_para * Startup parameter
 * The parameters are described as follows: send_notice_num: notification sending times. The value is not 0.
 * The recommended value is 40. send_seq_num: Sequence sending times. The value is not 0. The recommended value is 5.
 * is_neighbor_rcv_enable: TD_FALSE: disabled. Only the local network station and offline stations are notified to
 * start the identification. TD_TRUE: The function is enabled. That is, all stations, including the
 * other stations that have accessed the network, start identification once receiving notifications.
 *
 * @retval #EXT_ERR_SUCCESS  Success
 * @retval #EXT_ERR_BUSY     Failure. The current identification process is not complete.
 * @retval #EXT_ERR_INVALID_PARAMETER  The parameter is invalid.
 * @retval Other values.       For details, see soc_errno.h.
 * @par Dependency:
 *           @li soc_mdm_star_net.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_start_spc_net_identify(ext_mdm_spc_net_identify_start_para *para);

/**
 * @ingroup network
 * @brief  Stops the spc network identification.
 *
 * @par   Description:
 *         Stops the spc network identification.
 *
 * @attention The product type is CCO. This API takes effect only when the spc identification function is enabled.
 *
 * @param  None
 *
 * @retval #EXT_ERR_SUCCESS  Success
 * @retval #EXT_ERR_FAILURE  Failure
 * @retval Other values.       For details, see soc_errno.h.
 * @par Dependency:
 *           @li soc_mdm_star_net.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_stop_spc_net_identify(td_void);

/**
 * @ingroup network
 * @brief Set the chip version to dw11V200,it use to support equipement tests for spc identification.
 *
 * @par   Description:
 *        Enable or disable spc identification to support equipement tests, This API takes effect only when the spc
 *        identification function is enabled. it just used in send beacon frame period.
 *
 * @attention Only support 3.2MHz network, CCO Only
 *
 * @param  flag [IN] Type #td_bool  TD_TRUE: enable ; TD_FALSE: disable
 *
 * @retval td_void
 * @par Dependency:
 *            @li soc_mdm_star_net.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_set_spc_chip_version_11v200(td_bool flag);

/**
 * @ingroup network
 * @brief  Obtains the preamble index used by the 150 kHz frequency band.
 *
 * @par   Description:
 *         Obtains the preamble index used by the 150 kHz frequency band.
 *
 * @attention The product type is CCO or STA.
 *
 * @param  None
 *
 * @retval #td_u8
 * @par Dependency:
 *           @li soc_mdm_star_net.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u8 uapi_get_cur_preamble_index(td_void);

/**
 * @ingroup network
 * @brief  Randomly generates a preamble index.
 *
 * @par   Description:
 *         Randomly generates a preamble index.
 *
 * @attention The product type is CCO. This API takes effect only when the spc identification function is enabled.
 *            After this function is called, the preamble index of the CCO is different from that of the STA, but the
 *            preamble index will not effective immediately, after called uapi_start_spc_net_identify(), then the new
 *            value beginning to take effect. In this case, uapi_start_spc_net_identify() needs to be called again for
 *            identification.
 *
 * @param  None
 *
 * @retval #td_void
 * @par Dependency:
 *           @li soc_mdm_star_net.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_select_preamble_index(td_void);

/**
 * @ingroup network
 * @brief  Sets the preamble index to the default index.
 *
 * @par   Description:
 *         Sets the preamble index to the default index.
 *
 * @attention The product type is CCO. Only support 150kHz network,This API takes effect only when the spc
 *            identification function is enabled.
 *
 * @param  None
 *
 * @retval #td_void
 * @par Dependency:
 *           @li soc_mdm_star_net.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_set_preamble_index_default(td_void);
/**
 * @ingroup network
 * @brief Set User-defined information (only for band 3)
 *
 * @par  Description:
 *       Set the product type (only for band 3)
 * @attention  This interface does not make scope judgment for entry,
 *             but only low 4 bits will be used in this interface.
 * @attention  The set value is populated with the association request in the association
 *             request when the STA is working in band 3.
 * @param  user_info  [IN] Type #td_u8, user info.
 *
 * @retval   ret Type #td_u32
 *              @li #EXT_ERR_SUCCESS.
 *              @li #Other values: FAIL.
 *
 * @par Dependency:
 *           @li soc_mdm_star_net.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_mdm_set_user_defined_info(td_u8 user_info);
/**
 * @ingroup network
 * @brief Get User-defined information (only for band 3)
 *
 * @par  Description:
 * Get User-defined information (only for band 3)
 * @retval ret Type #td_u8 :Product type.
 * @par Dependency:
 * @li soc_mdm_star_net.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u8  uapi_mdm_get_user_defined_info(td_void);

/**
 * @ingroup network
 * @brief  report receive packet signal quality.
 */
typedef struct {
    td_s8 snr;   /**< Signal to noise ratio of received packet.  */
    td_u8 atten; /**< Attenuation value of the received packet. */
    td_u16 stei; /**< Source tei of the received packet.  */

    td_u32 snid; /**< Network ID of the received packet */
} ext_mdm_signal_info_of_packet;

/**
 * @ingroup network
 * @brief Report the attenuation, signal-to-noise ratio, etc. of received messages
 *
 * @par  Description:
 * Report the attenuation, signal-to-noise ratio, etc. of received messages
 *
 * @attention None
 * @param  reoprt_info [IN] Type #ext_mdm_signal_info_of_packet *, .
 *
 * @retval   None
 *
 * @par Dependency:
 * @li soc_mdm_star_net.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
typedef td_void (*ext_mdm_signal_info_report_callback)(ext_mdm_signal_info_of_packet *reoprt_info);

/**
 * @ingroup network
 * @brief Register reporting the attenuation, signal-to-noise ratio, etc. of received packet callback function.
 *
 * @par  Description:
 * Register reporting the attenuation, signal-to-noise ratio, etc. of received packet callback function.
 *
 * @param  cb  [IN] Type #ext_mdm_signal_info_report_callback, callback function name.
 *
 * @retval   ret Type #td_u32
 * @li #EXT_ERR_SUCCESS.
 * @li #EXT_ERR_INVALID_PARAMETER: FAIL.cb is TD_NULL.
 *
 * @par Dependency:
 * @li soc_mdm_star_net.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_register_signal_info_report(ext_mdm_signal_info_report_callback cb);

#ifdef __cplusplus
}
#endif
#endif