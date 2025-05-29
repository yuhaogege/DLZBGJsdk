/*
 * Copyright (c) CompanyNameMagicTag 2019-2019. All rights reserved.
 * Description: soc_config.
 * Author: CompanyName
 * Create: 2019-10-31
 */
#ifndef __SOC_CONFIG_H__
#define __SOC_CONFIG_H__
/****************************************************************************** */
#include <soc_config_rom.h>
#include <soc_types_rom.h>

#ifdef __NEW_FILE_ID__
#define EXT_DIAG_LOG_MSG_MOD_ID  __NEW_MOD_ID__
#define EXT_DIAG_LOG_MSG_FILE_ID __NEW_FILE_ID__
#else
#define EXT_DIAG_LOG_MSG_MOD_ID  0
#define EXT_DIAG_LOG_MSG_FILE_ID 0
#endif

/**
 * @ingroup hct_mem
 * Module ID, which is used to apply for memory and report diag messages. The message reporting function
 * is invisible to users during secondary development.
 * @li This module ID is used to support the memory Maintenance and testing. statistics function.
 * @li The ID of the message reporting function must be the same as that of the module ID in the mss_subsystem_db.xml.
 * @li The module ID ranges are [200, 400) for the BSP layer,
 * [500, 600) for the MAC layer, and [800, 900) for the app layers.
 * @li The module IDs cannot be deleted or changed.
 */
#define EXT_MOD_ID_BSP_MIN_ROM EXT_MOD_ID_BSP_MIN
#define EXT_MOD_ID_SAL_NVM     201
#define EXT_MOD_ID_SAL_DIAG    202
#define EXT_MOD_ID_SAL_DFX     203
#define EXT_MOD_ID_PARTITION   204
#define EXT_MOD_ID_DRV_UPG     205
#define EXT_MOD_ID_DRV_DFX     206
#define EXT_MOD_ID_APP_UPG     207
#define EXT_MOD_ID_SYS_ERR     208
#define EXT_MOD_ID_CPUP        209
#define EXT_MOD_ID_CPUP_DFX    210
#define EXT_MOD_ID_UART        211
#define EXT_MOD_ID_REMAP       212
#define EXT_MOD_ID_BSP_ROM     EXT_MOD_ID_ROM
#define EXT_MOD_ID_BSP_MAX_ROM EXT_MOD_ID_BSP_MAX

#define EXT_MOD_ID_MAC_MIN_ROM EXT_MOD_ID_MAC_MIN
#define EXT_MOD_ID_MAC_CVG     501
#define EXT_MOD_ID_MAC_SGM     502
#define EXT_MOD_ID_MAC_TX      503
#define EXT_MOD_ID_MAC_RX      506
#define EXT_MOD_ID_MAC_HBT     507
#define EXT_MOD_ID_MAC_ROUTE   508
#define EXT_MOD_ID_MAC_DFX     511
#define EXT_MOD_ID_MAC_ASSOC   512
#define EXT_MOD_ID_MAC_COO     513
#define EXT_MOD_ID_MAC_TDMA    515
#define EXT_MOD_ID_MAC_TOPO    516
#define EXT_MOD_ID_MAC_PHY_DRV 517
#define EXT_MOD_ID_MAC_NTB     518
#define EXT_MOD_ID_MAC_CPA     519
#define EXT_MOD_ID_MAC_NDM     520
#define EXT_MOD_ID_MAC_FA      521
#define EXT_MOD_ID_MAC_EMC     524
#define EXT_MOD_ID_MAC_NM      530
#define EXT_MOD_ID_MAC_TF      531

#define EXT_MOD_ID_MAC_MAX_ROM EXT_MOD_ID_MAC_MAX

#define EXT_MOD_ID_APP_MIN_ROM EXT_MOD_ID_APP_MIN
#define EXT_MOD_ID_APP_COMMON  812
#define EXT_MOD_ID_APP_MAX_ROM EXT_MOD_ID_APP_MAX

