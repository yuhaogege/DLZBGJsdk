/*
 * Copyright (c) CompanyNameMagicTag 2012-2018. All rights reserved.
 * Description: secondary development APIs of the PLC PHY
 * Author: CompanyName
 * Create: 2012-07-16
 * Add interface of setting tonemask table
 * Add interface of getting tonemask table
 */


#ifndef __SOC_MDM_PHY_H__
#define __SOC_MDM_PHY_H__
#include <soc_types_rom.h>
#if defined PRODUCT_CFG_NDM_COLLECT_DATA
#include <drv_collectdata.h>
#endif
/**
 * @ingroup phy
 * @brief  Frame type definition: beacon frame
 */
#define BEACON_DT 0

/**
 * @ingroup phy
 * @brief  Frame type definition: SOF frame
 */
#define SOF_DT 1
/**
 * @ingroup phy
 * @brief  Frame type definition: SACK frame
 */
#define SACK_DT 2
/**
 * @ingroup phy
 * @brief  Frame type definition: Multi-network coordination frame
 */
#define COORDINATE_DT 3
/**
 * @ingroup phy
 * @brief  Frame type definition: RTS_CTS frame under the IEEE 1901 protocol
 */
#define RTS_CTS_DT_P1901 4
/**
 * @ingroup phy
 * @brief  Frame type definition: Service frame under the IEEE 1901 protocol
 */
#define SERVICE_DT_P1901 5
/**
 * @ingroup phy
 * @brief  Frame type definition: Error frame
 */
#define ERR_DT 7
/**
 * @ingroup phy
 * @brief  Frame type definition: Maximum value for the frame type
 */
#define MAX_DT 8

/**
 * @ingroup phy
 * @brief  Forwarded packet from reassembly
 */
#define RETRANSMIT_TYPE 0

/**
 * @ingroup phy
 * @brief  Packet from the IP layer
 */
#define IP4_TYPE 1

/**
 * @ingroup phy
 * @brief  Packet from the local network management task
 */
#define NETWORK_TYPE 2

/**
 * @ingroup phy
 * @brief  Packet from the app layer
 */
#define APP_TYPE 3

/**
 * @ingroup phy
 * @brief  Packet from the Ethernet port
 */
#define ETH_TYPE 4

/**
 * @ingroup phy
 * @brief  Packet from the test module
 */
#define TEST_TYPE 5

/**
 * @ingroup phy
 * @brief  Unknown packet
 */
#define UNKNOWN_TYPE 0xFFFF

/**
 * @ingroup phy
 * @brief  Sets the analog TX power of the current module.
 *
 * @par  Description:
 * Sets the analog TX power of the current module.
 *
 * @attention This parameter takes effect in real time and is restored to the default value after power-off.
 * @param  afe_pga_gain    [IN] Type #td_u8, analog TX power of the current module. The maximum value is 10,
 * the minimum value is 0, and the unit is 2 dB.
 *
 * @retval #EXT_ERR_SUCCESS Setting succeeded.
 * @retval #EXT_ERR_FAILURE Setting failed.
 * @retval Other values.      For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_phy.h: This file describes the external APIs of the PLC physical layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_set_afe_pga_gain(td_u8 afe_pga_gain);

/**
 * @ingroup phy
 * @brief  Obtains the analog TX power of the current module.
 *
 * @par  Description:
 * Obtains the analog TX power of the current module.
 *
 * @attention None
 * @param  None
 *
 * @retval  Value, analog TX power of the current module. The maximum value is 10 and the minimum value is 0.
 * @par Dependency:
 * @li soc_mdm_phy.h: This file describes the external APIs of the PLC physical layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u8 uapi_get_afe_pga_gain(td_void);

/**
 * @ingroup phy
 * @brief  Sets the digital TX power of the current module.
 *
 * @par  Description:
 * Sets the digital TX power of the current module.
 *
 * @attention This parameter takes effect in real time and is restored to the default value after power-off.
 * @param power_step [IN] Type #td_s8, digital TX power of the current module. The maximum value is 26, the minimum
 * value is -40, and the unit is 1 dB.
 *
 * @retval #EXT_ERR_SUCCESS Setting succeeded.
 * @retval #EXT_ERR_INVALID_PARAMETER The parameter is invalid.
 * @par Dependency:
 * @li soc_mdm_phy.h: This file describes the external APIs of the PLC physical layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_set_power_step(td_s8 power_step);

/**
 * @ingroup phy
 * @brief  Obtains the digital TX power of the current module.
 *
 * @par  Description:
 * Obtains the digital TX power of the current module.
 *
 * @attention None
 * @param  None
 *
 * @retval  Value, digital TX power of the current module. The maximum value is 26 and the minimum value is -40.
 * @par Dependency:
 * @li soc_mdm_phy.h: This file describes the external APIs of the PLC physical layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_s8 uapi_get_power_step(td_void);

/**
 * @ingroup phy
 * @brief   Obtains the NTB clock value.
 *
 * @par  Description:
 * Obtains the NTB clock value.
 *
 * @attention None
 * @param  is_sync [IN/ OUT] Type #td_bool* , whether the synchronization is successful.
 *
 * @retval Non-0 values     When the synchronization is successful, it indicates the current NTB clock value.
 * @retval 0       The synchronization is not successful or the input parameter is invalid or not supported.
 * @par Dependency:
 * @li soc_mdm_phy.h: This file describes the external APIs of the PLC physical layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_get_ntb(td_bool *is_sync);

/**
 * @ingroup phy
 * @brief  NTB synchronization mode
 */
