/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: implementation of error handling of system errors.
 * Author: CompanyName
 * Create: 2012-07-14
 */

#include "soc_system_error.h"
#include "sal_common.h"

#include <dfx_sal.h>
#include <sal_inf.h>
#include <soc_partition_table.h>
#include <soc_sal_nv.h>
#include <sal_os_dfx.h>
#include <soc_mdm_watchdog.h>
#include <soc_mdm_task.h>
#include <soc_flash_prv.h>
#include <securec.h>
#include <soc_mdm_time.h>
#include <soc_mdm_ver.h>
#include <soc_mdm_crc.h>
#include <los_hwi.h>
#include <los_exc.h>
#include <los_hwi.h>
#include <soc_asm_func.h>

#define BLOCK_SIZE PRODUCT_CFG_FLASH_BLOCK_SIZE
#define STACK_INFO_ADDR 0x800 /* Stack information is stored relative to the relative address of the dead memory */
#define STACK_INFO_SIZE (BLOCK_SIZE - STACK_INFO_ADDR)
#define PEND_SV_ERR 14

typedef struct {
    td_u32 type;
    td_u32 addr;
    td_u32 end_addr;
} ext_stack_info_head;

typedef struct {
    unsigned msp;
    unsigned psp;
    unsigned exc_return_lr;
    unsigned fault_type;
    unsigned pri_mask;
    unsigned r0;
    unsigned r1;
    unsigned r2;
    unsigned r3;
    unsigned r12;
    unsigned lr;
    unsigned pc;
    unsigned xpsr;
    unsigned r4;
    unsigned r5;
    unsigned r6;
    unsigned r7;
    unsigned r8;
    unsigned r9;
    unsigned r10;
    unsigned r11;
    unsigned sp;
} osa_crash_reg_info;

static td_bool g_power_down = TD_FALSE;
static ext_syserr_exc_callback g_syserr_exc_user_hook;
#define EXT_WRITE_CRASH_INFO_PER_SIZE 512

EXT_VOLATILE ext_syserr_info g_sys_err_val;
#define sys_err_ptr() ((ext_syserr_info *)(&g_sys_err_val))

static ext_task_info_s g_task_info;

__isr td_u32 flash_read_crash(EXT_IN td_u32 addr, EXT_IN td_void *data, EXT_IN td_u32 size)
{
    return uapi_flash_read(addr, size, data);
}

__isr td_u32 flash_write_crash(EXT_IN td_u32 addr, EXT_IN EXT_CONST td_void *data, EXT_IN td_u32 size)
{
    return uapi_flash_write(addr, size, data, TD_FALSE);
}

__isr td_u32 flash_erase_crash(EXT_IN td_u32 addr, EXT_IN td_u32 size)
{
    return uapi_flash_erase(addr, size);
}

__isr td_char *get_eid_str(ext_syserr_id eid)
{
    if (eid == EXT_EID_FAULT_IN_ISR) {
        return EXT_EXC_IN_ISR_STR;
    } else if (eid == EXT_EID_FAULT_IN_TASK) {
        return EXT_EXC_IN_TASK_STR;
    } else if (eid == EXT_EID_SYS_REBOOT) {
        return EXT_EXC_SYS_REBOOT_STR;
    } else {
        return EXT_EXC_RESERVE_STR;
    }
}

#ifdef PRODUCT_CFG_SUPPORT_FLASH_PROTECT
__isr td_void set_protect_info(td_void)
{
    ext_flash_protect_ctrl *info;

    info = uapi_flash_protect_get_ctrl_info();

    sys_err_ptr()->protect_info.enable = info->enable;
    sys_err_ptr()->protect_info.current_block = info->current_block;
    sys_err_ptr()->protect_info.status_reg = info->flash_status_reg;
}
#endif

