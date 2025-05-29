/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Upgrade - Transmit file handling.
 * Author: CompanyName
 * Create: 2018-10-31
 */

#ifndef __TRANS_FILE_SLAVE_H__
#define __TRANS_FILE_SLAVE_H__

#include "soc_trans_save.h"

#ifdef __cplusplus
extern "C" {
#endif

td_u32 uapi_trans_slave_get_max_file_size(td_void);
td_u32 uapi_trans_slave_save_content(td_u32 offset, td_u32 size, EXT_CONST td_u8 *buffer);
td_u32 uapi_trans_slave_start_prepare(upg_trans_file_type file_type, td_u32 file_len, td_u32 crc);
td_u32 uapi_trans_slave_stop_restore(td_void);
td_u32 uapi_trans_slave_finish(td_void);
td_u32 uapi_trans_slave_check_file(td_void);
td_u32 uapi_upg_get_slave_addr(td_void);
td_u32 uapi_upg_transmit_mem_write_prepare(upg_trans_file_type file_type, td_u32 file_len);
td_u32 uapi_receive_plc_start_fram(td_u8 *data, td_u32 data_size);
td_u32 uapi_trans_get_partition_end_addr(td_void);

#ifdef __cplusplus
}
#endif
#endif /* __TRANS_FILE_SLAVE_H__ */
