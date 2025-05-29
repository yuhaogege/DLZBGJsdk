/*
 * Copyright (c) CompanyNameMagicTag 2020-2021. All rights reserved.
 * Description: APIs for the secondary development interfaces related to the PLC secure communication.
 * Note: Change History
 *     Version              Date         DefectNum           Description
 *     V100R001C00SPC016    2020-12-28   NA                  NA
 */

/** @defgroup network  Channel
 * @ingroup plc
 */
#ifndef SOC_MDM_PLC_SECURITY_H
#define SOC_MDM_PLC_SECURITY_H

#include <soc_types.h>
#include <soc_mdm_topo.h>
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup network
 * @brief  Indicates the PSK length. (The value is fixed and cannot be changed.)
 */
#define SECURITY_PSK_VALUE_LEN 32

/**
 * @ingroup network
 * @brief  Indicates the PSK_ID length. (The value is fixed and cannot be changed.)
 */
#define SECURITY_PSK_ID_LEN 15

/**
 * @ingroup network
 * @brief  Frequency band change reason
 */
typedef enum {
    NEGO_PSK_SUCCESSS = 0, /**< The negotiation is successful, and the STA can associate with the network. */
    NEGO_NOT_IN_WHITELIST = 1, /**< The STA is not in the CCO truelist. */
    NEGO_WRONG_PSK_OR_PSK_ID = 2, /**< The PSK or PSK ID of the STA is incorrect. */
    NEGO_GET_NEK_SUCCESS = 3, /**< The STA successfully obtains the NEK. */
    NEGO_UPDATE_NEK_BY_CCO = 4, /**< Periodically updating the NEK by CCO */

    NEGO_REPLY_NEK_GRANT = 20, /**< NEK certification passed */
    NEGO_REPLY_NEK_REFUSE = 21, /**< NEK certification failed */
    NEGO_REPLY_NEK_UNSUPPORT = 22, /**<  Unsupported NEK */
    NEGO_RCV_HELLO_REQUEST = 23, /**< The CCO receives the STA negotiation request. */
    NEGO_AUTH_STA_PSK_ID_ERROR = 24, /**< The CCO fails to verify the STA PSK ID. */
    NEGO_AUTH_STA_PSK_ERROR = 25, /**< The CCO fails to verify the STA PSK */
    NEGO_AUTH_PEER_MISSING = 26, /**< No valid peer information exists
                                    (the PSK or PSK ID will fail to be authenticated). */
} ext_mdm_negotiation_status;
/**
 * @ingroup network
 * @brief  MAC event information at the STA end: Security negotiation failed info.
 */
typedef struct {
    ext_mdm_negotiation_status status; /**< Refer to #ext_mdm_negotiation_status */
    td_u32 snid; /**< SNID of the network where security negotiation fails. */
    td_u32 pad[4]; /* Pad 4 */
} ext_mac_sta_event_negotiation_report;
/**
 * @ingroup network
 * @brief  MAC event information at the STA end: Security negotiation failed info.
 */