__isr td_void set_basic_info(ext_syserr_id eid)
{
    /* Unnecessary to check eid, internal control */
    sys_err_ptr()->basic_info.eid = eid;
    (td_void)strncpy_s(sys_err_ptr()->basic_info.type_name, EXT_EXC_TYPE_NAME_SIZE, get_eid_str(eid),
        EXT_EXC_TYPE_NAME_SIZE - 1);
    sys_err_ptr()->basic_info.type_name[EXT_EXC_TYPE_NAME_SIZE - 1] = 0;

    sys_err_ptr()->basic_info.crash_tsec = (td_u32)(uapi_get_tick64() / LOSCFG_BASE_CORE_TICK_PER_SECOND);
    sys_err_ptr()->basic_info.boot_ver = uapi_get_boot_ver();

    EXT_CONST ext_product_info *pst = uapi_get_product_info();
    if (sprintf_s(sys_err_ptr()->basic_info.kernel_ver, sizeof(sys_err_ptr()->basic_info.kernel_ver), "%s,%s,%s",
        pst->product_version, pst->build_date, pst->build_time) < 0) {
        return;
    }
}

__isr EXT_PRV td_void set_os_usage_info(td_void)
{
    EXT_CONST ext_mem_pool_crash_info_s *mem_info = TD_NULL;
    (td_void)uapi_os_get_resource_status(&(sys_err_ptr()->os_info.usage));

    mem_info = uapi_mem_get_sys_info_crash();
    if (mem_info != TD_NULL) {
        memcpy_s(&(sys_err_ptr()->os_info.mem), sizeof(sys_err_ptr()->os_info.mem), mem_info,
            sizeof(ext_mem_pool_crash_info_s));
    }
}

__isr EXT_PRV td_void set_remap_reg_info(td_void)
{
    for (td_u32 loop = 0; loop < 4; loop++) { /* 4 array */
        /* Offset 2 bytes */
        sys_err_ptr()->remap_info.reg_blockmap[loop] = reg_read_val(EXT_MMU_REG_BASE + (loop << 2));
    }
}

__isr td_void syserr_save_task_info(td_u32 task_id)
{
    ext_task_info_s *task_info = &g_task_info;

    sys_err_ptr()->os_info.task.id = task_id;
    if (uapi_task_get_info(task_id, task_info) == EXT_ERR_SUCCESS) {
        if (memcpy_s(sys_err_ptr()->os_info.task.name, EXT_EXC_TASK_NAME_SIZE, task_info->name,
            uapi_min(EXT_EXC_TASK_NAME_SIZE, sizeof(task_info->name))) != EOK) { }
        sys_err_ptr()->os_info.task.stack_size = (td_u16)task_info->stack_size;
        sys_err_ptr()->os_info.task.status = task_info->status;
        sys_err_ptr()->os_info.task.sp = task_info->sp;
        sys_err_ptr()->os_info.task.stack[0] = task_info->top_of_stack;
        sys_err_ptr()->os_info.task.stack[1] = task_info->bottom_of_stack;
        sys_err_ptr()->os_info.task.stack_peak = (td_u16)task_info->peak_used;
    }
}

__isr EXT_PRV td_void syserr_save_cur_task_info(td_void)
{
    td_u32 id;
    id = uapi_task_get_current_id();
    if (id != EXT_INVALID_TASK_ID) {
        syserr_save_task_info(id);
    }
}

__isr EXT_PRV td_void set_isr_info(td_void)
{
    sys_err_ptr()->os_info.task.id = g_cur_interrupt_idx;
    sys_err_ptr()->os_info.task.sp = 0xAABBCCDD; /* Easy to check if sp is valid */
    sys_err_ptr()->os_info.task.stack_size =
        (td_u16)((td_u32)(uintptr_t)(&__SYSTEM_STACK_END__) - (td_u32)(uintptr_t)(&__SYSTEM_STACK_BEGIN__));
    sys_err_ptr()->os_info.task.stack[0] = (td_u32)(uintptr_t)&__SYSTEM_STACK_BEGIN__;
    sys_err_ptr()->os_info.task.stack[1] = (td_u32)(uintptr_t)&__SYSTEM_STACK_END__;
}

