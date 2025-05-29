/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Implements system reset information store.
 * Author: CompanyName
 * Create: 2012-07-14
 */
#include "sal_res.h"
#include "sal_sys.h"

#include <sal_common.h>
#include <dw21_platform_rom.h>
#include <soc_mdm.h>
EXT_CPP_START

#define EXT_RST_TIMES_DELAY_STORAGE_TIME 30000

typedef struct {
    td_bool enable_save;
    td_u8 rsv[3]; /* Reserved 3B */
    td_u32 rst_times;
    ext_timer_handle_s handle;
} sal_rst_times_ctrl;

sal_rst_times_ctrl g_rst_times_ctrl = { TD_TRUE, { 0, 0, 0 }, 0, { { 0, 0 } } };

td_void rst_times_timer_callback(td_u32 param);
td_void rst_times_hisr(td_u32 data1, td_u32 data2, td_u32 data3, td_u32 data4, td_u32 data5);

/* Get system information, including system status, version number, etc. */
ext_sys_status_info *uapi_get_sys_info(td_void)
{
    return &(get_sal_ctx()->status_info);
}

/* Get Date Value td_u32 */
#define SZ_SIZE 16
static td_u32 get_datetime_value(EXT_CONST td_char *datetime, td_s8 len)
{
    td_char sz[SZ_SIZE];
    td_char *ptr = sz;
    td_u32 i = 0;

    (td_void)memset_s(sz, sizeof(sz), 0, sizeof(sz));
    (td_void)memcpy_s((td_void *)sz, (td_u32)sizeof(sz), (td_void *)datetime, (td_u8)len);
    while (*ptr == '0') {
        ptr++;
        if (i++ > SZ_SIZE) {
            break;
        }
    }

    return str2s32(ptr);
}

#define ver_to_bcd(v, r, spc) ((((v)&0xF) << 12) | (((r)&0xF) << 8) | ((spc)&0xFF))

/* build Version Initialization */
td_void uapi_init_build_version(td_void)
{
    sal_ctx_stru *ctx = get_sal_ctx();
    ext_sys_status_info *status_info = &ctx->status_info;
    ext_ue_product_ver *prod_ver = (ext_ue_product_ver *)sal_get_product_version();
    ext_product_info *prod_info = (ext_product_info *)uapi_get_product_info();

    status_info->sys_mode = ctx->sys_mode;
    status_info->boot_ver_idx = (td_u8)(UAPI_REG_READ_VAL32(DW21_SYSCTRL_SC_GEN_REG1_REG) & 0xFF);
    status_info->chip_version = 0;
    status_info->version_v = (td_u8)prod_ver->version_v;
    status_info->version_r = (td_u8)prod_ver->version_r;
    status_info->version_c = (td_u8)prod_ver->version_c;
    status_info->version_b = prod_ver->version_b;
    status_info->version_spc = prod_ver->version_spc;

    status_info->build_datetime[0] =                             /* the 0th digit is the year */
        (td_u8)get_datetime_value(&prod_info->build_date[1], 3); /* start 1st byte, length 3 */
    status_info->build_datetime[1] =                             /* the first digit is the month */
        (td_u8)get_datetime_value(&prod_info->build_date[5], 2); /* start 5th byte, length 2 */
    status_info->build_datetime[2] =                             /* the 2 digit is the day */
        (td_u8)get_datetime_value(&prod_info->build_date[8], 2); /* start 8th byte, length 2 */
    status_info->build_datetime[3] =                             /* the 3 digit is */
        (td_u8)get_datetime_value(&prod_info->build_time[0], 2); /* start 0th byte, length 2 */
    status_info->build_datetime[4] =                             /* the 4th bit is minutes */
        (td_u8)get_datetime_value(&prod_info->build_time[3], 2); /* start 3rd byte, length 2 */
    status_info->build_datetime[5] =                             /* the 5th digit is seconds */
        (td_u8)get_datetime_value(&prod_info->build_time[6], 2); /* start 6th byte, length 2 */

    status_info->boot_cause = EXT_ND_SYS_BOOT_CAUSE_NORMAL;
    status_info->boot_ver = status_info->boot_ver_idx;
    status_info->sw_ver = ver_to_bcd(status_info->version_v / 100, status_info->version_r, status_info->version_spc);
    status_info->year = status_info->build_datetime[0];
    status_info->month = status_info->build_datetime[1];
    status_info->day = status_info->build_datetime[2]; /* The 2 place is the day */
    status_info->manu_code[0] = 'W';
    status_info->manu_code[1] = 'D'; /* 8 */
    /* Move 8 bits to the right
    to take the high position */
    status_info->chip_code[0] = '1';
    status_info->chip_code[1] = 'P';
}

