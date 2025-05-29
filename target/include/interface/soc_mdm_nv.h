/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: NV items consist of the NV items in the factory partition and
 * NV items in the non-factory partition.The NV items in the
 * non-factory partition are classified into NV items in the
 * reserved partition and NV items in the common partition.
 * The values of NV items in the reserved partition are
 * reserved after the reserved partition is upgraded.
 * Author: CompanyName
 * Create: 2018-10-31
 */
/**
 * @defgroup hct_nv NV management
 * @ingroup system
 */

#ifndef __SOC_MDM_NV_H__
#define __SOC_MDM_NV_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup  hct_nv
 * Maximum length of an NV item (unit: byte).
 */
#define HNV_ITEM_MAXLEN 252 /* 256 - 4 */
#define PRODUCT_CFG_NV_REG_NUM_MAX 20

/**
 * @ingroup  hct_nv
 * Maximum number of registered uapi_nv_register_change_nofity_proc functions.
 */
#define NVM_CHANGED_PROC_NUM PRODUCT_CFG_NV_REG_NUM_MAX

/**
 * @ingroup  hct_nv
 * @brief  Initializes NV management in the non-factory partition.
 *
 * @par Description:
 * Initializes NV management.
 *
 * @attention Parameters are obtained from the partition table and cannot be set randomly.
 * @param  addr          [IN] Type #td_u32 Flash address of the NV working partition, corresponding to the flash
 * address of the member #EXT_FLASH_PARTITON_NV_FILE in the partition table.
 * @param  backup_addr   [IN] Type #td_u32 Flash address of the NV backup partition, corresponding to the flash address
 * of the member #EXT_FLASH_PARTITON_NV_BACKUP in the partition table.
 *
 * @retval #0               Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_nv.h: Describes NV APIs.
 * @see uapi_get_partition_table | uapi_nv_read | uapi_nv_write
 * @since DW21_V100R001C00B020
 */
td_u32 uapi_nv_init(td_u32 addr, td_u32 backup_addr);

/**
 * @ingroup  hct_nv
 * @brief  Sets the NV value.
 *
 * @par Description:
 * Sets the NV value.
 *
 * @attention
 * @li Only a non-factory NV item can be operated.
 * @li This API is called only in a task and cannot be called in an interrupt.
 * @param  id   [IN] Type #td_u8 NV item ID, ranging from #EXT_NV_NORMAL_ID_START to #EXT_NV_NORMAL_USR_ID_END.
 * @param  data [IN] Type #td_pvoid NV item data
 * @param  len  [IN] Type #td_u8 Length of an NV item (unit: byte). The maximum value is HNV_ITEM_MAXLEN.
 *
 * @retval #0               Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_nv.h: Describes NV APIs.
 * @see uapi_nv_read
 * @since DW21_V100R001C00
 */
td_u32 uapi_nv_write(td_u8 id, EXT_CONST td_pvoid data, td_u8 len);

/**
 * @ingroup  hct_nv
 * @brief  Reads the NV value.
 *
 * @par Description:
 * Reads the NV value.
 *
 * @attention
 * @li Only a non-factory NV item can be operated.
 * @li This API is called only in a task and cannot be called in an interrupt.
 * @param  id   [IN] Type #td_u8 NV item ID, ranging from #EXT_NV_NORMAL_ID_START to #EXT_NV_NORMAL_USR_ID_END.
 * @param  data [IN] Type #td_pvoid NV item data.
 * @param  len  [IN] Type #td_u8 Length of an NV item (unit: byte). The maximum value is HNV_ITEM_MAXLEN.
 *
 * @retval #0               Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_nv.h: Describes NV APIs.
 * @see uapi_nv_write.
 * @since DW21_V100R001C00
 */
td_u32 uapi_nv_read(td_u8 id, EXT_CONST td_pvoid data, td_u8 len);

/**
 * @ingroup  hct_nv
 * @brief  NV item change callback function
 *
 * @par Description:
 * NV item change callback function
 *
 * @attention This API can be called only after the SAL is initialized.
 * @param  id [IN] Type #td_u8 NV item ID
 *
 * @retval #0               Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_nv.h: Describes NV APIs.
 * @see None
 * @since DW21_V100R001C00
 */
typedef td_u32 (*ext_nvm_changed_notify_f)(td_u8 id);