/****************************************************************************** */
/* NV range */
/* PHY_DRV:  [0x0001, 0x0100) */
/* SAL:      [0x0100, 0x0200) */
/* MAC:      [0x0200, 0x0400) */
/* APP:      [0x0400, 0x0600) */
/* USER:     [0x8000, 0x9000) * */
/* FT:       [0x0F00, 0x1F00) */
/****************************************************************************** */
#define EXT_NV_PHY_ID_START  0x0001
#define EXT_NV_PHY_ID_END    0x0100
#define EXT_NV_SAL_ID_START  0x0100
#define EXT_NV_SAL_ID_END    0x0200
#define EXT_NV_MAC_ID_START  0x0200
#define EXT_NV_MAC_ID_END    0x0400
#define EXT_NV_APP_ID_START  0x0400
#define EXT_NV_APP_ID_END    0x0600
#define EXT_NV_USER_ID_START 0x8000
#define EXT_NV_USER_ID_END   0x9000
/****************************************************************************** */
/****************************************************************************** */
/* Statistic */
/* SAL	[0xA000, 0xA050) */
/* DRV	[0xA050, 0xA150) */
/* CMN	[0xA150, 0xA250) */
/* MAC	[0xA250, 0xA450) */
/* APP	[0xA450, 0xA850) */
/* Reserved [0xA850, 0xB000) */
/* Reserved [0xB200, 0xEE00) */
/* USR [0xEE00, 0xF000) */
/****************************************************************************** */
#define EXT_GOBJ_STAT_ID_MIN  0xA000
#define EXT_GOBJ_STAT_ID_MAMX 0xF000
#define EXT_GOBJ_PHY_ID_MIN 0x0001
#define EXT_GOBJ_PHY_ID_MAX 0x1000
#define EXT_GOBJ_MAC_ID_MIN 0x1000
#define EXT_GOBJ_MAC_ID_MAX 0x2000
#define EXT_GOBJ_DRV_ID_MIN 0x2000
#define EXT_GOBJ_DRV_ID_MAX 0x2100
#define EXT_GOBJ_NDM_ID_MIN 0x3400
#define EXT_GOBJ_NDM_ID_MAX 0x3600
#define EXT_GOBJ_MDM_ID_MIN 0x3600
#define EXT_GOBJ_MDM_ID_MAX 0x3800
#define EXT_GOBJ_NDC_ID_MIN 0x3800
#define EXT_GOBJ_NDC_ID_MAX 0x3900
#define EXT_GOBJ_APP_ID_MIN 0x5600
#define EXT_GOBJ_APP_ID_MAX 0x6600
#define EXT_GOBJ_CTS_ID_MIN 0xF000
#define EXT_GOBJ_CTS_ID_MAX 0xF500

/****************************************************************************** */
/* Global memory size settings */
/****************************************************************************** */
#define EXT_TOTAL_HEAP_SIZE    (8 * 1024 * 1024)  /* Default maximum memory size */
#define EXT_TOTAL_HEAP_SIZE_2M (2 * 1024 * 1024)

#define EXT_PLATFORM_ASIC_VER1    (1) /* 8M V100 program */
#define EXT_PLATFORM_ASIC_VER2    (2) /* 2M V100 program */
#define EXT_PLATFORM_ASIC_V200_8M (3) /* 8M V200 program */
#define EXT_PLATFORM_ASIC_V200_2M (4) /* 2M V200 program */

#define EXT_DIAG_MODE_DEFAULT_DYN_MEM_SIZE    (16 * 1024) /* Default dynamic memory size in DIAG mode */
#define EXT_DIAG_MODE_DEFAULT_STATIC_MEM_SIZE (32 * 1024) /* Default static memory size in DIAG mode */

#define EXT_HEAP_MIN_ALLOCATION_SIZE (64)  /* min. allocation memory size */

#define EXT_SYS_TICKS_PER_SECOND     (100)  /* ticks/sec */
#define EXT_SYS_MILLISECOND_PER_TICK EXT_MILLISECOND_PER_TICK

/* Maximum size of a single buffered DIAG packet */
#define EXT_DIAG_BUFFERING_MSG_MAX_LEN (100)
#define EXT_DIAG_BUFFERING_CMD_MAX_LEN (100)
#define EXT_DIAG_ACK_DATA_LEN          (100)

/* Size of the buffer queue in the DAIG channel */
/* If the number of messages exceeds this threshold, the memory is dynamically allocated */
#define EXT_DIAG_BUFFERING_MSG_NUM (8)
/* If the number of commands exceeds this threshold, the memory is dynamically allocated. */
#define EXT_DIAG_BUFFERING_CMD_NUM (5)

