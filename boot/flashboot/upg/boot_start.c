/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: boot start process
 * Author: CompanyName
 * Create: 2018-10-31
 */

#include "boot_start.h"
ext_start_ctrl g_start_ctrl;

/* solve the situation:the ue always restart when nv_file not ok and boot upg file exist */
EXT_PRV td_bool boot_upg_is_upgrade_boot_reason(ext_start_upg_reason upg_reason)
{
    return ((upg_reason == EXT_START_UPG_REASON_UPG) ? TD_TRUE : TD_FALSE);
}

EXT_PRV td_u32 boot_upg_process_phase1(ext_start_upg_reason upg_reason, td_u32 sub_reason)
{
    td_u32 ret;
    ext_start_ctrl *start_ctrl = &g_start_ctrl;
    ext_21_upg_head *upg_head = start_ctrl->upg_head; /* point to head of upgrade file */
    start_ctrl->upg_reason = upg_reason;

    /* the second stage of upgrade file check: secure head check, and the first stage check succeed */
    ret = boot_upg_check_upg_phase2();
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    /* condition of upgrading kernel:
     * upgrade file is kernel and all the situation of upgrading includes:
     * start-up info list check failed, kernel check failed, nv check failed,
     * running version does not match with the upgrade version.
     */
    if (upg_head->file_type == EXT_21_UPG_TYPE_KERNEL) {
        /* execute kernel upgrading */
        return boot_upg_kernel_process();
    }
    if (upg_head->file_type == EXT_21_UPG_TYPE_BOOT && (boot_upg_is_upgrade_boot_reason(upg_reason) == TD_TRUE)) {
        /* the only condition of boot upgrading:
         * upgrade file is boot and running version does not match with the upgrade */
        /* execute boot upgrading */
        return boot_upg_process_boot();
    }
    return EXT_ERR_UPG_FAIL_1;
}

td_u32 boot_upg_process(ext_start_upg_reason upg_reason, td_u32 sub_reason)
{
    ext_start_tbl *start_tbl = ext_start_tbl_get_tbl();

    td_u32 ret;

    boot_msg2("boot_upg_process reason=", upg_reason, sub_reason);
    /* the first step: execute upgrading process */
    ret = boot_upg_process_phase1(upg_reason, sub_reason);
    boot_msg1("boot_upg_process_phase1 ret=", ret);

    /* the second step: execute the process according to upgrading result */
    if (ret == EXT_ERR_SUCCESS) { /* reset if upgrade succeed */
        boot_upg_clear_upg_magic_all();
        boot_reset();
    } else {
        if (g_start_ctrl.start_enable) { /* start-up  normally if upgrade failed and the present file is able to run */
            boot_firmware_new(g_start_ctrl.flash_addr_ker + start_tbl->tbl_len);
        } else { /* reset if upgrade failed and present file is unable to run */
            boot_panic(ret);
        }
    }

    return EXT_ERR_SUCCESS;
}

td_void boot_start_pre_check(td_void)
{
    td_u32 ret;
    /* verify the start info list */
    ret = boot_start_check_read_tbl_info();
    if (ret != EXT_ERR_SUCCESS) {
        (td_void)boot_upg_check_upg_phase0();
        (td_void)boot_upg_check_upg_phase1();
        boot_upg_process(EXT_START_UPG_REASON_TBL_ERR, ret);
    }

    /* verify the kernel */
    ret = boot_start_check_kernel();
    if (ret != EXT_ERR_SUCCESS) {
        (td_void)boot_upg_check_upg_phase0();
        (td_void)boot_upg_check_upg_phase1();
        boot_upg_process(EXT_START_UPG_REASON_TBL_CODE1_ERR, ret);
    }
}
td_u32 boot_start_main(td_void)
{
    td_u32 ret;
    ext_start_ctrl *start_ctrl = &g_start_ctrl;

    boot_msg1("\n[PROCESS]boot_start magic=", PRODUCT_CFG_BOOT_VER_MAGIC);
    /* notify kernel with boot key position in flash */
    uapi_secure_save_key_pos();

    /* 1.initialize partition info and product ID by factory nv */
    ret = boot_upg_init_phase1();
    if (ret != EXT_ERR_SUCCESS) {
        boot_panic(ret);
    }

    /* 2.RAM info unsing for process of start-up initialization */
    (td_void)boot_upg_init_phash2();

    /* verify */
    boot_start_pre_check();

    /* mark the current running partition can start-up normally.  Use nv file check result following for upgrading,
       not use for start-up.
        * 1.execute upgrade when start info list and kernel are incorrect.  Not allow to start-up when upgrade file
        is incorrect.
        * 2.execute upgrade when nv file erro. allow to start-up when upgrade file is incorrect. */
    start_ctrl->start_enable = TD_TRUE;

    /* verify original nv file */
    ret = boot_start_check_nvfile();
    upg_debug_print1("nv_check ret=", ret);
    if (ret != EXT_ERR_SUCCESS) {
        (td_void)boot_upg_check_upg_phase0();
        (td_void)boot_upg_check_upg_phase1();
        boot_upg_process(EXT_START_UPG_REASON_TBL_NV_ERR, ret);
    }

    /* --------------for now, current version's reached the running condition-------------------- */
    (td_void)boot_upg_check_upg_phase0();
    boot_msg4("upg_info", start_ctrl->upg_file_phas0_ok, start_ctrl->new_kernel, start_ctrl->upg_magics_exist,
        start_ctrl->upg_file_phas1_ok);
    if (start_ctrl->upg_file_phas0_ok == TD_TRUE) {
        if (start_ctrl->new_kernel == TD_FALSE) {
            /* the first check stage for upgrade file.
             * enter the process of upgrading if boot or kernel version does not match with the running version
             */
            ret = boot_upg_check_upg_phase1();
            boot_msg1("pha1 ret=", ret);
            if (start_ctrl->ver_magic_different && start_ctrl->upg_file_phas1_ok && boot_upg_is_do_upg_from_nv()) {
                boot_upg_process(EXT_START_UPG_REASON_UPG, start_ctrl->upg_file_type);
            }
        } else if (start_ctrl->new_kernel == TD_TRUE && start_ctrl->upg_magics_exist) {
            boot_upg_clear_upg_magic_one();
            ret = boot_upg_check_upg_phase1();
            boot_msg2("ph1 ret=", ret, g_start_ctrl.flash_addr_upg);
            if (start_ctrl->upg_file_phas1_ok) {
                boot_upg_process(EXT_START_UPG_REASON_UPG, start_ctrl->upg_file_type);
            }
        }
    }

    boot_firmware_new(start_ctrl->flash_addr_ker + start_ctrl->start_tbl->tbl_len);
    return EXT_ERR_SUCCESS;
}
