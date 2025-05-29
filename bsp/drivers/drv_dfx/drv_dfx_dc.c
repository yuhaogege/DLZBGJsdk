/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: add data collect
 * Create: 2012-09-10
 */

#include "drv_dfx_cmn_inc.h"
#if defined PRODUCT_CFG_NDM_COLLECT_DATA
#include <drv_collectdata.h>
#include <soc_diag_async.h>
#include <dfx_diag_cmd_id.h>

#define EXT_DRV_DATA_COLLECT_MAX_SIZE (64 * 1024)
#define EXT_DRV_UPLOAD_DATA_ACK_SIZE 0X2800
#define EXT_DRV_UPLOAD_DATA_PACKET_SIZE 0X400
#define EXT_DRV_RECEIVE_DATA_PACKET_BLK_SIZE 300
#define EXT_DRV_RECEIVE_DATA_PACKET_TOTAL_SIZE 16384

dc_afe_backup g_afe_backup = { 0 };
typedef struct {
    ext_buf_s dc_info;
    td_pvoid dc_addr;
    td_pvoid malloc_addr;
} ext_dbk_dc_ctrl_s;
ext_dbk_dc_ctrl_s g_dbk_dc_ctrl;

typedef struct {
    td_u32 start_addr;
    td_u32 end_addr;
    td_u32 agc_gain; // 0xFFFFFFFF, NO fixed AGC
    td_u32 point_num;
    td_u32 time_out;
    td_u16 data_src;
    td_u16 triger_type;
} ext_diag_upload_start_req_s;

typedef struct {
    td_u32 return_ind;
    td_u32 total_len;
} ext_diag_upload_start_ind_s;

typedef struct {
    td_u32 receive_total_size;
    td_u16 receive_idx;
    td_u16 memget_flag;
    td_u16 receive_flag;
    td_u16 pad;
    td_void *data_addr;
} ext_diag_cache_mem_s;

ext_diag_cache_mem_s g_cache_mem = { 0 };
typedef struct {
    td_u32 file_total_size; // file total size
    td_u16 check_sum;       // Checksum
    td_u16 file_blk_size;   // Block size
    td_u16 index;           // Block index
    td_u16 pad;             // Block index
    td_u8 file_blk[0];      // data
} ext_send_transmit_blk_req_s;
typedef enum rceive_flag {
    E_RECEIVE_NULL = 0,
    E_RECEIVE_START,
    E_RECEIVE_ING,
    E_RECEIVE_DONE,
} ext_diag_rceive_flag_e;

EXT_PRV td_void dbk_dc_free_mem(td_void)
{
    if ((td_pvoid)g_dbk_dc_ctrl.malloc_addr != TD_NULL) {
        uapi_free(EXT_MOD_ID_DRV_DFX, g_dbk_dc_ctrl.malloc_addr);
        g_dbk_dc_ctrl.malloc_addr = TD_NULL;
    }
}
EXT_PRV td_pvoid dbk_dc_get_mem(td_u32 size)
{
    if (g_dbk_dc_ctrl.malloc_addr == TD_NULL) {
        if ((size != 0) && (size <= EXT_DRV_DATA_COLLECT_MAX_SIZE)) {
            g_dbk_dc_ctrl.malloc_addr = (td_pvoid)uapi_malloc(EXT_MOD_ID_DRV_DFX, size + ALIGNTYPE_64BYTE);
            if (g_dbk_dc_ctrl.malloc_addr != TD_NULL) {
                g_dbk_dc_ctrl.dc_addr =
                    (td_pvoid)((uintptr_t)align_length((uintptr_t)g_dbk_dc_ctrl.malloc_addr, ALIGNTYPE_64BYTE));
                memset_s(g_dbk_dc_ctrl.dc_addr, size, 0x77, size);
            }
        }
    }

    if (g_dbk_dc_ctrl.dc_addr == TD_NULL) {
        return TD_NULL;
    }

    return g_dbk_dc_ctrl.dc_addr;
}
td_u32 dbk_send_signal(EXT_IN td_pvoid cmd_param, td_u16 cmd_param_size)
{
    td_u32 ret = EXT_ERR_SUCCESS;
    ret = uapi_start_send_dc_signal((td_u8 *)cmd_param, cmd_param_size, &g_afe_backup);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    return ret;
}
td_void dbk_stop_send_signal(td_void)
{
    if (g_cache_mem.memget_flag == TD_TRUE) {
        dbk_dc_free_mem();
        g_cache_mem.memget_flag = TD_FALSE;
        g_cache_mem.data_addr = NULL;
        uapi_stop_send_dc_signal(&g_afe_backup);
    }
}
EXT_PRV td_u16 dc_calc_check_sum2(const td_pbyte ptr, td_u16 len)
{
    td_u16 check_sum = 0;

    if ((ptr != TD_NULL) && (len > 0)) {
        td_u16 i;
        for (i = 0; i < len; i++) {
            check_sum += ptr[i];
        }
    }
    return check_sum;
}

