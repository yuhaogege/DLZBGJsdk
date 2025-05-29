/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: dfx_sys deid.
 * Author: CompanyName
 * Create: 2021-1-27
 */

#ifndef __DFX_SYS_DEID_H__
#define __DFX_SYS_DEID_H__

/* System event, message, statistic range definition,
 * see the document <<dw11V100 Software System Level Diagnostic Solution Design Specification.docx>>
 */
#define EXT_DEID_MIN 0
#define EXT_DEID_MAX 1024
/* System level initialization event EXT_DEID_SYS_INIT_E, system level error event (non-system crash) */
typedef enum {
    EXT_DEID_SAL_HARDWARE_INIT_OK = 0,
    EXT_DEID_SAL_HARDWARE_INIT_ERR = 1,
    EXT_DEID_SAL_PHASE1_INIT_OK = 2,
    EXT_DEID_SAL_PHASE1_INIT_ERR = 3,
    EXT_DEID_SAL_START_PHASE2_INIT = 4,
    EXT_DEID_SAL_START_PHASE2_INIT_ERR = 5,
    EXT_DEID_SAL_CTX_HEAP_ERR = 6,
    EXT_DEID_SAL_CTX_INIT_START = 7,
    EXT_DEID_SAL_CTX_INIT_OK = 8,
    EXT_DEID_SAL_MEM_INIT_OK = 9,
    EXT_DEID_SAL_QES_INIT_OK = 10,
    EXT_DEID_SAL_TASKS_INIT_OK = 11,
    EXT_DEID_SYS_INIT_OK = 12,
    EXT_DEID_SAL_NV_INIT_ERR = 13,
    EXT_DEID_SAL_NV_INIT_OK = 14,
    EXT_DEID_FTM_DEVICE_CFG_ERR = 15,
    EXT_DEID_SYS_CRASH = 16,
    EXT_DEID_SYS_CRASH_MON_START = 17,
    EXT_DEID_SYS_APP_TASK_INIT_END = 18,
    EXT_DEID_SYS_TASK_START_END = 19,
    EXT_DEID_SAL_MAX = 20,

    EXT_DEID_DRV_INIT_MIN = 21,
    EXT_DEID_DRV_FLASH_INIT_OK = 22,         /* FLASH initialization completed */
    EXT_DEID_DRV_UART_INIT_OK = 23,          /* UART initialization completed */
    EXT_DEID_DRV_HRTIMER_INIT_OK = 24,       /* HRTIMER initialization completed */
    EXT_DEID_DRV_HRTIMER_DESTROY_OK = 25,    /* HRTIMER to initialize */
    EXT_DEID_DRV_ETH_INIT_OK = 26,           /* ETH initialization completed */
    EXT_DEID_DRV_ETH_REMOVE_OK = 27,         /* ETH to initialize */
    EXT_DEID_DRV_UPG_INIT_OK = 28,           /* UPG initialization completed */
    EXT_DEID_DRV_UPG_START_OK = 29,          /* UPG starts to upgrade */
    EXT_DEID_DRV_UPG_BACKUP_OK = 30,         /* UPG version fallback */
    EXT_DEID_DRV_UPG_STOP_OK = 31,           /* UPG upgrade stopped */
    EXT_DEID_DRV_UPG_TIMEOUT_FINISH_OK = 32, /* UPG timeout restart */
    EXT_DEID_DRV_UPG_APPCALL_FINISH_OK = 33, /* UPG callback restart */
    EXT_DEID_DRV_DFX_INIT_OK = 34,           /* DFX initialization is complete */

    EXT_DEID_DRV_SPI_INIT_OK = 35,
    EXT_DEID_DRV_CRC_INIT_OK = 36,
    EXT_DEID_DRV_GPIO_INIT_OK = 37,
    EXT_DEID_DRV_CIPHER_INIT_OK = 38,
    EXT_DEID_DRV_LED_INIT_OK = 39,
    EXT_DEID_DRV_IR_INIT_OK = 40,
    EXT_DEID_DRV_I2C_INIT_OK = 41,
    EXT_DEID_DRV_DMA_INIT_OK = 42,
    EXT_DEID_DRV_UPG_INIT_FAIL = 43,
    EXT_DEID_DRV_NV_UNALIGN = 44,
    EXT_DEID_DRV_INIT_MAX = 50,

    /* The following defines the initialization events of each module of the MAC protocol stack. */
    EXT_DEID_MAC_INIT_MIN = 51,
    EXT_DEID_MAC_PHY_INIT_OK = 52,
    EXT_DEID_MAC_PHY_INIT_NG = 53,
    EXT_DEID_MAC_BEACON_INIT_OK = 54,
    EXT_DEID_MAC_BEACON_INIT_NG = 55,
    EXT_DEID_MAC_CVG_INIT_OK = 56,
    EXT_DEID_MAC_CVG_INIT_NG = 57,
    EXT_DEID_MAC_MAC_BUFFER_INIT_OK = 58,
    EXT_DEID_MAC_MAC_BUFFER_INIT_NG = 59,
    EXT_DEID_MAC_DFX_INIT_OK = 60,
    EXT_DEID_MAC_DFX_INIT_NG = 61,
    EXT_DEID_MAC_LOG_INIT_OK = 62,
    EXT_DEID_MAC_LOG_INIT_NG = 63,
    EXT_DEID_MAC_SGM_INIT_OK = 64,
    EXT_DEID_MAC_SGM_INIT_NG = 65,
    EXT_DEID_MAC_NM_ASSOC_INIT_OK = 66,
    EXT_DEID_MAC_NM_ASSOC_INIT_NG = 67,
    EXT_DEID_MAC_NM_COO_INIT_OK = 68,
    EXT_DEID_MAC_NM_COO_INIT_NG = 69,
    EXT_DEID_MAC_NM_TDMA_INIT_OK = 72,
    EXT_DEID_MAC_NM_TDMA_INIT_NG = 73,
    EXT_DEID_MAC_NM_TOPO_INIT_OK = 74,
    EXT_DEID_MAC_NM_TOPO_INIT_NG = 75,
    EXT_DEID_MAC_SD_INIT_OK = 76,
    EXT_DEID_MAC_SD_INIT_NG = 77,
    EXT_DEID_MAC_ROUTE_INIT_OK = 78,
    EXT_DEID_MAC_ROUTE_INIT_NG = 79,
    EXT_DEID_MAC_RSB_INIT_OK = 80,
    EXT_DEID_MAC_RSB_INIT_NG = 81,
    EXT_DEID_MAC_RX_INIT_OK = 82,
    EXT_DEID_MAC_RX_INIT_NG = 83,
    EXT_DEID_MAC_TX_INIT_OK = 84,
    EXT_DEID_MAC_TX_INIT_NG = 85,
    EXT_DEID_MAC_INIT_MAX = 150,

    EXT_DEID_APP_INIT_MIN = 151,
    EXT_DEID_APP_MRS_FW_INIT_OK,
    EXT_DEID_APP_MRS_FW_INIT_FAIL,
    EXT_DEID_APP_376_2_PROTO_INIT_OK,
    EXT_DEID_APP_376_2_PROTO_INIT_FAIL,
    EXT_DEID_APP_645_PROTO_INIT_OK,
    EXT_DEID_APP_645_PROTO_INIT_FAIL,
    EXT_DEID_APP_CCO_CTX_INIT_OK,
    EXT_DEID_APP_CCO_CTX_INIT_FAIL,
    EXT_DEID_APP_STA_CTX_INIT_OK,
    EXT_DEID_APP_STA_CTX_INIT_FAIL,
    EXT_DEID_APP_QUEUE_INIT_OK,
    EXT_DEID_APP_QUEUE_INIT_FAIL,
    EXT_DEID_APP_MAC_LIST_INIT_OK,
    EXT_DEID_APP_MAC_LIST_INIT_FAIL,
    EXT_DEID_APP_MAC_MAP_INIT_OK,
    EXT_DEID_APP_MAC_MAP_INIT_FAIL,
    EXT_DEID_APP_SES_INIT_OK,
    EXT_DEID_APP_SES_INIT_FAIL,
    EXT_DEID_APP_READ_MREC_OK,
    EXT_DEID_APP_READ_MREC_FAIL,
    EXT_DEID_APP_READ_NV_OK,
    EXT_DEID_APP_READ_NV_FAIL,
    EXT_DEID_APP_START_MRS_OK,
    EXT_DEID_APP_INIT_MAX = 200,

    /* System Init Event */
    EXT_DEID_SYS_INIT_FLASH_CFG_ERR = 201, /* Flash configuration error, block size configuration and device
                                           * inconsistency
                                           */
    EXT_DEID_SYS_INIT_OPEN_DEV_ERR = 202,  /* failed to open the device */
    EXT_DEID_SYS_INIT_DEV_CFG_ERR = 203,   /* Get device configuration failed */
    EXT_DEID_SYS_INIT_HW_CFG_ERR = 204,    /* Failed to get hardware configuration */
    EXT_DEID_SYS_INIT_CFG_MEM_ERR = 205,   /* Get configuration memory error, use default memory, enter the minimum
                                           * running debug mode
                                           */
    EXT_DEID_SYS_INIT_MEM_FAIL = 206,
    EXT_DEID_SYS_INIT_R_UPG_CFG_FAIL = 207,
    EXT_DEID_SYS_INIT_VERIFY_MSG = 208,

    EXT_DEID_SYS_PLC_MODE = 220,

    /* USR initialization event [230, 255) */
    EXT_DEID_USR_INIT_EVNET_START = 230,
    EXT_DEID_USR_INIT_EVNET_END = 255,
    EXT_DEID_INIT_MAX = 255 /* must not be greater than 256 */
} ext_deid_sys_init;

/* System level error/alarm/important event definition [256, 1024) */
#define EXT_DEID_SAL_SYS_EVT_START (EXT_DEID_INIT_MAX + 1) /* 256 */

#endif /* __DFX_SYS_DEID_H__ */

