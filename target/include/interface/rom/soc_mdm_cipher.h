/**
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: hct_cipher Cipher module.
 */

#ifndef __SOC_MDM_CIPHER_H__
#define __SOC_MDM_CIPHER_H__

#include <soc_types_rom.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif // __cplusplus

#define EXT_SHA256_SIZE (32)
#define EXT_RSA_2048_LEN (256)
#define EXT_RSA_4096_LEN (512)

typedef td_void (*aes_prepare_func)(td_void);
typedef td_void (*aes_restore_func)(td_void);
typedef struct {
    aes_prepare_func prepare_func;
    aes_restore_func restore_func;
} EXT_AES_REGISTER_S;

/* Registration function, used for debug or low power mode */
EXT_EXTERN td_void uapi_aes_register(EXT_AES_REGISTER_S sfunc);

/*
 * @ingroup  hct_cipher
 * @brief AES calculation
 *
 * @par Description:
 * AES encryption/decryption
 *
 * @attention None
 * @param  key           [IN] Type #td_u8* Key pointer. The pointer needs to be 4-byte aligned. The maximum length is 32
 * bytes.
 * @param  key_len       [IN] Type #td_u32 Key length: 16 bytes (128 bits), 24 bytes (192 bits), or 32 bytes (256 bits)
 * @param  iv            [IN] Type #td_u8* Initial value. The fixed length is 16 bytes. Ensure that a valid non-null
 * pointer is transferred. If the initial value is not required, clear the valid space.
 * @param  src_phy_addr  [IN] Type #td_u32 Source physical address. The content must be a multiple of the key.
 * @param  dest_phy_addr [IN] Type #td_u32 Destination physical address. The pre-allocated space must be the same as
 * the source physical address space.
 * @param  length        [IN] Type #td_u32 Configured data length. The length of the input data is the same as that of
 * the output data.
 * @param  is_decrypt    [IN] Type #td_bool Encryption or decryption
 * @li TD_TRUE: decryption
 * @li TD_FALSE: encryption
 *
 * @retval #0             Success
 * @retval #Other values           Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_cipher.h: Describes the encryption and decryption APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_aes_calc(const td_u8 *key, const td_u32 key_len, const td_u8 iv[16], /* 16 count */
    const td_u32 src_phy_addr, const td_u32 dest_phy_addr, const td_u32 length, const td_bool is_decrypt);

/*
 * @ingroup  hct_cipher
 * @brief Initializes the AES module.
 *
 * @par Description:
 * Initializes the AES module. Multi-tasking is not supported.
 *
 * @attention Generally, this API is called during system initialization.
 * @param  None
 *
 * @retval #0      Success
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_cipher.h: Describes the encryption and decryption APIs.
 * @see  uapi_aes_deinit
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_aes_init(td_void);
/*
 * @ingroup  hct_cipher
 * @brief Deinitializes the AES module.
 *
 * @par Description:
 * Deinitializes the AES module.
 *
 * @attention  The AES algorithm has been used in communication packets between the PC and the DIAG module for debug on
 * the board. If the AES algorithm is deinitialized, this function becomes invalid.
 * @param  None
 *
 * @retval #0      Success
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_cipher.h: Describes the encryption and decryption APIs.
 * @see  uapi_aes_init
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_aes_deinit(td_void);

/*
 * @ingroup hct_cipher
 *
 * RSA data encryption/decryption algorithm policy
 */
typedef enum {
    EXT_UNF_RSA_ENC_SCHEME_NO_PADDING,        /* < No padding */
    EXT_UNF_RSA_ENC_SCHEME_RSAES_OAEP_SHA256, /* < RSAES-OAEP-SHA256 padding mode. The length of the source data must
                                                    be less than or equal to KeyLen minus 66 (unit: byte). */
    EXT_UNF_RSA_ENC_SCHEME_RSAES_PKCS1_V1_5,  /* < PKCS1_V1_5 padding mode. The length of the source data must be
                                                    less than or equal to KeyLen minus 11 (unit: byte). */
    EXT_UNF_RSA_ENC_SCHEME_MAX,               /* < Maximum value, which cannot be used */
} ext_unf_rsa_enc_scheme_e;