/**
 * @ingroup  hct_nv
 * @brief  Registers the NV item change notification function.
 *
 * @par Description:
 * Registers the NV item change notification function.
 *
 * @attention
 * Only the change notification function for non-factory NV items can be registered.
 * The maximum number of registered functions is #NVM_CHANGED_PROC_NUM. If the number of registered functions
 * exceeds the maximum, an error code is returned.
 * @param  min_id [IN] Type #td_u8 Minimum value of an NV item ID.
 * @param  max_id [IN] Type #td_u8 Maximum value of an NV item ID.
 * @param  func   [IN] Type #ext_nvm_changed_notify_f Handling function for NV item changes. That is, after an NV item
 * is changed, the NV module automatically calls the registered API.
 *
 * @retval #0               Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_nv.h: Describes NV APIs.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_nv_register_change_nofity_proc(td_u8 min_id, td_u8 max_id, ext_nvm_changed_notify_f func);

/**
 * @ingroup  hct_nv
 * @brief  Initializes NV management in the factory partition.
 *
 * @par Description:
 * Initializes NV management in the factory partition.
 *
 * @attention The parameters cannot be set randomly and must match the product delivery plan.
 * @param  addr [IN] Type #td_u32 Start address of the NV factory partition in the flash. The address is planned by
 * the factory and set by the boot macro #FACTORY_NV_ADDR.
 *
 * @retval #0               Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_nv.h: Describes NV APIs.
 * @see uapi_factory_nv_write | uapi_factory_nv_read.
 * @since DW21_V100R001C00
 */
td_u32 uapi_factory_nv_init(td_u32 addr);

/**
 * @ingroup  hct_nv
 * @brief  Sets the NV value in the factory partition.
 *
 * @par Description:
 * Sets the NV value in the factory partition.
 *
 * @attention None
 * @param  id   [IN] Type #td_u8 NV item ID, ranging from #EXT_NV_FACTORY_ID_START to #EXT_NV_FACTORY_USR_ID_END.
 * @param  data [IN] Type #td_pvoid NV item data
 * @param  len  [IN] Type #td_u8 Length of an NV item (unit: byte). The maximum value is #HNV_ITEM_MAXLEN.
 *
 * @retval #0               Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_nv.h: Describes NV APIs.
 * @see uapi_factory_nv_read.
 * @since DW21_V100R001C00
 */
td_u32 uapi_factory_nv_write(td_u8 id, td_pvoid pdata, td_u8 len);

/**
 * @ingroup  hct_nv
 * @brief  Reads the NV value in the factory partition.
 *
 * @par Description:
 * Reads the NV value in the factory partition.
 *
 * @attention None
 * @param  id   [IN] Type #td_u8 NV item ID, ranging from #EXT_NV_FACTORY_ID_START to #EXT_NV_FACTORY_USR_ID_END.
 * @param  data [IN] Type #td_pvoid NV item data.
 * @param  len  [IN] Type #td_u8 Length of an NV item (unit: byte). The maximum value is #HNV_ITEM_MAXLEN.
 *
 * @retval #0               Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_nv.h: Describes NV APIs.
 * @see uapi_factory_nv_write.
 * @since DW21_V100R001C00
 */
td_u32 uapi_factory_nv_read(td_u8 id, td_pvoid pdata, td_u8 len);

/* temporary interface */
td_u8 uapi_nv_len(td_u8 id);
td_u32 uapi_nv_soft_reset(td_u32 work_addr, td_u32 backup_addr);

typedef enum _ext_nv_type_ {
    EXT_TYPE_NV = 0,
    EXT_TYPE_FACTORY_NV,
    EXT_TYPE_TEMP,
    EXT_TYPE_NV_MAX,
} ext_nv_type;

td_u32 nv_init_common(td_u32 work_addr, td_u32 source_file_addr, ext_nv_type nv_type);
td_u32 nv_read_common(td_u8 id, td_pvoid pdata, td_u8 len, ext_nv_type nv_type);
td_u32 nv_write_common(td_u8 id, td_pvoid pdata, td_u8 len, ext_nv_type nv_type);
td_u32 uapi_nv_get_ver_magic(td_void);
td_u32 uapi_nv_sector_write(td_u8 *nv_file);

#ifdef __cplusplus
}
#endif
#endif /* __SOC_MDM_NV_H__ */
