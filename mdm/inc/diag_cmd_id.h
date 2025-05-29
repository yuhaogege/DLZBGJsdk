/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: DIAG - cmd id.
 * Author: CompanyName
 * Create: 2018-10-31
 */

#ifndef __DIAG_CMD_ID_H__
#define __DIAG_CMD_ID_H__

#define DIAG_L1_A_ID_MIN  0x0050 /* [0x0050, 0x1000) */
#define DIAG_L1_A_ID_MAX  0x1000
#define DIAG_MAC_A_ID_MIN 0x1000 /* [0x1000, 0x2000) */
#define DIAG_MAC_A_ID_MAX 0x2000

#define DIAG_L1_B_ID_MIN  0x2100 /* [0x2100, 0x2900) */
#define DIAG_L1_B_ID_MAX  0x2900
#define DIAG_MAC_B_ID_MIN 0x2900 /* [0x2900, 0x3000) */
#define DIAG_MAC_B_ID_MAX 0x3000

#define DIAG_APP_A_ID_MIN 0x5600 /* [0x5600, 0x5E00) */
#define DIAG_APP_A_ID_MAX 0x5E00

#define DIAG_APP_B_ID_MIN 0x5E00 /* [0x5E00, 0x6600) */
#define DIAG_APP_B_ID_MAX 0x6600

#define DAIG_SAL_ID_MIN 0x7000
#define DIAG_SAL_ID_MAX 0x8000

#define DIAG_CMD_VER_QRY_1 (0x3013)
#define DIAG_CMD_SYS_QRY_1 (0x3014)
#define DIAG_CMD_SET_UART_SPEED (0x3104)

/**
 * First, initialization and system related DIAG commands [0x5000, 0x5200)
 */
#define DIAG_CMD_HOST_CONNECT    (0x50FF)
#define DIAG_CMD_HOST_DISCONNECT (0x5001)
#define DIAG_CMD_CONNECT_RANDOM  (0x5002)
#define DIAG_CMD_CONNECT_M_CHECK (0x5003)
#define DIAG_CMD_PWD_CHANGE      (0x5004)
#define DIAG_CMD_HEART_BEAT      (0x5005)
#define DIAG_CMD_PLC_TEST        (0x5805) /* delete node from white list */

#define DIAG_CMD_SIM_INIT_STATUS_IND (0x5050)
#define DIAG_CMD_UE_POWERON_IND      (0x5051)
#define DIAG_CMD_UE_POWEROFF_IND     (0x5052)
#define DIAG_CMD_TIMER_OUT_IND       (0x5053)
#define DIAG_CMD_UE_SYS_STATUS_IND   (0x5054)
#define DIAG_CMD_PORT_TEST_IND       (0x5055)
#define DIAG_CMD_VCOM_SPY            (0x5056)

/**
 * Second, equipment operation instructions [0x5200, 0x5300)
 */
#define DIAG_CMD_POWER_OFF      (0x5200) /* Shutdown */
#define DIAG_CMD_UE_RST         (0x5201) /* Restart the device */
#define DIAG_CMD_POWER_INFO_IND (0x5202)
#define DIAG_CMD_SCPU_START     (0x5203) /* Turn on the slave CPU (including DSP) */

/* UE read and write commands, such as memory, registers, Flash and other peripherals. */
#define DIAG_CMD_MEM_RD       (0x5203)
#define DIAG_CMD_MEM_WR       (0x5204)
#define DIAG_CMD_REG_RD       (0x5205)
#define DIAG_CMD_REG_WR       (0x5206)
#define DIAG_CMD_GPIO_RD      (0x5207)
#define DIAG_CMD_GPIO_WR      (0x5208)
#define DIAG_CMD_MEM_DUMP     (0x5209) /* Data DUMP */
#define DIAG_CMD_MEM_DUMP_IND (0x520A)
#define DIAG_CMD_M            (0x5220) /* M */
#define DIAG_CMD_D            (0x5221) /* D */

