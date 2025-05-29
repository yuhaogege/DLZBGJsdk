/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description:  DFX_SAL
 * Author: CompanyName
 * Create: 2018-10-31
 */
#ifndef __DFX_SAL_H__
#define __DFX_SAL_H__
#include <soc_ft_nv.h>
#include <dfx_sys.h>
#include <soc_mdm_os_stat.h>

#define SAL_SUPPORT_SYS_ERR_INFO_API

/* ***************************************************************************** */
/* Command ID definition [0x7000, 0x8000) */
/* ***************************************************************************** */
#define ID_DIAG_CMD_REMOTE (0x7000)
#define ID_DIAG_CMD_REMOTE_USR (0x7001)
#define ID_DIAG_CMD_REMOTE_END (0x7002)
#define ID_DIAG_CMD_SYS_ERR_INFO (0x7003)     /* syserr, IND --> ext_syserr_info_s */
#define ID_DIAG_CMD_SYS_ERR_INFO_CLS (0x7004) /* clssyserr */
#define ID_DIAG_CMD_SYS_ERR_DUMP (0x7005)     /* syserr, IND --> ext_syserr_info_s */
#define ID_DIAG_CMD_DO_SYS_ERR (0x7006)       /* dosyserr */
#define ID_DIAG_CMD_DO_FRAME_ERR (0x7007)     /* DIAG error report */
#define ID_DIAG_CMD_DUMP_FILE (0x7008)
#define ID_DIAG_CMD_DUMP_MEM2K_FILE (0x7009)
#define ID_DIAG_CMD_SYS_CMD_END (0x700A)

#define ID_DIAG_CMD_SAL_SDM (0x7012) /* , ,DIAG_CMD_SAL_SYS_SDM_S */

#define ID_DIAG_CMD_SYS_ERR_DUMP_FINISH (0x7030) /* syserr dump finish report */
#define ID_DIAG_CMD_DUMP_FINISH (0x7031)

/* ICC measurement */
#define ID_DIAG_CMD_ICC_STAT (0x7060)

#define ID_DIAG_CMD_SAL_SET_SDM_MAC (0x7082)
#define ID_DIAG_CMD_SAL_SET_END (0x709A) /* reserved */

/* Demo : 0x7100 -- 0x7200, 0x7300--0x7350 */
/* IND */
#define ID_DIAG_CMD_SAL_SDM_DIAG_CHL (0x7900)

#define ID_DIAG_CMD_DUMP_FLASH_DIAG_START (0x7A00)
#define ID_DIAG_CMD_DUMP_FLASH_DIAG_END (0x7A01)
#define ID_DIAG_CMD_DUMP_FLASH_ERR (0x7A02)
#define ID_DIAG_CMD_DUMP_FLASH_NO_MSG (0x7A03)
#define ID_DIAG_CMD_CLEAR_FLASH_DIAG_START (0x7A04)
#define ID_DIAG_CMD_CLEAR_FLASH_DIAG_END (0x7A05)
#define ID_DIAG_CMD_TEST_CMD_SUCCESS_CNT (0x7A06)

/* The maximum number of supported modules */
#define SYS_DFX_MOD_INIT_MAX 24

#define EXT_DIAG_VER_PRODUCT_NAME_MAX_SIZE 30
#define EXT_DIAG_VER_PRODUCT_BUILD_DATE_MAX_SIZE 10
#define EXT_DIAG_VER_PRODUCT_BUILD_TIME_MAX_SIZE 8
#define EXT_DIAG_VER_PRODUCT_BUILD_HINT_MAX_SIZE 8
#define EXT_DIAG_VER_PRODUCT_BUILD_SPACE1_MAX_SIZE 1
#define EXT_DIAG_VER_PRODUCT_BUILD_SPACE2_MAX_SIZE 1
#define EXT_DIAG_VER_PRODUCT_BUILD_SEP_MAX_SIZE 1
#define EXT_DIAG_VER_PRODUCT_BUILD_TYPE_SIZE 1
#define EXT_DIAG_VER_PRODUCT_BUILD_OS_SIZE 1
#define EXT_DIAG_VER_FULL_PRODUCT_NAME_MAX_SIZE 60 /* VER Release Notes */
#define EXT_DIAG_VER_SDK_PRODUCT_NAME_MAX_SIZE 24  /* SDK version number */
#define EXT_DIAG_VER_BOOT_NAME_MAX_SIZE 8
#define EXT_DIAG_VER_FULL_HW_INFO_MAX_SIZE 40      /* Hardware Information */
#define EXT_DIAG_VER_FULL_DEV_VER_NAME_MAX_SIZE 40 /* Device Information */
#define EXT_DIAG_VER_PRODUCT_BUILD_SEP3_MAX_SIZE 1
#define EXT_DIAG_VER_PRODUCT_BUILD_WD_SIZE 1
#define EXT_DIAG_VER_CHIP_NAME_MAX_SIZE 4

/*
 * Reduce the structure, remotely query the version number command (mini_ver) structure,
 * and the members are extracted by the VER structure.
 */
typedef struct {
    td_char ver[EXT_DIAG_VER_FULL_PRODUCT_NAME_MAX_SIZE];
    td_char sdk_ver[EXT_DIAG_VER_SDK_PRODUCT_NAME_MAX_SIZE];
    td_char dev[EXT_DIAG_VER_FULL_DEV_VER_NAME_MAX_SIZE];
    td_char hw[EXT_DIAG_VER_FULL_HW_INFO_MAX_SIZE];
    td_char boot_ver[EXT_DIAG_VER_BOOT_NAME_MAX_SIZE];
} diag_cmd_soft_new_ver_stru;

/* Increase the structure used by the FEM command */
typedef struct {
    td_u32 ftm;
} diag_cmd_ftm_change_stru;

typedef enum {
    EXT_DIAG_CMD_CFG_UNKNOWN,
    EXT_DIAG_CMD_CFG_SET_CLEAN_DMS_CHL_CNT
} ext_diag_cmd_sal_cfg_set_action;

typedef struct {
    td_u32 cfg_id;
    td_u32 data;
} ext_diag_cmd_sal_cfg_set;

#ifdef EXT_SAL_DEV_DEBUG
#define ext_sal_assert(exp) EXT_SDM_VERIFY_EXPR(exp, 0, 0)
#else
#define ext_sal_assert(exp) ext_assert(exp)
#endif

typedef struct {
    td_u16 exe_high;
    td_u16 exc_high;
} diag_cmd_sal_stack_peek_info;

/* ************************************************ **************************** */
/* Parameter Description Reference << dw11V100 Copy Control Software Interface Design Specification.docx >> */
/* ************************************************ ***************************** */
typedef struct {
    td_u32 id;
} ext_dbg_stat_q;

typedef struct {
    td_u32 option;
} ext_dbg_dump_diag_items_req;

typedef struct {
    td_u32 option;
    td_char name[64]; /* len 64 bytes */
} ext_dbg_dfx_task_test;

/* ************************************************ ***************************** */
#endif /* __DFX_SAL_H__ */
