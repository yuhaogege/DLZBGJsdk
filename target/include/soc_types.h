/*
 * Copyright (c) CompanyNameMagicTag 2019-2019. All rights reserved.
 * Description: Data type description
 * Author: CompanyName
 * Create: 2019-10-31
 */
#ifndef SOC_TYPES_H
#define SOC_TYPES_H

#include <soc_product_config.h>
#include <soc_types_rom.h>
/***************************************************************************** */
/* Assertion, which is used only for internal debugging */
/* */
#undef EXT_SYS_DEBUG
#if defined(EXT_SYS_DEBUG)
#define ext_assert(expr)                               \
    {                                               \
        if (!(expr)) {                                       \
            UAPI_SYS_FatalVerifyPrv((td_pcstr) #expr, (td_u32)0, (td_u32)ext_sys_get_lr(), \
            (td_pcstr)0, (td_u32)__LINE__, (td_pcstr) __FUNCTION__); \
        }                                                   \
    }
#else
#define ext_assert(expr) ((td_void)0)
#define ASSERT(expr) ((td_void)0)
#endif
/***************************************************************************** */
/* Read/Write functions of the register */
#define REG_FORMAT_UAPI_U32(reg_var_name)      (*(td_u32_reg *)(&(reg_var_name)))
#define com_bbp_reg_wr(bbp_address, content) (*((td_u32_reg *)(bbp_address)) = (*((td_u32_reg *)(content))))
#define com_bbp_reg_rd(bbp_address)          (*((td_u32_reg *)(bbp_address)))

#include <soc_errno.h>
#include <soc_config.h>

#define UINT_MAX        0xffffffffU     /* max value for an unsigned int */
#define INT_MAX         0x7fffffff      /* max value for an int */
#define INT_MIN         (-0x7fffffff-1) /* min value for an int */
/* check uint multiplicine overflow, and give result
   ret TD_TRUE  1 : ok
       TD_FALSE 0 : is overflow
 */
inline static td_bool uint_2_multiply(td_u32 multiplier, td_u32 var, td_u32 *res)
{
    td_u64 temp = (td_u64)multiplier * var;
    if (res != TD_NULL) {
        *res = (td_u32)temp;
    }
    return (temp < UINT_MAX);
}

inline static td_bool int_2_multiply(td_s32 multiplier, td_s32 var, td_s32 *res)
{
    td_s64 temp = (td_s64)multiplier * var;
    if (res != TD_NULL) {
        *res = (td_s32)temp;
    }
    return (temp > INT_MIN && temp < INT_MAX);
}

#endif  /* SOC_TYPES_H */