typedef enum {
    NTB_NORMAL_MODE,   /**< NTB synchronization mode is required. */
    NTB_MAIN_CCO_MODE, /**< NTB synchronization mode is not required. */
} ext_ntb_sync_mode;

/**
 * @ingroup phy
 * @brief  FC length
 */
#define PHY_FC_SIZE 16

/**
 * @ingroup phy
 * @brief  Interrupt type
 */
typedef enum {
    PHY_BURST_SEND_END_INTERRUPT = (1 << 0), /**< Burst TX completion interrupt */
    PHY_BURST_RCV_END_INTERRUPT = (1 << 1),  /**< Burst RX completion interrupt */
    PHY_FC_OK_INTERRUPT = (1 << 2),          /**< FC detection success interrupt, left shift 2 bits */
    PHY_FC_ERR_INTERRUPT = (1 << 3),         /**< FC detection error interrupt, left shift 3 bits */
    PHY_PB_INTERRUPT = (1 << 4),             /**< PB decode completion interrupt, left shift 4 bits */
    PHY_SPC_RCV_INTERRUPT = (1 << 5),        /**< SPC RX interrupt, left shift 5 bits */
    PHY_SPC_SEND_INTERRUPT = (1 << 6),       /**< SPC TX interrupt, left shift 6 bits */
    PHY_STATE4_INTERRUPT = (1 << 8),         /**< Carrier sense STATE4 interrupt, left shift 8 bits */
    PHY_STATE3_INTERRUPT = (1 << 9),         /**< Carrier sense STATE3 interrupt, left shift 9 bits */
    PHY_STATE2_INTERRUPT = (1 << 10),        /**< Carrier sense STATE2 interrupt, left shift 10 bits */
    PHY_PARA_OK_INTERRUPT = (1 << 11),       /**< Parallel detection success interrupt, left shift 11 bits */
    PHY_NBI_POW_INTERRUPT = (1 << 12),       /**< NBI power statistics report interrupt, left shift 12 bits */
    PHY_NBI_TABLE_INTERRUPT = (1 << 13),     /**< NBI frequency domain table report interrupt, left shift 13 bits */
    PHY_ISP_SEND_END_INTERRUPT = (1 << 14),  /**< ISP TX completion interrupt, left shift 14 bits */
    PHY_FRAME_SINGAL_SEND_START_INTERRUPT = (1 << 15), /**< TX start interrupt, left shift 15 bits */
    PHY_ONLY_PREAMBLE_RCV_INTERRUPT = (1 << 16), /**< left shift 16 bits, Preamble RX interrupt when only the preamble
                                                      is sent */
    PHY_ONLYPREAMBLE_SEND_END_INTERRUPT = (1 << 17), /**< left 17 bits, Preamble TX completion interrupt only the
                                                          preamble is sent */
    PHY_RX_OVERFLOW_INTERRUPT = (1 << 18),           /**< RX write overflow interrupt, left shift 18 bits */
    PHY_SEND_WR_COLLISION_INTERRUPT = (1 << 19)      /**< TX write conflict interrupt, left shift 19 bits */
} ext_mdm_phy_irq_type;

