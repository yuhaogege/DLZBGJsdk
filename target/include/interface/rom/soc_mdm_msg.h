/**
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 *
 * Description: message APIs
 * @defgroup hct_msgqueue Message queue
 * @ingroup osa
 * @li Wait message: the wait message API cannot be called, to avoid uncontrollable exception scheduling.
 * @li TX message: In the interrupt off context, the message TX API cannot be called, to avoid uncontrollable exception
 * scheduling.
 * @li TX message (the timeout period is not 0): In the interrupt and task locked context, the message TX API cannot be
 * called, to avoid uncontrollable exception scheduling.
 */

#ifndef __SOC_MDM_MSG_H__
#define __SOC_MDM_MSG_H__
#include <soc_types_rom.h>
#include <soc_config_rom.h>

/**
 * @ingroup hct_msgqueue
 *
 * Failed to read the message queue.
 */
#define EXT_MSG_INVALID_MSG_NUM 0xFFFFFFFF

/**
 * @ingroup hct_msgqueue
 *
 * Default number of parameters in the system message structure
 */
#define EXT_SYS_MSG_PARAM_NUM_MAX 4

/**
 * @ingroup hct_msgqueue
 *
 * Default system message structure type
 */
typedef struct {
    td_u32 msg_id;                             /**< Message ID */
    uintptr_t param[EXT_SYS_MSG_PARAM_NUM_MAX]; /**< Message parameter */
} ext_sys_queue_msg;

