/******************************************************************************
 * @brief    hal_sectors c file - get variables from the linker file
 * Copyright (c) 2015 LiteOS LIMITED
******************************************************************************/

#include "hal_sectors.h"

//lint -esym(526, __INTHEAP_SIZE__)
//lint -esym(526, __INTHEAP_BEGIN__)
//lint -esym(526, __RAM_SIZE__)
//lint -esym(526, __RAM_BEGIN__)
// Declare the linker symbols
extern void *__HEAP_SIZE__;
extern void *__HEAP_BEGIN__;
extern void *__SYSTEM_STACK_END__;
extern void *__RAM_SIZE__;
extern void *__RAM_BEGIN__;

/**The size of the RAM assigned to the heap in internal RAM (in octets)*/
LITE_OS_SEC_DATA UINT32 hal_sectors_ram_heap_size = ((UINT32)&__HEAP_SIZE__);

/**The pointer to the start of the external RAM assigned to the internal RAM heap*/
LITE_OS_SEC_DATA VOID *hal_sectors_ram_heap_start = ((void*)&__HEAP_BEGIN__);

LITE_OS_SEC_DATA VOID *hal_sectors_stack_end = ((void*)&__SYSTEM_STACK_END__);

/**The total size of the RAM (in octets)*/
LITE_OS_SEC_DATA UINT32 hal_sectors_ram_size = ((UINT32)&__RAM_SIZE__);

/**The start address of RAM*/
LITE_OS_SEC_DATA VOID *hal_sectors_ram_start = ((void*)&__RAM_BEGIN__);
