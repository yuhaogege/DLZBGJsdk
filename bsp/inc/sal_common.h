/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: Adjust the directory structure to prepare for compiling the SDK. Sal_common.h code
 * Author: CompanyName
 * Create: 2012-07-25
 */
#ifndef __SAL_COMMON_H__
#define __SAL_COMMON_H__

#define SAL_EXCLUDE_RES_CFG_INF_HRD
#include <sal_inf.h>
#include <mac_inf.h>
#include <soc_sal_nv.h>
#include <soc_ndm.h>
#include <soc_mdm_isr.h>
#include <soc_mdm_flash.h>

#include <dfx_sal.h>
#include <dfx_sys.h>

#define sal_is_valid_handle(h_handle) ((h_handle).data[0] == (td_u32)TD_NULL ? TD_FALSE : TD_TRUE)

#define sal_error_not_found_log0(sz)
#define sal_error_not_found_log1(s1)
#define sal_warning_not_find_log0(sz)
#define sal_warning_not_find_log1(s1)
#define sal_info_log0(sz)
#define sal_i_log1(sz, v1)
#define sal_e_log0(sz)
#define sal_e_log1(sz, v1)
#define sal_i_log2(sz, v1, v2)
#define sal_e_log2(sz, v1, v2)
#define SAL_MEM_PADDING 4

#define EXT_DIAG_PRINT_MOD_MASK 0x00FFFFFF
#define EXT_DIAG_PRINT_LEVEL_MASK 0xFF000000

td_u32 salInitPowwerOnMem(td_pvoid addr);
td_u32 salExitPowwerOnMem(td_void);
td_pvoid salPowerOnHeapAlloc(td_u32 size);
td_u32 salPowerOnHeapFree(td_pvoid addr);
td_void InitTestCfg(td_void);
td_u32 sal_init_sys_phase1(td_void);
td_void sal_dfx_init(td_void);
td_void salSetSysStatus(td_u8 sys_status);
td_u32 sal_GetFlashInfo(ext_flash_info_s *flash_info);

#define MSP_VER_MAGIC "MspMajorMinorRevisionVersionMagicABCD^#12"
#define msp_lib_ver(major_version, minor_version, revision_version) \
    (((((td_u16)(major_version)) << 16) | ((td_u16)(minor_version))), (revision_version))
/* message definition */
#define ID_MSG_DIAG_BASE 0x100

enum {
    ID_MSG_DIAG_DATA_TX = (ID_MSG_DIAG_BASE + 1), /* Packet reporting */
    ID_MSG_DIAG_DATA_USER_TX,                     /* User data packet report */
    ID_MSG_DIAG_ACK_RX,
    ID_MSG_DIAG_ACK_TX, /* ACK packet, need to release the parameter pointer */
    ID_MSG_DIAG_RX,
    ID_MSG_DIAG_TIMER,
    ID_MSG_DIAG_ACK_MDM_RX
};
#define DMS_MAC_FRAME_PAYLOAD_SIZE_MAX (EXT_DMS_FRM_MAX_PAYLOAD_SIZE)

typedef enum {
    SYM_SYSMODE_NORMAL = 0,
    SYM_CORE_CHILD_ONLINE,
    SYM_CORE_CHILD_UPDATE
} sym_core_child_mode_enum;

#define diag_set_packets_lost_info(x)
#define diag_msg_free0(msg) uapi_free(EXT_MOD_ID_SAL_DIAG, (td_pvoid)((msg)->param[0]))
#define diag_msg_free1(msg) uapi_free(EXT_MOD_ID_SAL_DIAG, (td_pvoid)((msg)->param[1]))
#define diag_msg_free2(msg) uapi_free(EXT_MOD_ID_SAL_DIAG, (td_pvoid)((msg)->param[2]))

#define diag_buffering_msg_free(msg) \
    diag_free_buffer((diag_buffer_id_enum)ext_lou16((msg)->param[3]), (td_pvoid)((msg)->param[0]))

td_u32 sal_nv_app_init(td_void);

