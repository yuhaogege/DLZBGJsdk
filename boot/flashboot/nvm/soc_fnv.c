/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: factory nv
 * Author: CompanyName
 * Create: 2018-10-31
 */

#include <soc_fnv.h>
#include <soc_flashboot_flash.h>
#include <soc_boot_rom.h>
#include <crc32.h>

#ifdef DEBUG_FNV
#define nv_print_f0(s) boot_msg0(s)
#define nv_print_f1(s, v1) boot_msg1(s, v1)
#define nv_print_f2(s, v1, v2) boot_msg2(s, v1, v2)
#else
#define nv_print_f0(s)
#define nv_print_f1(s, v1)
#define nv_print_f2(s, v1, v2)
#endif

/* maximum count of nv ( modified by codex) */
#define NV_TOTAL_NUM_MAX 256

/* factory and normal seperated */
static ext_nv_ctrl g_nv_ctrl[EXT_TYPE_NV_MAX] = {
    0,
};

/* repair the nv file correspondence with dst_data address */
EXT_PRV td_void nv_repair_file(td_u8 *dst_data, const td_u8 *src_data, td_u32 len, td_u32 seq)
{
    ext_nv_manage *nv_head = (ext_nv_manage *)dst_data;

    if (memcpy_s(dst_data, HNV_BLOCK_SIZE, src_data, len) != EXT_ERR_SUCCESS) {
        return;
    }
    nv_head->seq = seq;
    nv_head->crc = uapi_crc32(0, dst_data + 8, HNV_CRC_SIZE); /* data shifts 8 */
}

/* nv inner interface: write a fixed address, then change to read from factory nv */
EXT_PRV td_u32 nv_repair_from_backup(td_u8 *pdata1, td_u32 flash_addr, td_u32 src_file_addr)
{
    td_u32 ret;

    ret = uapi_flash_read(src_file_addr, HNV_BLOCK_SIZE, pdata1);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    ret = uapi_flash_erase(flash_addr, HNV_BLOCK_SIZE);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }
    ret = uapi_flash_write(flash_addr, HNV_BLOCK_SIZE, pdata1, TD_FALSE);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }
    return ret;
}

/* check the legality of nv file */
EXT_PRV td_u32 nv_check_file(const td_u8 *pdata, td_u32 len, ext_nv_type nv_type)
{
    ext_nv_manage *nv_head = TD_NULL;
    td_u32 magic = FNV_FILE_SIGNATURE;

    nv_head = (ext_nv_manage *)pdata;
    if (nv_type != EXT_TYPE_FACTORY_NV) {
        magic = HNV_FILE_SIGNATURE;
    }

    if (magic != nv_head->magic) {
        return EXT_ERR_MAGIC_CHECK;
    }

    if (nv_head->total_num > NV_TOTAL_NUM_MAX) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    if (nv_head->crc != uapi_crc32(0, pdata + 8, len - 8)) { /* shifts 8 */
        return EXT_ERR_CRC_CHECK;
    }

    return EXT_ERR_SUCCESS;
}

/* return nv index */
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