/****************************************************************************** */
/****************************************************************************** */
/* Global static settings */
/****************************************************************************** */
#define EXT_SYS_MSG_PARAM_NUM_MAX_AULDATA 2

#define EXT_SYS_MSG_PARAM_NUM_MIN 4
#define EXT_PLC_MAC_ADDR_LEN      6
#define EXT_MDM_DEV_ID_LEN        6
#define EXT_METER_ADDR_LEN        6
#define EXT_IPV4_ADDR_LEN         4
#define EXT_ETH_MAC_ADDR_LEN      6
#define EXT_PLC_RELAY_NUM_MAX     3

/* Maximum size of a received DIAG packet, including the channel frame header and payload */
#define EXT_DIAG_PACKET_REQ_CACHE_MAX_SIZE (632)
/* Cache size of the DIAG subsystem (closely related to the tailoring).  */
/* If this threshold is exceeded, packets are discarded. To support 4 x 520, increase the value. */
#define EXT_DIAG_PACKET_ACK_MAX_SIZE       (1024) /* Length of a single ACK packet */
#define EXT_DIAG_PACKET_REQ_DATA_MAX_SIZE  (2100) /* Size of a request packet. The value is set to 2100 ( = 4 x 520) */
/* temporarily, which is reserved for subsequent testing. */
#define EXT_DIAG_FILENAME_MAX_LEN      (32)
#define EXT_DIAG_ERR_REPORT_TIME_LIMIT (2000)  /* ms */

#define EXT_DMS_MAC_PORT_TX_TIME_OUT       (0) /* s */
#define EXT_DMS_TCP_PORT_TX_TIME_OUT       (0) /* s */
#define EXT_DMS_UART_CCO_PORT_TX_TIME_OUT  (0) /* s */
#define EXT_DMS_UART_STA_PORT_TX_TIME_OUT  (0) /* s */
#define EXT_DMS_UART_DIAG_PORT_TX_TIME_OUT (4) /* s */

/* Task waiting exception */
#define EXT_SYS_TASK_WAIT_EXCEPTION_TIME_OUT 10000  /* Wait timeout period for task exceptions (unit: ms) */
/****************************************************************************** */
#if defined(PRODUCT_CFG_DBK)
#undef PRODUCT_CFG_MCORE_RAM_LOW_ADDR
#define PRODUCT_CFG_MCORE_RAM_LOW_ADDR 0x0
#endif

/****************************************************************************** */
#define EXT_SYS_RAM_ADDR_LIMIT (PRODUCT_CFG_MCORE_RAM_LOW_ADDR)
#define EXT_ADDR_RAM_BASE      ((td_u32)(EXT_SYS_RAM_ADDR_LIMIT))

/* Read/Write start address */
#if defined(PRODUCT_CFG_PLATFORM_WINDOWS) || defined(PRODUCT_CFG_FEATURE_UT)
#define EXT_ADDR_USER_WR (0x00001000)
#else
#define EXT_ADDR_USER_WR (0x10000000)
#endif

#if defined(PRODUCT_CFG_PLATFORM_WINDOWS) || defined(PRODUCT_CFG_FEATURE_UT) || defined(PRODUCT_CFG_DBK)
#define EXT_ADDR_USER_RAM_START (0x00001000)
#undef EXT_ADDR_RAM_BASE
#define EXT_ADDR_RAM_BASE     (0x00001000)
#define EXT_ADDR_USER_RAM_MAX 0xFFFFFFFF
#define set_ram_addr(x)      \
    if (x == 0)         \
    x = 0x02000000
#else
#define EXT_ADDR_USER_RAM_START ((td_u32)(EXT_ADDR_RAM_BASE))
#define EXT_ADDR_USER_RAM_MAX   (0xFFFFFFFF) /* This value cannot be determined due to dynamic loading. */
#define set_ram_addr(x)
#endif

#define EXT_ADDR_USER_RAM EXT_ADDR_USER_RAM_START
#define EXT_ADDR_CODE_RAM EXT_SYS_RAM_ADDR_LIMIT

