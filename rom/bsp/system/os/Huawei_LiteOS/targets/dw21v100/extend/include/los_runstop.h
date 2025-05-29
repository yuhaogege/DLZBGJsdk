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

/**@defgroup los_runstop Runstop
 * @ingroup kernel
 */


#ifndef _LOS_RUNSTOP_H
#define _LOS_RUNSTOP_H

#include "los_config.h"

#if (LOSCFG_KERNEL_RUNSTOP == YES)

#include "los_typedef.h"


/**
 * @ingroup los_runstop
 * Define the type of the runstop hook function.
 *
 */

typedef void (*los_runstop_fn)(void);

/**
 *@ingroup los_runstop
 *@brief Register the runstop callback.
 *
 *@par Description:
 *This API is used to register the runstop callback.
 *@attention
 *<ul>
 *<li>None</li>
 *</ul>
 *
 *@param None
 *@retval #los_runstop_fn       The runstop callback.
 *@par Dependency:
 *<ul><li>los_runstop.h: the header file that contains the API declaration.</li></ul>
 *@see None
 *@since Huawei LiteOS V100R001C00
 */
extern void LOS_RunStopCallBackRegister(los_runstop_fn cb);

/**
 *@ingroup los_runstop
 *@brief Add the light sleep vote.
 *
 *@par Description:
 *This API is used to add vote to stop system enter in light sleep.
 *@attention
 *<ul>
 *<li>None</li>
 *</ul>
 *
 *@param None
 *@retval None
 *@par Dependency:
 *<ul><li>los_runstop.h: the header file that contains the API declaration.</li></ul>
 *@see None
 *@since Huawei LiteOS V100R001C00
 */
extern void LOS_AddLightSleepVote(void);

/**
 *@ingroup los_runstop
 *@brief Remove the light sleep vote.
 *
 *@par Description:
 *This API is used to remove the vote to allow the system enter in light sleep.
 *@attention
 *<ul>
 *<li>None</li>
 *</ul>
 *
 *@param None
 *@retval None
 *@par Dependency:
 *<ul><li>los_runstop.h: the header file that contains the API declaration.</li></ul>
 *@see None
 *@since Huawei LiteOS V100R001C00
 */
extern void LOS_RemoveLightSleepVote(void);

/**
 *@ingroup los_runstop
 *@brief Add the deep sleep vote.
 *
 *@par Description:
 *This API is used to add vote to stop system enter in deep sleep.
 *@attention
 *<ul>
 *<li>None</li>
 *</ul>
 *
 *@param None
 *@retval None
 *@par Dependency:
 *<ul><li>los_runstop.h: the header file that contains the API declaration.</li></ul>
 *@see None
 *@since Huawei LiteOS V100R001C00
 */
extern void LOS_AddDeepSleepVote(void);

/**
 *@ingroup los_runstop
 *@brief Remove the deep sleep vote.
 *
 *@par Description:
 *This API is used to remove the vote to allow the system enter in deep sleep.
 *@attention
 *<ul>
 *<li>None</li>
 *</ul>
 *
 *@param None
 *@retval None
 *@par Dependency:
 *<ul><li>los_runstop.h: the header file that contains the API declaration.</li></ul>
 *@see None
 *@since Huawei LiteOS V100R001C00
 */
extern void LOS_RemoveDeepSleepVote(void);
#endif
#endif
