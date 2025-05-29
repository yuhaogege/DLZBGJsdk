/*
 * Copyright (c) CompanyNameMagicTag 2019-2019. All rights reserved.
 * Description: This document describes error codes. An error code redefined as
 * "SOC_ROM_ERR_XXX" indicates that it is used in the ROM and cannot be changed.
 * Author: CompanyName
 * Create: 2019-10-31
 */

#ifndef __SOC_ERRNO_H__
#define __SOC_ERRNO_H__
/****************************************************************************** */
#include <soc_errno_rom.h>
/**
* Error codes at the system layer [0-3000) , -1
*/
#define  EXT_ERR_SUCCESS                                EXT_ROM_ERR_SUCCESS
#define  EXT_ERR_INVALID_PARAMETER                      EXT_ROM_ERR_INVALID_PARAMETER
#define  EXT_ERR_INVALID_ID                                (2)
#define  EXT_ERR_NO_INITILIZATION                       EXT_ROM_ERR_NO_INITILIZATION
#define  EXT_ERR_NOT_FOUND                                 (4)
#define  EXT_ERR_FULL                                      (7)
#define  EXT_ERR_INSUFFICIENT_BUFFER                       (8)
#define  EXT_ERR_CONSUMED                                  (9)
#define  EXT_ERR_CONTINUE                                  (10)
#define  EXT_ERR_TOO_SMALL_BUFFER                          (11)
#define  EXT_ERR_NOT_FREEE_SPACE                           (12)
#define  EXT_ERR_NO_MORE_DATA                              (13)
#define  EXT_ERR_MALLOC_FAILUE                          EXT_ROM_ERR_MALLOC_FAILUE
#define  EXT_ERR_MALLOC_FAILURE                         EXT_ROM_ERR_MALLOC_FAILUE
#define  EXT_ERR_NOT_ENOUGH_MEMORY                         (16)
#define  EXT_ERR_INVALID_HEAP_ADDR                         (17)
#define  EXT_ERR_SHUTDOWN                                  (18)
#define  EXT_ERR_NOT_CONNECT                               (19)
#define  EXT_ERR_BUSY                                      (20) /* The system is busy or a conflict occurs. */
#define  EXT_ERR_INITILIZATION                             (21) /* Initialization failed. */
#define  EXT_ERR_POWER_ON_PROCESS                          (22) /* Some resources are unavailable during boot. */
#define  EXT_ERR_SKIP                                      (23)
#define  EXT_ERR_NOT_REGISTER                              (24)
#define  EXT_ERR_TOO_LARGE_FRAME                           (25)
#define  EXT_ERR_NO_OS_RES_INIT   (26) /* Some resources are unavailable during OS resource initialization. */
#define  EXT_ERR_NO_INIT_APP_OS_RES                        (27)
#define  EXT_ERR_INITILIZATION_ALREADY                     (28)
#define  EXT_ERR_RAM_ALIGN                                 (30)
#define  EXT_ERR_NOT_SUPPORT                               (40)
#define  EXT_ERR_NOT_EXIST                                 (41)
#define  EXT_ERR_FEATURE_NOT_SUPPORT                       (42)
#define  EXT_ERR_INVALID_CHIP_TYPE                         (43)
#define  EXT_ERR_INVALID_PRODUCT_TYPE                      (44)
#define  EXT_ERR_INVALID_PRODUCT_CONFIG                    (45)
#define  EXT_ERR_WRITE_FLASH_FAILURE                       (46)
#define  EXT_ERR_ACCESS_DENIED                             (47)
#define  EXT_ERR_MEM_POOL_REQUEST_FAIL                     (48)
#define  EXT_ERR_NOT_SUPPORT_ID                            (49)
#define  EXT_ERR_UNAVAILABLE                               (50)

#define EXT_ERR_TIMEOUT       EXT_ROM_ERR_TIMEOUT
#define EXT_ERR_CFG_NOT_ALLOW (52)

#define EXT_ERR_BAD_DATA                (53)
#define EXT_ERR_HEAP_INIT_FAIL          (54)
#define EXT_ERR_OSRES_INIT_FAIL         (55)
#define EXT_ERR_TASK_INIT_FAIL          (56)
#define EXT_ERR_ADDR_ACCESS_FAILURE     (57)
#define EXT_ERR_INVALID_TYPE            (58)
#define EXT_ERR_TOO_LARGE_DATA          (59)
#define EXT_ERR_INVALID_CODE_ADDR       (60)
#define EXT_ERR_INVALID_RAM_ADDR        (61)
#define EXT_ERR_INVALID_PERIOD          (62)
#define EXT_ERR_BAD_FRAME               (63)
#define EXT_ERR_BAD_FRAME_SIZE          (64)
#define EXT_ERR_BAD_FRAME_PORT          (65)
#define EXT_ERR_OBJ_NOT_FOUND           (66)
#define EXT_ERR_NV_CFG_DISABLE          (67)
#define EXT_ERR_EXIST                   (68)
#define EXT_ERR_REGISTER_PARAM          (69)
#define EXT_ERR_AUTH_INVALID            (70)
#define EXT_ERR_CHECKSUM_INVALID        (71)
#define EXT_ERR_PROTO_INVALID           (72)
#define EXT_ERR_INTERRUPT               (73)
#define EXT_ERR_HW_PRODUCT_ID_NOT_EXIST (74)
#define EXT_ERR_NOT_ENOUGH_FD           (75)
#define EXT_ERR_INVOKE_SEQUENCE         (76)
#define EXT_ERR_ALREADY_DONE            (77)

#define  EXT_ERR_LOADFILE_NEED_FIRST_BLOCK                 (104) /* while bin file load,req need first block */
#define  EXT_ERR_DIAG_CMD_SIZE_INVALID                     (106)

#define EXT_ERR_P2P_IN_LINKING   (100)
#define EXT_ERR_P2P_NOT_LINK     (101)
#define EXT_ERR_P2P_IN_SEARCHING (102)
#define EXT_ERR_P2P_PUSH_FREQUENTLY (103)
#define EXT_ERR_QUEUE_FULL       (300)

