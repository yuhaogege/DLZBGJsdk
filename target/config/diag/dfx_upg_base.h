/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description:  upg_base
 * Author: CompanyName
 * Create: 2018-10-31
 */
#ifndef __DFX_UPG_BASE_H__
#define __DFX_UPG_BASE_H__
#include <soc_types.h>
#include <soc_mdm_types.h>

typedef struct {
    td_u32 crc_val;
} ext_trans_file_usr_data; /* CRC */

typedef struct {
    td_u32 trans_writed_before : 1;  /* not receiving the transfer file for the first time */
    td_u32 trans_start_received : 1; /* the transmission start has been received */
    td_u8 trans_file_type;
    td_u8 pad[3]; /* pad 3 byte */
    td_u32 trans_id;
    td_u32 trans_save_content_flash_addr; /* store the flash address of the file */
    td_u32 trans_save_content_flash_size; /* store the flash size of the file */
    ext_trans_file_usr_data usr_data;
} ext_trans_file_ctrl;

typedef enum {
    EXT_UPG_BASE_DFX_ERR_READ_FTNV_FAIL = 0x1,
    EXT_UPG_BASE_DFX_ERR_WRITE_FTNV_FAIL = 0x2,
    EXT_UPG_BASE_DFX_ERR_READ_NORNV_FAIL = 0x4,
    EXT_UPG_BASE_DFX_ERR_MALLOC_FAIL = 0x8,
    EXT_UPG_BASE_DFX_ERR_CHECK_UPG_FILE_FAIL = 0x10,
} ext_upg_base_dfx_err;

typedef struct {
    td_u32 ver_magic;
    td_u32 flash_addr_ker;          /* where tbl is located */
    td_u32 cur_kernel_size;         /* size of tbl+kernel */
    td_u32 flash_addr_nv_file;      /* where nv is located */
    td_u32 flash_addr_tail_upg;     /* upgrade backup area end address */
    td_u32 flash_addr_nv_file_work; /* nv workspace */
    td_u32 dfx_init_ret;
    td_u32 dfx_err_type;
    td_u32 dfx_start_refresh_nv : 1; /* whether nv is refreshed at power-on */
    td_u32 pad : 31;

    ext_trans_file_ctrl trans_ctrl;
} ext_upg_kernel_ctrl;

typedef struct {
    td_u32 param[4]; /* 4 count */
} ext_dfx_upg_cmd;

typedef struct {
    ext_upg_kernel_ctrl upg_ctrl;
} ext_dfx_upg_ind;

#endif
