/*
 * Copyright (C) 2008 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES(INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _WCHAR_H_
#define _WCHAR_H_

#include "stdlib.h"
#include "stdarg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifndef MB_LEN_MAX
#define MB_LEN_MAX 16
#endif
#define __LIBC_HIDDEN__ __attribute__((visibility("hidden")))

typedef unsigned int    wint_t;
typedef unsigned int    wctype_t;
#define WEOF   (0xffffffffu)

typedef unsigned int char32_t;

typedef struct {
  uint8_t __seq[4];
#ifdef __LP64__
  char __reserved[4];
#endif
} mbstate_t;

enum {
    WC_TYPE_INVALID = 0,
    WC_TYPE_ALNUM,
    WC_TYPE_ALPHA,
    WC_TYPE_BLANK,
    WC_TYPE_CNTRL,
    WC_TYPE_DIGIT,
    WC_TYPE_GRAPH,
    WC_TYPE_LOWER,
    WC_TYPE_PRINT,
    WC_TYPE_PUNCT,
    WC_TYPE_SPACE,
    WC_TYPE_UPPER,
    WC_TYPE_XDIGIT,
    WC_TYPE_MAX
};
extern size_t wcrtomb(char *, wchar_t, mbstate_t *);
extern int mbsinit(const mbstate_t* ps);
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _WCHAR_H_ */
