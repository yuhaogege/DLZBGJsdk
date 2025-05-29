/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Middle layer - common.
 * Author: CompanyName
 * Create: 2018-10-31
 */

#ifndef __MID_CMN_H__
#define __MID_CMN_H__

#include "mid_main.h"

#ifdef __cplusplus
extern "C" {
#endif

#define EXT_BROADCAST_MAC_ADDR "\xff\xff\xff\xff\xff\xff"

#define ext_broadcast_mac(mac) (0 == memcmp((mac), EXT_BROADCAST_MAC_ADDR, EXT_PLC_MAC_ADDR_LEN))

td_u32 mid_send_msg(ext_sys_queue_msg *msg);

#ifdef __cplusplus
}
#endif
#endif /* __MID_CMN_H__ */