#if !defined(PRODUCT_CFG_FEATURE_UT)
#if defined(HAVE_PCLINT_CHECK)  /* Underlying layer API adaptation, not involved in the PCLINT check */
#define EXT_SYS_ADDR_USER_RAM (EXT_SYS_RAM_ADDR_LIMIT)
#else
#define EXT_SYS_ADDR_USER_RAM \
    (((EXT_ADDR_USER_RAM) < EXT_SYS_RAM_ADDR_LIMIT) ? (EXT_SYS_RAM_ADDR_LIMIT) : (EXT_ADDR_USER_RAM))
#endif
#else
#define EXT_SYS_ADDR_USER_RAM (EXT_ADDR_USER_RAM_START)
#endif

#if !defined(PRODUCT_CFG_FEATURE_UT)
#if defined(PRODUCT_CFG_CHECK_CODE_ADDR)
#define uapi_is_valid_ram_addr(_x) (((uintptr_t)(_x) > (td_u32)EXT_ADDR_USER_RAM_START) && \
    ((td_u32)(_x) < (td_u32)EXT_ADDR_USER_RAM_MAX))
#else
#define uapi_is_valid_ram_addr(_x) (((uintptr_t)(_x) > (td_u32)EXT_ADDR_RAM_BASE) && \
    ((td_u32)(_x) < (td_u32)EXT_ADDR_USER_RAM_MAX))
#endif
#else
#define uapi_is_valid_ram_addr(_x) (_x)
#endif

#if !defined(PRODUCT_CFG_FEATURE_UT)
#if defined(PRODUCT_CFG_CHECK_CODE_ADDR)
#define uapi_is_valid_code_addr(_x) (((uintptr_t)(_x)) > EXT_ADDR_CODE_RAM)
#else
#define uapi_is_valid_code_addr(_x) (((uintptr_t)(_x)) > EXT_SYS_RAM_ADDR_LIMIT)
#endif
#else
#define uapi_is_valid_code_addr(_x) (_x)
#endif

/****************************************************************************** */
/****************************************************************************** */
#define EXT_SYS_TASK_STK_MIN_SIZE 128

#define EXT_SYS_TASK_STK_WD1 0xA1A2A3A4
#define EXT_SYS_TASK_STK_WD2 0xB1B2B3B4

/* The value must be equal to the sum of EXT_SYS_MTSK_MAX_NUM and EXT_SYS_ATSK_MAX_NUM. */
#define EXT_SYS_TSK_MAX_NUM  20
/* The four arithmetic expression is not used because of the DBK parsing issue. */
#define EXT_SYS_HTSK_MAX_NUM 4
#define EXT_SYS_MQ_MAX_NUM   10
#define EXT_SYS_AQ_MAX_NUM   1

#define EXT_SYS_MTSK_MAX_NUM       18
#define EXT_SYS_ATSK_MAX_NUM       2
#define EXT_DIAG_DBG_SYNC_SIZE_MAX 3
/****************************************************************************** */
/****************************************************************************** */
/* NV range */
/****************************************************************************** */
/**
* @ingroup  hct_nv
*
* Start ID of the factory NV partition (including this ID)
 */
#define EXT_NV_FACTORY_ID_START (0x0)
/**
* @ingroup  hct_nv
*
* End ID of the factory NV partition (excluding this ID)
 */
#define EXT_NV_FACTORY_ID_END (0x16)

/**
* @ingroup  hct_nv
*
* Start ID of the user area in the factory NV partition (including this ID)
 */
#define EXT_NV_FACTORY_USR_ID_START EXT_NV_FACTORY_ID_END
/**
* @ingroup  hct_nv
*
* End ID of the user area in the factory NV partition (excluding this ID)
 */
#define EXT_NV_FACTORY_USR_ID_END (0x20)

/**
* @ingroup  hct_nv
*
* Start ID of the common NV partition (including this ID)
 */
#define EXT_NV_NORMAL_ID_START EXT_NV_FACTORY_USR_ID_END
/**
* @ingroup  hct_nv
*
* End ID of the common NV partition (excluding this ID)
 */
#define EXT_NV_NORMAL_ID_END (0x80)

/**
* @ingroup  hct_nv
*
* Start ID of the reserved upgrade partition for common NVs (including this ID)
 */