#define  EXT_ERR_QUEUE_NOT_EXIST                           (301)
#define  EXT_ERR_INVALID_TASK_ID                           (302)
#define  EXT_ERR_INVALID_TASK_QUEUE                        (303)
#define  EXT_ERR_QUEUE_EMPTY                               (304)
#define  EXT_ERR_EVT_EMPTY                                 (306)
#define  EXT_ERR_APP_CHL_UNKNOWN                           (410)
/* The app channel is invalid (valid chip type or product form not found). */
#define  EXT_ERR_INVALID_TASK_PRIO                         (411)

#define  EXT_ERR_NV_NOT_SUPPORT_ID  (520) /*  The NV item does not exist. "NVIM Not Exist" */
#define  EXT_ERR_NV_ERROR_READ      (521) /*  Failed to read the NV item. "Read NVIM Failure" */
#define  EXT_ERR_NV_NOT_SUPPORT_LENTH                      (522)
/*  Failed to write the NV due to a too long length. "Write Error for Length Overflow" */
#define  EXT_ERR_NV_BAD_BLOCK                              (523)
/*  Failed to write the NV due to a bad flash block. "Write Error for Flash Bad Block" */
#define  EXT_ERR_NV_ERROR_WRITE                            (524)
/*  Failed to write the NV due to other errors. "Write Error for Unknown Reason" */
#define  EXT_ERR_NO_MORE_MEMORY    (558) /*  Insufficient memory "No Memory" */
#define  EXT_ERR_OS_MSG_BODY_NULL  (565) /* The received message body is NULL (should not be NULL). */
#define  EXT_ERR_NV_INVALID_INDEX  (566)

#define EXT_ERR_UART_SYS_CALL (601)

#define EXT_ERR_QUEUE_NOT_OPEN    (1000)
#define EXT_ERR_QUEUE_UNAVAILABLE (1001)
#define EXT_ERR_QUEUE_SHUTDOWN    (1002)

#define EXT_ERR_NV_ITEM_IS_EMPTY             (1100)
#define EXT_ERR_NV_FLASH_FULL                (1103)
#define EXT_ERR_NV_NOT_SUPPORT_UPDATA        (1104)
#define EXT_ERR_NV_NEED_BACK_FAIL            (1105)
#define EXT_ERR_NV_INVALID_NVFILE            (1106)
#define EXT_ERR_NV_INVALID_NVFILE_SECTOR_BAD (1107)
#define EXT_ERR_NV_ITEM_TOO_LARGE            (1108)

/* Upgrade error codes */
#define EXT_ERR_SUM_UPG_DRV_START_FAIL     1200
#define  EXT_ERR_SUM_NOT_NETWORKED                         (1205) /* The networking is not completed. */
#define  EXT_ERR_SUM_UPG_FILE_CRC_INCORRECT                (1206) /* The CRC of the upgrade file fails. */
#define EXT_ERR_SUM_STRUCTURE_MISMATCH     1207                   /* structure mismatch */

#define EXT_ERR_SUM_MEMSET_FAIL                 1211            /* calling memset function failed */
#define  EXT_ERR_SUM_INVALID_UPG_ID  1212 /* The upgrade ID does not match or it is not in the upgrade process. */
#define EXT_ERR_SUM_UPG_ID_NOT_MATCH 1213
#define EXT_ERR_SUM_LIST_NUM_ERROR   1214
#define  EXT_ERR_SUM_LIST_MEM_NOT_ENOUGH        1215 /* The board memory is insufficient (station list). */
#define  EXT_ERR_SUM_LIST_CRC_ERROR             1216 /* The CRC is inconsistent with the actual calculation. */
#define EXT_ERR_SUM_LIST_ERROR_STATUS           1217
#define  EXT_ERR_SUM_STA_AFFIRM_STATUS_NOT_MATCH           1218
/* The STA status does not meet the expectation for executing the upgrade. */
#define  EXT_ERR_SUM_STA_START_STATUS_NOT_MATCH            1219
/* The STA status does not meet the expectation for starting the upgrade. */
#define EXT_ERR_SUM_LIST_NUM_NOT_MATCH 1220 /* The total number of site lists is inconsistent with bitmap */
#define EXT_ERR_SUM_LIST_BUF_LEN_ERROR 1221 /* Site list cache is not enough */
#define EXT_ERR_SUM_SAME_START_REQ 1222 /* The start upgrade frame is the same. */
#define EXT_ERR_SUM_NUM_LEN_NOT_MATCH 1223 /* number and length do not match */
#define EXT_ERR_SUM_FLASH_READ_FAIL 1224 /* failed to read flash */
#define EXT_ERR_SUM_FILE_LEN_ERROR 1225 /* The length of the upgrade file exceeds the upper limit. */
#define EXT_ERR_SUM_OPTION_ERROR 1226 /* The upgrade options do not match. */
#define EXT_ERR_SUM_NOT_OBTAIN_CACHE 1227 /* The upgrade cache is not obtained. */
#define EXT_ERR_SUM_POINTER_NULL 1228 /* The pointer is null. */
#define EXT_ERR_SUM_LIST_UPG_NOT_ONLINE 1229 /* The site list contains unlisted sites */

/* DIAG */
#define EXT_ERR_DIAG_PRINT_PARSER_FAIL   (1300)
#define EXT_ERR_DIAG_PRINT_CAL_DATA_SIZE (1301)

#define EXT_ERR_NV_BACKUP_FILE_FAIL  (2001)
#define EXT_ERR_NV_BACKUP_DATA_FAIL  (2002)
#define EXT_ERR_NV_WRITE_FILE_FAIL   (2003)
#define EXT_ERR_NV_UPDATA_DATA_FAIL  (2004)
#define EXT_ERR_NV_UPDATA_FILE_FAIL  (2005)
#define EXT_ERR_NV_NOT_SUPPORT_WRITE (2007)
#define  EXT_ERR_NV_FSEC_TOTAL_NUM_INVALID                 (2008)  /* The number of factory NV items is invalid. */
#define EXT_ERR_NV_FAIL_N_TIMES           (2009)

