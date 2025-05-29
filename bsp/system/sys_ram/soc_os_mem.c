/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: sys_os_mem.c
 */
#include <soc_types_rom.h>
#include <los_memory.h>
#include <soc_mdm_mem.h>

__isr EXT_CONST ext_mem_pool_crash_info_s *uapi_mem_get_sys_info_crash(td_void)
{
    return (ext_mem_pool_crash_info_s *)m_aucSysMem0;
}


#ifdef __MEM_HEAP_STAT_PATCH__
#include <soc_config_rom.h>
extern td_u32 g_heap_bsp_size;
extern td_u32 g_heap_mac_size;
extern td_u32 g_heap_app_size;
extern td_u32 get_node_size(td_pvoid pMem);

__hot td_pvoid uapi_malloc_patch(td_u32 mod_id, td_u32 size)
{
    td_pvoid paddr = TD_NULL;
    td_u32 real_size;
    paddr = LOS_MemAlloc((td_pvoid)m_aucSysMem0, size);
    if (paddr == TD_NULL) {
        return paddr;
    }
    real_size = get_node_size(paddr);
    if (mod_id >= EXT_MOD_ID_BSP_MIN && (mod_id <= EXT_MOD_ID_BSP_MAX)) {
        g_heap_bsp_size += real_size;
    }
    if (mod_id >= EXT_MOD_ID_MAC_MIN && (mod_id <= EXT_MOD_ID_MAC_MAX)) {
        g_heap_mac_size += real_size;
    }
    if (mod_id >= EXT_MOD_ID_APP_MIN && (mod_id <= EXT_MOD_ID_APP_MAX)) {
        g_heap_app_size += real_size;
    }
    return paddr;
}

__hot td_pvoid uapi_free_patch(td_u32 mod_id, td_pvoid addr)
{
    td_u32 size;
    size = get_node_size(addr);
    if (0 == LOS_MemFree((td_pvoid)m_aucSysMem0, (td_pvoid)addr)) {
        if (mod_id >= EXT_MOD_ID_BSP_MIN && (mod_id <= EXT_MOD_ID_BSP_MAX)) {
            g_heap_bsp_size -= size;
        }
        if (mod_id >= EXT_MOD_ID_MAC_MIN && (mod_id <= EXT_MOD_ID_MAC_MAX)) {
            g_heap_mac_size -= size;
        }
        if (mod_id >= EXT_MOD_ID_APP_MIN && (mod_id <= EXT_MOD_ID_APP_MAX)) {
            g_heap_app_size -= size;
        }
    }
}
#endif