/**
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: ROM usage error code.
 */

#ifndef __SOC_ERRNO_ROM_H__
#define __SOC_ERRNO_ROM_H__

/**
 * System layer [0-3000) , -1
 */
#define EXT_ROM_ERR_SUCCESS (0)           /* Success */
#define EXT_ROM_ERR_INVALID_PARAMETER (1) /* Common error: Invalid parameter */
#define EXT_ROM_ERR_NO_INITILIZATION (3)  /* Common error: Initialized module */
#define EXT_ROM_ERR_MALLOC_FAILUE (15)    /* Common error: Insufficient memory space */
#define EXT_ROM_ERR_TIMEOUT (51)          /* Common error: Timeout */
#define EXT_ROM_ERR_FAILURE (td_u32)(-1)  /* Common error: Failure */

/**
 * DRV layer [3000, 5000)
 */
/**
 * CIPHER error code definitions
 */
#define EXT_ROM_ERR_CIPHER_TRNG_INVALID_PARAMETER (3000)
#define EXT_ROM_ERR_CIPHER_TRNG_TIMEOUT (3001)
#define EXT_ROM_ERR_CIPHER_HASH_MALLOC_FAIL (3002)
#define EXT_ROM_ERR_CIPHER_HASH_SEM_CREATE_FAIL (3003)
#define EXT_ROM_ERR_CIPHER_HASH_START_FAIL (3004)
#define EXT_ROM_ERR_CIPHER_HASH_UPDATE_FAIL (3005)
#define EXT_ROM_ERR_CIPHER_HASH_FINAL_FAIL (3006)
#define EXT_ROM_ERR_CIPHER_HASH_HMAC_KEYLEN_ERR (3007)
#define EXT_ROM_ERR_CIPHER_HASH_HMAC_KEY_TIMEOUT_ERR (3008)
#define EXT_ROM_ERR_CIPHER_HASH_BUSY (3009)
#define EXT_ROM_ERR_CIPHER_HASH_TIMEOUT_ERR (3010)
#define EXT_ROM_ERR_CIPHER_HASH_NOT_START (3011)
#define EXT_ROM_ERR_CIPHER_EFUSE_INVALIDATE_ID (3012)
#define EXT_ROM_ERR_CIPHER_EFUSE_INVALIDATE_PARA (3013)
#define EXT_ROM_ERR_CIPHER_EFUSE_WRITE_ERR (3014)
#define EXT_ROM_ERR_CIPHER_EFUSE_INVALIDATE_AUTH (3015)
#define EXT_ROM_ERR_CIPHER_EFUSE_BUSY (3016)
#define EXT_ROM_ERR_CIPHER_EFUSE_TIMEOUT (3017)
#define EXT_ROM_ERR_CIPHER_RSA_INVALIDATE_ID (3018)
#define EXT_ROM_ERR_CIPHER_RSA_INVALIDATE_PARA (3019)
#define EXT_ROM_ERR_CIPHER_RSA_WRITE_ERR (3020)
#define EXT_ROM_ERR_CIPHER_RSA_INVALIDATE_AUTH (3021)
#define EXT_ROM_ERR_CIPHER_RSA_BUSY (3022)
#define EXT_ROM_ERR_CIPHER_RSA_TIMEOUT (3023)
#define EXT_ROM_ERR_CIPHER_RSA_GET_RANDOM_ERR (3024)
#define EXT_ROM_ERR_CIPHER_RSA_FAILED_DECRYPT (3025)
#define EXT_ROM_ERR_CIPHER_RSA_FAILED_ENCRYPT (3026)
#define EXT_ROM_ERR_CIPHER_RSA_CALC_ERR (3027)
#define EXT_ROM_ERR_CIPHER_RSA_CALC_CMP_ERR (3028)
#define EXT_ROM_ERR_CIPHER_RSA_MALLOC_ERR (3029)
#define EXT_ROM_ERR_CIPHER_RSA_SEM_ERR (3030)
#define EXT_ROM_ERR_CIPHER_RSA_INVALID_POINT (3031)
#define EXT_ROM_ERR_CIPHER_RSA_HASH_INIT_ERR (3032)
#define EXT_ROM_ERR_CIPHER_AES_INVALIDATE_PARA (3033)
#define EXT_ROM_ERR_CIPHER_AES_TIMEOUT (3034)
#define EXT_ROM_ERR_CIPHER_AES_BUSY (3035)
#define EXT_ROM_ERR_CIPHER_AES_START_ERR (3036)
#define EXT_ROM_ERR_CIPHER_AES_MALLOC_ERR (3037)
#define EXT_ROM_ERR_CIPHER_AES_SEM_ERR (3038)

