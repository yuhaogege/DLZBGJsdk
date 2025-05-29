#include <flash_prv.h>

__rom_addition td_u32 spi_flash_write_sr_reg(td_u8 cmd, td_u8 *p_data, td_u8 data_len, td_bool is_volatile)
{
    td_u32 temp_data = 0;
    td_u32 ret = EXT_ERR_SUCCESS;

    if (data_len > 0) {
        if (memcpy_s(&temp_data, sizeof(temp_data), p_data, data_len)) {
            return ret;
        }
    }

    if (is_volatile) {
        ext_sfc_write(SFC_REG_CMD_INS, SPI_CMD_VSR_WREN);
        ext_sfc_write (SFC_REG_CMD_CONFIG, (td_u32)(SFC_CMD_CONFIG_SEL_CS | SFC_CMD_CONFIG_START));
        ret = spif_wait_config_start();
        if (ret != EXT_ERR_SUCCESS) {
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
        SFC_CMD_CONFIG_SEL_CS |
        SFC_CMD_CONFIG_DATA_EN |
        SFC_CMD_CONFIG_DATA_CNT(data_len) |
        SFC_CMD_CONFIG_START);

    ret = spif_wait_config_start();
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    return EXT_ERR_SUCCESS;
}

__rom_addition td_u32 spi_flash_set_protect(td_u8 cmp_bp, td_bool is_volatile)
{
    td_u32 ret = EXT_ERR_SUCCESS;
    td_u8 p_data[2] = { 0 };
    td_u8 cmp = (cmp_bp >> 5) & 0x1;
    td_u8 bp = cmp_bp & 0x1F;

    ret = spi_flash_read_reg(SPI_CMD_RDSR, &p_data[0], 1);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }
    ret = spi_flash_read_reg(SPI_CMD_RDSR2, &p_data[1], 1);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }
    if (((p_data[0] & (0x1F << 2)) == (bp << 2)) &&   /* index 0,  move left 2bits */
        ((p_data[1] & (0x1 << 6)) == (cmp << 6))) {   /* index 1,  move left 6bits */
        return EXT_ERR_SUCCESS;
    }

    p_data[0] &= ~(0x1f << 2);                          /* move left 2bits */
    p_data[0] |= (td_u8)(bp << 2);                      /* move left 2bits */
    p_data[1] &= ~(0x1 << 6);                           /* move left 6bits */
    p_data[1] |= (td_u8)(cmp << 6);                     /* move left 6bits */
    ret = spi_flash_write_sr_reg(SPI_CMD_WRSR1, p_data, 2, is_volatile); /* write 2 bytes */
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    ret = spi_flash_read_reg(SPI_CMD_RDSR, &p_data[0], 1);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }
    ret = spi_flash_read_reg(SPI_CMD_RDSR2, &p_data[1], 1);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }
    if (((p_data[0] & (0x1F << 2)) == (bp << 2)) &&   /* index 0,  move left 2bits */
        ((p_data[1] & (0x1 << 6)) == (cmp << 6))) {   /* index 1,  move left 6bits */
        return EXT_ERR_SUCCESS;
    } else {
        return p_data[0] | (p_data[1] << 8);          /* index 1,  move left 8bits */
    }
}
