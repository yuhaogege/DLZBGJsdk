/*
 * Copyright (c) CompanyNameMagicTag 2012-2019. All rights reserved.
 * Description: 1. Delete the mrs directory sal_nvm.h code
 * Author: CompanyName
 * Create: 2012-08-21
 */

#ifndef __SAL_NVM_H__
#define __SAL_NVM_H__

/* Function name: NVM_WriteFactory */
/* Description: Write to factory NV item */
/* Parameter description: ulNVItemID [in] NV item ID */
/* Return value: EXT_ERR_SUCCESS indicates that the NV item was deleted successfully. */
/* ERR_SAL_FLASH_ALIGN_ERROR Flash alignment error */
/* ERR_SAL_FLASH_PROTECT_ERROR Flash protection */
/* Call requirements: Before the interface returns, */
/* if other tasks have called the original write NV interface (NVIM_WriteItem) to modify the production NV, */
/* will also take effect and there is a risk. Need to be guaranteed by the user; */
EXT_EXTERN td_u32 NVM_WriteFactory(td_u16 id, EXT_CONST td_pvoid data, td_u16 data_len);

/* Function name: NVIM_GetItemLenById */
/* Description: Get the length of the NV item */
/* Parameter description: ulId [in] NV item ID */
/* pusItemDataLen[out]: returns the actual length of the read */
/* Return value: ERR_MSP_SUCCESS indicates that the NV item can be successfully read. */
/* ERR_MSP_NV_ITEM_IS_EMPTY NV item is empty */
/* ERR_MSP_NV_NOT_SUPPORT_ID NV item ID does not exist */
/* ERR_MSP_NV_NOT_SUPPORT_LENTH NV item ID length exceeds the predefined actual length */
/* ERR_MSP_FLASH_ALIGN_ERROR Flash alignment error */
/* ERR_MSP_FLASH_PROTECT_ERROR Flash protection */
/* ERR_MSP_NOT_SUPPORT is not supported */
/* Call requirements: used in the task, can not be used in the interrupt, support multi-tasking operation. */
/* Call example: None */
/* Author: CompanyName */
EXT_EXTERN td_u32 NVM_GetItemLength(td_u16 id, td_u16 *pus_item_length);

#define NVIM_UE_SOFT_PRIVATE_BUILD_TIME_LEN (16)
#define NVIM_UE_PRODUCT_NAME_LEN (32)
#define NVIM_UE_PRIVATE_PRODUCT_NAME_LEN (32)
#define NVIM_UE_HW_NAME_LEN (32)
#define NVIM_UE_PRIVATE_HW_NAME_LEN (32)
#define NVIM_MANUF_ID_LEN (32)


/* ASL common NV item definition */
/* [0x0400-0x0500) --> Share */
/* [0x0500-0x0580) --> Headend */
/* [0x0580-0x0600) --> Terminal */
typedef enum {
    EN_NV_ID_AUTHORITY_VER = 0x0400,
    EN_NV_ID_AUTHORITY_TYPE,
    EN_NV_ID_IMEI,
    EN_NV_ID_IMSI,
    EN_NV_ID_BOARD_BSN,
    EN_NV_ID_BOARD_SN,
    EN_NV_ID_CS_SIM_LOCK_FUNC_SWITCH,
    EN_NV_ID_CS_SIM_LOCK,
    EN_NV_ID_MAGIC_NUM,
    EN_NV_ID_MSP_DEVICE_TYPE,
    EN_NV_ID_WATCH_DOG,
    NV_ID_SW_VER,
    NV_ID_MSP_CORE_DUMP

    /* max 0x0500 */
} asl_nv_item_enum;

/* Information stored in the flash of each NV item */
typedef struct {
    td_u16 item_id;
    td_u16 length;
    td_u32 flash_offset; /* The offset of the data in flash */
} nvm_lkp_table_node_stru;

typedef enum {
    EN_POM_SYSMODE_NORMAL = 0,
    EN_POM_SYSMODE_FTM,
    EN_POM_SYSMODE_ETM,
    EN_POM_SYSMODE_UNKNOWN
} sym_sysmode_enum;