/**
 * GPIO error code definitions
 */
#define EXT_ROM_ERR_GPIO_GROUP_NUMBER_BEYOUND (3201) /* Abandoned error code */
#define EXT_ROM_ERR_GPIO_BIT_NUMBER_BEYOUND (3202)   /* Abandoned error code */
#define EXT_ROM_ERR_GPIO_DIRECTION_NOT_MATCH (3203)  /* Abandoned error code */
#define EXT_ROM_ERR_GPIO_DIRECTION_INVALID (3204)    /* Abandoned error code */
#define EXT_ROM_ERR_GPIO_NOT_INIT (3205)
/* I/O module error: I/O interrupt registration | Deregistration error, I/O interrupt not initialized */
#define EXT_ROM_ERR_GPIO_FAILED_INIT (3206) /* Abandoned error code */
#define EXT_ROM_ERR_GPIO_REPEAT_INIT (3207)
/* I/O module error: I/O interrupt initialization error. The I/O interrupt is not initialized. */
#define EXT_ROM_ERR_GPIO_POINTER_NULL (3208)                   /* Abandoned error code */
#define EXT_ROM_ERR_GPIO_PARAMETER_ERROR (3209)                /* I/O module error: Invalid parameter */
#define EXT_ROM_ERR_GPIO_PORT_OVERFLOW_FAILED (3210)           /* Abandoned error code */
#define EXT_ROM_ERR_GPIO_INVALID_BUF (3211)                    /* Abandoned error code */
#define EXT_ROM_ERR_GPIO_PIN_OVERFLOW_FAILED (3212)            /* Abandoned error code */
#define EXT_ROM_ERR_GPIO_INTERRUPT_BIT_DISABLE_FAILED (3213)   /* Abandoned error code */
#define EXT_ROM_ERR_GPIO_INTERRUPT_MODE_OVERFLOW_FAILED (3214) /* Abandoned error code */
#define EXT_ROM_ERR_GPIO_INTERRUPT_MODE_FAILED (3215)          /* Abandoned error code */
#define EXT_ROM_ERR_GPIO_CURR_PIN_BUSY (3216)                  /* Abandoned error code */
#define EXT_ROM_ERR_GPIO_INVALID_SUSPEND (3217)                /* Abandoned error code */

/**
 * SPI Error code definitions
 */
#define EXT_ROM_ERR_SPI_PARAMETER_WRONG (3301) /* SPI module error: Invalid parameter */
#define EXT_ROM_ERR_SPI_NOT_ENOUGH_SOFT_HANDLE (3302)
/* SPI module error: Failed to open the SPI device due to an insufficient soft handle */
#define EXT_ROM_ERR_SPI_WRITE_TIMEOUT (3303)
/* SPI module error: Writing data to the chip timed out. (master device full-duplex/half-duplex write
interface and half-duplex read interface, and slave device write interface) */
#define EXT_ROM_ERR_SPI_READ_TIMEOUT (3304)
/* SPI module error: Reading data from the chip timed out. (master device full-duplex/half-duplex write
interface and half-duplex read interface, and slave device write interface) */
#define EXT_ROM_ERR_SPI_HW_NOT_OK (3305)
/* SPI module error: Failed to open the SPI device due to uninitialized hardware resources. */
#define EXT_ROM_ERR_SPI_RESTORE_INVALID_FD (3306) /* SPI module error: Internal error, invisible to external APIs */
/* SPI module error: Operation failed due to unavailable soft handle. */
#define EXT_ROM_ERR_SPI_INVALID_FD (3307)
#define EXT_ROM_ERR_SPI_DEV_CLOSED (3308) /* SPI module error: Soft handle released during the operation. */
#define EXT_ROM_ERR_SPI_REINIT (3309)
/* SPI module error: Module initialization failed due to repeated initialization. */
/**
 * Subcode definitions of LED error codes
 */
#define EXT_ROM_ERR_LED_PARAMETER_ERROR (3501) /* LED module error: Invalid parameter */

/**
 * WATCHDOG Error code definitions
 */
#define EXT_ROM_ERR_WATCHDOG_PARA_ERROR (3601) /* Watchdog module error: Invalid parameter */

/**
 * I2C Error code definitions
 */
