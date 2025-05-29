/*----------------------------------------------------------------------------
 * Copyright (c) <2013-2017>, <Huawei Technologies Co., Ltd>
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
#include <stdarg.h>
#include "printf.h"
#include "string.h"
LITE_OS_SEC_DATA_INIT OS_SENDCHR2CHL_HOOK_FUNC g_pLogHook = NULL;

LITE_OS_SEC_TEXT void register_log_hook(OS_SENDCHR2CHL_HOOK_FUNC fun) {
    g_pLogHook = fun;
}

void printf(const char *fmt, ...);

#define BUF 40

/*
 * Flags used during conversion.
 */
#define ALT             0x001           /* alternate form */
#define HEXPREFIX       0x002           /* add 0x or 0X prefix */
#define LADJUST         0x004           /* left adjustment */
#define LONGDBL         0x008           /* long double; unimplemented */
#define LONGINT         0x010           /* long integer */
#define QUADINT         0x020           /* quad integer */
#define SHORTINT        0x040           /* short integer */
#define ZEROPAD         0x080           /* zero (as opposed to blank) pad */
#define FPT             0x100           /* Floating point number */
#define SIZET           0x200           /* size_t */


#define to_digit(c)     ((c) - '0')
#define is_digit(c)     ((unsigned)to_digit(c) <= 9)
#define to_char(n)      ((n) + '0')

/*#define  SIZEBUF  256*/
#define PADSIZE 32             /* pad chunk size */
#define quad_t long long
#define u_quad_t unsigned long long


#define DEFPREC         6
#define SMALL_NUMBER_BUFFER


#define SARG() \
    (flags&QUADINT ? va_arg(arg, signed long long) : \
     flags&LONGINT ? va_arg(arg, long) : \
     flags&SHORTINT ? (long)(short)va_arg(arg, int) : \
     flags&SIZET ? (long)va_arg(arg, size_t) : \
     (long)va_arg(arg, int))
#define UARG() \
    (flags&QUADINT ? va_arg(arg, unsigned long long) : \
     flags&LONGINT ? va_arg(arg, unsigned long) : \
     flags&SHORTINT ? (unsigned long)(unsigned short)va_arg(arg, int) : \
     flags&SIZET ? va_arg(arg, size_t) : \
     (unsigned long)va_arg(arg, unsigned int))

#define PRINT(ptr,len)            \
    do{                            \
        int tmp_len = len;         \
        char *tmp_ptr = ptr;       \
        while(tmp_len-- && buff_end) { \
            buff_out[256 - buff_end--] = *tmp_ptr++; \
        } \
    }while(0)


#define PAD(howmany, with)                                                   \
    do{                                                              \
        if ((x = (howmany)) > 0) {                                               \
            while (x > PADSIZE) {                                                \
                PRINT(with, (unsigned int)PADSIZE);                                            \
                x -= PADSIZE;                                                    \
            }                                                                    \
            PRINT(with, x);                                                      \
        }                                                                        \
    }while(0)
        
LITE_OS_SEC_BSS static char blanks[PADSIZE];
LITE_OS_SEC_BSS static char zeroes[PADSIZE];