#define EXT_ERR_SYSTEM_CALL_ERROR  (2999)
#define  EXT_ERR_SYSTEM_INIT_STATUS                        (3000) /* Initial status. No error occurred. */
#define EXT_ERR_ASSERT          (3001)
#define EXT_ERR_RAM_ADDR_ASSERT (3002)
#define EXT_ERR_FAILURE         EXT_ROM_ERR_FAILURE
#define EXT_ERR_SYSTEM          (td_u32)(-1)

#define EXT_ERR_MEMCPY_FAIL EXT_ERR_FAILURE
/**
* DRV layer [3000, 5000)
 */
/**
* CIPHER error code definitions
 */
#define    EXT_ERR_CIPHER_TRNG_INVALID_PARAMETER              EXT_ROM_ERR_CIPHER_TRNG_INVALID_PARAMETER
#define    EXT_ERR_CIPHER_TRNG_TIMEOUT                        EXT_ROM_ERR_CIPHER_TRNG_TIMEOUT
#define    EXT_ERR_CIPHER_HASH_MALLOC_FAIL                    EXT_ROM_ERR_CIPHER_HASH_MALLOC_FAIL
#define    EXT_ERR_CIPHER_HASH_SEM_CREATE_FAIL                EXT_ROM_ERR_CIPHER_HASH_SEM_CREATE_FAIL
#define    EXT_ERR_CIPHER_HASH_START_FAIL                     EXT_ROM_ERR_CIPHER_HASH_START_FAIL
#define    EXT_ERR_CIPHER_HASH_UPDATE_FAIL                    EXT_ROM_ERR_CIPHER_HASH_UPDATE_FAIL
#define    EXT_ERR_CIPHER_HASH_FINAL_FAIL                     EXT_ROM_ERR_CIPHER_HASH_FINAL_FAIL
#define    EXT_ERR_CIPHER_HASH_HMAC_KEYLEN_ERR                EXT_ROM_ERR_CIPHER_HASH_HMAC_KEYLEN_ERR
#define    EXT_ERR_CIPHER_HASH_HMAC_KEY_TIMEOUT_ERR           EXT_ROM_ERR_CIPHER_HASH_HMAC_KEY_TIMEOUT_ERR
#define    EXT_ERR_CIPHER_HASH_BUSY                           EXT_ROM_ERR_CIPHER_HASH_BUSY
#define    EXT_ERR_CIPHER_HASH_TIMEOUT_ERR                    EXT_ROM_ERR_CIPHER_HASH_TIMEOUT_ERR
#define    EXT_ERR_CIPHER_HASH_NOT_START                      EXT_ROM_ERR_CIPHER_HASH_NOT_START
#define    EXT_ERR_CIPHER_EFUSE_INVALIDATE_ID                 EXT_ROM_ERR_CIPHER_EFUSE_INVALIDATE_ID
#define    EXT_ERR_CIPHER_EFUSE_INVALIDATE_PARA               EXT_ROM_ERR_CIPHER_EFUSE_INVALIDATE_PARA
#define    EXT_ERR_CIPHER_EFUSE_WRITE_ERR                     EXT_ROM_ERR_CIPHER_EFUSE_WRITE_ERR
#define    EXT_ERR_CIPHER_EFUSE_INVALIDATE_AUTH               EXT_ROM_ERR_CIPHER_EFUSE_INVALIDATE_AUTH
#define    EXT_ERR_CIPHER_EFUSE_BUSY                          EXT_ROM_ERR_CIPHER_EFUSE_BUSY
#define    EXT_ERR_CIPHER_EFUSE_TIMEOUT                       EXT_ROM_ERR_CIPHER_EFUSE_TIMEOUT
#define    EXT_ERR_CIPHER_RSA_INVALIDATE_ID                   EXT_ROM_ERR_CIPHER_RSA_INVALIDATE_ID
#define    EXT_ERR_CIPHER_RSA_INVALIDATE_PARA                 EXT_ROM_ERR_CIPHER_RSA_INVALIDATE_PARA
#define    EXT_ERR_CIPHER_RSA_WRITE_ERR                       EXT_ROM_ERR_CIPHER_RSA_WRITE_ERR
#define    EXT_ERR_CIPHER_RSA_INVALIDATE_AUTH                 EXT_ROM_ERR_CIPHER_RSA_INVALIDATE_AUTH
#define    EXT_ERR_CIPHER_RSA_BUSY                            EXT_ROM_ERR_CIPHER_RSA_BUSY
#define    EXT_ERR_CIPHER_RSA_TIMEOUT                         EXT_ROM_ERR_CIPHER_RSA_TIMEOUT
#define    EXT_ERR_CIPHER_RSA_GET_RANDOM_ERR                  EXT_ROM_ERR_CIPHER_RSA_GET_RANDOM_ERR
#define    EXT_ERR_CIPHER_RSA_FAILED_DECRYPT    	             EXT_ROM_ERR_CIPHER_RSA_FAILED_DECRYPT
#define    EXT_ERR_CIPHER_RSA_FAILED_ENCRYPT    	             EXT_ROM_ERR_CIPHER_RSA_FAILED_ENCRYPT
#define    EXT_ERR_CIPHER_RSA_CALC_ERR                        EXT_ROM_ERR_CIPHER_RSA_CALC_ERR
#define    EXT_ERR_CIPHER_RSA_CALC_CMP_ERR                    EXT_ROM_ERR_CIPHER_RSA_CALC_CMP_ERR
#define    EXT_ERR_CIPHER_RSA_MALLOC_ERR 		                 EXT_ROM_ERR_CIPHER_RSA_MALLOC_ERR
#define    EXT_ERR_CIPHER_RSA_SEM_ERR 		                 EXT_ROM_ERR_CIPHER_RSA_SEM_ERR
#define    EXT_ERR_CIPHER_RSA_INVALID_POINT 		             EXT_ROM_ERR_CIPHER_RSA_INVALID_POINT
#define    EXT_ERR_CIPHER_RSA_HASH_INIT_ERR 		             EXT_ROM_ERR_CIPHER_RSA_HASH_INIT_ERR
#define    EXT_ERR_CIPHER_AES_INVALIDATE_PARA                 EXT_ROM_ERR_CIPHER_AES_INVALIDATE_PARA
#define    EXT_ERR_CIPHER_AES_TIMEOUT                         EXT_ROM_ERR_CIPHER_AES_TIMEOUT
#define    EXT_ERR_CIPHER_AES_BUSY                            EXT_ROM_ERR_CIPHER_AES_BUSY
#define    EXT_ERR_CIPHER_AES_START_ERR                       EXT_ROM_ERR_CIPHER_AES_START_ERR
#define    EXT_ERR_CIPHER_AES_MALLOC_ERR                      EXT_ROM_ERR_CIPHER_AES_MALLOC_ERR
#define    EXT_ERR_CIPHER_AES_SEM_ERR                         EXT_ROM_ERR_CIPHER_AES_SEM_ERR

