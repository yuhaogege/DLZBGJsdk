/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: boot partition
 * Author: CompanyName
 * Create: 2018-10-31
 */

#include "boot_partition_table.h"

static ext_flash_partition_table g_partition_table;

#define ext_flash_partition_refresh_make_address()                              \
    do {                                                                       \
        src_addr = old_table->table[current_index].addr_blk * BLOCK_SIZE;      \
        dst_addr = new_table->table[current_index].addr_blk * BLOCK_SIZE;      \
        src_end_addr = old_table->table[current_index].addr_blk * BLOCK_SIZE + \
            old_table->table[current_index].size_blk * BLOCK_SIZE;             \
        dst_end_addr = new_table->table[current_index].addr_blk * BLOCK_SIZE + \
            new_table->table[current_index].size_blk * BLOCK_SIZE;             \
    } while (0)
/* move the highest address first. address grows down to lower address */
#define ext_flash_partition_refresh_proc1()               \
    do {                                                 \
        size_per_time = 0 - BLOCK_SIZE;                  \
        size_finished = (td_s32)(0 - abs_size_finished); \
        src_addr = src_end_addr - BLOCK_SIZE;            \
        dst_addr = dst_end_addr - BLOCK_SIZE;            \
    } while (0)
#define ext_flash_partition_refresh_proc2()                                                               \
    do {                                                                                                 \
        if ((inter_ctrl->table[i].top1_or_bottom0 == EXT_PARTITION_TB_TOP) ||                             \
            ((dst_addr > src_addr) && (inter_ctrl->table[i].top1_or_bottom0 == EXT_PARTITION_TB_AUTO))) { \
            ext_flash_partition_refresh_proc1();                                                          \
        } else {                                                                                         \
            size_per_time = BLOCK_SIZE;                                                                  \
            size_finished = (td_s32)abs_size_finished;                                                   \
        }                                                                                                \
    } while (0)
#define ext_flash_partition_refresh_proc3()                                                               \
    do {                                                                                                  \
        src_addr = (td_u32)((td_s32)src_addr + size_per_time);                                            \
        dst_addr = (td_u32)((td_s32)dst_addr + size_per_time);                                            \
        len += BLOCK_SIZE;                                                                                \
        inter_ctrl->table[current_index].finished_block = len / BLOCK_SIZE;                               \
        ret = uapi_common_nv_read(EXT_NV_FTM_FLASH_PARTIRION_CHANGE_ID, inter_ctrl,                       \
            sizeof(ext_pt_change_log), save_type); \
    } while (0)

ext_flash_partition_table *uapi_get_partition_table(td_void)
{
    return &g_partition_table;
}

static td_void uapi_flash_partition_refresh_free_memery(EXT_CONST ext_pt_change_log *inter_ctrl,
    EXT_CONST ext_flash_partition_table *new_table,
    EXT_CONST td_u8 *buffer)
{
    uapi_free(EXT_MOD_ID_PARTITION, (td_pvoid)buffer);
    uapi_free(EXT_MOD_ID_PARTITION, (td_pvoid)new_table);
    uapi_free(EXT_MOD_ID_PARTITION, (td_pvoid)inter_ctrl);
}

/* inner  interface */
static td_u32 flash_partition_get_ver(ext_partition_ver_info *ver)
{
    td_u32 ret;
    ext_flash_partition_table *ptable;

    /* inner  interface does not verify the legality of input parameter */
    ptable = boot_malloc(sizeof(ext_flash_partition_table));
    ret = uapi_common_nv_read(EXT_NV_FTM_FLASH_PARTIRION_TABLE_ID, ptable, sizeof(ext_flash_partition_table),
        EXT_TYPE_FACTORY_NV);
    if (ret == EXT_ERR_SUCCESS) {
        *ver = ptable->ver;
    }

    boot_free(ptable);
    return ret;
}