__isr td_void syserr_save_short_stack(td_u32 real_esp)
{
    td_u32 *ptr = TD_NULL;
    td_s32 i;
    td_u32 pre_sp = (td_u32)(real_esp - (EXT_SYS_ESP_PRE_DATA_MAX_NUM * sizeof(td_u32)));

    sys_err_ptr()->os_info.task.stack_data_index = EXT_SYS_ESP_PRE_DATA_MAX_NUM;

    sys_err_ptr()->os_info.task.real_sp = real_esp;

    ptr = (td_u32 *)(uintptr_t)pre_sp;

    for (i = 0; i < EXT_SYS_ESP_PRE_DATA_MAX_NUM; i++) {
        ptr++;
        if ((uintptr_t)ptr >= EXT_PERMANENT_MEM_START_ADDR &&
            (uintptr_t)ptr <= (EXT_PERMANENT_MEM_END_ADDR - sizeof(td_u32))) {
            sys_err_ptr()->os_info.task.stack_pre_data[i] = *ptr;
        } else {
            sys_err_ptr()->os_info.task.stack_pre_data[i] = 0x11223344; /* Easy to check if value is valid */
        }
    }

    ptr = (td_u32 *)(uintptr_t)real_esp;
    for (i = 0; i < EXT_SYS_ESP_DATA_MAX_NUM; i++) {
        ptr++;
        if ((uintptr_t)ptr >= EXT_PERMANENT_MEM_START_ADDR &&
            (uintptr_t)ptr <= (EXT_PERMANENT_MEM_END_ADDR - sizeof(td_u32))) {
            sys_err_ptr()->os_info.task.stack_data[i] = *ptr;
        } else {
            sys_err_ptr()->os_info.task.stack_data[i] = 0x11223344; /* Easy to check if value is valid */
        }
    }
}

__isr EXT_PRV td_u32 syserr_get_save_stack_info(td_u32 *save_st_addr, td_u32 *save_end_addr, td_u32 *save_type)
{
    syserr_task_info *ptask = (syserr_task_info *)(&(sys_err_ptr()->os_info.task));
    td_u32 stack_top = ptask->stack[0];
    td_u32 stack_bottom = ptask->stack[1];

    if (stack_top >= stack_bottom || stack_top < EXT_PERMANENT_MEM_START_ADDR ||
        stack_bottom > (EXT_PERMANENT_MEM_END_ADDR - sizeof(td_u32))) {
        /* The bottom of the stack or the top of the stack is illegal */
        if (sys_err_ptr()->os_info.task.real_sp >= EXT_PERMANENT_MEM_START_ADDR &&
            sys_err_ptr()->os_info.task.real_sp <= (EXT_PERMANENT_MEM_END_ADDR - sizeof(td_u32))) {
            /* SP running at legal address range, stores the stack bases on SP */
            *save_type = 1; /* Type 1: Store the stack info from real_sp to stack bottom */
            *save_st_addr = sys_err_ptr()->os_info.task.real_sp;
            *save_end_addr = uapi_min(sys_err_ptr()->os_info.task.real_sp + STACK_INFO_SIZE,
                (td_u32)(EXT_PERMANENT_MEM_END_ADDR - sizeof(td_u32)));
        } else {
            *save_type = 2; /* Type 2: Store nothing, cause stack information and sp are illegal */
            *save_st_addr = 0;
            *save_end_addr = 0;
            return EXT_ERR_FAILURE;
        }
    } else {
        /* Stack bottom and stack top are legal */
        if (stack_bottom - stack_top <= STACK_INFO_SIZE) {
            /* Enough space in flash to store the whole stack */
            *save_type = 3; /* Type 3: Store the whole stack, cause flash space is enough */
            *save_st_addr = stack_top;
            *save_end_addr = stack_bottom;
        } else {
            /* flash space is not enough */
            if (sys_err_ptr()->os_info.task.real_sp >= stack_top &&
                sys_err_ptr()->os_info.task.real_sp <= stack_bottom) { /* flash space */
                *save_type = 4;                                        /* Type 4: Store a part of stack info, */
                /* cause there isn't enough space left to store the whole stack. */
                *save_end_addr = uapi_min(sys_err_ptr()->os_info.task.real_sp + STACK_INFO_SIZE, stack_bottom);
                *save_st_addr = *save_end_addr - STACK_INFO_SIZE;
            } else {            /* SP is illegal, starting from the stack top */
                *save_type = 5; /* Type 5: Store the stack from the stack top, cause sp running at illegal address. */
                *save_st_addr = stack_top;
                *save_end_addr = stack_top + STACK_INFO_SIZE;
            }
        }
    }

    if ((*save_st_addr <= EXT_PERMANENT_MEM_START_ADDR) ||
        (*save_end_addr >= EXT_PERMANENT_MEM_END_ADDR - sizeof(td_u32)) || (*save_st_addr >= *save_end_addr) ||
        (*save_end_addr - *save_st_addr > STACK_INFO_SIZE)) {
        *save_type = 6; /* Type 6: Don't store stack info, cause stack bottom and stack top are at illegal addresses. */
        return EXT_ERR_FAILURE;
    }

    return EXT_ERR_SUCCESS;
}