/**
* GPIO error code definitions
 */
#define EXT_ERR_GPIO_GROUP_NUMBER_BEYOUND           EXT_ROM_ERR_GPIO_GROUP_NUMBER_BEYOUND
#define EXT_ERR_GPIO_BIT_NUMBER_BEYOUND             EXT_ROM_ERR_GPIO_BIT_NUMBER_BEYOUND
#define EXT_ERR_GPIO_DIRECTION_NOT_MATCH            EXT_ROM_ERR_GPIO_DIRECTION_NOT_MATCH
#define EXT_ERR_GPIO_DIRECTION_INVALID              EXT_ROM_ERR_GPIO_DIRECTION_INVALID
#define EXT_ERR_GPIO_NOT_INIT                       EXT_ROM_ERR_GPIO_NOT_INIT
#define EXT_ERR_GPIO_FAILED_INIT                    EXT_ROM_ERR_GPIO_FAILED_INIT
#define EXT_ERR_GPIO_REPEAT_INIT                    EXT_ROM_ERR_GPIO_REPEAT_INIT
#define EXT_ERR_GPIO_POINTER_NULL                   EXT_ROM_ERR_GPIO_POINTER_NULL
#define EXT_ERR_GPIO_PARAMETER_ERROR                EXT_ROM_ERR_GPIO_PARAMETER_ERROR
#define EXT_ERR_GPIO_PORT_OVERFLOW_FAILED           EXT_ROM_ERR_GPIO_PORT_OVERFLOW_FAILED
#define EXT_ERR_GPIO_INVALID_BUF                    EXT_ROM_ERR_GPIO_INVALID_BUF
#define EXT_ERR_GPIO_PIN_OVERFLOW_FAILED            EXT_ROM_ERR_GPIO_PIN_OVERFLOW_FAILED
#define EXT_ERR_GPIO_INTERRUPT_BIT_DISABLE_FAILED   EXT_ROM_ERR_GPIO_INTERRUPT_BIT_DISABLE_FAILED
#define EXT_ERR_GPIO_INTERRUPT_MODE_OVERFLOW_FAILED EXT_ROM_ERR_GPIO_INTERRUPT_MODE_OVERFLOW_FAILED
#define EXT_ERR_GPIO_INTERRUPT_MODE_FAILED          EXT_ROM_ERR_GPIO_INTERRUPT_MODE_FAILED
#define EXT_ERR_GPIO_CURR_PIN_BUSY                  EXT_ROM_ERR_GPIO_CURR_PIN_BUSY
#define EXT_ERR_GPIO_INVALID_SUSPEND                EXT_ROM_ERR_GPIO_INVALID_SUSPEND

/**
* SPI error code definitions
 */
#define EXT_ERR_SPI_PARAMETER_WRONG        EXT_ROM_ERR_SPI_PARAMETER_WRONG
#define EXT_ERR_SPI_NOT_ENOUGH_SOFT_HANDLE EXT_ROM_ERR_SPI_NOT_ENOUGH_SOFT_HANDLE
#define EXT_ERR_SPI_WRITE_TIMEOUT          EXT_ROM_ERR_SPI_WRITE_TIMEOUT
#define EXT_ERR_SPI_READ_TIMEOUT           EXT_ROM_ERR_SPI_READ_TIMEOUT
#define EXT_ERR_SPI_HW_NOT_OK              EXT_ROM_ERR_SPI_HW_NOT_OK
#define EXT_ERR_SPI_RESTORE_INVALID_FD     EXT_ROM_ERR_SPI_RESTORE_INVALID_FD
#define EXT_ERR_SPI_INVALID_FD             EXT_ROM_ERR_SPI_INVALID_FD
#define EXT_ERR_SPI_DEV_CLOSED             EXT_ROM_ERR_SPI_DEV_CLOSED
#define EXT_ERR_SPI_REINIT                 EXT_ROM_ERR_SPI_REINIT

/**
* CRC error code definitions
 */
#define EXT_ERR_CRC_BAD_PARA        (3401)
#define EXT_ERR_CRC_INVALID_SUSPEND (3402)

/**
* LED error code subcode definitions
 */
#define EXT_ERR_LED_PARAMETER_ERROR                 EXT_ROM_ERR_LED_PARAMETER_ERROR

/**
* WATCHDOG error code definitions
 */
#define EXT_ERR_WATCHDOG_PARA_ERROR                 EXT_ROM_ERR_WATCHDOG_PARA_ERROR

/**
* I2C error code definitions
 */
