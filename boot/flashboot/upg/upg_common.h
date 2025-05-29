/* *
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: upg common
 */

#ifndef __UPG_COMMON_H__
#define __UPG_COMMON_H__

#include "soc_21_upg.h"

#include <soc_types.h>
#include <securec.h>
#include <soc_flashboot_flash.h>
#include <soc_boot_rom.h>
#include <soc_flash_boot.h>

#ifdef UPG_DEBUG_PRINT
#define upg_debug_print0(s) boot_msg0(s)
#define upg_debug_print1(s, x1) boot_msg1(s, x1)
#define upg_debug_print2(s, x1, x2) boot_msg2(s, x1, x2)
#define upg_debug_print3(s, x1, x2, x3) boot_msg3(s, x1, x2, x3)
#define upg_debug_print4(s, x1, x2, x3, x4) boot_msg4(s, x1, x2, x3, x4)
#else
#define upg_debug_print0(s)
#define upg_debug_print1(s, x1)
#define upg_debug_print2(s, x1, x2)
#define upg_debug_print3(s, x1, x2, x3)
#define upg_debug_print4(s, x1, x2, x3, x4)
#endif

typedef struct {
    td_u32 boot_ver_magic;     /* boot version magic byte */
    td_u32 product_type;       /* current product type */
    td_u32 flash_addr_boot;    /* flash address: boot */
    td_u32 flash_addr_ker;     /* flash address: kernel */
    td_u32 flash_addr_nv_file; /* flash address: original nv file */
    td_u32 flash_addr_tail_upg;
    td_u32 flash_addr_upg; /* flash address: upgrade file. get address by calculating flash_addr_tail_upg */

    /* global space pointer */
    ext_start_tbl *start_tbl;                /* point to start info list */
    ext_21_upg_head *upg_head;             /* point to upgrade file head */
    ext_21_upg_head_section *upg_sections; /* point to upgrade section info */

    td_u16 env_ok : 1;
    td_u16 tbl_ok : 1;              /* start info list OK */
    td_u16 kernel_ok : 1;           /* start info list kernel OK */
    td_u16 nv_file_ok : 1;          /* start config nv original file OK */
    td_u16 start_enable : 1;        /* start enable with current configuration */
    td_u16 ver_magic_different : 1; /* upgrade file ver is different from running ver */
    td_u16 upg_file_phas1_ok : 1;   /* read upgrade nv failed */
    td_u16 upg_file_phas2_ok : 1;   /* all upgrade file check correct */
    td_u16 new_kernel : 1;          /* compare magic version flag */
    td_u16 upg_file_phas0_ok : 1;   /* exists upgrade file flag */
    td_u16 upg_magics_exist : 3;
    td_u16 reserver : 3; /* starting mode is section start */

    td_u8 upg_reason;    /* upgrading reason */
    td_u8 upg_file_type; /* read upgrading status from nv(ext_21_upg_type_e) */
} ext_start_ctrl;
extern ext_start_ctrl g_start_ctrl;

#define EXT_START_TBL_MAX_SIZE_LIMIT (16 * 1024) /* statrting configuration maximum size */

#define ext_upg_head_get_head() ((ext_21_upg_head *)(g_start_ctrl.upg_head))
#define ext_upg_head_get_section(n) ((ext_21_upg_head_section *)(&g_start_ctrl.upg_sections[n]))

#define ext_upg_head_get_sections() ((ext_21_upg_head_section *)(g_start_ctrl.upg_sections))

#define ext_start_tbl_get_sections() ((ext_start_tbl_section *)(g_start_ctrl.start_tbl->section))
#define ext_start_tbl_get_tbl() ((ext_start_tbl *)(g_start_ctrl.start_tbl))

#endif
