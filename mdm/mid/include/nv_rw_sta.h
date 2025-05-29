/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Middle layer - NV for sta/ndm.
 */

#ifndef __NV_RW_STA_H__
#define __NV_RW_STA_H__

#include <soc_mdm.h>
#include "mid_cmd_msg.h"

#ifdef __cplusplus
extern "C" {
#endif

td_u32 mid_sta_plc_nv_read(td_u16 cmd_id, EXT_CONST mid_plc_frame *frame);
td_u32 mid_sta_plc_nv_write(td_u16 cmd_id, EXT_CONST mid_plc_frame *frame);

#define mid_diag_nv_read(cmd_id, cmd, cmd_size, option)
#define mid_diag_nv_write(cmd_id, cmd, cmd_size, option)

#define mid_plc_nv_read(cmd_id, frame) mid_sta_plc_nv_read((cmd_id), (frame))
#define mid_plc_nv_write(cmd_id, frame) mid_sta_plc_nv_write((cmd_id), (frame))

#ifdef __cplusplus
}
#endif
#endif /* __NV_RW_STA_H__ */
