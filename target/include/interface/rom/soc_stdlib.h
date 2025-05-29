/**
 * Copyright (c) CompanyNameMagicTag 2017-2020. All rights reserved.
 * defgroup: hct_secure_c Secure C library APIs
 * ingroup: hct_secure
 * Create: 2017-04-14
 * A parameter is added to the security C APIs based on the standard C interface, that is, the upper limit of
 * the write operation address space to prevent out-of-bounds write.
 */

#ifndef __SOC_STDLIB_H__
#define __SOC_STDLIB_H__

#include <soc_types_rom.h>
#include <soc_mdm_types_rom.h>
#if defined(UT_SWITCH)
#include <time.h>
#include <stdarg.h>
#endif
EXT_START_HEADER

typedef int errno_t;
typedef unsigned int size_t;

#define EOK (0)

#if defined(UT_SWITCH)
#define CONST_TYPE const
#else
typedef long time_t;
struct tm {
    int tm_sec;   /**< The number of seconds after the minute, normally in the range 0 to 59,
                   * but can be up to 60 to allow for leap seconds */
    int tm_min;   /**< The number of minutes after the hour, in the range 0 to 59 */
    int tm_hour;  /**< The number of hours past midnight, in the range 0 to 23 */
    int tm_mday;  /**< The day of the month, in the range 1 to 31 */
    int tm_mon;   /**< The number of months since January, in the range 0 to 11 */
    int tm_year;  /**< The number of years since 1900. */
    int tm_wday;  /**< The number of days since Sunday, in the range 0 to 6 */
    int tm_yday;  /**< The number of days since January 1, in the range 0 to 365 */
    int tm_isdst; /**< A flag that indicates whether daylight saving time is in effect at the time described.
                   * The value is positive if daylight saving time is in effect, zero if it is not,
                   * and negative if the information is not available */
};

#ifndef __va_list__
#define __va_list__
typedef __builtin_va_list __gnuc_va_list;
typedef __gnuc_va_list va_list;


#endif

#define CONST_TYPE
#endif

