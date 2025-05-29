/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Upgrade - Transmit file handling - extra information.
 * Author: CompanyName
 * Create: 2018-10-31
 */

#include "trans_extra_info.h"
#include "soc_trans_save.h"

#include <soc_stdlib.h>

#pragma pack(1)
typedef struct {
    td_u8 id;
    td_u8 size;
} upg_std_extra_item;
#pragma pack()

#ifdef __cplusplus
extern "C" {
#endif

/* Parse the extended information. */
td_u32 uapi_std_get_extra_info_item(td_u8 *data, td_u32 data_size, td_u8 id, td_u8 **extra_item, td_u16 *size)
{
    if (extra_item == TD_NULL || *extra_item == TD_NULL) {
        return EXT_ERR_FAILURE;
    }

    td_u32 offset;
    upg_std_extra_item item_head = { 0 };
    td_u8 item_id;
    td_u16 item_size;

    for (offset = 0; offset < data_size;) {
        if (memcpy_s(&item_head, sizeof(item_head), &data[offset], sizeof(upg_std_extra_item)) != EOK) {
            break;
        }

        if (item_head.id == TRANS_FILE_CTRL_ID_END) {
            break;
        }

        if (item_head.id == TRANS_FILE_CTRL_ID_ALIGN) {
            offset += 1;
            continue;
        }

        item_id = item_head.id;
        item_size = item_head.size;

        if (item_id >= 0xC0 && item_id <= 0xEE) {
            item_size = item_head.size * 2; /* id between 0xC0 and 0xEE, size bitwidth = 2. */
        }

        if (item_id == id) {
            *size = item_size;
            *extra_item = (data + offset + sizeof(upg_std_extra_item));
            return EXT_ERR_SUCCESS;
        }

        offset += (td_u32)sizeof(upg_std_extra_item);
        offset += item_size;
    }

    return EXT_ERR_FAILURE;
}

#ifdef __cplusplus
}
#endif
