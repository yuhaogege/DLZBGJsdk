/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 */
#ifndef SOC_TYPES_ROM_H
#define SOC_TYPES_ROM_H

#if defined(HAVE_PCLINT_CHECK)
#define __isr
#endif

#ifndef MAC_V200_LLT
#ifndef __isr
#define __isr __attribute__((section(".isr.text")))
#endif

#ifndef __init
#define __init __attribute__((section(".init.text")))
#endif

#ifndef __exit
#define __exit __attribute__((section(".exit.text")))
#endif

#ifndef __hot
#define __hot __attribute__((section(".hot.text")))
#endif

#ifndef __text_ram
#define __text_ram __attribute__((section(".text.ram")))
#endif

#ifndef BSP_ROM_RODATA_SECTION
#define BSP_ROM_RODATA_SECTION __attribute__((section(".bsp.rom.rodata")))
#endif

#ifndef __attribute_format
#define __attribute_format(x, y) __attribute__((format(printf, x, y)))
#endif
#else
#define __isr
#define __init
#define __exit
#define __hot
#define __text_ram
#define BSP_ROM_RODATA_SECTION
#define __attribute_format(x, y)
#endif

// Definition of basic data types
typedef unsigned int td_u32;
typedef int td_s32;
typedef unsigned short td_u16;
typedef signed short td_s16;
typedef unsigned char td_u8;
typedef signed char td_s8;
typedef void td_void;
typedef char td_char;
typedef unsigned char td_uchar;
typedef td_u8 td_bool;
typedef void *td_pvoid;
typedef td_u8 td_byte;
typedef td_pvoid td_handle;
typedef td_byte *td_pbyte;
typedef float td_float;
typedef double td_double;
typedef volatile td_u32 td_u32_reg;
typedef td_pvoid td_func_ptr;
typedef td_u32 td_func;
typedef unsigned int td_size_t;
typedef int td_ssize_t;
typedef int td_offset_t;
typedef unsigned int uintptr_t;
typedef int intptr_t;

/* Used in DBK */
#define DBK_ENUM enum
#define EXT_U8A td_u8
#define EXT_U16A td_u16
#define EXT_CHARTA td_char

#undef NULL
#ifndef NULL
#define NULL 0
#endif
#define EXT_CONST const
#define EXT_REG register
#define EXT_U32_MAX 0xFFFFFFFF
#define EXT_U16_MAX 0xFFFF
#define EXT_U16_BITS 16

typedef EXT_CONST td_char *td_pcstr;
typedef unsigned long long td_u64;
typedef long long td_s64;
#define EXT_U32_BITS 32
#define EXT_S32_BITS 32
#define EXT_S32_MAX (~(~0 << (EXT_S32_BITS - 1)))
#define EXT_U8_BITS 8
#define EXT_U8_MAX 0xff
#define EXT_U4_BITS 4
#define EXT_U4_MAX 0x0f

#define EXT_PUBLIC extern

#if !defined(PRODUCT_CFG_FEATURE_UT)
#define EXT_PRV static
#else
#define EXT_PRV
#endif
#define STATIC static
#define EXT_CONST const
#define EXT_INLINE __inline
#define EXT_API
#define EXT_PRVL EXT_PRV EXT_INLINE

#define ext_section(name_string) __attribute__((section(name_string)))
#define EXT_PACKED __attribute__((packed))
#define EXT_ALIGNED4 __attribute__((aligned(4)))

#if defined(SAL_HAVE_NO_EXTERN_DEFINED)
#define EXT_EXTERN
#define EXT_EXTERN_C
#else
#if defined(PRODUCT_CFG_OS_WIN)
#define EXT_EXTERN extern EXT_API
#define EXT_EXTERN_C EXT_EXTERN
#define EXT_EAPI extern EXT_API
#else
#define EXT_EXTERN extern
#define EXT_EAPI
#define EXT_EXTERN_C
#endif
#endif

#ifdef __cplusplus
#define EXT_CPP_START extern "C" {
#define EXT_CPP_END }
#else
#define EXT_CPP_START
#define EXT_CPP_END
#endif