#define EXT_ROM_ERR_I2C_NOT_INIT (3650)          /* I2C module error: Uninitialized driver */
#define EXT_ROM_ERR_I2C_INVALID_PARAMETER (3651) /* I2C module error: Invalid parameter */
#define EXT_ROM_ERR_I2C_TIMEOUT_START (3652)     /* I2C module error: Sending the device address timed out. */
#define EXT_ROM_ERR_I2C_TIMEOUT_WAIT (3653)
#define EXT_ROM_ERR_I2C_TIMEOUT_STOP (3654) /* I2C module error: Stopping I2C transfer timed out. */
#define EXT_ROM_ERR_I2C_TIMEOUT_RCV_BYTE (3655)
#define EXT_ROM_ERR_I2C_TIMEOUT_RCV_BYTE_PROC (3656)
#define EXT_ROM_ERR_I2C_WAIT_SEM_FAIL (3657)
#define EXT_ROM_ERR_I2C_START_ACK_ERR (3658) /* I2C module error: Device address sent with a peer ACK error. */
#define EXT_ROM_ERR_I2C_WAIT_ACK_ERR (3659)

/**
 * Subcode definitions of flash error codes
 */
#define EXT_ROM_ERR_FLASH_NOT_INIT (3700)      /* Flash module error: Uninitialized driver */
#define EXT_ROM_ERR_FLASH_INVALID_PARAM (3701) /* Flash module error: Invalid parameter */
#define EXT_ROM_ERR_FLASH_INVALID_PARAM_BEYOND_ADDR (3702)
/* Flash module error: Read/Write/Erase exceeded the flash range. */
#define EXT_ROM_ERR_FLASH_INVALID_PARAM_SIZE_ZERO (3703)       /* Flash module error: Read/Write/Erase size 0 */
#define EXT_ROM_ERR_FLASH_INVALID_PARAM_ERASE_NOT_ALIGN (3704) /* Flash module error: Erase size not sector aligned. */
#define EXT_ROM_ERR_FLASH_INVALID_PARAM_IOCTL_DATA_NULL (3705) /* Flash module error: Invalid ioctl parameter */
#define EXT_ROM_ERR_FLASH_INVALID_PARAM_DATA_NULL (3706)       /* Abandoned error code */
#define EXT_ROM_ERR_FLASH_INVALID_PARAM_PAD1 (3707)            /* Abandoned error code */
#define EXT_ROM_ERR_FLASH_INVALID_PARAM_PAD2 (3708)            /* Abandoned error code */
#define EXT_ROM_ERR_FLASH_INVALID_PARAM_PAD3 (3709)            /* Abandoned error code */
#define EXT_ROM_ERR_FLASH_INVALID_PARAM_PAD4 (3710)            /* Abandoned error code */
#define EXT_ROM_ERR_FLASH_TIME_OUT_WAIT_READY (3711)
/* Flash module error: Waiting for the flash status timed out. */
#define EXT_ROM_ERR_FLASH_QUAD_MODE_READ_REG1 (3712) /* Abandoned error code */
#define EXT_ROM_ERR_FLASH_QUAD_MODE_READ_REG2 (3713) /* Abandoned error code */
#define EXT_ROM_ERR_FLASH_QUAD_MODE_COMPARE_REG (3714)
/* Flash module error: After the 4-wire read mode is enabled, register read back check fails. */
#define EXT_ROM_ERR_FLASH_NO_MATCH_FLASH (3715)
/* Flash module error: Initialization failed due to unmatched flash ID. */
#define EXT_ROM_ERR_FLASH_WRITE_ENABLE (3716)
/* Flash module error: Erase failed because waiting for the erase operation timed out. */
#define EXT_ROM_ERR_FLASH_NO_MATCH_ERASE_SIZE (3717)
/* Flash module error: Initialization failed because no matched erase command is found. */
#define EXT_ROM_ERR_FLASH_MAX_SPI_OP (3718)
/* Flash module error: Erasing failed. No proper erase command is found. */
/* Flash module error: ioctl failed due to unsupported ID. */
#define EXT_ROM_ERR_FLASH_NOT_SUPPORT_IOCTL_ID (3719)
#define EXT_ROM_ERR_FLASH_INVALID_CHIP_ID (3720)
/* Flash module error: Initialization failed due to a flash ID read failure. */
#define EXT_ROM_ERR_FLASH_RE_INIT (3721) /* Abandoned error code */
#define EXT_ROM_ERR_FLASH_WRITE_NOT_SUPPORT_ERASE (3722)
/* Flash module error: Writing failed due to unsupported previous erase option (do_erase = TD_TRUE). */
#define EXT_ROM_ERR_FLASH_WRITE_COMPARE_WRONG (3723)
/* Flash module error: Write failure. Read back check after the write fails. */
#define EXT_ROM_ERR_FLASH_WAIT_CFG_START_TIME_OUT (3724) /* Abandoned error code */

/**
 * Task error code definitions
 */
