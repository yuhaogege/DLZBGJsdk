/*----------------------------------------------------------------------------
 * Copyright (c) <2013-2015>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

/**@defgroup los_err Error handling
 * @ingroup kernel
 */

#ifndef _LOS_TRACK_H
#define _LOS_TRACK_H

#include "los_typedef.h"
#include "los_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup los_track
 * Track  mask
 */
typedef enum
{
    TRACK_ISR_FLAG   =   0x10,
    TRACK_IRQ_FLAG   =   0x20,
    TRACK_TASK_FLAG  =   0x40,
    TRACK_WORKQ_FLAG =   0x80,
    TRACK_SWTMR_FLAG =   0x100,
    TRACK_MUX_FLAG   =   0x200,
    TRACK_SEM_FLAG   =   0x400,
    TRACK_QUE_FLAG   =   0x800,
    TRACK_EVENT_FLAG =   0x1000
} LOS_TRACK_MASK;

/**
 * @ingroup los_track
 * Track type
 */
typedef enum
{
    TRAP_INT        = TRACK_ISR_FLAG,

    ENABLE_IRQ      = TRACK_IRQ_FLAG | 0,
    DISABLE_IRQ     = TRACK_IRQ_FLAG | 1,

    CREATE_TSK      = TRACK_TASK_FLAG | 0,
    SCHEDULE_TSK    = TRACK_TASK_FLAG | 1,
    DELETE_TSK      = TRACK_TASK_FLAG | 2,

    ENQUEUE_WORKQ   = TRACK_WORKQ_FLAG | 0,
    SCHEDULE_WORKQ  = TRACK_WORKQ_FLAG | 1,
    DEQUEUE_WORKQ   = TRACK_WORKQ_FLAG | 2,

    CREATE_SWTMR    = TRACK_SWTMR_FLAG | 0,
    START_SWTMR     = TRACK_SWTMR_FLAG | 1,
    MODIFY_SWTMR    = TRACK_SWTMR_FLAG | 2,
    SCHDING_SWTMR   = TRACK_SWTMR_FLAG | 3,
    SCHDED_SWTMR    = TRACK_SWTMR_FLAG | 4,
    STOP_SWTMR      = TRACK_SWTMR_FLAG | 5,
    DEL_SWTMR       = TRACK_SWTMR_FLAG | 6,
    DROP_SWTMR      = TRACK_SWTMR_FLAG | 7,

    MUX_CREATE      = TRACK_MUX_FLAG | 0,
    MUX_PEND        = TRACK_MUX_FLAG | 1,
    MUX_POST        = TRACK_MUX_FLAG | 2,
    MUX_DEL         = TRACK_MUX_FLAG | 3,

    SEM_CREATE      = TRACK_SEM_FLAG | 0,
    SEM_PEND        = TRACK_SEM_FLAG | 1,
    SEM_POST        = TRACK_SEM_FLAG | 2,
    SEM_DEL         = TRACK_SEM_FLAG | 3,

    QUE_CREATE      = TRACK_QUE_FLAG | 0,
    QUE_WRITE       = TRACK_QUE_FLAG | 1,
    QUE_READ        = TRACK_QUE_FLAG | 2,
    QUE_DEL         = TRACK_QUE_FLAG | 3,

    EVENT_CREATE    = TRACK_EVENT_FLAG | 0,
    EVENT_WRITE     = TRACK_EVENT_FLAG | 1,
    EVENT_READ      = TRACK_EVENT_FLAG | 2,
    EVENT_CLEAR     = TRACK_EVENT_FLAG | 3,
    EVENT_DEL       = TRACK_EVENT_FLAG | 4,
} LOS_TRACK_TYPE;

/**
 * @ingroup los_track
 * Track track item structure
 */
typedef struct tagTrackItem
{
    UINT16 usTrackType;  /**< Track type     */
    UINT16 usData0;      /**< Track ID     */
    UINT32 uwData1;      /**< Userdata     */
    UINT32 uwEntry;      /**< Hook function Entry     */
    UINT32 uwCurTime;    /**< The low 32 bit of the current time    */
} TRACK_ITEM_S;


/**
 * @ingroup los_track
 * Track manager structure
 */
typedef struct tagTrackMgr
{
    UINT16 usCurIndex;              /**< The current record index     */
    UINT16 usMaxRecordCount;        /**< The max num of track items     */
    TRACK_ITEM_S *pstTrackItems;    /**< Pointer to the track items     */
} TRACK_MGR_S;

typedef UINT32 (*USER_CUR_US_FUNC)(UINT32 * time);

/**
 * @ingroup los_track
 * Define the type of the task track hook function.
 *
 */
typedef VOID (*TRACK_PROC_HOOK)(UINT16 usTrackType, UINT16 uwTrackId, UINT32 uwUserData, UINT32 uwEntry);
extern TRACK_PROC_HOOK g_pTrackHook;


