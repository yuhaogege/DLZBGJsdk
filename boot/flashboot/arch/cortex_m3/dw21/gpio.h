/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: gpio
 */

#ifndef GPIO_H
#define GPIO_H

/*
 * GPIO Interface
 */
#define UAPI_IO_VAL_SET(cond, id, reg_val) \
    if (cond) {                          \
        (reg_val) |= (td_u32)(1 << (id));    \
    } else {                             \
        (reg_val) &= ~(td_u32)(1 << (id));   \
    }

#define UAPI_IO_DIR_GET(cond, id, val_addr)  \
    if ((cond) & (td_u32)(1 << (id))) {        \
        *(val_addr) = EXT_GPIO_DIRECTION_OUT; \
    } else {                               \
        *(val_addr) = EXT_GPIO_DIRECTION_IN;  \
    }

#define UAPI_IO_VALUE_GET(cond, id, val_addr) \
    if ((cond) & (td_u32)(1 << (id))) {         \
        *(val_addr) = EXT_GPIO_VALUE1;         \
    } else {                                \
        *(val_addr) = EXT_GPIO_VALUE0;         \
    }

#endif