/* Write NV system MODE information */
/* The corresponding operation ID is ID_NV_MSP_POM_WORK_MODE */
typedef struct {
    sym_sysmode_enum sys_mode;
    td_u32 is_nonsignal; /* 1: mean non-signaling mode 0: signaling mode */
} nvim_sys_mode_stru;

typedef struct {
    td_u8 parent_mode;
    td_u8 child_mode;
    td_u8 padding[2]; /* Reserved 2 bytes for alignment */
} nvim_private_sys_mode_stru;

typedef enum {
    MSP_OPNV_PARTITION = 0,
    MSP_PLNV_PARTITION,
    MSP_PSNV_LINE_PARTITION,
    MSP_URNV_PARTITION,
    MSP_MSPSPECIAL_PARTITION /* The partition can be read and written under any circumstances,
                              * just internal operations */
} msp_nv_partition_enum;

typedef struct {
    /* td_u32 ulType;       // 0: initial file, 1: current file */
    td_u32 file_len;
    td_u8 *data;
} nvm_export_file_info_stru;

/* Does not contain a non-private NV structure definition, */
/* the NV structure corresponds to the Polaris V100R001 NV list.xls */
/* only contains the structure used by MSP */
/* table structure refresh, the structure needs to be synchronized */
/* B */
/* ID£ºEN_NV_ID_CS_SIM_LOCK_FUNC_SWITCH */
typedef struct {
    td_u32 sim_lock_func_swt;  /* 0 is off, 1 is on */
    td_u32 simlock_fail_times; /* remaining maximum attempts */
    td_u32 max_lock_times;     /* maximum number of locks */
} nvm_cs_sim_lock_func_switch_stru;

#define AUL_MNC_NUM_SIZE 20
#define AUL_START_MCC_MNC_SIZE 20
#define AUL_END_MCC_MNC_SIZE 20
/* ID: EN_NV_ID_CS_SIM_LOCK */
typedef struct {
    td_u32 seg_num;
    td_u32 aul_mnc_num[AUL_MNC_NUM_SIZE]; /* MNC digits, 2 or 3 digits */
    td_u32 aul_start_mcc_mnc[AUL_START_MCC_MNC_SIZE];
    td_u32 aul_end_mcc_mnc[AUL_END_MCC_MNC_SIZE];
} nvm_cs_sim_lock_info_stru;

/* E */
/* Increase the ability to determine whether to start the software watchdog function
 * according to the specific NV item; */
#define NVIM_WTD_OPEN 1
#define NVIM_WTD_CLOSE 0
typedef struct {
    td_u16 watch_dog_flag; /* 0: Closed. 1: open */
    td_u16 period;         /* watch dog cycle */
} nvim_watchdog_stru;

/* E */
#define NVIM_COREDUMP_OPEN 1
#define NVIM_COREDUMP_CLOSE 0
typedef struct {
    td_u32 core_dump_flag; /* 0: Off. 1: Open. Default off */
} nv_id_msp_core_dump_stru;

#define BUILD_TIME_SIZE 32
#define SW_VER_SIZE 32
typedef struct {
    td_u8 build_time[BUILD_TIME_SIZE]; /* Compile time, format such as Oct 15 2009 16:27:00, ending with \0 */
    td_u8 sw_ver[SW_VER_SIZE];         /* external software version number, ending with \0 */
} nv_id_sw_ver_stru;

typedef enum {
    EN_SOFT_VERSIONV = 0,
    EN_SOFT_VERSIONR,
    EN_SOFT_VERSIONC,
    EN_SOFT_VERSIONB,
    EN_SOFT_VERSIONSPC
} msp_nv_version_enum;

typedef enum {
    NVIM_REVERT_SUSS,                /* recovery succeeded */
    NVIM_REVERT_FAIL,                /* recovery failed */
    NVIM_REVERT_BLOCK,               /* automatic recovery hangs */
    NVIM_REVERT_BLOCK_FOR_NO_BACKUP, /* Automatically resume without backup and hang */
    NVIM_REVERT_FAIL_FOR_NO_BACKUP   /* Force recovery without backup and fail */
} nvim_revert_state_en;

