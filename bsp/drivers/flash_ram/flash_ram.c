/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: FLASH drive adjustment. Flash_ram.c code
 * Author: CompanyName
 * Create: 2012-06-08
 */
#include "flash_ram.h"
#include "flash_prv.h"

#include <soc_mdm_types.h>
#include <soc_mdm_flash.h>
#include <soc_stdlib.h>
#include <soc_mdm_isr.h>
#include <soc_mdm_mux.h>
#include <soc_mdm_timer.h>
#include <soc_ft_nv.h>
#include <soc_mdm_ver.h>
#include <soc_partition_table.h>
#include <soc_flash_prv.h>
#include <soc_lowpower.h>
#ifdef PRODUCT_CFG_SUPPORT_FLASH_PROTECT
#include <soc_mdm_timer.h>
#include <soc_ft_nv.h>
#include <soc_mdm_isr.h>
#include <soc_mdm_flash_protect.h>
#endif

#define BLOCK_SIZE 0x1000
EXT_SPI_FLASH_CTRL_S g_flash_drv_ctrl = {
    0,
};

EXT_CONST EXT_SPI_FLASH_BASIC_INFO_S g_flash_usr_info_tbl[] = {
    { "UNKNOWN", {0, }, _16M, EXT_FLASH_SUPPORT_DEFAULT, 0, 0},
    { "W25Q40", {0xEF, 0x40, 0x13, }, _512K, EXT_FLASH_SUPPORT_ERASE464CHIP, 3, 0},
    { "GD25Q40", {0xC8, 0x40, 0x13, }, _512K, EXT_FLASH_SUPPORT_ERASE464CHIP, 3, 0},
    { "W25Q16", {0xEF, 0x40, 0x15, }, _2M, EXT_FLASH_SUPPORT_ERASE464CHIP, 3, 0},
};

#define BACK_BUFFER_SIZE 1024
#define DMA_BUFFER_SIZE 256
td_u32 g_back_buffer[BACK_BUFFER_SIZE] = { 0 };
td_u32 g_dma_buffer[DMA_BUFFER_SIZE] = { 0 };

flash_prepare_func g_flash_prepare_func;
flash_restore_func g_flash_restore_func;

#define FLASH_READ_TICK_MAX 50
#define FLASH_WRITE_TICK_MAX 500
#define FLASH_ERASE_TICK_MAX 1000

/* Flash protection feature code */
#ifdef PRODUCT_CFG_SUPPORT_FLASH_PROTECT

#define REG_PROTECT_FLASH (DW21_SYSCTRL_SC_GEN_REG3_REG)
/* The highest bit indicates whether flash protection is supported */
#define SUPPORT_PROTECT_FLASH ((td_u32)0x1 << 31)
#define PROTECT_TIMEOUT_1 6000    /* Volatile write flash status register, timeout value seconds */
#define PROTECT_TIMEOUT_2 3600000 /* Non-volatile write flash status register, timeout value hourly */

typedef struct {
    td_u32 cmp_bp : 6;
    td_u32 block : 13;
    td_u32 reseve : 13;
} ext_flash_protect_size;

ext_flash_protect_ctrl g_protect_ctrl = {
    0,
};

/*
 * 1. Different chip settings are different, please re-confirm the form when replacing the flash chip;
 * 2.bit settings refer to the Flash manual memory protection section, this table refers to W25Q40C and GD25Q40C;
 * 3. It can be streamlined or refined according to the actual space;
 */
EXT_CONST ext_flash_protect_size g_flash_protect_size[] = {
    /* bit[5]:cmp
     * bit[4]:sec:  1:4KB 0:64KB
     * bit[3]:tb:    0:top  1:bottom
     * bit[2:0]:bp[2:0]: See device manual
     */
    { 0b000000, 0x0,  0 },  /* not protected */
    { 0b011001, 0x1,  0 },  /* Protect 4KB */
    { 0b011010, 0x2,  0 },  /* Protect 8KB */
    { 0b011011, 0x4,  0 },  /* Protect 16KB */
    { 0b011100, 0x8,  0 },  /* Protect 32KB */
    { 0b001001, 0x10, 0 }, /* Protect 64KB */
    { 0b001010, 0x20, 0 }, /* Protect 128KB */
    { 0b100011, 0x40, 0 }, /* Protect 256KB */
    { 0b100010, 0x60, 0 }, /* Protect 384KB (512-128) */
    { 0b100001, 0x70, 0 }, /* Protection 448KB (512-64) */
    { 0b110110, 0x78, 0 }, /* Protect 480KB (512-32) */
    { 0b110011, 0x7C, 0 }, /* Protection 496KB (512-16) */
    { 0b110010, 0x7E, 0 }, /* Protect 504KB(512-8) */
    { 0b011111, 0x80, 0 }, /* Protect 512KB */
};

