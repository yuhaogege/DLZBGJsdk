/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Diagnosis (DIAG) Interaction APIs.
 * Author: CompanyName
 * Create: 2018-10-31
 */
/**
 * @defgroup hct_cpup CPU usage
 * @ingroup  mid
 */

#ifndef __SOC_MDM_DIAG_H__
#define __SOC_MDM_DIAG_H__

#include <soc_mdm_uart.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup  hct_diag
 * @brief Registers the callback function for DIAG channel status changes.
 *
 * @par Description:
 * Registers the callback function for DIAG channel status changes. That is, when the DIAG channel is
 * connected or disconnected, the function registered by this API is called back.
 *
 * @attention None
 * @param  port_num [IN] Type #td_u16 Port number.
 * @param  connect  [IN] Type #td_bool Connection status.
 *
 * @retval #0               Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_diag.h: Describes DIAG APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
typedef td_u32 (*ext_diag_connect_f)(td_u16 port_num, td_bool connect);

/**
 * @ingroup  hct_diag
 * @brief Registers the callback function for DIAG channel status changes.
 *
 * @par Description:
 * Registers the callback function for DIAG channel status changes. That is, when the DIAG channel is
 * connected or disconnected, the function registered by this API is called back.
 *
 * @attention None
 * @param  connect_notify [IN] Type #ext_diag_connect_f User function.
 *
 * @retval #0               Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_diag.h: Describes DIAG APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
td_u32 uapi_diag_register_connect_notify(ext_diag_connect_f connect_notify);

/**
 * @ingroup  hct_diag
 * @brief Checks the connection status of the DIAG channel.
 *
 * @par Description:
 * Checks the connection status of the DIAG channel.
 *
 * @attention None
 * @param  None
 *
 * @retval #TD_FALSE disconnected.
 * @retval #TD_TRUE  connected.
 * @par Dependency:
 * @li soc_mdm_diag.h: Describes DIAG APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
td_bool uapi_diag_is_connect(td_void);

/**
 * @ingroup  hct_diag
 * @brief Callback function for handling diagnosis commands.
 *
 * @par Description:
 *           Callback function for handling diagnosis commands, that is, the function body for executing commands.
 *
 * @attention
 *           @li If the returned value is not EXT_ERR_CONSUMED, the DIAG framework automatically forwards
 *               the returned value to the host.
 *           @li If the return value is EXT_ERR_CONSUMED, it indicates that the user sends a response (local connection)
 *               to the host through the uapi_diag_send_ack_packet API. The DIAG framework does not automatically

 *               respond to the host.
 *           @li The return value of the API cannot be EXT_ERR_NOT_FOUND.
 * @param  cmd_id         [IN] Type #td_u16 Command ID
 * @param  cmd_param      [IN] Type #td_pvoid Pointer to the user input command.
 * @param  cmd_param_size [IN] Type #td_u16 Length of the command input by the user (unit: byte).
 * @param  option         [IN] Type #td_u8#EXT_DIAG_CMD_INSTANCE_LOCAL and #EXT_DIAG_CMD_INSTANCE_IREMOTE are supported.
 *
 * @retval #0               Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 *           @li soc_mdm_diag.h: Describes DIAG APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
typedef td_u32 (*ext_diag_cmd_f)(td_u16 cmd_id, td_pvoid cmd_param, td_u16 cmd_param_size, td_u8 option);

/**
 * @ingroup  hct_diag
 * Command registration structure. The same callback function can be used within the command ID range.
 *
 * If a single command is used, the maximum and minimum IDs are the same.
 */
typedef struct {
    td_u16 min_id;              /* Minimum DIAG ID */
    td_u16 max_id;              /* Maximum DIAG ID */
    ext_diag_cmd_f fn_input_cmd; /* This Handler is used to process the DBK command. */
} ext_diag_cmd_reg_obj;