/* Function name: nvm_GetNvMainInfo */
/* Function description: Get the global index information of the NV module */
/* Parameter description: pulNvCount [out] Count of all NV items */
/* pstNvimInfo[out] pointer to the NV index table */
/* Author: CompanyName */
td_u32 nvm_GetNvMainInfo(td_u32 *pul_nv_count, nvm_lkp_table_node_stru **nvim_info);

/* Function name: nvm_GetInitState */
/* Function Description: Get the initialization status of the NV module */
/* Parameter Description: */
/* Return value Description: 0 NV initialization is successful, */
/* you can call the interface, otherwise NV is not available */
/* Author: CompanyName */
td_u32 nvm_GetInitState(td_void);

/* Set the partition to be readable, readable and writable according to the input SECTION ID. */
td_u32 nvm_SetPartitionPolicy(msp_nv_partition_enum part, td_bool write, td_bool read);

/* NV initialization phase 1, only used for initialization, can get the relevant NV file attribute information. */
td_u32 nvm_InitPhase1(td_pvoid tmp_cache, td_u32 size);

/* NV initialization phase 2, after initialization, can access NV items normally. */
td_u32 nvm_InitPhase2(td_void);

#if !defined(PRODUCT_CFG_CORE_TYPE_BOOTLOADER) && defined(PRODUCT_CFG_NVM_EXT_SUPPORT)
/* Get manufacturer information */
td_u32 nvm_PrivateGetManufacturerId(EXT_OUT td_char szManufacturerId[NVIM_MANUF_ID_LEN]);
#endif

td_u32 nvmCountBadSectorForData(td_void);
td_u32 nvmCountBadSectorForIndex(td_void);

/* NV item change call interface processing */
td_u32 nvm_ChangedProcRun(td_u16 id);

td_u32 nvm_ChangeSysMode(sym_sysmode_enum mode);

td_u32 NVM_ReadItem(td_u16 id, EXT_OUT td_void *data, td_u16 data_len, td_u16 *pus_real_data_len);

typedef struct {
    td_u32 nv_file_sector_addr; /* nv file area start address */
    td_u32 nv_file_sector_num;  /* number of sectors in the nv file */
    td_u32 erase_sector_addr;   /* sector start address (index area address) to be erased */
    td_u16 erase_sector_num;    /* number of sectors erased */
} nvm_nvfile_update;

td_u32 NVM_File_Check(td_u32 addr);

/* Function name: NVM_UpdateNvFileFactorySection */
/* Function Description: Synchronize the NV item of the Factory to the upgrade NV file */
/* Parameter Description: */
/* td_pbyte pucInitFile [in] NV upgrade file buffer */
/* ulInitFileSize [in] NV upgrade file buffer size */
/* pstFileUpdate [out] returns the information needed for the upgrade, if it is TD_NULL, it is ignored. */
/* kaddr [in] runs the flash location of the kernel */
/* return value: */
/* EXT_ERR_SUCCESS: The function executed successfully */
/* Others are error codes. */
/* Call requirements: */
/* Author: CompanyName */
EXT_EAPI td_u32 NVM_UpdateNvFileFactorySection(EXT_INOUT td_pbyte puc_init_file, td_u32 init_file_size,
    EXT_OUT nvm_nvfile_update *file_update);

EXT_EAPI td_u32 NVM_ReadNvFileItem(td_u16 id, td_pvoid data, td_u16 data_len);

/* Function Description : Read the number of NV items in the NV file */
/* Parameter description: pstNvFile[in] : Upgrade file */
/* Return value: EXT_ERR_SUCCESS The operation was successful */
/* EXT_ERR_FAILURE NV file FLASH read failed */
EXT_EAPI td_u32 NVM_GetNvFileItemNum(td_void);

/* Function name: NVM_GetNvFileAddr */
/* Description: Get the starting address of the NV file */
/* Parameter description: None */
/* Return Value: Non-zero: Indicates the starting address of the NV file in the NV area */
/* zero : indicates that a valid address could not be found */
/* Author: CompanyName */
EXT_EAPI td_u32 NVM_GetNvFileAddr(td_void);

#endif
