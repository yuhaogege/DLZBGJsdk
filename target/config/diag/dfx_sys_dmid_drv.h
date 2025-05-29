/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: dfx_sys dmid drv.
 * Author: CompanyName
 * Create: 2021-1-27
 */

#ifndef __DFX_SYS_DMID_DRV_H__
#define __DFX_SYS_DMID_DRV_H__

/* ************************************************ ***************************** */
/* DRV message definition */
/* ************************************************ ***************************** */
/* range [0x0150, 0x0250) */
/* ************************************************ ***************************** */
#define EXT_DMID_DRV_FLASH_PROBE 0x0170 /* FLASH chip information detection */
#define EXT_DMID_DRV_FLASH_OP 0x0171    /* FLASH operation information */

#define EXT_DMID_DRV_UART_CHANGE_PARA 0x0180 /* UART parameter modification */

#define EXT_DMID_DRV_SPI_TIMEOUT 0x0190 /* SPI operation timed out */

/* ************************************************ ***************************** */
/* Structure definition */
/* ************************************************ ***************************** */
typedef struct {
    td_u8 id[3]; /* 3 count */
    td_u8 pad;
    td_char name[10]; /* 10 count */
} ext_dmid_drv_flash_probe;
typedef struct {
    td_u8 uctype;
    td_u8 pad[3]; /* pad 3 byte */
    td_u32 uladdr;
    td_u32 ullen;
} ext_dmid_drv_flash_op;

typedef struct {
    td_u32 ulport;
    td_u32 ulbaud_rate;
    td_u32 uldata_bits;
    td_u32 ulstop_bits;
    td_u32 ulparity;
} ext_dmid_drv_uart_change_para;

typedef struct {
    td_u32 ulport;
    td_u32 uldirection;
    td_u32 ultimeout;
} ext_dmid_drv_spi_timeout;

#endif /* __DFX_SYS_DMID_DRV_H__ */

