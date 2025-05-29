/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * @defgroup hct_io IO
 * @ingroup drivers
 */

#ifndef __SOC_MDM_IO_H__
#define __SOC_MDM_IO_H__

#include <soc_types_rom.h>
#include <soc_mdm_types_rom.h>

/**
 * @ingroup hct_io
 *
 * GPIO ID
 * GPIO 1-31 is used in the register.
 * DW21E uses the GPIO 1锟�22, anDWDw21S uses GPIO 0锟�21.
 * To be compatible with the two SoCs, GPIO 0锟�22 is defined in the software API.
 */
typedef enum {
    EXT_GPIO_IDX_0,   /**< GPIO0 */
    EXT_GPIO_IDX_1,   /**< GPIO1 */
    EXT_GPIO_IDX_2,   /**< GPIO2 */
    EXT_GPIO_IDX_3,   /**< GPIO3 */
    EXT_GPIO_IDX_4,   /**< GPIO4 */
    EXT_GPIO_IDX_5,   /**< GPIO5 */
    EXT_GPIO_IDX_6,   /**< GPIO6 */
    EXT_GPIO_IDX_7,   /**< GPIO7 */
    EXT_GPIO_IDX_8,   /**< GPIO8 */
    EXT_GPIO_IDX_9,   /**< GPIO9 */
    EXT_GPIO_IDX_10,  /**< GPIO10 */
    EXT_GPIO_IDX_11,  /**< GPIO11 */
    EXT_GPIO_IDX_12,  /**< GPIO12 */
    EXT_GPIO_IDX_13,  /**< GPIO13 */
    EXT_GPIO_IDX_14,  /**< GPIO14 */
    EXT_GPIO_IDX_15,  /**< GPIO15 */
    EXT_GPIO_IDX_16,  /**< GPIO16 */
    EXT_GPIO_IDX_17,  /**< GPIO17 */
    EXT_GPIO_IDX_18,  /**< GPIO18 */
    EXT_GPIO_IDX_19,  /**< GPIO19 */
    EXT_GPIO_IDX_20,  /**< GPIO20 */
    EXT_GPIO_IDX_21,  /**< GPIO21 */
    EXT_GPIO_IDX_22,  /**< GPIO22 */
    EXT_GPIO_IDX_MAX, /**< Maximum value, which cannot be used */
} ext_gpio_idx_e;

/**
 * @ingroup hct_io
 *
 * I/O level
 */
typedef enum {
    EXT_GPIO_VALUE0 = 0, /**< Low level */
    EXT_GPIO_VALUE1      /**< High level */
} ext_gpio_value_e;

/**
 * @ingroup hct_io
 *
 * I/O direction
 */
typedef enum {
    EXT_GPIO_DIRECTION_IN = 0, /**< Input */
    EXT_GPIO_DIRECTION_OUT     /**< Output */
} ext_gpio_direction_e;

/**
 * @ingroup hct_io
 *
 * I/O interrupt trigger mode. This bit is used with EXT_GPIO_INT_POLARITY_E.
 */
typedef enum {
    EXT_INT_LEVEL = 0, /**< The interrupt is triggered in level-sensitive mode. */
    EXT_INT_EDGE       /**< Interrupt triggered at the rising edge or falling edge */
} ext_gpio_int_e;

/**
 * @ingroup hct_io
 *
 * I/O interrupt polarity. This pin is used with EXT_GPIO_INT_E.
 */
typedef enum {
    EXT_EDGE_FALL_LEVEL_LOW = 0, /**< Interrupt triggered at low level or falling edge */
    EXT_EDGE_RISE_LEVEL_HIGH     /**< Interrupt triggered at high level or rising edge */
} ext_gpio_int_polarity_e;

/**
 * @ingroup hct_io
 *
 * Enumeration of the I/O control register. The enumerated value corresponds to the offset address. For details, see the
 * SoC data sheet. To be compatible with the two chips, the control registers of the two chips are written together.
 * This API is used to set the I/O pin drive strength and enable pull-up.
 */
