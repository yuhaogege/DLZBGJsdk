/*
 * Copyright (c) CompanyNameMagicTag 2019-2019. All rights reserved.
 * Description: mdm_data_collect.
 * Author: CompanyName
 * Create: 2019-10-31
 */

#ifndef SOC_MDM_DATACOLLECT
#define SOC_MDM_DATACOLLECT
#include <soc_types.h>
#include <soc_mdm_types.h>
#if defined PRODUCT_CFG_NDM_COLLECT_DATA


/**
 * @ingroup hct_data_collect
 * Region where the collected data is stored
 */
typedef struct {
    uintptr_t buf_addr[2]; /* 2 addr */
    td_u32 buf_size[2];    /* 2 size */
}ext_buf_s;

/**
 * @ingroup hct_data_collect
 * Data Collection Trigger Point
 */
typedef enum {
    EN_TRIG_TYPE_FST_PB = 0,            /**< 0 £ºThe decoding of the first PB block is complete. */
    EN_TRIG_TYPE_SND_PB,                /**< 1 £ºThe decoding of the second PB block is complete. */
    EN_TRIG_TYPE_TRD_PB,                /**< 2 £ºThe decoding of the third PB block is complete. */
    EN_TRIG_TYPE_FTH_PB,                /**< 3 £ºThe fourth PB block is decoded. */
    EN_TRIG_TYPE_FIV_PB,                /**< 4 £ºThe decoding of the fifth PB block is complete. */
    EN_TRIG_TYPE_FC_START,              /**< 5 £ºFC Start */
    EN_TRIG_TYPE_FC_ERR,                /**< 6 £ºTime domain frame tail, including FC decoding errors */
    EN_TRIG_TYPE_NOISE,                 /**< 7 :     blind mining */
    EN_TRIG_TYPE_STAT_FOUR,             /**< 8 :     Carrier sense status 4 */
    EN_TRIG_TYPE_TX_ENABLE_FFT,         /**< 9 £ºTX FFT enable */
    EN_TRIG_TYPE_TX_START_FFT,          /**< 10£ºFFT start of the TX */
    EN_TRIG_TYPE_ENABLE_IFFT,           /**< 11£ºSymbol-aligned IFFT enable */
    EN_TRIG_TYPE_START_IFFT,            /**< 12£ºSymbol-aligned IFFT start */
    EN_TRIG_TYPE_ENABLE_FFT,            /**< 13£ºFFT enable for frequency domain equalization */
    EN_TRIG_TYPE_START_FFT,             /**< 14£ºFFT start for frequency domain equalization */
    EN_TRIG_TYPE_PREM_FFT_OUTPUT_START, /**< 15£ºPreamble FFT output starts. */
    EN_TRIG_TYPE_RREM_FFT_OUTPUT_END,   /**< 16£ºThe FFT output of the preamble ends. */
    EN_TRIG_TYPE_PAYLOAD_START,         /**< 17£ºStart to decode PayLoad. */
    EN_TRIG_TYPE_CTR12FFT_DIN_EN,       /**< 18£ºctrl2fft_din_en */
    EN_TRIG_TYPE_CTR12FFT_DOUT_EN,      /**< 19£ºctrl2fft_dout_en */
    EN_TRIG_TYPE_NBI_FFT_ENABLE,        /**< 20£ºNBI FFT enable */
    EN_TRIG_TYPE_NBI_FFT_START,         /**< 21£ºNBI FFT Start */
    EN_TRIG_TYPE_PL_DECODE_FAIL,        /**< 22£ºPL decoding failure */
    EN_TRIG_TYPE_FC_DECODE_FAIL,        /**< 23£ºFC decoding fails. */
    EN_TRIG_TYPE_FC_0PPM_DECODE_FAIL,   /**< 24£ºFC decoding failure (0 ppm) */
    EN_TRIG_TYPE_PARA_DETECT_FAIL,      /**< 25£º Parallel check failed. */
    EN_TRIG_TYPE_FRAME_END,             /**< 26£ºframe_end */
    EN_TRIG_TYPE_FC_START_COUNT,        /**< 27£ºCount of fc_start intervals. */
    EN_TRIG_TYPE_MAX,
    EN_TRIG_TYPE_MASK_DATA = 256,         /**< 256£ºData mask triggering */
}dc_trig_type_enum;

/**
 * @ingroup hct_data_collect
 * Data collection (from time domain to frequency domain, from front to rear)
 */