/**
* @ingroup los_track
* Track error code: The maximum number of track items is too large.
*
* Value: 0x02001400
*
* Solution: Decrease the maximum number of track items.
*/
#define LOS_ERRNO_TRACK_TOO_LARGE_COUNT       LOS_ERRNO_OS_ERROR(LOS_MOD_TRACK, 0x00)


/**
* @ingroup los_track
* Track error code: The maximum number of track items is  0.
*
* Value: 0x02001401
*
* Solution: Pass in correct maximum number of track items.
*/
#define LOS_ERRNO_TRACK_PARA_ISZERO           LOS_ERRNO_OS_ERROR(LOS_MOD_TRACK, 0x01)


/**
* @ingroup los_track
* Track error code: The request for memory fails.
*
* Value: 0x02001402
*
* Solution: Decrease the maximum number of track items.
*/
#define LOS_ERRNO_TRACK_NO_MEMORY             LOS_ERRNO_OS_ERROR(LOS_MOD_TRACK, 0x02)


/**
* @ingroup los_track
* Track error code: The parameter index is invalid.
*
* Value: 0x02001403
*
* Solution: Check whether the parameter is applicable for the current operation.
*/
#define LOS_ERRNO_TRACK_INVALID_PARAM          LOS_ERRNO_OS_ERROR(LOS_MOD_TRACK, 0x03)


/**
* @ingroup los_track
* Track error code: The pointer to an input parameter is NULL.
*
* Value: 0x02001404
*
* Solution: Check whether the pointer to the input parameter is usable.
*/
#define LOS_ERRNO_TRACK_PTR_NULL               LOS_ERRNO_OS_ERROR(LOS_MOD_TRACK, 0x04)


/**
* @ingroup los_cpup
* Track  error code: The track is not initialized.
*
* Value: 0x02001405
*
* Solution: Check whether the track is initialized.
*/
#define LOS_ERRNO_TRACK_NO_INIT                LOS_ERRNO_OS_ERROR(LOS_MOD_TRACK, 0x05)


/**
* @ingroup los_cpup
* Track  error code: Reintialize the track memory.
*
* Value: 0x02001406
*
* Solution:Don't need to intialize the track.
*/
#define LOS_ERRNO_TRACK_RE_INIT                LOS_ERRNO_OS_ERROR(LOS_MOD_TRACK, 0x06)




/**
 * @ingroup los_track
 *  Mask of information to track
 */
extern UINT16  g_track_mask;






/**
 *@ingroup los_track
 *@brief Initializes memory of track manager and track items
 *
 *@par Description:
 *This API is used to initialize memory of track manager and track items
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 *@param usMaxRecordCount        [IN]  #UINT16    The  max track count of track items.
 *
 *@retval LOS_ERRNO_TRACK_RE_INIT                     Reintialize the track memory.
 *@retval LOS_ERRNO_TRACK_TOO_LARGE_COUNT    The record count is too large.
 *@retval LOS_ERRNO_TRACK_NO_MEMORY              The request for memory fails.
 *@par Dependency:
 *<ul><li>los_track.h: the header file that contains the API declaration.<ul><li>
 *@see None.
 *@since Huawei LiteOS V200R001C00
 */
extern UINT32 LOS_TrackInit(UINT16 usMaxRecordCount);



/**
 *@ingroup los_track
 *@brief Defalt track hookfunction
 *
 *@par Description:
 *This API is used to record the track type,track id,userdata and function entry in the track items.
 *
 * @attention
 * <ul>
 * <li>g_func_get_cur_us funciton is used to get the low 32 bit of current time of each item.</li>
 * <li>If the user wants to use own time count funtion, g_func_get_cur_us funciton must be registered first. </li>
 * <li>If g_func_get_cur_us function is null, osTrackhook will transform current tickcount into milliseconds and record the low 32 bit of it.</li>
 * </ul>
 *
 *@param usTrackType        [IN]  #UINT16                   The  type of track.
 *@param uwTrackId           [IN]  #UINT16                   The   id  of track.
 *@param uwUserData         [IN]  #UINT32                   The  user data .
 *@param uwEntry              [IN]  #UINT32                   The  entry of function .
 *
 *@retval None.
 *@par Dependency:
 *<ul><li>los_track.h: the header file that contains the API declaration.<ul><li>
 *@see None.
 *@since Huawei LiteOS V200R001C00
 */
extern VOID osTrackhook(UINT16 usTrackType, UINT16 uwTrackId, UINT32 uwUserData, UINT32 uwEntry);