td_u32 flash_protect_config(td_u32 ep_addr, td_u32 timeout);
td_u32 flash_volatile_protect_config(td_u32 ep_addr, td_u32 timeout);

td_void flash_lock(td_void)
{
    if (!uapi_is_int_context()) {
        uapi_mux_pend(g_flash_drv_ctrl.mutex_handle, EXT_SYS_WAIT_FOREVER);
    }
}

td_void flash_unlock(td_void)
{
    if (!uapi_is_int_context()) {
        uapi_mux_post(g_flash_drv_ctrl.mutex_handle);
    }
}

EXT_PRV td_u32 spi_flash_write_sr_reg(td_u8 cmd, td_u8 *data, td_u8 data_len, td_bool is_volatile)
{
    td_u32 temp_data = 0;
    td_u32 ret;

    /* Internal pass parameters, the judgment return value is omitted here */
    (td_void)memcpy_s(&temp_data, sizeof(temp_data), data, data_len);

    if (is_volatile) {
        ext_sfc_write(SFC_REG_CMD_INS, SPI_CMD_VSR_WREN);
        ext_sfc_write(SFC_REG_CMD_CONFIG, (td_u32)(SFC_CMD_CONFIG_SEL_CS | SFC_CMD_CONFIG_START));
        ret = spif_wait_config_start();
        if (ret != EXT_ROM_ERR_SUCCESS) {
            return ret;
        }
    } else {
        ret = spif_write_enable(TD_TRUE);
        if (ret) {
            return ret;
        }
    }

    ext_sfc_write(SFC_REG_CMD_INS, cmd);
    ext_sfc_write(SFC_REG_CMD_DATABUF1, temp_data);

    ext_sfc_write(SFC_REG_CMD_CONFIG,
        SFC_CMD_CONFIG_SEL_CS | SFC_CMD_CONFIG_DATA_EN | SFC_CMD_CONFIG_DATA_CNT(data_len) | SFC_CMD_CONFIG_START);

    ret = spif_wait_config_start();
    if (ret != EXT_ROM_ERR_SUCCESS) {
        return ret;
    }

    return EXT_ROM_ERR_SUCCESS;
}
#define SPI_REG_DATA_COUNT 2
#define SPI_CMP_BP_SHIFT_RIGHT_COUNT 5
#define SPI_FLASH_STATUS_REG_SHIFT_LEFT_COUNT 8
#define SPI_FLASH_WRITE_SR_REG_BP_SHIFT_LEFT_COUNT 2
#define SPI_FLASH_WRITE_SR_REG_CMP_SHIFT_LEFT_COUNT 6
td_u32 spi_flash_set_protect(td_u8 cmp_bp, td_bool is_volatile)
{
    td_u32 ret;
    td_u8 data[SPI_REG_DATA_COUNT] = { 0 };
    td_u8 cmp = (cmp_bp >> SPI_CMP_BP_SHIFT_RIGHT_COUNT) & 0x1;
    td_u8 bp = cmp_bp & 0x1F;
    ext_flash_protect_ctrl *ctrl = &g_protect_ctrl;

    ret = spi_flash_read_reg(SPI_CMD_RDSR, &data[0], 1);
    if (ret != EXT_ROM_ERR_SUCCESS) {
        return ret;
    }
    ret = spi_flash_read_reg(SPI_CMD_RDSR2, &data[1], 1);
    if (ret != EXT_ROM_ERR_SUCCESS) {
        return ret;
    }
    ctrl->flash_status_reg = data[0] | (data[1] << SPI_FLASH_STATUS_REG_SHIFT_LEFT_COUNT);
    if (((data[0] & (0x1F << SPI_FLASH_WRITE_SR_REG_BP_SHIFT_LEFT_COUNT)) ==
        (bp << SPI_FLASH_WRITE_SR_REG_BP_SHIFT_LEFT_COUNT)) &&
        ((data[1] & (0x1 << SPI_FLASH_WRITE_SR_REG_CMP_SHIFT_LEFT_COUNT)) ==
        (cmp << SPI_FLASH_WRITE_SR_REG_CMP_SHIFT_LEFT_COUNT))) {
        return EXT_ROM_ERR_SUCCESS;
    }

    data[0] &= ~(0x1f << SPI_FLASH_WRITE_SR_REG_BP_SHIFT_LEFT_COUNT);
    data[0] |= (td_u8)(bp << SPI_FLASH_WRITE_SR_REG_BP_SHIFT_LEFT_COUNT);
    data[1] &= ~(0x1 << SPI_FLASH_WRITE_SR_REG_CMP_SHIFT_LEFT_COUNT);
    data[1] |= (td_u8)(cmp << SPI_FLASH_WRITE_SR_REG_CMP_SHIFT_LEFT_COUNT);
    ret = spi_flash_write_sr_reg(SPI_CMD_WRSR1, data, SPI_REG_DATA_COUNT, is_volatile);
    if (ret != EXT_ROM_ERR_SUCCESS) {
        return ret;
    }

    ret = spi_flash_read_reg(SPI_CMD_RDSR, &data[0], 1);
    if (ret != EXT_ROM_ERR_SUCCESS) {
        return ret;
    }
    ret = spi_flash_read_reg(SPI_CMD_RDSR2, &data[1], 1);
    if (ret != EXT_ROM_ERR_SUCCESS) {
        return ret;
    }
    ctrl->flash_status_reg = data[0] | (data[1] << SPI_FLASH_STATUS_REG_SHIFT_LEFT_COUNT);
    if (((data[0] & (0x1F << SPI_FLASH_WRITE_SR_REG_BP_SHIFT_LEFT_COUNT)) ==
        (bp << SPI_FLASH_WRITE_SR_REG_BP_SHIFT_LEFT_COUNT)) &&
        ((data[1] & (0x1 << SPI_FLASH_WRITE_SR_REG_CMP_SHIFT_LEFT_COUNT)) ==
        (cmp << SPI_FLASH_WRITE_SR_REG_CMP_SHIFT_LEFT_COUNT))) {
        return EXT_ROM_ERR_SUCCESS;
    } else {
        return EXT_ERR_FLASH_PROTECT_WRITE_STATUS_REG;
    }
}