/*
 * @ingroup hct_cipher
 *
 * RSA private key structure.
 */
typedef struct {
    td_u8 *p_u8_n;  /* < Pointer to the RSA modulus N. The length is key_len.
                        The insufficient part is to be padded with 0 by the user before the valid data. */
    td_u8 *p_u8_d;  /* < Pointer to the RSA private key D. The length is key_len.
                        The insufficient part is to be padded with 0 by the user before the valid data. */
    td_u16 key_len; /* < The value can be 512, 256, or 128 (unit: byte). */
    td_u16 revd;    /* < Reserved */
} ext_unf_rsa_pri_key_s;

/*
 * @ingroup hct_cipher
 *
 * RSA public key structure
 */
typedef struct {
    td_u8 *p_u8_n;  /* < Pointer to the RSA modulus N. The length is key_len.
                        The insufficient part is to be padded with 0 by the user before the valid data. */
    td_u8 *p_u8_e;  /* < Pointer to the RSA private key E. The length is key_len.
                        The insufficient part is to be padded with 0 by the user before the valid data. */
    td_u16 key_len; /* < The value can be 512, 256, or 128 (unit: byte). */
    td_u16 revd;    /* < Reserved */
} ext_unf_rsa_pub_key_s;

/*
 * @ingroup hct_cipher
 *
 * RSA public key encryption/decryption algorithm parameter structure
 */
typedef struct {
    ext_unf_rsa_enc_scheme_e en_scheme; /* < RSA data encryption/decryption algorithm policy */
    ext_unf_rsa_pub_key_s pub_key;      /* < RSA public key structure */
} ext_rsa_pub_enc_s;

/*
 * @ingroup hct_cipher
 *
 * Structure of the RSA private key decryption algorithm parameters
 */
typedef struct {
    ext_unf_rsa_enc_scheme_e en_scheme; /* < RSA data encryption/decryption padding algorithm */
    ext_unf_rsa_pri_key_s pri_key;      /* < RSA private key structure */
} ext_rsa_pri_enc_s;

typedef td_void (*rsa_prepare_func)(td_void);
typedef td_void (*rsa_restore_func)(td_void);
typedef struct {
    rsa_prepare_func prepare_func;
    rsa_restore_func restore_func;
} EXT_RSA_REGISTER_S;

/* Registration function, used for debug or low power mode */
EXT_EXTERN td_void uapi_rsa_register(EXT_RSA_REGISTER_S sfunc);