#define EXT_ROM_ERR_TASK_INVALID_PARAM (6000) /* Task module error: Invalid parameter */
#define EXT_ROM_ERR_TASK_CREATE_FAIL (6001)
/* Task module error: Failed to create the task due to an underlying layer error of the OS. */
#define EXT_ROM_ERR_TASK_DELETE_FAIL (6002)
/* Task module error: Failed to delete the task due to an underlying layer error of the OS. */
#define EXT_ROM_ERR_TASK_SUPPEND_FAIL (6003)
/* Task module error: Failed to suspend the task due to an underlying layer error of the OS. */
#define EXT_ROM_ERR_TASK_RESUME_FAIL (6004)
/* Task module error: Failed to wake up the task due to an underlying layer error of the OS. */
#define EXT_ROM_ERR_TASK_GET_PRI_FAIL (6005)
/* Task module error: Failed to obtain the task priority due to an underlying layer error of the OS. */
#define EXT_ROM_ERR_TASK_SET_PRI_FAIL (6006)
/* Task module error: Failed to set the task priority due to an underlying layer error of the OS. */
#define EXT_ROM_ERR_TASK_LOCK_FAIL (6007)   /* Abandoned error code */
#define EXT_ROM_ERR_TASK_UNLOCK_FAIL (6008) /* Abandoned error code */
#define EXT_ROM_ERR_TASK_DELAY_FAIL (6009)
/* Task module error: Failed to sleep the task due to an underlying layer error of the OS. */
#define EXT_ROM_ERR_TASK_GET_INFO_FAIL (6010)
/* Task module error: Failed to obtain task information due to an underlying layer error of the OS. */
#define EXT_ROM_ERR_TASK_REGISTER_SCHEDULE_FAIL (6011) /* Abandoned error code */
/**
 * Message error code definitions
 */
#define EXT_ROM_ERR_MSG_INVALID_PARAM (6020) /* Message module error: Invalid parameter */
#define EXT_ROM_ERR_MSG_CREATE_Q_FAIL (6021)
/* Message module error: Failed to create the message queue due to an underlying layer error of the OS. */
#define EXT_ROM_ERR_MSG_DELETE_Q_FAIL (6022) /* Abandoned error code */
#define EXT_ROM_ERR_MSG_WAIT_FAIL (6023)
/* Message module error: Failed to wait for the message due to an underlying layer error of the OS. */
#define EXT_ROM_ERR_MSG_SEND_FAIL (6024)
/* Message module error: Failed to send the message due to an underlying layer error of the OS. */
#define EXT_ROM_ERR_MSG_GET_Q_INFO_FAIL (6025) /* Abandoned error code */
#define EXT_ROM_ERR_MSG_Q_DELETE_FAIL (6026)
/* Message module error: Failed to delete the message queue due to an underlying layer error of the OS. */
/* Message module error: Waiting for the message timed out. */
#define EXT_ROM_ERR_MSG_WAIT_TIME_OUT (EXT_ROM_ERR_TIMEOUT)

/**
 * Semaphore error code definitions
 */
#define EXT_ROM_ERR_SEM_INVALID_PARAM (6040) /* Semaphore module error: Invalid parameter */
#define EXT_ROM_ERR_SEM_CREATE_FAIL (6041)
/* Semaphore module error: Failed to create the semaphore due to an OS underlying layer error. */
#define EXT_ROM_ERR_SEM_DELETE_FAIL (6042)
/* Semaphore module error: Failed to delete the semaphore due to an underlying layer error of the OS. */
#define EXT_ROM_ERR_SEM_WAIT_FAIL (6043)
/* Semaphore module error: Failed to wait for the semaphore due to an underlying layer error of the OS. */
#define EXT_ROM_ERR_SEM_SIG_FAIL (6044)
/* Semaphore module error: Failed to release the semaphore due to an underlying layer error of the OS. */
#define EXT_ROM_ERR_SEM_WAIT_TIME_OUT (EXT_ROM_ERR_TIMEOUT) /* Semaphore module error: Waiting for semaphore timed out. \
                                                           */

/**
 * Mutex error code definitions
 */
#define EXT_ROM_ERR_MUX_INVALID_PARAM (6050) /* Mutex module error: Invalid parameter */
#define EXT_ROM_ERR_MUX_CREATE_FAIL (6051)
/* Mutex module error: Failed to create the mutex due to an underlying layer error of the OS. */
#define EXT_ROM_ERR_MUX_DELETE_FAIL (6052)
/* Mutex module error: Failed to delete the mutex due to an underlying layer error of the OS. */
#define EXT_ROM_ERR_MUX_PEND_FAIL (6053)
/* Mutex module error: Failed to obtain the mutex due to an underlying layer error of the OS. */
#define EXT_ROM_ERR_MUX_POST_FAIL (6054)
/* Mutex module error: Failed to release the mutex due to an underlying layer error of the OS. */
/**
 * Interrupt error code definitions
 */