/**
 * @ingroup  hct_msgqueue
 * @brief   Creates a message queue.
 *
 * @par Description:
 * Creates a message queue.
 *
 * @attention The number of message queues supported by the system needs to be set during initialization.
 *
 * @param  id                        [OUT] Type #td_u32* Handle of the created message queue
 * @param  name                      [IN]  Type #td_char* Pointer to the message queue name. The corresponding string
 * space needs to be statically stored by the caller and is not cached at the underlying layer.
 * @param  queue_len                 [IN]  Type #td_u16 Message queue length, that is, the number of messages that can
 * be stored in the message queue
 * @param  msg_size                  [IN]  Type #td_u32 Size of each message in the message queue (unit: byte)
 *
 * @retval #EXT_ROM_ERR_SUCCESS           Success
 * @retval #EXT_ROM_ERR_MSG_INVALID_PARAM An input argument is incorrect, the handle pointer is null, the name address is
 * null, or the message queue length is 0.
 * @retval #EXT_ROM_ERR_MSG_CREATE_Q_FAIL An error occurred when creating the message queue, for example, insufficient
 * memory or insufficient message queue resources.
 * @par Dependency:
 * @li soc_mdm_msg.h: Describes message queue APIs.
 * @li soc_config.h: Describes the message queue configuration.
 * @li soc_errno.h: Describes file configuration error codes.
 * @see uapi_msg_queue_delete
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_msg_queue_create(EXT_OUT td_u32 *id, td_char name[8], td_u16 queue_len, td_u32 msg_size);

/**
 * @ingroup  hct_msgqueue
 * @brief  Deletes a message queue.
 *
 * @par Description:
 * Deletes a message queue.
 *
 * @attention None
 * @param  id                        [IN] Type #td_u32 Message queue handle
 *
 * @retval #EXT_ROM_ERR_SUCCESS           Success
 * @retval #EXT_ROM_ERR_MSG_Q_DELETE_FAIL An error occurred with the message queue, for example, the ID is out of range,
 * the message queue is not created, or the message queue is in use.
 * @par Dependency:
 * @li soc_mdm_msg.h: Describes message queue APIs.
 * @li soc_config.h: Describes the message queue configuration.
 * @li soc_errno.h: Describes file configuration error codes.
 * @see uapi_msg_queue_create
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_msg_queue_delete(td_u32 id);

/**
 * @ingroup  hct_msgqueue
 * @brief   Sends a message.
 *
 * @par Description:
 * Sends a message.
 *
 * @attention None
 * @param  id                        [IN] Type #td_u32 Message queue handle
 * @param  msg                       [IN] Type #td_pvoid Message content pointer
 * @param  timeout                   [IN] Type #td_u32 Timeout period for sending a message. The value 0 indicates that
 * the message is sent immediately.
 * @param  msg_size                  [IN] Type #td_u32 Size of the sent message (unit: byte)
 *
 * @retval #EXT_ROM_ERR_SUCCESS           Success
 * @retval #EXT_ROM_ERR_MSG_SEND_FAIL     An error occurs with the message queue, for example, an input argument is
 * incorrect, the message queue is not created, the size of the sent data is greater than the configured size when the
 * queue is created, or the API is used in an interrupt but the timeout period is not 0.
 * @retval #EXT_ROM_ERR_MSG_INVALID_PARAM An input argument is incorrect or the message queue pointer is null.
 * @par Dependency:
 * @li soc_mdm_msg.h: Describes message queue APIs.
 * @li soc_config.h: Describes the message queue configuration.
 * @li soc_errno.h: Describes file configuration error codes.
 * @see uapi_msg_queue_wait
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_msg_queue_send(td_u32 id, td_pvoid msg, td_u32 timeout, td_u32 msg_size);

/**
 * @ingroup  hct_msgqueue
 * @brief  Receives a message.
 *
 * @par Description:
 * Receives a message.
 *
 * @attention None
 * @param  id                        [IN]     Type #td_u32 Message queue handle
 * @param  msg                       [OUT]    Type #td_pvoid Message content pointer
 * @param  timeout                   [IN]     Type #td_u32 Timeout period for receiving a message. The value
 * #EXT_SYS_WAIT_FOREVER indicates permanent wait.
 * @param  msg_size                  [IN]     Type #td_u32 Expected message length (unit: byte)
 *
 * @retval #EXT_ROM_ERR_SUCCESS           Success
 * @retval #EXT_ROM_ERR_MSG_WAIT_FAIL     An error occurs with the message queue, for example, an input argument is
 * incorrect, the message queue is not created, the size of the waiting message is greater than the size set when the
 * queue is created, or the API is used in an interrupt but the timeout period is not 0.
 * @retval #EXT_ROM_ERR_MSG_INVALID_PARAM An input argument is incorrect or the message queue pointer is null.
 * @retval #EXT_ROM_ERR_MSG_WAIT_TIME_OUT No message is received when the waiting times out.
 * @par Dependency:
 * @li soc_mdm_msg.h: Describes message queue APIs.
 * @li soc_config.h: Describes the message queue configuration.
 * @li soc_errno.h: Describes file configuration error codes.
 * @see uapi_msg_queue_send
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_msg_queue_wait(td_u32 id, EXT_OUT td_pvoid msg, td_u32 timeout, td_u32 msg_size);

/**
 * @ingroup  hct_msgqueue
 * @brief   Checks whether the message queue is full.
 *
 * @par Description:
 * Checks whether the message queue is full.
 *
 * @attention None
 * @param  id        [IN] Type #td_u32 Message queue handle
 *
 * @retval #TD_TRUE  The message queue is full or the message queue information fails to be obtained.
 * @retval #TD_FALSE The message queue is not full.
 * @par Dependency:
 * @li soc_mdm_msg.h: Describes message queue APIs.
 * @see None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_bool uapi_msg_queue_is_full(td_u32 id);

/**
 * @ingroup  hct_msgqueue
 * @brief   Obtains the number of used message queues.
 *
 * @par Description:
 * Obtains the number of used message queues.
 *
 * @attention None
 * @param  id                           [IN] Type #td_u32 Message queue handle
 *
 * @retval #EXT_ROM_ERR_MSG_INVALID_MSG_NUM  Failed to read the message queue.
 * @retval Not EXT_ROM_ERR_MSG_INVALID_MSG_NUM Number of used message queues
 * @par Dependency:
 * @li soc_mdm_msg.h: Describes message queue APIs.
 * @see None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_msg_queue_get_msg_num(td_u32 id);

/**
 * @ingroup  hct_msgqueue
 * @brief  Obtains the number of message queues.
 *
 * @par Description:
 * Obtains the number of message queues.
 *
 * @attention None
 * @param  id                           [IN] Type #td_u32 Message queue handle
 *
 * @retval #EXT_ROM_ERR_MSG_INVALID_MSG_NUM  An error occurs with the message queue. For example: An input argument is
 * incorrect, or the message queue is not created.
 * @retval Not EXT_ROM_ERR_MSG_INVALID_MSG_NUM Number of message queues
 * @par Dependency:
 * @li soc_mdm_msg.h: Describes message queue APIs.
 * @see None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_msg_queue_get_msg_total(td_u32 id);

#endif
