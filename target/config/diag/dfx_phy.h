/*
 * Copyright (c) CompanyNameMagicTag 2012-2018. All rights reserved.
 * Description: Interfaces Between the PHY Driver and Other Modules
 * Author: CompanyName
 * Create: 2012-07-16
 */

#ifndef DFX_PHY_H
#define DFX_PHY_H

#include "sal_inf.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Check the validity of the analog signal attenuation value. */
#define is_afe_analog_tx_db_valid(db) ((db) < PHY_AFE_ANALOG_TX_DB_MAX)

/* Data collection control switch */
#define COLLECT_DATA_ENABLE 0xa5  /* semaphore protection enabled for the PHY TX buffer when the data sampling      \
                                   * function is enabled. This field is not used when the data sampling function is \
                                   * disabled.                                                                      \
                                   */
#define COLLECT_DATA_DISABLE 0x00 /* Disabling the Data Collection Function */

#define PHY_WORK_CTRL_ENABLE 0  /* Enables the PHY. */
#define PHY_WORK_CTRL_DISABLE 1 /* Disable the PHY. */

typedef enum {
    PHY_AFE_ANALOG_TX_DB_0 = 0, /* Actual attenuation: 20 dB */
    PHY_AFE_ANALOG_TX_DB_2,     /* Actual attenuation: 18 dB */
    PHY_AFE_ANALOG_TX_DB_4,     /* Actual attenuation: 16 dB */
    PHY_AFE_ANALOG_TX_DB_6,     /* Actual attenuation: 14 dB */
    PHY_AFE_ANALOG_TX_DB_8,     /* Actual attenuation: 12 dB */
    PHY_AFE_ANALOG_TX_DB_10,    /* Actual attenuation: 10 dB */
    PHY_AFE_ANALOG_TX_DB_12,    /* Actual attenuation: 8 dB */
    PHY_AFE_ANALOG_TX_DB_14,    /* Actual attenuation: 6 dB */
    PHY_AFE_ANALOG_TX_DB_16,    /* Actual attenuation: 4 dB */
    PHY_AFE_ANALOG_TX_DB_18,    /* Actual attenuation: 2 dB */
    PHY_AFE_ANALOG_TX_DB_20,    /* The actual attenuation is 0 dB, that is, the total power. */
    PHY_AFE_ANALOG_TX_DB_MAX
} phy_afe_analog_tx_db;

/* Sets the flag indicating that data collection is mutually exclusive with the transmitter. */
EXT_EXTERN td_void drv_set_collect_data_flag(td_u32 flag);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
/* Sets the working mode of the PHY. This interface is used to enable or disable the PLC TX/RX function. */
EXT_EXTERN td_void drv_set_phy_work_ctrl_flag(td_u8 flag);

/* To query the PHY working mode */
EXT_EXTERN td_u8 drv_get_phy_work_ctrl_flag(td_void);

/* Starts cyclic signal transmission. */
EXT_EXTERN td_u32 drv_start_send_signal(td_u8 *buf, td_u32 buf_size);

/* Stops transmitting signals cyclically. */
EXT_EXTERN td_void drv_stop_send_signal(td_void);

/* To set the analog transmit gain */
EXT_EXTERN td_u32 drv_set_afe_analog_txdb(phy_afe_analog_tx_db db);

#endif

#ifdef __cplusplus
}
#endif

#endif
