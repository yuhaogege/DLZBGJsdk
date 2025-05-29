/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Network interface, implementation of other interfaces of the system. sal_dev_misc.c code
 * Author: CompanyName
 * Create: 2012-07-14
 */

#include "sal_dev_misc.h"
#include <soc_mdm_cipher.h>

td_u32 uapi_get_random_num32(td_u32 min, td_u32 max)
{
    td_u32 random = 0;
    td_u32 module = max + 1 - min;

    if (module == 0) {
        module = 0xffffffff;
    }

    (td_void)uapi_trng_getrandom(&random);

    return (td_u32)((random % module) + min);
}
