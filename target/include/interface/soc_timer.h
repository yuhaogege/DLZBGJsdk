/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Timer base structure.
 * Author: CompanyName
 * Create: 2018-10-31
 */

#ifndef EXT_TIMER_H_
#define EXT_TIMER_H_

#include "dw21_platform.h"

typedef struct {
    volatile unsigned int load_count;    /* !< Offset: 0x00 */
    volatile unsigned int current_value; /* !< Offset: 0x04 */
    volatile unsigned int ctrl_reg;      /* !< Offset: 0x08 */
    volatile unsigned int eoi;           /* !< Offset: 0x0C */
    volatile unsigned int int_status;    /* !< Offset: 0x10 */
} timer_type;

typedef enum {
    TIMER_MODE_ONECE = 0,
    TIMER_MODE_CYCLE = 1,
} timer_mode;

typedef enum {
    TIMER_INT_UNMASK = 0,
    TIMER_INT_MASK = 1,
} timer_int_mask;

#endif /* EXT_TIMER_H_ */
