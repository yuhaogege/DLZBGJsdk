/************************************************************************
*             Copyright (CompanyNameMagicTag) CompanyNameMagicTag
*                           ALL RIGHTS RESERVED
* FileName: uart_auth.c
* Description:
*
*************************************************************************/

#include "soc_boot_rom.h"
#include "uart_auth.h"

typedef enum {
    RX_STATUS_WAIT_START_0,
    RX_STATUS_WAIT_START_1,
    RX_STATUS_WAIT_START_2,
    RX_STATUS_WAIT_START_3,
    RX_STATUS_WAIT_SIZE_0,
    RX_STATUS_WAIT_SIZE_1,
    RX_STATUS_WAIT_CS_0,
    RX_STATUS_WAIT_CS_1,
    RX_STATUS_WAIT_TYPE_0,
    RX_STATUS_WAIT_TYPE_1,
    RX_STATUS_WAIT_PAD_0,
    RX_STATUS_WAIT_PAD_1,
    RX_STATUS_WAIT_DATA,
} RX_STATUS_E;

#define UART_RX_DELAY_MS   (100)   /* 100ms */
#define UART_FRAME_TIMEOUT (20000) /* The first character times out for 20 seconds. */
#define UTM_DISABLE        (1)
#define SDC_DISABLE        (0)
#define UART_ABNORMAL_MAX  (100)

td_s32 uart_auth_init(td_void);

td_s32 uart_auth_deinit(td_void);

td_s32 uart_auth_random_tx(UART_AUTH_CTX_S *ctx);

td_u32 uart_auth_result_tx(UART_AUTH_CTX_S *ctx);

td_s32 uart_auth_random_response_rx(UART_AUTH_CTX_S *ctx);

td_s32 uart_auth_random_response_verify(UART_AUTH_CTX_S *ctx);

td_s32 uart_auth_frame_rx(UART_AUTH_CTX_S *ctx, td_u32 first_byte_timeout_ms);

td_s32 uart_auth_frame_head_rx(UART_AUTH_CTX_S *ctx, td_u32 first_byte_timeout_ms);

td_s32 uart_auth_frame_data_rx(UART_AUTH_CTX_S *ctx);

td_s32 uart_auth_base_process(UART_AUTH_CTX_S *ctx);

td_s32 uart_auth_challenge_process(UART_AUTH_CTX_S *ctx);

/* Obtains the length (bits) random number. The length must be a multiple of 32 */
td_s32 uart_auth_get_random_data(td_u32 *data, td_u16 length);

td_s32 uart_auth_challenge_tx(UART_AUTH_CTX_S *ctx, td_u16 k_bits);

td_s32 uart_auth_challenge_response_verify(UART_AUTH_CTX_S *ctx, td_u16 k_bits);

td_s32 uart_auth_wait_usr_interrupt(UART_AUTH_CTX_S *ctx, td_u32 interrupt_timeout_ms);

td_s32 uart_auth_usr_interrupt_verify(UART_AUTH_CTX_S *ctx);

UART_AUTH_CTX_S *g_uart_auth_ctx = TD_NULL;

td_void uart_auth_init_addr(td_void)
{
    return;
}

td_s32 uart_auth_init(td_void)
{
    if (TD_NULL != g_uart_auth_ctx) {
        (td_void) memset_s(g_uart_auth_ctx, sizeof(UART_AUTH_CTX_S), 0, sizeof(UART_AUTH_CTX_S));
        return BOOT_SUCCESS;
    }

    g_uart_auth_ctx = (UART_AUTH_CTX_S *)boot_malloc(sizeof(UART_AUTH_CTX_S));
    if (g_uart_auth_ctx != NULL) {
        (td_void) memset_s(g_uart_auth_ctx, sizeof(UART_AUTH_CTX_S), 0, sizeof(UART_AUTH_CTX_S));
        return BOOT_SUCCESS;
    }
    return BOOT_FAILURE;
}

