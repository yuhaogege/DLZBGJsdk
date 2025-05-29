
#ifndef __SOC_BOOT_ERR_H__
#define __SOC_BOOT_ERR_H__

#define EXT_ERR_SUCCESS (0)
#define EXT_ERR_FAILURE (unsigned int)(-1)

#define EXT_ERR_FLASH_NOT_INIT                       (3700)
#define EXT_ERR_FLASH_INVALID_PARAM                  (3701)
#define EXT_ERR_FLASH_INVALID_PARAM_BEYOND_ADDR      (3702)
#define EXT_ERR_FLASH_INVALID_PARAM_SIZE_ZERO        (3703)
#define EXT_ERR_FLASH_INVALID_PARAM_ERASE_NOT_ALIGN  (3704)
#define EXT_ERR_FLASH_INVALID_PARAM_IOCTRL_DATA_NULL (3705)
#define EXT_ERR_FLASH_INVALID_PARAM_DATA_NULL        (3706)
#define EXT_ERR_FLASH_INVALID_PARAM_PAD1             (3707)
#define EXT_ERR_FLASH_INVALID_PARAM_PAD2             (3708)
#define EXT_ERR_FLASH_INVALID_PARAM_PAD3             (3709)
#define EXT_ERR_FLASH_INVALID_PARAM_PAD4             (3710)
#define EXT_ERR_FLASH_TIME_OUT_WAIT_READY            (3711)
#define EXT_ERR_FLASH_QUAD_MODE_READ_REG1            (3712)
#define EXT_ERR_FLASH_QUAD_MODE_READ_REG2            (3713)
#define EXT_ERR_FLASH_QUAD_MODE_COMPARE_REG          (3714)
#define EXT_ERR_FLASH_NO_MATCH_FLASH                 (3715)
#define EXT_ERR_FLASH_WRITE_ENABLE                   (3716)
#define EXT_ERR_FLASH_NO_MATCH_ERASE_SIZE            (3717)
#define EXT_ERR_FLASH_MAX_SPI_OP                     (3718)
#define EXT_ERR_FLASH_NOT_SUPPORT_IOCTRL_ID          (3719)
#define EXT_ERR_FLASH_INVALID_CHIP_ID                (3720)
#define EXT_ERR_FLASH_RE_INIT                        (3721)
#define EXT_ERR_FLASH_WRITE_NOT_SUPPORT_ERASE        (3722)
#define EXT_ERR_FLASH_WRITE_COMPARE_WRONG            (3723)

#define EXT_ERR_CIPHER_HASH_MALLOC_FAIL          (3002)
#define EXT_ERR_CIPHER_HASH_SEM_CREATE_FAIL      (3003)
#define EXT_ERR_CIPHER_HASH_START_FAIL           (3004)
#define EXT_ERR_CIPHER_HASH_UPDATE_FAIL          (3005)
#define EXT_ERR_CIPHER_HASH_FINAL_FAIL           (3006)
#define EXT_ERR_CIPHER_HASH_HMAC_KEYLEN_ERR      (3007)
#define EXT_ERR_CIPHER_HASH_HMAC_KEY_TIMEOUT_ERR (3008)
#define EXT_ERR_CIPHER_HASH_BUSY                 (3009)
#define EXT_ERR_CIPHER_HASH_TIMEOUT_ERR          (3010)
#define EXT_ERR_CIPHER_HASH_NOT_START            (3011)

#define EXT_ERR_UPG_CHECK_TBL_READ_FAIL         (5002) /* Failed to read the TBL when the TBL verification is started. */
#define EXT_ERR_UPG_CHECK_TBL_LEN_FAIL          (5003) /* The TBL length is abnormal during TBL verification. */
#define EXT_ERR_UPG_CHECK_TBL_CRC_FAIL          (5005) /* The TBL check is started, and the CRC error occurs. */
#define EXT_ERR_UPG_CHECK_TBL_READ_SECTION_FAIL (5004) /* Start TBL verification.Failed to read the subsection of TBL. */
#define EXT_ERR_UPG_CHECK_KERNEL_CRC            (5006) /* Starts kernel verification,CRC error */
#define EXT_ERR_UPG_CHECK_NVFILE_CRC            (5007) /* Start NV verification FILE,CRC error */
#define EXT_ERR_UPG_CHECK_NVFILE_VER_MAGIC      (5008) /* Starts NV file verification.The version does not match. */

#define EXT_ERR_UPG_CHECK_UPG_READ_UPG_FAIL       (5012) /* Upgrade Process:Failed to read the upgrade file. */
#define EXT_ERR_UPG_CHECK_UPG_MAGIC_MANUCODE_FAIL (5013) /* Upgrade Process:The magic number or vendor version is 
                                                           incorrect during the upgrade. */
