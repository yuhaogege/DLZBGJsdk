/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: All code updates to get address information from partition table,
 * B4 can compile flash_partition_table.c code
 * Author: CompanyName
 * Create: 2012-08-28
 */
#include "flash_partition_table.h"
#include <soc_stdlib.h>
#include <soc_mdm_nv.h>
#include <soc_mdm_mem.h>
#include <soc_mdm_flash.h>
#include <soc_mdm_task.h>
#include <soc_mdm_sem.h>
#include <soc_ft_nv.h>
#include <soc_partition_table.h>
#include <dw21_platform_rom.h>
#include <soc_stdlib.h>

#define partition_id(i) (i)
/* Corresponding partition table, input as subscript, output as id */
#define array_index(id) (id)
/* Corresponding partition table, input is id, output is subscript */
#define FACTORY_NV_ADDR_REG (DW21_SYSCTRL_SC_GEN_REG3_REG) /* Low 24bit indicates factory NV address */
#define FACTORY_NV_ADDR_MASK 0xFFFFFF
#define REFRESH_IDLE 0
#define REFRESH_BUSY 1
#define BLOCK_SIZE PRODUCT_CFG_FLASH_BLOCK_SIZE
#define PT_DEBUG_PRINT printf

/* Codex rectification, need to judge the range of NV address value */
#define NV_ADDR_VALUE_MAX (16 * 1024 * 1024)

static ext_flash_partition_table g_partition_table;
static td_u32 g_partition_sem_handle;

__isr ext_flash_partition_table *uapi_get_partition_table(td_void)
{
    return &g_partition_table;
}

/* Internal interface */
td_u32 flash_partition_get_ver(ext_partition_ver_info *ver)
{
    td_u32 ret;
    ext_flash_partition_table *ptable = TD_NULL;

    /* The internal interface does not judge the legality of the entry. */
    ptable = uapi_malloc(EXT_MOD_ID_PARTITION, sizeof(ext_flash_partition_table));
    if (ptable == TD_NULL) {
        return EXT_ERR_PARTITION_MALLOC_FAIL;
    }

    ret = nv_read_common(EXT_NV_FTM_FLASH_PARTIRION_TABLE_ID, ptable, sizeof(ext_flash_partition_table),
        EXT_TYPE_FACTORY_NV);
    if (ret == EXT_ERR_SUCCESS) {
        if (memcpy_s((td_pvoid)ver, sizeof(ext_partition_ver_info), (td_pvoid)&ptable->ver, sizeof(ptable->ver))
            != EOK) {
            uapi_free(EXT_MOD_ID_PARTITION, ptable);
            return EXT_ERR_FAILURE;
        }
    }

    uapi_free(EXT_MOD_ID_PARTITION, ptable);
    return ret;
}

td_u32 uapi_flash_partition_get_ver(ext_partition_ver_info *ver)
{
    if (ver == TD_NULL) {
        return EXT_ERR_PARTITION_INVALID_PARAM;
    }
    return flash_partition_get_ver(ver);
}

EXT_PRV ext_partition_ver flash_partition_check_ver(EXT_CONST ext_partition_ver_info *ver)
{
    ext_partition_ver_info local_ver;
    td_u32 ret;

    /* Incompatible with the old version, this restriction is only done under the kernel, not in the boot */
    ret = flash_partition_get_ver(&local_ver); /* The local version number is read from 0x4 */
    if (ret != EXT_ERR_SUCCESS) {
        return EXT_PARTITION_VER_ERR;
    }

    if (local_ver.ver_num == 0) {
        return EXT_PARTITION_VER_ERR;
    }
    if ((local_ver.bitmap & ver->bitmap) == 0) {
        return EXT_PARTITION_VER_ERR;
    }

    if (local_ver.ver_num > ver->ver_num) {
        return EXT_PARTITION_VER_ERR;
    } else if (local_ver.ver_num == ver->ver_num) {
        return EXT_PARTITION_VER_THE_SAME;
    } else {
        return EXT_PARTITION_VER_NEW;
    }
}

ext_partition_ver uapi_flash_partition_check_ver(EXT_CONST ext_partition_ver_info *ver)
{
    if (ver == TD_NULL) {
        return EXT_PARTITION_VER_ERR;
    }
    return flash_partition_check_ver(ver);
}

