/*
 * Copyright (c) CompanyNameMagicTag 2020-2021. All rights reserved.
 * Description: APIs for the secondary development interfaces related to the fast sending.
 * Note: Change History
 *     Version              Date         DefectNum           Description
 *     V100R001C00SPC016    2020-12-28   NA                  NA
 */

/** @defgroup network  Channel
 * @ingroup plc
 */
#ifndef SOC_MDM_FASE_SEND_H
#define SOC_MDM_FASE_SEND_H

#include <soc_types.h>

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @ingroup network
 * @brief  Specifies the type of the registration callback function that notifies the app of the sending result after
 *         the PLC fast sending is complete.
 *
 * @par  Description:
 *         Specifies the type of the registration callback function that notifies the app of the sending result after
 *         the PLC fast sending is complete.
 *
 * @attention None
 * @param  send_result [OUT] Type #td_u32 Sending result. #EXT_ERR_SUCCESS: The sending is successful. For other
 *         information, see soc_errno.h.
 *
 * @retval   None
 * @par Dependency:
 *         @li soc_mdm_fast_send.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
typedef td_void (*ext_mac_bb_plc_send_result_fun)(td_u32 send_result);

/**
 * @ingroup network
 * Defines the fast sending mode.
 */
typedef enum {
    EXT_MAC_FAST_SEND_BACKOFF = 0, /**< The result sending is backed off. > */
    EXT_MAC_FAST_SEND_PRIORITY,    /**< The result is preferentially sent. > */
    EXT_MAC_FAST_SEND_PREEMPT,     /**< The result is sent immediately. > */
    EXT_MAC_FAST_SEND_MAX
} ext_mdm_fast_send_mode;

/**
 * @ingroup network
 * Defines the ext_mdm_bb_plc_send_para information.
 */
typedef struct {
    td_u8 send_phase; /**< TX phase, expressed in bit. bit0: A phase; bit1:B phase; bit2: C phase.
                           If these bits all set to 0, the TX phase is not specified. If they are specified, data is
                           transmitted in three phases. */
    td_u8 resend_cnt; /**< Maximum number of sending times. If the value is 0, the default value is used. If the value
                           is not 0, the set value is used. */
    td_u8 send_mode;  /**< Fast sending mode. For details, see #ext_mdm_fast_send_mode. */
    td_u8 broadcast_flag; /**< Broadcast flag. The value 1 indicates broadcast, and the value 0 indicates unicast. */
    td_u16 data_length;   /**< Data sending degree */
    td_u16 pad;
    td_u8 oda[EXT_PLC_MAC_ADDR_LEN]; /**< Destination MAC address */
    td_u16 time_out;                /**< Timeout interval of packets in the queue, in ms > */
    ext_mac_bb_plc_send_result_fun bb_plc_send_result_fun; /**< Callback function for reporting the sending result */
    td_u8 data[0];                                        /**< Buffer needs to be sent */
}ext_mdm_bb_plc_send_para;

/**
 * @ingroup network
 * @brief Allows users to implement fast sending through the pb.
 *
 * @par  Description:
 *           Allows users to implement fast sending through the pb.
 *
 * @attention None
 * @param  mac_bb_plc_send_para [IN] Type #ext_mdm_bb_plc_send_para Data and control parameters delivered by users
 *        Where: send_phase indicates the TX phase. bit0: A phase; bit1: B phase; bit2: C phase. If the bit is set to 1,
 *        the bit is valid. If the bit is set to 0, the bit is invalid.
 *        The maximum value is 7, indicating that the three phases are all specified. The minimum value is 0, indicating
 *        that the three phases are not specified.
 *        The sending modes end_mode and ext_mdm_fast_send_mode are invalid if the value is greater than
 *        #EXT_MAC_FAST_SEND_PREEMPT.
 *        In the preceding information, data_length indicates the length of the delivered data, valid range of the State
 *        Grid protocol is (0, 492], and valid range of the 1901 protocol is (0, 488].
 *        When the sending mode is set to EXT_MAC_FAST_SEND_BACKOFF, the fast packet sending result is not reported to
 *        prevent callback when normal packets are sent.
 * @retval   ret Type #td_u32
 *        @li #EXT_ERR_SUCCESS: The startup is successful.
 *        @li #EXT_ERR_FAILURE: The startup fails.
 *        @li #EXT_ERR_INVALID_PARAMETER: Invalid parameter.
 *        @li #EXT_ERR_MALLOC_FAILUE: Failed to apply for space.
 * @par Dependency:
 *        @li soc_mdm_fast_send.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_fast_send_pb_on_plc(EXT_CONST ext_mdm_bb_plc_send_para *mac_bb_plc_send_para);

/**
 * @ingroup network
 * Defines the ext_mac_report_rcv_fast_send_pkt information.
 */
