#ifndef _LOS_CPUP_EXT_H
#define _LOS_CPUP_EXT_H

#include "los_cpup.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 *@ingroup los_cpup
 *@brief Obtain the CPU usage of all tasks.
 *
 *@par Description:
 *This API is used to obtain the CPU usage of all tasks according to the passed-in maximum number of threads.
 *@attention
 *<ul>
 *<li>This API can be called only after the CPU usage is initialized. Otherwise, the CPU usage fails to be obtained.</li>
 *<li>The input parameter pointer must not be NULL, and the maximum number of threads must be usable. Otherwise, the CPU usage fails to be obtained.</li>
 *<li>The input parameter pointer should point to the structure array who's size be greater than (usMaxNum * sizeof (CPUP_INFO_S)).</li>
 *</ul>
 *
 *@param usMaxNum       [IN] UINT16. The Maximum number of threads.
 *@param pstCpupInfo    [OUT]Type.   CPUP_INFO_S* Pointer to the task CPUP information structure to be obtained.
 *
 *@retval #LOS_ERRNO_CPUP_NO_INIT                  The CPU usage is not initialized.
 *@retval #LOS_ERRNO_CPUP_TASK_PTR_NULL            The input parameter pointer is NULL.
 *@retval #LOS_ERRNO_CPUP_MAXNUM_INVALID           The maximum number of threads is invalid.
 *@retval #LOS_OK                                  The CPU usage of all tasks is successfully obtained.
 *@par Dependency:
 *<ul><li>los_cpup.h: the header file that contains the API declaration.</li></ul>
 *@see
 *@since Huawei LiteOS V100R001C00
 */
extern UINT32 LOS_AllTaskCpuUsage(UINT16 usMaxNum, CPUP_INFO_S *pstCpupInfo);

/**
 *@ingroup los_cpup
 *@brief Reset the data of CPU usage.
 *
 *@par Description:
 *This API is used to reset the data of CPU usage.
 *@attention
 *<ul>
 *<li>None.</li>
 *</ul>
 *
 *@param None.
 *
 *@retval #None.
 *
 *@par Dependency:
 *<ul><li>los_cpup.h: the header file that contains the API declaration.</li></ul>
 *@see
 *@since Huawei LiteOS V100R001C00
 */
extern VOID LOS_CpupReset(VOID);





#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_CPUP_EXT_H */


