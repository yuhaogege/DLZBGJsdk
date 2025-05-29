/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: sal_inf
 */
#ifndef SAL_INF_H
#define SAL_INF_H
#include <soc_types.h>
EXT_START_HEADER
#include <soc_sal.h>
#include <soc_ndm.h>
#include <soc_mdm_task.h>

EXT_EAPI EXT_CONST ext_ue_product_ver *sal_get_product_version(td_void);

typedef struct {
    td_u16 trans_id;
    td_u16 rsd;

    td_u16 user_data;
    td_u16 payload_size;

    td_pvoid payload;
} ext_nd_remote_packet;

#include <dfx_sal.h>

#if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER)
EXT_EAPI td_u32 diag_report_dremote_package(td_u16 ulId, td_pbyte pucPacket, td_u16 usPacketSize,
    ext_nd_remote_frame_ind_head *pstRemoteFrm, td_bool bSync);
#else
#define diag_report_dremote_package
#endif

#define EXT_SYS_SET_EXCEPTION_ISR     4
#define EXT_SYS_SET_EXCEPTION_ISR_E   5
#define EXT_SYS_SET_EXCEPTION_ISR_CNT 6
#define EXT_SYS_SET_PRODUCT_ID_CHIP    11
#define EXT_SYS_SET_PRODUCT_ID_PROUDCT 12
#define EXT_SYS_SET_EXCEPTION_MAX 20
td_u32 uapi_get_rst_times(td_void);

#if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER)
/* subcontracting parameters when handing in dbk */
typedef struct {
    td_u16 cmd_id;
    td_u16 packet_size;        /* subcontract data size */
    td_u16 fragment_id;        /* sub-package sn number, each sub-packet is incremented from 0 */
    td_u16 fragment_count : 9; /* total number of sub-packets */
    /*
     * 0: no need to form a package to report;
     * 1: need to form a complete package to report.
     *    in the original packet application scenario of the mac, the value is 0
     */
    td_u16 rsb_pkt_flag : 1;
    td_u16 is_last_packet : 1; /* reply to the last packet of the message */
    td_u16 rsd : 5;            /* reserved, default is 0 */
    td_u16 fragment_trans_id; /* one packet is used for the same transid. */
    /* this value is obtained by ext_diag_getfragmenttransid */
    td_u16 trans_id;

    td_pbyte puc_packet; /* subcontracted data area */
} ext_diag_fragment_packet_param_stru;

EXT_EAPI td_u32 diag_report_fragment_pkt(td_u8 instance_id,
    const ext_diag_fragment_packet_param_stru *param, td_bool sync);

#endif /* PRODUCT_CFG_CORE_TYPE_BOOTLOADER */

EXT_END_HEADER

#endif /* __SAL_INF_H__ */