typedef struct {
    td_u8 *data;        /**< Pointer to the received packet */
    td_u16 data_length; /**< Length of the received packet */
    td_u16 pad;
    td_u8 osa[EXT_PLC_MAC_ADDR_LEN]; /**< Sent source MAC address > */
    td_u8 oda[EXT_PLC_MAC_ADDR_LEN]; /**< Received destination MAC address > */
} ext_mac_report_rcv_fast_send_pkt;

/**
 * @ingroup network
 * @brief Based on the fast sending channel function, the RX end reports data to the app for registering the callback
 *        function type.
 *
 * @par  Description:
 *        Based on the fast sending channel function, the RX end reports data to the app for registering the callback
 *        function type.
 *
 * @attention None
 * @param  data [IN] Type #ext_mac_report_rcv_fast_send_pkt Report of the data to the user
 * After receiving the FC fast sending packet, the receive end reports the maximum 8-byte payload carried in the FC to
 * the application layer.
 * The application layer obtains the carried content based on the length of the actually sent data.
 * @retval   None
 *
 * @par Dependency:
 *           @li soc_mdm_fast_send.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
typedef td_void (*ext_mdm_fast_send_rcv_call_back)(ext_mac_report_rcv_fast_send_pkt *data);

/**
 * @ingroup network
 * @brief Allows users to implement fast sending by using FC sack frames.
 *
 * @par  Description:
 *           Allows users to implement fast sending by using FC sack frames.
 *
 * @attention None
 * @param  mac_bb_plc_send_para [IN] Type #ext_mdm_bb_plc_send_para Data and control parameters delivered by users
 *         Where: send_phase indicates the TX phase. bit0: A phase; bit1: B phase; bit2: C phase. If the bit is set to
 *         1, the bit is valid. If the bit is set to 0, the bit is invalid.
 *         The maximum value is 7, indicating that the three phases are all specified. The minimum value is 0,
 *         indicating that the three phases are not specified.
 *         The sending modes end_mode and ext_mdm_fast_send_mode are invalid if the value is greater than
 *         #EXT_MAC_FAST_SEND_PREEMPT.
 *         data_length indicates the length of the delivered data, and the valid range is (0, 8].
 *
 * @retval   ret Type #td_u32
 *              @li #EXT_ERR_SUCCESS: The startup is successful.
 *              @li #EXT_ERR_FAILURE: The startup fails.
 *              @li #EXT_ERR_INVALID_PARAMETER: Invalid parameter.
 * @par Dependency:
 *           @li soc_mdm_fast_send.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_fast_send_sack_on_plc(EXT_CONST ext_mdm_bb_plc_send_para *mac_bb_plc_send_para);

/**
 * @ingroup network
 * @brief Allows users to register a callback function for FC fast sending.
 *
 * @par  Description:
 *           Allows users to register a callback function for FC fast sending.
 *
 * @attention None
 * @param  cb [IN] Type #ext_mdm_fast_send_rcv_call_back Registration callback function delivered by the user
 * The content of osa is invalid if the FC sending policy is used.
 *
 * @retval   ret Type #td_u32
 *              @li #EXT_ERR_SUCCESS: The startup is successful.
 *              @li #EXT_ERR_INVALID_PARAMETER: Invalid parameter.
 * @par Dependency:
 *           @li soc_mdm_fast_send.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_register_fast_send_fc_callback(ext_mdm_fast_send_rcv_call_back cb);
#ifdef __cplusplus
}
#endif

#endif