#define     EXT_ERR_I2C_NOT_INIT                     EXT_ROM_ERR_I2C_NOT_INIT
#define     EXT_ERR_I2C_INVALID_PARAMETER            EXT_ROM_ERR_I2C_INVALID_PARAMETER
#define     EXT_ERR_I2C_TIMEOUT_START                EXT_ROM_ERR_I2C_TIMEOUT_START
#define     EXT_ERR_I2C_TIMEOUT_WAIT                 EXT_ROM_ERR_I2C_TIMEOUT_WAIT
#define     EXT_ERR_I2C_TIMEOUT_STOP                 EXT_ROM_ERR_I2C_TIMEOUT_STOP
#define     EXT_ERR_I2C_TIMEOUT_RCV_BYTE             EXT_ROM_ERR_I2C_TIMEOUT_RCV_BYTE
#define     EXT_ERR_I2C_TIMEOUT_RCV_BYTE_PROC        EXT_ROM_ERR_I2C_TIMEOUT_RCV_BYTE_PROC
#define     EXT_ERR_I2C_WAIT_SEM_FAIL                EXT_ROM_ERR_I2C_WAIT_SEM_FAIL
#define     EXT_ERR_I2C_START_ACK_ERR                EXT_ROM_ERR_I2C_START_ACK_ERR
#define     EXT_ERR_I2C_WAIT_ACK_ERR                 EXT_ROM_ERR_I2C_WAIT_ACK_ERR

/**
* FLASH error code subcode definitions
 */
#define EXT_ERR_FLASH_NOT_INIT                       EXT_ROM_ERR_FLASH_NOT_INIT
#define EXT_ERR_FLASH_INVALID_PARAM                  EXT_ROM_ERR_FLASH_INVALID_PARAM
#define EXT_ERR_FLASH_INVALID_PARAM_BEYOND_ADDR      EXT_ROM_ERR_FLASH_INVALID_PARAM_BEYOND_ADDR
#define EXT_ERR_FLASH_INVALID_PARAM_SIZE_ZERO        EXT_ROM_ERR_FLASH_INVALID_PARAM_SIZE_ZERO
#define EXT_ERR_FLASH_INVALID_PARAM_ERASE_NOT_ALIGN  EXT_ROM_ERR_FLASH_INVALID_PARAM_ERASE_NOT_ALIGN
#define EXT_ERR_FLASH_INVALID_PARAM_IOCTRL_DATA_NULL EXT_ROM_ERR_FLASH_INVALID_PARAM_IOCTL_DATA_NULL
#define EXT_ERR_FLASH_INVALID_PARAM_DATA_NULL        EXT_ROM_ERR_FLASH_INVALID_PARAM_DATA_NULL
#define EXT_ERR_FLASH_INVALID_PARAM_PAD1             EXT_ROM_ERR_FLASH_INVALID_PARAM_PAD1
#define EXT_ERR_FLASH_INVALID_PARAM_PAD2             EXT_ROM_ERR_FLASH_INVALID_PARAM_PAD2
#define EXT_ERR_FLASH_INVALID_PARAM_PAD3             EXT_ROM_ERR_FLASH_INVALID_PARAM_PAD3
#define EXT_ERR_FLASH_INVALID_PARAM_PAD4             EXT_ROM_ERR_FLASH_INVALID_PARAM_PAD4
#define EXT_ERR_FLASH_TIME_OUT_WAIT_READY            EXT_ROM_ERR_FLASH_TIME_OUT_WAIT_READY
#define EXT_ERR_FLASH_QUAD_MODE_READ_REG1            EXT_ROM_ERR_FLASH_QUAD_MODE_READ_REG1
#define EXT_ERR_FLASH_QUAD_MODE_READ_REG2            EXT_ROM_ERR_FLASH_QUAD_MODE_READ_REG2
#define EXT_ERR_FLASH_QUAD_MODE_COMPARE_REG          EXT_ROM_ERR_FLASH_QUAD_MODE_COMPARE_REG
#define EXT_ERR_FLASH_NO_MATCH_FLASH                 EXT_ROM_ERR_FLASH_NO_MATCH_FLASH
#define EXT_ERR_FLASH_WRITE_ENABLE                   EXT_ROM_ERR_FLASH_WRITE_ENABLE
#define EXT_ERR_FLASH_NO_MATCH_ERASE_SIZE            EXT_ROM_ERR_FLASH_NO_MATCH_ERASE_SIZE
#define EXT_ERR_FLASH_MAX_SPI_OP                     EXT_ROM_ERR_FLASH_MAX_SPI_OP
#define EXT_ERR_FLASH_NOT_SUPPORT_IOCTRL_ID          EXT_ROM_ERR_FLASH_NOT_SUPPORT_IOCTL_ID
#define EXT_ERR_FLASH_INVALID_CHIP_ID                EXT_ROM_ERR_FLASH_INVALID_CHIP_ID
#define EXT_ERR_FLASH_RE_INIT                        EXT_ROM_ERR_FLASH_RE_INIT
#define EXT_ERR_FLASH_WRITE_NOT_SUPPORT_ERASE        EXT_ROM_ERR_FLASH_WRITE_NOT_SUPPORT_ERASE
#define EXT_ERR_FLASH_WRITE_COMPARE_WRONG            EXT_ROM_ERR_FLASH_WRITE_COMPARE_WRONG
#define EXT_ERR_FLASH_WAIT_CFG_START_TIME_OUT        EXT_ROM_ERR_FLASH_WAIT_CFG_START_TIME_OUT
#define EXT_ERR_FLASH_PROTECT_BOOT_COMPATIBILITY     (3725)
#define EXT_ERR_FLASH_PROTECT_WRITE_STATUS_REG       (3726)
#define EXT_ERR_FLASH_INVALID_RAM_ADDR               (3727)

/**
* UART error code subcode definitions
 */
#define EXT_ERR_UART_INVALID_SUSPEND   (3801)
#define EXT_ERR_UART_INVALID_PARITY    (3802)
#define EXT_ERR_UART_INVALID_DATA_BITS (3803)
#define EXT_ERR_UART_INVALID_STOP_BITS (3804)
#define EXT_ERR_UART_INVALID_BAUD      (3805)
#define EXT_ERR_UART_INVALID_COM_PORT  (3806)