td_s32 uart_auth_deinit(td_void)
{
    td_s32 ret = BOOT_FAILURE;
    ret = boot_free(g_uart_auth_ctx);

    if (ret == BOOT_SUCCESS) {
        g_uart_auth_ctx = TD_NULL;
    }

    return ret;
}

UART_AUTH_CTX_S *uart_auth_ctx(td_void)
{
    return g_uart_auth_ctx;
}

td_s32 uart_auth_random_tx(UART_AUTH_CTX_S *ctx)
{
    PACKET_DATA_HEAD *head = &ctx->packet.head;
    td_u8 *payload = ctx->packet.payload;
    td_u32 *random = ctx->random;
    td_s32 ret = BOOT_SUCCESS;

    /* Generate a random number. */
    (td_void) memset_s(random, sizeof(ctx->random), 0, sizeof(ctx->random));
    ret = uart_auth_get_random_data(random, RANDOM_BITS);

    if (BOOT_SUCCESS != ret) {
        return BOOT_FAILURE;
    }

    head->start_flag = UART_PACKET_START_FLAG;
    head->packet_size = UART_AUTH_CHALLANGE_LEN * sizeof(td_u32);
    head->type = UART_AUTH_TYPE_RANDOM;
    head->pad = 0;
    (td_void) memcpy_s(payload, UART_PACKET_PAYLOAD_MAX, random, head->packet_size);
    head->check_sum = crc16_ccitt(0, payload, head->packet_size);

    serial_put_buf ((const char *)&(ctx->packet), (int)(head->packet_size + sizeof(PACKET_DATA_HEAD)));

    return BOOT_SUCCESS;
}

td_u32 uart_auth_result_tx(UART_AUTH_CTX_S *ctx)
{
    PACKET_DATA_HEAD *head = &ctx->packet.head;
    td_u8 *payload = ctx->packet.payload;
    td_char *result = "SUCCESS";

    head->start_flag = UART_PACKET_START_FLAG;
    head->packet_size = (td_u16)strlen(result);
    (td_void) memcpy_s(payload, UART_PACKET_PAYLOAD_MAX, result, (size_t)head->packet_size);
    head->check_sum = crc16_ccitt(0, payload, head->packet_size);

    serial_put_buf ((const char *)&(ctx->packet), (int)(head->packet_size + sizeof(PACKET_DATA_HEAD)));

    return BOOT_SUCCESS;
}

td_s32 uart_auth_frame_rx(UART_AUTH_CTX_S *ctx, td_u32 first_byte_timeout_ms)
{
    PACKET_DATA_INFO *packet = &ctx->packet;
    PACKET_DATA_HEAD *head = &packet->head;
    td_u8 *payload = packet->payload;
    td_s32 ret = BOOT_SUCCESS;
    td_u16 cs = 0;

    /* Reset receiving status */
    ctx->status = RX_STATUS_WAIT_START_0;
    ctx->received = 0;
    (td_void) memset_s(packet, sizeof(PACKET_DATA_INFO), 0, sizeof(PACKET_DATA_INFO));

    ret = uart_auth_frame_head_rx(ctx, first_byte_timeout_ms);

    if (BOOT_SUCCESS != ret) {
        return ret;
    }

    ret = uart_auth_frame_data_rx(ctx);

    if (BOOT_SUCCESS != ret) {
        return BOOT_FAILURE;
    }

    cs = crc16_ccitt(0, payload, head->packet_size);

    if (cs != head->check_sum) {
        return BOOT_FAILURE;
    }

    return BOOT_SUCCESS;
}