/* ext_flash_partition_table_s+ext_pt_change_ctrl_s */
EXT_PRV td_u32 flash_partition_check(const ext_flash_partition_chage_info *change_info, td_u16 size)
{
    td_u8 i;
    ext_partition_ver ver_ret;
    ext_flash_partition_table *old_info = uapi_get_partition_table();

    /* Check rule: the length is in line with expectations, and the maximum length is not judged for expansion */
    if (size < sizeof(ext_flash_partition_chage_info)) {
        return EXT_ERR_PARTITION_INVALID_PARAM;
    }

    if (change_info->change_ctrl_info.total_num > EXT_FLASH_PARTITON_MAX) {
        return EXT_ERR_PARTITION_INVALID_PARAM;
    }

    if (size < sizeof(ext_flash_partition_chage_info) + sizeof(ext_pt_change_detail) *
        change_info->change_ctrl_info.total_num) {
        return EXT_ERR_PARTITION_INVALID_PARAM;
    }

    /* Check rule: not the same as the original partition table */
    if (memcmp(old_info, &change_info->new_info, sizeof(ext_flash_partition_table)) == 0) {
        return EXT_ERR_PARTITION_UPDATA_TBL_SAME;
    }

    /* Detect partition version number */
    ver_ret = flash_partition_check_ver(&change_info->change_ctrl_info.ver_support);
    if (ver_ret == EXT_PARTITION_VER_THE_SAME) {
        return EXT_ERR_PARTITION_UPDATA_VER_SAME;
    } else if (ver_ret != EXT_PARTITION_VER_NEW) {
        return EXT_ERR_PARTITION_UPDATA_VER_ERR;
    }

    for (i = 0; i < change_info->change_ctrl_info.total_num; i++) {
        if (change_info->change_item[i].id <= partition_id(EXT_FLASH_PARTITON_KERNEL)) {
            return EXT_ERR_PARTITION_UPDATA_NOT_SUPPORT_ARER;
        }
        /* If it is smaller than the actual kernel size, it will fail */
        /* If the size is smaller than the actual upgrade area, it will fail */
    }
    return EXT_ERR_SUCCESS;
}

EXT_PRV td_void flash_partition_changinfo_2_changelog(const ext_flash_partition_chage_info *change_info,
    ext_pt_change_log *change_log)
{
    ext_pt_change_status *change_log_items = change_log->table;
    td_u32 i;

    (td_void)memset_s(change_log, sizeof(ext_pt_change_log), 0, sizeof(ext_pt_change_log));

    (td_void)memcpy_s(&change_log->ctrl, sizeof(ext_pt_change_ctrl), &change_info->change_ctrl_info,
        sizeof(change_info->change_ctrl_info));

    for (i = 0; i < change_info->change_ctrl_info.total_num; i++) {
        change_log_items[i].id = change_info->change_item[i].id;
        change_log_items[i].top1_or_bottom0 = change_info->change_item[i].top1_or_bottom0;
        change_log_items[i].move = TD_TRUE;
    }

    /* Write the modification method to NV */
    change_log->ctrl.status = REFRESH_BUSY;
    return;
}