td_u32 uapi_flash_partition_get_ver(ext_partition_ver_info *ver)
{
    if (ver == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    return flash_partition_get_ver(ver);
}

/* inner  interface */
EXT_PRV ext_partition_ver flash_partition_check_ver(const ext_partition_ver_info *ver)
{
    ext_partition_ver_info local_ver;
    td_u32 ret;

    if (ver->boot_support == 0) {
        return EXT_PARTITION_VER_ERR;
    }

    ret = flash_partition_get_ver(&local_ver);
    if (ret != EXT_ERR_SUCCESS) {
        return EXT_PARTITION_VER_ERR;
    }

    if (local_ver.ver_num == 0) {
        return EXT_PARTITION_VER_ERR;
    }

    if ((local_ver.map & ver->map) == 0) {
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

ext_partition_ver uapi_flash_partition_check_ver(const ext_partition_ver_info *ver)
{
    if (ver == TD_NULL) {
        return EXT_PARTITION_VER_ERR;
    }

    return flash_partition_check_ver(ver);
}

EXT_PRV td_u32 flash_partition_reset(td_void)
{
    ext_flash_partition_table *table = TD_NULL;

    table = uapi_get_partition_table();
    return uapi_common_nv_read(EXT_NV_FTM_FLASH_PARTIRION_TABLE_ID, table, sizeof(ext_flash_partition_table),
        EXT_TYPE_FACTORY_NV);
}
EXT_PRV td_u32 uapi_flash_self_check(ext_pt_change_log *inter_ctrl, ext_flash_partition_table *new_table,
    const ext_flash_partition_table *old_table, ext_nv_type *save_type)
{
    td_u32 ret;
    ext_nv_type type;

    ret = uapi_common_nv_read(EXT_NV_FTM_FLASH_PARTIRION_CHANGE_ID, inter_ctrl, sizeof(ext_pt_change_log),
        EXT_TYPE_FACTORY_NV);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    if (inter_ctrl->ctrl.status != REFRESH_BUSY) {
        ret = EXT_ERR_SUCCESS;
        return ret;
    }

    ret = uapi_common_nv_read(EXT_NV_FTM_FLASH_PARTIRION_TABLE_B_ID, (td_pvoid)new_table,
        sizeof(ext_flash_partition_table), EXT_TYPE_FACTORY_NV);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    if (inter_ctrl->ctrl.temp_area_blk != 0) {
        type = EXT_TYPE_TEMP;
        /* refresh the data */
        ret = uapi_temp_nv_init(inter_ctrl->ctrl.temp_area_blk * BLOCK_SIZE,
            old_table->table[EXT_FLASH_PARTITON_FACTORY_NV].addr_blk * BLOCK_SIZE);
        if (ret != EXT_ERR_SUCCESS) {
            return ret;
        }

        ret = uapi_common_nv_read(EXT_NV_FTM_FLASH_PARTIRION_CHANGE_ID, inter_ctrl, sizeof(ext_pt_change_log), type);
        if (ret != EXT_ERR_SUCCESS) {
            return ret;
        }
    } else {
        type = EXT_TYPE_FACTORY_NV;
    }

    *save_type = type;
    return ret;
}
EXT_PRV td_u32 uapi_flash_after_move(ext_pt_change_log *inter_ctrl, EXT_CONST ext_flash_partition_table *new_table)
{
    td_u32 ret;
    do {
        ret = uapi_common_nv_write(EXT_NV_FTM_FLASH_PARTIRION_TABLE_ID,
            (td_pvoid)new_table,
            sizeof(ext_flash_partition_table),
            EXT_TYPE_FACTORY_NV);
        if (ret != EXT_ERR_SUCCESS) {
            break;
        }

        inter_ctrl->ctrl.status = REFRESH_IDLE;
        ret = uapi_common_nv_write(EXT_NV_FTM_FLASH_PARTIRION_CHANGE_ID, inter_ctrl, sizeof(ext_pt_change_log),
            EXT_TYPE_FACTORY_NV); /* write nv partiton list one more time */
        if (ret != EXT_ERR_SUCCESS) {
            break;
        }

        ret = flash_partition_reset();
        if (ret != EXT_ERR_SUCCESS) {
            break;
        }
    } while (0);

    return ret;
}

EXT_PRV td_bool uapi_flash_partition_refresh_malloc(ext_pt_change_log **inter_ctrl,
    ext_flash_partition_table **new_table, td_u8 **buffer)
{
    if (inter_ctrl == TD_NULL || new_table == TD_NULL || buffer == TD_NULL) {
        return TD_FALSE;
    }
    *inter_ctrl = (ext_pt_change_log *)uapi_malloc(EXT_MOD_ID_PARTITION, sizeof(ext_pt_change_log));
    if (*inter_ctrl == TD_NULL) {
        return TD_FALSE;
    }
    *new_table = (ext_flash_partition_table *)uapi_malloc(EXT_MOD_ID_PARTITION, sizeof(ext_flash_partition_table));
    if (*new_table == TD_NULL) {
        uapi_free(EXT_MOD_ID_PARTITION, *inter_ctrl);
        return TD_FALSE;
    }
    *buffer = (td_u8 *)uapi_malloc(EXT_MOD_ID_PARTITION, BLOCK_SIZE);
    if (*buffer == TD_NULL) {
        uapi_free(EXT_MOD_ID_PARTITION, *new_table);
        uapi_free(EXT_MOD_ID_PARTITION, *inter_ctrl);
        return TD_FALSE;
    }
    return TD_TRUE;
}
#define ext_flash_partition_refresh_proc44()                                                            \
    do {                                                                                               \
        ext_flash_partition_refresh_proc2();                                                            \
        src_addr = (td_u32)((td_s32)src_addr + size_finished); /* not consider the process of error */ \
        dst_addr = (td_u32)((td_s32)dst_addr + size_finished);                                         \
    } while (0)

#define ext_flash_partition_refresh_proc45()                                                                       \
    do {                                                                                                           \
        inter_ctrl->table[i].move = TD_FALSE;                                                                      \
        ret = uapi_common_nv_write(EXT_NV_FTM_FLASH_PARTIRION_CHANGE_ID, inter_ctrl, sizeof(ext_pt_change_log),    \
            save_type); \
    } while (0)

#define ext_flash_partition_refresh_proc52() \
    do {                                    \
    } while (0)

EXT_PRV td_u32 uapi_flash_partition_refresh_proc55(ext_pt_change_log *inter_ctrl,
    ext_flash_partition_table *new_table, td_u8 *buffer, ext_nv_type save_type)
{
    ext_flash_partition_table *old_table = uapi_get_partition_table();
    td_u32 src_addr, dst_addr;
    td_s32 size_finished, size_per_time;
    td_u32 ret = EXT_ERR_SUCCESS;
    for (td_u8 i = 0; i < inter_ctrl->ctrl.total_num; i++) {
        td_u32 src_end_addr, dst_end_addr;
        td_u32 current_index = array_index(inter_ctrl->table[i].id);
        if ((inter_ctrl->table[i].move == TD_FALSE) ||
            (current_index >= EXT_FLASH_PARTITON_MAX)) {
            continue;
        }

        ext_flash_partition_refresh_make_address();
        /* not consider the change of version direction */
        if (((new_table->table[current_index].dir == 0) && (src_addr == dst_addr)) ||
            ((new_table->table[current_index].dir != 0) && (src_end_addr == dst_end_addr))) {
            continue;
        }
        /* select the smaller size to move */
        td_u32 total_size = (old_table->table[current_index].size_blk <= new_table->table[current_index].size_blk) ?
            (old_table->table[current_index].size_blk * BLOCK_SIZE) :
            (new_table->table[current_index].size_blk * BLOCK_SIZE);
        td_u32 abs_size_finished = (inter_ctrl->table[current_index].finished_block) * BLOCK_SIZE;
        ext_flash_partition_refresh_proc44();
        for (td_u32 len = abs_size_finished; len < total_size;) {
            ret = uapi_flash_read(src_addr, BLOCK_SIZE, buffer);
            if (ret != EXT_ERR_SUCCESS) {
                uapi_flash_partition_refresh_free_memery(inter_ctrl, new_table, buffer);
                return ret;
            }
            boot_msg2("addr ", src_addr, dst_addr);
            ret = uapi_flash_erase(dst_addr, BLOCK_SIZE);
            ret |= uapi_flash_write(dst_addr, BLOCK_SIZE, buffer, TD_FALSE);
            if (ret != EXT_ERR_SUCCESS) {
                uapi_flash_partition_refresh_free_memery(inter_ctrl, new_table, buffer);
                return ret;
            }
            ext_flash_partition_refresh_proc3();
            if (ret != EXT_ERR_SUCCESS) {
                uapi_flash_partition_refresh_free_memery(inter_ctrl, new_table, buffer);
                return ret;
            }
        }
        ext_flash_partition_refresh_proc45();
        if (ret != EXT_ERR_SUCCESS) {
            uapi_flash_partition_refresh_free_memery(inter_ctrl, new_table, buffer);
            return ret;
        }
    }
    return ret;
}


td_u32 uapi_flash_partition_refresh(td_void)
{
    td_u32 ret;
    ext_flash_partition_table *old_table = uapi_get_partition_table();
    ext_nv_type save_type;
    ext_pt_change_log *inter_ctrl = TD_NULL;
    ext_flash_partition_table *new_table = TD_NULL;
    td_u8 *buffer = TD_NULL;
    if (uapi_flash_partition_refresh_malloc(&inter_ctrl, &new_table, &buffer) != TD_TRUE) {
        return EXT_ERR_MALLOC_FAILUE;
    }

    do {
        /* start self check */
        ret = uapi_flash_self_check(inter_ctrl, new_table, old_table, &save_type);
        if ((ret != EXT_ERR_SUCCESS) || (inter_ctrl->ctrl.status != REFRESH_BUSY)) {
            break;
        }
        ret = uapi_flash_partition_refresh_proc55(inter_ctrl, new_table, buffer, save_type);
        if ((ret != EXT_ERR_SUCCESS)) {
            break;
        }

        ret = uapi_flash_after_move(inter_ctrl, new_table);
    } while (0);

    uapi_flash_partition_refresh_free_memery(inter_ctrl, new_table, buffer);
    return ret;
}

td_u32 uapi_flash_partition_init(td_void)
{
    td_u32 ret;
    ext_flash_partition_table *table = TD_NULL;

    (td_void)uapi_factory_nv_init(FACTORY_NV_ADDR);
    table = uapi_get_partition_table();
    ret = uapi_factory_nv_read(EXT_NV_FTM_FLASH_PARTIRION_TABLE_ID, table, sizeof(ext_flash_partition_table));
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    ret = uapi_flash_partition_refresh();
    return ret;
}
