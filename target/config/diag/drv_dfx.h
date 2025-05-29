/*
 * Copyright (c) CompanyNameMagicTag 2012-2018. All rights reserved.
 * Description: DFX of driver interfaces
 * Author: CompanyName
 * Create: 2012-07-16
 */

#ifndef DRV_DFX_H
#define DRV_DFX_H
#include <soc_types.h>
#include <dfx_sys.h>
#include <soc_mdm_mem.h>
#include <soc_mdm_pga.h>
#include <soc_mdm_adc.h>
#include <dw21_platform.h>
#include <soc_lowpower.h>
EXT_START_HEADER

/* Enable the following internal debugging items only when necessary. */
#define TASK_DEBUG 1

/* Note: After ***** is added, the value of ID_DIAG_CMD_DRV_OS_MAX must be changed to the last command. */
#define DRV_DFX_TASK_UPLOAD 6 /* Number of tasks uploaded each time. */
#define DRV_DFX_TASK_MAX 30   /* Total number of tasks */
#define DRV_DFX_TIMER_MAX 40
#define DRV_DFX_TRACE_MAX 10

#define DRV_DFX_KEY_NUM_MAX 12 /* Number of keys */

typedef struct _dfx_sys_mem_info {
    ext_mdm_mem_info_s sys_mem_info;
    td_u32 reset_handler_size;
    td_u32 rom_data_size;
    td_u32 rom_bss_size;
    td_u32 data_size;
    td_u32 bss_size;
    td_u32 sys_stack_size;
    td_u32 permanent_text_size;
    td_u32 dyn_size;
    td_u32 heap_size;
    td_u32 bsp_heap_size;
    td_u32 mac_heap_size;
    td_u32 app_heap_size;
} dfx_sys_mem_info;

typedef enum _dfx_os_obj_class {
    DFXOSOBJINVALID = 0,
    DFXOSOBJCPU,
    DFXOSOBJTASK,
    DFXOSOBJMEM,
    DFXOSOBJTIMER,
    DFXOSOBJMAX
} dfx_os_obj_class;

typedef enum {
    UPLOAD_DATA_012 = 12, /* Naming format: function name_num */
    DFX_TASK_SHOW_018 = 18,
    UPLOAD_DATA_019 = 19,
    UPLOAD_DATA_CONFIG_020,
} drv_dfx_log_id_enum;

typedef struct _dfx_os_wd_info {
    td_u16 wd_id;        /* watchdog ID */
    td_u8 mode;          /* watchdog mode */
    td_u8 status;        /* status of watchdog */
    td_u32 handle;       /* Pointer, which may change. This parameter is reserved. */
    td_u32 *wd_rtn;      /* routine to call upon expiration */
    td_u32 wd_param;     /* arbitrary parameter to routine */
    td_u32 remain_ticks; /* number of ticks before expiration */
} dfx_os_wd_info;

typedef struct _dfx_os_stack_info {
    td_u32 base;   /* stack base */
    td_u32 end;    /* stack end */
    td_u32 size;   /* stack size */
    td_u32 cur;    /* current stack usage in bytes */
    td_u32 high;   /* maximum stack usage in bytes */
    td_u32 margin; /* current stack margin in bytes */
} dfx_os_stack_info;

typedef struct _dfx_os_task_info {
    char name[12];           /* task name is of 12 chars */
    td_u32 *entry;           /* no used */
    td_u32 *id;              /* task ID */
    td_s32 pri;              /* task priority */
    char status[12];         /* task status is of 12 chars */
    td_u32 usedmem;          /* Original PC */
    td_u32 sp;               /* task stack pointer */
    td_u32 pad1;             /* 48 */
    dfx_os_stack_info stack; /* task execution stack */
    td_u32 sem_id;           /* First member of the original exception stack */
    td_u32 evtmask;          /* Second member of the original exception stack */
    td_u32 pc;
    td_u32 lr;
    td_u32 xpsr;
    td_u32 pad2[1];                       /* 96 */
    td_u32 *stack_trc[DRV_DFX_TRACE_MAX]; /* task stack trace */
} dfx_os_task_info;
/* Task structure. Many 16 bits occupy 32 bits. This parameter is temporarily not used. */
typedef struct _dfx_os_task_stc_size {
    char name[12];   /* task name is of 12 chars */
    td_u16 exe_high; /* task execution stack */
    td_u16 exc_high; /* task exception stack */
} dfx_os_task_stc_size;

typedef struct {
    td_u16 exe_high;
    td_u16 exc_high;
} dfx_os_task_stcsize_item;