/*
 * @ingroup  hct_cipher
 * @brief  Initializes the RSA module.
 *
 * @par Description:
 * Initializes the RSA module, transferred during system boot. Multi-tasking is not supported.
 *
 * @attention  Before calling this function, ensure that uapi_hash_init has been called.
 * @param  p_key     [IN] Type #td_u8* The user applies for space based on the maximum key size.
 * If the value is empty, the underlying layer dynamically applies for the keylen space.
 * @param  key_len   [IN] Type #td_u16 Size of the key space transferred by the user (unit: byte). The value can be 128,
 * 256, or 512.
 *
 * @retval #0       Success
 * @retval #Other values     Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_cipher.h: Describes the encryption and decryption APIs.
 * @see  uapi_rsa_deinit
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_rsa_init(td_u8 *p_key, td_u16 key_len);
/*
 * @ingroup  hct_cipher
 * @brief  Deinitializes the RSA.
 *
 * @par Description:
 * Deinitializes the RSA. Multi-tasking is not supported.
 *
 * @attention None
 * @param  None
 *
 * @retval #0      Success
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_cipher.h: Describes the encryption and decryption APIs.
 * @see  uapi_rsa_init
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_rsa_deinit(td_void);

/*
 * @ingroup  hct_cipher
 * @brief Encrypts the RSA public key.
 *
 * @par Description:
 * Encrypts the RSA public key.
 *
 * @attention None
 * @param  p_rsa_enc   [IN]  Type #ext_rsa_pub_enc_s* RSA public key encryption/decryption algorithm parameters
 * @param  p_input      [IN]  Type #td_u8* Input data source address
 * @param  in_len     [IN]  Type #td_u32 Input data source length (unit: byte)
 * @param  p_output     [OUT] Type #td_u8* Destination address of the output data
 * @param  p_out_len     [OUT] Type #td_u32* Output data length (unit: byte)
 *
 * @retval #0          Success
 * @retval #Other values        Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_cipher.h: Describes the encryption and decryption APIs.
 * @see  uapi_rsa_private_decrypt
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_rsa_public_encrypt(ext_rsa_pub_enc_s *p_rsa_enc, td_u8 *p_input, td_u32 in_len, td_u8 *p_output,
    td_u32 *p_out_len);

/*
 * @ingroup  hct_cipher
 * @brief Decrypts the RSA private key.
 *
 * @par Description:
 * Decrypts the RSA private key.
 *
 * @attention None
 * @param  p_rsa_dec   [IN]  Type #ext_rsa_pri_enc_s* RSA private key encryption/decryption algorithm parameters
 * @param  p_input      [IN]  Type #td_u8* Input data source address
 * @param  in_len     [IN]  Type #td_u32 Input data source length (unit: byte)
 * @param  p_output     [OUT] Type #td_u8* Destination address of the output data
 * @param  p_out_len     [OUT] Type #td_u32* Output data length (unit: byte)
 *
 * @retval #0          Success
 * @retval #Other values        Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_cipher.h: Describes the encryption and decryption APIs.
 * @see  uapi_rsa_public_encrypt
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_rsa_private_decrypt(ext_rsa_pri_enc_s *p_rsa_dec, td_u8 *p_input, td_u32 in_len, td_u8 *p_output,
    td_u32 *p_out_len);

/*
 * @ingroup  hct_cipher
 * @brief Encrypts the RSA private key.
 *
 * @par Description:
 * Encrypts the RSA private key.
 *
 * @attention None
 * @param  p_rsa_enc   [IN]  Type #ext_rsa_pri_enc_s* Parameters of the RSA private key encryption/decryption algorithm.
 * Only the EXT_UNF_RSA_ENC_SCHEME_NO_PADDING mode is supported.
 * @param  p_input      [IN]  Type #td_u8* Input data source address
 * @param  in_len     [IN]  Type #td_u32 Input data source length (unit: byte)
 * @param  p_output     [OUT] Type #td_u8* Destination address of the output data
 * @param  p_out_len     [OUT] Type #td_u32* Output data length (unit: byte)
 *
 * @retval #0          Success
 * @retval #Other values        Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_cipher.h: Describes the encryption and decryption APIs.
 * @see  uapi_rsa_public_decrypt
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_rsa_private_encrypt(ext_rsa_pri_enc_s *p_rsa_enc, td_u8 *p_input, td_u32 in_len, td_u8 *p_output,
    td_u32 *p_out_len);

/*
 * @ingroup  hct_cipher
 * @brief Decrypts the RSA public key.
 *
 * @par Description:
 * Decrypts the RSA public key.
 *
 * @attention None
 * @param  p_rsa_dec  [IN]  Type #ext_rsa_pub_enc_s* Parameters of the RSA public key encryption/decryption algorithm.
 * Only the EXT_UNF_RSA_ENC_SCHEME_NO_PADDING mode is supported.
 * @param  p_input     [IN]  Type #td_u8* Input data source address
 * @param  in_len    [IN]  Type #td_u32 Input data source length (unit: byte)
 * @param  p_output    [OUT] Type #td_u8* Destination address of the output data
 * @param  p_out_len    [OUT] Type #td_u32* Output data length (unit: byte)
 *
 * @retval #0         Success
 * @retval #Other values       Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_cipher.h: Describes the encryption and decryption APIs.
 * @see  uapi_rsa_private_encrypt
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_rsa_public_decrypt(ext_rsa_pub_enc_s *p_rsa_dec, td_u8 *p_input, td_u32 in_len, td_u8 *p_output,
    td_u32 *p_out_len);
/*
 * @ingroup hct_cipher
 * RSA Verifies the RSA signature padding type.
 */
