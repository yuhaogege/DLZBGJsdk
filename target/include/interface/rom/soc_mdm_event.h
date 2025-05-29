/**
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 *
 * Description: system event API
 * @li Before sending, waiting, or clearing an event, the event must be created to obtain the event ID.
 * @li Wait event: The wait event API cannot be called in the interrupt, interrupt off,
 * and lock task contexts to avoid uncontrollable exceptional scheduling.
 * @li TX event: The TX event API cannot be called in the interrupt off context to avoid uncontrollable exceptional
 * scheduling.
 * @li Each bit of bit[0:23] of an event can represent an event type. The meaning of each bit is allocated by the user.
 * @li Bit[24:31] of an event are reserved and cannot be used by the user.
 *
 */
#ifndef __SOC_MDM_EVENT_H__
#define __SOC_MDM_EVENT_H__
#include <soc_types_rom.h>
#include <soc_mdm_types_rom.h>

#define EXT_INVALID_EVENT_ID (0xffffffff) /**< Failed to obtain the event ID. */
#define EXT_EVENT_WAITMODE_AND                                       \
    (4) /**< If all expected events occur, the wait is successful. \
                 It cannot be used with EXT_EVENT_WAITMODE_OR at the same time. */
#define EXT_EVENT_WAITMODE_OR                                                \
    (2)                           /**< If any of the expected events occurs, the wait is successful. \
                                           It cannot be used with EXT_EVENT_WAITMODE_AND at the same time. */
#define EXT_EVENT_WAITMODE_CLR (1) /**< The waited event is cleared when the wait event is successful. */

/**
 * @ingroup  hct_event
 * @brief  Creates an event.
 *
 * @par Description:
 * Creates an event to obtain the event ID.
 *
 * @attention None
 *
 * @param  id       [OUT] Type #td_u32* Event ID
 * @param  name     [IN]  Type #td_char Event name
 *
 * @retval #0       Success
 * @retval #Other values     Failure. For details, see soc_errno.h.
 *
 * @par Dependency:
 * @li soc_mdm_event.h: Describes event APIs.
 * @see  uapi_event_delete
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_event_create(td_u32 *id, td_char name[8]); /* 8 len */

/**
 * @ingroup  hct_event
 * @brief  Defines the TX event.
 *
 * @par Description:
 * Defines the TX event.
 *
 * @attention None
 *
 * @param  id         [IN] Type #td_u32 Event ID
 * @param  event_bits [IN] Type #td_u32 Set of events to be sent, which may be one bit or multiple bits in bits 0-23.
 *
 * @retval #0         Success
 * @retval #Other values       Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_event.h: Describes event APIs.
 * @see  uapi_event_wait
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_event_send(td_u32 id, td_u32 event_bits);

/**
 * @ingroup  hct_event
 * @brief  Defines the wait event.
 *
 * @par Description:
 * Defines the wait event.
 *
 * @attention None
 *
 * @param  id         [IN]  Type #td_u32 Event ID
 * @param  wait_bits  [IN]  Type #td_u32 Set of events to be waited for, which may be one bit or multiple bits in bits
 * 0-23.
 * @param  event_bits [OUT] Type #td_u32* Set of events to be waited for when the wait event is successful.
 * @param  timeout    [IN]  Type #td_u32 Waiting timeout period (unit: ms)
 * @param  flag       [IN]  Type #td_u32 Waiting option. For details, see #EXT_EVENT_WAITMODE_AND,
 * #EXT_EVENT_WAITMODE_OR, and #EXT_EVENT_WAITMODE_CLR.
 *
 * @retval #0         Success
 * @retval #Other values       Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_event.h: Describes event APIs.
 * @see  uapi_event_send
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_event_wait(td_u32 id, td_u32 wait_bits, EXT_OUT td_u32 *event_bits, td_u32 timeout, td_u32 flag);

/**
 * @ingroup  hct_event
 * @brief  Defines the clearing event.
 *
 * @par Description:
 * Defines the clearing event.
 *
 * @attention None
 *
 * @param  id         [IN] Type #td_u32 Event ID
 * @param  event_bits [IN] Type #td_u32 Set of events to be cleared, which may be one bit or multiple bits in bits 0-23.
 *
 * @retval #0         Success
 * @retval #Other values       Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_event.h: Describes event APIs.
 * @see  uapi_event_wait
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_event_clear(td_u32 id, td_u32 event_bits);

/**
 * @ingroup  hct_event
 * @brief  Defines the deletion event.
 *
 * @par Description:
 * Defines the deletion event (releasing an event ID).
 *
 * @attention None
 *
 * @param  id   [IN] Type #td_u32 Event ID
 *
 * @retval #0    Success
 * @retval #Other values  Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_event.h: Describes event APIs.
 * @see  uapi_event_create
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_event_delete(td_u32 id);

/**
 * @ingroup  hct_event
 * @brief  Initializes event resources.
 *
 * @par Description:
 * Initializes event resources. This API is called during system initialization only once. Currently, it is called in
 * sal_res.c. Change the number of event resources based on the site requirements.
 *
 * @attention None
 * @param  max_event_cnt   [IN] Type #td_u8 Number of event resources.
 * This parameter needs to be modified during initialization when the app adds an event.
 * @param  p_event_space   [IN] Type #td_pvoid Event resource space. If the value is null,
 * it indicates that the space is applied internally. If this parameter is not null,
 * external space is used to create event resources. Currently, set this parameter to TD_NULL.
 *
 * @retval #0              Success
 * @retval #Other values            Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_event.h: Describes event APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_event_init(td_u8 max_event_cnt, td_pvoid p_event_space);

#endif
