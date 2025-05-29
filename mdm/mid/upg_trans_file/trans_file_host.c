/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Upgrade - Transmit file handling.
 * Author: CompanyName
 * Create: 2018-10-31
 */

#include "trans_file_host.h"
#include <soc_partition_table.h>
#include <soc_mdm_flash.h>
#include <soc_sal_nv.h>
#include <soc_mdm_nv.h>
#include <soc_21_upg.h>

#ifdef __cplusplus
extern "C" {
#endif

td_u32 g_trans_file_host_file_st_addr = 0;
td_u32 g_trans_file_host_file_size = 0;

td_void uapi_trans_host_stop_restore(td_void)
{
    /* Set back to default to avoid conflict between APP and DBK */
    g_trans_file_host_file_st_addr = 0;
    g_trans_file_host_file_size = 0;
}

/* call this interface before transmiting file */
td_u32 uapi_trans_host_prepare(upg_trans_file_type file_type, td_u32 file_len, td_u32 crc)
{
    ext_flash_partition_table *partion_tbl = uapi_get_partition_table();
    ext_flash_partition_info *table = &partion_tbl->table[EXT_FLASH_PARTITON_UPG_BACKUP];
    td_u32 tail_flash_addr = 0;
    if (uint_2_multiply(table->addr_blk + table->size_blk, BLOCK_SIZE, &tail_flash_addr) == TD_FALSE) {
        return EXT_ERR_FAILURE;
    }
    ext_trans_host_nv nv = { 0 };

    if (file_type == UPG_TRANS_FILE_CCO_TO_STA) {
        if (g_trans_file_host_file_st_addr == 0) {
            /* APP does NOT modify start address, but DBK does */
            g_trans_file_host_file_st_addr = tail_flash_addr - file_len - TRANS_OTHER_FILE_OFFSET;
        }

        g_trans_file_host_file_size = file_len;
    } else {
        if (g_trans_file_host_file_st_addr == 0) {
            g_trans_file_host_file_st_addr = (td_u32)(tail_flash_addr - sizeof(ext_21_upg_tail_new) - file_len);
        }

        g_trans_file_host_file_size = file_len;
    }

    nv.addr = g_trans_file_host_file_st_addr;
    nv.size = g_trans_file_host_file_size;
    nv.crc = crc;

    return uapi_nv_write(EXT_NV_SYS_TRANS_HOST, &nv, sizeof(nv));
}

td_u32 uapi_trans_host_get_content(td_u32 offset, td_u32 size, td_u8 *buffer)
{
    return uapi_flash_read(g_trans_file_host_file_st_addr + offset, size, buffer);
}

td_u32 uapi_trans_host_restart_continue_init(td_void)
{
    ext_trans_host_nv nv = { 0 };
    td_u32 ret;

    ret = uapi_nv_read(EXT_NV_SYS_TRANS_HOST, &nv, sizeof(ext_trans_host_nv));
    if (ret == EXT_ERR_SUCCESS) {
        g_trans_file_host_file_st_addr = nv.addr;
        g_trans_file_host_file_size = nv.size;
    }

    return ret;
}

#ifdef __cplusplus
}
#endif