/**
 * @ingroup  hct_diag
 * @brief  Registers the command handling function.
 *
 * @par Description:
 * The DIAG subsystem supports a maximum of 20 different command tables. You can run
 * uapi_diag_get_cmd_stat_reg_info to query the number of registered command tables.
 *
 * @attention The range of command IDs that can be used by the SDK user is [0xFD00, 0xFFFF).
 * @param  cmd_tbl  [IN] Type #ext_diag_cmd_reg_obj* Command table, which must be declared as a constant array
 * and transmitted to this parameter.
 * @param  cmd_num  [IN] Type #td_u16 Number of commands. The value cannot be 0.
 *
 * @retval #0               Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_diag.h: Describes DIAG APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
td_u32 uapi_diag_register_cmd(EXT_CONST ext_diag_cmd_reg_obj *cmd_tbl, td_u16 cmd_num);

/**
 * @ingroup hct_diag
 * Statistic description structure.
 */
typedef struct {
    td_u16 id;               /**< Statistics ID. */
    td_u16 array_cnt;        /**< Number of statistic structures. */
    td_u32 stat_packet_size; /**< Size of a single statistic structure (unit: byte). */
    td_pvoid stat_packet;    /**< Pointer to the statistic structure. */
} ext_diag_sys_stat_obj;

/**
 * @ingroup  hct_diag
 * @brief  Registers the system diagnosis statistic.
 *
 * @par Description:
 * The DIAG subsystem can register a maximum of 20 statistic tables. You can run
 * uapi_diag_get_cmd_stat_reg_info to query the number of registered statistic tables.
 *
 * @attention The range of statistic IDs that can be used by the SDK user is [0xEE00, 0xF000).
 * @param  stat_obj_tbl     [IN] Type #ext_diag_sys_stat_obj_s* Table of statistics objects.
 * @param  obj_num          [IN] Type #td_u16 Number of statistics objects. The value cannot be 0.
 *
 * @retval #0               Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_diag.h: Describes DIAG APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
td_u32 uapi_diag_register_stat_obj(EXT_CONST ext_diag_sys_stat_obj *stat_obj_tbl, td_u16 obj_num);

/**
 * @ingroup  hct_diag
 * Local instance  -->DBK  Interaction command between the host software and the local station.
 */
#define EXT_DIAG_CMD_INSTANCE_DEFAULT ((td_u8)0)

/**
 * @ingroup  hct_diag
 * Local instance  -->DBK  Interaction command between the host software and the local station.
 */
#define EXT_DIAG_CMD_INSTANCE_LOCAL EXT_DIAG_CMD_INSTANCE_DEFAULT

/**
 * @ingroup  hct_diag
 * Command received from a remote station or sent from the local station to a remote station.
 */
#define EXT_DIAG_CMD_INSTANCE_IREMOTE 1

/**
 * @ingroup  hct_diag
 * Command that the host software sends to a remote station or the command received from a remote station and
 * reported to the host. (invisible in the DW21 secondary development)
 */
#define EXT_DIAG_CMD_INSTANCE_DREMOTE 2
#define EXT_DIAG_CMD_INSTANCE_DREMOTE_NDM 3 /* Not supported */
#define EXT_DIAG_CMD_INSTANCE_UNKNOWN 0xFF  /* Invalid parameter, used for exception determination. */