#define EXT_NV_STABLE_ID_START EXT_NV_NORMAL_ID_END
/**
* @ingroup  hct_nv
*
* End ID of the reserved upgrade partition for common NVs (excluding this ID)
 */
#define EXT_NV_STABLE_ID_END (0x98)

/**
* @ingroup  hct_nv
*
* User ID of the reserved upgrade partition for common NVs (including this ID)
 */
#define EXT_NV_STABLE_USR_ID_START EXT_NV_STABLE_ID_END
/**
* @ingroup  hct_nv
*
* User ID of the reserved upgrade partition for common NVs (excluding this ID)
 */
#define EXT_NV_STABLE_USR_ID_END (0xA0)

/**
* @ingroup  hct_nv
*
* Start ID of the reserved user area in the common NV partition (including this ID)
 */
#define EXT_NV_NORMAL_USR_ID_START EXT_NV_STABLE_USR_ID_END
/**
* @ingroup  hct_nv
*
* End ID of the reserved user area in the common NV partition (excluding this ID)
 */
#define EXT_NV_NORMAL_USR_ID_END (0xFF)

/****************************************************************************** */
/****************************************************************************** */
/* Channel settings */
/****************************************************************************** */
#define EXT_DMS_CHL_UART_PORT_MIN  (0) /* min. */
#define EXT_DMS_CHL_UART_PORT_APP  (0) /* UART port number supported by the app channel.  */
/* The value must be less than EXT_DMS_CHL_UART_PORT_NUM_MAX. */
#define EXT_DMS_CHL_UART_PORT_DIAG (1) /* UART port number supported by the DIAG channel.  */
/* The value must be less than EXT_DMS_CHL_UART_PORT_NUM_MAX. */
#define EXT_DMS_CHL_UART_PORT_MAX  (1) /* max. */

#define EXT_DMS_CHL_UART_PORT_NUM_MAX  (3)
#define EXT_DMS_CHL_UART_PORT_NUM_MAX2 (3)

#define EXT_DMS_CHL_MAC_PORT_MIN  (0x10)
#define EXT_DMS_CHL_MAC_PORT_RESV (0x10) /* Not used for DW21. It indicates the DIAG channel for dw11 V200.
If this function is developed, compatibility tests must be performed. */
#define EXT_DMS_CHL_MAC_PORT_APP  (0x11) /* To be compatible with dw11 V200, this value cannot be modified. */
#define EXT_DMS_CHL_MAC_PORT_UPG  (0x12) /* To be compatible with dw11 V200, this value cannot be modified. */
#define EXT_DMS_CHL_MAC_PORT_MAX  (0x12)

#define EXT_DMS_CHL_DIAG_PORT_MAX   (30003)  /* max. */
#define EXT_DMS_CHL_FLASH_PORT_DAIG (0xE000) /* Local flash port */

#define EXT_DMS_CHL_PORT_ANY     (0xFFF0)
#define EXT_DMS_CHL_PORT_NA      (0xFFF1)
#define EXT_DMS_CHL_PORT_INVALID (0xFFFF)

#define EXT_DIAG_CHL_UART_BLOCK_WRITE_TIMEOUT 10   /* Block write timeout (unit: ms) */
#define EXT_DMS_CHL_UART_BLOCK_WRITE_TIMEOUT  10   /* Block write timeout (unit: ms) */
#define EXT_DMS_CHL_EXCEPTION_POLLING_WAIT    5000 /* Device read timeout period for task exceptions (unit: ms) */

#define EXT_DMS_CHL_FLASH_BLOCK_WRITE_TIMEOUT 10  /* Block write timeout (unit: ms) */
#define EXT_DMS_CHL_FLASH_BLOCK_READ_TIMEOUT  5

#define EXT_DMS_UART_EXCEPTION_POLLING_WAIT 1000  /* Timeout period for UART exceptions (unit: ms) */

/* Number of times that the client fails to be detected in heartbeat mode after the channel is connected */
#define EXT_DMS_CHL_CONNECT_DETECT_NUM 3

#define EXT_DMS_CHL_REPORT_PACKET_TIMEOUT 10  /* Batch report delay */

