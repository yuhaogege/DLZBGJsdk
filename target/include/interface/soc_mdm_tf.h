/*
 * Copyright (c) CompanyNameMagicTag 2020-2021. All rights reserved.
 * Description: APIs for the secondary development interfaces related to the transformer district.
 * Note: Change History
 *     Version              Date         DefectNum           Description
 *     V100R001C00SPC016    2020-12-28   NA                  NA
 */


/** @defgroup network  Channel
 * @ingroup plc
 */
#ifndef SOC_MDM_TF_H
#define SOC_MDM_TF_H

#include <soc_types.h>
#include <soc_mdm_topo.h>
#ifdef __cplusplus
extern "C" {
#endif

 /**
 * @ingroup network
 * @brief  Loads the transformer district identification component.
 *
 * @par  Description:
 *         Loads the transformer district identification component to enable the transformer district identification
 *         function.
 *
 * @attention The product type is STA.
 *         @li This API can be called only after the component is initialized,that is,after uapi_mac_cp_init is called.
 * @param   None
 *
 * @retval  None
 * @par Dependency:
 *           @li soc_mdm_tf.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_load_tf_snr_component(td_void);
/**
 * @ingroup network
 * @brief  Sets the switch of transformer district identification.
 */
typedef enum {
    TF_OFF = 0,                         /**< Disables transformer district identification. */
    TF_ON_WITH_STA_JOIN_BY_TF_RESULT,   /**< Enables transformer district identification. Stations are added to the
                                         * network based on the identification result.
                                         */
    TF_MODE_INVALID,                    /**< Invalid value */
} ext_tf_mode;
/**
 * @ingroup network
 * Output structure of the software-based transformer district result
 */
typedef struct {
    td_bool is_tf_complete;     /**< Whether the identification is completed */
    td_bool is_belong_join_net; /**< Whether it belongs to or joins the transformer district */
    td_u16 requery_time;        /**< Recommended wait time for the query (unit: s) */
} ext_mdm_snr_tf_result;

/**
 * @ingroup  transformer_district
 * @brief  Sets the switch of transformer district identification.
 *
 * @par  Description:
 *         Sets the switch of transformer district identification.
 *
 * @attention The product type is STA.
 * @param  tf_mode [IN] Type #ext_tf_mode Software-based transformer district identification
 *
 * @retval #EXT_ERR_SUCCESS The message indicating the successful setting is sent.
 * @retval Other values The message indicating the setting failure is sent. For details, see soc_errno.h.
 * @par Dependency:
 *           @li soc_mdm_tf.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_set_tf_mode(ext_tf_mode tf_mode);

/**
 * @ingroup  transformer_district
 * @brief  Queries the current switch status of transformer district identification.
 *
 * @par  Description:
 *         Queries the current switch status of transformer district identification.
 *
 * @attention The product type is STA.
 * @param  None
 *
 * @retval ret type #ext_tf_mode Current  switch status of transformer district identification
 * @par Dependency:
 *           @li soc_mdm_tf.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
ext_tf_mode uapi_get_tf_mode(td_void);

/**
 * @ingroup network
 * @brief  Sets the home network of the station transformer district.
 *
 * @par  Description:
 *         Sets the home network of the station transformer district.
 *         After the setting is successful, if the transformer district identification function is enabled, the station
 *         preferentially uses the configured home network as the transformer district identification result.
 *         The home network information is stored in the NV item of the factory partition, which does not disappear even
 *         during power-off and upgrade.
 *
 * @attention The product type is STA.
 * @param   belong_cco_mac [IN] Type #td_u8 MAC address of the CCO on the home network
 *
 * @retval  #EXT_ERR_SUCCESS The setting message is successfully sent.
 * @retval  Other values indicate that the setting message fails to be sent. For details, see soc_errno.h.
 * @par Dependency:
             @li soc_mdm_tf.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_set_belong_network(EXT_CONST td_u8 belong_cco_mac[EXT_PLC_MAC_ADDR_LEN]);

/**
 * @ingroup network
 * Home network settings
 */
typedef struct {
    td_u8 belong_status; /**< Home network setting status. For details, see #ext_belong_network_set_status. */
    td_u8 pad;
    td_u8 belong_cco_mac[EXT_PLC_MAC_ADDR_LEN]; /**< Set MAC address of the CCO on the home network */
} ext_belong_network_set_info;

/**
 * @ingroup network
 * Home network setting status
 */
typedef enum {
    EXT_NOT_SET_BELONG_NETWORK = 0, /**< The home network is not set. */
    EXT_SET_BELONG_CCO_MAC,         /**< Sets a MAC address for the CCO on the home network. */
} ext_belong_network_set_status;

/**
 * @ingroup network
 * @brief  Queries the currently set home network.
 *
 * @par  Description:
 *         Queries the currently set home network.
 *
 * @attention The product type is STA.
 * @param   belong_info [OUT] Type #ext_belong_network_set_info* Output of the home network settings
 *
 * @retval  #EXT_ERR_SUCCESS Success
 * @retval  #EXT_ERR_FAILURE Failure
 * @par Dependency:
 *           @li soc_mdm_tf.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_get_belong_network_set_info(ext_belong_network_set_info *belong_info);

/**
 * @ingroup network
 * @brief  Clears the home network settings.
 *
 * @par  Description:
 *         Clears the home network settings.
 *
 * @attention The product type is STA.
 * @param   None
 *
 * @retval  #EXT_ERR_SUCCESS The setting message is successfully sent.
 * @retval  Other values indicate that the setting message fails to be sent. For details, see soc_errno.h.
 * @par Dependency:
             @li soc_mdm_tf.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_clear_belong_network(td_void);

#ifdef __cplusplus
}
#endif
#endif