/* Helper routine */
#define init_exception_polling_wait() uapi_sleep(EXT_DMS_CHL_EXCEPTION_POLLING_WAIT)
#define uart_exception_polling_wait() uapi_sleep(EXT_DMS_UART_EXCEPTION_POLLING_WAIT)

/* public macro definition */
#define sal_inc_u32(x) (x)++
#define sal_inc_u16(x) (x)++
#define sal_dec_u(x)   \
    do {               \
        if ((x) > 0) { \
            (x)--;     \
        }              \
    } while (0)
#define SDM_DMS_ID_NV_INIT 0
#define SDM_DMS_ID_TCP_D_FRM_CNT 1
#define SDM_DMS_ID_UART_D_FRM_CNT 2
#define SDM_DMS_ID_4 4

#define SDM_DMS_ID_TCP_CHL 5
#define SDM_DMS_ID_TCP_RCV_STATUS 6
#define SDM_DMS_ID_TCP_CHL_ACCEPT 7
#define SDM_DMS_ID_TCP_RCV_DATA 8
#define SDM_DMS_ID_TCP_SND_STATUS 9

#define SDM_DMS_ID_SELECT_RET 10
#define SDM_DMS_ID_NOTHING_MONITOR 11
#define SDM_DMS_ID_SELECT_INPUT1 12
#define SDM_DMS_ID_SELECT_INPUT2 13
#define SDM_DMS_ID_TCP_CHL_ACCPET_RET 14
#define SDM_DMS_ID_TC_SCK_CLOSE_RET1 15
#define SDM_DMS_ID_TC_SCK_CLOSE_RET2 16
#define SDM_DMS_ID_TC_SCK_CLOSE_RET3 17
#define SDM_DMS_ID_TC_SCK_CLOSE_RET4 18
#define SDM_DMS_ID_TC_SCK_CLOSE_RET0 19
#define SDM_DMS_ID_TCP_SND_TIME_MAX_CHK_S 20
#define SDM_DMS_ID_TCP_SND_TIME_MAX_CHK 21

#define SDM_DMS_ID_T_C_CNT_B 22
#define SDM_DMS_ID_T_C_CNT_A 23
/* Statistics of key phases (success and failure) during initialization */
typedef struct {
    td_u32 addr;                /* flash start address */
    td_u32 start_sector_id;     /* starting block number */
    td_u32 sector_size;         /* the size of a sector of flash */
    td_u32 sector_num;          /* number of sectors of flash */
    td_u32 page_size;           /* page size */
    td_u32 total_size;          /* total flash size */
    td_u32 item_num_per_sector; /* the maximum number of nv_item(temp_itme) stored in each sector */
} sal_flash_cfg_info_stru;

typedef td_bool (*diag_is_connect_f)(td_u16 usPortNum); /* Phycial channel connection status. */
#define sal_make_u32(p) (((td_u32)(p)[0] << 24) + ((td_u32)(p)[1] << 16) + ((td_u32)(p)[2] << 8) + (td_u32)(p)[3])
#define is_space(c) (((c) == 0x20) || (((c) >= 0x09) && ((c) <= 0x0D)))
#define is_digital(c) (((c) >= '0') && ((c) <= '9'))

/* ASCII stirng to S32 */
EXT_PRVL td_u32 str2s32(EXT_CONST td_char *buffer)
{
    EXT_CONST td_char *ptr = (EXT_CONST td_char *)buffer;
    td_u32 num = 0;
    td_s32 c = *ptr;

    while (is_space(c)) {
        c = *++ptr;
    }

    while (is_digital(c)) {
        num = (td_u32)((10 * num) + (c - '0'));
        c = *++ptr;
    }

    return ((td_u32)num);
}

td_u32 dms_TcpSetKeepAlive(td_u16 port_num, td_s32 enable, td_s32 tcp_keep_idle, td_s32 tcp_keep_cnt);
td_u32 dms_TcpNoDelay(td_u16 port_num, td_bool enable);
td_u32 dms_TcpRcvFrmTimeOut(td_u16 port_num, td_u16 time_out);