#if defined(EXT_HAVE_CROSS_COMPILER_ARM_ARMCC)
#define EXT_NOP __asm { nop }
#define ext_dbg_break() __asm { swi 0x14DEAD }
#elif defined(EXT_HAVE_CROSS_COMPILER_DIAB)
#define EXT_NOP
#define ext_dbg_break()
#else
#define EXT_NOP
#ifdef PRODUCT_CFG_OS_WIN
#define ext_dbg_break() _asm { int 3 }
#else
#define ext_dbg_break()
#endif
#endif
#ifndef EXT_START_HEADER
#define EXT_START_HEADER EXT_CPP_START
#endif
#ifndef EXT_END_HEADER
#define EXT_END_HEADER EXT_CPP_END
#endif

#undef EXT_OUT
#undef EXT_IN
#undef EXT_INOUT
#define EXT_OUT
#define EXT_IN
#define EXT_INOUT

#define TD_FALSE 0
#define TD_TRUE 1

#ifdef __cplusplus
#define TD_NULL 0
#else
#define TD_NULL ((void *)0)
#endif

#define ext_array_count(x) ((sizeof(x) / sizeof((x)[0])))

#if !defined(ext_unref_param) && !defined(EXT_HAVE_CROSS_COMPILER_DIAB)
#define ext_unref_param(P) (P) = (P)
#else
#define ext_unref_param(P)
#endif

#if defined(PRODUCT_CFG_PLATFORM_DW21)
#if defined(EXT_HAVE_CROSS_COMPILER_ARM_ARMCC)
#define EXT_VOLATILE volatile
#define ext_sys_get_lr() __return_address()
#elif defined(HAVE_PCLINT_CHECK)
#define EXT_VOLATILE
#define ext_sys_get_lr() 0
#elif defined(EXT_HAVE_CROSS_COMPILER_DIAB)
#define EXT_VOLATILE volatile
#define ext_sys_get_lr() 0
#else
#define EXT_VOLATILE __volatile__
#define ext_sys_get_lr() 0
#endif
#else
#define EXT_VOLATILE
#define ext_sys_get_lr() 0
#endif

#define ext_aligin_u32_size(x) (((x) & (~3)) + 4) // Construct 4-byte alignment.
#define ext_is_align_u32(x) (!((x)&3))          // Check whether data is 4-byte aligned.
#define ext_is_unalign_u32(x) ((x)&3)           // Check whether data is 4-byte aligned.
#if defined(HAVE_PCLINT_CHECK)
#define ext_field_offset(s, m) (0) // Structure member offset
#else
#define ext_field_offset(s, m) ((td_u32) & (((s *)0)->m)) // Structure member offset
#endif

#define EXT_CHAR_CR '\r' // 0x0D
#define EXT_CHAR_LF '\n' // 0x0A

#define uapi_make_u16(a, b) ((td_u16)(((td_u8)(a)) | ((td_u16)((td_u8)(b))) << 8))
#define ext_make_u32(a, b) ((td_u32)(((td_u16)(a)) | ((td_u32)((td_u16)(b))) << 16))
#define ext_make_u64(a, b) ((td_u64)(((td_u32)(a)) | ((td_u64)((td_u32)(b))) << 32))

#define ext_swap16(x) ((((x)&0xff00) >> 8) | (((x)&0x00ff) << 8))
#define ext_swap32(x) \
    ((((x)&0xff000000) >> 24) | (((x)&0x00ff0000) >> 8) | (((x)&0x0000ff00) << 8) | (((x)&0x000000ff) << 24))

#define ext_hiu32(l) ((td_u32)(((td_u64)(l) >> 32) & 0xFFFFFFFF))
#define ext_lou32(l) ((td_u32)(l))

#define ext_hiu16(l) ((td_u16)(((td_u32)(l) >> 16) & 0xFFFF))
#define ext_lou16(l) ((td_u16)(l))
#define ext_hiu8(l) ((td_u8)(((td_u16)(l) >> 8) & 0xFF))
#define ext_lou8(l) ((td_u8)(l))

#define ext_max(a, b) (((a) > (b)) ? (a) : (b))
#define uapi_min(a, b) (((a) < (b)) ? (a) : (b))
#undef MIN_T
#define MIN_T uapi_min

#define uapi_make_identifier(a, b, c, d) ext_make_u32(uapi_make_u16(a, b), uapi_make_u16(c, d))
#define ext_make_ver16(spc, b) ((td_u16)(((td_u8)((spc)&0x0F)) | ((td_u16)((td_u8)(b))) << 12))