__isr EXT_PRV td_void syserr_save_stack_info(ext_syserr_id eid, td_u16 rid)
{
    unsigned int fault_from = (rid & 0xFF);
    unsigned int fault_reason = ((rid & 0xFF00) >> 8); /* Get high 8 bits */
    syserr_reg_info *reg_info = &(sys_err_ptr()->reg_info);

    if (fault_from == OS_EXC_CAUSE_FROM_SVC || fault_reason == OS_EXC_CAUSE_SVC) {
        if (reg_info->r[0] == 0x02000218 || reg_info->r[0] == 0x02000223) { /* Task stack check error */
            /* r[1] passes uwTaskID which from LOS_TASK_CB, find LOS_TASK_CB top pStackPointer, offset 16 */
            LOS_TASK_CB *task_tcb = (LOS_TASK_CB *)((uintptr_t)reg_info->r[1] - 16); /* 16 offset from sp to taskid */
            /* Store stack info while Stack across the boundary */
            syserr_save_short_stack(task_tcb->uwTopOfStack);
            syserr_save_task_info(task_tcb->uwTaskID);
            return;
        }
    }

    if (eid == EXT_EID_FAULT_IN_ISR) {
        syserr_save_short_stack(reg_info->sp);
        set_isr_info(); /* Store system stack information */
        return;
    } else {
        syserr_save_short_stack(reg_info->sp);
        syserr_save_cur_task_info(); /* Store current task info */
        return;
    }
}

__isr td_void uapi_syserr_save_cmn_info_step1(ext_syserr_id eid, td_u16 rid)
{
    sys_err_ptr()->sig_s = EXT_SYS_ERR_SIGNATURE;
    sys_err_ptr()->sig_e = EXT_SYS_ERR_SIGNATURE;
    sys_err_ptr()->power_magic = EXT_ALL_F_32;

    sys_err_ptr()->basic_info.rid = rid;
    sys_err_ptr()->basic_info.eid = eid;
    sys_err_ptr()->basic_info.crash_tsec = (td_u32)(uapi_get_tick64() / LOSCFG_BASE_CORE_TICK_PER_SECOND);
    sys_err_ptr()->basic_info.boot_ver = uapi_get_boot_ver();

    /* Converts eid to a crash notice string */
    strncpy_s(sys_err_ptr()->basic_info.type_name,
              EXT_EXC_TYPE_NAME_SIZE, get_eid_str(eid),
              EXT_EXC_TYPE_NAME_SIZE - 1);
    sys_err_ptr()->basic_info.type_name[EXT_EXC_TYPE_NAME_SIZE - 1] = 0;

    /* Store the kernel version */
    EXT_CONST ext_product_info *pst = uapi_get_product_info();
    if (sprintf_s(sys_err_ptr()->basic_info.kernel_ver, sizeof(sys_err_ptr()->basic_info.kernel_ver), "%s,%s,%s",
        pst->product_version, pst->build_date, pst->build_time) < 0) {
        return;
    }
}