typedef enum {
    DIAG_CHL_PCL_DBG_RPT_OPT_CCO2STA,
    DIAG_CHL_PCL_DBG_RPT_OPT_STA2CCO,
} diag_chl_pcl_dbg_rpt_opt;

#define sal_stub_init() \
    do {                \
    } while (0)
#define SAL_DIAG_DATA_PK_PADDING sizeof(td_u32)

enum {
    MSG_OPT_DUMP_MSG = 1,
    MSG_OPT_CLEAR_MSG = 2
};
#if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER)
#define SAL_DFX_SYS_VAL_DECLARE
#endif
#define sal_user_data_stat(m)                                            \
    do {                                                                 \
        if (EXT_USERDATA_ID_APP == ucUserDataId) {                        \
            SAL_SDM_SET_SYS_INC_RET(ret, aulUserDataStat, (m), (m) + 1);     \
        } else if (EXT_USERDATA_ID_BIDB == ucUserDataId) {                \
            SAL_SDM_SET_SYS_INC_RET(ret, aulUserDataStat, (m) + 2, (m) + 3); \
        }                                                                \
    } while (0)
#define dms_chk_frm_mac_port(port_num) \
    (((td_u32)(port_num) >= (td_u32)EXT_DMS_CHL_MAC_PORT_MIN) && \
        ((td_u32)(port_num) <= (td_u32)EXT_DMS_CHL_MAC_PORT_MAX))

#define dms_chk_frm_uart_port(port_num) ((td_u32)(port_num) <= (td_u32)EXT_DMS_CHL_UART_PORT_MAX)

/* Helper */
#define EXT_BIDB_SIGNATURE uapi_make_identifier('b', 'i', 'D', 'B')
#define EXT_TF_LIST_SIGNATURE uapi_make_identifier('T', 'F', 'L', '1')
#define EXT_BLACK_LIST_SIGNATURE uapi_make_identifier('B', 'L', 'L', '1')

#define DB_VAL_MAX 0x00FFFFFF

#if defined(EXT_DEBUG)
#define SAL_BUILD_TYPE_NAME 'D'
#else
#define SAL_BUILD_TYPE_NAME 'R'
#endif

#define PRODUCT_CFG_WD_ENABLE

#if defined(PRODUCT_CFG_WD_ENABLE)
#define SAL_WDG_TYPE_NAME 'W'
#else
#define SAL_WDG_TYPE_NAME '0'
#endif

#define DW_PRODUCT_VER 1

#define DIAG_ERR_RPT_LEN 48
EXT_EXTERN td_u32 dms_CloseAllSockets(td_u32 option);
td_bool drv_is_fix_aagc_enable(td_void);
td_u32 crc32(td_u32 crc, const td_char *buf, td_u32 len);

/* *
 * @ingroup sal_common
 * @brief Turn off the AFE's receive high-pass filter
 *
 * @par Description: Call the function of this function:
 *  Global variables accessed, Modified global variables,Modified BBP register list
 * Start the upgrade, it is the record upgrade mode and upgrade address.
 *
 * @attention None.
 *
 * @retval #0 Success.
 * @retval #!0 failed. See soc_errno.h for details.
 * @par Dependency:
 * @li sal_common.h: The file is used to describe the sal common interface.
 * @see None.
 * @since DW21_V100R001C00
 */
td_void drv_close_afe_rx_hpf(td_void);
td_void low_power_change_freq(void);
td_void low_power_switch_clk(td_void);
td_void low_power_change_sfc_freq(td_void);
td_u32 diag_get_sdk_product_ver(td_char *product, td_u16 max_size);
td_u32 diag_get_product_ver(td_char *product, td_u16 max_size);
td_u32 SAL_route_table_show(td_u8 *buf, td_u32 buf_len);

EXT_EXTERN ext_stat_diag_qry g_stat_diag;

#endif /* __SAL_COMMON_H__ */
