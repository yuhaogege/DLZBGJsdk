/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: boot head
 */

#ifndef __BOOT_HEADER_H__
#define __BOOT_HEADER_H__

#include <secure.h>
#include <soc_types.h>
#include <soc_boot_rom.h>

#define FLASH_BOOT_PREAMBLE 0x55AA55AA
#define FLASH_BOOT_HEAD_MAGIC 0x33CC33CC

#define DIE_ID_LENGTH 24

#define EXT_HASH_ALG_SHA256 0

typedef struct {
    td_u8 mod_n[RSA_4096_LENGTH]; /* Mod N */
    td_u8 exp_e[RSA_4096_LENGTH]; /* Exp E */
} root_pub_key;

typedef struct {
    td_u32 hash_alg : 16;   /* The signature algorithm: HASH algorithm, 0x0: SHA256, other: reserved. */
    td_u32 sign_alg : 6;    /* The signature algorithm: Signature algorithm,
                             * 0: RSA_PKCS1; 1: RSA_PSS; other: reserved.\n
                             * RSA_PSS should use the same HASH algorithm with [15:0]. */
    td_u32 sign_param : 10; /* The signature algorithm: Signature parameter, default '0'. */
} sign_alg;

typedef struct {
    td_u32 version;               /* Default 1. */
    sign_alg sign_alg;            /* The signature algorithm. */
    td_u32 category;              /* The category of SubKey,
                                   * which could be used to distinguish different products or chips. */
    td_u32 key_id;                /* [4:0] SubKeyID, 0~31; [31:5] Reserved. */
    td_u32 length;                /* Length of SubKey. RSA2048: 256 Bytes. */
    td_u32 sign_length;           /* Signature length of SubKey. RSA4096: 512Bytes. */
    td_u8 mod_n[RSA_2048_LENGTH]; /* Mod N */
    td_u8 exp_e[RSA_2048_LENGTH]; /* Exp E */
    td_u8 sign[RSA_4096_LENGTH];  /* Signature of Subkey. */
} sub_key_info;

typedef struct {
    td_u32 version;               /* Default 1. */
    sign_alg sign_alg;            /* The signature algorithm. */
    td_u32 time;                  /* UTC timestamp, time of signing. */
    td_u32 die_id_length;         /* length of Die ID. 0x18000000 */
    td_u8 die_id[DIE_ID_LENGTH];  /* Die ID. 24Bytes */
    td_u32 key_length;            /* Length of DevKey. RSA2048: 512 bytes = 256 bytes N + 256 bytes E. 0x00020000 */
    td_u8 mod_n[RSA_2048_LENGTH]; /* Mod N */
    td_u8 exp_e[RSA_2048_LENGTH]; /* Exp E */
    td_u32 sign_length;           /* Length of sign. RSA2048: 256 Bytes. 0x00010000 */
    td_u8 sign[RSA_2048_LENGTH];  /* Signature of Subkey. */
} maintenance_key;

typedef struct {
    td_u32 preamble;           /* Head information preamble code 0x55AA55AA,
                                * it needs to be validated at secure startup. */
    td_u32 head_length;        /* Head information total length, sizeof(BOOT_HEADER_S). */
    td_u32 root_pubk_alg;      /* RootPubK algorithm. 0x0: RSA4096; other: reserved. */
    td_u32 root_pubk_offset;   /* Offset of RootPubK, based on Bootloader head. */
    td_u32 root_pubk_length;   /* RootPubK length. RSA4096, value is 1024, total length of mod N and exp E. */
    td_u32 root_pubk_hash_alg; /* RootPubK HASH algorithm. At startup,
                                * BootROM calculate HASH value of RootPubK by this algorithm.
                                * 0x0: SHA256, other: reserved. */

    td_u32 sub_key_offset; /* Offset of SubKeyCert, based on Bootloader head. */
    td_u32 sub_key_length; /* Length of SubKeyCert. */

    td_u32 maintenance_key_offset; /* Offset of maintenance cert, based on Bootloader head.
                                    * if 0, no maintenance cert. */
    td_u32 maintenance_key_length; /* Length of maintenance cert. */

    td_u32 code_section_length; /* Length of Bootloader Code Section. */
    td_u32 code_section_offset; /* Offset of Bootloader Code Section. */

    sign_alg sign_alg; /* The signature algorithm. */

    td_u32 sign_offset; /* Offset of Bootloader Code Section Signature. */
    td_u32 sign_length; /* Length of Bootloader Code Section Signature. */

    td_u32 head_magic; /* Head information over. Magic is 0x33CC33CC.
                        * BootROM should check HeadMagic & Preamble at first. */
} boot_header;

typedef struct {
    td_u32 version;
    td_u32 compressed_flag : 1;
    td_u32 reserved : 31;
    td_u32 pad;
} boot_code_head;

#endif /* __BOOT_HEADER_H__ */
