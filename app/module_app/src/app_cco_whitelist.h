/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: White list handle.
 */

#ifndef APP_CCO_WHITELIST_H
#define APP_CCO_WHITELIST_H

#include "soc_types.h"

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
#ifdef __cplusplus
extern "C" {
#endif

#define WHITE_LIST_NODE_SIZE EXT_PLC_MAC_ADDR_LEN

#if !defined(PRODUCT_CFG_SDK_WHTIE_LIST_SUPPORT)
#define WHITE_LIST_MAGIC     uapi_make_identifier('W', 'L', 'S', 'T') /* WLST */
#define WHITE_LIST_DATA_SIZE 4086 /* 4096 - sizeof(crc) - sizeof(magic) - sizeof(total) */
/* White List: size - 4KB; */
typedef struct {
    td_u32 crc;                       /* CRC32 check, including magic, total, and list */
    td_u32 magic;                     /* WLST */
    td_u16 total;                     /* Total number of nodes */
    td_u8 list[WHITE_LIST_DATA_SIZE]; /* The 6-byte value is a node. The invalid node is
                                         filled with all FFs: 6 * 681. */
} app_whitelist_head;
#else
typedef struct {
    td_u16 total;                     /* Total number of nodes */
    td_u8 list[0];
} app_whitelist_head;
#endif
/* Setup whitelist to MAC */
td_u32 app_whitelist_setup(td_void);
/* White List: clear */
td_u32 app_whitelist_clear(td_void);
/* White List: Get White List */
app_whitelist_head *app_get_whitelist(td_void);
/* White List: Add node */
td_u32 app_whitelist_add(td_pbyte list, td_u16 count);
/* White List: Del node */
td_u32 app_whitelist_delete(td_pbyte list, td_u16 count);
/* timeout proc */
td_void app_whitelist_timeout(td_void);
td_u16 app_get_whitelist_sta_num(td_void);
td_void app_whitelist_timeout_notify_mac(td_void);
#ifdef __cplusplus
}
#endif
#endif
#endif

