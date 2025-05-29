/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: fpb related code upload fpb_lib.h code
 * Author: CompanyName
 * Create: 2012-04-17
 */
#ifndef __FPB_LIB_H__
#define __FPB_LIB_H__
#include "stdint.h"

#define REMAP_TABLE_ADDR (0x20000000UL)

#define FPB_REG_MAX 254

#define LITERAL_REG1_INDEX 6
#define LITERAL_REG2_INDEX 7

typedef enum {
    FPB_TYPE_INSTR = 0,
    FPB_TYPE_LITERAL = 1,
    FPB_TYPE_MAX
} fpb_comp_type;

void fpb_init(void);

uint32_t fpb_add_patch(uint32_t old_addr, uint32_t patch_value, fpb_comp_type fpb_type);

uint32_t fpb_delete_patch(uint32_t old_addr, fpb_comp_type fpb_type);

void fpb_disable(void);
#endif