td_s32 uart_auth_frame_head_rx(UART_AUTH_CTX_S *ctx, td_u32 first_byte_timeout_ms)
{
    PACKET_DATA_HEAD *head = &ctx->packet.head;
    td_s32 ch = -1;
    td_bool reset_flag = TD_FALSE;
    td_u16 rcv = 0;
    td_u8 *payload = (td_u8 *)head;

    while (rcv <= UART_ABNORMAL_MAX) {
        if (ctx->status == RX_STATUS_WAIT_START_0) {
            ch = serial_getc_timeout(first_byte_timeout_ms * 1000);
        } else {
            ch = serial_getc_timeout(UART_RX_DELAY_MS * 1000);
        }

        if (ch < 0) {
            if (ctx->status == RX_STATUS_WAIT_START_0) {
                return EXT_FIRST_CHAR_TIMEOUT;
            } else {
                return BOOT_FAILURE;
            }
        }

        rcv++;

        if (TD_TRUE == reset_flag) {
            reset_flag = TD_FALSE;
            head->start_flag = 0;
            ctx->status = RX_STATUS_WAIT_START_0;
        }

        if (ctx->status <= RX_STATUS_WAIT_START_3) {
            td_u32 start_flag = UART_PACKET_START_FLAG;
            td_u8 *start_byte = (td_u8 *)&start_flag;

            if (ch == start_byte[ctx->status]) {
                payload[ctx->status] = (td_u8)ch;
                ctx->status++;
                continue;
            } else if (ch == 0xEF) {
                payload[RX_STATUS_WAIT_START_0] = (td_u8)ch;
                ctx->status = RX_STATUS_WAIT_START_1;
                continue;
            }
            reset_flag = TD_TRUE;
            continue;
        } else {
            payload[ctx->status] = (td_u8)ch;

            if (ctx->status == RX_STATUS_WAIT_SIZE_1) {
                if (head->packet_size > UART_PACKET_PAYLOAD_MAX) {
                    reset_flag = TD_TRUE;
                    continue;
                }
            }

            ctx->status++;

            if (ctx->status == RX_STATUS_WAIT_DATA) {
                return BOOT_SUCCESS;
            }
        }
    }

    return BOOT_FAILURE;
}

td_s32 uart_auth_frame_data_rx(UART_AUTH_CTX_S *ctx)
{
    PACKET_DATA_HEAD *head = &ctx->packet.head;
    td_u8 *payload = ctx->packet.payload;
    td_s32 ch = -1;

    ctx->received = 0;

    while (ctx->received < head->packet_size) {
        ch = serial_getc_timeout(UART_RX_DELAY_MS * 1000);

        if (ch < 0) {
            return BOOT_FAILURE;
        }

        payload[ctx->received++] = (td_u8)ch;
    }

    return BOOT_SUCCESS;
}

#define RANDOM_RETRY_MAX (100)
td_s32 uart_auth_random_response_rx(UART_AUTH_CTX_S *ctx)
{
    td_s32 ret = BOOT_SUCCESS;
    td_u32 i = 0;

    /* Discards the received fast-interruption packets. */
    for (i = 0; i < RANDOM_RETRY_MAX; i++) {
        uapi_watchdog_feed();
        ret = uart_auth_frame_rx(ctx, UART_FRAME_TIMEOUT);

        if (BOOT_SUCCESS != ret) {
      /* If the first character times out (no character is received for a long time), a failure message is returned. */
            if (ret == EXT_FIRST_CHAR_TIMEOUT) {
                return BOOT_FAILURE;
            } else {
                /* The non-first character times out. For example, if 0xEF is left in the first-order RX FIFO but 
                   the actual signature data is not received, the system continues to wait. */
                continue;
            }
        }

        if (ctx->packet.head.type == UART_AUTH_TYPE_CTRL_B
            || ctx->packet.head.type == UART_AUTH_TYPE_CTRL_C) {
            continue;
        } else {
            ret = uart_auth_random_response_verify(ctx);
            return ret;
        }
    }

    return BOOT_FAILURE;
}