typedef enum {
    EXT_IO_CTL_RSTN = 0x940,
    EXT_IO_CTL_SEL = 0x91C,
    EXT_IO_CTL_SFC_WPN = 0x860,   /**< Flash write protection pin */
    EXT_IO_CTL_SFC_HOLDN = 0x864, /**< Flash Hold pin */
    EXT_IO_CTL_SFC_CLK = 0x920,   /**< Flash CLK pin */
    EXT_IO_CTL_SFC_CSN = 0x924,   /**< Flash CSN pin */
    EXT_IO_CTL_SFC_DI = 0x928,    /**< Flash DI pin, input relative to the SoC */
    EXT_IO_CTL_SFC_DO = 0x92C,    /**< Flash DO pin, output relative to the SoC */
    EXT_IO_CTL_GPIO_0 = 0x800,
    EXT_IO_CTL_GPIO_1 = 0x804,
    EXT_IO_CTL_GPIO_2 = 0x808,
    EXT_IO_CTL_GPIO_3 = 0x80C,
    EXT_IO_CTL_GPIO_4 = 0x810,
    EXT_IO_CTL_LDRVR_PD = 0x814,
    EXT_IO_CTL_GPIO_6 = 0x930,
    EXT_IO_CTL_GPIO_7 = 0x81C,
    EXT_IO_CTL_GPIO_8 = 0x820,
    EXT_IO_CTL_GPIO_9 = 0x824,
    EXT_IO_CTL_GPIO_10 = 0x828,
    EXT_IO_CTL_GPIO_11 = 0x834,
    EXT_IO_CTL_GPIO_12 = 0x838,
    EXT_IO_CTL_UART1_RXD = 0x83C,
    EXT_IO_CTL_UART1_TXD = 0x840,
    EXT_IO_CTL_GPIO_15 = 0x844,
    EXT_IO_CTL_GPIO_16 = 0x848,
    EXT_IO_CTL_GPIO_17 = 0x84C,
    EXT_IO_CTL_GPIO_18 = 0x850,
    EXT_IO_CTL_GPIO_19 = 0x854,
    EXT_IO_CTL_GPIO_20 = 0x858,
    EXT_IO_CTL_GPIO_21 = 0x85C,
    EXT_IO_CTL_GPIO_22 = 0x884,
} ext_io_ctl_idx_e;

/**
 * @ingroup hct_io
 *
 * GPIO pull-up configuration
 */
typedef enum {
    EXT_GPIO_DEN = 0, /**< Pull-up disabled */
    EXT_GPIO_PU_EN,   /**< Pull-up enabled */
    EXT_GPIO_PU_MAX,
} ext_io_pull_e;

/**
 * @ingroup hct_io
 *
 * I/O drive strength. For details, see the GPIO description in the DW21 V100 SoC Data Sheet.
 */
typedef enum {
    EXT_IO_DRIVER_STRENGTH_0 = 0, /**< Drive strength level 0 (highest) */
    EXT_IO_DRIVER_STRENGTH_1,     /**< Drive strength level 1 */
    EXT_IO_DRIVER_STRENGTH_2,     /**< Drive strength level 2 */
    EXT_IO_DRIVER_STRENGTH_3,     /**< Drive strength level 3 (lowest) (default) */
    EXT_IO_DRIVER_STRENGTH_MAX,
} ext_io_driver_strength_e;

typedef void (*gpio_isr_callback_func)(void);

