/*  $OpenBSD: stdio.h,v 1.35 2006/01/13 18:10:09 miod Exp $ */
/*  $NetBSD: stdio.h,v 1.18 1996/04/25 18:29:21 jtc Exp $   */

/*-
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Chris Torek.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *  @(#)stdio.h 5.17 (Berkeley) 6/3/91
 */

/**@defgroup fs Filesystem
 *
 */

#ifndef _STDIO_H_
#define _STDIO_H_

//#include "sys/cdefs.h"
#include "sys/types.h"
#include "stdarg.h"
#include "stddef.h"

#include "los_printf.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * stdio state variables.
 *
 * The following always hold:
 *
 *  if (_flags&(__SLBF|__SWR)) == (__SLBF|__SWR),
 *      _lbfsize is -_bf._size, else _lbfsize is 0
 *  if _flags&__SRD, _w is 0
 *  if _flags&__SWR, _r is 0
 *
 * This ensures that the getc and putc macros (or inline functions) never
 * try to write or read from a file that is in `read' or `write' mode.
 * (Moreover, they can, and do, automatically switch from read mode to
 * write mode, and back, on "r+" and "w+" files.)
 *
 * _lbfsize is used only to make the inline line-buffered output stream
 * code as compact as possible.
 *
 * _ub, _up, and _ur are used when ungetc() pushes back more characters
 * than fit in the current _bf, or when ungetc() pushes back a character
 * that does not match the previous one in _bf.  When this happens,
 * _ub._base becomes non-nil (i.e., a stream has ungetc() data iff
 * _ub._base!=NULL) and _up and _ur save the current values of _p and _r.
 *
 * NOTE: if you change this structure, you also need to update the
 * std() initializer in findfp.c.
 */



#define __SLBF  0x0001      /* line buffered */
#define __SNBF  0x0002      /* unbuffered */
#define __SRD   0x0004      /* OK to read */
#define __SWR   0x0008      /* OK to write */
    /* RD and WR are never simultaneously asserted */
#define __SRW   0x0010      /* open for reading & writing */
#define __SEOF  0x0020      /* found EOF */
#define __SERR  0x0040      /* found error */
#define __SMBF  0x0080      /* _buf is from malloc */
#define __SAPP  0x0100      /* fdopen()ed in append mode */
#define __SSTR  0x0200      /* this is an sprintf/snprintf string */
#define __SOPT  0x0400      /* do fseek() optimization */
#define __SNPT  0x0800      /* do not do fseek() optimization */
#define __SOFF  0x1000      /* set iff _offset is in fact correct */
#define __SMOD  0x2000      /* true => fgetln modified _p text */
#define __SALC  0x4000      /* allocate string space dynamically */
#define __SIGN  0x8000      /* ignore this file in _fwalk */

/*
 * The following three definitions are for ANSI C, which took them
 * from System V, which brilliantly took internal interface macros and
 * made them official arguments to setvbuf(), without renaming them.
 * Hence, these ugly _IOxxx names are *supposed* to appear in user code.
 *
 * Although numbered as their counterparts above, the implementation
 * does not rely on this.
 */
#define _IOFBF  0       /* setvbuf should set fully buffered */
#define _IOLBF  1       /* setvbuf should set line buffered */
#define _IONBF  2       /* setvbuf should set unbuffered */

#define BUFSIZ  (8*1024)       /* size of buffer used by setbuf */
#define EOF (-1)


/**
* @ingroup  stdio
* @brief read character from the iostream.
*
* @par Description:
* This API is used to read from the iostream and is line buffered; this means it will not return until you press ENTER
* @attention None.
*
* @param  None.
*
* @retval int return a character which got from the iostream.
* @par Dependency:
* <ul><li>stdio.h: the header file that contains the API declaration.</li></ul>
* @see
* @since Huawei LiteOS V100R001C00
*/
int  getchar(void);

/**
* @ingroup  stdio
* @brief print formatted data to stdout.
*
* @par Description:
* This API is used to print formatted data to stdout.
* @attention max buffer size is 256Bytes.
* @param  fmt         [IN]    Type #const char*    C string that contains the text to be written to stdout.
* @retval int  return the number of characters printed.
* @retval negative value   If an output error is encountered.
* @par Dependency:
* <ul><li>stdio.h: the header file that contains the API declaration.</li></ul>
* @see
* @since Huawei LiteOS V100R001C00
*/
void printf(const char *fmt, ...);


/**
* @ingroup  stdio
* @brief write character to stdout.
*
* @par Description:
* This API is used to write character to stdout.
* @attention NONE.
* @param  int         [IN]    Type #int the int promotion of the character to be written.
* @retval int character the character written is returned.
* @retval EOF  if a writing error occurs, EOF is returned and the error indicate(ferror) is set.
* @par Dependency:
* <ul><li>stdio.h: the header file that contains the API declaration.</li></ul>
* @see
* @since Huawei LiteOS V100R001C00
*/
int  putchar(int);


/**
* @ingroup  stdio
* @brief write formatted data to string.
*
* @par Description:
* This API is used to write formatted data to string.
* @attention  max buffer size is 256Bytes.
* @param  str         [OUT]   Type # char * pointer to a buffer where the resulting C-string stored.
* @param  fmt         [IN]    Type # const char * C string that contains the text to be written to stdout.
* @retval int the total number of characters written.
* @retval on failure, a negative number is returned.
* @par Dependency:
* <ul><li>stdio.h: the header file that contains the API declaration.</li></ul>
* @see
* @since Huawei LiteOS V100R001C00
*/
int  sprintf(char * __restrict, const char * __restrict, ...);


/**
* @ingroup  stdio
* @brief write formatted data from variable arguments list to string.
*
* @par Description:
* This API is used to write formatted data from variable arguments list to string.
* @attention  max buffer size is 256Bytes.
* @param  __restrict        [OUT] Type # char * pointer to a buffer where the resulting C-string is stored.
*                                                   the buffer should be large enough to contain the resulting string.
*
* @param  __restrict        [IN]   Type # const char * C string that contains a format string.
* @param  va_list           [IN]   Type # va_list a value identifying a variable arguments list initialized with
*                                                 va_start va_list is a spectial type define in <stdarg.h>.
* @retval int the total number of characters written.
* @retval on failure, the error indicator(ferror) is set and a negative number is returned.

* @par Dependency:
* <ul><li>stdio.h: the header file that contains the API declaration.</li></ul>
* @see
* @since Huawei LiteOS V100R001C00
*/
int  vsprintf(char * __restrict, const char * __restrict,
    va_list);

int vsnprintf(char *str, size_t len, const char *fmt, va_list ap);


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _STDIO_H_ */