EXT_PRV td_void flash_protect_job(td_u32 data_1, td_u32 data_2, td_u32 data_3, td_u32 data_4, td_u32 data_5)
{
    td_bool is_volatile = (td_bool)data_1;
    ext_flash_protect_ctrl *ctrl = &g_protect_ctrl;
    td_u32 ret;

    ext_unref_param(data_2);
    ext_unref_param(data_3);
    ext_unref_param(data_4);
    ext_unref_param(data_5);
    if (is_volatile) {
        ret = flash_volatile_protect_config(0xFFFFFFFF, 0);
    } else {
        ret = flash_protect_config(0xFFFFFFFF, 0);
    }
    if (ret != EXT_ERR_SUCCESS) {
        ctrl->job_run_errs++;
    }
}

EXT_PRV td_void flash_protect_timeout(td_u32 is_volatile)
{
    td_u32 ret;
    ext_flash_protect_ctrl *ctrl = &g_protect_ctrl;

    ret = uapi_irq_add_job(EXT_JOB_PRIORITY_15, flash_protect_job, (td_u32)is_volatile, 0, 0, 0, 0);
    if (ret != EXT_ERR_SUCCESS) {
        ctrl->job_add_errs++;
    }
    ctrl->timer_started = TD_FALSE;
}

EXT_PRV td_u32 get_timer_val(ext_flash_protect_type type)
{
    if (type == EXT_FLASH_PROTECT_TYPE_1) {
        return PROTECT_TIMEOUT_1;
    } else if (type == EXT_FLASH_PROTECT_TYPE_2) {
        return PROTECT_TIMEOUT_2;
    } else {
        return 0;
    }
}