#define EXT_ERR_UPG_CHECK_UPG_FILE_TOO_LEN        (5014) /* Upgrade Process:The upgrade file is too long. */
#define EXT_ERR_UPG_CHECK_UPG_CRC_WRONG           (5015) /* Upgrade Process:CHECK SUM check error */
#define EXT_ERR_UPG_CHECK_UPG_READ_SECTION_FAIL   (5016) /* Upgrade Process:An error occurred when reading the section */
#define EXT_ERR_UPG_CHECK_UPG_MALLOC_FAIL         (5017) /* Upgrade Process:Failed to apply for memory. */
#define EXT_ERR_UPG_FAIL_1                        (5018)
#define EXT_ERR_UPG_FAIL_2                        (5019)
#define EXT_ERR_UPG_FAIL_3                        (5020)
#define EXT_ERR_UPG_FAIL_4                        (5021)
#define EXT_ERR_UPG_SECURE_BOOT_1                 (5031)
#define EXT_ERR_UPG_SECURE_BOOT_2                 (5032)
#define EXT_ERR_UPG_SECURE_BOOT_3                 (5033)
#define EXT_ERR_UPG_SECURE_BOOT_4                 (5034)
#define EXT_ERR_UPG_SECURE_BOOT_5                 (5035)
#define EXT_ERR_UPG_SECURE_BOOT_6                 (5036)
#define EXT_ERR_UPG_SECURE_BOOT_7                 (5037)
#define EXT_ERR_UPG_SECURE_BOOT_8                 (5038)
#define EXT_ERR_UPG_SECURE_BOOT_9                 (5039)
#define EXT_ERR_UPG_SECURE_BOOT_10                (5040)
#define EXT_ERR_UPG_SECURE_BOOT_11                (5041)
#define EXT_ERR_UPG_SECURE_BOOT_12                (5042)
#define EXT_ERR_UPG_SECURE_BOOT_13                (5043)
#define EXT_ERR_UPG_SECURE_BOOT_14                (5044)
#define EXT_ERR_UPG_SECURE_BOOT_15                (5045)
#define EXT_ERR_UPG_SECURE_BOOT_16                (5046)
#define EXT_ERR_UPG_SECURE_BOOT_17                (5047)
#define EXT_ERR_UPG_SECURE_BOOT_18                (5048)
#define EXT_ERR_UPG_SECURE_BOOT_19                (5049)
#define EXT_ERR_UPG_SECURE_BOOT_20                (5050)
#define EXT_ERR_UPG_SECURE_BOOT_21                (5051)
#define EXT_ERR_UPG_SECURE_BOOT_22                (5052)
#define EXT_ERR_UPG_SECURE_BOOT_23                (5053)
#define EXT_ERR_UPG_SECURE_BOOT_24                (5054)
#define EXT_ERR_UPG_SECURE_BOOT_25                (5055)
#define EXT_ERR_UPG_SECURE_BOOT_26                (5056)
#define EXT_ERR_UPG_SECURE_BOOT_27                (5057)
#define EXT_ERR_UPG_SECURE_BOOT_28                (5058)

#define EXT_ERR_UPG_CHECK_UPG_END_MAGIC_FAIL  (5065)
#define EXT_ERR_UPG_CHECK_UPG_END_OFFSET_FAIL (5066)

#define EXT_ERR_NO_INIT           (5100)
#define EXT_ERR_INIT_ALREADY      (5101)
#define EXT_ERR_INVALID_PARAMETER (5102)
#define EXT_ERR_MALLOC_FAILUE     (5103)
#define EXT_ERR_NOT_SUPPORT       (5104)
#define EXT_ERR_CRC_CHECK         (5105)
#define EXT_ERR_MAGIC_CHECK       (5106)
#define EXT_ERR_NOT_FOUND         (5107)
#define EXT_ERR_FAIL_N_TIMES      (5108)
#define EXT_ERR_ALREADY_DONE      (5109)

#define EXT_ERR_UPG_START_1_CHECK_UPG_HEAD             (7510)
#define EXT_ERR_UPG_HEAD_CHECK_MAGIC_WRONG             (EXT_ERR_UPG_START_1_CHECK_UPG_HEAD + 0)
#define EXT_ERR_UPG_HEAD_CHECK_MANUCODE_WRONG          (EXT_ERR_UPG_START_1_CHECK_UPG_HEAD + 1)
#define EXT_ERR_UPG_HEAD_CHECK_FILE_TYPE_WRONG         (EXT_ERR_UPG_START_1_CHECK_UPG_HEAD + 2)
#define EXT_ERR_UPG_HEAD_CHECK_FILE_LEN_WRONG          (EXT_ERR_UPG_START_1_CHECK_UPG_HEAD + 3)
#define EXT_ERR_UPG_HEAD_CHECK_PRODUCT_LIST_WRONG      (EXT_ERR_UPG_START_1_CHECK_UPG_HEAD + 4)
#define EXT_ERR_UPG_HEAD_CHECK_PARTION_NOT_SAME        (EXT_ERR_UPG_START_1_CHECK_UPG_HEAD + 5)
#define EXT_ERR_UPG_HEAD_CHECK_PARTION_NOT_SUPPORT_UPG (EXT_ERR_UPG_START_1_CHECK_UPG_HEAD + 7)
#define EXT_ERR_UPG_HEAD_CHECK_PARTION_NO_GO           (EXT_ERR_UPG_START_1_CHECK_UPG_HEAD + 6)
#define EXT_ERR_UPG_HEAD_CHECK_NOT_SURE                (EXT_ERR_UPG_START_1_CHECK_UPG_HEAD + 8)

#endif

