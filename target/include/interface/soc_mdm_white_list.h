/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: APIs for the reject list
 * History:
 * Version              Date         DefectNum           Description
 * V100R001C00SPC019    2022-01-01   NA                  This is the first release.
 */

/** @defgroup auth_list  network
 * @ingroup plc
 */

#ifndef __SOC_MDM_WHITE_LIST_H__
#define __SOC_MDM_WHITE_LIST_H__

#include "soc_types.h"
/**
 * @ingroup auth_list
 * @brief white list init
 * @par  Description:
 * get mesh mode
 * @attention The product type is CCO
 * @retval   ret type #td_u32: #EXT_ERR_SUCCESS: success
 * @li For details about the return values, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_white_list.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_init_white_list(td_void);

/**
 * @ingroup auth_list
 * @brief add new mac to white list
 * @par  Description:
 * add new mac to white list
 * @attention The product type is CCO
 * @param  list  [IN] type #td_pbyte, new mac list
 * @param  count  [IN] type #td_u16, list contains mac num
 * @retval  ret type #td_u32: #EXT_ERR_SUCCESS: success
 * @li For details about the return values, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_white_list.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_add_mac_by_list(td_pbyte list, td_u16 count);
/**
 * @ingroup auth_list
 * @brief delete mac list
 * @par  Description:
 * delete mac list
 * @attention The product type is CCO
 * @param  list  [IN] type #td_pbyte, new mac list
 * @param  count  [IN] type #td_u16, list contains mac num
 * @retval  ret type #td_u32: #EXT_ERR_SUCCESS: delete success
 * @li For details about the return values, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_white_list.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_delete_mac_by_list(td_pbyte list, td_u16 count);
/**
 * @ingroup auth_list
 * @brief query mac list
 * @par  Description:
 * query mac list
 * @attention The product type is CCO
 * @param  list  [IN] type #td_pbyte, save mac list
 * @param  count  [IN] type #td_u16, query sta mac num
 * @retval  ret type #td_u32: #EXT_ERR_SUCCESS: success
 * @li For details about the return values, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_white_list.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_query_white_list_info(td_pbyte list, td_u16 count);
/**
 * @ingroup auth_list
 * @brief query mac list num
 * @par  Description:
 * query mac list num
 * @attention The product type is CCO
 * @retval  ret type #td_u16: white list num
 * @par Dependency:
 * @li soc_mdm_white_list.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u16 uapi_get_white_list_valid_mac_num(td_void);
/**
 * @ingroup auth_list
 * @brief clear mac list
 * @par  Description:
 * clear mac list
 * @attention The product type is CCO
 * @li For details about the return values, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_white_list.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_clear_white_list(td_void);

/**
 * @ingroup auth_list
 * @brief set delay time
 * @par  Description:
 * set delay time
 * @attention The product type is STA
 * @param  save_flash_time  [IN] type #td_u16, delay save flash time, unit :s
 * @param  set_to_mac_time  [IN] type #td_u16, delay effect time, unit :s
 * @retval For details about the return values, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_white_list.h: Describes the external APIs of the link layer.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_set_white_list_time(td_u16 save_flash_time, td_u16 set_to_mac_time);
#endif