#define EXT_ERR_DRV_UPG_NO_VERSION        (3900)
#define EXT_ERR_DRV_BLK_INVALID           (3901)
#define EXT_ERR_DRV_UPG_FILE_INVALID      (3902)
#define EXT_ERR_DRV_UPG_CODE_FILE_INVALID (3903)
#define EXT_ERR_DRV_UPG_NV_FILE_INVALID   (3904)
#define EXT_ERR_DRV_UPG_FLASH_BAD         (3905)
#define EXT_ERR_DRV_UPG_BUSY              (3906)
#define EXT_ERR_DRV_UPG_UPG_ID_INVALID    (3907)
#define EXT_ERR_DRV_UPG_VERSION_OLDER     (3908)

#define EXT_ERR_LOW_POWER_INVALID_PARAM (3950)

/* Range of data collection error codes (4600) */
#define EXT_ERR_COLLECT_DATA_OVERFLOW_ERR             4600 /* The chip displays an overflow error. */
#define EXT_ERR_COLLECT_DATA_OVERFLOW_SW_CHECK        4601 /* The length parameter configured after the software
                                                         * determines the trigger point exceeds the maximum
                                                         * length of the data collection range.
                                                         */


/****************************************************************************** */
/* MAC layer [5000, 6000) */
/****************************************************************************** */
#define EXT_ERR_NOT_FOUND_NETWORK    (5000) /* The NDC fails to find the network. */
#define EXT_ERR_NDM_VER              (5001) /* The NDM version cannot be compatible with the CCO/STA version. */
#define EXT_ERR_MULTI_APP            (5002) /* Too many users */
#define EXT_ERR_STOP_SEARCH_NETWORK  (5004) /* Network search stops. */
#define EXT_ERR_MAC_SNID_CHANGING    (5005)
#define EXT_ERR_TF_IDENTIFY_DOING    (5006) /* Identifying the transformer district */
#define EXT_ERR_IN_SEIZE_CNNC_STATUS 5007   /* is seized by other device */
#define EXT_ERR_IS_BUSY              5008   /* need use seize mode */
#define EXT_ERR_LOST_CNNC            5009   /* lost connnect */
#define EXT_ERR_IS_CONNECTED         5010   /* is already in connected */
#define EXT_ERR_MONITORING           5011   /* is already in monitoring */

/**
* Task error code definitions
 */
#define EXT_ERR_TASK_INVALID_PARAM          EXT_ROM_ERR_TASK_INVALID_PARAM
#define EXT_ERR_TASK_CREATE_FAIL            EXT_ROM_ERR_TASK_CREATE_FAIL
#define EXT_ERR_TASK_DELETE_FAIL            EXT_ROM_ERR_TASK_DELETE_FAIL
#define EXT_ERR_TASK_SUPPEND_FAIL           EXT_ROM_ERR_TASK_SUPPEND_FAIL
#define EXT_ERR_TASK_RESUME_FAIL            EXT_ROM_ERR_TASK_RESUME_FAIL
#define EXT_ERR_TASK_GET_PRI_FAIL           EXT_ROM_ERR_TASK_GET_PRI_FAIL
#define EXT_ERR_TASK_SET_PRI_FAIL           EXT_ROM_ERR_TASK_SET_PRI_FAIL
#define EXT_ERR_TASK_LOCK_FAIL              EXT_ROM_ERR_TASK_LOCK_FAIL
#define EXT_ERR_TASK_UNLOCK_FAIL            EXT_ROM_ERR_TASK_UNLOCK_FAIL
#define EXT_ERR_TASK_DELAY_FAIL             EXT_ROM_ERR_TASK_DELAY_FAIL
#define EXT_ERR_TASK_GET_INFO_FAIL          EXT_ROM_ERR_TASK_GET_INFO_FAIL
#define EXT_ERR_TASK_REGISTER_SCHEDULE_FAIL EXT_ROM_ERR_TASK_REGISTER_SCHEDULE_FAIL
/**
* Message error code definitions
 */
#define EXT_ERR_MSG_INVALID_PARAM   EXT_ROM_ERR_MSG_INVALID_PARAM
#define EXT_ERR_MSG_CREATE_Q_FAIL   EXT_ROM_ERR_MSG_CREATE_Q_FAIL
#define EXT_ERR_MSG_DELETE_Q_FAIL   EXT_ROM_ERR_MSG_DELETE_Q_FAIL
#define EXT_ERR_MSG_WAIT_FAIL       EXT_ROM_ERR_MSG_WAIT_FAIL
#define EXT_ERR_MSG_SEND_FAIL       EXT_ROM_ERR_MSG_SEND_FAIL
#define EXT_ERR_MSG_GET_Q_INFO_FAIL EXT_ROM_ERR_MSG_GET_Q_INFO_FAIL
#define EXT_ERR_MSG_Q_DELETE_FAIL   EXT_ROM_ERR_MSG_Q_DELETE_FAIL
#define EXT_ERR_MSG_WAIT_TIME_OUT   EXT_ROM_ERR_MSG_WAIT_TIME_OUT
/**
* Semaphore error code definitions
 */
#define EXT_ERR_SEM_INVALID_PARAM EXT_ROM_ERR_SEM_INVALID_PARAM
#define EXT_ERR_SEM_CREATE_FAIL   EXT_ROM_ERR_SEM_CREATE_FAIL
#define EXT_ERR_SEM_DELETE_FAIL   EXT_ROM_ERR_SEM_DELETE_FAIL
#define EXT_ERR_SEM_WAIT_FAIL     EXT_ROM_ERR_SEM_WAIT_FAIL
#define EXT_ERR_SEM_SIG_FAIL      EXT_ROM_ERR_SEM_SIG_FAIL
#define EXT_ERR_SEM_WAIT_TIME_OUT EXT_ROM_ERR_SEM_WAIT_TIME_OUT
/**
* Mutex error code definitions
 */