/**
 * @brief   Reads the average SNR.
 *
 * @par  Description:
 * Reads the average SNR.
 *
 * @attention None
 * @param  None
 *
 * @retval #td_s8 Average SNR. Value range: -128 to +127
 * @par Dependency:
 * @li soc_mdm_phy.h: This file describes the external APIs of the PLC physical layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_s8 uapi_get_avg_snr(td_void);

/**
 * @ingroup phy
 * @brief    Reads the channel attenuation.
 *
 * @par  Description:
 * Reads the channel attenuation.
 *
 * @attention None
 * @param  None
 *
 * @retval #td_u8 Channel attenuation. Value range: 0 to 127
 * @par Dependency:
 * @li soc_mdm_phy.h: This file describes the external APIs of the PLC physical layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u8 uapi_get_channel_attenuation(td_void);

/**
 * @ingroup phy
 * @brief  State grid mode
 */
#define PHY_DETECT_VER_SG 0

/**
 * @ingroup phy
 * @brief  1901.1 mode
 */
#define PHY_DETECT_VER_1901_1 1
/**
 * @ingroup phy
 * @brief  NBI report table size
 */
#define PHY_NBI_TABLE_SIZE 8

/**
 * @ingroup phy
 * @brief    Initializes the PHY.
 *
 * @par  Description:
 * Initializes the PHY.
 *
 * @attention None
 * @param  None
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_phy.h: This file describes the external APIs of the PLC physical layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_phy_init(td_void);

/**
 * @ingroup phy
 * All phases
 */
#define PHASE_ALL 0

/**
 * @ingroup phy
 * Phase A
 */
#define PHASE_A 1

/**
 * @ingroup phy
 * Phase B
 */
#define PHASE_B 2

/**
 * @ingroup phy
 * Phase C
 */
#define PHASE_C 3

/**
 * @ingroup phy
 * Invalid phase
 */
#define INVALID_PHASE 4

/**
 * @ingroup phy
 * Maximum number of phases
 */
#define PHASE_CNT 3

/**
 * @ingroup phy
 * Multi-phase control mode: phase disabled
 */
#define PHASE_MODE_OFF 0

/**
 * @ingroup phy
 * Multi-phase control mode: phase enabled
 */
#define PHASE_MODE_ON 1

/**
 * @ingroup phy
 * @brief    Initializes the 3-phase GPIO.
 *
 * @par  Description:
 * Initializes the 3-phase GPIO.
 *
 * @attention None
 * @param  None
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_phy.h: This file describes the external APIs of the PLC physical layer.
 * @see None
 * @since DW21_V100R001C00
 */
typedef td_void (*phs_gpio_init_func)(td_void);

/**
 * @ingroup phy
 * @brief    Sets the phase switch.
 *
 * @par  Description:
 * Sets the phase switch.
 *
 * @attention None
 * @param  phase_name     [IN] Type #td_u32, phase name, all phases: #PHASE_ALL, phase A: #PHASE_A, phase B: #PHASE_B,
 * phase C: #PHASE_C
 * @param  phase_mode     [IN] Type #td_u8, phase switch on: #PHASE_MODE_ON, phase switch off: #PHASE_MODE_OFF
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_phy.h: This file describes the external APIs of the PLC physical layer.
 * @see None
 * @since DW21_V100R001C00
 */
typedef td_void (*phs_set_single_phase_func)(td_u32 phase_name, td_u8 phase_mode);