typedef struct {
    dfx_os_task_stcsize_item item[25]; /* Entry has 25 items at maximum */
} dfx_os_task_stcsize_report;

typedef struct {
    td_u32 m_queue[10]; /* m_queue is of 10 words */
    td_u32 a_queue[3];  /* a_queue is of 3 words */
} dfx_os_queuestat_report;

typedef struct _dfx_os_spy_task {
    char name[12];         /* task name is of 12 chars */
    td_u32 *entry;         /* task entry function */
    td_u32 *id;            /* task ID of this record entry */
    td_s32 pri;            /* task priority */
    td_u32 total_ticks;    /* task ticks since spy start */
    td_u32 total_per_cent; /* task percentage since spy start */
    td_u32 inc_ticks;      /* task mode ticks since last report */
    td_u32 inc_per_cent;   /* task percentage since last report */
} dfx_os_spy_task;

typedef struct _dfx_os_spy_info {
    td_u32 total_ticks;    /* all ticks since start */
    td_u32 total_per_cent; /* all percentage since start */
    td_u32 inc_ticks;      /* all ticks since last report */
    td_u32 inc_per_cent;   /* all percentage since last reprot */

    td_u32 kernel_total_ticks;       /* sum of knl ticks across CPUs since start */
    td_u32 kernel_total_per_cent;    /* sum of knl percentage across CPUs since start */
    td_u32 interrupt_total_ticks;    /* sum of int ticks across CPUs since start */
    td_u32 interrupt_total_per_cent; /* sum of int percentage across CPUs since start */
    td_u32 idle_total_ticks;         /* sum of idle ticks across CPUs since start */
    td_u32 idle_total_per_cent;      /* sum of idle percentage across CPUs since start */

    td_u32 kernel_inc_ticks;       /* CPU kernel mode ticks since last report */
    td_u32 kernel_inc_per_cent;    /* CPU kernel mode percentage since last report */
    td_u32 interrupt_inc_ticks;    /* CPU interrupt mode ticks since last report */
    td_u32 interrupt_inc_per_cent; /* CPU interrupt mode percentage since last report */
    td_u32 idle_inc_ticks;         /* CPU idle mode ticks since last report */
    td_u32 idle_inc_per_cent;      /* CPU idle mode ticks percentage last report */

    td_u32 n_tasks;
    dfx_os_spy_task task_info[DRV_DFX_TASK_MAX];
} drv_dfx_spy_info;

typedef struct {
    td_u32 cnt;
    td_u32 is_last;
    dfx_os_wd_info ast_entrys[DRV_DFX_TIMER_MAX];
} drv_dfx_timer_info;

typedef struct {
    td_u32 cnt;
    td_u32 is_last;
    dfx_os_task_info ast_entrys[DRV_DFX_TASK_UPLOAD];
} drv_dfx_task_info_upload;

typedef struct {
    td_u32 cnt;
    dfx_os_task_info ast_entrys[DRV_DFX_TASK_MAX];
} drv_dfx_task_info;

typedef struct {
    td_u32 start_addr;
    td_u32 end_addr;
    td_u32 agc_gain; /* 0xFFFFFFFF, NO fixed AGC */
    td_u32 point_num;
    td_u32 timeout;
    td_u16 data_src;
    td_u16 triger_type;
} ext_diag_upload_start_req;

typedef struct {
    td_u32 return_code;
    td_u32 total_length;
} ext_diag_upload_start_ind;

typedef enum {
    EXT_SI4438_CMD_TYPE_POWERUP = 0x0,       /* 0: offset 0x0 */
    EXT_SI4438_CMD_TYPE_CFG_INIT,            /* 1: offset 0x10 */
    EXT_SI4438_CMD_TYPE_PARTINFO,            /* 2: offset 0x20 */
    EXT_SI4438_CMD_TYPE_PARTINFO_PHASE2,     /* 3: offset 0x30 */
    EXT_SI4438_CMD_TYPE_START_TX,            /* 4: offset 0x40 */
    EXT_SI4438_CMD_TYPE_START_RX,            /* 5: offset 0x50 */
    EXT_SI4438_CMD_TYPE_INT_STATUS,          /* 6: offset 0x60 */
    EXT_SI4438_CMD_TYPE_INT_STATUS_PHASE2,   /* 7: offset 0x70 */
    EXT_SI4438_CMD_TYPE_SET_PROPERTY,        /* 8: offset 0x80 */
    EXT_SI4438_CMD_TYPE_CHANGE_STATE,        /* 9: offset 0x90 */
    EXT_SI4438_CMD_TYPE_FIFO_INFO,           /* 10: offset 0xa0 */
    EXT_SI4438_CMD_TYPE_FIFO_INFO_PHASE2,    /* 11: offset 0xb0 */
    EXT_SI4438_CMD_TYPE_TX_FIFO,             /* 12: offset 0xc0 */
    EXT_SI4438_CMD_TYPE_RX_FIFO,             /* 13: offset 0xd0 */
    EXT_SI4438_CMD_TYPE_GET_PROPERTY,        /* 14: offset 0xe0 */
    EXT_SI4438_CMD_TYPE_GET_PROPERTY_PHASE2, /* 15: offset 0xf0 */
    EXT_SI4438_CMD_TYPE_PIN_CFG,             /* 16: offset 0x100 */
    EXT_SI4438_CMD_TYPE_FRR_A_READ,          /* 17: offset 0x110 */
    EXT_SI4438_CMD_TYPE_MAX = 18,            /* The DBK cannot identify the enumerated value.
                                             * This issue is temporarily avoided.
                                             */
} ext_si4438_cmd_type_en;