#define EXT_ERR_MUX_INVALID_PARAM EXT_ROM_ERR_MUX_INVALID_PARAM
#define EXT_ERR_MUX_CREATE_FAIL   EXT_ROM_ERR_MUX_CREATE_FAIL
#define EXT_ERR_MUX_DELETE_FAIL   EXT_ROM_ERR_MUX_DELETE_FAIL
#define EXT_ERR_MUX_PEND_FAIL     EXT_ROM_ERR_MUX_PEND_FAIL
#define EXT_ERR_MUX_POST_FAIL     EXT_ROM_ERR_MUX_POST_FAIL

/**
* Interrupt error code definitions
 */
#define EXT_ERR_ISR_INVALID_PARAM       EXT_ROM_ERR_ISR_INVALID_PARAM
#define EXT_ERR_ISR_REQ_IRQ_FAIL        EXT_ROM_ERR_ISR_REQ_IRQ_FAIL
#define EXT_ERR_ISR_ADD_JOB_MALLOC_FAIL EXT_ROM_ERR_ISR_ADD_JOB_MALLOC_FAIL
#define EXT_ERR_ISR_ADD_JOB_SYS_FAIL    EXT_ROM_ERR_ISR_ADD_JOB_SYS_FAIL
#define EXT_ERR_ISR_DEL_IRQ_FAIL        EXT_ROM_ERR_ISR_DEL_IRQ_FAIL

/**
* Event error code definitions
 */
#define EXT_ERR_EVENT_INVALID_PARAM     EXT_ROM_ERR_EVENT_INVALID_PARAM
#define EXT_ERR_EVENT_CREATE_NO_HADNLE  EXT_ROM_ERR_EVENT_CREATE_NO_HADNLE
#define EXT_ERR_EVENT_CREATE_SYS_FAIL   EXT_ROM_ERR_EVENT_CREATE_SYS_FAIL
#define EXT_ERR_EVENT_SEND_FAIL         EXT_ROM_ERR_EVENT_SEND_FAIL
#define EXT_ERR_EVENT_WAIT_FAIL         EXT_ROM_ERR_EVENT_WAIT_FAIL
#define EXT_ERR_EVENT_CLEAR_FAIL        EXT_ROM_ERR_EVENT_CLEAR_FAIL
#define EXT_ERR_EVENT_RE_INIT           EXT_ROM_ERR_EVENT_RE_INIT
#define EXT_ERR_EVENT_NOT_ENOUGH_MEMORY EXT_ROM_ERR_EVENT_NOT_ENOUGH_MEMORY
#define EXT_ERR_EVENT_NOT_INIT          EXT_ROM_ERR_EVENT_NOT_INIT
#define EXT_ERR_EVENT_DELETE_FAIL       EXT_ROM_ERR_EVENT_DELETE_FAIL
#define EXT_ERR_EVENT_WAIT_TIME_OUT     EXT_ROM_ERR_EVENT_WAIT_TIME_OUT

/**
* Memory management error code definitions
 */
#define EXT_ERR_MEM_INVALID_PARAM                 EXT_ROM_ERR_MEM_INVALID_PARAM
#define EXT_ERR_MEM_CREAT_POOL_FAIL               EXT_ROM_ERR_MEM_CREAT_POOL_FAIL
#define EXT_ERR_MEM_CREATE_POOL_NOT_ENOUGH_HANDLE EXT_ROM_ERR_MEM_CREATE_POOL_NOT_ENOUGH_HANDLE
#define EXT_ERR_MEM_FREE_FAIL                     EXT_ROM_ERR_MEM_FREE_FAIL
#define EXT_ERR_MEM_RE_INIT                       EXT_ROM_ERR_MEM_RE_INIT
#define EXT_ERR_MEM_NOT_INIT                      EXT_ROM_ERR_MEM_NOT_INIT
#define EXT_ERR_MEM_CREAT_POOL_MALLOC_FAIL        EXT_ROM_ERR_MEM_CREAT_POOL_MALLOC_FAIL
#define EXT_ERR_MEM_GET_INFO_FAIL                 EXT_ROM_ERR_MEM_GET_INFO_FAIL
#define EXT_ERR_MEM_GET_OS_INFO_NOK               EXT_ROM_ERR_MEM_GET_OS_INFO_NOK

/**
* Timer error code definitions
 */
#define EXT_ERR_TIMER_INVALID_PARAM     EXT_ROM_ERR_TIMER_INVALID_PARAM
#define EXT_ERR_TIMER_CREATE_TIMER_FAIL EXT_ROM_ERR_TIMER_CREATE_TIMER_FAIL

/**
* CPU usage and call track record error code definitions
 */
#define EXT_ERR_CPUP_NOT_INIT      (6140)
#define EXT_ERR_CPUP_INVALID_PARAM (6141)

/**
* Dynamic loading error code definitions
 */
#define EXT_ERR_REMAP_FAULT                       (7001)
#define EXT_ERR_REMAP_GET_ENTRY_FAULT             (7002)
#define EXT_ERR_REMAP_ERROR_INPUT                 (7003)
#define EXT_ERR_REMAP_ERROR_UNMAPED               (7004)
#define EXT_ERR_REMAP_OTHER_FAULT                 (7005)
#define EXT_ERR_REMAP_INVALID_FAULT_ADDR          (7006) /* Invalid address */
#define EXT_ERR_REMAP_ALREADY_REMAPED             (7007)
#define EXT_ERR_REMAP_ISR_OR_INTLOCK              (7008)
#define EXT_ERR_REMAP_RE_ENTRY                    (7009)
#define EXT_ERR_REMAP_GET_VIR_ENTRY_FAIL          (7010)
#define EXT_ERR_REMAP_VIR_ENTRY_MAPPED_PHY_ENTRY  (7011)
#define EXT_ERR_REMAP_LRUK_NOT_FOUND_ENTRY        (7012)
#define EXT_ERR_REMAP_COPY_DATA_INVALID_VIR_ENTRY (7013)
#define EXT_ERR_REMAP_OTHER_1                     (7014)
#define EXT_ERR_REMAP_OTHER_2                     (7015)
#define EXT_ERR_REMAP_INIT_VIR_ENTRY_NOT_ENOUGH   (7016)
#define EXT_ERR_REMAP_INIT_TASK_CREATE_FAIL       (7017)
#define EXT_ERR_REMAP_INIT_EVENT_CREATE_FAIL      (7018)