/**
 *@ingroup los_track
 *@brief Start recording the track information.
 *
 *@par Description:
 *This API is used to Start recording the track information.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 *@param usTrackMask  [IN]  #UINT16  The  mask of information needed to track.
 *
 *@retval   #LOS_ERRNO_TRACK_NO_INIT  The track is not initialized.
 *@retval   #LOS_OK                                The track start successfully.
 *@par Dependency:
 *<ul><li>los_track.h: the header file that contains the API declaration.<ul><li>
 *@see None.
 *@since Huawei LiteOS V200R001C00
 */
extern UINT32 LOS_TrackStart(UINT16 usTrackMask);




/**
 *@ingroup los_track
 *@brief Stop recording the track information.
 *
 *@par Description:
 *This API is used to Stop recording the track information.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 *@param None.
 *
 *@retval  #UINT16 .  Save the mask of information needed to track.
 *@par Dependency:
 *<ul><li>los_track.h: the header file that contains the API declaration.<ul><li>
 *@see None.
 *@since Huawei LiteOS V200R001C00
 */
extern UINT16 LOS_TrackStop(VOID);


/**
 *@ingroup los_track
 *@brief Get the track manger information.
 *
 *@par Description:
 *This API is used to get the track manger information.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 *@param pInfo    [OUT] #TRACK_MGR_S*  Pointer to the track manager information structure to be obtained.
 *
 *@retval type #LOS_ERRNO_TRACK_NO_INIT     The track is not initialized.
 *@retval type #LOS_ERRNO_TRACK_PTR_NULL   Null parameter.
 *@retval type #LOS_OK                          The track manager information structure is successfully obtained.
 *@par Dependency:
 *<ul><li>los_track.h: the header file that contains the API declaration.<ul><li>
 *@see None.
 *@since Huawei LiteOS V200R001C00
 */
extern UINT32 LOS_TrackInfoGet(TRACK_MGR_S *pInfo);



/**
 *@ingroup los_track
 *@brief Get the track item information structure by index.
 *
 *@par Description:
 *This API is used to get the track item information structure by index.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 *@param usIndex            [IN] #UINT16               The index of track items.
 *@param pItemInfo        [OUT] #TRACK_ITEM_S* Pointer to the track item information structure to be obtained.
 *@retval type #LOS_ERRNO_TRACK_NO_INIT              The track is not initialized.
 *@retval type #LOS_ERRNO_TRACK_PTR_NULL            Null parameter.
 *@retval type #LOS_ERRNO_TRACK_INVALID_PARAM   The index is lager than the max count of track item.
 *@retval type #LOS_OK                                   The track item information structure is successfully obtained.
 *@par Dependency:
 *<ul><li>los_track.h: the header file that contains the API declaration.<ul><li>
 *@see None.
 *@since Huawei LiteOS V200R001C00
 */
extern UINT32 LOS_TrackItemGetById(UINT16 usIndex, TRACK_ITEM_S *pItemInfo);


/**
 *@ingroup los_track
 *@brief Delete the track.
 *
 *@par Description:
 *This API is used to delete the track.
 *@attention
 *<ul>
 *<li>None.</li>
 *</ul>
 *
 *@param  None.
 *
 *@retval #LOS_ERRNO_TRACK_NO_INIT         The track is not initialized.
 *@retval #LOS_OK                                       The track is successfully deleted.
 *@par Dependency:
 *<ul><li>los_track.h: the header file that contains the API declaration.</li></ul>
 *@see LOS_TrackInit
 *@since Huawei LiteOS V100R001C00
 */
extern UINT32 LOS_TrackDelete(VOID);
extern UINT32 LOS_CurUsRegHook(USER_CUR_US_FUNC pfnHook);
extern UINT32 LOS_TrackRegHook(TRACK_PROC_HOOK pfnHook);

/**
 * @ingroup los_config
 * support user-defined cpu occupancy rate statistics */
#if (LOSCFG_BASE_MISC_TRACK == YES)

#define LOSCFG_WORKQ_TRACK_ENABLE           YES
#define LOSCFG_SWTMR_TRACK_ENABLE           YES
#define LOSCFG_TASK_TRACK_ENABLE            YES
#define LOSCFG_ISR_TRACK_ENABLE             YES
#define LOSCFG_SEM_TRACK_ENABLE             YES
#define LOSCFG_MUX_TRACK_ENABLE             YES
#define LOSCFG_QUEUE_TRACK_ENABLE           YES
#define LOSCFG_EVENT_TRACK_ENABLE           YES
#define LOSCFG_IRQ_TRACK_ENABLE             YES

#define LOSCFG_CPUP_ENABLE                  YES                           
#endif



/****************************************************************/
/****************************************************************/

/****************************************************************/


/***************************************************************/
/****************************************************************/

/****************************************************************/

/****************************************************************/
/****************************************************************/
/****************************************************************/
/****************************************************************/


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_ERR_H */