typedef enum {
    EN_TRIG_AD_12_BIT = 0,               /* 0 £º 12bit AD Output */
    EN_TRIG_ASYNC_PRO_10_BIT = 11,       /* 11£º10bit Async_Pro Output */
    EN_TRIG_CUBIC_10_BIT = 1,            /* 1 £º 10bit Cubic Output */
    EN_TRIG_IMP_10_BIT = 2,              /* 2 £º 10bit Imp Output */
    EN_TRIG_DCOC_10_BIT = 3,             /* 3 £º 10bit DCOC Output */
    EN_TRIG_NF_10_BIT = 4,               /* 4 :     10bit NF filter output */
    EN_TRIG_DAGC_10_BIT = 5,             /* 5 £º 10bit DAGC output */
    EN_TRIG_CSMA_4_BIT = 6,              /* 6 £º 4bit Carrier sense status */
    EN_TRIG_FFT_ORDER_32_BIT = 7,        /* 7 £º 32bit FFT sequential output (q, i) */
    EN_TRIG_FFT_DISORDER_32_BIT = 8,     /* 8 £º 32bit FFT out-of-order output (q, i) */
    EN_TRIG_FC_12_BIT = 9,               /* 9 £º 12bit FC demodulation output (q, i) */
    EN_TRIG_PAYLOAD_12_BIT = 10,         /* 10£º12bit PayLoad demodulation output (q, i) */
    EN_TRIG_TF_10_BIT_AD_DATA = 12,      /* 12£º10bit Transformer district identification AD data */
    EN_TRIG_DA_12_BIT = 13,              /* 13£º12bit DA data */
    EN_TRIG_IMP_18_BIT = 14,             /* 14£º18bit IMP output */
    EN_TRIG_FFT_16_BIT = 15,             /* 15£º16bit FFT data */
    EN_TRIG_CS_16_BIT = 16,              /* 16£º16bit CS data */
    EN_TRIG_LOGIC_16_LBIT_LOGIC = 17,    /* 17£ºLower 16 bits of logical data */

    /* The bit width of DC_CAPTURE_CTRL enTrigerType is 8 bits, but the available
       * bit width of the software is 16 bits. Therefore, the test sequence mode starts from 256.
       */
    EN_TRIG_INCREMENTAL_SEQUENCE = 256,  /* 256£ºThe offset address for the incremental sequence is 0x80,
                                          * and the corresponding member self_test_mode is set to 1.
                                          */
    EN_TRIG_DESCENDING_SEQUENCE = 257,   /* 257£ºThe member self_test_mode corresponding to the offset
                                          * address 0x80 of the descending sequence is set to 2.
                                          */
    EN_TRIG_LOGIC_32_BIT_LOGIC = 258,    /* 258:      32-bit logical data */
    EN_TRIG_LOGIC_16_HBIT_LOGIC = 259,   /* 259:Upper 16-bit logical data */

    EN_TRIG_DATA_MAX
}dc_trig_data_enum;

/**
 * @ingroup hct_data_collect
 * Data Collection Configuration Parameters
 */
typedef struct {
    td_u32 wait_envok_time;         /* Waiting time for the data collection environment to become stable (ms) */
    td_u32 wait_result_timeout;     /* Timeout interval for waiting for data collection results (ms) */
    uintptr_t  start_addr;          /* Start address of the allocated memory.
                                                           * The address must be 64-byte-aligned.
                                                           */
    uintptr_t  end_addr;            /* End address of the allocated memory. The address must be 64-byte-aligned. */
    td_s32  agc_gain;               /* Configuring Gains */
    td_u32  after_point_len;        /* Length of the data collected after the trigger point
                                                           * when data collection is triggered
                                                           */
    dc_trig_type_enum  triger_type; /* Data Collection Trigger Point */
    dc_trig_data_enum  data_src;    /* Data collection(from time domain to frequency domain, from front to rear) */
}dc_input_para_stru;


/**
* @ingroup  hct_data_collect
* @brief  Initializes the data collection function.
*
* @par Description:
* Initializes the data collection function.
* @attention None.
*
* @retval #EXT_ERR_SUCCESS Success.
* @retval Other values: failure. For details, see soc_errno.h.
*
* @par Dependency:
* @li soc_mdm_datacollect.h: Header file where the interface declaration is located.
* @see  None.
* @since SocMDL_V100R001C00
*/
td_u32 uapi_dc_init(td_void);

/**
* @ingroup  hct_data_collect
* @brief  Common data collection.
*
* @par Description:
* Common data collection.
* @attention The start address and end address of the data collection memory space must be 64-byte-aligned.
*
* @param  pDcInput [IN] Indicates the data collection parameters.
* @param  sBuf [OUT] Data storage area.
*
* @retval #EXT_ERR_SUCCESS Success.
* @retval Other values: failure. For details, see soc_errno.h.
*
* @par Dependency:
* @li soc_mdm_datacollect.h: Header file where the interface declaration is located.
* @see  None.
* @since SocMDL_V100R001C00
*/
td_u32 uapi_dc_start_normal_data_collect(EXT_IN dc_input_para_stru *dc_input, EXT_OUT ext_buf_s *buf);

#endif
#endif

