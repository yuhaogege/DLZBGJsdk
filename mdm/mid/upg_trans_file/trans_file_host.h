/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Upgrade - Transmit file handling.
 * Author: CompanyName
 * Create: 2018-10-31
 */

#ifndef __TRANS_FILE_HOST_H__
#define __TRANS_FILE_HOST_H__

#include "soc_trans_save.h"

#ifdef __cplusplus
extern "C" {
#endif

td_void uapi_trans_host_stop_restore(td_void);
td_u32 uapi_trans_host_prepare(upg_trans_file_type file_type, td_u32 file_len, td_u32 crc);
td_u32 uapi_trans_host_get_content(td_u32 offset, td_u32 size, td_u8 *buffer);
td_u32 uapi_trans_host_restart_continue_init(td_void);

#ifdef __cplusplus
}
#endif
#endif /* __TRANS_FILE_HOST_H__ */
