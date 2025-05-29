/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Crash log management.
 * Author: CompanyName
 * Create: 2018-10-31
 */
/**
 * @defgroup hct_reset_save Crash log management
 * @ingroup  system
 */

#ifndef SOC_MDM_CRASH_H
#define SOC_MDM_CRASH_H

#include <soc_types.h>
#include <soc_mdm_types.h>
#include <soc_mdm_os_stat.h>
#include <soc_mdm_mem.h>
#include <soc_mdm_isr.h>
#include <soc_watchdog.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup  hct_reset_save
 * Version of the crash log module.
 */
#define EXT_SYSERR_VERSION 7 /* Crash log code version. */

#define EXT_SYS_ERR_SIGNATURE uapi_make_identifier('s', 'y', 's', 'E')
#define EXT_POWER_DOWN 0x646F776E    /* Power-down flag. */
#define EXT_NO_POWER_DOWN 0xFFFFFFFF /* When the system crashes, this flag is logged. */

/**
 * @ingroup  hct_reset_save
 * Length of the name string of an exception type.
 */
#define EXT_EXC_TYPE_NAME_SIZE 8

/**
 * @ingroup  hct_reset_save
 * Length of the task name string of a logged exception.
 */
#define EXT_EXC_TASK_NAME_SIZE 8

/**
 * @ingroup  hct_reset_save
 * Depth of the scheduling track before an exception.
 */
#define EXT_EXC_TRACE_DEPTH 30

/**
 * @ingroup  hct_reset_save
 * Exception stack information depth (unit: word)
 */
#define EXT_SYS_ESP_DATA_MAX_NUM 48

/**
 * @ingroup  hct_reset_save
 * Extended depth of the exception stack SP to the stack bottom (unit: word).
 */
#define EXT_SYS_ESP_PRE_DATA_MAX_NUM 16
#define EXT_SYS_ESP_PRE_DATA_PAD 0xEEEEEEEE

/**
 * @ingroup  hct_reset_save
 * Version of the running firmware when the exception occurs.
 */
#define EXT_EXC_KERNEL_VER_LEN_MAX 52

#if !defined(PRODUCT_CFG_DBK)
#ifdef EXT_BUILD_VER_PRODUCT_LEN_MAX
#if (EXT_EXC_KERNEL_VER_LEN_MAX < EXT_BUILD_VER_PRODUCT_LEN_MAX)
#error "EXT_EXC_KERNEL_VER_LEN_MAX != EXT_BUILD_VER_PRODUCT_LEN_MAX"
#endif
#endif
#endif

/**
 * @ingroup hct_reset_save
 * Crash type eid
 */
typedef enum {
    EXT_EID_FAULT_IN_TASK = 1, /**< Exception in a task. */
    EXT_EID_FAULT_IN_ISR,      /**< Exception in an interrupt. */
    EXT_EID_SYS_REBOOT,        /**< Proactive reboot. */
    EXT_EID_RESERVE,           /**< Reserved. */
    EXT_EID_MAX = 0xFF,
} ext_syserr_id;

/* Enumeration in the initialization, which can be user-defined. */
typedef enum {
    INIT_PHASE_SAL_INIT_1 = 1,
    INIT_PHASE_SAL_INIT_BEGIN = 20,
    INIT_PHASE_SAL_INIT_END,
} ext_init_phase;

/* Kernel register information and interrupt status. */
typedef struct {
    td_u32 reg_fsr;
    td_u32 reg_hfsr;
    td_u32 reg_bfar;
    td_u32 reg_shcsr;
    td_u32 reg_icsr;
    td_u32 reg_isr_pend;
    td_u32 reg_isr_active;
} ext_crash_reg_extra_info;

/* Dynamic loading information, for internal debug. */
typedef struct {
    td_u32 reg_blockmap[4]; /* 4 count */
    td_u32 vir_addr;
    td_u32 phy_addr;
    td_u32 flash_addr;
    td_u32 reg_bfsr;
    td_u32 reg_faultpc;
    td_u32 reg_bfar;
    td_u32 ret;
} syserr_remap_info;

