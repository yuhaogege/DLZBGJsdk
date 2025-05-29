/**
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * @defgroup hct_mem  Memory
 * @ingroup osa
 */

#ifndef __SOC_MDM_MEM_H__
#define __SOC_MDM_MEM_H__
#include <soc_types_rom.h>

/**
 * @ingroup hct_mem
 * Overall memory information
 */
typedef struct {
    td_u32 total;              /**< Total space of the memory pool (unit: byte) */
    td_u32 used;               /**< Used space of the memory pool (unit: byte) */
    td_u32 free;               /**< Free space of the memory pool (unit: byte) */
    td_u32 free_node_num;      /**< Number of free nodes in the memory pool */
    td_u32 used_node_num;      /**< Number of used nodes in the memory pool */
    td_u32 max_free_node_size; /**< Maximum size of the node in the free space of the memory pool (unit: byte) */
    td_u32 malloc_fail_count;  /**< Number of memory application failures */
    td_u32 peek_size;          /**< Peak memory usage of the memory pool */
} ext_mdm_mem_info_s;

typedef struct {
    td_u32 pool_addr;    /**< Memory pool address */
    td_u32 pool_size;    /**< Memory pool size */
    td_u32 fail_count;   /**< Number of memory application failures */
    td_u32 peek_size;    /**< Peak memory usage of the memory pool */
    td_u32 cur_use_size; /**< Used space of the memory pool (unit: byte) */
} ext_mem_pool_crash_info_s;

/**
 * @ingroup  hct_mem
 * @brief  Dynamically applies for memory.
 *
 * @par Description:
 * Dynamically applies for memory.
 *
 * @attention None
 * @param  mod_id  [IN] Type #td_u32 ID of the called module. The value is EXT_MOD_ID_XXX.
 * @param  size    [IN] Type #td_u32 Requested memory size (unit: byte)
 *
 * @retval # Not TD_NULL Success
 * @retval #TD_NULL   Failure. The memory is insufficient.
 * @par Dependency:
 * @li soc_mdm_mem.h: Describes memory APIs.
 * @see  uapi_free
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_pvoid uapi_malloc(td_u32 mod_id, td_u32 size);

/**
 * @ingroup  hct_mem
 * @brief  Releases the memory that is dynamically applied for.
 *
 * @par Description:
 * Releases the memory that is dynamically applied for.
 *
 * @attention None
 * @param  mod_id  [IN] Type #td_u32 ID of the called module. The value is EXT_MOD_ID_XXX.
 * @param  addr    [IN] Type #td_pvoid Start address of the requested memory. The validity of the address is ensured by
 * the caller.
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_mem.h: Describes memory APIs.
 * @see  uapi_malloc
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_void uapi_free(td_u32 mod_id, td_pvoid addr);

/**
 * @ingroup  hct_mem
 * @brief  Dynamically applies for memory by using the ROM partition code.
 *
 * @par Description:
 * Dynamically applies for memory by using the ROM partition code.
 *
 * @attention None
 * @param  rom_mod_id  [IN] Type #td_u32 ID of the called module
 * @param  size        [IN] Type #td_u32 Requested memory size (unit: byte)
 *
 * @retval # Not TD_NULL     Success
 * @retval #TD_NULL       Failure. The memory is insufficient.
 * @par Dependency:
 * @li soc_mdm_mem.h: Describes memory APIs.
 * @see  uapi_rom_free
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_pvoid uapi_rom_malloc(td_u32 rom_mod_id, td_u32 size);

/**
 * @ingroup  hct_mem
 * @brief  Dynamically releases memory by using the ROM partition code.
 *
 * @par Description:
 * Dynamically releases memory by using the ROM partition code.
 *
 * @attention None
 * @param  rom_mod_id  [IN] Type #td_u32 ID of the called module
 * @param  addr        [IN] Type #td_pvoid Start address of the memory
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_mem.h: Describes memory APIs.
 * @see  uapi_rom_malloc
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_void uapi_rom_free(td_u32 rom_mod_id, td_pvoid addr);
/**
 * @ingroup  hct_mem
 * @brief  Obtains the memory information.
 *
 * @par Description:
 * Obtains the memory information.
 *
 * @attention None
 * @param  mem_inf [OUT] Type #ext_mdm_mem_info_s* Overall memory information
 *
 * @retval #0      Success
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_mem.h: Describes memory APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_mem_get_sys_info(EXT_OUT ext_mdm_mem_info_s *mem_inf);

/**
 * @ingroup  hct_mem
 * @brief  Obtains memory information, used in a crash process.
 *
 * @par Description:
 * Obtains memory information, used in a crash process. When the board is reset due to a memory exception, if
 * uapi_mem_get_sys_info is used to obtain memory information, another exception may occur. In this case, use
 * uapi_mem_get_sys_info_crash instead.
 *
 * @attention None
 *
 * @retval #ext_mem_pool_crash_info_s   Memory information
 *
 * @par Dependency:
 * @li soc_mdm_mem.h: Describes memory APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_CONST ext_mem_pool_crash_info_s *uapi_mem_get_sys_info_crash(td_void);

#endif
