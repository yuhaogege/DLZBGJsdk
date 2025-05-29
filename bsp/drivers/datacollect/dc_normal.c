/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: add data collect
 * Create: 2012-09-10
 */
#include <soc_types.h>
#include <soc_mdm_types.h>
#if defined PRODUCT_CFG_NDM_COLLECT_DATA

#include <drv_collectdata.h>
#include <soc_mdm_sem.h>

// interface lock
typedef struct dc_info {
    td_u32 sem_id;
    td_char name[8]; // name 8
    td_bool is_init;
    td_bool pad[3]; // pad 3
} st_dc_info;

st_dc_info g_dc_info_st = {
    0,
};

static inline td_void dc_interface_mutex_lock(td_void)
{
    uapi_sem_wait(g_dc_info_st.sem_id, EXT_SYS_WAIT_FOREVER);
}

static inline td_void dc_interface_mutex_unlock(td_void)
{
    uapi_sem_signal(g_dc_info_st.sem_id);
}

td_u32 uapi_dc_init(td_void)
{
    if (g_dc_info_st.is_init) {
        return EXT_ERR_FAILURE;
    }

    td_u32 ret = uapi_sem_bcreate(&(g_dc_info_st.sem_id), g_dc_info_st.name, EXT_SEM_ONE);
    if (ret != EXT_ERR_SUCCESS) {
        g_dc_info_st.is_init = TD_FALSE;
        return ret;
    }
    g_dc_info_st.is_init = TD_TRUE;

    return ret;
}

td_u32 uapi_dc_start_normal_data_collect(EXT_IN dc_input_para_stru *dc_input, EXT_OUT ext_buf_s *s_buf)
{
    td_u32 ret = EXT_ERR_SUCCESS;
    if (dc_input == TD_NULL || s_buf == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    if (g_dc_info_st.is_init == TD_FALSE) {
        return EXT_ERR_INITILIZATION;
    }

    dc_interface_mutex_lock();

    ret = dc_data_collect(dc_input, s_buf, EXT_DRV_DATA_COLLECT_STEP_ALL);

    dc_interface_mutex_unlock();
    return ret;
}
#endif