/**
 * @ingroup  hct_io
 * @brief  Initializes all I/O output values and pin directions.
 *
 * @par Description:
 * Sets all I/O output values (output active) and pin directions.
 *
 * @attention None
 * @param  val  [IN] Type #td_u32* I/O output value
 * @param  dir  [IN] Type #td_u32* I/O direction
 *
 * @retval #0    Success
 * @retval #Other values  Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_io.h: Describes I/O APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_io_init(EXT_CONST td_u32 *val, EXT_CONST td_u32 *dir);

/**
 * @ingroup  hct_io
 * @brief  Sets the direction of a single I/O pin.
 *
 * @par Description:
 * Sets the direction of a single I/O pin.
 *
 * @attention None
 * @param  gpio_id [IN] Type #ext_gpio_idx_e I/O index
 * @param  enDir   [IN] Type #EXT_GPIO_DIRECTION_E I/O direction.
 *
 * @retval #0       Success
 * @retval #Other values     Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_io.h: Describes I/O APIs.
 * @see  uapi_io_get_dir
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_io_set_dir(ext_gpio_idx_e gpio_id, ext_gpio_direction_e enDir);

/**
 * @ingroup  hct_io
 * @brief  Obtains the direction of a single I/O pin.
 *
 * @par Description:
 * Obtains the direction of a single I/O pin.
 *
 * @attention None
 * @param  gpio_id [IN]  Type #ext_gpio_idx_e I/O index
 * @param  pDir    [OUT] Type #ext_gpio_direction_e* I/O direction
 *
 * @retval #0       Success
 * @retval #Other values     Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_io.h: Describes I/O APIs.
 * @see  uapi_io_set_dir
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_io_get_dir(ext_gpio_idx_e gpio_id, ext_gpio_direction_e *pDir);

/**
 * @ingroup  hct_io
 * @brief  Sets the output level of a single I/O pin.
 *
 * @par Description:
 * Sets the output level of a single I/O pin.
 *
 * @attention None
 * @param  gpio_id [IN] Type #ext_gpio_idx_e I/O index
 * @param  enVal   [IN] Type #EXT_GPIO_VALUE_E Output value.
 * @li 0: low level
 * @li 1: high level
 *
 * @retval #0       Success
 * @retval #Other values     Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_io.h: Describes I/O APIs.
 * @see  uapi_io_get_input_val |uapi_io_get_output_val
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_io_set_ouput_val(ext_gpio_idx_e gpio_id, ext_gpio_value_e enVal);

/**
 * @ingroup  hct_io
 * @brief  Obtains the input level of a single I/O pin.
 *
 * @par Description:
 * Obtains the input level of a single I/O pin.
 *
 * @attention None
 * @param  gpio_id [IN]  Type #ext_gpio_idx_e I/O index
 * @param  pVal    [OUT] Type #ext_gpio_value_e* Output value
 * @li 0: low level
 * @li 1: high level
 *
 * @retval #0       Success
 * @retval #Other values     Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_io.h: Describes I/O APIs.
 * @see  uapi_io_set_ouput_val |uapi_io_get_output_val
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_io_get_input_val(ext_gpio_idx_e gpio_id, ext_gpio_value_e *pVal);

/**
 * @ingroup  hct_io
 * @brief  Obtains the output level of a single I/O pin.
 *
 * @par Description:
 * Obtains the output level of a single I/O pin.
 *
 * @attention None
 * @param  gpio_id [IN]  Type #ext_gpio_idx_e I/O index
 * @param  pVal    [OUT] Type #ext_gpio_value_e* Output value
 * @li 0: low level
 * @li 1: high level
 *
 * @retval #0       Success
 * @retval #Other values     Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_io.h: Describes I/O APIs.
 * @see  uapi_io_set_ouput_val |uapi_io_get_input_val
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_io_get_output_val(ext_gpio_idx_e gpio_id, ext_gpio_value_e *pVal);

/**
 * @ingroup  hct_io
 * @brief  Enables the interrupt function of a GPIO pin.
 *
 * @par Description:
 * Enables the interrupt function of a GPIO pin.
 *
 * @attention None
 * @param  gpio_id       [IN] Type #ext_gpio_idx_e I/O index
 * @param  int_type      [IN] Type #EXT_GPIO_INT_E Interrupt type
 * @li 0: level-triggered
 * @li 1: edge-triggered
 * @param  int_polarity  [IN] Type #EXT_GPIO_INT_POLARITY_E Interrupt polarity
 * @li 0: triggered at falling edge or low level
 * @li 1: triggered at rising edge or high level
 * @param  callback_func [IN] Type #gpio_isr_callback_func Interrupt callback function
 *
 * @retval #0             Success
 * @retval #Other values           Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_io.h: Describes I/O APIs.
 * @see  uapi_io_int_disconnect
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_io_int_connect(ext_gpio_idx_e gpio_id, ext_gpio_int_e int_type, ext_gpio_int_polarity_e int_polarity,
    gpio_isr_callback_func callback_func);

/**
 * @ingroup  hct_io
 * @brief  Disables the interrupt function of a GPIO pin.
 *
 * @par Description:
 * Disables the interrupt function of a GPIO pin.
 *
 * @attention None
 * @param  gpio_id [IN] Type #ext_gpio_idx_e I/O index
 *
 * @retval #0       Success
 * @retval #Other values     Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_io.h: Describes I/O APIs.
 * @see  uapi_io_int_connect
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_io_int_disconnect(ext_gpio_idx_e gpio_id);

/**
 * @ingroup  hct_io
 * @brief  I/O interrupt initialization
 *
 * @par Description:
 * I/O interrupt initialization
 *
 * @attention
 * @li In the initialization phase, the API needs to be called to enable the I/O interrupt so that the I/O interrupt can
 * be responded.
 * @li This API cannot be called repeatedly. It can be called only once during initialization.
 *
 * @param  None
 *
 * @retval #0       Success
 * @retval #Other values     Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_io.h: Describes I/O APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_io_int_init(td_void);

/**
 * @ingroup  hct_io
 * @brief Sets the I/O multiplexing.
 *
 * @par Description:
 * Sets the I/O multiplexing.
 *
 * @attention None
 * @param  io_id    [IN] Type #ext_gpio_idx_e I/O index
 * @param  func_val [IN] Type #td_u8 I/O multiplexing
 * @li For details about the multiplexing value, see the multiplexing register description in the SoC data sheet.
 * @li The user ensures the validity of the input register value. This API does not check the validity of the register
 * value.
 * @retval #0       Success
 * @retval #Other values     Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_io.h: Describes I/O APIs.
 * @see  uapi_io_get_func
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_io_set_func(ext_gpio_idx_e io_id, td_u8 func_val);

/**
 * @ingroup  hct_io
 * @brief Obtains the I/O multiplexing.
 *
 * @par Description:
 * Obtains the I/O multiplexing.
 *
 * @attention None
 * @param  io_id    [IN] Type #ext_gpio_idx_e I/O index
 * @param  func_val [OUT] Type #td_u8* I/O multiplexing
 *
 * @retval #0       Success
 * @retval #Other values     Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_io.h: Describes I/O APIs.
 * @see  uapi_io_set_func
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_io_get_func(ext_gpio_idx_e io_id, td_u8 *func_val);

/**
 * @ingroup  hct_io
 * @brief Sets the I/O drive strength.
 *
 * @par Description:
 * The I/O drive strength is closely related to hardware design. For details, see SoC data sheet.
 *
 * @attention None
 * @param  id    [IN] Type #ext_io_drv_idx_e I/O index
 * @param  level [IN] Type #ext_io_driver_strength_e I/O drive strength level
 *
 * @retval #0       Success
 * @retval #Other values     Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_io.h: Describes I/O APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_io_set_driver_strength(ext_io_ctl_idx_e id, ext_io_driver_strength_e level);

/**
 * @ingroup  hct_io
 * @brief Enables the I/O pull-up.
 *
 * @par Description:
 * Enables the I/O pull-up. For details, see SoC data sheet.
 *
 * @attention This API can be called in an interrupt.
 * @param  id  [IN] Type #ext_io_ctl_idx_e I/O index
 * @param  enSpu [IN] Type #ext_io_spu_e I/O pull-up enable
 *
 * @retval #0       Success
 * @retval #Other values     Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_io.h: Describes I/O APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_io_set_pull(ext_io_ctl_idx_e id, ext_io_pull_e enPu);

#endif