td_s32 uart_auth_random_response_verify(UART_AUTH_CTX_S *ctx)
{
    td_s32 ret = BOOT_SUCCESS;
    PACKET_DATA_HEAD *head = &ctx->packet.head;
    td_u8 *payload = ctx->packet.payload;
    SIGN_PUBKEY_FRAME_HEAD_S *rx_head = (SIGN_PUBKEY_FRAME_HEAD_S *)payload;
    td_u32 length = sizeof(SIGN_PUBKEY_FRAME_HEAD_S);
    td_u8 efuse_hash[SHA_256_LENGTH] = {
        0,
    };
    td_u8 temp_hash[SHA_256_LENGTH] = {
        0,
    };
    td_u8 *data_sign = TD_NULL;
    td_u8 *data_n = TD_NULL;
    td_u8 *data_e = TD_NULL;

    if ((head->packet_size <= length) || (head->type != UART_AUTH_TYPE_RANDOM)) {
        return BOOT_FAILURE;
    }

    length = sizeof(SIGN_PUBKEY_FRAME_HEAD_S);
    length += rx_head->signature_len;
    length += rx_head->pubkey_n_len;
    length += rx_head->pubkey_e_len;

    if ((length != head->packet_size)
        || (rx_head->signature_len != RSA_2048_LENGTH)
        || (rx_head->pubkey_n_len > RSA_2048_LENGTH)
        || (rx_head->pubkey_e_len > RSA_2048_LENGTH)
        || (rx_head->padding_mode > EXT_MODE_PKCS_PSS)) {
        return BOOT_FAILURE;
    }

    data_sign = payload + sizeof(SIGN_PUBKEY_FRAME_HEAD_S);
    data_n = data_sign + rx_head->signature_len;
    data_e = data_n + rx_head->pubkey_n_len;

    /* Verify pubkey */
    (td_void) memset_s(&ctx->pubkey, sizeof(ctx->pubkey), 0, sizeof(ctx->pubkey));
    (td_void) memcpy_s(&ctx->pubkey.mod_n[RSA_2048_LENGTH - rx_head->pubkey_n_len], rx_head->pubkey_n_len, data_n,
                       rx_head->pubkey_n_len);
    (td_void) memcpy_s(&ctx->pubkey.exp_e[RSA_2048_LENGTH - rx_head->pubkey_e_len], rx_head->pubkey_e_len, data_e,
                       rx_head->pubkey_e_len);

    ret = uapi_hash_sha256((td_u8 *)&ctx->pubkey, sizeof(UART_AUTH_PUBKEY_S), temp_hash);

    if (BOOT_SUCCESS != ret) {
        return BOOT_FAILURE;
    }

    ret = uapi_efuse_read(EXT_EFUSE_UART_PUBKEY_RW_ID, efuse_hash);

    if (BOOT_SUCCESS != ret) {
        return BOOT_FAILURE;
    }

    if (0 != memcmp(temp_hash, efuse_hash, SHA_256_LENGTH)) {
        return BOOT_FAILURE;
    }

    ret = uapi_hash_sha256((td_u8 *)ctx->random, sizeof(ctx->random), temp_hash);

    if (BOOT_SUCCESS != ret) {
        return BOOT_FAILURE;
    }

    ret = uapi_rsa_verify(ctx->pubkey.mod_n, ctx->pubkey.exp_e, RSA_2048_LENGTH,
                        (ext_rsa_padding_mode)rx_head->padding_mode, temp_hash, data_sign);

    if (BOOT_SUCCESS != ret) {
        return BOOT_FAILURE;
    }

    return BOOT_SUCCESS;
}

