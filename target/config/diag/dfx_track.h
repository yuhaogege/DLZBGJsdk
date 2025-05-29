/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: dfx_track .
 * Author: CompanyName
 * Create: 2018-10-31
 */
#ifndef __DFX_TRACK_H__
#define __DFX_TRACK_H__
#include <soc_types.h>
#include <soc_mdm_types.h>

typedef struct {
    td_u32 array_count; /* how many pieces of information are recorded in the track statistics? */
    td_u32 option;      /* track statistics support which type of track is logged (scheduling, off interrupt...) */
} ext_dfx_track_st_cmd;  /* start track statistics cmd */

typedef struct {
    td_u32 ret;        /* start error code */
} ext_dfx_track_st_ind; /* start track statistics ind */

typedef struct {
    td_u32 pad;
} ext_dfx_track_stop_cmd; /* stop track statistics cmd */

typedef struct {
    td_u32 ret;          /* stop error code */
} ext_dfx_track_stop_ind; /* stop track statistics ind */

typedef struct {
    /*
     * whether to perform a lock operation, a lock operation means to view new information,
     * and no lock operation means to re-view history information
     */
    td_u32 lock;
} ext_dfx_track_get_info_cmd; /* get track statistics cmd */

typedef struct {
    td_u32 ret;                /* error code */
    td_u32 array_count;        /* how many track statistics are supported at most, set at startup */
    td_u32 option;             /* what kind of track statistics are supported, startup is setting */
    td_u32 lock_pos;           /* currently locked subscript id */
} ext_dfx_track_info_total_ind; /* report the overall ind of the trajectory */

typedef struct {
    td_u32 ret;
    td_u32 seq; /* sequence number from 0 to ularraycount-1 */

    td_u32 cur_time;          /* statistical time point */
    td_u32 trace_type;        /* statistical type */
    td_u32 entry;             /* indicates the location of the call, the body of the task, etc. */
    td_u32 data0;             /* data 0 */
    td_u32 data1;             /* data 1 */
} ext_dfx_track_info_item_ind; /* report a track ind message */

#endif
