/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: soc_uart_print_debug,
 */
#ifndef __SOC_UART_PRINT_DEBUG_H__
#define __SOC_UART_PRINT_DEBUG_H__

#define ext_print_error(fmt, ...)
#define ext_print_warning(fmt, ...)
#define ext_print_info(fmt, ...)
#define ext_print_debug(fmt, ...)

/* ICC print macro switch */
#define EXT_ICC_PRINT_ERR     ext_print_error
#define EXT_ICC_PRINT_WARNING ext_print_warning
#define EXT_ICC_PRINT_INFO    ext_print_info
#define EXT_ICC_PRINT_DEBUG   ext_print_debug

/* FMGR print macro switch */
#define EXT_FMGR_PRINT_ERR     ext_print_error
#define EXT_FMGR_PRINT_WARNING ext_print_warning
#define EXT_FMGR_PRINT_INFO    ext_print_info
#define EXT_FMGR_PRINT_DEBUG   ext_print_debug

#define EXT_SD_DC_PRINT_ERR     ext_print_error
#define EXT_SD_DC_PRINT_WARNING ext_print_warning
#define EXT_SD_DC_PRINT_INFO    ext_print_info
#define EXT_SD_DC_PRINT_DEBUG   ext_print_debug

/* Upgrade print macro switch */
#define EXT_3518_UPG_PRINT_ERR     ext_print_error
#define EXT_3518_UPG_PRINT_WARNING ext_print_warning
#define EXT_3518_UPG_PRINT_INFO    ext_print_info
#define EXT_3518_UPG_PRINT_DEBUG   ext_print_debug

#endif