typedef struct {
    ext_mdm_negotiation_status status; /**< Refer to #ext_mdm_negotiation_status */
    td_u8 sta_mac[EXT_PLC_MAC_ADDR_LEN];
    td_u16 pad;
} ext_mac_cco_event_negotiation_report;
/**
 * @ingroup network
 * @brief  Registers the function that obtains PSK information.
 *
 * @par  Description:
 *          Registers the function that obtains PSK information.
 *
 * @attention This callback function is called only in the 1901 protocol, instead of in other protocols.
 * @param  p_psk [OUT] Type #td_u8** Pointer to the PSK address. The memory space is maintained by the application
 *         layer. The PSK length is fixed at #SECURITY_PSK_VALUE_LEN, which cannot be changed.
 * @param  p_psk_id [OUT] Type #td_u8** : Pointer to the PSK_ID address. The memory space is maintained by the
 *         application layer. The PSK_ID length is fixed at #SECURITY_PSK_ID_LEN, which cannot be changed.
 *
 * @retval  None
 * @par Dependency:
 *           @li soc_mdm_plc_security.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
typedef td_void (*ext_get_psk_info)(td_u8 **p_psk, td_u8 **p_psk_id);

/**
 * @ingroup network
 * @brief Registers the function that obtains PSK information.
 *
 * @par  Description:
 *        Registers the function that obtains PSK information.
 *
 * @attention The registration API does not distinguish protocol versions, but the registered callback function is used
 *            only in the 1901 protocol.Other protocols are not used and must be called before uapi_security_init_p1901.
 * @param  get_psk_info [IN] Type #ext_get_psk_info Registration callback function delivered by the user
 * @retval   None
 * @par Dependency:
 *           @li soc_mdm_plc_security.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_register_get_psk_info_func(ext_get_psk_info get_psk_info);

/**
 * @ingroup network
 * @brief  Initializes the security function of the 1901 protocol.
 *
 * @par  Description:
 *         Initializes the security function of the 1901 protocol.
 *
 * @attention None
 * @param  None
 *
 * @retval   None
 * @par Dependency:
 *           @li soc_mdm_plc_security.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_security_init_p1901(td_void);

/**
 * @ingroup network
 * @brief  Obtain the format of the callback function of the PSK by using the PSK ID.
 *
 * @par  Description:
 *         Obtain the format of the callback function of the PSK by using the PSK ID.
 *
 * @attention This callback function is called only in the 1901 protocol, instead of in other protocols.
 * @param  psk [OUT] Type #td_u8** Pointer to the PSK address. The memory space is maintained by the application
 *         layer. The PSK length is fixed at #SECURITY_PSK_VALUE_LEN, which cannot be changed.
 * @param  psk_len [OUT] Type #td_u8* : #psk length. The value is fixed at #SECURITY_PSK_VALUE_LEN, which
 *         cannot be changed.
 * @param  psk_id [IN] Type #td_u8* : PSK ID sent by the STA.
 * @param  psk_id_len [IN] Type #td_u8* : PSK ID length.
 * @retval  #EXT_ERR_SUCCESS: Succeeded in obtaining PSK information by PSK ID.
 * @retval  #EXT_ERR_FAILURE: Failed to obtain PSK information based on the PSK ID.
 * @par Dependency:
 *           @li soc_mdm_plc_security.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
typedef td_u32 (*ext_get_psk_info_by_psk_id)(td_u8 **psk, td_u16 *psk_len, const td_u8 *psk_id,
    const td_u32 psk_id_len);
/**
 * @ingroup network
 * @brief Registers the function that obtains PSK information by PSK ID.
 *
 * @par  Description:
 *        Registers the function that obtains PSK information by PSK ID.
 * @attention
 *           @li The product type is CCO.
 *           @li The registration API does not distinguish protocol versions, but the registered callback function
 *           is used only in the 1901 protocol. Other protocols are not used and must be called before
 *           uapi_security_init_p1901.
 * @param  cb [IN] Type #ext_get_psk_info_by_psk_id Registration callback function delivered by the user
 * @retval   None
 * @par Dependency:
 *           @li soc_mdm_plc_security.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_register_get_psk_info_by_psk_id_func(ext_get_psk_info_by_psk_id cb);

/**
 * @ingroup network
 * @brief Set the delay for the STA to send key exchange packets.
 * @par  Description:
 * Set the delay for the STA to send key exchange packets.
 * @attention The product type is STA. #delay_time ranges in [0, 6000], unit : ms.
 * @retval #EXT_ERR_SUCCESS.If #delay_time exceeds 6000, the value is automatically set to 6000.
 * @par Dependency:
 * @li soc_mdm_plc_security.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_mdm_set_delay_key_exchange_time(td_u32 delay_time);
#ifdef __cplusplus
}
#endif
#endif