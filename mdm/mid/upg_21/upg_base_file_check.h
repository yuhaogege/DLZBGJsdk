/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Upgrade base funciton - file check.
 */

#ifndef __UPG_BASE_FILE_CHECK_H__
#define __UPG_BASE_FILE_CHECK_H__

#include <soc_types.h>

#ifdef __cplusplus
extern "C" {
#endif

td_u32 upg_check_chip_magic(td_u32 chip_magic);
td_u32 upg_check_manufactuer(td_u16 manufactuer_code);
td_u32 upg_check_file_type(td_u8 file_type);

#ifdef __cplusplus
}
#endif
#endif /* __UPG_BASE_FILE_CHECK_H__ */
