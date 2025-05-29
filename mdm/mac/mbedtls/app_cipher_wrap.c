/*
 * Copyright (c) CompanyNameMagicTag 2018-2018. All rights reserved.
 * Description: APP UART handle.
 */

#include <soc_mdm_cipher.h>
#include <soc_mdm_mem.h>
#include <soc_types.h>
#include <soc_mdm_timecnt.h>
#include <soc_types_rom.h>
#include <soc_stdlib.h>
#include <mbedtls/aes.h>
#include <mbedtls/timing.h>
#include <mbedtls/config.h>
EXT_CPP_START
#if defined (MBEDTLS_AES_ALT)
#define AES_KEY_BIT_LENGH       256
#define AES_KEY_BYTE_LENGH      32
uint8_t g_aes_deckey[AES_KEY_BYTE_LENGH] = {0};
uint8_t g_aes_enckey[AES_KEY_BYTE_LENGH] = {0};

void mbedtls_aes_init(mbedtls_aes_context *ctx)
{
    ext_unref_param(ctx);

    return;
}

void mbedtls_aes_free(mbedtls_aes_context *ctx)
{
    ext_unref_param(ctx);

    return;
}

int mbedtls_aes_setkey_enc(mbedtls_aes_context *ctx, const unsigned char *key,
                           unsigned int keybits)
{
    ext_unref_param(ctx);

    if (key == TD_NULL) {
        return -1;
    }

    if (keybits != AES_KEY_BIT_LENGH) {
        printf("error,keybits %d not equal 256\r\n", keybits);
        return -1;
    }
    memcpy_s(g_aes_enckey, AES_KEY_BYTE_LENGH, key, AES_KEY_BYTE_LENGH);
    return 0;
}

int mbedtls_aes_setkey_dec(mbedtls_aes_context *ctx, const unsigned char *key,
                           unsigned int keybits)
{
    ext_unref_param(ctx);

    if (key == TD_NULL) {
        return -1;
    }

    if (keybits != AES_KEY_BIT_LENGH) {
        printf("error, keybits %d not equal 256\r\n", keybits);
        return -1;
    }

    memcpy_s(g_aes_deckey, AES_KEY_BYTE_LENGH, key, AES_KEY_BYTE_LENGH);
    return 0;
}
int mbedtls_internal_aes_encrypt(mbedtls_aes_context *ctx,
                                 const unsigned char input[16],
                                 unsigned char output[16])
{
    ext_unref_param(ctx);
    uint8_t iv[16] = {0};

    if (input == TD_NULL || output == TD_NULL) {
        return -1;
    }

    uapi_aes_calc(g_aes_enckey, 32, iv, (const uint32_t)input, (const uint32_t)output, 16, 0);

    return 0;
}

int mbedtls_internal_aes_decrypt(mbedtls_aes_context *ctx,
                                 const unsigned char input[16],
                                 unsigned char output[16])
{
    ext_unref_param(ctx);

    if (input == TD_NULL || output == TD_NULL) {
        return -1;
    }

    uint8_t iv[16] = {0};
    uapi_aes_calc(g_aes_deckey, 32, iv, (const uint32_t)input, (const uint32_t)output, 16, 1);

    return 0;
}


int mbedtls_aes_crypt_ecb(mbedtls_aes_context *ctx,
                          int mode,
                          const unsigned char input[16],
                          unsigned char output[16])
{
    ext_unref_param(ctx);

    uint8_t iv[16] = {0};

    if (input == TD_NULL || output == TD_NULL) {
        return -1;
    }
    if (1 == mode)  // encrypt
        uapi_aes_calc(g_aes_enckey, 32, iv, (const uint32_t)input, (const uint32_t)output, 16, TD_FALSE);
    else
        uapi_aes_calc(g_aes_deckey, 32, iv, (const uint32_t)input, (const uint32_t)output, 16, TD_TRUE);
    return 0;
}

int mbedtls_aes_crypt_cbc(mbedtls_aes_context *ctx,
                          int mode,
                          size_t length,
                          unsigned char iv[16],
                          const unsigned char *input,
                          unsigned char *output)
{
    ext_unref_param(ctx);

    if (iv == TD_NULL || input == TD_NULL || output == TD_NULL) {
        return -1;
    }

    if (1 == mode)  // encrypt
        uapi_aes_calc(g_aes_enckey, 32, iv, (const uint32_t)input, (const uint32_t)output, length, TD_FALSE);
    else  // decrypt
        uapi_aes_calc(g_aes_deckey, 32, iv, (const uint32_t)input, (const uint32_t)output, length, TD_TRUE);
    return 0;
}

#endif

#if defined (MBEDTLS_TIMING_ALT)
/*
 * Set delays to watch
 */
#define TIME_MS_TO_US     1000
#define FUN_RETRUN_TWO    2
unsigned long mbedtls_timing_get_timer(struct mbedtls_timing_hr_time *val, int reset)
{
    struct _hr_time *t = (struct _hr_time *) val;

    if (reset) {
        gettimeofday(&t->start, NULL);
        return 0;
    } else {
        unsigned long delta;
        struct timeval now;
        gettimeofday(&now, NULL);
        delta = (now.tv_sec - t->start.tv_sec) * 1000ul
              + (now.tv_usec - t->start.tv_usec) / TIME_MS_TO_US;
        return delta;
    }
}

void mbedtls_timing_set_delay(void *data, uint32_t int_ms, uint32_t fin_ms)
{
    mbedtls_timing_delay_context *ctx = (mbedtls_timing_delay_context *) data;

    ctx->int_ms = int_ms;
    ctx->fin_ms = fin_ms;

    if (fin_ms != 0)
        (void) mbedtls_timing_get_timer(&ctx->timer, 1);
}

/*
 * Get number of delays expired
 */
int mbedtls_timing_get_delay(void *data)
{
    mbedtls_timing_delay_context *ctx = (mbedtls_timing_delay_context *) data;
    unsigned long elapsed_ms;

    if (ctx->fin_ms == 0)
        return -1;

    elapsed_ms = mbedtls_timing_get_timer(&ctx->timer, 0);
    if (elapsed_ms >= ctx->fin_ms)
        return FUN_RETRUN_TWO;

    if (elapsed_ms >= ctx->int_ms)
        return 1;

    return 0;
}
#endif

EXT_CPP_END