EXT_PRV td_u32 receive_cache_packet(EXT_IN td_pvoid cmd_param, td_u16 cmd_param_size)
{
    int ret = EXT_ERR_SUCCESS;

    ext_send_transmit_blk_req_s *pst_blk = (ext_send_transmit_blk_req_s *)cmd_param;
    ext_unref_param(cmd_param_size);
    td_u16 check_sum = dc_calc_check_sum2((td_u8 *)pst_blk->file_blk, pst_blk->file_blk_size);
    if (check_sum != pst_blk->check_sum) {
        return EXT_ERR_FAILURE;
    }

    if (pst_blk->index != (g_cache_mem.receive_idx + 1)) {
        return EXT_ERR_FAILURE;
    }
    if (g_cache_mem.receive_total_size + pst_blk->file_blk_size > pst_blk->file_total_size) {
        return EXT_ERR_FAILURE;
    }

    td_u8 *p = g_cache_mem.data_addr;
    p += g_cache_mem.receive_total_size;
    g_cache_mem.receive_idx = pst_blk->index;
    g_cache_mem.receive_total_size += pst_blk->file_blk_size;
    ret = memcpy_s((td_void *)p, pst_blk->file_blk_size, pst_blk->file_blk, pst_blk->file_blk_size);
    if (ret != EOK) {
        return EXT_ERR_MEMCPY_FAIL;
    }
    if (g_cache_mem.receive_total_size == pst_blk->file_total_size) {
        g_cache_mem.receive_idx = 0;
        g_cache_mem.receive_flag = E_RECEIVE_DONE;
        g_cache_mem.receive_total_size = 0;
    } else if (g_cache_mem.receive_total_size < pst_blk->file_total_size) {
        g_cache_mem.receive_flag = E_RECEIVE_ING;
    }
    return EXT_ERR_SUCCESS;
}
EXT_PRV td_u32 receive_data_config_prv(td_u16 us_id, EXT_IN td_pvoid cmd_param, td_u16 cmd_param_size, td_u32 option)
{
    ext_unref_param(option);
    ext_send_transmit_blk_req_s *pstBlk = (ext_send_transmit_blk_req_s *)cmd_param;
    if (us_id == ID_DIAG_CMD_DRV_RECEIVE_DATA_STOP) {
        dbk_stop_send_signal();

        return EXT_ERR_SUCCESS;
    }
    if (pstBlk->file_total_size > EXT_DRV_RECEIVE_DATA_PACKET_TOTAL_SIZE) {
        return EXT_ERR_FAILURE;
    }
    ext_diag_upload_start_ind_s ret_len = { 0 };

    if (!g_cache_mem.memget_flag) {
        g_cache_mem.data_addr = dbk_dc_get_mem(pstBlk->file_total_size);
        if (g_cache_mem.data_addr == NULL) {
            return EXT_ERR_FAILURE;
        }
        g_cache_mem.memget_flag = TD_TRUE;
    }
    ret_len.return_ind = receive_cache_packet(cmd_param, cmd_param_size);
    ret_len.total_len = pstBlk->file_blk_size;
    if (ret_len.return_ind != EXT_ERR_SUCCESS) {
        ret_len.total_len = pstBlk->index;
    }

    if (g_cache_mem.receive_flag == (td_u16)E_RECEIVE_DONE) {
        dbk_send_signal(g_cache_mem.data_addr, (td_u16)pstBlk->file_total_size);
        g_cache_mem.receive_flag = E_RECEIVE_NULL;
    }
    if (uapi_diag_report_packet(us_id, (td_u8)EXT_DIAG_CMD_INSTANCE_LOCAL, (td_pbyte)&ret_len,
        (td_u16)sizeof(ext_diag_upload_start_ind_s), TD_TRUE) != EXT_ERR_SUCCESS) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    return EXT_ERR_SUCCESS;
}
td_u32 receive_data_config(td_u16 us_id, EXT_IN td_pvoid cmd_param, td_u16 cmd_param_size, td_u8 option)
{
    return uapi_diag_async_cmd(us_id, cmd_param, cmd_param_size, option, receive_data_config_prv);
}