/**
 * @ingroup  hct_secure_c
 * @brief  Copies the source string to the destination buffer.
 *
 * @par Description:
 * Copies the source string to the destination buffer.
 * @attention None
 *
 * @param dest [OUT] Type #char * Destination buffer
 * @param dest_max [IN] Type #size_t Size of the destination buffer
 * @param src [IN] Type #const #char * Source buffer
 *
 * @retval #EXT_ERR_SUCCESS Success.
 * @retval Other values Failure.
 *
 * @par Dependency:
 * @li soc_stdlib.h: This file declares the APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
extern errno_t strcpy_s(char *dest, size_t dest_max, const char *src);

/**
 * @ingroup  hct_secure_c
 * @brief  Copies the source string of a specified length to the destination buffer.
 *
 * @par Description:
 * Copies the source string of a specified length to the destination buffer.
 * @attention None
 *
 * @param dest [IN] Type #char * Destination buffer
 * @param dest_max [IN] Type #size_t Size of the destination buffer
 * @param src [IN] Type #const #char * Source buffer
 * @param count [IN] Type #size_t Number of characters copied from the source buffer
 *
 * @retval #EXT_ERR_SUCCESS Success.
 * @retval Other values Failure.
 *
 * @par Dependency:
 * @li soc_stdlib.h: This file declares the APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
extern errno_t strncpy_s(char *dest, size_t dest_max, const char *src, size_t count);

/**
 * @ingroup  hct_secure_c
 * @brief  Concatenates the source string to the end of the destination string.
 *
 * @par Description:
 * Concatenates the source string to the end of the destination string.
 * @attention None
 *
 * @param dest [IN] Type #char * Destination buffer
 * @param dest [IN] Type #size_t Size of the destination buffer
 * @param src [IN] Type #const #char * Source buffer
 *
 * @retval #EXT_ERR_SUCCESS Success.
 * @retval Other values Failure.
 *
 * @par Dependency:
 * @li soc_stdlib.h: This file declares the APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
extern errno_t strcat_s(char *dest, size_t dest_max, const char *src);

/**
 * @ingroup  hct_secure_c
 * @brief  Concatenates the source string of a specified length to the end of the destination string.
 *
 * @par Description:
 * Concatenates the source string of a specified length to the end of the destination string.
 * @attention None
 *
 * @param dest [IN] Type #char * Destination buffer
 * @param dest_max [IN] Type #size_t Size of the destination buffer
 * @param src [IN] Type #const #char *. Source buffer
 * @param count [IN] Type #size_t Number of characters to be concatenated for the source buffer
 *
 * @retval #EXT_ERR_SUCCESS Success.
 * @retval Other values Failure.
 *
 * @par Dependency:
 * @li soc_stdlib.h: This file declares the APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
extern errno_t strncat_s(char *dest, size_t dest_max, const char *src, size_t count);

/**
 * @ingroup  hct_secure_c
 * @brief  Copies the data from the source buffer to the destination buffer.
 *
 * @par Description:
 * Copies the data from the source buffer to the destination buffer.
 * @attention None
 *
 * @param dest [OUT] Type #void * Destination buffer
 * @param dest_max [IN] Type #size_t Size of the destination buffer
 * @param src [IN] Type #const #void * Source buffer
 * @param count [IN] Type #UAPI_U32 Number of characters copied from the source buffer
 *
 * @retval #EXT_ERR_SUCCESS Success.
 * @retval Other values Failure.
 *
 * @par Dependency:
 * @li soc_stdlib.h: This file declares the APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
extern errno_t memcpy_s(void *dest, size_t dest_max, const void *src, size_t count);

/**
 * @ingroup  hct_secure_c
 * @brief  Sets the size of the destination buffer to a specific value.
 *
 * @par Description:
 * Sets the size of the destination buffer to a specific value.
 * @attention None
 *
 * @param dest [IN] Type #void * Destination buffer
 * @param dest_max [IN] Type #size_t Size of the destination buffer
 * @param c [IN] Type #int Character to be set in the destination buffer
 * @param count [IN] Type #size_t Number of characters to be set in the destination buffer
 *
 * @retval #EXT_ERR_SUCCESS Success.
 * @retval Other values Failure.
 *
 * @par Dependency:
 * @li soc_stdlib.h: This file declares the APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
extern errno_t memset_s(void *dest, size_t dest_max, int c, size_t count);

/**
 * @ingroup  hct_secure_c
 * @brief  Moves the data from the source buffer to the destination buffer.
 *
 * @par Description:
 * Moves the data from the source buffer to the destination buffer.
 * @attention None
 *
 * @param dest [OUT] Type #void * Destination buffer
 * @param dest_max [IN] Type #size_t Size of the destination buffer
 * @param src [IN] Type #const #void* Source buffer
 * @param count [IN] Type #size_t Number of characters moved from the source buffer
 *
 * @retval #EXT_ERR_SUCCESS Success.
 * @retval Other values Failure.
 *
 * @par Dependency:
 * @li soc_stdlib.h: This file declares the APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
extern errno_t memmove_s(void *dest, size_t dest_max, const void *src, size_t count);

/**
 * @ingroup  hct_secure_c
 * @brief  Splits a string into substrings according to the specified separators.
 *
 * @par Description:
 * Splits a string into substrings according to the specified separators.
 * @attention None
 *
 * @param token [IN] Type #char * String to be split
 * @param delimit [IN] Type #const char * String separator
 * @param context [IN] Type #char* Position information after a call to EXT_strtok_s is saved
 *
 * @retval #char* Type pointer Point to the next token.
 * @retval #TD_NULL A specified substring is not found or an error occurs.
 *
 * @par Dependency:
 * @li soc_stdlib.h: This file declares the APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
extern char *strtok_s(char *token, const char *delimit, char **context);

/**
 * @ingroup  hct_secure_c
 * @brief  Formats the data and outputs it to the destination buffer.
 *
 * @par Description:
 * Formats the data and outputs it to the destination buffer.
 * @attention None
 *
 * @param dest [OUT] Type #char * Destination buffer
 * @param dest_max [IN] Type #size_t Size of the destination buffer
 * @param format [IN] Type #const #char * Formatting control string
 * @param ... [IN] Optional parameter
 *
 * @retval #EXT_ERR_SUCCESS Success.
 * @retval Other values Error code.
 *
 * @par Dependency:
 * @li soc_stdlib.h: This file declares the APIs.
 * @see None
 * @since DW21_V100R001C00
 */
extern int sprintf_s(char *dest, size_t dest_max, const char *format, ...) __attribute_format(3, 4);

/**
 * @ingroup  hct_secure_c
 * @brief Formats the data according to a specified length and outputs the data to the destination buffer.
 *
 * @par Description:
 * Formats the data according to a specified length and outputs the data to the destination buffer.
 * @attention None
 *
 * @param dest [OUT] Type #char * Destination buffer
 * @param dest_max [IN] Type #size_t Size of the destination buffer
 * @param size_t [IN] Type #size_t Number of formatted characters to be output to the destination buffer
 * @param format [IN] Type #const #char * Formatting control string
 * @param ... [IN] Optional parameter
 *
 * @retval #EXT_ERR_SUCCESS Success.
 * @retval Other values Error code.
 *
 * @par Dependency:
 * @li soc_stdlib.h: This file declares the APIs.
 * @see None
 * @since DW21_V100R001C00
 */