/**
 * @ingroup  hct_diag
 * @brief  Reports DIAG packets.
 *
 * @par Description:
 * Reports DIAG channel packets to the DIAG client.
 *
 * @attention None
 * @param  cmd_id       [IN] Type #td_u16 DIAG data packet ID. For example, if the packet ID is the same as the command
 * request ID, use the cmd_id parameter in the command callback function ext_diag_cmd_f.
 * @param  instance_id  [IN] Type #td_u8 Command type. Valid values are as follows: EXT_DIAG_CMD_INSTANCE_LOCAL and
 * EXT_DIAG_CMD_INSTANCE_IREMOTE. Use the option parameter in the command callback function
 * ext_diag_cmd_f to notify the DIAG subsystem whether the packet is a local packet or a
 * remote packet.
 * @param  packet     [IN] Type #td_pbyte Buffer address of the data packet. This function does not release the pointer.
 * @param  pakcet_size  [IN] Type #td_u16 Data packet size (unit: byte)
 * @param  sync         [IN] Type #td_bool Synchronous or asynchronous DIAG packet pushing. TRUE indicates that the
 * packets are pushed synchronously and the operation is blocked. FALSE indicates the packets
 * are pushed asynchronously (with the memory allocated, the packet is cashed by the OS queue
 * before being pushed), and the operation is not blocked.
 *
 * @retval #0               Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_diag.h: Describes DIAG APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
td_u32 uapi_diag_report_packet(td_u16 cmd_id, td_u8 instance_id, const td_pbyte packet, td_u16 pakcet_size,
    td_bool sync);

/**
 * @ingroup  hct_diag
 * @brief  Sends ACK packets to the DIAG client.
 *
 * @par Description:
 * Sends ACK packets to the DIAG client.
 *
 * @attention This API can be used only for the response to local commands, not remote commands.
 * @param  cmd_id      [IN] Type #td_u16 DIAG ACK packet ID. For example, if the packet ID is the same as the command
 * request ID, use the cmd_id parameter in the command callback function ext_diag_cmd_f.
 * @param  instance_id [IN] Type #td_u8 Command type. The value is fixed to EXT_DIAG_CMD_INSTANCE_LOCAL.
 * @param  buffer     [IN] Type #td_pvoid Buffer address of the data packet. This function does not release the pointer.
 * @param  buffer_size [IN] Type #td_u16 Buffer size (unit: byte). The maximum value is 1024.
 *
 * @retval #0               Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_diag.h: Describes DIAG APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
td_u32 uapi_diag_send_ack_packet(td_u16 cmd_id, td_u8 instance_id, EXT_IN td_pvoid buffer, td_u16 buffer_size);

#define EXT_DIAG_LOG_OPT_LOCAL_REQ 1         /**< Local command request from the host to DIAG. */
#define EXT_DIAG_LOG_OPT_LOCAL_IND 2         /**< local response from DIAG to the host. */
#define EXT_DIAG_LOG_OPT_LOCAL_ACK 3         /**< Local ACK from DIAG to the host. */
/**< Remote command request from the local station to a remote station. */
#define EXT_DIAG_LOG_OPT_REMOTE_REQ_TO_PLC 4
/**< Received remote command request from a remote station to the local * station. */
#define EXT_DIAG_LOG_OPT_REMOTE_REQ_FROM_PLC 5
/**< Remote command response from the local station to a remote station */
#define EXT_DIAG_LOG_OPT_REMOTE_RSP_TO_PLC 6
/**< Received remote command response from a remote station to the local * station. */
#define EXT_DIAG_LOG_OPT_REMOTE_RSP_FROM_PLC 7

/**
 * @ingroup  hct_diag
 * Describes the command type transferred to the app layer.
 */
typedef struct {
    td_u32 opt; /**< Option configuration, which is used to set the command as a local command or a remote
                 * command. The value is a EXT_DIAG_LOG_OPT_XXX macro such as #EXT_DIAG_LOG_OPT_LOCAL_REQ.
                 */
    td_u8 plc_mac[EXT_PLC_MAC_ADDR_LEN]; /**< MAC address of the target board of
                                         * the remote command. This parameter is not
                                         * involved in a local command.
                                         */
    td_u8 pad[2];                       /**< Reserved, not supported currently.pad 2 */
} ext_diag_log_ctrl;

