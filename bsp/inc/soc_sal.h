/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: SAL (System Function Application Layer), including OS interface/DMS/NV/DIAG and
 * system initialization and OS resource allocation management. soc_sal.h code
 * Author: CompanyName
 * Create: 2012-07-14
 */

#ifndef __SOC_SAL_H__
#define __SOC_SAL_H__
#include <soc_types.h>
#include <soc_mdm_types.h>
#include <soc_ft_nv.h>
#include <soc_sal_prv.h>
#include <soc_mdm_mac.h>
EXT_START_HEADER

#define EXT_SYS_POWER_CUT_RESTART 1
#define EXT_SYS_NON_POWER_CUT_RESTART 0

#define EXT_MONITOR_INTERVAL_MS (1000)
#define EXT_MONITOR_INTERVAL_SEC (EXT_MONITOR_INTERVAL_MS / 1000)

/* *
 * @ingroup soc_sal
 * @brief Task sleep
 *
 * @par Description: Task sleep
 *
 * @attention None.
 * When the input parameter is less than 10ms, it is treated as 10ms.
 * Divide downward alignment when greater than 10ms.
 * @retval EXT_ERR_SUCCESS Success.
 * @retval #!0 failed. See soc_errno.h for details.
 * @par Dependency:
 * @li sal_common.h: The file is used to describe the soc sal interface.
 * @see None.
 * @since None.
 */
#define ext_sys_task_wait_exception_polling_wait() uapi_sleep(EXT_SYS_TASK_WAIT_EXCEPTION_TIME_OUT)

/* DMS Interface */
EXT_EAPI td_u8 uapi_load_dev_type(td_void);

/* *
 * @ingroup soc_sal
 * @brief Report the layer message to 'Layer Message Viewer' on PC side.
 *
 * @par Description: The function to install by OS HOOK.
 *
 * @attention None.
 * pRptMessage [in]
 * Pointer to EXT_DIAG_LAYER_MSG_S structure which hold the layer message.
 * ucType [in]
 * Reported to the DBK category, the values are as follows.
 * EXT_DIAG_TYPE_A(0) indicates the DBK development and commissioning version.
 * EXT_DIAG_TYPE_B(1) indicates the field maintenance version of the DBK.
 * @retval EXT_ERR_SUCCESS indicates success.
 * @retval EXT_ERR_NO_INITILIZATION device not initialized.
 * @retval EXT_ERR_QUEUE_FULL OS queue is full, allocation failed.
 * @retval EXT_ERR_FAILURE unknown error.
 * @retval EXT_ERR_CFG_NOT_ALLOW log is not allowed to be reported.
 * @retval EXT_ERR_INVALID_PARAMETER is illegal.
 * @retval EXT_ERR_NOT_ENOUGH_MEMORY memory allocation failed.
 * @par Dependency:
 * @li sal_common.h: The file is used to describe the soc sal interface.
 * @see None.
 * @since None.
 */
EXT_EAPI td_u32 diag_report_layer_msg(td_u16 msg_id, td_u16 src_module_id, td_u16 dst_module_id, td_pvoid ptr_packet,
    td_u16 packet_size);
/* This interface is not currently used as the sdk interface */
EXT_EAPI td_u32 uapi_diag_run_cmd(td_u32 cmd_id, td_u8 option, td_pvoid data, td_u16 data_size);

/* *
 * @ingroup soc_sal
 * @brief System diagnostic information is reported.
 *
 * @par Description: None.
 *
 * @attention None.
 * @param usId [in] Diagnostic package ID.
 * @param usSrcModId [in] Source module ID 0 means no module ID specified.
 * @param usDestModId [in] destination module ID, 0 means no module ID specified.
 * @param pPacket [in] Diagnostic packet buffer size.
 * @param usPacketSize [in] Diagnostic packet buffer size.
 * @param ulOption [in] Diagnostic option, taking any of the following values.
 * a) EXT_SYS_DMID_BUSINESS_L0, EXT_SYS_DMID_BUSINESS_L1, EXT_SYS_DMID_BUSINESS_L2
 * b) EXT_SYS_DMID_NET_L0, EXT_SYS_DMID_NET_L1, EXT_SYS_DMID_NET_L2
 * c) EXT_SYS_DSID_BUSINESS_L0, EXT_SYS_DSID_BUSINESS_L1, EXT_SYS_DSID_BUSINESS_L2
 * @retval EXT_ERR_SUCCESS Success.
 * @retval Other values indicate error codes.
 * @par Dependency:
 * @li sal_common.h: The file is used to describe the soc sal interface.
 * @see None.
 * @since None.
 */
EXT_EAPI td_u32 diag_report_sys_msg(ext_diag_layer_msg layer_msg, td_u16 msg_level);

EXT_EAPI td_u32 diag_report_usr_msg(td_u16 msg_id, td_u16 src_module_id, td_u16 dst_module_id, td_pvoid ptr_packet,
    td_u16 packet_size, td_u16 msg_level);
#define ext_diag_report_usr_message(msg_id, src_module_id, dst_module_id, ptr_packet, packet_size, msg_level) \
    diag_report_usr_msg(msg_id, src_module_id, dst_module_id, ptr_packet, packet_size, msg_level)

/* *
 * @ingroup soc_sal
 * @brief Get information about the MAC layer globally
 *
 * @par Description: None.
 *
 * @attention None.
 * @param pstInfo [out] : ext_mac_config_info structure pointer.
 * @retval EXT_ERR_SUCCESS indicates success.
 * @par Dependency:
 * @li sal_common.h: The file is used to describe the soc sal interface.
 * @see None.
 * @since None.
 */
EXT_EAPI td_u32 uapi_mac_query_info(EXT_OUT ext_mac_config_info *info);

/* Helper routine */
#define ext_sys_task_exception_polling_wait() uapi_sleep(EXT_DMS_CHL_EXCEPTION_POLLING_WAIT)
#define ext_sys_task_wait_forever() uapi_sleep(0xFFFFFFFF)

/* *
 * @ingroup soc_sal
 * @brief Get system information, including system status, version number, etc.
 *
 * @par Description: None.
 *
 * @attention Part of the system status information is not static, you need to call
 * the interface in real time to obtain system status information for synchronization.
 * @param None
 * @retval ext_sys_status_info.
 * @par Dependency:
 * @li sal_common.h: The file is used to describe the soc sal interface.
 * @see None.
 * @since None.
 */
EXT_EAPI ext_sys_status_info *uapi_get_sys_info(td_void);

#if !defined(PRODUCT_CFG_DBK)
#define ext_diag_printf_v(id, file_name, line_num, fmt, ...)
#endif
#define ext_dialog_print0(id, sz)
#define ext_dialog_print1(id, sz, p1)
#define ext_dialog_print2(id, sz, p1, p2)
#define ext_dialog_print3(id, sz, p1, p2, p3)
#define ext_dialog_print4(id, sz, p1, p2, p3, p4)
#if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER)
EXT_EAPI td_u32 UAPI_DIAG_SendNdmFrame(td_u16 packet_id, td_pbyte puc_packet, td_u16 packet_size,
    td_u8 aucMacAddr[EXT_PLC_RELAY_NUM_MAX][EXT_PLC_MAC_ADDR_LEN], td_u32 sn);
#endif

td_u32 UAPI_SYS_route_table_show(td_u8 *buf, td_u32 buf_len);

td_u32 uapi_sys_reboot(td_u32 cause);

EXT_END_HEADER
#endif /* __SOC_SAL_H__ */