extern int snprintf_s(char *dest, size_t dest_max, size_t count, const char *format, ...) __attribute_format(4, 5);

/**
 * @ingroup  hct_secure_c
 * @brief  Formats the data and outputs it to the destination buffer.
 *
 * @par Description:
 * Formats the data and outputs it to the destination buffer.
 * @attention None
 *
 * @param dest [OUT] Type #char * Destination buffer
 * @param dest_max [IN] Type #size_t Size of the destination buffer
 * @param format [IN] Type #const #char * Formatting control string
 * @param arg_list [IN] Type #va_list Parameter list
 *
 * @retval #EXT_ERR_SUCCESS Success.
 * @retval Other values Error code.
 *
 * @par Dependency:
 * @li soc_stdlib.h: This file declares the APIs.
 * @see None
 * @since DW21_V100R001C00
 */
extern int vsprintf_s(char *dest, size_t dest_max, const char *format, va_list arg_list);

/**
 * @ingroup  hct_secure_c
 * @brief Formats the data according to a specified length and outputs the data to the destination buffer.
 *
 * @par Description:
 * Formats the data according to a specified length and outputs the data to the destination buffer.
 * @attention None
 *
 * @param dest [OUT] Type #char * Destination buffer
 * @param dest_max [IN] Type #size_t Size of the destination buffer
 * @param size_t [IN] Type #size_t Number of formatted characters to be output to the destination buffer
 * @param format [IN] Type #const #char * Formatting control string
 * @param arg_list [IN] Type #va_list Parameter list
 *
 * @retval #EXT_ERR_SUCCESS Success.
 * @retval Other values Error code.
 *
 * @par Dependency:
 * @li soc_stdlib.h: This file declares the APIs.
 * @see None
 * @since DW21_V100R001C00
 */
extern int vsnprintf_s(char *dest, size_t dest_max, size_t count, const char *format, va_list arg_list);

/*
 * C library APIs
 */
extern int isalnum(int c);
extern int isalpha(int c);
extern int isblank(int c);
extern int iscntrl(int c);
extern int isdigit(int c);
extern int isgraph(int c);
extern int islower(int c);
extern int isprint(int c);
extern int ispunct(int c);
extern int isspace(int c);
extern int isupper(int c);
extern int isxdigit(int c);
extern int tolower(int c);
extern int toupper(int c);
extern int atoi(const char *str);
extern long atol(const char *str);
extern long strtol(const char *nptr, char **endptr, int base);
extern unsigned long strtoul(const char *nptr, char **endptr, int base);
extern CONST_TYPE void *memchr(const void *src, int c, size_t n);
extern int memcmp(const void *str1, const void *str2, size_t n);
extern CONST_TYPE char *strchr(const char *s, int c);
extern int strcmp(const char *s1, const char *s2);
extern size_t strlen(const char *src);
extern CONST_TYPE char *strstr(const char *s, const char *find);
extern char *strnstr(const char *s1, const char *s2, size_t len);
extern struct tm *gmtime(const time_t * const timep);
extern struct tm *gmtime_r(const time_t * const timep, struct tm *result);
extern double log10(double x);

#define isascii(c) ((c) >= 0 && (c) <= 0x7f)

#define ext_mdm_str_len(str) strlen(str)
#define uapi_mdm_memcmp(buf1, buf2, count) memcmp(buf1, buf2, (td_u32)count)
#define ext_mdm_str_toul(nptr, endptr, ibase) strtoul(nptr, endptr, ibase)
#define ext_mdm_str_str(s1, s2) strstr(s1, s2);
#define ext_mdm_strn_str(s1, s2, len) strnstr(s1, s2, len)
#define ext_mdm_str_cucr(s, c) strchr(s, c)

/**
* @ingroup  hct_uart
* @brief  Formatting output API for the UART port
*
* @par Description:
*         Formats the data according to a specified length and outputs the data to the destination buffer.
* @attention This function occupies 300 bytes of stack space. Ensure that the stack space is sufficient.
*
* @param fmt [IN] Type #const char * Formatting input of the variable parameter

*
* @retval #EXT_ERR_SUCCESS Success.
* @retval Other values Error code.
*
* @par Dependency:
* @li soc_stdlib.h: This file declares the APIs.
* @see None
* @since DW21_V100R001C00
 */
#if !defined(UT_SWITCH)
extern void printf(const char *fmt, ...) __attribute_format(1, 2);
#endif
EXT_END_HEADER
#endif // __SOC_STDLIB_H__