td_u32 upload_data(td_u16 us_id, EXT_IN td_pvoid cmd_param, td_u16 cmd_param_size, td_u8 option)
{
    ext_unref_param(cmd_param);
    ext_unref_param(cmd_param_size);
    ext_unref_param(option);
    /* if less than 1K, data is flipped, and record length of the first part of ulTransmitedLen. No flipping is 0. */
    td_u32 transmited_len = 0;
    td_u32 send_len = 0;
    td_u32 total_left = 0;
    total_left = g_dbk_dc_ctrl.dc_info.buf_size[0] + g_dbk_dc_ctrl.dc_info.buf_size[1];
    while (total_left > 0) {
        send_len = (total_left >= EXT_DRV_UPLOAD_DATA_PACKET_SIZE - transmited_len) ?
            (EXT_DRV_UPLOAD_DATA_PACKET_SIZE - transmited_len) :
            total_left;
        transmited_len = 0;

        /* data flipping: record length of the first part of ulTransmitedLen */
        if (send_len > g_dbk_dc_ctrl.dc_info.buf_size[0]) {
            send_len = g_dbk_dc_ctrl.dc_info.buf_size[0];
            transmited_len = send_len;
        }
        if (uapi_diag_report_packet(us_id, (td_u8)EXT_DIAG_CMD_INSTANCE_LOCAL,
            (td_pbyte)g_dbk_dc_ctrl.dc_info.buf_addr[0], (td_u16)send_len, TD_TRUE) != EXT_ERR_SUCCESS) {
            dbk_dc_free_mem();
            return EXT_ERR_INVALID_PARAMETER;
        }
        total_left -= send_len;
        g_dbk_dc_ctrl.dc_info.buf_size[0] -= send_len;
        g_dbk_dc_ctrl.dc_info.buf_addr[0] += send_len;


        /* update the contents of buf */
        if (0 == g_dbk_dc_ctrl.dc_info.buf_size[0]) {
            g_dbk_dc_ctrl.dc_info.buf_size[0] = g_dbk_dc_ctrl.dc_info.buf_size[1];
            g_dbk_dc_ctrl.dc_info.buf_addr[0] = g_dbk_dc_ctrl.dc_info.buf_addr[1];
            g_dbk_dc_ctrl.dc_info.buf_size[1] = 0;
            g_dbk_dc_ctrl.dc_info.buf_addr[1] = 0;
        }
        break;
    }
    if (total_left == 0) {
        dbk_dc_free_mem();
    }

    return EXT_ERR_SUCCESS;
}

