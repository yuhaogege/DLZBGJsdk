/*
 * Copyright (c) CompanyNameMagicTag 2019-2019. All rights reserved.
 * Description: This document describes error codes. An error code redefined as
 * .
 * Author: CompanyName
 * Create: 2019-10-31
 */

#ifndef __SOC_FIXROM_SYM_H__
#define __SOC_FIXROM_SYM_H__


/* 
    0x000163d9	91097 0x000163d8	91096
    void rom_boot_malloc_init(td_u32 heap_start_addr, td_u32 heap_end_addr)
*/
#define ROM_BOOT_MALLOC_INIT ((void (*)(td_u32, td_u32))0x000163dd)

/* 
    0x00016455	91221 0x00016454	91220
    void *rom_boot_malloc(unsigned int size)
*/
#define ROM_BOOT_MALLOC ((void* (*)(td_u32))0x00016459)
/* 
    0x00016529	91433 0x00016528	91432
    td_s32 rom_boot_free(void *addr)
*/
#define ROM_BOOT_FREE ((td_s32 (*)(void*))0x0001652d)

#endif  /* __FIXROM_SYM_H__ */