__isr td_void uapi_syserr_save_cmn_info_step2(ext_syserr_id eid, td_u16 rid)
{
    ext_flash_partition_table *table = uapi_get_partition_table();
    ext_unref_param(eid);
    ext_unref_param(rid);

    td_u8 *data = (td_u8 *)sys_err_ptr() + ext_field_offset(ext_syserr_info, basic_info);
    td_u16 len = sizeof(ext_syserr_info) - ext_field_offset(ext_syserr_info, basic_info);
    sys_err_ptr()->crc_val = uapi_crc16(0, data, len);
    flash_erase_crash(table->table[EXT_FLASH_PARTITON_CRASH_INFO].addr_blk * BLOCK_SIZE,
        table->table[EXT_FLASH_PARTITON_CRASH_INFO].size_blk * BLOCK_SIZE);

    flash_write_crash(table->table[EXT_FLASH_PARTITON_CRASH_INFO].addr_blk * BLOCK_SIZE, (td_pvoid)sys_err_ptr(),
        sizeof(ext_syserr_info));
}

__isr td_void uapi_syserr_save_fault_info_step1(ext_syserr_id eid, td_u16 rid)
{
    set_os_usage_info(); /* Storage system resource usage */

#ifdef PRODUCT_CFG_SUPPORT_FLASH_PROTECT
    set_protect_info(); /* Store flash protection information */
#endif

    uapi_get_track_info(&(sys_err_ptr()->track_info));

    set_remap_reg_info(); /* Store remap information */

    syserr_save_stack_info(eid, rid);
}

__isr EXT_PRV td_void syserr_save_total_stack(td_u32 flash_st_addr, td_u32 ram_st_addr, td_u32 write_size)
{
    td_u32 copyed_size = 0;
    td_u32 this_size;

    /*
     * Note: The exception stack cannot be stored directly through the flash API,
     * because the contents of the exception stack will change at the same time when the exception stack is stored.
     * This causes fail to do the FLASH readback comparison, and return as an exception.
     * Eventually, only part of the stack content can be stored.
     */
    while (copyed_size < write_size) {
        this_size = uapi_min(write_size - copyed_size, EXT_WRITE_CRASH_INFO_PER_SIZE);
        if (memcpy_s((td_pvoid)sys_err_ptr(), sizeof(ext_syserr_info), (td_pvoid)((uintptr_t)ram_st_addr + copyed_size),
            this_size) != EOK) {
            break;
        }
        (td_void)flash_write_crash(flash_st_addr + copyed_size, (td_pvoid)sys_err_ptr(), this_size);
        copyed_size += this_size;
    }
}

__isr td_void uapi_syserr_save_fault_info_step2(td_void)
{
    td_u32 ret;
    ext_stack_info_head stack_info = {
        0,
    };
    ext_flash_partition_table *table = uapi_get_partition_table();

    ret = syserr_get_save_stack_info(&stack_info.addr, &stack_info.end_addr, &stack_info.type);
    if (ret == EXT_ERR_SUCCESS) {
        flash_write_crash((td_u32)(table->table[EXT_FLASH_PARTITON_CRASH_INFO].addr_blk * BLOCK_SIZE + STACK_INFO_ADDR -
            sizeof(stack_info)),
            (td_pvoid)&stack_info, sizeof(stack_info));

        syserr_save_total_stack(table->table[EXT_FLASH_PARTITON_CRASH_INFO].addr_blk * BLOCK_SIZE + STACK_INFO_ADDR,
            stack_info.addr, stack_info.end_addr - stack_info.addr);
    }
}