td_u32 uapi_flash_partition_config(ext_flash_partition_chage_info *change_info, td_u16 size)
{
    td_u32 ret;
    ext_pt_change_log *change_log = TD_NULL;

    if (change_info == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    change_log = uapi_malloc(EXT_MOD_ID_PARTITION, sizeof(ext_pt_change_log));
    if (change_log == TD_NULL) {
        ret = EXT_ERR_PARTITION_MALLOC_FAIL;
        goto END;
    }

    /* Verify the correctness of the partition replacement information */
    ret = flash_partition_check(change_info, size);
    if (ret != EXT_ERR_SUCCESS) {
        goto END;
    }

    /* Write the latest partition information to the backup area */
    ret = uapi_factory_nv_write(EXT_NV_FTM_FLASH_PARTIRION_TABLE_B_ID, &(change_info->new_info),
        sizeof(ext_flash_partition_table));
    if (ret != EXT_ERR_SUCCESS) {
        goto END;
    }

    /* Convert the contents of the partition upgrade file to the contents of the partition log file */
    flash_partition_changinfo_2_changelog(change_info, change_log);

    /* Write partition log information to NV */
    ret = uapi_factory_nv_write(EXT_NV_FTM_FLASH_PARTIRION_CHANGE_ID, change_log, sizeof(ext_pt_change_log));
    if (ret != EXT_ERR_SUCCESS) {
        goto END;
    }

    ret = EXT_ERR_SUCCESS;
END:
    if (change_log != TD_NULL) {
        uapi_free(EXT_MOD_ID_PARTITION, change_log);
    }
    return ret;
}

td_u32 uapi_flash_partition_get_sem_handle(td_u32 *handle)
{
    td_u32 ret;
    static td_bool bool_init = TD_FALSE;

    if (handle == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    if (bool_init != TD_TRUE) {
        ret = uapi_sem_bcreate(&g_partition_sem_handle, "part_sem", EXT_SEM_ONE);
        if (ret != EXT_ERR_SUCCESS) {
            return EXT_ERR_FAILURE;
        }
        bool_init = TD_TRUE;
    }
    *handle = g_partition_sem_handle;

    return EXT_ERR_SUCCESS;
}

__init td_u32 flash_partition_init_tbl(td_void)
{
    td_u32 ret;
    ext_flash_partition_table *table = TD_NULL;

    table = uapi_get_partition_table();
    ret = uapi_factory_nv_read(EXT_NV_FTM_FLASH_PARTIRION_TABLE_ID, table, sizeof(ext_flash_partition_table));
    if (ret != EXT_ERR_SUCCESS) {
        return EXT_ERR_NV_ERROR_READ;
    }
    return EXT_ERR_SUCCESS;
}

__init td_u32 flash_partition_init_ftnv(td_void)
{
    td_u32 fnv_addr;

    UAPI_REG_READ(FACTORY_NV_ADDR_REG, fnv_addr);
    fnv_addr &= FACTORY_NV_ADDR_MASK;
    if (fnv_addr > NV_ADDR_VALUE_MAX) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    return uapi_factory_nv_init(fnv_addr);
}

__init td_u32 uapi_flash_partition_init(td_void)
{
    static td_bool b_init = TD_FALSE;
    td_u32 ret;

    if (b_init == TD_TRUE) {
        return EXT_ERR_SUCCESS;
    }

    /* 1. Initialize the factory area NV */
    ret = flash_partition_init_ftnv(); /* ps:it's better move fnv init out of this function */
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    /* 2. Initialize tbl */
    ret = flash_partition_init_tbl();
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    /* 3. Replace the partition table */
    ret = uapi_flash_partition_refresh(EXT_PARTITON_SOFT_RESET);
    if (ret == EXT_ERR_SUCCESS) {
        b_init = TD_TRUE; /* ps:if false ,what to do ? */
    }

    return ret;
}

/* must be mutually exclusive before and after the call */
EXT_PRV td_u32 flash_partition_reset(ext_partition_reset reset)
{
    td_u32 ret;

    if (reset == EXT_PARTITON_SOFT_RESET) {
        ext_flash_partition_table *table = TD_NULL;

        table = uapi_get_partition_table();
        ret = nv_read_common(EXT_NV_FTM_FLASH_PARTIRION_TABLE_ID, table, sizeof(ext_flash_partition_table),
            EXT_TYPE_FACTORY_NV);
        if (ret != EXT_ERR_SUCCESS) {
            return ret;
        }
        /* 1. The crash is instant, no need to refresh */
        /* 2.NV area soft reset */
        ret = uapi_nv_soft_reset(table->table[EXT_FLASH_PARTITON_NV_FILE].addr_blk * BLOCK_SIZE,
            table->table[EXT_FLASH_PARTITON_NV_BACKUP].addr_blk * BLOCK_SIZE);
    } else if (reset == EXT_PARTITON_HARD_RESET) {
        ret = EXT_ERR_SUCCESS;
    } else {
        ret = EXT_ERR_SUCCESS;
    }

    return ret;
}
typedef struct {
    ext_pt_change_log *inter_ctrl;
    ext_flash_partition_table *old_table;
    ext_flash_partition_table *new_table;
    td_u8 *tmp_buf;
} partition_refresh_param;

EXT_PRV td_u32 partition_refresh_create_res(partition_refresh_param *param)
{
    /* Resource Preparation */
    param->inter_ctrl = uapi_malloc(EXT_MOD_ID_PARTITION, sizeof(ext_pt_change_log));
    param->new_table = uapi_malloc(EXT_MOD_ID_PARTITION, sizeof(ext_flash_partition_table));
    param->tmp_buf = (td_u8 *)uapi_malloc(EXT_MOD_ID_PARTITION, BLOCK_SIZE);
    if (param->inter_ctrl == TD_NULL || param->new_table == TD_NULL || param->tmp_buf == TD_NULL) {
        return EXT_ERR_NOT_ENOUGH_MEMORY;
    }
    return EXT_ERR_SUCCESS;
}

EXT_PRV td_void partition_refresh_free_res(EXT_CONST partition_refresh_param *param)
{
    if (param->inter_ctrl) {
        uapi_free(EXT_MOD_ID_PARTITION, param->inter_ctrl);
    }
    if (param->new_table) {
        uapi_free(EXT_MOD_ID_PARTITION, param->new_table);
    }
    if (param->tmp_buf) {
        uapi_free(EXT_MOD_ID_PARTITION, param->tmp_buf);
    }
    return;
}

EXT_PRV td_u32 partition_refresh_prepare1(EXT_CONST partition_refresh_param *param)
{
    td_u32 ret = nv_read_common(EXT_NV_FTM_FLASH_PARTIRION_CHANGE_ID, param->inter_ctrl, sizeof(ext_pt_change_log),
        EXT_TYPE_FACTORY_NV);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    ret = nv_read_common(EXT_NV_FTM_FLASH_PARTIRION_TABLE_B_ID, param->new_table, sizeof(ext_flash_partition_table),
        EXT_TYPE_FACTORY_NV);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    return EXT_ERR_SUCCESS;
}

EXT_PRV td_bool partition_refresh_is_need(EXT_CONST partition_refresh_param *param)
{
    if (param->inter_ctrl->ctrl.status != REFRESH_BUSY) {
        return TD_FALSE;
    }
    return TD_TRUE;
}

EXT_PRV td_u32 partition_refresh_prepare2(EXT_CONST partition_refresh_param *param, ext_nv_type *save_type)
{
    td_u32 ret = EXT_ERR_SUCCESS;
    if (param->inter_ctrl->ctrl.temp_area_blk != 0) {
        *save_type = EXT_TYPE_TEMP;
        /* Refresh data ,ps:why? */
        ret = nv_init_common(param->inter_ctrl->ctrl.temp_area_blk * BLOCK_SIZE,
            param->old_table->table[EXT_FLASH_PARTITON_FACTORY_NV].addr_blk * BLOCK_SIZE, *save_type);
        if (ret != EXT_ERR_SUCCESS) {
            return ret;
        }
        ret = nv_read_common(EXT_NV_FTM_FLASH_PARTIRION_CHANGE_ID, param->inter_ctrl, sizeof(ext_pt_change_log),
            *save_type); /* ps:reread? */
        if (ret != EXT_ERR_SUCCESS) {
            return ret;
        }
    } else {
        *save_type = EXT_TYPE_FACTORY_NV;
    }

    return EXT_ERR_SUCCESS;
}

typedef struct {
    td_u32 cur_idx;
    td_u32 ctrl_idx;
    td_u32 src_addr;
    td_u32 dst_addr;
    td_u32 src_end_addr;
    td_u32 dst_end_addr;
} partition_move_item_param;

EXT_PRV td_bool partiton_refresh_is_move_item(EXT_CONST partition_refresh_param *param,
    partition_move_item_param *item_param)
{
    td_u32 cur_idx = item_param->cur_idx;
    td_u32 ctrl_idx = item_param->ctrl_idx;
    td_u32 old_table_size = 0;
    td_u32 new_table_size = 0;

    if (param->inter_ctrl->table[ctrl_idx].move == TD_FALSE) {
        return TD_FALSE;
    }
    if (uint_2_multiply(param->old_table->table[cur_idx].addr_blk, BLOCK_SIZE, &old_table_size) == TD_FALSE ||
        uint_2_multiply(param->new_table->table[cur_idx].addr_blk, BLOCK_SIZE, &new_table_size) == TD_FALSE) {
        return TD_FALSE;
    }
    item_param->src_addr = old_table_size;
    item_param->dst_addr = new_table_size;
    item_param->src_end_addr =
        param->old_table->table[cur_idx].addr_blk * BLOCK_SIZE + param->old_table->table[cur_idx].size_blk * BLOCK_SIZE;
    item_param->dst_end_addr =
        param->new_table->table[cur_idx].addr_blk * BLOCK_SIZE + param->new_table->table[cur_idx].size_blk * BLOCK_SIZE;

    if (param->new_table->table[ctrl_idx].dir == 0) {
        /* Does not consider the direction of the front and rear versions */
        if (item_param->src_addr == item_param->dst_addr) {
            return TD_FALSE;
        }
    } else {
        if (item_param->src_end_addr == item_param->dst_end_addr) {
            return TD_FALSE;
        }
    }

    return TD_TRUE;
}

EXT_PRV td_u32 partition_refresh_copy_one_blk(EXT_CONST partition_refresh_param *param,
                                              td_u32 src_addr,
                                              td_u32 dst_addr)
{
    td_u32 ret;

    ret = uapi_flash_read_in(src_addr, BLOCK_SIZE, param->tmp_buf);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    ret = uapi_flash_erase_in(dst_addr, BLOCK_SIZE);
    ret |= uapi_flash_write_in(dst_addr, BLOCK_SIZE, param->tmp_buf, TD_FALSE);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    return ret;
}

EXT_PRV td_bool partiton_refresh_move_item(EXT_CONST partition_refresh_param *param,
    partition_move_item_param *item_param, ext_nv_type save_type)
{
    td_u32 ret = EXT_ERR_SUCCESS;
    td_u32 total_size; /* Actually move the size, the three compare the small value, the first two contrast size */
    td_s32 size_finished;
    td_u32 abs_size_finished;
    td_u32 len;
    td_s32 size_per_time;
    td_u32 cur_idx = item_param->cur_idx;
    td_u32 ctrl_idx = item_param->ctrl_idx;
    if (uint_2_multiply(param->old_table->table[cur_idx].addr_blk, BLOCK_SIZE, TD_NULL) == TD_FALSE ||
        uint_2_multiply(param->new_table->table[cur_idx].addr_blk, BLOCK_SIZE, TD_NULL) == TD_FALSE) {
        return TD_FALSE;
    }

    /* Take size small value move */
    total_size = (param->old_table->table[cur_idx].size_blk <= param->new_table->table[cur_idx].size_blk) ?
        (param->old_table->table[cur_idx].size_blk * BLOCK_SIZE) :
        (param->new_table->table[cur_idx].size_blk * BLOCK_SIZE);

    abs_size_finished = (param->inter_ctrl->table[cur_idx].finished_block) * BLOCK_SIZE;
    if ((param->inter_ctrl->table[ctrl_idx].top1_or_bottom0 == EXT_PARTITION_TB_TOP) ||
        ((item_param->dst_addr > item_param->src_addr) &&
        (param->inter_ctrl->table[ctrl_idx].top1_or_bottom0 == EXT_PARTITION_TB_AUTO))) {
        /* High address moves first, address grows down */
        size_per_time = 0 - BLOCK_SIZE;
        size_finished = (td_s32)(0 - abs_size_finished);
        /* Last corresponding block address */
        item_param->src_addr = item_param->src_end_addr - BLOCK_SIZE;
        item_param->dst_addr = item_param->dst_end_addr - BLOCK_SIZE;
    } else {
        /* Low address moves first, address grows up */
        size_per_time = BLOCK_SIZE;
        size_finished = (td_s32)abs_size_finished;
    }

    item_param->src_addr = (td_u32)((td_s32)item_param->src_addr + size_finished); /* This is not fault-tolerant */
    item_param->dst_addr = (td_u32)((td_s32)item_param->dst_addr + size_finished);

    for (len = abs_size_finished; len < total_size;) {
        ret = partition_refresh_copy_one_blk(param, item_param->src_addr, item_param->dst_addr);
        if (ret != EXT_ERR_SUCCESS) {
            return (td_bool)ret;
        }
        item_param->src_addr = (td_u32)((td_s32)item_param->src_addr + size_per_time);
        item_param->dst_addr = (td_u32)((td_s32)item_param->dst_addr + size_per_time);
        len += BLOCK_SIZE;

        param->inter_ctrl->table[cur_idx].finished_block = len / BLOCK_SIZE;
        ret = nv_write_common(EXT_NV_FTM_FLASH_PARTIRION_CHANGE_ID, param->inter_ctrl, sizeof(ext_pt_change_log),
            save_type);
        if (ret != EXT_ERR_SUCCESS) {
            return (td_bool)ret;
        }
    }
    return (td_bool)ret;
}

/* refresh real proc */
EXT_PRV td_u32 partition_refresh_move(EXT_CONST partition_refresh_param *param, ext_nv_type save_type)
{
    td_u32 ret = EXT_ERR_SUCCESS;
    partition_move_item_param item_param = { 0 };
    for (td_u32 ctrl_idx = 0; ctrl_idx < param->inter_ctrl->ctrl.total_num; ctrl_idx++) {
        item_param.ctrl_idx = ctrl_idx;
        item_param.cur_idx = array_index(param->inter_ctrl->table[ctrl_idx].id);

        if (TD_FALSE == partiton_refresh_is_move_item(param, &item_param)) {
            continue;
        }

        ret = partiton_refresh_move_item(param, &item_param, save_type);
        if (ret != EXT_ERR_SUCCESS) {
            return ret;
        }

        param->inter_ctrl->table[ctrl_idx].move = TD_FALSE;
        ret = nv_write_common(EXT_NV_FTM_FLASH_PARTIRION_CHANGE_ID, param->inter_ctrl, sizeof(ext_pt_change_log),
            save_type);
        if (ret != EXT_ERR_SUCCESS) {
            return ret;
        }
    }
    return ret;
}

EXT_PRV td_u32 partition_refresh_finish(EXT_CONST partition_refresh_param *param, ext_partition_reset reset)
{
    /* wrtite new tbl */
    td_u32 ret = nv_write_common(EXT_NV_FTM_FLASH_PARTIRION_TABLE_ID,
                                 param->new_table,
                                 sizeof(ext_flash_partition_table),
        EXT_TYPE_FACTORY_NV);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    /* write ctrl */
    param->inter_ctrl->ctrl.status = REFRESH_IDLE;
    ret = nv_write_common(EXT_NV_FTM_FLASH_PARTIRION_CHANGE_ID, param->inter_ctrl, sizeof(ext_pt_change_log),
        EXT_TYPE_FACTORY_NV);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    ret = flash_partition_reset(reset);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }
    return EXT_ERR_SUCCESS;
}

td_u32 uapi_flash_partition_refresh(ext_partition_reset reset)
{
    td_u32 ret;
    ext_nv_type save_type;
    partition_refresh_param param = { 0 };
    param.old_table = uapi_get_partition_table();

    /* init resource */
    ret = partition_refresh_create_res(&param);
    if (ret != EXT_ERR_SUCCESS) {
        goto END;
    }

    /* init cfg from nv */
    ret = partition_refresh_prepare1(&param);
    if (ret != EXT_ERR_SUCCESS) {
        goto END;
    }

    if (TD_FALSE == partition_refresh_is_need(&param)) {
        ret = EXT_ERR_SUCCESS; /* Does not require a refresh to return directly to success */
        goto END;
    }

    /* ps:??????? */
    ret = partition_refresh_prepare2(&param, &save_type);
    if (ret != EXT_ERR_SUCCESS) {
        goto END;
    }

    /* do refresh */
    partition_refresh_move(&param, save_type);
    if (ret != EXT_ERR_SUCCESS) {
        goto END;
    }
    /* finish work */
    ret = partition_refresh_finish(&param, reset);
END:
    partition_refresh_free_res(&param);
    return ret;
}