typedef enum {
    EXT_MODE_PKCS_V15 = 0x00, /* < RSA_PKCS1 */
    EXT_MODE_PKCS_PSS = 0x01, /* < RSA_PSS */
} ext_rsa_padding_mode;

/*
 * @ingroup  hct_cipher
 * @brief Verifies the RSA signature padding type.
 *
 * @par Description:
 * Verifies the RSA signature padding type.
 *
 * @attention None
 * @param  p_u8_N         [IN] Type #td_u8* Modulus. The length of the string is keylen (unit: byte).
 * If the length is insufficient, pad it with zeros.
 * @param  p_u8_e         [IN] Type #td_u8* Public key. The length of the string is keylen (unit: byte).
 * If the length is insufficient, pad it with zeros.
 * @param  key_len    [IN] Type #td_u32 Modulus or public key length (unit: byte).
 * Only 128, 256, or 512 is supported. For example: 256 indicates RSA2048.
 * @param  padding_mode  [IN] Type #ext_rsa_padding_mode Padding mode of the source text
 * after using the HASH algorithm for signature
 * @param  p_u8_hash_data  [IN] Type #td_u8* HASH value of the source text. In this function, the HASH value is
 * compared.
 * @param  p_u8_sign      [IN] Type #td_u8* Signature to be verified
 *
 * @retval #0           Success
 * @retval #Other values         Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_cipher.h: Describes the encryption and decryption APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_rsa_verify(EXT_CONST td_u8 *EXT_CONST p_u8_n, EXT_CONST td_u8 *EXT_CONST p_u8_e,
    EXT_CONST td_u32 key_len, ext_rsa_padding_mode padding_mode, td_u8 *p_u8_hash_data,
    EXT_CONST td_u8 *EXT_CONST p_u8_sign);

/*
 * @ingroup  hct_cipher
 *
 * eFUSE area ID EXT_EFUSE_XXX_RO_ID indicates that the area is read-only,
 * and EXT_EFUSE_XXX_RW_ID indicates that the area is readable and writable.
 */
typedef enum {
    EXT_EFUSE_CHIP_RO_ID = 0,        /* < DW21 chip ID */
    EXT_EFUSE_DIE_RO_ID = 1,         /* < Unique ID of DW21 */
    EXT_EFUSE_ROOT_PUBKEY_RW_ID = 2, /* < Root key SHA256 */
    /* < SHA256 value of the RSA public key used for UART connection authentication */
    EXT_EFUSE_UART_PUBKEY_RW_ID = 3,
    EXT_EFUSE_BUCK_TRIM_RO_ID = 4, /* < DC-DC voltage scaling calibration value */
    EXT_EFUSE_TSENSOR_RO_ID = 5,   /* < Temperature detection calibration value */
    /*
     * The 32.768 kHz clock is used to provide the stable
     * clock of some modules in the deep sleep. This API is used to calibrate the clock.
     */
    EXT_EFUSE_OSC_TRIM_RO_ID = 6,
    EXT_EFUSE_LDO_TRIM_RO_ID = 7,          /* < Not supported */
    EXT_EFUSE_RSIM_RW_ID = 8,              /* < Not supported */
    EXT_EFUSE_SUBKEY_CAT_RW_ID = 9,        /* < The value for DW21 is fixed to 0. */
    EXT_EFUSE_INSECURE_VERSION_RW_ID = 10, /* < Used to distinguish the R&D version from the production version */
    EXT_EFUSE_JTM_RW_ID = 11,              /* < JTAG mask */
    EXT_EFUSE_UTM0_RW_ID = 12,             /* < UART0 mask */
    EXT_EFUSE_UTM1_RW_ID = 13,             /* < UART0 mask */
    EXT_EFUSE_SDC_RW_ID = 14,              /* < UART/ JTAG authentication switch */
    EXT_EFUSE_BOOT_SEL_RW_ID = 15,         /* < Boot mode. 0: boot from ROM; 1: boot from flash */
    EXT_EFUSE_SECURE_BOOT_RW_ID = 16,      /* < Secured boot switch */
    EXT_EFUSE_IDX_MAX,                     /* < eFUSE maximum area ID, which cannot be used */
} ext_efuse_idx_e;