__isr td_u32 uapi_syserr_get(ext_syserr_info *info)
{
    td_u32 ret;
    td_u16 crc_val;
    ext_flash_partition_table *table = uapi_get_partition_table();

    td_u8 *data = TD_NULL;
    td_u16 len;

    if (info == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    ret = uapi_flash_read(table->table[EXT_FLASH_PARTITON_CRASH_INFO].addr_blk * BLOCK_SIZE, sizeof(ext_syserr_info),
        (td_u8 *)info);
    info->power_down = g_power_down;
    if (ret != EXT_ERR_SUCCESS) {
        info->crc_usable = TD_FALSE;
        info->basic_info.now_tsec = uapi_get_seconds();
        return ret;
    }
    if ((info->sig_s != EXT_SYS_ERR_SIGNATURE) || (info->sig_e != EXT_SYS_ERR_SIGNATURE)) {
        info->crc_usable = TD_FALSE;
        info->basic_info.now_tsec = uapi_get_seconds();
        return EXT_ERR_NOT_FOUND;
    }
    data = (td_u8 *)sys_err_ptr() + ext_field_offset(ext_syserr_info, basic_info);
    len = sizeof(ext_syserr_info) - ext_field_offset(ext_syserr_info, basic_info);
    crc_val = uapi_crc16(0, data, len);
    if (crc_val == info->crc_val) {
        info->crc_usable = TD_TRUE;
    }
    info->basic_info.now_tsec = uapi_get_seconds();

    return EXT_ERR_SUCCESS;
}

__isr EXT_PRV td_void temp_watchdog_func(td_u32 data)
{
    ext_unref_param(data);
    return;
}

__isr td_u32 uapi_syserr_wdg_init(td_bool enable, td_u32 time, td_bool record_crash_info)
{
    td_u32 ret = EXT_ERR_SUCCESS;
    ext_wdg_mode_e mode = (record_crash_info == TD_TRUE) ? EXT_WDG_INTER : EXT_WDG_ERR;

    if (enable) {
        ret = uapi_watchdog_register(mode, temp_watchdog_func, time);
        if (ret == EXT_ERR_SUCCESS) {
            sys_err_ptr()->wdg_info.enable = TD_TRUE;
            sys_err_ptr()->wdg_info.time_ms = 0x7FFFFFFF & time;
        }

        if (mode == EXT_WDG_INTER) {
            m_pstHwiForm[WatchDog_IRQn + OS_SYS_VECTOR_CNT] = (HWI_PROC_FUNC)osExcWatchDog;
        }
    } else {
        uapi_watchdog_disable();
        sys_err_ptr()->wdg_info.enable = TD_FALSE;
    }
    return ret;
}

td_void hardware_init(td_void)
{
    td_u32 reg_val;
    reg_val = reg_read_val(DW21_SYSCTRL_SC_HW_ID_REG);
    sys_err_ptr()->hw_info.hw_id = reg_val & ((1 << SYSCTRL_HW_ID_LEN) - 1);
    sys_err_ptr()->hw_info.jtag_mode = (reg_val & (1 << SYSCTRL_JTAG_EN_OFFSET)) ? 1 : 0;
    reg_val = reg_read_val(DW21_SYSCTRL_SC_VERSION_ID_REG);
    sys_err_ptr()->hw_info.chip_ = (reg_val & (1 << SYSCTRL_TYPE_ID_OFFSET)) ? 1 : 0;
}

td_void uapi_syserr_init(td_void)
{
    td_u32 temp_size = 0;
    ext_flash_partition_table *table = uapi_get_partition_table();
    if (!uint_2_multiply(table->table[EXT_FLASH_PARTITON_CRASH_INFO].addr_blk, BLOCK_SIZE, &temp_size)) {
        return;
    }
    td_u32 addr = temp_size + ext_field_offset(ext_syserr_info, power_magic);
    td_u32 power_magic = 0;

    sys_err_ptr()->basic_info.log_ver = EXT_SYSERR_VERSION;
    sys_err_ptr()->info_len = sizeof(ext_syserr_info);
    hardware_init();
    flash_read_crash(addr, &power_magic, sizeof(power_magic));
    /* This is specially designed to write only when EXT_NO_POWER_DOWN, to avoid
    individual bit errors causing write repeatedly at power-on */
    if (power_magic == EXT_NO_POWER_DOWN) {
        power_magic = EXT_POWER_DOWN;
        flash_write_crash(addr, &power_magic, sizeof(power_magic));
    } else {
        g_power_down = TD_TRUE;
    }
}

__isr ext_syserr_info *uapi_syserr_ptr(td_void)
{
    return (sys_err_ptr());
}

td_void uapi_syserr_exc_register(ext_syserr_exc_callback func)
{
    g_syserr_exc_user_hook = func;
}

typedef struct {
    td_u8 times;
} ext_crash_ctrl;

ext_crash_ctrl g_crash_ctrl = { 0 };

__isr EXT_PRV td_void sys_err_make_core_info(td_void)
{
    sys_err_ptr()->core_info.cur_task_id = (td_u16)uapi_get_cur_task_id();
    sys_err_ptr()->core_info.new_task_id = (td_u16)uapi_get_new_task_id();
    UAPI_REG_READ32(0xE000ED04, sys_err_ptr()->core_info.reg_icsr); /* Interrupt Control and Status Register ICSR */
    UAPI_REG_READ32(0xE000ED28, sys_err_ptr()->core_info.reg_fsr);
    UAPI_REG_READ32(0xE000ED2C, sys_err_ptr()->core_info.reg_hfsr);
    UAPI_REG_READ32(0xE000ED38, sys_err_ptr()->core_info.reg_bfar); /* bus fault address register */
    /* System Handler control and status register (some abnormal activity in the suspension) */
    UAPI_REG_READ32(0xE000ED24, sys_err_ptr()->core_info.reg_shcsr);
    UAPI_REG_READ32(0xE000E200, sys_err_ptr()->core_info.reg_isr_pend);   /* Interrupt enable */
    UAPI_REG_READ32(0xE000E300, sys_err_ptr()->core_info.reg_isr_active); /* Interrupt enable */
}

__isr EXT_PRV td_void sys_err_make_reg_info(EXT_CONST osa_crash_reg_info *m3_info)
{
    if (m3_info == TD_NULL) {
        return;
    }

    do {
        sys_err_ptr()->reg_info.pc = m3_info->pc;
        sys_err_ptr()->reg_info.sp = m3_info->sp;
        sys_err_ptr()->reg_info.msp = m3_info->msp;
        sys_err_ptr()->reg_info.psp = m3_info->psp;
        sys_err_ptr()->reg_info.exc_return_lr = m3_info->exc_return_lr;
        sys_err_ptr()->reg_info.xpsr = m3_info->xpsr;
        sys_err_ptr()->reg_info.pri_mask = m3_info->pri_mask;
        sys_err_ptr()->reg_info.r[0] = m3_info->r0;   /* 0th working register */
        sys_err_ptr()->reg_info.r[1] = m3_info->r1;   /* 1st working register */
        sys_err_ptr()->reg_info.r[2] = m3_info->r2;   /* 2nd working register */
        sys_err_ptr()->reg_info.r[3] = m3_info->r3;   /* 3rd working register */
        sys_err_ptr()->reg_info.r[4] = m3_info->r4;   /* 4th working register */
        sys_err_ptr()->reg_info.r[5] = m3_info->r5;   /* 5th working register */
        sys_err_ptr()->reg_info.r[6] = m3_info->r6;   /* 6th working register */
        sys_err_ptr()->reg_info.r[7] = m3_info->r7;   /* 7th working register */
        sys_err_ptr()->reg_info.r[8] = m3_info->r8;   /* 8th working register */
        sys_err_ptr()->reg_info.r[9] = m3_info->r9;   /* 9th working register */
        sys_err_ptr()->reg_info.r[10] = m3_info->r10; /* 10th working register */
        sys_err_ptr()->reg_info.r[11] = m3_info->r11; /* 11th working register */
        sys_err_ptr()->reg_info.r[12] = m3_info->r12; /* 12th working register */
        sys_err_ptr()->reg_info.r[13] = m3_info->sp;  /* 13th working register */
        sys_err_ptr()->reg_info.r[14] = m3_info->lr;  /* 14th working register */
    } while (0);
}

__isr EXT_PRV td_void sys_err_conver_eid_rid(td_u32 crash_from, ext_syserr_id *eid, td_u16 *rid)
{
    td_u8 fault_from;
    td_u8 fault_reason;
    syserr_core_info *core_info = &(sys_err_ptr()->core_info);
    syserr_reg_info *reg_info = &(sys_err_ptr()->reg_info);

    if (reg_info->psp == reg_info->sp) {
        *eid = EXT_EID_FAULT_IN_TASK;
    } else {
        *eid = EXT_EID_FAULT_IN_ISR;
    }
    reg_info->sp += 32; /* When crashing, m3 pushes in 8 registers for a total of 32 bytes. */

    if (crash_from == OS_EXC_CAUSE_FROM_SVC) {
        fault_from = OS_EXC_CAUSE_FROM_SVC;
        fault_reason = 0xFF;
        if (reg_info->r[0] == 0x02000218 || reg_info->r[0] == 0x02000223) {
            *eid = EXT_EID_FAULT_IN_TASK;
        }
    } else if (crash_from == OS_EXC_CAUSE_FROM_WATCH_DOG) {
        fault_from = OS_EXC_CAUSE_FROM_WATCH_DOG;
        fault_reason = OS_EXC_CAUSE_FROM_WATCH_DOG;
    } else {
        /* OS_EXC_CAUSE_HARDFAULT,OS_EXC_CAUSE_NMI,OS_EXC_CAUSE_BUSFAULT */
        fault_from = (td_u8)crash_from;
        if (core_info->reg_hfsr & 0x80000000) {
            fault_reason = OS_EXC_CAUSE_DEBUGEVT;
        } else if (core_info->reg_hfsr & 0x00000002) {
            fault_reason = OS_EXC_CAUSE_VECTBL;
        } else {
            td_u32 reg_val = ((core_info->reg_fsr) & (~((1 << 15) | (1 << 7)))); /* Clear low 15th and lower 7th */
            td_u32 x = uapi_clz(reg_val);
            if (x == 32) {                       /* When crashing, m3 pushes in 8 registers for a total of 32 bytes. */
                fault_reason = OS_EXC_CAUSE_SVC; /* illegal branch */
            } else {
                fault_reason = g_uwExcTbl[x];
            }
            if (x == 32 && /* When crashing, m3 pushes in 8 registers for a total of 32 bytes */
                (reg_info->r[0] == 0x02000218 || reg_info->r[0] == 0x02000223)) {
                *eid = EXT_EID_FAULT_IN_TASK;
            }
        }
    }
    *rid = (((td_u16)fault_from) | ((td_u16)fault_reason << 8)); /* Take the lower 8 digits */
}

__isr td_void osa_exc_hook(td_u32 crash_from, EXT_CONST osa_crash_reg_info *reg_info)
{
    ext_syserr_id eid = EXT_EID_RESERVE;
    td_u16 rid = 0;

    if (g_crash_ctrl.times > 0) {
        while (1) { }
    }
    g_crash_ctrl.times++;

    sys_err_make_reg_info(reg_info);
    sys_err_make_core_info();
    sys_err_conver_eid_rid(crash_from, &eid, &rid);

    uapi_watchdog_feed();
    uapi_syserr_save_cmn_info_step1(eid, rid);
    uapi_syserr_save_fault_info_step1(eid, rid);
    uapi_syserr_save_cmn_info_step2(eid, rid);
    uapi_syserr_save_fault_info_step2();

    if (g_syserr_exc_user_hook) {
        g_syserr_exc_user_hook();
    }

    uapi_watchdog_register(EXT_WDG_NORMAL, TD_NULL, 500); /* deley 500ms */
    while (1) { } /* System error, blocking infinite loop PC_LINT alarm */
}