td_bool uart_auth_process(ext_uart_e en_uart, td_u32 interrupt_timeout_ms)
{
    UART_AUTH_CTX_S *ctx = TD_NULL;
    td_s32 ret = BOOT_SUCCESS;
    td_u8 data = 1;
    ext_efuse_idx_e utm_temp;

    if (en_uart == UART0) {
        utm_temp = EXT_EFUSE_UTM0_RW_ID;
    } else if (en_uart == UART1) {
        utm_temp = EXT_EFUSE_UTM1_RW_ID;
    } else {
        return TD_FALSE;
    }

/* UTM：1,The serial port is disabled and exits directly without entering the interruption and authentication process */
    if (BOOT_SUCCESS == uapi_efuse_read(utm_temp, &data)) {
        if (UTM_DISABLE == data) {
            return TD_FALSE;
        }
    }

    ret = uart_auth_init();

    if (BOOT_SUCCESS != ret) {
        boot_put_errno(EXT_PRINT_ERRNO_UART_AUTH_INIT);
        return TD_FALSE;
    }

    ctx = uart_auth_ctx();

    set_start_status(EXT_BOOT_WAIT_UART_INTERRUPT);

    ret = uart_auth_wait_usr_interrupt(ctx, interrupt_timeout_ms);

    if (BOOT_SUCCESS != ret) {
        uart_auth_deinit();
        return TD_FALSE;
    }

    serial_cancel_mute();

    /* SDC：0，The authentication is disabled and the authentication process is not started. (By default, 
       the authentication process is enabled, and the eFUSE read failure authentication process is enabled.)) */
    if (BOOT_SUCCESS == uapi_efuse_read(EXT_EFUSE_SDC_RW_ID, &data)) {
        if (SDC_DISABLE == data) {
            /* Authentication not required */
            uart_auth_deinit();
            set_start_status(EXT_BOOT_OMIT_UART_AUTH);
            return TD_TRUE;
        }
    }

    for (;;) {
        /* Basic Authentication Process */
        ret = uart_auth_base_process(ctx);

        if (BOOT_SUCCESS == ret) {
            uart_auth_deinit();
            set_start_status(EXT_BOOT_UART_AUTH_SUCCESS);
            return TD_TRUE;
        }

        set_start_status(EXT_BOOT_UART_CHANLLANGE);
        /* Challenge process */
        ret = uart_auth_challenge_process(ctx);

        if (BOOT_SUCCESS != ret) {
            break;
        }
    }
    boot_put_errno(EXT_PRINT_ERRNO_UART_AUTH_ERR);
    uart_auth_deinit();
    return TD_FALSE;
}

td_s32 uart_auth_base_process(UART_AUTH_CTX_S *ctx)
{
    td_s32 ret = BOOT_SUCCESS;
    set_start_status(EXT_BOOT_UART_SEND_RAND);

    /* step 1. Sends 1536-bit random numbers. */
    ret = uart_auth_random_tx(ctx);

    if (BOOT_SUCCESS != ret) {
        boot_put_errno(EXT_PRINT_ERRNO_UART_RANDOM_SEND_ERR);
        return BOOT_FAILURE;
    }

    uapi_watchdog_feed();
    /* step 2. Received random number response packet: signature and public key */
    ret = uart_auth_random_response_rx(ctx);

    if (BOOT_SUCCESS != ret) {
        boot_put_errno(EXT_PRINT_ERRNO_UART_RANDOM_RSPONSE_ERR);
        return BOOT_FAILURE;
    }

    set_start_status(EXT_BOOT_UART_RECV_MSG);
    uart_auth_result_tx(ctx);

    return BOOT_SUCCESS;
}

