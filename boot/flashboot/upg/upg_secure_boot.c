/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: boot security
 * Author: CompanyName
 * Create: 2018-10-31
 */

#include "upg_secure_boot.h"

EXT_PRV td_u32 flash_boot_verify_sign_alg(const sign_alg *alg)
{
    if ((alg->hash_alg == EXT_HASH_ALG_SHA256) && (alg->sign_alg <= EXT_MODE_PKCS_PSS)) {
        return EXT_ERR_SUCCESS;
    }

    return EXT_ERR_UPG_SECURE_BOOT_26;
}

EXT_PRV td_u32 flash_boot_verify_bootloader(EXT_CONST bootloader_section_and_len *sec_and_len, const td_u8 *mod_n,
    const td_u8 *exp_e, const td_u8 *signature, td_u8 sign_alg_type)
{
    td_u32 ret;
    td_u8 hash[SHA_256_LENGTH];
    td_u8 *hash_point = hash;
    (td_void)memset_s(hash, SHA_256_LENGTH, 0, SHA_256_LENGTH);

    ret = (td_u32)uapi_hash_sha256(sec_and_len->code_section, sec_and_len->section_length, hash);
    if (ret != EXT_ERR_SUCCESS) {
        return EXT_ERR_UPG_SECURE_BOOT_24;
    }

    ret = (td_u32)uapi_rsa_verify(mod_n, exp_e, RSA_2048_LENGTH, (ext_rsa_padding_mode)sign_alg_type,
        hash_point, signature);
    if (ret != EXT_ERR_SUCCESS) {
        return EXT_ERR_UPG_SECURE_BOOT_25;
    }

    return EXT_ERR_SUCCESS;
}

EXT_PRV td_u32 flash_boot_verify_root_pub_key(const root_pub_key *root_pubk)
{
    td_u32 ret;
    td_u8 hash[SHA_256_LENGTH];
    td_u8 efuse_hash[SHA_256_LENGTH];
    td_u8 *efuse_hash_point = efuse_hash;

    (td_void)memset_s(hash, SHA_256_LENGTH, 0, SHA_256_LENGTH);
    ret = (td_u32)uapi_hash_sha256((td_u8 *)root_pubk, sizeof(root_pub_key), hash);
    if (ret != EXT_ERR_SUCCESS) {
        return EXT_ERR_UPG_SECURE_BOOT_11;
    }

    (td_void)memset_s(efuse_hash, SHA_256_LENGTH, 0, SHA_256_LENGTH);
    ret = (td_u32)uapi_efuse_read(EXT_EFUSE_ROOT_PUBKEY_RW_ID, efuse_hash_point);
    if (ret != EXT_ERR_SUCCESS) {
        return EXT_ERR_UPG_SECURE_BOOT_12;
    }

    if (memcmp(hash, efuse_hash, SHA_256_LENGTH) != 0) {
        return EXT_ERR_UPG_SECURE_BOOT_13;
    }

    return EXT_ERR_SUCCESS;
}

EXT_PRV td_u32 flash_boot_verify_sub_key(EXT_CONST root_pub_key *root_pubk, EXT_CONST sub_key_info *sub_key)
{
    td_u32 ret;
    td_u32 category = (td_u32)-1;
    td_u8 hash[SHA_256_LENGTH];
    td_u8 *hash_point = hash;
    ret = (td_u32)uapi_efuse_read(EXT_EFUSE_SUBKEY_CAT_RW_ID, (td_u8 *)&category);
    if (ret != EXT_ERR_SUCCESS) {
        /* read 'category' failed. */
        return EXT_ERR_UPG_SECURE_BOOT_14;
    }
    if ((sub_key->category != category) || (sub_key->length != RSA_2048_LENGTH) ||
        (sub_key->sign_length != RSA_4096_LENGTH)) {
        /* 'category' not match. */
        return EXT_ERR_UPG_SECURE_BOOT_15;
    }
    ret = flash_boot_verify_sign_alg(&sub_key->sign_alg);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }
    if (sub_key->key_id > 31) { /* if more than 31, 'key_id' is invalid. */
        return EXT_ERR_UPG_SECURE_BOOT_16;
    }
    (td_void)memset_s(hash, SHA_256_LENGTH, 0, SHA_256_LENGTH);
    ret = (td_u32)uapi_hash_sha256((td_u8 *)sub_key, sizeof(sub_key_info) - RSA_4096_LENGTH, hash);
    if (ret != EXT_ERR_SUCCESS) {
        return EXT_ERR_UPG_SECURE_BOOT_17;
    }
    ret = (td_u32)uapi_rsa_verify(root_pubk->mod_n, root_pubk->exp_e, RSA_4096_LENGTH,
        (ext_rsa_padding_mode)sub_key->sign_alg.sign_alg, hash_point, sub_key->sign);
    if (ret != EXT_ERR_SUCCESS) {
        return EXT_ERR_UPG_SECURE_BOOT_18;
    }
    return EXT_ERR_SUCCESS;
}