/**
 * @ingroup phy
 * @brief    Registers the 3-phase switch initialization and control API.
 *
 * @par  Description:
 * Registers the 3-phase switch initialization and control API.
 *
 * @attention Set_tx_single_phase and set_rx_single_phase should be resident functions. You need to add #__hot before
 * the function definition.
 * @param  phase_gpio_init         [IN] Type #phs_gpio_init_func, 3-phase switch initialization function
 * @param  set_tx_single_phase     [IN] Type #phs_set_single_phase_func, 3-phase setting TX switch function
 * @param  set_rx_single_phase     [IN] Type #phs_set_single_phase_func, 3-phase setting RX switch function
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_phy.h: This file describes the external APIs of the PLC physical layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_register_phase_func(phs_gpio_init_func phase_gpio_init, phs_set_single_phase_func set_tx_single_phase,
    phs_set_single_phase_func set_rx_single_phase);

/**
 * @ingroup phy
 * @brief    Registers the LED processing packet TX and RX completion function.
 *
 * @par  Description:
 * Registers the LED processing packet TX and RX completion function.
 *
 * @attention None
 * @param  dt     [IN] Type #td_u8, frame type, for example, beacon frame: #BEACON_DT, SOF frame: #SOF_DT
 * @param  source_type     [IN] Type #td_u16, SOF frame packet source (valid only when dt is set to SOF_DT),
 * for example, coming from the app: #APP_TYPE, except #RETRANSMIT_TYPE.
 * @param  phase     [IN] Type #td_u8, packet TX/RX phase, all phases: #PHASE_ALL, phase A: #PHASE_A, phase B: #PHASE_B,
 * phase C: #PHASE_C
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_phy.h: This file describes the external APIs of the PLC physical layer.
 * @see None
 * @since DW21_V100R001C00
 */
typedef td_void (*led_handle_frame_rcv_send_end)(td_u8 dt, td_u16 source_type, td_u8 phase);

/**
 * @ingroup phy
 * @brief    Registers the LED processing packet TX completion API.
 *
 * @par  Description:
 * Registers the LED processing packet TX completion API.
 *
 * @attention None
 * @param  led_handle_frame_send_end     [IN] Type #led_handle_frame_rcv_send_end, LED processing packet TX
 * completion function
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_phy.h: This file describes the external APIs of the PLC physical layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_register_led_handle_frame_send_end_func(led_handle_frame_rcv_send_end led_handle_frame_send_end);

/**
 * @ingroup phy
 * @brief    Registers the LED processing packet RX completion function.
 *
 * @par  Description:
 * Registers the LED processing packet RX completion function.
 *
 * @attention Only the beacon frame and SOF frame processing RX completion is supported.
 * @param  led_handle_frame_rcv_end     [IN] Type #led_handle_frame_rcv_send_end, LED processing packet RX
 * completion function
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_phy.h: This file describes the external APIs of the PLC physical layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_register_led_handle_frame_rcv_end_func(led_handle_frame_rcv_send_end led_handle_frame_rcv_end);

/**
 * @ingroup phy
 * @brief   External LD switch control callback function.
 *
 * @par  Description:
 * External LD switch control callback function.
 *
 * @attention This function is a resident function. You need to add #__hot before the function definition.
 * @param  is_enable     [IN] Type #td_bool,Whether to enable external LD,Function enable: #TD_TRUE
 * Function disable: #TD_FALSE.
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_phy.h: This file describes the external APIs of the PLC physical layer.
 * @see None
 * @since DW21_V100R001C00
 */
typedef td_void (*drv_line_driver_swicth)(td_bool is_enable);

/**
 * @ingroup phy
 * @brief    Registers the external LD switch control callback function.
 *
 * @par  Description:
 * Registers the external LD switch control callback function.
 *
 * @attention None
 * @param  func     [IN] Type #drv_line_driver_swicth,the external LD switch control callback function,Unregister:
 * #TD_NULL.
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_phy.h: This file describes the external APIs of the PLC physical layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_register_drv_ld_switch(drv_line_driver_swicth func);
/**
 * @ingroup network
 * @brief  tonemask table length.
 */
#define EXT_MDM_TONE_MASK_LEN 8
/**
 * @ingroup network
 * @brief  tonemask information.
 */