#define EXT_DIAG_MODULE_SYS (5)  /* Number of types that can be configured for the SYS MSG */
#define EXT_DIAG_MODULE_DEV (30) /* Number of modules that can be configured for the DEV MSG */
#define EXT_DIAG_MODULE_USR (5)  /* Number of categories that can be configured for the USR MSG */

#define EXT_DIAG_WAIT_MESSAGE_TIMEOUT        (50)  /* unit: ms */
#define EXT_DIAG_WAI_DIAG_MESSAGE_TIMEOUT    (100) /* Minimum cycle for reporting a DIAG packet (unit: ms) */
#define EXT_DMS_FLASH_EMPTY_DATA             (0xFFFFFFFF)
#define EXT_DMS_LF_PADDING                   (0xA1A2A3A4)
#define EXT_SYS_TASK_WAIT_EXCEPTION_TIME_OUT 10000          /* Wait timeout period for task exceptions (unit: ms) */
#define EXT_SYS_TASK_DROP_POLLING_WAIT       (1000 * 60 * 60) /* unit: ms */

#define EXT_DMS_TCP_SND_TIME_MAX_CHK (5000)

/* NDM */
#define EXT_ND_NDM_START_DEFAULT 0x3400
#define EXT_ND_NDM_END_DEFAULT   0x3600

/****************************************************************************** */
/* Default baud rate settings */
#define EXT_DMS_CCO_APP_UART_DEFAULT_BAUDRATE (9600)
#define EXT_DMS_CCO_APP_UART_DEFAULT_DATABIT  (8)
#define EXT_DMS_CCO_APP_UART_DEFAULT_STOP     (1)
#define EXT_DMS_CCO_APP_UART_DEFAULT_PARITY   (2)
#define EXT_DMS_CCO_APP_UART_DEFAULT_FLOWCTRL (0)

#define EXT_DMS_STA_APP_UART_DEFAULT_BAUDRATE (2400)
#define EXT_DMS_STA_APP_UART_DEFAULT_DATABIT  (8)
#define EXT_DMS_STA_APP_UART_DEFAULT_STOP     (1)
#define EXT_DMS_STA_APP_UART_DEFAULT_PARITY   (2)
#define EXT_DMS_STA_APP_UART_DEFAULT_FLOWCTRL (0)

#if defined(PRODUCT_CFG_SUPPORT_UART2)
#define EXT_DMS_EQUIP_UART_DEFAULT_BAUDRATE  (115200)
#define EXT_DMS_EQUIP_UART_DEFAULT_BAUDRATE2 (115200 * 2)
#define EXT_DMS_EQUIP_UART_DEFAULT_DATABIT   (8)
#define EXT_DMS_EQUIP_UART_DEFAULT_STOP      (1)
#define EXT_DMS_EQUIP_UART_DEFAULT_PARITY    (2)
#define EXT_DMS_EQUIP_UART_DEFAULT_FLOWCTRL  (0)
#endif
/****************************************************************************** */
/****************************************************************************** */
/* DFX ID Definition to DBK */
/****************************************************************************** */
#define EXT_DFX_ID_BASE_PHY 0x0050 /* PHY DBK packet ID ranges: [0x0050, 0x1000) [0x2100, 0x2900) */
#define EXT_DFX_ID_BASE_MAC 0x1000 /* MAC DBK packet ID ranges: [0x1000, 0x2000) [0x2900, 0x3000) [0x3000, 0x4000) */
#define EXT_DFX_ID_BASE_APP 0x5000 /* App DBK packet ID ranges: [0x5600, 0x5E00) [0x5E00, 0x6600) */
#define EXT_DFX_ID_BASE_SAL 0x7000 /* SAL DBK packet ID range: [0x7000, 0x8000) */
#define EXT_DFX_ID_BASE_SYS 0x8000 /* SYS DBK packet ID range: [0x8000, 0xA000) */

/* Ranges of messages between module layers */
#define EXT_DFX_MSG_ID_BASE_PHY 0x0050 /* PHY: [0x50, 0x1000) */
#define EXT_DFX_MSG_ID_BASE_MAC 0x1000 /* MAC:(0x1000,0x5000) */
#define EXT_DFX_MSG_ID_BASE_APP 0x5000 /* APP:(0x5000,0x7000) */
#define EXT_DFX_MSG_ID_BASE_SAL 0x7000 /* SAL DBK packet ID range: [0x7000, 0x8000) */
#define EXT_DFX_MSG_ID_BASE_SYS 0x8000 /* SYS DBK packet ID range [0x8000, 0xA000) */

