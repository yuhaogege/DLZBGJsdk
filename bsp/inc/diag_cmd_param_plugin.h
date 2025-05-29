/* *
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: DIAG - param plugin.
 */

#ifndef DIAG_SYS_CMD_PARAM_PLUGIN_H
#define DIAG_SYS_CMD_PARAM_PLUGIN_H

#include "diag_cmd_id.h"

/* *
 * CMD ID : DIAG_CMD_NV_QRY  Search a NV item
 * REQ    : DIAG_CMD_NV_QRY_REQ_STRU
 * CNF    : DIAG_CMD_NV_QRY_CNF_STRU
 * IND    : NONE
 */
typedef struct {
    td_u32 nv_id; /* NV item ID to be acquired */
} diag_cmd_nv_qry_req_stru;

typedef struct {
    td_u32 rc;
    /* indicates whether the execution result is successful, 0 indicates success, and the others are error codes. */
    td_u32 nv_id;     /* Get the NV item Id */
    td_u32 data_size; /* Size of the NV item data obtained */
    td_u8 data[0];    /* Get NV item data */
} diag_cmd_nv_qry_cnf_stru;

/* *
 * CMD ID : DIAG_CMD_NV_WR
 * REQ    : DIAG_CMD_NV_WR_REQ_STRU
 * CNF    : DIAG_CMD_NV_WR_CNF_STRU
 * IND    : NONE
 */
typedef struct {
    td_u32 nv_id;     /* NV ID to be written */
    td_u32 data_size; /* Size of NV item data to be written */
    td_u8 data[0];    /* data buffer */
} diag_cmd_nv_wr_req_stru;

typedef struct {
    td_u32 rc;
    /* indicates whether the execution result is successful, 0 indicates success, and the others are error codes. */
    td_u32 nv_id;
} diag_cmd_nv_wr_cnf_stru;

#endif /* __DIAG_SYS_CMD_PARAM_PLUGIN_H__ */