td_s32 uart_auth_challenge_tx(UART_AUTH_CTX_S *ctx, td_u16 k_bits)
{
    PACKET_DATA_HEAD *head = &ctx->packet.head;
    td_u8 *payload = ctx->packet.payload;
    td_u32 *random = ctx->random;
    td_s32 ret = BOOT_SUCCESS;

    (td_void) memset_s(random, sizeof(ctx->random), 0, sizeof(ctx->random));
    ret = uart_auth_get_random_data(random, CHANLLANGE_BITS);

    if (BOOT_SUCCESS != ret) {
        return BOOT_FAILURE;
    }

    head->start_flag = UART_PACKET_START_FLAG;
    head->packet_size = SHA_256_LENGTH;
    head->type = UART_AUTH_TYPE_CHALLANGE;
    head->pad = 0;

    ret = uapi_hash_sha256((td_u8 *)random, k_bits / 8, payload);
    if (BOOT_SUCCESS != ret) {
        return BOOT_FAILURE;
    }

    head->check_sum = crc16_ccitt(0, payload, head->packet_size);

    serial_put_buf ((const char *)&(ctx->packet), (int)(head->packet_size + sizeof(PACKET_DATA_HEAD)));

    return BOOT_SUCCESS;
}

td_s32 uart_auth_challenge_process(UART_AUTH_CTX_S *ctx)
{
    td_u16 k_bits = CHANLLANGE_K_BITS_MIN;
    td_s32 ret = BOOT_SUCCESS;

    for (;;) {
        /* step 1. Pick 1024 bits random data. */
        /* Pick K LSB of this 1024bits and caculating SHA256 result of this K bits data. */
        ret = uart_auth_challenge_tx(ctx, k_bits);

        if (BOOT_SUCCESS != ret) {
            break;
        }

        uapi_watchdog_feed();
        /* step 2. Wait 'PC' reponse this K bits */
        ret = uart_auth_frame_rx(ctx, UART_FRAME_TIMEOUT);

        if (BOOT_SUCCESS != ret) {
            break;
        }

        uapi_watchdog_feed();
        ret = uart_auth_challenge_response_verify(ctx, k_bits);

        if (BOOT_SUCCESS == ret) {
            return BOOT_SUCCESS;
        }

        if (k_bits < CHANLLANGE_K_BITS_MAX) {
            k_bits *= CHANLLANGE_MULTI;
        }
    }

    return BOOT_FAILURE;
}

/* Obtains the length (bits) random number. The length must be a multiple of 32. */
td_s32 uart_auth_get_random_data(td_u32 *random_data, td_u16 length)
{
    td_u16 size = length / 0x20;
    td_u16 i = 0;

    if ((length & 0x1F) != 0) {
        return BOOT_FAILURE;
    }

    for (i = 0; i < size; i++) {
        uapi_watchdog_feed();
        uapi_get_random(&random_data[i]);
    }

    return BOOT_SUCCESS;
}

td_s32 uart_auth_challenge_response_verify(UART_AUTH_CTX_S *ctx, td_u16 k_bits)
{
    td_u16 k_bytes = k_bits / 8;
    PACKET_DATA_HEAD *head = &ctx->packet.head;

    if ((head->packet_size != k_bytes) || (head->type != UART_AUTH_TYPE_CHALLANGE)) {
        return BOOT_FAILURE;
    }

    if (0 != memcmp(ctx->packet.payload, ctx->random, k_bytes)) {
        return BOOT_FAILURE;
    }

    return BOOT_SUCCESS;
}

td_s32 uart_auth_wait_usr_interrupt(UART_AUTH_CTX_S *ctx, td_u32 interrupt_timeout_ms)
{
    td_s32 ret = BOOT_SUCCESS;

    ret = uart_auth_frame_rx(ctx, interrupt_timeout_ms);

    if (BOOT_SUCCESS != ret) {
        return BOOT_FAILURE;
    }

    return uart_auth_usr_interrupt_verify(ctx);
}

td_s32 uart_auth_usr_interrupt_verify(UART_AUTH_CTX_S *ctx)
{
    PACKET_DATA_HEAD *head = &ctx->packet.head;

    if ((head->packet_size == 1) && (g_st_softvalue.uart_interrupt_type == head->type) &&
        (ctx->packet.payload[0] == g_st_softvalue.interrupt_char)) {
        return BOOT_SUCCESS;
    }

    return BOOT_FAILURE;
}