typedef struct {
    td_u8 frequency; /**< Frequency band number */
    td_u8 rsvd[3];   /**< Reserved 3 bytes */

    td_u32 tone_mask[EXT_MDM_TONE_MASK_LEN]; /**< tonemask table */
} ext_mdm_set_user_tonemask;
/**
 * @ingroup network
 * @brief Get the tonemask configuration of the specified band
 *
 * @par  Description:
 * Get the tonemask configuration of the specified band.Returns the default value if the
 * tonemask has not been configured using the secondary development interface.
 *
 * @param  freq[IN] #td_u8,frequency num ,range:[1,8].
 * @param  tone_mask_input[IN] #ext_mdm_set_user_tonemask *, tonemask information.
 * @retval ret Type #td_u32,
 * @li EXT_ERR_INVALID_PARAMETER: wrong input parameters.
 * @li EXT_ERR_FAILURE : fail to get tonemask information.
 * @li EXT_ERR_SUCCESS : get tonemask information successfully.
 * @par Dependency:
 * @li soc_mdm_phy.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_mdm_get_tonemask_by_freq(td_u8 freq, ext_mdm_set_user_tonemask *tone_mask_input);
/**
 * @ingroup network
 * @brief Set the tonemask configuration of the specified band.
 *
 * @par  Description:
 * Set the tonemask configuration of the specified band.
 *
 * @attention Called before uapi_phy_init().Supports up to 3 frequency band slots.
 * @attention This interface needs to be used with the frequency band configuration file.
 * Tone mask setting value and frequency band configuration file need to find FAE to give
 * reference value, can not be arbitrarily configured.
 * @param  tone_mask_input[IN] #ext_mdm_set_user_tonemask *, tonemask information,
 * @li frequency : frequency number to be configured.
 * @li tone_mask : tonemask table value.
 * @retval ret Type #td_u32,
 * @li EXT_ERR_INVALID_PARAMETER: wrong input parameters.
 * @li EXT_ERR_NOT_SUPPORT : not support to config emc tonemask(it was setted from NV).
 * @li EXT_ERR_SUCCESS : get tonemask information successfully.
 * @li EXT_ERR_FULL :  the number of frequency bands that currently supports configuration is 3.
 * @par Dependency:
 * @li soc_mdm_phy.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_mdm_set_tonemask(EXT_CONST ext_mdm_set_user_tonemask *tone_mask_input);
/**
 * @ingroup network
 * @brief Set the powerloading
 *
 * @par  Description:
 * Set the powerloading.
 *
 * @param  start[IN] #td_u16, Subcarrier start number, range:[0,0xFF],
 * @param  end[IN] #td_u16, End number of subcarrier, range:[start,0xFF],
 * @param  value[IN] #td_u16, digital power value.[0, 0x1F];
 * @retval ret Type #td_u32,
 * @li EXT_ERR_INVALID_PARAMETER: wrong input parameters.
 * @li EXT_ERR_SUCCESS : set successfully.
 * @par Dependency:
 * @li soc_mdm_phy.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_set_powerloading(td_u16 start, td_u16 end, td_u16 value);

/**
 * @ingroup network
 * @brief  TONEMASK mode,corresponds to the 2.5M~5.7M of the 1901.1.
 */
#define TONE_MASK_MODE_2 0x01

/**
 * @ingroup network
 * @brief Sets the ToneMask mode.
 *
 * @par  Description:
 * Sets the ToneMask mode.
 *
 * @attention None.
 * @param  mode  [IN] Type #td_u8
 * @li #TONE_MASK_MODE_2: 1901.1 2.5-5.7m.
 * @retval None.
 * @par Dependency:
 * @li soc_mdm_phy.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_mdm_set_tone_mask_mode(td_u8 mode);
#if defined PRODUCT_CFG_NDM_COLLECT_DATA
td_u32 uapi_start_send_dc_signal(td_u8 *buf, td_u32 buf_size, dc_afe_backup *afe_backup);
td_void uapi_stop_send_dc_signal(dc_afe_backup *afe_backup);
#endif

#endif
