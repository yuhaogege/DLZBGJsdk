/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Track APIs.
 * Author: CompanyName
 * Create: 2018-10-31
 */
/**
 * @defgroup hct_track Track
 * @ingroup system
 */

#ifndef __SOC_MDM_TRACK_H__
#define __SOC_MDM_TRACK_H__

#include <soc_types.h>
#include <soc_mdm_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup hct_track
 * Enumeration of track statistics types.
 */
typedef enum {
    EXT_TRACK_TYPE_ISR = 0x10, /**< Interrupt response track_id:interrupt ID track_data:0 track_entry:0. */

    EXT_TRACK_TYPE_EVT_WAIT_SUCCCESS = 0x20, /**< Wait event succeeded, track_id: event ID track_data: wait event bits
                                             * track_entry:lr.
                                             */
    EXT_TRACK_TYPE_SEM_WAIT_SUCCCESS = 0x21, /**< Wait semaphore succeeded, track_id: semaphore ID track_data:0
                                             * track_entry:lr.
                                             */
    EXT_TRACK_TYPE_MSG_WAIT_SUCCCESS = 0x22, /**< Wai message succeeded, track_id: message queue ID track_data:
                                             * message ID track_entry: message pointer.
                                             */

    EXT_TRACK_TYPE_JOB_PROCESS = 0x31, /**< Add a job, track_id: lower 16 bits of parameter 2 track_data:
                                       * parameter 1 track_entry: callback function.
                                       */

    EXT_TRACK_TYPE_TASK_CREATE = 0x40,   /**< Create a task, track_id: task ID track_data:0 track_entry:0. */
    EXT_TRACK_TYPE_TASK_SCHEDULE = 0x41, /**< Schedule a task, track_id: task ID track_data: new task returned
                                         * to the PC track_entry: task body entry.
                                         */
    EXT_TRACK_TYPE_TASK_DELETE = 0x42,   /**< Delete a task, track_id: task ID track_data:0 track_entry:0. */

    EXT_TRACK_TYPE_TIMER_PROCESS = 0x51, /**< System timer callback, track_id:0 track_data: timer callback parameter
                                         * track_entry: timer callback function.
                                         */
} ext_track_type;

/**
 * @ingroup  hct_track
 * @brief  Records a track.
 *
 * @par Description:
 * Records a track.
 *
 * @attention
 * @li A task/interrupt not scheduled after the CPU usage module is initialized or the CPU usage statistics are
 * reset is excluded.
 *
 * @param track_type  [IN] Type #td_u16 Track type.
 * @param track_id    [IN] Type #td_u16 Track paramter.
 * @param track_data  [IN] Type #td_u32 Track paramter.
 * @param track_entry [IN] Type #td_u32 Track paramter.
 * @par Dependency:
 * @li soc_mdm_track.h: This file describes the track APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
td_void uapi_track_record(td_u16 track_type, td_u16 track_id, td_u32 track_data, td_u32 track_entry);

#ifdef __cplusplus
}
#endif
#endif /* __SOC_MDM_TRACK_H__ */
