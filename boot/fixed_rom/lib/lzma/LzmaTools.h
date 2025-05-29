/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Usefuls routines based on the LzmaTest.c file from LZMA SDK 4.65
 *
 * Copyright (CompanyNameMagicTag) 2007-2008 Industrie Dial Face S.p.A.
 *
 * Copyright (CompanyNameMagicTag) 1999-2005 Igor Pavlov
 */

#ifndef __LZMA_TOOL_H__
#define __LZMA_TOOL_H__

#include <soc_types.h>

/* The return value of the function is the actual read/write length. Note that the return value is not success/fail */
typedef td_u32 (*LZMA_STREAM_FCT)(td_u32 offset, td_u8* buffer, td_u32 size);

typedef struct
{
    td_u32 offset;
    LZMA_STREAM_FCT func;
} LZMA_STREAM_S;

extern int lzmaBuffToBuffDecompress (unsigned char* outStream, unsigned int* uncompressedSize,
                                     unsigned char* inStream, unsigned int length, ISzAlloc* alloc);

extern unsigned int LzmaDecode2(const Byte* prop_data, unsigned int prop_size, ELzmaStatus* status,
    ISzAlloc* alloc, LZMA_STREAM_S* in_stream, LZMA_STREAM_S* out_stream,
    unsigned int uncompress_len, unsigned int compress_len);
#endif