/* stage of data prepare */
EXT_PRV td_u32 nv_init_data_prepare(td_u8 **pdata, td_u32 flash_addr)
{
    td_u32 ret;
    td_u8 *data = TD_NULL;

    data = boot_malloc(HNV_BLOCK_SIZE);
    if (data == TD_NULL) {
        ret = EXT_ERR_MALLOC_FAILUE;
        return ret;
    }

    ret = uapi_flash_read(flash_addr, HNV_BLOCK_SIZE, data);
    if (ret != EXT_ERR_SUCCESS) {
        boot_free(data);
        return ret;
    }

    *pdata = data;
    return ret;
}
EXT_PRV td_void nv_init_data_check_all_success(const td_u8 *pdata1, const td_u8 *pdata2, td_u32 flash_addr,
    ext_nv_ctrl *nv_ctrl)
{
    ext_nv_manage *nv_head1 = (ext_nv_manage *)pdata1;
    ext_nv_manage *nv_head2 = (ext_nv_manage *)pdata2;
    ext_nv_manage *nv_head = TD_NULL;

    if (nv_head1->seq > nv_head2->seq) {
        nv_ctrl->current_addr = flash_addr;
        nv_ctrl->next_addr = flash_addr + HNV_BLOCK_SIZE;
        nv_head = nv_head1;
    } else {
        nv_ctrl->current_addr = flash_addr + HNV_BLOCK_SIZE;
        nv_ctrl->next_addr = flash_addr;
        nv_head = nv_head2;
    }

    nv_ctrl->total_num = nv_head->total_num;
    nv_ctrl->seq = nv_head->seq;
    nv_ctrl->index = (ext_nv_item_index *)&nv_head->nv_item_data[0];
}
EXT_PRV td_u32 nv_init_data_check_all_fail(td_u32 work_addr, td_u32 source_file_addr, ext_nv_type nv_type,
    ext_nv_ctrl *nv_ctrl, td_u8 *pdata1)
{
    td_u32 ret;
    td_u32 flash_addr = work_addr;
    ext_nv_manage *nv_head = TD_NULL;
    ext_nv_manage *nv_head1 = (ext_nv_manage *)pdata1;
    if (nv_type != EXT_TYPE_FACTORY_NV) {
        ret = nv_repair_from_backup(pdata1, work_addr, source_file_addr);
        if (ret != EXT_ERR_SUCCESS) {
            return ret;
        }

        nv_ctrl->current_addr = flash_addr;
        nv_ctrl->next_addr = flash_addr + HNV_BLOCK_SIZE;
        nv_head = nv_head1;
        nv_ctrl->total_num = nv_head->total_num; /* to be optimized :  too much repetition */
        nv_ctrl->seq = nv_head->seq;
        nv_ctrl->index = (ext_nv_item_index *)&nv_head->nv_item_data[0];
    } else {
        /* factory nv unsupport repair */
        ret = EXT_ERR_NOT_SUPPORT;
        return ret;
    }

    return ret;
}
EXT_PRV td_u32 nv_init_data_check_once_fail(td_u8 *pdata1, td_u32 check1, td_u8 *pdata2, td_u32 flash_addr,
    ext_nv_ctrl *nv_ctrl)
{
    td_u32 ret;
    td_u32 temp;
    td_u8 *to_repair_data = TD_NULL;
    td_u8 *right_data = TD_NULL;
    ext_nv_manage *nv_head = TD_NULL;
    ext_nv_manage *nv_head1 = (ext_nv_manage *)pdata1;
    ext_nv_manage *nv_head2 = (ext_nv_manage *)pdata2;
    if (check1 != EXT_ERR_SUCCESS) {
        nv_ctrl->current_addr = flash_addr + HNV_BLOCK_SIZE;
        nv_ctrl->next_addr = flash_addr;

        right_data = pdata2;
        to_repair_data = pdata1;

        nv_head = nv_head2;
    } else {
        nv_ctrl->current_addr = flash_addr;
        nv_ctrl->next_addr = flash_addr + HNV_BLOCK_SIZE;

        right_data = pdata1;
        to_repair_data = pdata2;

        nv_head = nv_head1;
    }

    nv_ctrl->total_num = nv_head->total_num;
    nv_ctrl->seq = nv_head->seq;
    nv_ctrl->index = (ext_nv_item_index *)&nv_head->nv_item_data[0];

    /* stage of data repair */
    nv_ctrl->seq += 1;
    nv_repair_file(to_repair_data, right_data, HNV_BLOCK_SIZE, nv_ctrl->seq);
    ret = uapi_flash_erase(nv_ctrl->next_addr, HNV_BLOCK_SIZE);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    ret = uapi_flash_write(nv_ctrl->next_addr, HNV_BLOCK_SIZE, to_repair_data, TD_FALSE);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    temp = nv_ctrl->current_addr;
    nv_ctrl->current_addr = nv_ctrl->next_addr;
    nv_ctrl->next_addr = temp;
    return ret;
}
/* stage of data check */
EXT_PRV td_u32 nv_init_data_check(td_u32 work_addr, td_u32 source_file_addr, ext_nv_type nv_type, ext_nv_ctrl *nv_ctrl,
    td_u8 *data[2]) /* length 2 */
{
    td_u32 check1, check2;
    td_u32 ret = EXT_ERR_SUCCESS;
    td_u32 flash_addr = work_addr;

    if ((data[0] == TD_NULL) || (data[1] == TD_NULL)) {
        return EXT_ERR_FAILURE;
    }

    check1 = nv_check_file(data[0], HNV_BLOCK_SIZE, nv_type);
    check2 = nv_check_file(data[1], HNV_BLOCK_SIZE, nv_type);
    if ((check1 == EXT_ERR_SUCCESS) && (check2 == EXT_ERR_SUCCESS)) {
        nv_init_data_check_all_success(data[0], data[1], flash_addr, nv_ctrl);
    } else if ((check1 != EXT_ERR_SUCCESS) && (check2 != EXT_ERR_SUCCESS)) {
        ret = nv_init_data_check_all_fail(work_addr, source_file_addr, nv_type, nv_ctrl, data[0]);
    } else {
        ret = nv_init_data_check_once_fail(data[0], check1, data[1], flash_addr, nv_ctrl);
    }

    return ret;
}