EXT_PRV td_u32 flash_protect(td_u32 addr, td_u32 timeout, td_bool is_volatile, td_bool to_reduce_times)
{
    td_u32 ret;
    td_s8 i;
    ext_flash_protect_ctrl *ctrl = &g_protect_ctrl;

    if (ctrl->enable == TD_FALSE) {
        return EXT_ERR_SUCCESS; /* Returned successfully without being enabled */
    }
    td_u32 block = (addr / BLOCK_SIZE);
    if (to_reduce_times && block >= ctrl->current_block) {
        /* To erase/write the address has been unlocked */
        return EXT_ERR_SUCCESS;
    }
    for (i = (sizeof(g_flash_protect_size) / sizeof(ext_flash_protect_size)) - 1; i >= 0; i--) {
        if (block >= g_flash_protect_size[i].block) {
            ctrl->protected_times++;
            ret = spi_flash_set_protect(g_flash_protect_size[i].cmp_bp, is_volatile);
            if (ret != EXT_ERR_SUCCESS) {
                ctrl->protect_errs++;
                return ret;
            }

            ctrl->current_block = g_flash_protect_size[i].block;

            break;
        }
    }
    if (i < 0) {
        ctrl->protect_errs++;
        return EXT_ERR_NOT_FOUND;
    }
    if (timeout) {
        if (timeout == PROTECT_TIMEOUT_AUTO) {
            ctrl->timer_timeout = get_timer_val((ext_flash_protect_type)(ctrl->default_type));
        }
        ret = uapi_timer_start(&(ctrl->timer_handle), (timer_proc_func)flash_protect_timeout, ctrl->timer_timeout,
            EXT_TIMER_TYPE_ONCE, is_volatile);
        if (ret != EXT_ERR_SUCCESS) {
            ctrl->start_timer_errs++;
            return ret;
        }
        ctrl->timer_started = TD_TRUE;
        ctrl->timer_created = TD_TRUE;
    } else {
        if ((ctrl->timer_created == TD_TRUE) || (ctrl->timer_started == TD_TRUE)) {
            (td_void)uapi_timer_stop(&(ctrl->timer_handle));
            ctrl->timer_started = TD_FALSE;
        }
    }
    return EXT_ERR_SUCCESS;
}

td_u32 uapi_flash_protect_init(ext_flash_protect_type type)
{
    ext_flash_protect_ctrl *ctrl = &g_protect_ctrl;

    if (type >= EXT_FLASH_PROTECT_TYPE_MAX) {
        return EXT_ERR_INVALID_PARAMETER;
    }
    ctrl->default_type = type;

    return EXT_ERR_SUCCESS;
}

td_u32 uapi_flash_protect_enable(td_bool enable)
{
    ext_flash_protect_ctrl *ctrl = &g_protect_ctrl;

    flash_lock();
    if (enable) {
        td_u32 reg_val;

        if (ctrl->default_type == EXT_FLASH_PROTECT_NONE) {
            flash_unlock();
            return EXT_ERR_NOT_SUPPORT;
        }
        reg_val = UAPI_REG_READ_VAL32(REG_PROTECT_FLASH);
        if (!(reg_val & SUPPORT_PROTECT_FLASH)) {
            flash_unlock();
            return EXT_ERR_FLASH_PROTECT_BOOT_COMPATIBILITY;
        }
        ctrl->enable = TD_TRUE;
    } else {
        flash_protect_config(0, 0); /* Release all protection */
        ctrl->enable = TD_FALSE;
    }
    flash_unlock();
    return EXT_ERR_SUCCESS;
}

td_u32 flash_protect_config(td_u32 ep_addr, td_u32 timeout)
{
    td_u32 ret;

    flash_lock();
    ret = flash_protect(ep_addr, timeout, TD_FALSE, TD_FALSE);
    flash_unlock();
    return ret;
}

td_u32 flash_volatile_protect_config(td_u32 ep_addr, td_u32 timeout)
{
    td_u32 ret;
    ext_flash_protect_ctrl *ctrl = &g_protect_ctrl;

    flash_lock();
    if (ctrl->default_type != EXT_FLASH_PROTECT_TYPE_1) {
        flash_unlock();
        return EXT_ERR_NOT_SUPPORT;
    }
    ret = flash_protect(ep_addr, timeout, TD_TRUE, TD_FALSE);
    flash_unlock();

    return ret;
}

td_u32 uapi_flash_protect_config(td_u32 ep_addr, td_u32 timeout)
{
    ext_flash_protect_ctrl *ctrl = &g_protect_ctrl;

    if (ctrl->enable == TD_FALSE) {
        return EXT_ERR_NOT_SUPPORT;
    }

    return flash_protect_config(ep_addr, timeout);
}

