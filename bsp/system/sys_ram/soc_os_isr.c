/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: soc_os_isr.c
 */
#include <soc_types_rom.h>
#include <los_hwi.h>
#include <soc_mdm_isr.h>
#include <errno.h>

extern td_u32 g_int_enable_status;
/*
  * 中断注册补丁函数
  * 调整优先级，支持看门狗优先级高于其他中断，用于定位中断中死循环问题
 */
__hot td_u32 uapi_irq_request_patch(td_u32 vector, td_u16 priority, lisr_proc_func routine, td_u32 param)
{
    td_u32 ret = LOS_OK;
    td_u16 pri = 2;

    if (TD_NULL == routine) {
        return EXT_ROM_ERR_ISR_INVALID_PARAM;
    }

    if (vector == WatchDog_IRQn) {
        pri = 1;
    }
    ret = LOS_HwiCreate(vector, pri, 0, (HWI_PROC_FUNC)routine, param);
    if (LOS_OK != ret) {
        return EXT_ROM_ERR_ISR_REQ_IRQ_FAIL;
    } else { /* LOS_HwiCreate中有调用NVIC_EnableIRQ，所以此处记录 */
        g_int_enable_status |= 1 << vector;
    }

    return EXT_ROM_ERR_SUCCESS;
}
#pragma GCC diagnostic pop

__hot td_u32 uapi_irq_free_patch(td_u32 vector)
{
    td_u32 ret = LOS_OK;

    /* los接口内判断了vector合法性 */
    ret = LOS_HwiDelete(vector);
    if (LOS_OK != ret) {
        return EXT_ROM_ERR_ISR_DEL_IRQ_FAIL;
    } else {
        g_int_enable_status &= ~(1 << vector);
    }

    return EXT_ROM_ERR_SUCCESS;
}

__hot td_void uapi_irq_enable_patch(td_u32 vector)
{
    td_u32 int_val;

    if (vector >= OS_HWI_MAX_NUM) {
        return;
    }
    int_val = LOS_IntLock();
    g_int_enable_status |= 1 << vector;
    NVIC_EnableIRQ((IRQn_Type)vector);
    LOS_IntRestore(int_val);
}

__hot td_void uapi_irq_disable_patch(td_u32 vector)
{
    td_u32 int_val;

    if (vector >= OS_HWI_MAX_NUM) {
        return;
    }
    int_val = LOS_IntLock();
    g_int_enable_status &= ~(1 << vector);
    NVIC_DisableIRQ((IRQn_Type)vector);
    LOS_IntRestore(int_val);
}