EXT_PRV td_u32 flash_boot_verify_maintenance_key(const sub_key_info *sub_key, const maintenance_key *m_key)
{
    td_u8 die_id[DIE_ID_LENGTH];
    td_u8 hash[SHA_256_LENGTH];
    td_u8 *hash_point = hash;
    td_u8 *die_id_point = die_id;
    (td_void)memset_s(die_id, DIE_ID_LENGTH, 0, DIE_ID_LENGTH);
    if ((td_u32)uapi_efuse_read(EXT_EFUSE_DIE_RO_ID, die_id_point) != EXT_ERR_SUCCESS) {
        /* read 'DIE ID' failed. */
        return EXT_ERR_UPG_SECURE_BOOT_19;
    }
    if (memcmp(m_key->die_id, die_id, DIE_ID_LENGTH) != 0) {
        /* 'DIE id' not match. */
        return EXT_ERR_UPG_SECURE_BOOT_20;
    }
    if ((m_key->die_id_length != DIE_ID_LENGTH) || (m_key->key_length != RSA_2048_LENGTH * 2) /* 2*256 */
        || (m_key->sign_length != RSA_2048_LENGTH)) {
        /* 'length' error. */
        return EXT_ERR_UPG_SECURE_BOOT_21;
    }
    if (flash_boot_verify_sign_alg(&m_key->sign_alg) != EXT_ERR_SUCCESS) {
        return EXT_ERR_UPG_SECURE_BOOT_26;
    }
    (td_void)memset_s(hash, SHA_256_LENGTH, 0, SHA_256_LENGTH);
    if ((td_u32)uapi_hash_sha256((td_u8 *)m_key, sizeof(maintenance_key) - RSA_2048_LENGTH, hash) != EXT_ERR_SUCCESS) {
        return EXT_ERR_UPG_SECURE_BOOT_22;
    }
    if ((td_u32)uapi_rsa_verify(sub_key->mod_n, sub_key->exp_e, RSA_2048_LENGTH,
        (ext_rsa_padding_mode)m_key->sign_alg.sign_alg, hash_point, m_key->sign) != EXT_ERR_SUCCESS) {
        return EXT_ERR_UPG_SECURE_BOOT_23;
    }
    return EXT_ERR_SUCCESS;
}

EXT_PRV td_u32 flash_boot_verify_proc(td_u8 *data, const boot_header *header)
{
    td_u32 ret;
    td_u8 *mod_n = TD_NULL;
    td_u8 *exp_e = TD_NULL;
    root_pub_key *root_pubk = TD_NULL;
    sub_key_info *sub_key = TD_NULL;

    root_pubk = (root_pub_key *)(data + header->root_pubk_offset);

    if (header->root_pubk_length != sizeof(root_pub_key)) {
        return EXT_ERR_UPG_SECURE_BOOT_3;
    }

    ret = flash_boot_verify_root_pub_key(root_pubk);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    sub_key = (sub_key_info *)(data + header->sub_key_offset);
    if (header->sub_key_length != sizeof(sub_key_info)) {
        return EXT_ERR_UPG_SECURE_BOOT_5;
    }

    ret = flash_boot_verify_sub_key(root_pubk, sub_key);
    if (ret != EXT_ERR_SUCCESS) {
        return EXT_ERR_UPG_SECURE_BOOT_6;
    }

    mod_n = sub_key->mod_n;
    exp_e = sub_key->exp_e;

    if (header->maintenance_key_offset != 0) {
        maintenance_key *m_key = (maintenance_key *)(data + header->maintenance_key_offset);
        ret = flash_boot_verify_maintenance_key(sub_key, m_key);
        if (ret != EXT_ERR_SUCCESS) {
            return EXT_ERR_UPG_SECURE_BOOT_7;
        }

        mod_n = m_key->mod_n;
        exp_e = m_key->exp_e;
    }

    bootloader_section_and_len sec_and_len = { data + header->code_section_offset, header->code_section_length };
    ret =
        flash_boot_verify_bootloader(&sec_and_len, mod_n, exp_e, data + header->sign_offset, header->sign_alg.sign_alg);
    if (ret != EXT_ERR_SUCCESS) {
        return EXT_ERR_UPG_SECURE_BOOT_8;
    }

    return ret;
}

td_u32 flash_boot_security_boot_verify(td_pvoid flash_boot_addr) /* absolute address */
{
    td_u32 ret;
    td_u8 *data = (td_u8 *)flash_boot_addr;
    boot_header *header = flash_boot_addr;
    boot_code_head *code_head = TD_NULL;
    td_u8 secure_flag = 0;

    boot_msg1("verify1=", (uintptr_t)flash_boot_addr);

    if ((header->preamble != FLASH_BOOT_PREAMBLE) || (header->head_magic != FLASH_BOOT_HEAD_MAGIC)) {
        return EXT_ERR_UPG_SECURE_BOOT_1;
    }

    ret = (td_u32)uapi_efuse_read(EXT_EFUSE_SECURE_BOOT_RW_ID, &secure_flag);
    if (ret != EXT_ERR_SUCCESS) {
        return EXT_ERR_UPG_SECURE_BOOT_2;
    }

    code_head = (boot_code_head *)(data + header->code_section_offset);

    if (secure_flag == 1) {
        td_u32 nv_counter = 0;

        ret = flash_boot_verify_proc(data, header);
        if (ret != EXT_ERR_SUCCESS) {
            return ret;
        }

        ret = (td_u32)uapi_efuse_read(EXT_EFUSE_INSECURE_VERSION_RW_ID, (td_u8 *)&nv_counter);
        if (ret != EXT_ERR_SUCCESS) {
            return EXT_ERR_UPG_SECURE_BOOT_9;
        }

        boot_msg0("verify11");

        /* DEVELOPE: highest bit equal to 1; OUTSIDE: highest bit equal to 1; efuse highest bit equal to 1; */
        if (((~nv_counter) & code_head->version) != code_head->version) {
            return EXT_ERR_UPG_SECURE_BOOT_10;
        }
    }
    return EXT_ERR_SUCCESS;
}