td_u32 uapi_flash_volatile_protect_config(td_u32 ep_addr, td_u32 timeout)
{
    ext_flash_protect_ctrl *ctrl = &g_protect_ctrl;

    if (ctrl->enable == TD_FALSE) {
        return EXT_ERR_NOT_SUPPORT;
    }

    return flash_volatile_protect_config(ep_addr, timeout);
}

ext_flash_protect_ctrl *uapi_flash_protect_get_ctrl_info(td_void)
{
    return &g_protect_ctrl;
}

#endif /* PRODUCT_CFG_SUPPORT_FLASH_PROTECT */

EXT_PRV td_u32 flash_check_ram_addr_size(EXT_CONST td_u32 addr, td_u32 size)
{
    if (addr >= EXT_PERMANENT_MEM_END_ADDR && addr < EXT_DYNAMIC_MEM_VIR_END_ADDR) {
        return EXT_ERR_FLASH_INVALID_RAM_ADDR;
    }

    if (addr + size > EXT_PERMANENT_MEM_END_ADDR && addr + size <= EXT_DYNAMIC_MEM_VIR_END_ADDR) {
        return EXT_ERR_FLASH_INVALID_RAM_ADDR;
    }
    return EXT_ERR_SUCCESS;
}
/*
 * Quickly read the flash interface for missing pages, requiring:
 * The FLASH and RAM addresses are aligned in 4 bytes.
 * Internal address writes use physical addresses instead of virtual addresses.
 */
td_u32 uapi_flash_fast_read(td_u32 flash_addr, td_u32 flash_read_size, td_u8 *flash_read_data)
{
    td_u32 ret;
    EXT_SPI_FLASH_CTRL_S *spif_ctrl = &g_flash_drv_ctrl;

    if (spif_ctrl->bInit != TD_TRUE) {
        return EXT_ERR_NO_INITILIZATION;
    }

    flash_lock();
    ret = spif_ctrl->p_read(spif_ctrl, flash_addr, flash_read_size, flash_read_data, TD_FALSE);
    flash_unlock();

    return ret;
}

td_u32 uapi_flash_erase_in(EXT_IN td_u32 flash_addr, EXT_IN td_u32 flash_erase_size)
{
    td_u32 ret = EXT_ERR_FAILURE;
    EXT_SPI_FLASH_CTRL_S *spif_ctrl = &g_flash_drv_ctrl;

    ret = sfc_check_para(spif_ctrl, flash_addr, flash_erase_size, EXT_FLASH_CHECK_PARAM_OPT_ERASE);
    if (ret != EXT_ERR_SUCCESS) {
        flash_info_print("uapi_flash_erase ret1:%x\r\n", ret);
        return ret;
    }

    flash_lock();
    if (g_flash_prepare_func) {
        g_flash_prepare_func();
    }
#ifdef PRODUCT_CFG_SUPPORT_FLASH_PROTECT
    ret = flash_protect(flash_addr, PROTECT_TIMEOUT_AUTO, TD_TRUE, TD_TRUE);
    if (ret != EXT_ERR_SUCCESS) {
        if (g_flash_restore_func) {
            g_flash_restore_func();
        }
        flash_unlock();
        return ret;
    }
#endif /* PRODUCT_CFG_SUPPORT_FLASH_PROTECT */
    ret = flash_erase_prv(spif_ctrl, flash_addr, flash_erase_size);
    if (g_flash_restore_func) {
        g_flash_restore_func();
    }
    flash_unlock();

    flash_info_print("uapi_flash_erase ret:%x addr:%x len:%x\r\n", ret, flash_addr, flash_erase_size);
    return ret;
}

