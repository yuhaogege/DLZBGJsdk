#ifndef __SOC_FIX_SF_CMD_H__
#define __SOC_FIX_SF_CMD_H__
#include <soc_types.h>

typedef td_u32 (*FLASH_INIT_FUNC)(td_void);
typedef td_u32 (*FLASH_READ_FUNC)(td_u32 flash_addr, td_u32 flash_read_size, td_u8 *p_flash_read_data);
typedef td_u32 (*FLASH_WRITE_FUNC)(td_u32 flash_addr, td_u32 flash_write_size, td_u8 *p_flash_write_data,
               td_bool do_erase);
typedef td_u32 (*FLASH_ERASE_FUNC)(EXT_IN td_u32 flash_addr, EXT_IN td_u32 flash_erase_size);

typedef struct
{
    FLASH_INIT_FUNC init;
    FLASH_READ_FUNC read;
    FLASH_WRITE_FUNC write;
    FLASH_ERASE_FUNC erase;
} EXT_FLASH_CMD_FUNC_S;

td_u32 uapi_cmd_regist_flash_cmd(EXT_FLASH_CMD_FUNC_S *pFuncs);

#endif

