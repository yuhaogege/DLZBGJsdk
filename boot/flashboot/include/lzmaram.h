/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: lzma ram
 */

#ifndef __LZMARAM_H__
#define __LZMARAM_H__

#include <soc_types.h>

typedef td_u32 (*lzma_stream_fct)(td_u32 offset, td_u8 *buffer, td_u32 size);

typedef struct {
    td_u32 offset;
    lzma_stream_fct func;
} lzma_stream;

typedef struct {
    td_pbyte (*alloc)(td_pbyte p, unsigned int size);
    void (*free)(td_pbyte p, td_pbyte address); /* address can be 0 */
} i_sz_alloc;

extern unsigned int LzmaDecode2(const unsigned char *prop_data, unsigned int prop_size, td_u32 *status,
    i_sz_alloc *alloc, lzma_stream *in_stream, lzma_stream *out_stream, unsigned int uncompress_len,
    unsigned int compress_len);
unsigned int uapi_lzma_decompress(const td_u8 lzma_head[13], td_u32 head_size, /* head 13B */
    unsigned int compress_len, lzma_stream_fct in_func, lzma_stream_fct out_func);
unsigned int uapi_lzma_get_uncompress_len(const td_u8 lzma_head[13], td_u32 head_size, /* head 13B */
    unsigned int *decompressed_len, unsigned int *dic_size);
unsigned int uapi_lzma_mem_detect(const td_u8 lzma_head[13], td_u32 head_size); /* head 13B */

#endif