td_u32 uapi_flash_write_in(EXT_CONST td_u32 flash_addr, td_u32 flash_write_size, EXT_CONST td_u8 *flash_write_data,
    td_bool do_erase)
{
    td_u32 ret = EXT_ERR_FAILURE;
    EXT_SPI_FLASH_CTRL_S *spif_ctrl = &g_flash_drv_ctrl;

    if (flash_write_data == TD_NULL) {
        flash_info_print("write pBuf\r\n");
        return EXT_ERR_INVALID_PARAMETER;
    }

    ret = flash_check_ram_addr_size((td_u32)(uintptr_t)flash_write_data, flash_write_size);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    ret = sfc_check_para(spif_ctrl, flash_addr, flash_write_size, EXT_FLASH_CHECK_PARAM_OPT_WRITE);
    if (ret != EXT_ERR_SUCCESS) {
        flash_info_print("uapi_flash_write ret1:%x\r\n", ret);
        return ret;
    }

    flash_lock();
    uapi_lowpower_forbid(EXT_LOWPOWER_ID_FLASH, EXT_SLEEP_DEEP);
    if (g_flash_prepare_func) {
        g_flash_prepare_func();
    }
#ifdef PRODUCT_CFG_SUPPORT_FLASH_PROTECT
    ret = flash_protect(flash_addr, PROTECT_TIMEOUT_AUTO, TD_TRUE, TD_TRUE);
    if (ret != EXT_ERR_SUCCESS) {
        if (g_flash_restore_func) {
            g_flash_restore_func();
        }
        uapi_lowpower_unforbid(EXT_LOWPOWER_ID_FLASH, EXT_SLEEP_DEEP, FLASH_WRITE_TICK_MAX);
        flash_unlock();
        return ret;
    }
#endif /* PRODUCT_CFG_SUPPORT_FLASH_PROTECT */
    ret = flash_write_prv(spif_ctrl, flash_addr, (td_u8 *)flash_write_data, flash_write_size, do_erase);
    if (g_flash_restore_func) {
        g_flash_restore_func();
    }
    uapi_lowpower_unforbid(EXT_LOWPOWER_ID_FLASH, EXT_SLEEP_DEEP, FLASH_WRITE_TICK_MAX);
    flash_unlock();

    flash_info_print("uapi_flash_write ret:%x addr:%x len:%x\r\n", ret, flash_addr, flash_write_size);
    return ret;
}

td_u32 uapi_flash_read_in(td_u32 flash_addr, td_u32 flash_read_size, td_u8 *flash_read_data)
{
    EXT_SPI_FLASH_CTRL_S *spif_ctrl = &g_flash_drv_ctrl;
    td_u32 ret;

    if (flash_read_data == TD_NULL) {
        flash_info_print("pBuf fail\r\n");
        return EXT_ERR_FLASH_INVALID_PARAM_DATA_NULL;
    }

    ret = flash_check_ram_addr_size((td_u32)(uintptr_t)flash_read_data, flash_read_size);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    ret = sfc_check_para(spif_ctrl, flash_addr, flash_read_size, EXT_FLASH_CHECK_PARAM_OPT_READ);
    if (ret != EXT_ERR_SUCCESS) {
        flash_info_print("flash_check_para fail %x\r\n", ret);
        return ret;
    }

    flash_lock();
    uapi_lowpower_forbid(EXT_LOWPOWER_ID_FLASH, EXT_SLEEP_DEEP);
    if (g_flash_prepare_func) {
        g_flash_prepare_func();
    }
    ret = flash_read_prv(spif_ctrl, flash_addr, flash_read_data, flash_read_size);
    if (g_flash_restore_func) {
        g_flash_restore_func();
    }
    uapi_lowpower_unforbid(EXT_LOWPOWER_ID_FLASH, EXT_SLEEP_DEEP, FLASH_READ_TICK_MAX);
    flash_unlock();

    flash_info_print("uapi_flash_read ret2:%x addr:%x len:%x\r\n", ret, flash_addr, flash_read_size);
    return ret;
}

/*
 * Command definition, support for general command input, and remapping, replacing own functions
 */
td_u32 uapi_flash_ioctl_in(td_u16 cmd, td_void *data)
{
    td_u32 ret;
    EXT_SPI_FLASH_CTRL_S *spif_ctrl = &g_flash_drv_ctrl;

    if (spif_ctrl->bInit != TD_TRUE) {
        return EXT_ERR_NO_INITILIZATION;
    }

    flash_lock();
    uapi_lowpower_forbid(EXT_LOWPOWER_ID_FLASH, EXT_SLEEP_DEEP);
    ret = flash_ioctl(spif_ctrl, cmd, data);
    uapi_lowpower_unforbid(EXT_LOWPOWER_ID_FLASH, EXT_SLEEP_DEEP, FLASH_ERASE_TICK_MAX);
    flash_unlock();
    return ret;
}