td_u32 uapi_rst_times_init(td_void)
{
    td_u32 ret;

    if (g_rst_times_ctrl.enable_save == TD_TRUE) {
        ext_sys_reset_times nv;

        ret = uapi_nv_read(EXT_NV_SYS_RST_TIMES, &nv, sizeof(ext_sys_reset_times));
        if (ret != EXT_ERR_SUCCESS) {
            return EXT_ERR_FAILURE;
        }
        g_rst_times_ctrl.rst_times = nv.rst_times + 1;
        uapi_timer_start(&g_rst_times_ctrl.handle, rst_times_timer_callback, EXT_RST_TIMES_DELAY_STORAGE_TIME,
            EXT_TIMER_TYPE_ONCE, 0);
    } else {
        g_rst_times_ctrl.rst_times = uapi_get_random_num32(0, EXT_ALL_F_32);
    }

    return EXT_ERR_SUCCESS;
}

__isr td_void rst_times_timer_callback(td_u32 param)
{
    ext_unref_param(param);
    uapi_irq_add_job(EXT_JOB_PRIORITY_15, rst_times_hisr, 0, 0, 0, 0, 0);
}

__isr td_void rst_times_hisr(td_u32 data1, td_u32 data2, td_u32 data3, td_u32 data4, td_u32 data5)
{
    ext_unref_param(data1);
    ext_unref_param(data2);
    ext_unref_param(data3);
    ext_unref_param(data4);
    ext_unref_param(data5);

    if (g_rst_times_ctrl.enable_save == TD_TRUE) {
        ext_sys_reset_times nv;
        nv.rst_times = g_rst_times_ctrl.rst_times;
        uapi_nv_write(EXT_NV_SYS_RST_TIMES, &nv, sizeof(ext_sys_reset_times));
    }
}

td_u32 uapi_get_rst_times(td_void)
{
    return g_rst_times_ctrl.rst_times;
}

td_void uapi_enable_reset_times_save(td_bool enable)
{
    g_rst_times_ctrl.enable_save = enable;
}

td_u32 uapi_get_user_info(ext_mdm_user_info *user_info)
{
    ext_sys_status_info *status_info = &get_sal_ctx()->status_info;

    if (user_info == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    (td_void)memset_s(user_info, sizeof(ext_mdm_user_info), 0, sizeof(ext_mdm_user_info));

    user_info->sw_ver = status_info->sw_ver;
    user_info->boot_ver = status_info->boot_ver;
    user_info->year = status_info->year;
    user_info->month = status_info->month;
    user_info->day = status_info->day;
    user_info->chip_code[0] = status_info->chip_code[0];
    user_info->chip_code[1] = status_info->chip_code[1];
    user_info->manu_code[0] = status_info->manu_code[0];
    user_info->manu_code[1] = status_info->manu_code[1];

    return EXT_ERR_SUCCESS;
}

td_u32 uapi_set_user_info(EXT_CONST ext_mdm_user_info *user_info)
{
    ext_sys_status_info *status_info = &get_sal_ctx()->status_info;

    if (user_info == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    status_info->sw_ver = user_info->sw_ver;
    status_info->boot_ver = user_info->boot_ver;
    status_info->year = user_info->year;
    status_info->month = user_info->month;
    status_info->day = user_info->day;
    status_info->chip_code[0] = user_info->chip_code[0];
    status_info->chip_code[1] = user_info->chip_code[1];
    status_info->manu_code[0] = user_info->manu_code[0];
    status_info->manu_code[1] = user_info->manu_code[1];

    return EXT_ERR_SUCCESS;
}

EXT_CPP_END
