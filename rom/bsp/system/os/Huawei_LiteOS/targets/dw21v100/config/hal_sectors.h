/******************************************************************************
 * @brief    header file for the hal_sectors
 * Copyright (c) 2015 LiteOS LIMITED
******************************************************************************/

#ifndef _LITEOS_HAL_SECTORS_H
#define _LITEOS_HAL_SECTORS_H

#include "los_typedef.h"

// HAL sectors API


/**he size of the RAM assigned to the heap in internal RAM (in octets)*/
extern UINT32 hal_sectors_ram_heap_size;

/**The pointer to the start of the external RAM assigned to the internal RAM heap*/
extern VOID *hal_sectors_ram_heap_start;

extern VOID *hal_sectors_stack_end;

/**The total size of the RAM (in octets)*/
extern UINT32 hal_sectors_ram_size;

/**The start address of RAM*/
extern VOID* hal_sectors_ram_start;

#endif /* _LITEOS_HAL_SECTORS_H */
