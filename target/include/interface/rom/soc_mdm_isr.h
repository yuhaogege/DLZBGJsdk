/**
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * @defgroup hct_isr Interrupt
 * @ingroup osa
 */

#ifndef __SOC_MDM_ISR_H__
#define __SOC_MDM_ISR_H__

#include <soc_types_rom.h>

#define EXT_EXC_FLAG_NO_FLOAT 0x10000000
#define EXT_EXC_FLAG_FAULTADDR_VALID 0x01
#define FLAG_HWI_ACTIVE 0x20000

typedef struct {
    /* handler save */
    td_u32 r4;
    td_u32 r5;
    td_u32 r6;
    td_u32 r7;
    td_u32 r8;
    td_u32 r9;
    td_u32 r10;
    td_u32 r11;
    td_u32 pri_mask;
    /* auto save */
    td_u32 sp;
    td_u32 r0;
    td_u32 r1;
    td_u32 r2;
    td_u32 r3;
    td_u32 r12;
    td_u32 lr;
    td_u32 pc;
    td_u32 xpsr;
} ext_exc_context_s;

/**
 * @ingroup  hct_isr
 * @brief  HISR callback function type
 *
 * @par Description:
 * HISR callback function type
 *
 * @attention None
 * @param  data1 [IN] Type #td_u32 Callback input parameter 1
 * @param  data2 [IN] Type #td_u32 Callback input parameter 2
 * @param  data3 [IN] Type #td_u32 Callback input parameter 3
 * @param  data4 [IN] Type #td_u32 Callback input parameter 4
 * @param  data5 [IN] Type #td_u32 Callback input parameter 5
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_isr.h: Describes ISR APIs.
 * @see uapi_irq_add_job
 * @since DW21_V100R001C00
 */
typedef td_void (*hisr_proc_func)(td_u32 data1, td_u32 data2, td_u32 data3, td_u32 data4, td_u32 data5);

/**
 * @ingroup  hct_isr
 * @brief  LISR callback function type
 *
 * @par Description:
 * LISR callback function type
 *
 * @attention None
 * @param  data [IN] Type #td_u32 Callback input parameter 1
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_isr.h: Describes ISR APIs.
 * @see uapi_irq_request
 * @since DW21_V100R001C00
 */
typedef td_void (*lisr_proc_func)(td_u32 data);

