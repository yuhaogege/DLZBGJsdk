/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Adjust the directory structure to prepare for compiling the SDK. soc_sal_cfg.h code
 * Author: CompanyName
 * Create: 2012-07-14
 */
#ifndef __SOC_SAL_CFG_H__
#define __SOC_SAL_CFG_H__
#include <soc_types.h>
#include <soc_mdm_types.h>

#define RAM_SIZE_8M (0x800000)
#define RAM_SIZE_2M (0x200000)
#define RAM_SIZE_32M (0x2000000)

#define ARM_AHB_266M (266000000)
#define ARM_AHB_133M (133000000)
#define ARM_APB_66_5M (66500000)
#define ARM_APB_33_25M (33250000)
#define ARM_AHB_75M (75000000)
#define ARM_APB_37_5M (37500000)

#ifdef PRODUCT_CFG_BOARD_TYPE_ASIC
#ifdef PRODUCT_CFG_PRODUCT_TYPE_STA
#define HW_RAM_DEFAULT_SIZE (RAM_SIZE_2M)  /* Default hardware memory size */
#define USE_RAM_DEFAULT_SIZE (RAM_SIZE_2M) /* Default software for memory size */
#define DEFAULT_ETH_ENABLE_STAT TD_FALSE   /* Default EHT enable state */
#else
#define HW_RAM_DEFAULT_SIZE (RAM_SIZE_8M)
#define USE_RAM_DEFAULT_SIZE (RAM_SIZE_8M)
#define DEFAULT_ETH_ENABLE_STAT TD_TRUE
#endif
#else
#define HW_RAM_DEFAULT_SIZE (RAM_SIZE_32M)  /* Default hardware memory size */
#define USE_RAM_DEFAULT_SIZE (RAM_SIZE_32M) /* The default software applies memory size */
#define DEFAULT_ETH_ENABLE_STAT TD_TRUE     /* Default EHT enable state */

#endif

typedef struct {
    td_u32 sw_ram_size; /* software uses ram size */

    td_u32 cpu_freq : 1;       /* ext_cfg_performance_high: no down frequency ext_cfg_performance_low: down frequency */
    td_u32 enable_crc_clk : 1; /* whether to enable the crc clock */
    td_u32 enable_eth_clk : 1; /* whether to enable eth clock */
    td_u32 enable_cipher_clk : 1; /* whether to enable the cipher clock */
    td_u32 enable_dt_clk : 1;     /* whether to enable the dt (test function) clock */
    td_u32 enable_dmac_clk : 1;   /* whether to enable the dmac clock */
    td_u32 enable_spi_clk : 1;    /* whether to enable spi clock */
    td_u32 enable_sw_eth : 1;     /* whether to open the network port */
    td_u32 reserver : 24;
} ext_sal_usr_cfg;

#ifdef PRODUCT_CFG_FEATURE_INTERNAL_APP
td_void INNER_APP_InitCfgEarly(td_void);
#endif

#endif /* __SOC_SAL_CFG_H__ */