/**
 * DIAG_CMD_SW_VER_QRY: Software version information observable DIAG command
 * DIAG_CMD_HW_VER_QRY: Hardware version information observable DIAG command
 * DIAG_CMD_IMEI_QRY: Query IMEI number
 * DIAG_CMD_HW_INFO_QRY: device name in product version information
 * DIAG_CMD_SOC_INFO_QRY: Query the version of the chip
 */
#define DIAG_CMD_SW_VER_QRY   (0x520B)
#define DIAG_CMD_HW_VER_QRY   (0x520C)
#define DIAG_CMD_IMEI_QRY     (0x520D)
#define DIAG_CMD_HW_INFO_QRY  (0x520E)
#define DIAG_CMD_SOC_INFO_QRY (0x520F)
/* DIAG_CMD_BUF_CFG : LOG emergency and normal buffer settings, the purpose is to store buffered data in MUX */
#define DIAG_CMD_BUF_CFG    (0x5210)
#define DIAG_CMD_GETBUF_CFG (0x5211) /* DIAG_CMD_GETBUF_CFG : Get emergency and normal buffer settings */

#define DIAG_CMD_GET_UNLOCK_CODE (0x5212)

/**
 * Third, the log command [0x5300, 0x5400)
 */
/* Message configuration and reporting commands */
#define DIAG_CMD_MSG_CFG_SET_SYS (0x5310)
#define DIAG_CMD_MSG_CFG_SET_DEV (0x5311)
#define DIAG_CMD_MSG_CFG_SET_USR (0x5312)
#define DIAG_CMD_MSG_CFG_RST     (0x5313)
#define DIAG_CMD_MSG_RPT_SYS     (0x5314)
#define DIAG_CMD_MSG_RPT_LAYER   (0x5315)
#define DIAG_CMD_MSG_RPT_USR     (0x5316)

#define DIAG_CMD_VER_QRY (0x5318) /* ver command deleted */

#define DIAG_CMD_SYS_QRY     (0x5319) /* sdm_sq */
#define DIAG_CMD_SYSE_QRY_R  (0x5323) /* sdm_sq */
#define DIAG_CMD_SYS_END     (0x5325)
#define DIAG_CMD_CHL_SET     (0x5333)
#define DIAG_CMD_CHL_SET_END (0x5338)

/**
 * Fourth, the measurable command [0x5400, 0x5500)
 */
#define DIAG_CMD_GET_CPU_INFO (0x5400)

#define DIAG_CMD_DBK_AT_SWT       (0x5450)
#define DIAG_CMD_DBK_AT           (0x5451)
#define DIAG_CMD_MSP_SDM_QURY     (0x5452)
#define DIAG_CMD_MSP_SDM_QURY_IND (0x5453)
#define DIAG_CMD_GTR_SET          (0x5454)

/**
 * Five, NV read and write commands [0x5500, 0x5600)
 */
#define DIAG_CMD_NV_RD            (0x5500)
#define DIAG_CMD_NV_WR            (0x5501)
#define DIAG_CMD_NV_RD_IND        (0x5502)
#define DIAG_CMD_NV_QRY           (0x5503)
#define DIAG_CMD_NV_IMPORT        (0x5504)
#define DIAG_CMD_NV_EXPORT        (0x5505)
#define DIAG_CMD_NV_EXPORT_IND    (0x5506)
#define DIAG_CMD_NV_DEL           (0x5507)
#define DIAG_CMD_NV_FILE_LOAD     (0x5508)
#define DIAG_CMD_NV_FILE_UPDATE   (0x5509)
#define DIAG_CMD_NV_FILE_EXPORT   (0x550A)
#define DIAG_CMD_NV_UPDATE        (0x550B)
#define DIAG_CMD_NV_BACK          (0x550C)
#define DIAG_CMD_NV_REFRESH       (0x550D)
#define DIAG_CMD_NV_CHANGESYSMODE (0x550E)

#endif /* __DIAG_CMD_ID_H__ */