/**
* @ingroup  hct_isr
* @brief  Interrupt off
*
* @par Description:
*           Interrupt off
*
* @attention
*           @li A function that causes scheduling cannot be executed in an interrupt off context, for example, uapi_sleep
and other blocked APIs.
*           @li Interrupt off only protects short-time operations that can be expected. Otherwise, the interrupt
response and the performance may be affected.
* @param  None
*
* @retval # Interruption status value  Interrupt status before interrupt off
* @par Dependency:
           @li soc_mdm_isr.h: Describes interrupt APIs.
* @see  uapi_int_restore
* @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_int_lock(td_void);

/**
 * @ingroup  hct_isr
 * @brief  Restores the status before interrupt off.
 *
 * @par Description:
 * Restores the status before interrupt off.
 *
 * @attention The input argument must be the value of CPSR that is saved before interrupt off.
 * @param  int_value [IN] Type #td_u32 Interrupt status
 *
 * @retval None
 * @par Dependency:
 * @li soc_mdm_isr.h: Describes interrupt APIs.
 * @see  uapi_int_lock
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_void uapi_int_restore(td_u32 int_value);

/**
 * @ingroup  hct_isr
 * @brief  Enables a specified interrupt.
 *
 * @par Description:
 * Enables a specified interrupt.
 *
 * @attention None
 * @param  vector [IN] Type #td_u32 Interrupt ID
 * @li For details, see section 2.6.1 in the DW21 V100 SoC Data Sheet.
 * @li Interrupt number = Number in the SoC data sheet ¨C 16. For example, the WDT interrupt is numbered 0.
 * @retval None
 * @par Dependency:
 * @li soc_mdm_isr.h: Describes interrupt APIs.
 * @see  uapi_irq_disable
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_void uapi_irq_enable(td_u32 vector);
EXT_EXTERN td_void uapi_irq_enable_patch(td_u32 vector);

/**
 * @ingroup  hct_isr
 * @brief  Disables a specified interrupt.
 *
 * @par Description:
 * Disables a specified interrupt.
 *
 * @attention None
 * @param  vector [IN] Type #td_u32 Interrupt ID
 * @li For details, see section 2.6.1 in the DW21 V100 SoC Data Sheet.
 * @li Interrupt number = Number in the SoC data sheet minus 16. For example, the WDT interrupt is numbered 0.
 * @retval None
 * @par Dependency:
 * @li soc_mdm_isr.h: Describes interrupt APIs.
 * @see  uapi_irq_enable
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_void uapi_irq_disable(td_u32 vector);
EXT_EXTERN td_void uapi_irq_disable_patch(td_u32 vector);

/**
 * @ingroup  hct_isr
 * @brief  Registers an interrupt.
 *
 * @par Description:
 * Registers an interrupt.
 *
 * @attention The interruption handling program cannot take too long a time, which affects the timely response of the
 * CPU to the interrupt.
 * @param  vector   [IN] Type #td_u32 Interrupt ID
 * @li For details, see section 2.6.1 in the DW21 V100 SoC Data Sheet.
 * @li Interrupt number = Number in the SoC data sheet minus 16. For example, the WDT interrupt is numbered 0.
 * @param  priority [IN] Type #td_u16 Interrupt priority. The priority is unified internally. Interrupt nesting is not
 * supported. This parameter does not take effect.
 * @param  routine  [IN] Type #td_pvoid Interrupt callback function.
 * @param  param    [IN] Type #td_u32 Parameter transferred to the callback function
 *
 * @retval #0       Success
 * @retval #Other values     Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_isr.h: Describes interrupt APIs.
 * @see  uapi_irq_free
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_irq_request(td_u32 vector, td_u16 priority, lisr_proc_func routine, td_u32 param);
EXT_EXTERN td_u32 uapi_irq_request_patch(td_u32 vector, td_u16 priority, lisr_proc_func routine, td_u32 param);

/**
 * @ingroup  hct_isr
 * @brief  Clears a registered interrupt.
 *
 * @par Description:
 * Clears a registered interrupt.
 *
 * @attention None
 * @param  vector [IN] Type #td_u32 Interrupt ID
 * @li For details, see section 2.6.1 in the DW21 V100 SoC Data Sheet.
 * @li Interrupt number = Number in the SoC data sheet minus 16. For example, the WDT interrupt is numbered 0.
 * @retval #0     Success
 * @retval #Other values   Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_isr.h: Describes interrupt APIs.
 * @see  uapi_irq_request
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_irq_free(td_u32 vector);
EXT_EXTERN td_u32 uapi_irq_free_patch(td_u32 vector);

#define EXT_JOB_PRIORITY_0 (0)
#define EXT_JOB_PRIORITY_1 (1)
#define EXT_JOB_PRIORITY_2 (2)
#define EXT_JOB_PRIORITY_3 (3)
#define EXT_JOB_PRIORITY_4 (4)
#define EXT_JOB_PRIORITY_5 (5)
#define EXT_JOB_PRIORITY_6 (6)
#define EXT_JOB_PRIORITY_7 (7)
#define EXT_JOB_PRIORITY_8 (8)
#define EXT_JOB_PRIORITY_9 (9)
#define EXT_JOB_PRIORITY_10 (10)
#define EXT_JOB_PRIORITY_11 (11)
#define EXT_JOB_PRIORITY_12 (12)
#define EXT_JOB_PRIORITY_13 (13)
#define EXT_JOB_PRIORITY_14 (14)
#define EXT_JOB_PRIORITY_15 (15)

/**
 * @ingroup  hct_isr
 * @brief  Creates the interrupt HISR function. The second half of the interrupt is mounted to the workqueue.
 *
 * @par Description:
 * Creates the interrupt HISR function. The second half of the interrupt is mounted to the workqueue.
 *
 * @attention None
 * @param  priority [IN] Type #td_u32 Priority of the second half of the interrupt. The value is 0-15
 * (EXT_JOB_PRIORITY_xx). A smaller value indicates a higher priority.
 * @param  func     [IN] Type #HISR_PROC_FUNC Entrance to the function for handling the second half of the interrupt.
 * @param  data1    [IN] Type #td_u32 Parameter 1 transferred to the callback function
 * @param  data2    [IN] Type #td_u32 Parameter 2 transferred to the callback function
 * @param  data3    [IN] Type #td_u32 Parameter 3 transferred to the callback function
 * @param  data4    [IN] Type #td_u32 Parameter 4 transferred to the callback function
 * @param  data5    [IN] Type #td_u32 Parameter 5 transferred to the callback function
 *
 * @retval #0       Success
 * @retval #Other values     Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_isr.h: Describes interrupt APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_irq_add_job(td_u32 priority, hisr_proc_func func, td_u32 data1, td_u32 data2, td_u32 data3,
    td_u32 data4, td_u32 data5);

/**
 * @ingroup  hct_isr
 * @brief  Checks whether it is in the interrupt context.
 *
 * @par Description:
 * Checks whether it is in the interrupt context.
 *
 * @attention None
 * @param  None
 *
 * @retval #0     Not in the interrupt context
 * @retval #1     In the interrupt context
 * @par Dependency:
 * @li soc_mdm_isr.h: Describes interrupt APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_bool uapi_is_int_context(td_void);

/**
 * @ingroup  hct_isr
 * @brief  Clears the interrupt suspension status.
 *
 * @par Description:
 * Clears the interrupt suspension status.
 *
 * @attention None
 * @param  vector [IN] Type #td_u32 Interrupt ID
 * @li For details, see section 2.6.1 in the DW21 V100 SoC Data Sheet.
 * @li Interrupt number = Number in the SoC data sheet minus 16. For example, the WDT interrupt is numbered 0.
 * @retval None
 * @par Dependency:
 * @li soc_mdm_isr.h: Describes interrupt APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_void uapi_isr_clear_pending_irq(td_u32 vector);

#endif
