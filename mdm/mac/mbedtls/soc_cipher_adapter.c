/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: Chip adapter.
 */

#include <mbedtls/config.h>
#include "soc_mdm_cipher.h"

#if defined(MBEDTLS_DEPRECATED_REMOVED)
void mbedtls_sha256(const unsigned char *input,
                    size_t ilen,
                    unsigned char output[32],
                    int is224)
{
    if (g_is_hash_init == TD_FALSE) {
        uapi_hash_init();
    }
    
    if (0 == is224) {           // only support SHA256
        uapi_hash_sha256(input, ilen, output);
    } else {
        printf("error!not support sha224\r\n");
    }
}
#endif

