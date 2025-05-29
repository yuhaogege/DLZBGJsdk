/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: nv implementation modification: implemented as 2 ping-pong,
 * and divided into factory area, and ordinary nv area soc_nvm.c code
 * Author: CompanyName
 * Create: 2012-08-26
 */
#include "soc_nvm.h"
#include <soc_errno.h>
#include <soc_mdm_crc.h>
#include <soc_mdm_mem.h>
#include <soc_mdm_sem.h>
#include <soc_mdm_flash.h>
#include <soc_mdm_nv.h>
#include <soc_mdm_task.h>
#include <soc_stdlib.h>
#include <soc_ft_nv.h>
#include <soc_hb_ver.h>

/* The maximum number of NV, codex rectification */
#define NV_TOTAL_NUM_MAX 256
typedef struct {
    ext_nvm_changed_notify_f func;
    td_u8 min_id;
    td_u8 max_id;
    td_u16 count;
} nvm_changed_proc_stru;
/* temp */
#define NV_BACKUP_ADDR 0x6000

/*
 * #define NV_PRINTF printf
 * #define NV_PRINTF
 */
static ext_nv_ctrl g_nv_ctrl[EXT_TYPE_NV_MAX];
static nvm_changed_proc_stru g_nv_change_notify_list[NVM_CHANGED_PROC_NUM];
td_u16 g_nv_change_notify_list_cnt = 0;

static td_u32 g_nv_sem_handle;

EXT_PRV td_u32 uapi_nv_get_sem_handle(td_u32 *handle)
{
    td_u32 ret;
    static td_bool bool_init = TD_FALSE;

    if (handle == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    if (bool_init != TD_TRUE) {
        ret = uapi_sem_bcreate(&g_nv_sem_handle, "nv_sem", EXT_SEM_ONE);
        if (ret != EXT_ERR_SUCCESS) {
            return EXT_ERR_FAILURE;
        }
        bool_init = TD_TRUE;
    }
    *handle = g_nv_sem_handle;

    return EXT_ERR_SUCCESS;
}

/* Fix the dst_data address corresponding to the nv file */
EXT_PRV td_u32 nv_repair_file(td_u8 *dst_data, td_u32 dst_len, const td_u8 *src_data, td_u32 src_len, td_u32 seq)
{
    ext_nv_manage *nv_head = (ext_nv_manage *)dst_data;

    if (memcpy_s(dst_data, dst_len, src_data, src_len) != EOK) {
        return EXT_ERR_FAILURE;
    }
    nv_head->seq = seq;
    nv_head->crc = uapi_crc32(0, dst_data + 8, HNV_CRC_SIZE); /* Remove the magic word and CRC8 bytes */
    return EXT_ERR_SUCCESS;
}

/* nv internal interface, first write dead address, then changed to read factory nv */
EXT_PRV td_u32 nv_repair_from_backup(td_u8 *pdata1, td_u32 flash_addr, td_u32 src_file_addr)
{
    td_u32 ret;

    ret = uapi_flash_read_in(src_file_addr, HNV_BLOCK_SIZE, pdata1);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    ret = uapi_flash_write_in(flash_addr, HNV_BLOCK_SIZE, pdata1, TD_TRUE);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }
    return ret;
}

/* Check the legality of nv files */
EXT_PRV td_u32 nv_check_file(const td_u8 *pdata, td_u32 len, ext_nv_type nv_type)
{
    ext_nv_manage *nv_head = TD_NULL;
    td_u32 magic = HNV_FILE_SIGNATURE;

    nv_head = (ext_nv_manage *)pdata;
    if (nv_type == EXT_TYPE_FACTORY_NV) {
        magic = FNV_FILE_SIGNATURE;
    }
    if (magic != nv_head->magic) {
        return EXT_ERR_INVALID_HEAP_ADDR;
    }
    if (nv_head->total_num > NV_TOTAL_NUM_MAX) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    if (nv_head->crc != uapi_crc32(0, pdata + 8, len - 8)) { /* Remove the magic word and crc total 8 bytes */
        return EXT_ERR_BAD_DATA;
    }
    return EXT_ERR_SUCCESS;
}