/* Register information. */
typedef struct {
    td_u32 pc; /* Address of the current running command. */
    td_u32 sp; /* Stack pointer */
    td_u32 msp;
    td_u32 psp;
    td_u32 xpsr;
    td_u32 exc_return_lr;
    td_u32 pri_mask;
    td_u32 r[15]; /* Working register 15 count */
} syserr_reg_info;

typedef struct {
    td_char name[EXT_EXC_TASK_NAME_SIZE];
    td_u32 id;
    td_u32 status;          /* For details, see the definitions of macros such as OS_TASK_STATUS_XXX
                             * in the los_task.ph file.
                             */
    td_u8 reserve[3];       /* pad 3 */
    td_u8 stack_data_index; /* Number of stack messages extended to the stack bottom. */
    td_u16 stack_peak;
    td_u16 stack_size;
    td_u32 sp;
    td_u32 stack[2]; /* 2 count */
    td_u32 real_sp;
    td_u32 stack_pre_data[EXT_SYS_ESP_PRE_DATA_MAX_NUM]; /* Stack information extended to the stack bottom,
                                                         * for stack rollback.
                                                         */
    td_u32 stack_data[EXT_SYS_ESP_DATA_MAX_NUM];         /* Stack information. */
} syserr_task_info;                                     /* general interrupt use */

typedef struct {
    ext_os_resource_use_stat_s usage; /* Number of used system resources. */
    syserr_task_info task;           /* Task information. */
    ext_mem_pool_crash_info_s mem;    /* Dynamic memory pool information. */
} syserr_os_info;

typedef struct {
    td_u8 log_ver;                                 /* Crash log module version. */
    td_u8 eid;                                     /* Error type ID. For details, see #ext_syserr_id. */
    td_u16 rid;                                    /* Exception cause sub-ID. */
    td_u32 now_tsec;                               /* Current relative boot time. */
    td_u32 crash_tsec;                             /* Exception time relative to the boot time. */
    td_u32 init_phase;                             /* Initialization phase, corresponding to #ext_init_phase. */
    td_u32 boot_ver;                               /* Bootloader version when the exception occurs. */
    td_char kernel_ver[EXT_EXC_KERNEL_VER_LEN_MAX]; /* Running version when the exception occurs. */
    td_char type_name[EXT_EXC_TYPE_NAME_SIZE];      /* Type, corresponding to eid. */
} syserr_basic_info;

typedef struct {
    td_u32 time_ms : 31;
    td_u32 enable : 1;
} syserr_wdg_info;

typedef struct {
    td_u32 current_block : 13;
    td_u32 reserved : 18;
    td_u32 enable : 1;
    td_u32 status_reg;
} syserr_fp_info;

typedef struct {
    td_u32 hw_id : 12;
    td_u32 reserve : 18;
    td_u32 chip_ : 1;
    td_u32 jtag_mode : 1;
} syserr_hw_info;

typedef struct {
    td_u16 type;      /**< Track type. */
    td_u16 id;        /**< Track ID. */
    td_u32 data;      /**< Userdata. */
    td_u32 entry;     /**< Hook function Entry. */
    td_u32 timestamp; /**< The low 32 bit of the current time, (unit: us). */
} syserr_track_item;

typedef struct {
    td_u16 current_item;
    td_u16 item_cnt;
    syserr_track_item item[EXT_EXC_TRACE_DEPTH];
} syserr_track_info;

typedef struct {
    td_u32 data[1];
    td_u32 reg_isr_pend;
    td_u32 reg_icsr;
    td_u32 reg_shcsr;
    td_u32 reg_fsr;
    td_u32 reg_hfsr;
    td_u32 reg_bfar;
    td_u32 reg_isr_active;
    td_u16 cur_task_id;
    td_u16 new_task_id;
} syserr_core_info;

/**
 * @ingroup hct_reset_save
 * Saved exception information.
 */