EXT_PRV td_u32 nv_init_index_space(ext_nv_ctrl *nv_ctrl)
{
    td_u32 ret = EXT_ERR_SUCCESS;
    td_u32 index_size;
    td_u8 *index = TD_NULL;
    if (uint_2_multiply((td_u32)sizeof(ext_nv_item_index), (td_u32)nv_ctrl->total_num, &index_size) == TD_FALSE) {
        return EXT_ERR_FAILURE;
    }
    index = boot_malloc(index_size);
    if (index == TD_NULL) {
        ret = EXT_ERR_MALLOC_FAILUE;
        return ret;
    }

    if (memcpy_s(index, index_size, nv_ctrl->index,
            sizeof(ext_nv_item_index) * nv_ctrl->total_num) != EXT_ERR_SUCCESS) {
        nv_print_f0("memcpy_s fail.");
    }

    nv_ctrl->index = (ext_nv_item_index *)index;
    nv_ctrl->init_flag = TD_TRUE;
    return ret;
}
td_u32 nv_init_common(td_u32 work_addr, td_u32 source_file_addr, ext_nv_type nv_type)
{
    td_u32 ret;
    ext_nv_ctrl *nv_ctrl = &g_nv_ctrl[nv_type];
    td_u8 *pdata1 = TD_NULL;
    td_u8 *pdata2 = TD_NULL;
    td_u32 flash_addr = work_addr;

    if (nv_ctrl->init_flag == TD_TRUE) {
        ret = EXT_ERR_INIT_ALREADY;
        return ret;
    }

    /* stage of data prepare */
    ret = nv_init_data_prepare(&pdata1, flash_addr);
    if (ret != EXT_ERR_SUCCESS) {
        if (pdata1 != TD_NULL) {
            boot_free(pdata1);
        }
        return ret;
    }

    ret = nv_init_data_prepare(&pdata2, flash_addr + HNV_BLOCK_SIZE);
    if (ret != EXT_ERR_SUCCESS) {
        boot_free(pdata1);
        if (pdata2 != TD_NULL) {
            boot_free(pdata2);
        }
        return ret;
    }

    do {
        /* stage of data check */
        td_u8 *data[2] = { pdata1, pdata2 }; /* array length 2 */
        ret = nv_init_data_check(work_addr, source_file_addr, nv_type, nv_ctrl, data);
        if (ret != EXT_ERR_SUCCESS) {
            break;
        }

        if (nv_ctrl->total_num > NV_TOTAL_NUM_MAX) {
            break;
        }

        /* apply for index space */
        ret = nv_init_index_space(nv_ctrl);
    } while (0);

    /* original backup not repair temporarily */
    boot_free(pdata2);
    boot_free(pdata1);
    return ret;
}

td_u32 nv_read_common(td_u8 id, td_pvoid pdata, td_u8 len, ext_nv_type nv_type)
{
    td_u32 ret;
    td_u32 i;
    td_u32 crc = 0;
    td_u8 item_len;
    td_bool to_check_crc;

    /* check parameter */
    if ((pdata == TD_NULL) || (len == 0)) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    ext_nv_ctrl *nv_ctrl = &g_nv_ctrl[nv_type];
    if (nv_ctrl->init_flag != TD_TRUE) {
        return EXT_ERR_NO_INIT;
    }

    ext_nv_item_index *nv_index = nv_find_item(TD_NULL, id, nv_type);
    if (nv_index == TD_NULL) {
        nv_print_f2("can't find nv id 0x%x len=%x\r\n", id, len);
        return EXT_ERR_NOT_FOUND;
    }

    if (nv_index->nv_len == len) {
        item_len = len;
        to_check_crc = TD_TRUE;
    } else {
        item_len = uapi_min(nv_index->nv_len, len);
        to_check_crc = TD_FALSE;
    }

    /* allow error */
    for (i = 0; i < HNV_FAULT_TOLERANT_TIMES; i++) {
        ret = uapi_flash_read(nv_ctrl->current_addr + nv_index->nv_offset, item_len, pdata);
        if (ret != EXT_ERR_SUCCESS) {
            continue;
        }
        /* check crc when length is the same */
        if (to_check_crc == TD_TRUE) {
            ret = uapi_flash_read(nv_ctrl->current_addr + nv_index->nv_offset + item_len, sizeof(crc), (td_u8 *)&crc);
            if (ret != EXT_ERR_SUCCESS) {
                continue;
            }

            if (crc != uapi_crc32(0, pdata, item_len)) {
                continue;
            }
        }
        break;
    }

    if (i >= HNV_FAULT_TOLERANT_TIMES) {
        ret = EXT_ERR_FAIL_N_TIMES;
    }

    return ret;
}