/**
* FPB error code definitions
 */
#define EXT_ERR_FPB_COMP_REPEAT    (7100)
#define EXT_ERR_FPB_NO_COMP        (7101)
#define EXT_ERR_FPB_TYPE           (7102)
#define EXT_ERR_FPB_NO_FREE_COMP   (7103)
#define EXT_ERR_FPB_ADDR_NOT_ALIGN (7104)
#define EXT_ERR_FPB_TARGET_ADDR    (7105)

/**
* Upgrade error code definitions
 */
#define EXT_ERR_UPG_START_1_CHECK_UPG_HEAD             (7510)
/* Upgrade module: The upgrade file check fails due to an incorrect SoC code. */
#define EXT_ERR_UPG_HEAD_CHECK_MAGIC_WRONG             (EXT_ERR_UPG_START_1_CHECK_UPG_HEAD + 0)
/* Upgrade module: The upgrade file check fails due to an incorrect manufacturer code. */
#define EXT_ERR_UPG_HEAD_CHECK_MANUCODE_WRONG          (EXT_ERR_UPG_START_1_CHECK_UPG_HEAD + 1)
/* Upgrade module: The upgrade file check fails due to an incorrect file type. */
#define EXT_ERR_UPG_HEAD_CHECK_FILE_TYPE_WRONG         (EXT_ERR_UPG_START_1_CHECK_UPG_HEAD + 2)
/* Upgrade module: The upgrade file check fails due to an incorrect file length. */
#define EXT_ERR_UPG_HEAD_CHECK_FILE_LEN_WRONG          (EXT_ERR_UPG_START_1_CHECK_UPG_HEAD + 3)
/* Upgrade module: The upgrade file check fails due to an incorrect product type. */
#define EXT_ERR_UPG_HEAD_CHECK_PRODUCT_LIST_WRONG      (EXT_ERR_UPG_START_1_CHECK_UPG_HEAD + 4)
/* Upgrade module: The upgrade file check fails due to unmatched flash partitioning. */
#define EXT_ERR_UPG_HEAD_CHECK_PARTION_NOT_SAME        (EXT_ERR_UPG_START_1_CHECK_UPG_HEAD + 5)
/* Upgrade module: The upgrade file check fails due to unmatched flash partitioning and
upgrade based on in the current partition version is not supported. */
#define EXT_ERR_UPG_HEAD_CHECK_PARTION_NOT_SUPPORT_UPG (EXT_ERR_UPG_START_1_CHECK_UPG_HEAD + 7)
/* Upgrade module: The upgrade file check fails because branches are executed. */
#define EXT_ERR_UPG_HEAD_CHECK_PARTION_NO_GO           (EXT_ERR_UPG_START_1_CHECK_UPG_HEAD + 6)
/* Abandoned error code */
#define EXT_ERR_UPG_HEAD_CHECK_NOT_SURE                (EXT_ERR_UPG_START_1_CHECK_UPG_HEAD + 8)
#define EXT_ERR_UPG_START_2_TRANS_FILE           (7530)
#define EXT_ERR_UPG_TRANS_CHECK_FILE_FAIL        (EXT_ERR_UPG_START_2_TRANS_FILE + 0)
#define EXT_ERR_UPG_TRANS_SAVE_CONTENT_OUT_RANGE (EXT_ERR_UPG_START_2_TRANS_FILE + 1)
#define EXT_ERR_UPG_TRANS_SAVE_INVALID_PARAM     (EXT_ERR_UPG_START_2_TRANS_FILE + 2)

#define EXT_ERR_UPG_START_3_OTHER             (7550)
#define EXT_ERR_UPG_ERR_INVALID_PARAM         (EXT_ERR_UPG_START_3_OTHER + 0)
/* Abandoned error code */
#define EXT_ERR_UPG_ERR_MALLOC_FAIL           (EXT_ERR_UPG_START_3_OTHER + 1)
/* Upgrade module: Failed to apply for memory. */
#define EXT_ERR_UPG_FILE_CHECK_SUM_WRONG      (EXT_ERR_UPG_START_3_OTHER + 2)
/* Upgrade module: The file HASH check fails. */
#define EXT_ERR_UPG_INVALID_UPG_TYPE          (EXT_ERR_UPG_START_3_OTHER + 3)
/* Abandoned error code */
#define EXT_ERR_UPG_CHECK_UPG_END_MAGIC_FAIL  (EXT_ERR_UPG_START_3_OTHER + 4)
/* Upgrade module: wrong file tail magic word */
#define EXT_ERR_UPG_CHECK_UPG_END_OFFSET_FAIL (EXT_ERR_UPG_START_3_OTHER + 5)
/* Upgrade module: wrong file tail file length */
#define EXT_ERR_UPG_REFRESH_NV_MALLOC_FAIL    (EXT_ERR_UPG_START_3_OTHER + 6)
/* Upgrade module: Failed to apply for memory during NV update. */
#define EXT_ERR_PARTITION_INVALID_PARAM           (7600)
#define EXT_ERR_PARTITION_UPDATA_TBL_SAME         (7601)
#define EXT_ERR_PARTITION_UPDATA_VER_SAME         (7602)
#define EXT_ERR_PARTITION_UPDATA_VER_ERR          (7603)
#define EXT_ERR_PARTITION_UPDATA_NOT_SUPPORT_ARER (7604)
#define EXT_ERR_PARTITION_SEM_NOT_INIT            (7605)
#define EXT_ERR_PARTITION_MALLOC_FAIL             (EXT_ERR_MALLOC_FAILUE)

/****************************************************************************** */
/****************************************************************************** */
/* Extended */
/****************************************************************************** */
/**
* The maximum error code is 0xFFFF.
 */
#define EXT_ERR_MAX  0xFFFF
/****************************************************************************** */
/****************************************************************************** */
#endif  /* __SOC_ERRNO_H__ */