/*
 * @ingroup  hct_cipher
 *
 * @li eFUSE lock ID. Every 1 bit corresponds to 128 bits of the storage area.
 * @li The bits of the enumeration value ID are merged based on the usage of the eFUSE storage area.
 * @li EXT_EFUSE_LOCK_CHIP_DIE_X_ID  Number of operated bits
 */
typedef enum {
    EXT_EFUSE_LOCK_CHIP_DIE_2_ID,                  /* < EXT_EFUSE_CHIP_RO_ID and EXT_EFUSE_DIE_RO_ID locked */
    EXT_EFUSE_LOCK_ROOT_PUBKEY_2_ID,               /* < EXT_EFUSE_ROOT_PUBKEY_RW_ID locked */
    EXT_EFUSE_LOCK_UART_PUBKEY_2_ID,               /* < EXT_EFUSE_UART_PUBKEY_RW_ID locked */
    EXT_EFUSE_LOCK_BUCK_TSENSOR_OSC_LOD_TRIM_1_ID, /* < Areas between EXT_EFUSE_BUCK_TRIM_RO_ID
                                                            and EXT_EFUSE_LDO_TRIM_RO_ID locked */
    EXT_EFUSE_LOCK_RSIM_SUBKEYCAT_VER_1_ID,        /* < Areas between EXT_EFUSE_RSIM_RW_ID and
                                                            EXT_EFUSE_INSECURE_VERSION_RW_ID locked */
    /* < Areas between EXT_EFUSE_JTM_RW_ID and EXT_EFUSE_UTM1_RW_ID locked */
    EXT_EFUSE_LOCK_JTM_UTM_1_ID,
    /* < Areas between EXT_EFUSE_SDC_RW_ID and EXT_EFUSE_SECURE_BOOT_RW_ID locked */
    EXT_EFUSE_LOCK_SDC_BOOTSEL_SECBOOT_1_ID,
    EXT_EFUSE_LOCK_USER1_1_ID,     /* < Reserved area locked */
    EXT_EFUSE_LOCK_USER2_1_ID,     /* < Reserved area locked */
    EXT_EFUSE_LOCK_RESERVED1_1_ID, /* < Reserved area locked */
    EXT_EFUSE_LOCK_RESERVED2_1_ID, /* < Reserved area locked */
    EXT_EFUSE_LOCK_RESERVED3_1_ID, /* < Reserved area locked */
    EXT_EFUSE_LOCK_MAX,            /* < Maximum lock ID, which cannot be used */
} ext_efuse_lock_e;

/*
 * @ingroup  hct_cipher
 * @brief Obtains the preset length of each eFUSE area.
 *
 * @par Description:
 * Obtains the preset length of each eFUSE area. For details about the area description,
 * see the ext_efuse_idx_e and DW21 V100 API Development Guide.
 *
 * @attention None
 * @param  idx  [IN]  Type #ext_efuse_idx_e eFUSE ID
 * @retval #0         Success
 * @retval #Other values       Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_cipher.h: Describes the encryption and decryption APIs.
 * @see  uapi_efuse_write|uapi_efuse_read
 * @since DW21_V100R001C00
 */
