/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Upgrade - Transmit file handling - extra information.
 * Author: CompanyName
 * Create: 2018-10-31
 */

#ifndef __TRANS_EXTRA_INFO_H__
#define __TRANS_EXTRA_INFO_H__

#include <soc_types.h>

#ifdef __cplusplus
extern "C" {
#endif

td_u32 uapi_std_get_extra_info_item(td_u8 *data, td_u32 data_size, td_u8 id, td_u8 **extra_item, td_u16 *size);

#ifdef __cplusplus
}
#endif
#endif /* __TRANS_EXTRA_INFO_H__ */
