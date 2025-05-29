/* *
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Upgrade - Transmit file storage handling. \n
 * Author: CompanyName
 * Create: 2012-11-11
 */

#ifndef __SOC_TRANS_SAVE_H__
#define __SOC_TRANS_SAVE_H__

#include <soc_types.h>
#include <soc_mdm_types.h>
#ifdef EXT_UPG_DEBUG_PRINTF
#include <soc_stdlib.h>
#define trans_print printf
#else
#define trans_print(fmt, ...)
#endif

#define BLOCK_SIZE PRODUCT_CFG_FLASH_BLOCK_SIZE
#define TRANS_OTHER_FILE_OFFSET 4 /* cco storage sta upgrade file, offset 4 bytes. */

typedef enum {
    UPG_TRANS_FILE_TO_SELF,    /* < CCO/STA/NDM stores its own upgrade file, and the file tail is aligned to
    the end of the backup area for storage */
    UPG_TRANS_FILE_CCO_TO_STA, /* < The upgrade file stored on the CCO is forwarded to the STA. The end of the file
    is aligned with the end of the backup area plus 4 bytes for storage. */
    UPG_TRANS_FILE_MAX
} upg_trans_file_type;

#define TRANS_FILE_CTRL_ID_FILE_HEAD 0xC1 /* < File header information */
#define TRANS_FILE_CTRL_ID_ALIGN 0xFE     /* < Alignment information. You can use this value to fill the alignment \
                                           * when you want some information to be n-byte aligned later.            \
                                           * There is no size and data after the ID.                               \
                                           */
#define TRANS_FILE_CTRL_ID_END 0xFF       /* < End message, see this ID indicates the end of the extra information, \
                                        there is no size and data behind the ID */

#endif /* __SOC_TRANS_SAVE_H__ */