typedef struct {
    td_u32 sig_s;       /**< Head flag of the exception information. */
    td_u32 power_magic; /**< Power-down magic word, for internal use. */

    td_u32 power_down : 1; /**< Power-down flag. If the value is 1, it indicates that the saved information
                            * is history information. Power down after the storage or a storage failure may
                            * occur. If the value is 0, it indicates that the system is booted after an
                            * exception without reboot.
                            */
    td_u32 crc_usable : 1; /**< If the value is 1, it indicates that the CRC is valid. */
    td_u32 info_len : 14;  /**< Total length of information, including sig_s and sig_e. */
    td_u32 crc_val : 16;   /**< CRC value from basic_info to sig_e (sig_e excluded) */

    syserr_basic_info basic_info; /**< Basic information in the exception information. */
    syserr_os_info os_info;       /**< System information in the exception information. */
    syserr_reg_info reg_info;     /**< Register information in the exception information. */
    syserr_remap_info remap_info; /**< Dynamic loading information in the exception information. */
    syserr_wdg_info wdg_info;     /**< Watchdog information in the exception information. */
    syserr_fp_info protect_info;  /**< Flash protection information in the exception information. */
    syserr_hw_info hw_info;       /**< Hardware information in the exception information. */
    syserr_track_info track_info; /**< Calling track information in the exception information. */
    syserr_core_info core_info;   /**< Chip kernel information in the exception information. */
    td_u32 debug_rsv[9];          /**< Reserved for a fixed version and can be user-defined.9count */
    td_u32 sig_e;                 /**< Tail flag of the exception information. */
} ext_syserr_info;

/**
 * @ingroup  hct_reset_save
 * @brief Initializes the exception information module.
 *
 * @par Description:
 * Initializes the exception information module.
 *
 * @attention This API can be called only once. Repeated initialization and multi-task calling are not supported.
 * @param  None
 *
 * @par Dependency:
 * @li soc_mdm_crash.h: Describes Crash log APIs.
 * @see None.
 * @since DW21_V100R001C00B020
 */
td_void uapi_syserr_init(td_void);

/**
 * @ingroup  hct_reset_save
 * @brief Obtains the system exception information pointer (in the memory).
 *
 * @par Description:
 * Obtains the system exception information pointer (in the memory).
 *
 * @attention None
 * @param  None
 *
 * @retval Pointer to the ext_syserr_info structure.
 * @par Dependency:
 * @li soc_mdm_crash.h: Describes Crash log APIs.
 * @see None.
 * @since DW21_V100R001C00B024
 */
ext_syserr_info *uapi_syserr_ptr(td_void);

/**
 * @ingroup  hct_reset_save
 * @brief Obtains the system exception information pointer (in the flash).
 *
 * @par Description:
 * Obtains the system exception information pointer (in the flash).
 *
 * @attention None
 * @param  info [OUT] Type #ext_syserr_info* Pointer to the ext_syserr_info structure.
 *
 * @retval #0               Success.
 * @retval #Other values    Failure. For details, see soc_errno.h.
 * @par Dependency:
 * @li soc_mdm_crash.h: Describes Crash log APIs.
 * @see None.
 * @since DW21_V100R001C00B024
 */
td_u32 uapi_syserr_get(ext_syserr_info *info);

/*
 * Callback function for handling the crash due to a registration exception.
 * Ensure that this function and the internal functions are in the resident RAM. (The __isr prefix can be used.)
 */
typedef td_void (*ext_syserr_exc_callback)(td_void);

/**
 * @ingroup  hct_reset_save
 * @brief Register exception handling callback.
 *
 * @par Description:
 * Register exception handling callback.
 *
 * @attention None
 * @param  None
 *
 * @par Dependency:
 * @li soc_mdm_crash.h: Describes Crash log APIs.
 * @see None.
 * @since DW21_V100R001C00B024
 */
EXT_EXTERN td_void uapi_syserr_exc_register(ext_syserr_exc_callback func);

#ifdef __cplusplus
}
#endif
#endif /* SOC_MDM_CRASH_H */
