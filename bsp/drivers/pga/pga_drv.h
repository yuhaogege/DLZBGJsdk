/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Upload SAR-ADC/PGA driver code. Pga_drv.h code
 * Author: CompanyName
 * Create: 2012-12-11
 */
#ifndef __PGA_DRV_H__
#define __PGA_DRV_H__

#include <soc_mdm_pga.h>
#include <soc_mdm_sem.h>
#include <soc_mdm_types.h>
#include <soc_mdm_isr.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    td_bool is_init;
    td_u8 resv[3]; /* keep 3 bytes for alignment */
    td_u32 pga_sem;
} pga_ctl;

extern pga_ctl g_pga_ctl;

#define pga_lock()                                               \
    do {                                                         \
        if (uapi_is_int_context() == 0) {                              \
            uapi_sem_wait(g_pga_ctl.pga_sem, EXT_SYS_WAIT_FOREVER); \
        }                                                        \
    } while (0)
#define pga_unlock()                          \
    do {                                      \
        if (uapi_is_int_context() == 0) {           \
            uapi_sem_signal(g_pga_ctl.pga_sem); \
        }                                     \
    } while (0)

#define REG_PGA_BASE 0x40040000
#define REG_ANALOG_WR40 (REG_PGA_BASE + 0x34)
#define REG_ANALOG_WR44 (REG_PGA_BASE + 0x38)

/* REG_ANALOG_WR40 */
typedef struct {
    td_u32 resv1 : 7;
    td_u32 clk_gate : 1;

    td_u32 buf_clk_sel : 2;
    td_u32 buf_clk_rst : 1;
    td_u32 buf_pd_clk : 1;
    td_u32 pga_clk_sel : 2;
    td_u32 pga_clk_rst : 1;
    td_u32 pga_pd_clk : 1;

    td_u32 rsvd2 : 16;
} reg_analog_wr40;

/* REG_ANALOG_WR44 */
typedef struct {
    td_u32 sar_vcm_debug : 2;
    td_u32 pd_pga0 : 1;
    td_u32 pd_pga1 : 1;
    td_u32 pga_ctune : 4;

    td_u32 pga0_gain : 1;
    td_u32 pga1_gain : 1;
    td_u32 pga_vcm_sel : 2;
    td_u32 pga0_lp : 2;
    td_u32 pga1_lp : 2;

    td_u32 resv1 : 16;
} reg_analog_wr44;

#ifdef __cplusplus
}
#endif

#endif
