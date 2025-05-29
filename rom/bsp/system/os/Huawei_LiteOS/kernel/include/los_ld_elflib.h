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

/** @defgroup dynload Dynamic loading
 * @ingroup kernel
 */

#ifndef _LOS_LD_ELFLIB_H
#define _LOS_LD_ELFLIB_H

#include "los_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


/**
 *@ingroup dynload
 *@brief Load a module.
 *
 *@par Description:
 *This API is used to load a module under a particular module file path.
 *@attention
 *<ul>
 *<li>None.</li>
 *</ul>
 *
 *@param pscElfFileName	[IN] Module file path.
 *
 *@retval NULL (VOID *)0: The module fails to be loaded.
 *@retval Non-NULL  The module is successfully loaded.
 *@par Dependency:
 *<ul><li>los_ld_elflib.h: the header file that contains the API declaration.</li></ul>
 *@see LOS_ModuleUnload
 *@since Huawei LiteOS V100R001C00
 */
extern VOID *LOS_ModuleLoad(INT8 *pscElfFileName);

/**
 *@ingroup dynload
 *@brief Unload a module.
 *
 *@par Description:
 *This API is used to unload a module with a particular module handle.
 *@attention
 *<ul>
 *<li>None.</li>
 *</ul>
 *
 *@param pstHandle	[IN] Module handle.
 *
 *@retval #LOS_NOK 1: The module fails to be unloaded.
 *@retval #LOS_OK  0: The module is successfully unloaded.
 *@par Dependency:
 *<ul><li>los_ld_elflib.h: the header file that contains the API declaration.</li></ul>
 *@see LOS_ModuleLoad
 *@since Huawei LiteOS V100R001C00
 */
extern UINT32 LOS_ModuleUnload(VOID *pstHandle);

/**
 *@ingroup dynload
 *@brief Initialize a dynamic linker.
 *
 *@par Description:
 *This API is used to initialize a dynamic linker under a particular symbol table file path.
 *@attention
 *<ul>
 *<li>Before using dynamic loading, call this API to initialize the dynamic linker.</li>
 *</ul>
 *
 *@param pscElfFileName	[IN] Path of the symbol table file.
 *
 *@retval #LOS_NOK 1: The dynamic linker fails to be initialized.
 *@retval #LOS_OK  0: The dynamic linker is successfully initialized.
 *@par Dependency:
 *<ul><li>los_ld_elflib.h: the header file that contains the API declaration.</li></ul>
 *@see LOS_LD_Destroy
 *@since Huawei LiteOS V100R001C00
 */
extern UINT32 LOS_LD_Init(INT8 *pscElfFileName);

/**
 *@ingroup dynload
 *@brief Destroy a dynamic linker.
 *
 *@par Description:
 *This API is used to destroy a dynamic linker.
 *@attention
 *<ul>
 *<li>When dynamic loading is no longer needed, call this API to destroy the dynamic linker.</li>
 *</ul>
 *
 *@param None.
 *
 *@retval None.
 *@par Dependency:
 *<ul><li>los_ld_elflib.h: the header file that contains the API declaration.</li></ul>
 *@see LOS_LD_Init
 *@since Huawei LiteOS V100R001C00
 */
extern VOID LOS_LD_Destroy(VOID);

/**
 *@ingroup dynload
 *@brief Search for a symbol address.
 *
 *@par Description:
 *This API is used to search for the address of a symbol according to a particular module handle and symbol name.
 *@attention
 *<ul>
 *<li>If the value of pHandle is NULL, Huawei LiteOS searches for symbols (including system symbols) in the global symbol table. If pHandle is set to a valid module handle, Huawei LiteOS searches for symbols in the module that comes with the module handle.</li>
 *</ul>
 *
 *@param pHandle	[IN] Module handle.
 *@param pscName	[IN] Name of the symbol to be searched for.
 *
 *@retval NULL	(VOID *)0: The symbol address is not found.
 *@retval Non-NULL	Symbol address.
 *@par Dependency:
 *<ul><li>los_ld_elflib.h: the header file that contains the API declaration.</li></ul>
 *@see LOS_LD_Init | LOS_LD_Destroy
 *@since Huawei LiteOS V100R001C00
 */
extern VOID *LOS_FindSymByName(VOID *pHandle, INT8 *pscName);

/**
 *@ingroup dynload
 *@brief Add a default path.
 *
 *@par Description:
 *This API is used to add a path to default paths.
 *@attention
 *<ul>
 *<li></li>
 *</ul>
 *
 *@param pscPath	[IN] Path to be added to default paths.
 *
 *@retval LOS_OK : The path is added successfully.
 *@retval LOS_NOK	: The path is added unsuccessfully.
 *@par Dependency:
 *<ul><li>los_ld_elflib.h: the header file that contains the API declaration.</li></ul>
 *@see LOS_LD_Init | LOS_LD_Destroy
 *@since Huawei LiteOS V100R001C00
 */
extern INT32 LOS_PathAdd(INT8 *pscPath);


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_LD_ELFLIB_H */