#define ext_set_bit_i(val, n) ((val) |= (1 << (n)))
#define ext_clr_bit_i(val, n) ((val) &= ~(1 << (n)))
#define ext_is_bit_set_i(val, n) ((val) & (1 << (n)))
#define ext_is_bit_clr_i(val, n) (~((val) & (1 << (n))))
#define ext_switch_bit_i(val, n) ((val) ^= (1 << (n)))
#define EXT_GET_BIT_I(val, n) (((val) >> (n)) & 1)

#define ext_u8_bit_val(b7, b6, b5, b4, b3, b2, b1, b0) \
    (((b7) << 7) | ((b6) << 6) | ((b5) << 5) | ((b4) << 4) | ((b3) << 3) | ((b2) << 2) | ((b1) << 1) | ((b0) << 0))
#define ext_u16_bit_val(b12, b11, b10, b9, b8, b7, b6, b5, b4, b3, b2, b1, b0)                                      \
    (td_u16)(((b12) << 12) | ((b11) << 11) | ((b10) << 10) | ((b9) << 9) | ((b8) << 8) | \
        ((b7) << 7) | ((b6) << 6) | ((b5) << 5) | \
        ((b4) << 4) | ((b3) << 3) | ((b2) << 2) | ((b1) << 1) | ((b0) << 0))

#if defined(PRODUCT_CFG_DBK) && defined(EXT_HAVE_NOTIVE_COMPILER_VC)
#define __FUNCTION__ "NA"
#endif
// *****************************************************************************
// *****************************************************************************
#define ext_set_u32_ptr_val(ptr, offset, val) *((td_u32 *)(((td_u8 *)(ptr)) + (offset))) = (val)
#define ext_get_u32_ptr_val(ptr, offset) *((td_u32 *)(((td_u8 *)(ptr)) + (offset)))
// *****************************************************************************
// *****************************************************************************
#define EXT_SIZE_1K (1024)
#define EXT_SIZE_1M (1024 * 1024)
// *****************************************************************************
/* Register access APIs */
#define UAPI_REG_WRITE32(addr, val) (*(volatile unsigned int *)(uintptr_t)(addr) = (val))
#define ext_reg_write16(addr, val) (*(volatile unsigned short *)(uintptr_t)(addr) = (val))
#define UAPI_REG_READ32(addr, val) ((val) = *(volatile unsigned int *)(uintptr_t)(addr))
#define UAPI_REG_READ_VAL32(addr) (*(volatile unsigned *)(uintptr_t)(addr))

#define reg_write32(addr, val) (*(volatile unsigned int *)(uintptr_t)(addr) = (val))
#define reg_read32(addr, val) ((val) = *(volatile unsigned int *)(uintptr_t)(addr))
#define reg_read_val(addr) (*(volatile unsigned *)(uintptr_t)(addr))

#define ext_reg_clrbit(addr, g_pos) ((UAPI_REG_READ_VAL32(addr)) &= ~((unsigned int)(1) << (g_pos)))
#define ext_reg_setbit(addr, g_pos) ((UAPI_REG_READ_VAL32(addr)) |= ((unsigned int)(1) << (g_pos)))
#define ext_reg_setbits(addr, g_pos, bits, val)                                                                 \
    (UAPI_REG_READ_VAL32(addr) = (UAPI_REG_READ_VAL32(addr) & (~((((unsigned int)1 << (bits)) - 1) << (g_pos)))) | \
        ((unsigned int)((val) & (((unsigned int)1 << (bits)) - 1)) << (g_pos)))
#define ext_reg_getbits(addr, g_pos, bits) ((UAPI_REG_READ_VAL32(addr) >> (g_pos)) & (((UINT32)1 << (bits)) - 1))

/*
    Feature description:
    Obtains the offset of a member in the structure.
 */
#define ext_offset_of_member(type, member) ((td_u32) & ((type *)0)->member)

#ifdef HAVE_PCLINT_CHECK
#define UAPI_LIKELY(x) (x)
#define UAPI_UNLIKELY(x) (x)
#else
#define UAPI_LIKELY(x) __builtin_expect(!!(x), 1)
#define UAPI_UNLIKELY(x) __builtin_expect(!!(x), 0)
#endif
#define EXT_ALWAYS_STAIC_INLINE __attribute__((always_inline)) static inline

// *****************************************************************************
#include <soc_errno_rom.h>
#include <soc_config_rom.h>
#endif // SOC_TYPES__ROM_H