/**
 * @ingroup  hct_diag
 * @brief  Callback function for notifying DIAG operations
 *
 * @par Description:
 * Carbon copies data to the app layer by using a callback function in the case of data interaction between
 * the host and the board.
 *
 * @attention It is not allowed to call an API for sending data in the DIAG subsystem in this callback function.
 * @param  cmd_id       [IN] Type #td_u16 Command ID.
 * @param  buffer       [IN] Type #td_pvoid Data pointer transferred to the app layer.
 * @param  buffer_size  [IN] Type #td_u16 Length of the data transferred to the app layer (unit: byte)
 * @param  log_ctrl     [IN] Type #ext_diag_log_ctrl* Command type sent to the app layer
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_diag.h: Describes DIAG APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
typedef td_void (*ext_diag_cmd_notify_f)(td_u16 cmd_id,
    td_pvoid buffer, td_u16 buffer_size, ext_diag_log_ctrl *log_ctrl);

/**
 * @ingroup  hct_diag
 * @brief  Registers the callback function for notifying DIAG operations.
 *
 * @par Description:
 * Carbon copies data to the app layer by using a callback function in the case of data interaction between
 * the host and the board.
 *
 * @attention If func is null, the registration is canceled and the registered notification callback is invalid.
 * @param  func  [IN] Type #ext_diag_cmd_notify_f* When data interaction occurs between the DBK and the board, this API
 * is used to notify the app layer.
 *
 * @retval #0               Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_diag.h: Describes DIAG APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
td_u32 uapi_diag_register_cmd_notify(ext_diag_cmd_notify_f func);

/**
 * @ingroup  hct_diag
 * @brief  Sets the UART parameters of the DIAG channel.
 *
 * @par Description:
 * Sets the UART parameters of the used by the DIAG subsystem before the DIAG subsystem is initialized.
 *
 * @attention The parameters must be set before the initialization of the DIAG subsystem.
 * @param  uart_port [IN] Type #ext_uart_port UART port number used by the DIAG subsystem.
 * @param  uart_cfg  [IN] Type #ext_uart_init_type UART configuration used by the DIAG subsystem.
 *
 * @retval #0               Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_diag.h: Describes DIAG APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
td_u32 uapi_diag_set_uart_param(ext_uart_port uart_port, ext_uart_init_type uart_cfg);

/**
 * @ingroup  hct_diag
 * @brief  Initializes the DIAG subsystem.
 *
 * @par Description:
 * Initializes the DIAG subsystem.
 *
 * @attention The initialization of the DIAG subsystem needs to be performed only once. Repeated initialization
 * is invalid.
 * @param  None
 *
 * @retval #0               Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_diag.h: Describes DIAG APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
td_u32 uapi_diag_init(td_void);

/**
 * @ingroup  hct_diag
 * Structure for querying the number of command tables and statistic object tables.
 */
typedef struct {
    td_u8 cmd_list_total_cnt;  /**< Number of command registration tables can be registered. */
    td_u8 cmd_list_used_cnt;   /**< Number of registered command tables. */
    td_u8 stat_list_total_cnt; /**< Number of statistic object tables can be registered. */
    td_u8 stat_list_used_cnt;  /**< Number of registered statistic object tables. */
} ext_diag_cmd_stat_reg_info;

/**
 * @ingroup  hct_diag
 * @brief  Queries the registration status of the command tables and statistic tables of the DIAG subsystem.
 *
 * @par Description:
 * Queries the registration status of the command tables and statistic tables of the DIAG subsystem.
 * Number of registered command tables and number of command tables can be registered/ Number of registered
 * statistic object tables
 *
 * @attention None
 * @param     None
 *
 * @retval #ext_diag_cmd_stat_reg_info Structure of the number of registered command tables and statistic tables.
 * @par Dependency:
 * @li soc_mdm_diag.h: Describes DIAG APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
ext_diag_cmd_stat_reg_info uapi_diag_get_cmd_stat_reg_info(td_void);

#ifdef __cplusplus
}
#endif
#endif /* __SOC_MDM_DIAG_H__ */