td_u16 uapi_efuse_get_id_size(ext_efuse_idx_e idx);
/*
 * @ingroup  hct_cipher
 * @brief Reads the eFUSE.
 *
 * @par Description:
 * Reads the eFUSE. For details about the area description, see the ext_efuse_idx_e and DW21 V100 API Development
 * Guide.
 *
 * @attention The user needs to call uapi_efuse_get_id_size to obtain the area length, apply for space, and transfer the
 * space address.
 * @param  en_efuse_id  [IN]  Type #ext_efuse_idx_e eFUSE ID
 * @param  data     [OUT] Type #td_u8* Address for saving the read data
 *
 * @retval #0         Success
 * @retval #Other values       Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_cipher.h: Describes the encryption and decryption APIs.
 * @see  uapi_efuse_write|uapi_efuse_get_id_size
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_efuse_read(ext_efuse_idx_e en_efuse_id, td_u8 *data);

/*
 * @ingroup  hct_cipher
 * @brief Writes the eFUSE.
 *
 * @par Description:
 * Writes the eFUSE. For details about the area description, see the ext_efuse_idx_e and DW21 V100 API Development
 * Guide.
 *
 * @attention The user needs to call uapi_efuse_get_id_size to obtain the area length, apply for space,
 * and place the data to be written into the requested area.
 * @param  en_efuse_id   [IN] Type #ext_efuse_idx_e eFUSE ID
 * @param  data      [IN] Type #td_u8* Data to be written to the eFUSE
 *
 * @retval #0          Success
 * @retval #Other values        Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_cipher.h: Describes the encryption and decryption APIs.
 * @see  uapi_efuse_read|uapi_efuse_get_id_size
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_efuse_write(ext_efuse_idx_e en_efuse_id, td_u8 *data);

/*
 * @ingroup  hct_cipher
 * @brief Locks an area in the eFUSE. After the lock takes effect upon reboot, the area cannot be written.
 *
 * @par Description:
 * Locks an area in the eFUSE. After being locked, the area cannot be written.
 * Each bit corresponds to the 128 bits of the storage area.
 *
 * @attention
 * @li This operation takes effect only after the system is rebooted.
 * @li Locking is a risky operation. For details, see the ext_efuse_lock_e and DW21 V100 API Development Guide.
 * @param  en_efuse_lock_id  [IN] Type #ext_efuse_lock_e eFUSE ID to be locked
 *
 * @retval #0             Success
 * @retval #Other values           Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_cipher.h: Describes the encryption and decryption APIs.
 * @see  uapi_efuse_get_lockstat
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_efuse_lock(ext_efuse_lock_e en_efuse_lock_id);

/*
 * @ingroup  hct_cipher
 * @brief Obtains the lock status of the eFUSE and queries which areas are locked.
 *
 * @par Description:
 * Obtains the lock status of the eFUSE and queries which areas are locked.
 *
 * @attention Locking is a risky operation. For details, see the ext_efuse_lock_e and DW21 V100 API Development Guide.
 * @param  lock_stat   [OUT] Type #td_u16* Lock status of the eFUSE
 *
 * @retval #0          Success
 * @retval #Other values        Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_cipher.h: Describes the encryption and decryption APIs.
 * @see  uapi_efuse_lock
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_efuse_get_lockstat(td_u16 *lock_stat);

/*
 * @ingroup  hct_cipher
 * @brief Reads the eFUSE.
 *
 * @par Description:
 * Reads a reserved area in the eFUSE. Reading from any specified address is supported.
 *
 * @attention This API is used to operate the content in a reserved area. To read other pre-allocated areas,
 * uapi_efuse_read is recommended by #ext_efuse_idx_e.
 * @param  start_bit  [IN]  Type #td_u16 Start bit. The address must be 8-bit aligned.
 * @param  size       [IN]  Type #td_u16 Number of bits to be read. If the input is not 8-bit aligned,
 * the function performs 8-bit alignment internally. The user needs to process the read data before using it.
 * @param  key_data  [OUT] Type #td_u8* Address for saving the read data
 *
 * @retval #0         Success
 * @retval #Other values       Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_cipher.h: Describes the encryption and decryption APIs.
 * @see  uapi_efuse_usr_write
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_efuse_usr_read(td_u16 start_bit, td_u16 size, td_u8 *key_data);

/*
 * @ingroup  hct_cipher
 * @brief Writes data to a specified address of the eFUSE.
 *
 * @par Description:
 * Writes data to a specified address in the eFUSE, typically a reserved area.
 *
 * @attention
 * @li Generally, this API is used to write a reserved area. To write other pre-allocated areas,
 * uapi_efuse_write and uapi_efuse_lock are recommended by #ext_efuse_idx_e.
 * @li Data can be written to any address, but exercise with caution to avoid conflicts.
 * @li Write operations are risky operations. For details, see the DW21 V100 API Development Guide.
 * @param  start_bit  [IN] Type #td_u16 Start bit. The value of start_bit plus size must be less than the eFUSE size
 * (2048).
 * @param  size       [IN] Type #td_u16 Number of bits to be written. 1-to-256-bit write is supported.
 * @param  key_data  [IN] Type #td_u8* Address for the data to be written. The maximum length is 32 bytes.
 *
 * @retval #0         Success
 * @retval #Other values       Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_cipher.h: Describes the encryption and decryption APIs.
 * @see  uapi_efuse_usr_read
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_efuse_usr_write(td_u16 start_bit, td_u16 size, td_u8 *key_data);

/*
 * @ingroup  hct_cipher
 * @brief  Initializes the HASH algorithm, covering SHA256 and HMAC calculation.
 *
 * @par Description:
 * Initializes the HASH algorithm, covering SHA256 and HMAC calculation.
 *
 * @attention This API can be called only once in initialization after power on. Multi-tasking is not supported.
 * @param  None
 *
 * @retval #0      Success
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_cipher.h: Describes the encryption and decryption APIs.
 * @see  uapi_hash_deinit
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_hash_init(td_void);

/*
 * @ingroup  hct_cipher
 * @brief  Deinitializes the HASH algorithm, covering SHA256 and HMAC calculation.
 *
 * @par Description:
 * Deinitializes the HASH algorithm, covering SHA256 and HMAC calculation.
 *
 * @attention Multi-tasking is not supported.
 * @param  None
 *
 * @retval #0      Success
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_cipher.h: Describes the encryption and decryption APIs.
 * @see  uapi_hash_init
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_hash_deinit(td_void);

/*
 * @ingroup hct_cipher
 * Input parameters for initializing the HASH algorithm
 */