LITE_OS_SEC_TEXT int __dprintf(const char *format, va_list arg,OUTPUT_FUNC pFputc,void *cookie)
{
    char *fmt;
    int ch;
    int x,y;
    char *cp ;
    int flags;
    int ret;
    int width;
    int prec;
    char sign;
    UINT32 wc;

    u_quad_t _uquad;
    enum {OCT,DEC,HEX} base = OCT;
    int dprec;
    int fieldsz;
    int realsz;
    int size;
    char *xdigs;

    char buff_out[MAX_PRINT_SIZE] = {0};
    unsigned int buff_end = MAX_PRINT_SIZE;
    char buf[BUF];
    char ox[2];

    if (NULL == pFputc)
    {
        return -1;
    }

    memset(blanks, ' ', PADSIZE);
    memset(zeroes, '0', PADSIZE);

    xdigs = (char *)NULL;
    fmt = (char*)format;
    ret = 0;

    while(buff_end){
        cp = (char*)fmt;
        while(x=((wc = *fmt) != 0)){
            fmt += x;
            if(wc == '%'){
                fmt--;
                break;
            }
        }//end while(x=..

        if ((y = fmt - cp) != 0)
        {
            PRINT(cp,y);/*lint -e42 -e40 -e52*/
        }

        if((x <= 0))
        {
           goto end;
        }

        fmt++;
        flags = 0;
        dprec = 0;
        sign = '\0';
        prec = -1;
        width = 0;

rflag:
        ch = *fmt++;
reswitch:
        switch (ch) {
                case '*':
                    if ((width = va_arg(arg, int)) >= 0)
                        goto rflag;
                    width = -width;
                case '-': /*lint !e616*/
                    flags |= LADJUST;
                    goto rflag;
                case ' ':
                    if(!sign)
                        sign = ' ';
                    goto rflag;
                case '.':
                    if ((ch = *fmt++) == '*') {
                        x = va_arg(arg, int);
                        prec = x < 0 ? -1 : x;
                        goto rflag;
                    }
                    x = 0;
                    while (is_digit(ch)) {
                        x = 10 * x + to_digit(ch);
                        ch = *fmt++;
                    }
                    prec = x < 0 ? -1 : x;
                    if(flags & LADJUST &&(prec > 0))
                    {
                        prec++;
                    }
                    goto reswitch;
                case '#':
                    flags |= ALT;
                    goto rflag;
                case '+':
                    sign = '+';
                    goto rflag;
                case '0':
                    flags |= ZEROPAD;
                    goto rflag;
                case '1': case '2': case '3': case '4':
                case '5': case '6': case '7': case '8': case '9':
                    x = 0;
                    do {
                        x = 10 * x + to_digit(ch);
                        ch = *fmt++;
                    } while (is_digit(ch));
                    width = x;
                    goto reswitch;
                case 'h':
                    flags |= SHORTINT;
                    goto rflag;
                case 'l':
                    if (*fmt == 'l') {
                        fmt++;
                        flags |= QUADINT;
                    } else {
                        flags |= LONGINT;
                    }
                    goto rflag;
                case 'q':
                    flags |= QUADINT;
                    goto rflag;
                case 'c':
                    *(cp = buf) = va_arg(arg, int);
                    size = 1;
                    sign = '\0';
                    break;
                case 'e':
                case 'E':
                case 'f':
                case 'g':
                case 'G':
                    (void) va_arg(arg, double);
                    cp = "";
                    size = 0;
                    sign = '\0';
                    break;
                case 'O':
                    flags |= LONGINT;
                case 'o': /*lint !e616*/
                    _uquad = UARG(); /*lint !e571 !e64 !e48 !e23*/
                    base = OCT;
                    goto nosign;
                case 'D':
                    flags |= LONGINT;
                case 'd': /*lint !e616*/
                case 'i':
                    _uquad = SARG();
#ifndef _NO_LONGLONG
                    if ((quad_t)_uquad < 0)
#else
                        if ((long) _uquad < 0)
#endif
                        {
                            _uquad = (~_uquad + 1);
                            sign = '-';
                        }
                    base = DEC;
                    goto number;
                case 'U':
                    flags |= LONGINT;
                case 'u': /*lint !e616*/
                    _uquad = UARG();
                    base = DEC;
                    goto nosign;

                /*lint -e48*/
                case 'n':
#ifndef ___NO_LONGLONG
                    if (flags & QUADINT)
                        *va_arg(arg, quad_t *) = ret;
                    else
#endif
                        if (flags & LONGINT)
                            *va_arg(arg, long *) = ret;
                        else if (flags & SHORTINT)
                            *va_arg(arg, short *) = ret;
                        else if (flags & SIZET)
                            *va_arg(arg, size_t *) = ret;
                        else
                            *va_arg(arg, int *) = ret;
                    continue;
                case 's':
                    if ((cp = va_arg(arg, char *)) == NULL) /*lint !e64*/
                        cp = (char *)"(null)";
                    if (prec >= 0) {
                        char *p = (char *)memchr(cp, 0, prec);
                        if (p != NULL) {
                            size = p - cp;
                            if (size > prec)
                                size = prec;
                        } else
                            size = prec;
                    } else
                        size = strlen(cp);
                    sign = '\0';
                    break;
                case 'p':
                    _uquad = (unsigned long)va_arg(arg, void *); /*lint !e48 */
                    base = HEX;
                    xdigs = (char *)"0123456789abcdef";
                    flags |= HEXPREFIX;
                    ch = 'x';
                    goto nosign;
                case 'z':
                    flags |= SIZET;
                    goto rflag;
                case 'X':
                    xdigs = (char *)"0123456789ABCDEF";
                    goto hex;
                case 'x':
                    xdigs = (char *)"0123456789abcdef";
hex:            _uquad = UARG();
                base = HEX;
                if (flags & ALT && _uquad != 0)
                    flags |= HEXPREFIX;
nosign:         sign = '\0';

number:         if ((dprec = prec) >= 0)
                    flags &= ~ZEROPAD;
                cp = buf + BUF;
                if (_uquad != 0 || prec != 0) {
                    switch(base){
                        case DEC:
                            if(flags & QUADINT) {
                                while (_uquad >= 10) {
                                    u_quad_t next = _uquad / 10;
                                    *--cp = to_char(_uquad - (next * 10));
                                    _uquad = next;
                                }
                                *--cp = to_char(_uquad);
                            }
                            else {
                                unsigned long v = (unsigned long)_uquad;
                                while (v >= 10) {
                                    unsigned long next = v / 10;
                                    *--cp = to_char(v - (next * 10));
                                    v = next;
                                }
                                *--cp = to_char(v);
                            }
                            break;
                        case HEX:
                            do {
                                *--cp = xdigs[_uquad & 15]; /*lint !e613*/
                                _uquad >>= 4;
                            } while (_uquad);
                            break;
                        case OCT:
                            do{
                                *--cp = to_char(_uquad & 7);
                                _uquad >>= 3;
                            }while(_uquad);
                            if (flags & ALT && *cp != '0')
                                *--cp = '0';
                            break;
                        default:
                            cp = (char *)"bug in __dprintf: bad base";
                            size = strlen(cp);
                            goto skipsize;
                    }
                }//end if(_uquad
                size = buf + BUF - cp;
skipsize:
                break;
                default:
                if (ch == '\0')
                {
                    goto end;
                }
                cp = buf;
                *cp = ch;
                size = 1;
                sign = '\0';
                break;
            }//end switch(ch)..
            fieldsz = size;
            if (sign)
                fieldsz++;
            else if (flags & HEXPREFIX)
                fieldsz+= 2;
            realsz = dprec > fieldsz ? dprec : fieldsz;

            if ((flags & (LADJUST|ZEROPAD)) == 0) {
                if (width - realsz > 0) {
                    PAD(width - realsz, blanks);/*lint -e42 -e40 -e52*/
                }
            }

            if (sign) {
                PRINT(&sign, 1);/*lint -e42 -e40 -e52*/
            } else if (flags & HEXPREFIX) {
                ox[0] = '0';
                ox[1] = ch;
                PRINT(ox, 2);/*lint -e42 -e40 -e52*/
            }

            if ((flags & (LADJUST|ZEROPAD)) == ZEROPAD) {
                if (width - realsz > 0) {

                    PAD(width - realsz, zeroes);/*lint -e42 -e40 -e52*/
                }
            }

            if (dprec - fieldsz > 0)
            {
                PAD(dprec - fieldsz, zeroes);/*lint -e42 -e40 -e52*/
            }

            PRINT(cp, size);/*lint -e42 -e40 -e52*/

            if (flags & LADJUST)
            {
                if (width - realsz > 0)
                {
                    PAD(width - realsz, blanks);/*lint -e42 -e40 -e52*/
                }
            }
    }//end whild(end)
end:
    ret = pFputc(buff_out, MAX_PRINT_SIZE - buff_end, cookie);
    return ret;

}

LITE_OS_SEC_TEXT int _dprintf(const char *fmt, va_list ap)
{
    int uwlen;
    if(g_pLogHook)
    {
        uwlen = __dprintf(fmt, ap, (OUTPUT_FUNC) g_pLogHook, (void *)NULL); /*lint !e611 !e64*/
        return uwlen;
    }
    return -1;
}

LITE_OS_SEC_TEXT void printf(const char *fmt, ...)
{
    va_list ap;
    if(g_pLogHook)
    {
        va_start(ap, fmt); /*lint !e530*/

        void *pfn_put = (OUTPUT_FUNC)g_pLogHook;
        (void)__dprintf(fmt, ap, (OUTPUT_FUNC)pfn_put, (void *)NULL);
        va_end(ap);
    }

}