/* System diagnosis ID range */
#define EXT_DFX_STAT_ID_BASE_SYS 0xA000  /* SYS STAT packet ID range: [0xA000, 0xF000) */
#define EXT_DFX_STAT_ID_MAX_SYS  0xF000
/****************************************************************************** */
/****************************************************************************** */
/* OS setting */
/****************************************************************************** */
#define EXT_SYS_TASK_TIME_SLICE (1)  /* tick number */
/****************************************************************************** */
#define PRODUCT_CFG_FEATURE_DIAG_RX
/****************************************************************************** */
/****************************************************************************** */
/* DIAG System-level message, statistics, and event settings */
/****************************************************************************** */
#define EXT_SYS_COMMAND_RPT_MSG  0xfffffff0
#define EXT_SYS_DIAG_BUSINIESS   0xffffff10
#define EXT_SYS_DIAG_NET         0xffffff20
#define EXT_SYS_DIAG_MSG_LEVEL0  0xffffff00
#define EXT_SYS_DIAG_MSG_LEVEL1  0xffffff01
#define EXT_SYS_DIAG_MSG_LEVEL2  0xffffff02
#define EXT_SYS_DIAG_STAT_LEVEL0 0xffffff00
#define EXT_SYS_DIAG_STAT_LEVEL1 0xffffff04
#define EXT_SYS_DIAG_STAT_LEVEL2 0xffffff08
#define EXT_SYS_DIAG_MASK        0x000000FF

/* MSG SYS and USR subclasses definition (16-bit) */
#define EXT_MSG_SYS_L0 0xff10 /* MSG SYS Level0 */
#define EXT_MSG_SYS_L1 0xff20 /* MSG SYS Level1 */
#define EXT_MSG_SYS_L2 0xff01 /* MSG SYS Level2 */
#define EXT_MSG_SYS_L3 0xff02 /* MSG SYS Level3 */
#define EXT_MSG_SYS_L4 0xff04 /* MSG SYS Level4 */

#define EXT_MSG_USR_L0 0xf3e8 /* MSG USR Level0 */
#define EXT_MSG_USR_L1 0xf3e9 /* MSG USR Level1 */
#define EXT_MSG_USR_L2 0xf3ea /* MSG USR Level2 */
#define EXT_MSG_USR_L3 0xf3eb /* MSG USR Level3 */
#define EXT_MSG_USR_L4 0xf3ec /* MSG USR Level4 */

/* Replace the following macro definitions with the preceding subclasses in the future: */
/* Level definitions */
#define EXT_SYS_DMID_BUSINESS_L0 ((EXT_SYS_DIAG_BUSINIESS | EXT_SYS_DIAG_MSG_LEVEL0) & EXT_SYS_DIAG_MASK)
#define EXT_SYS_DMID_BUSINESS_L1 ((EXT_SYS_DIAG_BUSINIESS | EXT_SYS_DIAG_MSG_LEVEL1) & EXT_SYS_DIAG_MASK)
#define EXT_SYS_DMID_BUSINESS_L2 ((EXT_SYS_DIAG_BUSINIESS | EXT_SYS_DIAG_MSG_LEVEL2) & EXT_SYS_DIAG_MASK)
#define EXT_SYS_DMID_NET_L0      (EXT_MSG_SYS_L0)
#define EXT_SYS_DMID_NET_L1      (EXT_MSG_SYS_L1)
#define EXT_SYS_DMID_NET_L2      (EXT_MSG_SYS_L2)
#define EXT_SYS_DSID_BUSINESS_L0 ((EXT_SYS_DIAG_BUSINIESS | EXT_SYS_DIAG_STAT_LEVEL0) & EXT_SYS_DIAG_MASK)
#define EXT_SYS_DSID_BUSINESS_L1 ((EXT_SYS_DIAG_BUSINIESS | EXT_SYS_DIAG_STAT_LEVEL1) & EXT_SYS_DIAG_MASK)
#define EXT_SYS_DSID_BUSINESS_L2 ((EXT_SYS_DIAG_BUSINIESS | EXT_SYS_DIAG_STAT_LEVEL2) & EXT_SYS_DIAG_MASK)
#define EXT_SYS_DSID_NET_L0      ((EXT_SYS_DIAG_BUSINIESS | EXT_SYS_DIAG_STAT_LEVEL0) & EXT_SYS_DIAG_MASK)
#define EXT_SYS_DSID_NET_L1      ((EXT_SYS_DIAG_BUSINIESS | EXT_SYS_DIAG_STAT_LEVEL1) & EXT_SYS_DIAG_MASK)
#define EXT_SYS_DSID_NET_L2      ((EXT_SYS_DIAG_BUSINIESS | EXT_SYS_DIAG_STAT_LEVEL2) & EXT_SYS_DIAG_MASK)
/****************************************************************************** */
/****************************************************************************** */
/* Other */
/****************************************************************************** */
#define EXT_STR_NOT_FOUND_CMD "D-FRM(%X) NOT SUPPORT"
/****************************************************************************** */
/****************************************************************************** */
/* Macro definitions related to version compatibility */
/****************************************************************************** */
#define EXT_SYS_RSD_INVALID_VALUE (0)