EXT_PRV td_u32 upload_data_config_prv_proc(dc_input_para_stru *str_dc_para, ext_diag_upload_start_ind_s *ret_len,
    ext_diag_upload_start_req_s *p_rx)
{
    if (str_dc_para == TD_NULL || p_rx == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    if ((p_rx->end_addr < p_rx->start_addr) || (p_rx->end_addr - p_rx->start_addr > EXT_DRV_DATA_COLLECT_MAX_SIZE)) {
        ret_len->return_ind = EXT_ERR_INVALID_PARAMETER;
        return EXT_ERR_INVALID_PARAMETER;
    }
    td_pvoid ul_addr = dbk_dc_get_mem(p_rx->end_addr - p_rx->start_addr);
    if (ul_addr == TD_NULL) {
        ret_len->return_ind = EXT_ERR_MALLOC_FAILUE;
        return EXT_ERR_MALLOC_FAILUE;
    }
    str_dc_para->data_src = (dc_trig_data_enum)p_rx->data_src;
    str_dc_para->triger_type = (dc_trig_type_enum)p_rx->triger_type;
    str_dc_para->agc_gain = (td_s32)p_rx->agc_gain;
    str_dc_para->after_point_len = p_rx->point_num;
    str_dc_para->end_addr = (uintptr_t)ul_addr + (p_rx->end_addr - p_rx->start_addr);
    str_dc_para->start_addr = (uintptr_t)ul_addr;
    str_dc_para->wait_result_timeout = p_rx->time_out;
    str_dc_para->wait_envok_time = 1000; /* 1000 */
    return EXT_ERR_SUCCESS;
}

EXT_PRV td_u32 upload_data_config_prv(td_u16 us_id, EXT_IN td_pvoid cmd_param, td_u16 cmd_param_size, td_u32 option)
{
    td_u32 ulRet = EXT_ERR_SUCCESS;
    ext_diag_upload_start_ind_s ret_len = { 0 };
    dc_input_para_stru strDcPara = { 0 };
    (td_void)memset_s((td_void *)&g_dbk_dc_ctrl.dc_info, sizeof(g_dbk_dc_ctrl.dc_info), 0, sizeof(ext_buf_s));
    if (us_id == ID_DIAG_CMD_DRV_UPLOAD_DATA_STOP) {
        dbk_dc_free_mem();
        return EXT_ERR_SUCCESS;
    }
    ext_unref_param(option);
    ext_diag_upload_start_req_s *pRx = (ext_diag_upload_start_req_s *)cmd_param;
    td_u32 procRet = upload_data_config_prv_proc(&strDcPara, &ret_len, pRx);
    if (procRet != EXT_ERR_SUCCESS) {
        goto fail;
    }
    if (EXT_ERR_SUCCESS != (ulRet = uapi_dc_start_normal_data_collect(&strDcPara, &g_dbk_dc_ctrl.dc_info))) {
        ret_len.return_ind = ulRet;
        goto fail;
    }
    ret_len.return_ind = EXT_ERR_SUCCESS;
    ret_len.total_len = g_dbk_dc_ctrl.dc_info.buf_size[0] + g_dbk_dc_ctrl.dc_info.buf_size[1];
    if (uapi_diag_report_packet(us_id, (td_u8)EXT_DIAG_CMD_INSTANCE_LOCAL, (td_pbyte)&ret_len,
        (td_u16)sizeof(ext_diag_upload_start_ind_s), TD_TRUE) != EXT_ERR_SUCCESS) {
        (td_void)memset_s((td_void *)&g_dbk_dc_ctrl.dc_info, sizeof(g_dbk_dc_ctrl.dc_info), 0, sizeof(ext_buf_s));
        dbk_dc_free_mem();
        return EXT_ERR_INVALID_PARAMETER;
    }
    uapi_sleep(10);
    if (EXT_ERR_SUCCESS == upload_data(ID_DIAG_CMD_DRV_UPLOAD_DATA, cmd_param, cmd_param_size, (td_u8)option)) {
        return EXT_ERR_SUCCESS;
    } else {
        ret_len.return_ind = EXT_ERR_INVALID_RAM_ADDR;
    }
fail:
    dbk_dc_free_mem();
    (td_void)memset_s((td_void *)&g_dbk_dc_ctrl.dc_info, sizeof(g_dbk_dc_ctrl.dc_info), 0, sizeof(ext_buf_s));
    if (uapi_diag_report_packet(us_id, (td_u8)EXT_DIAG_CMD_INSTANCE_LOCAL, (td_pbyte)&ret_len,
        (td_u16)sizeof(ext_diag_upload_start_ind_s), TD_TRUE) != EXT_ERR_SUCCESS) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    return ret_len.return_ind;
}

td_u32 upload_data_config(td_u16 us_id, EXT_IN td_pvoid cmd_param, td_u16 cmd_param_size, td_u8 option)
{
    return uapi_diag_async_cmd(us_id, cmd_param, cmd_param_size, option, upload_data_config_prv);
}

#endif