td_u32 uapi_flash_erase(EXT_CONST td_u32 addr, EXT_CONST td_u32 size)
{
    return uapi_flash_erase_in(addr, size);
}
td_u32 uapi_flash_write(EXT_CONST td_u32 addr, td_u32 size, EXT_CONST td_u8 *data, td_bool do_erase)
{
    return uapi_flash_write_in(addr, size, data, do_erase);
}
td_u32 uapi_flash_read(EXT_CONST td_u32 addr, EXT_CONST td_u32 size, td_u8 *data)
{
    return uapi_flash_read_in(addr, size, data);
}
td_u32 uapi_flash_ioctl(EXT_IN td_u16 cmd, EXT_INOUT td_void *data)
{
    return uapi_flash_ioctl_in(cmd, data);
}
#define CHIP_ID_SIZE 8
#define DMA_RAM_SIZE 1024

td_u32 uapi_flash_init(td_void)
{
    EXT_SPI_FLASH_CTRL_S *spif_ctrl = &g_flash_drv_ctrl;
    td_u8 chip_id[CHIP_ID_SIZE] = { 0 };
    td_u32 reg_val;

    if (spif_ctrl->bInit == TD_TRUE) {
        return EXT_ERR_FLASH_RE_INIT;
    }

    uapi_lowpower_forbid(EXT_LOWPOWER_ID_FLASH, EXT_SLEEP_DEEP);
    /* Configure the FLASH clock */
    reg_val = UAPI_REG_READ_VAL32(DW21_CRG_REG_SC_PERI_CLKSEL_REG);
    reg_val &= ~(0x1f << CRG_REG_SFC_CLK_SEL_OFFSET);
    reg_val |= (0x9 << CRG_REG_SFC_CLK_SEL_OFFSET); /* divided by 4 75m */
    UAPI_REG_WRITE32(DW21_CRG_REG_SC_PERI_CLKSEL_REG, reg_val);

    /* Configure DMA buffer */
    spif_ctrl->p_dma_ram_buffer = (td_u8 *)g_dma_buffer;
    spif_ctrl->dma_ram_size = DMA_RAM_SIZE;

    /* Configure write with wipe backup space, support write interface do_erase to TD_TRUE option */
    spif_ctrl->p_back_up_buf = (td_u8 *)g_back_buffer;

    /* Initialize the FLASH controller */
    sfc_sys_init();

    /* Initialize the FLASH driver according to the FLASH ID */
    td_u32 ret = spi_flash_basic_info_probe(spif_ctrl, chip_id, (EXT_SPI_FLASH_BASIC_INFO_S *)g_flash_usr_info_tbl,
        sizeof(g_flash_usr_info_tbl) / sizeof(g_flash_usr_info_tbl[0]));
    if (ret != EXT_ERR_SUCCESS) {
        goto END;
    }

    /* Configure FLASH to be 4-wire read mode */
    if (spi_flash_enable_quad_mode() == EXT_ERR_SUCCESS) {
        (td_void)memcpy_s(&spif_ctrl->opt_read, sizeof(spif_ctrl->opt_read), &g_spi_opt_fast_quad_out_read,
            sizeof(SPI_FLASH_OPERATION_S));
    }

    /* Create a FLASH mutex */
    ret = uapi_mux_create(&(spif_ctrl->mutex_handle), "sfc");
    if (ret != EXT_ERR_SUCCESS) {
        goto END;
    }
#ifdef PRODUCT_CFG_SUPPORT_FLASH_PROTECT
    /* For the DW21EV100 chip, the user needs to adapt the
    protection driver according to the actual Flash selection */
    if (EXT_FTM_CHIP_TYPE_21SV == uapi_get_hw_chip_type()) {
        if (!(spif_ctrl->basic_info.chip_attribute & EXT_SPI_FLASH_CHIP_ATTRIBUTE_IS_DEFAULT_ITEM)) {
            ret = uapi_flash_protect_init((ext_flash_protect_type)(spif_ctrl->basic_info.protect_type));
            if (ret == EXT_ERR_SUCCESS) {
                (td_void)uapi_flash_protect_enable(TD_TRUE);
                ret = uapi_flash_protect_config(spif_ctrl->basic_info.chip_size, 0);
            }
        }
    }
#endif /* PRODUCT_CFG_SUPPORT_FLASH_PROTECT */
    spif_ctrl->bInit = TD_TRUE;
END:
    if (ret) {
        sfc_sys_exit();
    }
    uapi_lowpower_unforbid(EXT_LOWPOWER_ID_FLASH, EXT_SLEEP_DEEP, 0);
    return ret;
}

__init td_void uapi_flash_register(EXT_FLASH_REGISTER_S sfunc)
{
    g_flash_prepare_func = sfunc.prepare_func;
    g_flash_restore_func = sfunc.restore_func;
}
