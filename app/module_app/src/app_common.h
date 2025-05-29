/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: APP common API.
 */

#ifndef APP_COMMON_H
#define APP_COMMON_H

#include "app_res.h"
#include "soc_mdm_timer.h"
#include "soc_mdm_msg.h"
#include "soc_types.h"

#define app_printf(fmt, ...)        printf(fmt, ##__VA_ARGS__)
#define app_print_buf(sz, buf, len) \
    do {                                               \
        td_s32 tmp_length = (len);                     \
        td_s32 tmp_i = 0;                              \
        app_printf((sz));                              \
        for (tmp_i = 0; tmp_i < tmp_length; tmp_i++) { \
            app_printf(" %02X", (buf)[tmp_i]);  \
        }                                              \
        app_printf("\n");                    \
    } while (0)

#define INVALID_MAC_ADDR    "\x00\x00\x00\x00\x00\x00"
#define BROADCAST_MAC_ADDR  "\xff\xff\xff\xff\xff\xff"
#define LOCAL_BROADCAST_MAC "\x00\xff\xff\xff\xff\xff"

#define is_invalid_mac_addr(x)         (memcmp((x), INVALID_MAC_ADDR, EXT_PLC_MAC_ADDR_LEN) == 0)
#define is_broadcast_mac_addr(x)       (memcmp((x), BROADCAST_MAC_ADDR, EXT_PLC_MAC_ADDR_LEN) == 0)
#define is_local_broadcast_mac_addr(x) (memcmp((x), LOCAL_BROADCAST_MAC, EXT_PLC_MAC_ADDR_LEN) == 0)
#define is_valid_mac_addr(x) (!is_invalid_mac_addr(x) && !is_broadcast_mac_addr(x) && !is_local_broadcast_mac_addr(x))

td_pbyte find_byte_in_stream(td_pbyte data, td_u16 data_length, td_u8 ch);
td_u32 app_timer_start(app_timer_id id, td_u32 period, ext_timer_type_e option);
td_u32 app_timer_stop(app_timer_id id);
td_u32 app_send_message(EXT_CONST ext_sys_queue_msg *msg);
td_u32 app_set_mac_addr(EXT_CONST td_u8 mac[EXT_PLC_MAC_ADDR_LEN], td_u32 mac_size);

#endif