typedef struct {
    td_u8 *hmac_key;            /* < Key pointer */
    td_u32 hmac_key_len;        /* < Key length, less than 119 bytes */
    EXT_CONST td_u8 *input_data; /* < Pointer to the data to be calculated */
    td_u32 input_data_len;      /* < Length of the data to be calculated */
} ext_hmac_para_s;

typedef td_void (*hash_prepare_func)(td_void);
typedef td_void (*hash_restore_func)(td_void);
typedef struct {
    hash_prepare_func prepare_func;
    hash_restore_func restore_func;
} EXT_HASH_REGISTER_S;

/*
 * @ingroup  hct_cipher
 * @brief HMAC encryption
 *
 * @par Description:
 * HMAC encryption
 *
 * @attention None
 * @param  p_input     [IN]  Type #ext_hmac_para_s* Encryption parameters, including the key,
 * key length, source text, and source text length
 * @param  p_output_hash  [OUT] Type #td_u8* Encrypted text
 *
 * @retval #0           Success
 * @retval #Other values         Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_cipher.h: Describes the encryption and decryption APIs.
 * @see  uapi_hash_sha256
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_hash_hmac(EXT_CONST ext_hmac_para_s *p_input, td_u8 *p_output_hash);

/*
 * @ingroup  hct_cipher
 * @brief Calculates the SHA256 value.
 *
 * @par Description:
 * Calculates the SHA256 value of specified data.
 *
 * @attention None
 * @param  p_input_data     [IN]  Type #td_u8* Input data to be calculated
 * @param  input_data_len   [IN]  Type #td_u32 Length of the data to be calculated (unit: byte)
 * @param  p_output_hash    [OUT] Type #td_u8* Calculated SHA256 value
 *
 * @retval #0             Success
 * @retval #Other values           Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_cipher.h: Describes the encryption and decryption APIs.
 * @see  uapi_hash_hmac
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_hash_sha256(EXT_CONST td_u8 *p_input_data, td_u32 input_data_len, td_u8 *p_output_hash);

/*
 * @ingroup  hct_cipher
 * @brief Starts piecewise HASH calculation.
 *
 * @par Description:
 * Starts piecewise HASH calculation.
 *
 *
 * @attention None
 * @param  phandle  [OUT] Type #td_u16* Returns the handle of the current operation.
 *
 * @retval #0       Success
 * @retval #Other values     Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_cipher.h: Describes the encryption and decryption APIs.
 * @see  uapi_hash_update|uapi_hash_final
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_hash_start(td_u16 *phandle);

/*
 * @ingroup  hct_cipher
 * @brief Inputs the source data to be calculated piece by piece.
 *
 * @par Description:
 * Inputs the source data to be calculated piece by piece. The API can be called multiple times.
 *
 * @attention None
 * @param  handle        [IN] Type #td_u16 Handle returned by the uapi_hash_start API
 * @param  p_input_data    [IN] Type #td_u8* Input data pointer
 * @param  input_data_len  [IN] Type #td_u32 Input data length (unit: byte)
 *
 * @retval #0            Success
 * @retval #Other values          Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_cipher.h: Describes the encryption and decryption APIs.
 * @see  uapi_hash_start|uapi_hash_final
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_hash_update(td_u16 handle, EXT_CONST td_u8 *p_input_data, td_u32 input_data_len);

/*
 * @ingroup  hct_cipher
 * @brief Outputs the result of the piecewise calculation.
 *
 * @par Description:
 * Outputs the result of the piecewise calculation.
 *
 * @attention None
 * @param  handle        [IN]  Type #td_u16 Handle returned by the uapi_hash_start API
 * @param  p_output_hash   [OUT] Type #td_u8* Output HASH value
 *
 * @retval #0            Success
 * @retval #Other values          Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_cipher.h: Describes the encryption and decryption APIs.
 * @see  uapi_hash_start|uapi_hash_update
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_hash_final(td_u16 handle, td_u8 *p_output_hash);

EXT_EXTERN td_void uapi_hash_register(EXT_HASH_REGISTER_S sfunc);

/*
 * @ingroup  hct_cipher
 * @brief Generates a 32-bit true random number based on the random source of the random number module.
 *
 * @par Description:
 * Generates a 32-bit true random number based on the random source of the random number module.
 *
 * @attention None
 * @param  pvalue [OUT] Type #td_u32* Random number
 *
 * @retval #0       Success
 * @retval #Other values     Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_random.h: Describes the encryption and decryption APIs.
 * @see None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_trng_getrandom(EXT_OUT td_u32 *pvalue);

typedef td_void (*trng_prepare_func)(td_void);
typedef td_void (*trng_restore_func)(td_void);
typedef struct {
    trng_prepare_func prepare_func;
    trng_restore_func restore_func;
} EXT_TRNG_REGISTER_S;

/* Registration function, used for debug or low power mode. The value can be null. */
EXT_EXTERN td_void uapi_trng_register(EXT_TRNG_REGISTER_S sfunc);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif // __cplusplus

#endif // __SOC_MDM_CIPHER_H__
