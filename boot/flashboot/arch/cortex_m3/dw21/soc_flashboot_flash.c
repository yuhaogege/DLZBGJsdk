/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: FLASH ram boot
 */

#include <soc_flashboot_flash.h>
#include "flash_prv.h"

EXT_SPI_FLASH_CTRL_S g_flash_drv_ctrl = {
    0,
};

EXT_CONST EXT_SPI_FLASH_BASIC_INFO_S g_flash_usr_info_tbl[] = {
    {"UNKNOWN", {0, }, _16M, EXT_FLASH_SUPPORT_DEFAULT, 0, 0},
    {"W25Q40", {0xEF, 0x40, 0x13, }, _512K, EXT_FLASH_SUPPORT_ERASE464CHIP, 3, 0},
    {"GD25Q40", {0xC8, 0x40, 0x13, }, _512K, EXT_FLASH_SUPPORT_ERASE464CHIP, 3, 0},
    {"W25Q16", {0xEF, 0x40, 0x15, }, _2M, EXT_FLASH_SUPPORT_ERASE464CHIP, 3, 0},
};

td_u32 g_dma_buffer[256] = { 0 }; /* buffer: 256*4 */

#ifdef FEATURE_SUPPORT_FLASH_PROTECT
static td_bool g_flash_protect_enable = TD_FALSE;

EXT_PRV td_u32 uapi_flash_protect_cancel(td_void)
{
    if (!g_flash_protect_enable) {
        return EXT_ERR_SUCCESS;
    }
    return spi_flash_set_protect(0, TD_FALSE);
}

td_u32 uapi_flash_protect_enable(td_bool enable)
{
    td_u32 reg_val;

    g_flash_protect_enable = enable;
    UAPI_REG_READ((EXT_SYSCTRL_REG_BASE + REG_FLASH_PROTECT), reg_val);

    if (enable) {
        UAPI_REG_WRITE((EXT_SYSCTRL_REG_BASE + REG_FLASH_PROTECT), reg_val | SUPPORT_FLASH_PROTECT);
    } else {
        UAPI_REG_WRITE((EXT_SYSCTRL_REG_BASE + REG_FLASH_PROTECT), reg_val & (~SUPPORT_FLASH_PROTECT));
    }
    return 0;
}
#endif /* FEATURE_SUPPORT_FLASH_PROTECT */

td_u32 uapi_flash_erase(EXT_IN td_u32 flash_addr, EXT_IN td_u32 flash_erase_size)
{
    td_u32 ret = EXT_ERR_FAILURE;
    EXT_SPI_FLASH_CTRL_S *spif_ctrl = &g_flash_drv_ctrl;

    ret = sfc_check_para(spif_ctrl, flash_addr, flash_erase_size, EXT_FLASH_CHECK_PARAM_OPT_ERASE);
    if (EXT_ERR_SUCCESS != ret) {
        return ret;
    }

#ifdef FEATURE_SUPPORT_FLASH_PROTECT
    ret = uapi_flash_protect_cancel();
    if (EXT_ERR_SUCCESS != ret) {
        return ret;
    }
#endif
    ret = flash_erase_prv(spif_ctrl, flash_addr, flash_erase_size);
    flash_info_print("uapi_flash_erase ret:%x addr:%x len:%x\r\n", ret, flash_addr, flash_erase_size);
    return ret;
}

td_u32 uapi_flash_write(td_u32 flash_addr, td_u32 flash_write_size, td_u8 *flash_write_data, td_bool do_erase)
{
    td_u32 ret;
    EXT_SPI_FLASH_CTRL_S *spif_ctrl = &g_flash_drv_ctrl;

    if (flash_write_data == TD_NULL) {
        return EXT_ERR_FLASH_INVALID_PARAM_DATA_NULL;
    }
    ret = sfc_check_para(spif_ctrl, flash_addr, flash_write_size, EXT_FLASH_CHECK_PARAM_OPT_WRITE);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

#ifdef FEATURE_SUPPORT_FLASH_PROTECT
    ret = uapi_flash_protect_cancel();
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }
#endif
    ret = flash_write_prv(spif_ctrl, flash_addr, flash_write_data, flash_write_size, do_erase);
    flash_info_print("uapi_flash_write ret:%x addr:%x len:%x\r\n", ret, flash_addr, flash_write_size);
    return ret;
}

td_u32 uapi_flash_read(td_u32 flash_addr, td_u32 flash_read_size, td_u8 *flash_read_data)
{
    EXT_SPI_FLASH_CTRL_S *spif_ctrl = &g_flash_drv_ctrl;
    td_u32 ret;

    if (flash_read_data == TD_NULL) {
        return EXT_ERR_FLASH_INVALID_PARAM_DATA_NULL;
    }

    ret = sfc_check_para(spif_ctrl, flash_addr, flash_read_size, EXT_FLASH_CHECK_PARAM_OPT_READ);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    ret = flash_read_prv(spif_ctrl, flash_addr, flash_read_data, flash_read_size);
    flash_info_print("uapi_flash_read ret2:%x addr:%x len:%x\r\n", ret, flash_addr, flash_read_size);
    return ret;
}

td_u32 uapi_flash_init(td_void)
{
    td_u32 ret;
    EXT_SPI_FLASH_CTRL_S *spif_ctrl = &g_flash_drv_ctrl;
    td_u8 chip_id[8] = { 0 }; /* chip id: 8B */

    if (spif_ctrl->bInit == TD_TRUE) {
        return EXT_ERR_FLASH_RE_INIT;
    }

    spif_ctrl->p_dma_ram_buffer = (td_u8 *)g_dma_buffer;
    spif_ctrl->dma_ram_size = 1024; /* ram size: 1024 */

    ret = spi_flash_basic_info_probe(spif_ctrl, chip_id, (EXT_SPI_FLASH_BASIC_INFO_S *)g_flash_usr_info_tbl,
        sizeof(g_flash_usr_info_tbl) / sizeof(g_flash_usr_info_tbl[0]));
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }
#ifdef FEATURE_SUPPORT_FLASH_PROTECT
    /* for chip DW21EV100, according to requirement, enable OR disable the feature of flash protection */
    if (EXT_CHIP_TYPE_21SV == uapi_get_hw_chip_type()) {
        uapi_flash_protect_enable(TD_TRUE);
    }
#endif
    spif_ctrl->bInit = TD_TRUE;
    return ret;
}
