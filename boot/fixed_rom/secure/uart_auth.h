/************************************************************************
*             Copyright (CompanyNameMagicTag) CompanyNameMagicTag
*                           ALL RIGHTS RESERVED
* FileName: uart_auth.h
* Description:
*
*************************************************************************/

#ifndef __UART_AUTH_H__
#define __UART_AUTH_H__

#include <secure.h>

#define UART_PACKET_START_FLAG (0xDEADBEEF)

#define UART_PACKET_PAYLOAD_MAX (2048)
#define UART_AUTH_CHALLANGE_LEN (48) /* 1536/8/4 = 48 */

#define RANDOM_BITS (1536)

#define CHANLLANGE_BITS       (1024)
#define CHANLLANGE_K_BITS_MIN (16)
#define CHANLLANGE_K_BITS_MAX (CHANLLANGE_BITS)
#define CHANLLANGE_MULTI      (4)

enum {
    UART_AUTH_TYPE_RANDOM = 1,
    UART_AUTH_TYPE_CHALLANGE,
    UART_AUTH_TYPE_CTRL_B,
    UART_AUTH_TYPE_CTRL_C,
};

typedef struct
{
    td_u32 start_flag;  /**< Start ID: 0xDEADBEEF */
    td_u16 packet_size; /**< Packet length: indicates the actual length of transmitted data. 
                             The value cannot exceed 2048 bytes. */
    td_u16 check_sum;   /**< Checksum */
    td_u16 type;        /**< Packet Type */
    td_u16 pad;
} PACKET_DATA_HEAD;

typedef struct
{
    PACKET_DATA_HEAD head;
    td_u8 payload[UART_PACKET_PAYLOAD_MAX]; /**< Packet data */
} PACKET_DATA_INFO;

typedef struct
{
    td_u16 signature_len; /**< Signature length: RSA2048: 256 bytes */
    td_u16 pubkey_n_len;  /**< Length of the public key N: RSA2048: 256 bytes */
    td_u16 pubkey_e_len;  /**< Length of the public key E£º0x010001£¬The default value is 3 bytes */
    td_u8 padding_mode;   /**< Padding mode£ºext_rsa_padding_mode */
    td_u8 pad;
} SIGN_PUBKEY_FRAME_HEAD_S;

typedef struct
{
    td_u8 mod_n[RSA_2048_LENGTH];
    td_u8 exp_e[RSA_2048_LENGTH];
} UART_AUTH_PUBKEY_S;

/* UART AUTH context */
typedef struct
{
    td_u8 status;
    td_u8 pad;
    td_u16 received;
    td_u32 random[UART_AUTH_CHALLANGE_LEN];
    PACKET_DATA_INFO packet;
    UART_AUTH_PUBKEY_S pubkey;
} UART_AUTH_CTX_S;

UART_AUTH_CTX_S *uart_auth_ctx(td_void);

#endif  // __UART_AUTH_H__