typedef struct {
    td_u32 ndm_work_mode; /* The value 0 indicates the normal mode, and the value 2 indicates the NDM test mode. */
    td_u32 ret;
    td_u32 reserved;
} ext_dfx_ndm_mode_cfg;

#define MAX_VIR_BLOCK 80

extern td_u32 g_time0_count;
extern td_u32 g_time1_count;
extern td_u32 g_time2_count;
extern td_u32 g_time3_count;
extern td_u32 g_time4_flag;
extern td_u32 g_time4_count;
extern td_u32 g_time5_flag;
extern td_u32 g_time5_count;

typedef struct {
    td_u32 swap_count;
    td_u32 swap_count_per_sec;
    td_u32 block_swapin[MAX_VIR_BLOCK];

    td_u32 handle_fc_once_time;
    td_u32 handle_fc_average_time;

    td_u32 handle_pb_once_time;
    td_u32 handle_pb_average_time;

    td_u32 handle_burst_once_time;
    td_u32 handle_burst_average_time;

    td_u32 cco_handle_dbk_cmd_once_time;
    td_u32 cco_handle_dbk_cmd_average_time;

    td_u32 handle_tx_task_once_time;
    td_u32 handle_tx_task_average_time;

    td_u32 sta_handle_response_once_time;
    td_u32 sta_handle_response_average_time;
} diag_cmd_remap_vir_block_fifo_count_stru;

typedef struct time_measure {
    td_u32 rx_us1;
    td_u32 rx_us2;
    td_u32 rx_us;
    td_u32 rx_max;
} time_measure_t;

extern time_measure_t g_time_measure_t[6]; /* 6 items in time_measure entry */

EXT_EXTERN ext_dsid_drv_eth_info *uapi_drv_get_net_dfx(td_void);

td_u32 sdkVerQry(td_u16 id, EXT_IN td_pvoid cmd_param, td_u16 cmd_param_size, td_u8 option);

typedef struct {
    td_u32 num; /* Test No. */
    ext_pga_cfg pga_cfg;
} pga_test_req;

typedef struct {
    td_u32 num; /* Test No. */
    td_u32 ret;
} pga_test_ind;

typedef struct {
    td_u32 num; /* Test Case No. */
    ext_adc_channel_index_en channel_idx;
    td_u32 data_ptr;
    td_u32 data_size;
    td_u32 timer_us;
    /* For internal use only */
    td_u16 ret;
    td_bool is_finished;
    td_u8 resv;
    td_u32 write; /* 256 */
} adc_test_req;

typedef struct {
    td_u32 num; /* Test No. */
    td_u32 ret;
    td_u32 para; /* Test Duration */
} adc_test_ind;

/* ADC test */
#define TIMER_REG_BASE (TIMER1_REG_BASE)
#define TIMER_IRQNR 8

#define TIMER_LOADCOUNT_REG 0x00
#define TIMER_CURRCOUNT_REG 0x04
#define TIMER_CONTROL_REG 0x08
#define TIMER_EOI_REG 0x0c
#define TIMER_INTSTATUS_REG 0x10

#define TIMER_ENABLE_BIT (1 << 0)
#define TIMER_PERIOD_BIT (1 << 1)
#define TIMER_INTEN_BIT (1 << 2)
#define TIMER_LOCK_BIT (1 << 3)

#define TIMER_CLOCK_BASE 25

#define timer_writrel(reg, val) UAPI_REG_WRITE32(TIMER_REG_BASE + (reg), (val))

#define timer_readl(reg) UAPI_REG_READ_VAL32(TIMER_REG_BASE + (reg))

EXT_END_HEADER
#endif /* DRV_DFX_H */
