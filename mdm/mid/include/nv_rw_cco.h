/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Middle layer - NV for cco.
 * Author: CompanyName
 * Create: 2018-10-31
 */

#ifndef __NV_RW_CCO_H__
#define __NV_RW_CCO_H__

#include <soc_mdm.h>
#include "mid_cmd_msg.h"

#ifdef __cplusplus
extern "C" {
#endif

td_u32 mid_cco_diag_nv_read(td_u16 cmd_id, td_pvoid cmd, td_u16 cmd_size, td_u32 option);
td_u32 mid_cco_diag_nv_write(td_u16 cmd_id, td_pvoid cmd, td_u16 cmd_size, td_u32 option);

td_u32 mid_cco_plc_nv_proc(td_u16 cmd_id, EXT_CONST mid_plc_frame *frame);

#define mid_diag_nv_read(cmd_id, cmd, cmd_size, option) mid_cco_diag_nv_read((cmd_id), (cmd), (cmd_size), (option))
#define mid_diag_nv_write(cmd_id, cmd, cmd_size, option) mid_cco_diag_nv_write((cmd_id), (cmd), (cmd_size), (option))

#define mid_plc_nv_read(cmd_id, frame) mid_cco_plc_nv_proc((cmd_id), (frame))
#define mid_plc_nv_write(cmd_id, frame) mid_cco_plc_nv_proc((cmd_id), (frame))

#ifdef __cplusplus
}
#endif
#endif /* __NV_RW_CCO_H__ */
