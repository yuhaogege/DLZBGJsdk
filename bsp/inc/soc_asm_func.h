/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: remap:soc_asm_func.h code
 * Author: CompanyName
 * Create: 2012-01-24
 */

#ifndef __SOC_ASM_FUNC_H__
#define __SOC_ASM_FUNC_H__
#include <soc_types.h>
#include <soc_mdm_types.h>

__attribute__((always_inline)) static inline td_u32 uapi_get_primask(void)
{
    td_u32 result = 0;
    __ASM volatile("MRS %0, primask" : "=r"(result));

    return (result);
}

__attribute__((always_inline)) static inline unsigned int uapi_clz(unsigned int x)
{
    return (unsigned int)__builtin_clz(x);
}

#endif