#define EXT_ROM_ERR_ISR_INVALID_PARAM (6060) /* Interrupt module error: Invalid parameter */
#define EXT_ROM_ERR_ISR_REQ_IRQ_FAIL (6061)
/* Interrupt module error: Failed to register the interrupt due to an underlying layer error of the OS. */
#define EXT_ROM_ERR_ISR_ADD_JOB_MALLOC_FAIL (6062) /* Abandoned error code */
#define EXT_ROM_ERR_ISR_ADD_JOB_SYS_FAIL (6063)
/* Interrupt module error: Failed to create the interrupt HISR due to an underlying layer error of the OS. */
#define EXT_ROM_ERR_ISR_DEL_IRQ_FAIL (6064)
/* Interrupt module error: Failed to clear the registered interrupt due to an underlying layer error of the OS. */
/**
 * Event error code definitions
 */
#define EXT_ROM_ERR_EVENT_INVALID_PARAM (6080) /* Event module error: Invalid parameter */
#define EXT_ROM_ERR_EVENT_CREATE_NO_HADNLE (6081)
/* Event module error: Trying to obtain the event handle for event creation but the event resource is insufficient. */
#define EXT_ROM_ERR_EVENT_CREATE_SYS_FAIL (6082)
/* Event module error: Failed to create the event due to an underlying layer error of the OS. */
#define EXT_ROM_ERR_EVENT_SEND_FAIL (6083)
/* Event module error: Failed to send the event due to an underlying layer error of the OS. */
#define EXT_ROM_ERR_EVENT_WAIT_FAIL (6084)
/* Event module error: Failed to wait for the event due to an underlying layer error of the OS. */
#define EXT_ROM_ERR_EVENT_CLEAR_FAIL (6086)
/* Event module error: Failed to clear the event due to an underlying layer error of the OS. */
#define EXT_ROM_ERR_EVENT_RE_INIT (6087)
/* Event module error: Failed to initialize the event resource due to repeated initialization. */
#define EXT_ROM_ERR_EVENT_NOT_ENOUGH_MEMORY (6088)
/* Event module error: Failed to initialize the event resource due to a memory application failure. */
#define EXT_ROM_ERR_EVENT_NOT_INIT (6089)
/* Event module error: Uninitialized event resource */
#define EXT_ROM_ERR_EVENT_DELETE_FAIL (6090)
/* Event module error: Failed to delete the event due to an underlying layer error of the OS. */
#define EXT_ROM_ERR_EVENT_WAIT_TIME_OUT EXT_ROM_ERR_TIMEOUT /* Event module error: Waiting for the event timed out. */
/**
 * Memory management error code definitions
 */
#define EXT_ROM_ERR_MEM_INVALID_PARAM (6100)                 /* Memory module error: Invalid parameter */
#define EXT_ROM_ERR_MEM_CREAT_POOL_FAIL (6101)               /* Abandoned error code */
#define EXT_ROM_ERR_MEM_CREATE_POOL_NOT_ENOUGH_HANDLE (6102) /* Abandoned error code */
#define EXT_ROM_ERR_MEM_FREE_FAIL (6103)                     /* Abandoned error code */
#define EXT_ROM_ERR_MEM_RE_INIT (6104)                       /* Abandoned error code */
#define EXT_ROM_ERR_MEM_NOT_INIT (6105)                      /* Abandoned error code */
#define EXT_ROM_ERR_MEM_CREAT_POOL_MALLOC_FAIL (6106)        /* Abandoned error code */
#define EXT_ROM_ERR_MEM_GET_INFO_FAIL (6107)                 /* Abandoned error code */
#define EXT_ROM_ERR_MEM_GET_OS_INFO_NOK (6108)
/* Memory module error: Failed to obtain the memory information due to an underlying layer error of the OS. */
/**
 * Timer error code definitions
 */
#define EXT_ROM_ERR_TIMER_INVALID_PARAM (6120) /* Timer module error: Invalid parameter */
#define EXT_ROM_ERR_TIMER_CREATE_TIMER_FAIL (6121)
/* Timer module error: Failed to start the timer due to an underlying layer error of the OS. */
/**
OS resource statistic error codes
 */
#define EXT_ROM_ERR_OS_STAT_INVALID_PARAM (6130) /* Obtain the system resource user module. Invalid parameter */
#endif                                          // __SOC_ERRNO_ROM_H__