/* Returns the corresponding NV index */
EXT_PRV ext_nv_item_index *nv_find_item(const td_u8 *pdata, td_u8 id, ext_nv_type nv_type)
{
    ext_nv_manage *nv_head = TD_NULL;
    ext_nv_item_index *index = TD_NULL;
    td_u16 total_num;

    if (pdata == TD_NULL) {
        total_num = g_nv_ctrl[nv_type].total_num;
        index = g_nv_ctrl[nv_type].index;
    } else {
        nv_head = (ext_nv_manage *)pdata;
        total_num = nv_head->total_num;
        index = (ext_nv_item_index *)&nv_head->nv_item_data[0];
    }

    for (int i = 0; i < total_num; i++) {
        if (id == index[i].nv_id) {
            return &index[i];
        }
    }
    return (ext_nv_item_index *)TD_NULL;
}
EXT_PRV td_void nv_init_check_success_proc(td_u32 flash_addr, ext_nv_ctrl *nv_ctrl, const ext_nv_manage *nv_head1,
    const ext_nv_manage *nv_head2)
{
    ext_nv_manage *nv_head = TD_NULL;
    if (nv_head1->seq > nv_head2->seq) {
        nv_ctrl->current_addr = flash_addr;
        nv_ctrl->next_addr = flash_addr + HNV_BLOCK_SIZE;
        nv_head = (ext_nv_manage *)nv_head1;
    } else {
        nv_ctrl->current_addr = flash_addr + HNV_BLOCK_SIZE;
        nv_ctrl->next_addr = flash_addr;
        nv_head = (ext_nv_manage *)nv_head2;
    }
    nv_ctrl->total_num = nv_head->total_num;
    nv_ctrl->seq = nv_head->seq;
    nv_ctrl->index = (ext_nv_item_index *)&nv_head->nv_item_data[0];
    nv_ctrl->ver_magic = nv_head->ver_magic;
}
EXT_PRV td_u32 nv_init_repair_all(ext_nv_type nv_type, td_u8 *pdata1, td_u32 flash_addr, td_u32 source_file_addr,
    ext_nv_ctrl *nv_ctrl)
{
    td_u32 ret;

    /* Factory NV does not support repair */
    if (nv_type == EXT_TYPE_FACTORY_NV) {
        return EXT_ERR_NOT_SUPPORT;
    }

    ret = nv_repair_from_backup(pdata1, flash_addr, source_file_addr);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    nv_ctrl->current_addr = flash_addr;
    nv_ctrl->next_addr = flash_addr + HNV_BLOCK_SIZE;
    ext_nv_manage *nv_head = (ext_nv_manage *)pdata1;
    nv_ctrl->total_num = nv_head->total_num; /* Repeat too much, to be optimized */
    nv_ctrl->seq = nv_head->seq;
    nv_ctrl->index = (ext_nv_item_index *)&nv_head->nv_item_data[0];
    nv_ctrl->ver_magic = nv_head->ver_magic;

    return ret;
}
EXT_PRV td_u32 nv_init_repair_one_part(td_u32 check1, td_u32 flash_addr, ext_nv_ctrl *nv_ctrl, td_u8 *pdata1,
    td_u8 *pdata2)
{
    td_u8 *right_data = TD_NULL;
    td_u8 *to_repair_data = TD_NULL;
    ext_nv_manage *nv_head = TD_NULL;

    if (check1 != EXT_ERR_SUCCESS) {
        nv_ctrl->current_addr = flash_addr + HNV_BLOCK_SIZE;
        nv_ctrl->next_addr = flash_addr;
        right_data = pdata2;
        to_repair_data = pdata1;
        nv_head = (ext_nv_manage *)pdata2;
    } else {
        nv_ctrl->current_addr = flash_addr;
        nv_ctrl->next_addr = flash_addr + HNV_BLOCK_SIZE;
        right_data = pdata1;
        to_repair_data = pdata2;
        nv_head = (ext_nv_manage *)pdata1;
    }
    nv_ctrl->total_num = nv_head->total_num;
    nv_ctrl->seq = nv_head->seq;
    nv_ctrl->index = (ext_nv_item_index *)&nv_head->nv_item_data[0];
    nv_ctrl->ver_magic = nv_head->ver_magic;
    /* Data repair phase */
    nv_ctrl->seq += 1;
    td_u32 ret = nv_repair_file(to_repair_data, HNV_BLOCK_SIZE, right_data, HNV_BLOCK_SIZE, nv_ctrl->seq);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }
    ret = uapi_flash_write_in(nv_ctrl->next_addr, HNV_BLOCK_SIZE, to_repair_data, TD_TRUE);
    /* With the erasing is to waste memory */
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }
    td_u32 temp = nv_ctrl->current_addr;
    nv_ctrl->current_addr = nv_ctrl->next_addr;
    nv_ctrl->next_addr = temp;
    return EXT_ERR_SUCCESS;
}
EXT_PRV td_u32 nv_init_data_prepare_proc(td_u32 flash_addr, td_u8 **pdata1, td_u8 **pdata2)
{
    td_u32 ret;
    if (pdata1 == TD_NULL || pdata2 == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    td_u8 *data1 = uapi_malloc(EXT_MOD_ID_SAL_NVM, HNV_BLOCK_SIZE);
    if (data1 == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }
    ret = uapi_flash_read_in(flash_addr, HNV_BLOCK_SIZE, data1);
    if (ret != EXT_ERR_SUCCESS) {
        uapi_free(EXT_MOD_ID_SAL_NVM, data1);
        return ret;
    }
    /* Read and check the 2 area */
    td_u8 *data2 = uapi_malloc(EXT_MOD_ID_SAL_NVM, HNV_BLOCK_SIZE);
    if (data2 == TD_NULL) {
        uapi_free(EXT_MOD_ID_SAL_NVM, data1);
        return EXT_ERR_MALLOC_FAILUE;
    }
    ret = uapi_flash_read_in(flash_addr + HNV_BLOCK_SIZE, HNV_BLOCK_SIZE, data2);
    if (ret != EXT_ERR_SUCCESS) {
        uapi_free(EXT_MOD_ID_SAL_NVM, data1);
        uapi_free(EXT_MOD_ID_SAL_NVM, data2);
        return ret;
    }
    *pdata1 = data1;
    *pdata2 = data2;
    return EXT_ERR_SUCCESS;
}

EXT_PRV td_u32 nv_init_malloc_item_index(ext_nv_ctrl *nv_ctrl)
{
    td_u32 index_size = 0;
    if (nv_ctrl->total_num > NV_TOTAL_NUM_MAX) {
        return EXT_ERR_MALLOC_FAILUE;
    }
    /* Index space application */
    if (uint_2_multiply((td_u32)sizeof(ext_nv_item_index), (td_u32)nv_ctrl->total_num, &index_size) == TD_FALSE) {
        return EXT_ERR_FAILURE;
    }
    td_u8 *index = uapi_malloc(EXT_MOD_ID_SAL_NVM, index_size);
    if (index == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }
    if (memcpy_s(index, index_size, nv_ctrl->index, (td_u32)(sizeof(ext_nv_item_index) * nv_ctrl->total_num)) != EOK) {
        uapi_free(EXT_MOD_ID_SAL_NVM, index);
        return EXT_ERR_FAILURE;
    }
    nv_ctrl->index = (ext_nv_item_index *)index;
    nv_ctrl->init_flag = TD_TRUE;

    return EXT_ERR_SUCCESS;
}

EXT_PRV td_u32 nv_init_common_data(td_u32 work_addr, td_u32 src_file_addr, ext_nv_type nv_type, ext_nv_ctrl *nv_ctrl)
{
    /* Data preparation phase */
    td_u8 *pdata1 = TD_NULL;
    /* Read and check the 2 area */
    td_u8 *pdata2 = TD_NULL;
    td_u32 ret = nv_init_data_prepare_proc(work_addr, &pdata1, &pdata2);
    if (ret != EXT_ERR_SUCCESS || pdata2 == TD_NULL || pdata1 == TD_NULL) {
        return ret;
    }
    /* Data inspection phase */
    td_u32 check1 = nv_check_file(pdata1, HNV_BLOCK_SIZE, nv_type);
    td_u32 check2 = nv_check_file(pdata2, HNV_BLOCK_SIZE, nv_type);
    if ((check1 == EXT_ERR_SUCCESS) && (check2 == EXT_ERR_SUCCESS)) {
        nv_init_check_success_proc(work_addr, nv_ctrl, (ext_nv_manage *)pdata1, (ext_nv_manage *)pdata2);
    } else if ((check1 != EXT_ERR_SUCCESS) && (check2 != EXT_ERR_SUCCESS)) {
        ret = nv_init_repair_all(nv_type, pdata1, work_addr, src_file_addr, nv_ctrl);
        if (ret != EXT_ERR_SUCCESS) {
            goto POSITION2;
        }
    } else {
        ret = nv_init_repair_one_part(check1, work_addr, nv_ctrl, pdata1, pdata2);
        if (ret != EXT_ERR_SUCCESS) {
            goto POSITION2;
        }
    }
    ret = nv_init_malloc_item_index(nv_ctrl);

    /* The original backup is not considered for repair */
POSITION2:
    uapi_free(EXT_MOD_ID_SAL_NVM, pdata2);
    uapi_free(EXT_MOD_ID_SAL_NVM, pdata1);
    return ret;
}

td_u32 nv_init_common(td_u32 work_addr, td_u32 src_file_addr, ext_nv_type nv_type)
{
    if ((nv_type >= EXT_TYPE_NV_MAX) || (work_addr & (HNV_BLOCK_SIZE - 1)) || (src_file_addr & (HNV_BLOCK_SIZE - 1))) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    ext_nv_ctrl *nv_ctrl = &g_nv_ctrl[nv_type];
    if (nv_ctrl->init_flag != TD_TRUE) {
        td_u32 ret = uapi_nv_get_sem_handle(&nv_ctrl->sem_handle);
        if (ret != EXT_ERR_SUCCESS) {
            return ret;
        }
    }

    return nv_init_common_data(work_addr, src_file_addr, nv_type, nv_ctrl);
}

EXT_PRV td_u8 nv_get_idx_len(td_u32 id, ext_nv_type nv_type)
{
    ext_nv_item_index *nv_index = TD_NULL;

    nv_index = nv_find_item(TD_NULL, (td_u8)id, nv_type);
    if (nv_index == TD_NULL) {
        return 0;
    }
    return nv_index->nv_len;
}
td_u32 nv_read_common(td_u8 id, td_pvoid pdata, td_u8 len, ext_nv_type nv_type)
{
    td_u32 ret;
    td_u32 i;
    ext_nv_ctrl *nv_ctrl = TD_NULL;
    ext_nv_item_index *nv_index = TD_NULL;
    td_u32 crc = 0;
    td_u8 item_len;
    td_bool to_check_crc;

    /* Parameter judgment */
    if (nv_type >= EXT_TYPE_NV_MAX) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    nv_ctrl = &g_nv_ctrl[nv_type];
    nv_index = nv_find_item(TD_NULL, id, nv_type);
    if (nv_index == TD_NULL) {
        return EXT_ERR_NV_ERROR_READ;
    }
    if (nv_index->nv_len == len) {
        item_len = len;
        to_check_crc = TD_TRUE;
    } else {
        item_len = uapi_min(nv_index->nv_len, len);
        to_check_crc = TD_FALSE;
    }

    /* Allow trial and error */
    for (i = 0; i < HNV_FAULT_TOLERANT_TIMES; i++) {
        ret = uapi_flash_read_in(nv_ctrl->current_addr + nv_index->nv_offset, item_len, pdata);
        if (ret != EXT_ERR_SUCCESS) {
            continue;
        }
        /* Verify crc  when the length is equal */
        if (to_check_crc == TD_TRUE) {
            ret = uapi_flash_read_in(nv_ctrl->current_addr + nv_index->nv_offset + item_len, sizeof(crc),
                (td_u8 *)&crc);
            if (ret != EXT_ERR_SUCCESS || crc != uapi_crc32(0, pdata, item_len)) {
                continue;
            }
        }
        break;
    }
    if (i >= HNV_FAULT_TOLERANT_TIMES) {
        ret = EXT_ERR_NV_FAIL_N_TIMES;
    }

    return ret;
}

EXT_PRV td_u32 nv_changed_handle(td_u8 id)
{
    td_u32 ret = EXT_ERR_NOT_FOUND;
    nvm_changed_proc_stru *notify_list = g_nv_change_notify_list;
    td_u8 i;

    for (i = 0; i < NVM_CHANGED_PROC_NUM; i++) {
        if ((id >= notify_list[i].min_id) && (id <= notify_list[i].max_id)) {
            if (notify_list[i].func != TD_NULL) {
                notify_list[i].func(id);
                ret = EXT_ERR_SUCCESS;
            }
            continue;
        }
    }

    return ret;
}

td_u32 nv_sector_write(const td_u8 *nv_file, ext_nv_type nv_type)
{
    td_u32 ret;
    ext_nv_ctrl *nv_ctrl = &g_nv_ctrl[nv_type];
    ext_nv_manage *nv_head = (ext_nv_manage *)nv_file;
    td_u32 i;

    /* replace the version number, seq, crc */
    nv_head->ver_magic = uapi_ger_ver_magic();
    nv_head->seq = nv_ctrl->seq + 1;
    nv_head->crc = uapi_crc32(0, nv_file + 8, HNV_BLOCK_SIZE - 8); /* Remove the magic word and crc total 8 bytes */

    /* write NV */
    for (i = 0; i < HNV_FAULT_TOLERANT_TIMES; i++) {
        ret = uapi_flash_write(nv_ctrl->next_addr, HNV_BLOCK_SIZE, nv_file, TD_TRUE);
        if (ret == EXT_ERR_SUCCESS) {
            break;
        }
    }

    /* Update the NV control structure */
    if (ret == EXT_ERR_SUCCESS) {
        nv_ctrl->seq++;
        td_u32 temp = nv_ctrl->current_addr;
        nv_ctrl->current_addr = nv_ctrl->next_addr;
        nv_ctrl->next_addr = temp;
    }
    return ret;
}
EXT_PRV td_u32 nv_write_proc(ext_nv_ctrl *nv_ctrl, td_u8 *nv_file, const td_pvoid pdata, td_u8 item_len,
    const ext_nv_item_index *nv_index)
{
    td_u8 i;
    ext_nv_manage *nv_head = TD_NULL;

    for (i = 0; i < HNV_FAULT_TOLERANT_TIMES; i++) {
        if (uapi_flash_read_in(nv_ctrl->current_addr, HNV_BLOCK_SIZE, nv_file) != EXT_ERR_SUCCESS) {
            continue;
        }
        /* Update data with two crc and seq */
        td_u32 crc = uapi_crc32(0, pdata, item_len);
        if (memcpy_s(nv_file + nv_index->nv_offset, HNV_BLOCK_SIZE - nv_index->nv_offset, pdata, item_len) != EOK) {
            continue;
        }
        if (memcpy_s(nv_file + nv_index->nv_offset + item_len, HNV_BLOCK_SIZE - (nv_index->nv_offset + item_len), &crc,
            sizeof(crc)) != EOK) {
            continue;
        }
        nv_head = (ext_nv_manage *)nv_file;
        nv_head->seq += 1;
        nv_head->crc = uapi_crc32(0, nv_file + 8, HNV_BLOCK_SIZE - 8); /* Remove the magic word and crc total 8 bytes */
        if (uapi_flash_write_in(nv_ctrl->next_addr, HNV_BLOCK_SIZE, nv_file, TD_TRUE) != EXT_ERR_SUCCESS) {
            continue;
        }
        break;
    }
    /* global information refresh */
    td_u32 temp = nv_ctrl->current_addr;
    nv_ctrl->current_addr = nv_ctrl->next_addr;
    nv_ctrl->next_addr = temp;
    if (nv_head != TD_NULL) {
        nv_ctrl->seq = nv_head->seq;
    }
    if (i >= HNV_FAULT_TOLERANT_TIMES) {
        uapi_free(EXT_MOD_ID_SAL_NVM, nv_file);
        return EXT_ERR_NV_FAIL_N_TIMES;
    }
    return EXT_ERR_SUCCESS;
}
td_u32 nv_write_common(td_u8 id, const td_pvoid pdata, td_u8 len, ext_nv_type nv_type)
{
    td_u8 item_len = len;
    td_u8 backup_num = 1;
    if (nv_type >= EXT_TYPE_NV_MAX) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    ext_nv_ctrl *nv_ctrl = &g_nv_ctrl[nv_type];
    /* If you read back equal, don't write */
    td_u8 *item_readback = uapi_malloc(EXT_MOD_ID_SAL_NVM, HNV_ITEM_MAXLEN);
    if (item_readback == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }
    td_u32 ret = nv_read_common(id, item_readback, len, nv_type);
    if (ret != EXT_ERR_SUCCESS) {
        uapi_free(EXT_MOD_ID_SAL_NVM, item_readback);
        return ret;
    }
    if (memcmp(pdata, item_readback, len) == 0) {
        uapi_free(EXT_MOD_ID_SAL_NVM, item_readback);
        return EXT_ERR_SUCCESS;
    }
    uapi_free(EXT_MOD_ID_SAL_NVM, item_readback);
    /* Regular writing process */
    ext_nv_item_index *nv_index = nv_find_item(TD_NULL, id, nv_type);
    if (nv_index == TD_NULL) {
        return EXT_ERR_NV_ERROR_READ;
    }
    /* Length processing */
    if (nv_index->nv_len != len) {
        item_len = uapi_min(nv_index->nv_len, len);
    }
    /* read back 4k replacement and re-do crc */
    td_u8 *nv_file = uapi_malloc(EXT_MOD_ID_SAL_NVM, HNV_BLOCK_SIZE);
    if (nv_file == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }
    if (nv_type == EXT_TYPE_FACTORY_NV) {
        backup_num = 2; /* Factory area operations back up each other, write 2 copies */
    }
    for (; backup_num > 0; backup_num--) {
        ret = nv_write_proc(nv_ctrl, nv_file, pdata, item_len, nv_index);
        if (ret != EXT_ERR_SUCCESS) {
            return ret;
        }
    }
    uapi_free(EXT_MOD_ID_SAL_NVM, nv_file);
    return EXT_ERR_SUCCESS;
}
td_u32 uapi_nv_get_ver_magic(td_void)
{
    return g_nv_ctrl[EXT_TYPE_NV].ver_magic;
}
td_u32 uapi_nv_init(td_u32 addr, td_u32 backup_addr)
{
    return nv_init_common(addr, backup_addr, EXT_TYPE_NV);
}

td_u32 uapi_nv_sector_write(td_u8 *nv_file)
{
    return nv_sector_write(nv_file, EXT_TYPE_NV);
}

/* External interface, generally used for upgrade, check the legality of nv original file */
td_u32 uapi_nv_check_file(td_u32 flash_addr)
{
    td_u32 ret;
    td_u8 *data = NULL;

    data = uapi_malloc(EXT_MOD_ID_SAL_NVM, HNV_BLOCK_SIZE);
    if (data == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }
    ret = uapi_flash_read_in(flash_addr, HNV_BLOCK_SIZE, data);
    if (ret != EXT_ERR_SUCCESS) {
        uapi_free(EXT_MOD_ID_SAL_NVM, data);
        return ret;
    }
    ret = nv_check_file(data, HNV_BLOCK_SIZE, EXT_TYPE_NV);
    if (ret != EXT_ERR_SUCCESS) {
        uapi_free(EXT_MOD_ID_SAL_NVM, data);
        return ret;
    }
    uapi_free(EXT_MOD_ID_SAL_NVM, data);

    return EXT_ERR_SUCCESS;
}

td_u32 uapi_nv_flush_keep_ids(td_u8 *addr, td_u32 len)
{
    ext_nv_item_index *index = TD_NULL; /* destination address corresponds to NV index */
    td_u32 ret;
    td_u16 i;
    ext_nv_item_index *index_item = TD_NULL;
    td_u8 *temp_data = TD_NULL;
    td_u32 crc;
    if (addr == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    ext_nv_ctrl *nv_ctrl = &g_nv_ctrl[EXT_TYPE_NV];
    if (nv_ctrl->init_flag != TD_TRUE) {
        return EXT_ERR_INITILIZATION;
    }
    ret = nv_check_file(addr, len, EXT_TYPE_NV);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }
    ext_nv_manage *nv_head = (ext_nv_manage *)addr;
    index = (ext_nv_item_index *)&nv_head->nv_item_data[0];
    if (nv_head->keep_id_range[0] > nv_head->keep_id_range[1]) {
        return EXT_ERR_NOT_SUPPORT_ID;
    }
    temp_data = uapi_malloc(EXT_MOD_ID_SAL_NVM, 256); /* 256 bytes is enough to use */
    if (temp_data == TD_NULL) {
        return EXT_ERR_NOT_ENOUGH_MEMORY;
    }
    for (i = 0; i < nv_head->total_num; i++) {
        index_item = &index[i];
        if (index_item->nv_id >= nv_head->keep_id_range[0] && index_item->nv_id <= nv_head->keep_id_range[1]) {
            ret = nv_read_common(index_item->nv_id, temp_data, index_item->nv_len, EXT_TYPE_NV);
            if (ret != EXT_ERR_SUCCESS) {
                continue;
            }
            td_u8 nv_len = nv_get_idx_len(index_item->nv_id, EXT_TYPE_NV);
            errno_t sret = memcpy_s((addr + index_item->nv_offset), index_item->nv_len, temp_data,
                uapi_min(index_item->nv_len, nv_len));
            if (sret == 0) {
                crc = uapi_crc32(0, addr + index_item->nv_offset, index_item->nv_len);
                *(td_u32 *)(addr + (index_item->nv_offset + index_item->nv_len)) = crc;
            }
        }
    }
    uapi_free(EXT_MOD_ID_SAL_NVM, temp_data);
    crc = uapi_crc32(0, addr + 8, nv_head->flash_size - 8); /* Remove the magic word and crc total 8 bytes */
    *(td_u32 *)(addr + 4) = crc;                          /* Offset magic word 4 bytes */
    return EXT_ERR_SUCCESS;
}

/* Factory NV position is fixed, and two pieces are close to each other */
/* addr is passed in by boot, opt is pending */
td_u32 uapi_factory_nv_init(td_u32 addr)
{
    td_u32 ret;
    static td_bool b_init = TD_FALSE;

    if (b_init == TD_TRUE) {
        return EXT_ERR_SUCCESS;
    }

    ret = nv_init_common(addr, 0, EXT_TYPE_FACTORY_NV);
    if (ret == EXT_ERR_SUCCESS) {
        b_init = TD_TRUE;
    }

    return ret;
}

EXT_PRV td_u32 uapi_comm_nv_check_data(const td_u8 id,
    const td_pvoid pdata,
    const td_u8 len,
    const ext_nv_type nv_type)
{
    EXT_PRV const ext_nv_allocte_info nv_allocate_tbl[] = {
        {EXT_TYPE_NV, EXT_NV_NORMAL_ID_START, EXT_NV_NORMAL_USR_ID_END, 0},
        {EXT_TYPE_FACTORY_NV, EXT_NV_FACTORY_ID_START, EXT_NV_FACTORY_USR_ID_END, 0}
    };

    td_u32 loop;
    for (loop = 0; loop < ext_array_count(nv_allocate_tbl); loop++) {
        if (nv_type == nv_allocate_tbl[loop].nv_type) {
            if ((id >= nv_allocate_tbl[loop].start_id) &&
                (id <= nv_allocate_tbl[loop].end_id)) {
                break;
            }
        }
    }

    if (loop == ext_array_count(nv_allocate_tbl)) {
        return EXT_ERR_NOT_SUPPORT;
    }

    if ((pdata == TD_NULL) || (len == 0) || (len > HNV_ITEM_MAXLEN)) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    return EXT_ERR_SUCCESS;
}

td_u32 uapi_nv_pipeline_entry(const td_u8 id, td_pvoid pdata, td_u8 len, const ext_nv_pipeline_ctrl* ctrl)
{
    td_u32 ret;
    const ext_nv_ctrl *nv_ctrl = &g_nv_ctrl[ctrl->nv_type];

    if (nv_ctrl->init_flag != TD_TRUE) {
        return EXT_ERR_INITILIZATION;
    }

    ret = uapi_comm_nv_check_data(id, pdata, len, ctrl->nv_type);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    ret = uapi_sem_wait(nv_ctrl->sem_handle, EXT_SYS_WAIT_FOREVER);
    if (ret != EXT_ERR_SUCCESS) {
        return EXT_ERR_SEM_WAIT_FAIL;
    }

    if (ctrl->nv_operate == EXT_NV_WRITE_OPERATE) {
        ret = nv_write_common(id, pdata, len, ctrl->nv_type);
    } else {
        ret = nv_read_common(id, pdata, len, ctrl->nv_type);
    }

    uapi_sem_signal(nv_ctrl->sem_handle);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    if (ctrl->notify_enable == TD_TRUE) {
        ret = nv_changed_handle(id);
    }

    return EXT_ERR_SUCCESS;
}

td_u32 uapi_nv_write(td_u8 id, const td_pvoid pdata, td_u8 len)
{
    EXT_PRV const ext_nv_pipeline_ctrl ctrl = {EXT_TYPE_NV, EXT_NV_WRITE_OPERATE, TD_TRUE, 0};

    return uapi_nv_pipeline_entry(id, (td_pvoid)pdata, len, &ctrl);
}

td_u32 uapi_factory_nv_write(td_u8 id, const td_pvoid pdata, td_u8 len)
{
    EXT_PRV const ext_nv_pipeline_ctrl ctrl = {EXT_TYPE_FACTORY_NV, EXT_NV_WRITE_OPERATE, TD_FALSE, 0};

    return uapi_nv_pipeline_entry(id, (td_pvoid)pdata, len, &ctrl);
}

td_u32 uapi_nv_read(td_u8 id, td_pvoid pdata, td_u8 len)
{
    EXT_PRV const ext_nv_pipeline_ctrl ctrl = {EXT_TYPE_NV, EXT_NV_READ_OPERATE, TD_FALSE, 0};

    return uapi_nv_pipeline_entry(id, (td_pvoid)pdata, len, &ctrl);
}

td_u32 uapi_factory_nv_read(td_u8 id, td_pvoid pdata, td_u8 len)
{
    EXT_PRV const ext_nv_pipeline_ctrl ctrl = {EXT_TYPE_FACTORY_NV, EXT_NV_READ_OPERATE, TD_FALSE, 0};

    return uapi_nv_pipeline_entry(id, (td_pvoid)pdata, len, &ctrl);
}

/* temporary interface */
td_u8 uapi_nv_len(td_u8 id)
{
    ext_nv_item_index *nv_index = TD_NULL;
    ext_nv_type nv_type;

    if (uapi_is_nv_ftm((td_u16)id)) {
        nv_type = EXT_TYPE_FACTORY_NV;
    } else {
        nv_type = EXT_TYPE_NV;
    }

    nv_index = nv_find_item(TD_NULL, id, nv_type);
    if (nv_index != TD_NULL) {
        return nv_index->nv_len;
    } else {
        return 0;
    }
}

td_u32 uapi_nv_register_change_nofity_proc(td_u8 min_id, td_u8 max_id, ext_nvm_changed_notify_f func)
{
    td_u32 ret;
    ext_nv_ctrl *nv_ctrl = TD_NULL;
    nvm_changed_proc_stru *notify_list = g_nv_change_notify_list;
    td_u8 i;

    if ((min_id > max_id) || (func == TD_NULL)) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    nv_ctrl = &g_nv_ctrl[EXT_TYPE_NV];
    ret = uapi_sem_wait(nv_ctrl->sem_handle, EXT_SYS_WAIT_FOREVER);
    if (ret != EXT_ERR_SUCCESS) {
        return EXT_ERR_SEM_WAIT_FAIL;
    }
    for (i = 0; i < NVM_CHANGED_PROC_NUM; i++) {
        if (notify_list[i].func == TD_NULL) {
            notify_list[i].min_id = min_id;
            notify_list[i].max_id = max_id;
            notify_list[i].func = func;
            g_nv_change_notify_list_cnt++;
            ret = EXT_ERR_SUCCESS;
            break;
        }
    }
    if (i >= NVM_CHANGED_PROC_NUM) {
        ret = EXT_ERR_FULL;
    }
    uapi_sem_signal(nv_ctrl->sem_handle);

    return ret;
}

td_u32 uapi_nv_soft_reset(td_u32 work_addr, td_u32 backup_addr)
{
    td_u32 ret;
    ext_nv_ctrl *nv_ctrl = TD_NULL;

    nv_ctrl = &g_nv_ctrl[EXT_TYPE_NV];
    if (nv_ctrl->init_flag != TD_TRUE) {
        return EXT_ERR_SUCCESS; /* Not initialized, no reset is required, returning success */
    }
    uapi_free(EXT_MOD_ID_SAL_NVM, nv_ctrl->index);
    uapi_sem_delete(nv_ctrl->sem_handle);
    nv_ctrl->init_flag = TD_FALSE;
    ret = nv_init_common(work_addr, backup_addr, EXT_TYPE_NV);
    if (ret != EXT_ERR_SUCCESS) {
        return EXT_ERR_FAILURE;
    }

    return EXT_ERR_SUCCESS;
}