EXT_PRV td_u32 nv_write_para_check(const td_pvoid pdata, td_u8 len, ext_nv_type nv_type, ext_nv_ctrl **nv_ctrl)
{
    if ((pdata == TD_NULL) || (len == 0) || (len > HNV_ITEM_MAXLEN)) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    *nv_ctrl = &g_nv_ctrl[nv_type];
    if ((*nv_ctrl)->init_flag != TD_TRUE) {
        return EXT_ERR_NO_INIT;
    }
    return EXT_ERR_SUCCESS;
}
EXT_PRV td_u32 nv_write_cmp_readback(td_u8 id, const td_pvoid pdata, td_u8 len, ext_nv_type nv_type)
{
    td_u32 ret;
    td_u8 *item_readback = TD_NULL;

    item_readback = boot_malloc(HNV_ITEM_MAXLEN);
    if (item_readback == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }

    ret = nv_read_common(id, item_readback, len, nv_type);
    if (ret != EXT_ERR_SUCCESS) {
        boot_free(item_readback);
        return ret;
    }

    if (memcmp(pdata, item_readback, len) == 0) {
        boot_free(item_readback);
        return EXT_ERR_SUCCESS;
    }
    boot_free(item_readback);

    return EXT_ERR_NOT_FOUND;
}

typedef struct {
    td_pvoid pdata;
    td_u8 len;
    td_u8 pad[3]; /* pad3 */
} nv_write_data_and_len;
EXT_PRV td_u32 nv_write_update_data(const nv_write_data_and_len *data_and_len, const ext_nv_ctrl *nv_ctrl,
    td_u8 *nv_file, const ext_nv_item_index *nv_index, ext_nv_manage **out_head)
{
    td_u32 ret;
    td_u32 crc;
    td_u8 item_len = data_and_len->len;
    ext_nv_manage *nv_head = TD_NULL;

    if (out_head == TD_NULL) {
        return EXT_ERR_FAILURE;
    }

    ret = uapi_flash_read(nv_ctrl->current_addr, HNV_BLOCK_SIZE, nv_file);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    /* update data  includes 2 of crc and seq */
    crc = uapi_crc32(0, data_and_len->pdata, item_len);
    if (item_len > 0 && memcpy_s(nv_file + nv_index->nv_offset, HNV_BLOCK_SIZE - nv_index->nv_offset,
        data_and_len->pdata, item_len) != 0) {
        return EXT_ERR_FAILURE;
    }

    if (memcpy_s(nv_file + nv_index->nv_offset + item_len, HNV_BLOCK_SIZE - nv_index->nv_offset - item_len, &crc,
        sizeof(crc)) != 0) {
        return EXT_ERR_FAILURE;
    }

    nv_head = (ext_nv_manage *)nv_file;
    nv_head->seq += 1;
    nv_head->crc = uapi_crc32(0, nv_file + 8, HNV_BLOCK_SIZE - 8); /* shifts 8 */
    ret = uapi_flash_erase(nv_ctrl->next_addr, HNV_BLOCK_SIZE);
    ret += uapi_flash_write(nv_ctrl->next_addr, HNV_BLOCK_SIZE, nv_file, TD_FALSE);
    if (ret != EXT_ERR_SUCCESS) {
        return EXT_ERR_FAILURE;
    }

    *out_head = nv_head;
    return EXT_ERR_SUCCESS;
}