/* Bits 31-29 indicate the V version. A maximum of seven versions are supported.  */
/* 1 indicates 100. 2 indicates 200. Value range: [0, 6] */
/* Bits 28-26 indicate the R version. A maximum of seven versions are supported. Value range: [0, 6] */
/* Bits 25-20 indicate the C version. A maximum of 64 versions are supported. Value range: [0, 63] */
/* Bits 19-18 are reserved. */
/* Bits 17-8 indicate the B version. A maximum of 1024 versions are supported. Value range: [0,1023] */
/* Bits 7-0 indicate the SPC version. A maximum of 256 versions are supported. Value range: [0,255] */
#define ext_sys_inf_ver(_v, _r, _c, _b, _spc) ((((_v) & 0x07) << 29) | \
    (((_r) & 0x07) << 26) | (((_c) & 0x3F) << 20) | (((_b) & 0x03FF) << 8) | ((_spc) & 0x0FF))

#define ext_sys_inf_ver_v(_ver)   (((_ver) >> 29) & 0x07)
#define ext_sys_inf_ver_r(_ver)   (((_ver) >> 26) & 0x07)
#define ext_sys_inf_ver_c(_ver)   (((_ver) >> 20) & 0x3F)
#define ext_sys_inf_ver_b(_ver)   (((_ver) >> 8) & 0x03FF)
#define ext_sys_inf_ver_spc(_ver) (((_ver)) & 0x0FF)

#define ext_sys_inf_v100r001c00_bspc(_b, _spc) ext_sys_inf_ver(1, 1, 1, _b, _spc)

/* CMP.VER: VRCB define. */
#define EXT_SYS_INF_CMP_VER_V 100
#define EXT_SYS_INF_CMP_VER_R 1
#define EXT_SYS_INF_CMP_VER_C 1
#define EXT_SYS_INF_CMP_VER_B 19
/****************************************************************************** */
/****************************************************************************** */
/* Task Trace Interface Macros */
#define EXT_SYS_TT_FUNC_MAX_NUM 16
/****************************************************************************** */
/****************************************************************************** */
/* Definition and declaration of product user configuration */
/* Note: */
/* 1) Do not change the following names. */
/* 2) The following names are referenced in code. The suffixes _U, _OLD, and _NEW are not allowed. */
/****************************************************************************** */
#if defined(PRODUCT_CFG_SUPPORT_CONFIG_VARIABLE_DEFINITION)
#if defined(PRODUCT_CFG_SUPPORT_CONFIG_VARIABLE)
#endif
#else
#if defined(PRODUCT_CFG_SUPPORT_CONFIG_VARIABLE)
#endif
#endif

/********************** DIAG task settings ******************************************************** */
#define DIAG_PROC_TASK_STACK_SIZE (2048)
#define DIAG_QUEUE_MSG_NUM        (64)
#define DIAG_UART_TASK_PRIO       (4)
#define DIAG_PROC_TASK_PRIO       (5)

/****************************************************************************** */
#endif  /* __SOC_CONFIG_H__ */

