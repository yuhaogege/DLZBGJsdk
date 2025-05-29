#ifndef BOOT_SOC_TYPES_H
#define BOOT_SOC_TYPES_H

/* Basic Data Type Definition */
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
typedef td_byte *td_pbyte;
typedef float td_float;
typedef double td_double;
typedef volatile td_u32 td_u32_reg;
typedef td_u32 td_func;
typedef const char *td_pcstr;

typedef unsigned long long uint64_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
typedef unsigned long ulong;
typedef int ptrdiff_t;
typedef unsigned int uintptr_t;

#define EXT_PRV    static
#define EXT_PRVL   static inline
#define EXT_EXTERN extern
#define EXT_CONST  const

#define EXT_OUT
#define EXT_IN
#define EXT_INOUT

#define TD_FALSE 0
#define TD_TRUE  1

#ifdef __cplusplus
#define TD_NULL 0
#else
#define TD_NULL ((void*)0)
#endif

#ifndef NULL
#define NULL (void*)0
#endif

#define SZ_1KB 1024
#define SZ_1MB (SZ_1KB * SZ_1KB)

#define ext_array_count(x) (sizeof(x) / sizeof((x)[0]))

#define EXT_ALIGN_4(x)        ((unsigned int)(x + 0x3) & (~0x3)) /* Construct 4-byte address alignment */
#define ext_is_align_u32(x)   (!((x)&3))                         /* Check whether 4-byte alignment is used */
#define ext_is_unalign_u32(x) ((x)&3)                            /* Check whether 4-byte alignment is used */
#if defined(HAVE_PCLINT_CHECK)
#define ext_field_offset(s, m) (0)  /* structure member offset */
#else
#define ext_field_offset(s, m) ((td_u32) & (((s*)0)->m))  /* structure member offset */
#endif
#define EXT_CHAR_CR '\r' /* 0x0D */
#define EXT_CHAR_LF '\n' /* 0x0A */

#define BOOT_SUCCESS     (0)
#define BOOT_FAILURE     (-1)
#define uapi_make_u16(a, b) ((td_u16)(((td_u8)(a)) | ((td_u16)((td_u8)(b))) << 8))
#define ext_make_u32(a, b) ((td_u32)(((td_u16)(a)) | ((td_u32)((td_u16)(b))) << 16))
#define ext_hiu16(l)      ((td_u16)(((td_u32)(l) >> 16) & 0xFFFF))
#define ext_lou16(l)      ((td_u16)(l))
#define ext_hiu8(l)       ((td_u8)(((td_u16)(l) >> 8) & 0xFF))
#define ext_lou8(l)       ((td_u8)(l))

#define ext_max(a, b) (((a) > (b)) ? (a) : (b))
#define uapi_min(a, b) (((a) < (b)) ? (a) : (b))

#define ext_set_bit_i(val, n)                                                  ((val) |= (1 << n))
#define ext_clr_bit_i(val, n)                                                  ((val) &= ~(1 << n))
#define ext_is_bit_set_i(val, n)                                               ((val) & (1 << n))
#define ext_is_bit_clr_i(val, n)                                               (~((val) & (1 << n)))
#define ext_switch_bit_i(val, n)                                               ((val) ^= (1 << n))
#define EXT_GET_BIT_I(val, n)                                                  (((val) >> n) & 1)
#define ext_u8_bit_val(b7, b6, b5, b4, b3, b2, b1, b0)                         (b7 << 7) | (b6 << 6) | (b5 << 5) | (b4 << 4) | (b3 << 3) | (b2 << 2) | (b1 << 1) | (b0 << 0)
#define ext_u16_bit_val(b12, b11, b10, b9, b8, b7, b6, b5, b4, b3, b2, b1, b0) (td_u16)((b12 << 12) | (b11 << 11) | (b10 << 10) | (b9 << 9) | (b8 << 8) | (b7 << 7) | (b6 << 6) | (b5 << 5) | (b4 << 4) | (b3 << 3) | (b2 << 2) | (b1 << 1) | (b0 << 0))

#define ALIGNTYPE_1BYTE      1
#define ALIGNTYPE_2BYTE      2
#define ALIGNTYPE_4BYTE      4
#define ALIGNTYPE_8BYTE      8
#define ALIGNTYPE_64BYTE     64
#define ALIGNTYPE_32BYTE     32
#define ALIGNTYPE_4K         4096
#define align_next(val, a)   ((((val) + ((a)-1)) & (~((a)-1))))
#define align_length(val, a) align_next(val, a)
#define EXT_ALL_F_32          0xFFFFFFFF
#define EXT_ALL_F_16          0xFFFF

#define boot_panic(x)                     \
    do {                                  \
        for (;;) {                        \
            boot_msg1("BOOT_PANIC=", x); \
        }                                 \
    } while (0)

#ifndef __rom_addition
#define __rom_addition __attribute__((section(".rom_addition.text")))
#endif

#define UINT_MAX        0xffffffffU     /* max value for an unsigned int */
inline static td_bool uint_2_multiply(td_u32 multiplier, td_u32 var, td_u32 *res)
{
    uint64_t temp = (uint64_t)multiplier * var;
    if (res != TD_NULL) {
        *res = (td_u32)temp;
    }
    return (temp < UINT_MAX);
}

#endif