td_u32 nv_write_common(td_u8 id, const td_pvoid pdata, td_u8 len, ext_nv_type nv_type)
{
    ext_nv_ctrl *nv_ctrl = TD_NULL;
    ext_nv_manage *nv_head = TD_NULL;
    td_u8 i;
    /* backup mutually and write for 2 copies when operate factory part */
    td_u8 backup_num = (nv_type == EXT_TYPE_FACTORY_NV) ? 2 : 1; /* 2 */

    /* check parameter */
    td_u32 ret = nv_write_para_check(pdata, len, nv_type, &nv_ctrl);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    /* not write when read back is the same */
    ret = nv_write_cmp_readback(id, pdata, len, nv_type);
    if (ret != EXT_ERR_NOT_FOUND) {
        return ret;
    }

    /* formally write process */
    ext_nv_item_index *nv_index = nv_find_item(TD_NULL, id, nv_type);
    if (nv_index == TD_NULL) {
        return EXT_ERR_NOT_FOUND;
    }

    nv_write_data_and_len data_and_len;
    data_and_len.pdata = pdata;
    /* length handle */
    data_and_len.len = len;
    if (nv_index->nv_len != len) {
        data_and_len.len = uapi_min(nv_index->nv_len, len);
    }

    /* read 4K for replace, and calc crc again */
    td_u8 *nv_file = boot_malloc(HNV_BLOCK_SIZE);

    for (; backup_num > 0; backup_num--) {
        for (i = 0; i < HNV_FAULT_TOLERANT_TIMES; i++) {
            ret = nv_write_update_data(&data_and_len, nv_ctrl, nv_file, nv_index, &nv_head);
            if (ret != EXT_ERR_SUCCESS) {
                continue;
            }
            break;
        }

        /* refresh global info */
        td_u32 temp = nv_ctrl->current_addr;
        nv_ctrl->current_addr = nv_ctrl->next_addr;
        nv_ctrl->next_addr = temp;
        if (nv_head != TD_NULL) {
            nv_ctrl->seq = nv_head->seq;
            nv_head = TD_NULL;
        }

        if (i >= HNV_FAULT_TOLERANT_TIMES) {
            boot_free(nv_file);
            return EXT_ERR_FAIL_N_TIMES;
        }
    }

    boot_free(nv_file);
    return EXT_ERR_SUCCESS;
}

td_u32 uapi_factory_nv_init(td_u32 flash_addr)
{
    return nv_init_common(flash_addr, 0, EXT_TYPE_FACTORY_NV);
}

td_u32 uapi_factory_nv_read(td_u8 id, td_pvoid pdata, td_u8 len)
{
    return nv_read_common(id, pdata, len, EXT_TYPE_FACTORY_NV);
}

td_u32 uapi_factory_nv_write(td_u8 id, const td_pvoid pdata, td_u8 len)
{
    return nv_write_common(id, pdata, len, EXT_TYPE_FACTORY_NV);
}

td_u32 uapi_temp_nv_init(td_u32 flash_addr, td_u32 backup_addr)
{
    return nv_init_common(flash_addr, backup_addr, EXT_TYPE_TEMP);
}

td_u32 uapi_common_nv_read(td_u8 id, td_pvoid pdata, td_u8 len, ext_nv_type nv_type)
{
    return nv_read_common(id, pdata, len, nv_type);
}

td_u32 uapi_common_nv_write(td_u8 id, EXT_CONST td_pvoid pdata, td_u8 len, ext_nv_type nv_type)
{
    return nv_write_common(id, pdata, len, nv_type);
}

/* fallowing functions are nv interfaces */
td_u32 uapi_nv_check_file(td_u32 flash_addr)
{
    td_u32 ret;
    td_u8 *data = NULL;
    ext_nv_manage nv_head = { 0 };

    ret = uapi_flash_read(flash_addr, sizeof(ext_nv_manage), (td_u8 *)&nv_head);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    data = boot_malloc(nv_head.flash_size);
    if (data == TD_NULL) {
        return EXT_ERR_MALLOC_FAILUE;
    }

    ret = uapi_flash_read(flash_addr, nv_head.flash_size, data);
    if (ret != EXT_ERR_SUCCESS) {
        boot_free(data);
        return ret;
    }

    ret = nv_check_file(data, nv_head.flash_size, EXT_TYPE_NV);
    if (ret != EXT_ERR_SUCCESS) {
        boot_free(data);
        return ret;
    }

    boot_free(data);
    return EXT_ERR_SUCCESS;
}

td_u32 uapi_nv_get_ver(td_u32 flash_addr, td_u32 *ver)
{
    td_u32 ret;

    if (ver == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    ret = uapi_flash_read(flash_addr + ext_struct_offset(ext_nv_manage, ver_magic), sizeof(td_u32), (td_u8 *)ver);
    if (ret != EXT_ERR_SUCCESS) {
        return EXT_ERR_FAILURE;
    }
    return EXT_ERR_SUCCESS;
}
