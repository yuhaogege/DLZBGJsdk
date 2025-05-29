/*
 * Copyright (c) CompanyNameMagicTag 2019-2020. All rights reserved.
 * Description: APP power manager.
 */

#include <soc_types_rom.h>
#include <soc_mdm_cipher.h>
#include <soc_mdm_nv.h>
#include <soc_lowpower.h>
#include <soc_sal_nv.h>

#ifdef __cplusplus
extern "C" {
#endif
static td_void aes_lp_prepare(td_void)
{
    uapi_lowpower_forbid(EXT_LOWPOWER_ID_SPACC, EXT_SLEEP_LIGHT);
}
static td_void aes_lp_restore(td_void)
{
    uapi_lowpower_unforbid(EXT_LOWPOWER_ID_SPACC, EXT_SLEEP_LIGHT, 0);
}

static td_void hash_lp_prepare(td_void)
{
    uapi_lowpower_forbid(EXT_LOWPOWER_ID_HASH, EXT_SLEEP_LIGHT);
}
static td_void hash_lp_restore(td_void)
{
    uapi_lowpower_unforbid(EXT_LOWPOWER_ID_HASH, EXT_SLEEP_LIGHT, 0);
}

static td_void trng_lp_prepare(td_void)
{
    uapi_lowpower_forbid(EXT_LOWPOWER_ID_TRNG, EXT_SLEEP_LIGHT);
}
static td_void trng_lp_restore(td_void)
{
    uapi_lowpower_unforbid(EXT_LOWPOWER_ID_TRNG, EXT_SLEEP_LIGHT, 0);
}

static td_void rsa_lp_prepare(td_void)
{
    uapi_lowpower_forbid(EXT_LOWPOWER_ID_RSA, EXT_SLEEP_LIGHT);
}
static td_void rsa_lp_restore(td_void)
{
    uapi_lowpower_unforbid(EXT_LOWPOWER_ID_RSA, EXT_SLEEP_LIGHT, 0);
}

static td_void uapi_lowpower_vote_register(td_void)
{
    EXT_AES_REGISTER_S aes_cfg;
    EXT_HASH_REGISTER_S hash_cfg;
    EXT_TRNG_REGISTER_S trng_cfg;
    EXT_RSA_REGISTER_S rsa_cfg;

    aes_cfg.prepare_func = aes_lp_prepare;
    aes_cfg.restore_func = aes_lp_restore;
    uapi_aes_register(aes_cfg);

    hash_cfg.prepare_func = hash_lp_prepare;
    hash_cfg.restore_func = hash_lp_restore;
    uapi_hash_register(hash_cfg);

    trng_cfg.prepare_func = trng_lp_prepare;
    trng_cfg.restore_func = trng_lp_restore;
    uapi_trng_register(trng_cfg);

    rsa_cfg.prepare_func = rsa_lp_prepare;
    rsa_cfg.restore_func = rsa_lp_restore;
    uapi_rsa_register(rsa_cfg);
}

td_void app_pm_init(td_void)
{
    td_u32 ret;
    ext_nv_sys_startup cfg;

    ret = uapi_nv_read(EXT_NV_SYS_STARTUP_ID, (td_pvoid)&cfg, sizeof(ext_nv_sys_startup));
    if (ret == EXT_ERR_SUCCESS) {
        uapi_lowpower_vote_register();
        /* configure watchdog interrupt to wake up sleep */
        uapi_lowpower_wakeup_config(EXT_LOWPOWER_ID_WDG, TD_TRUE);
        /* configure PLC communication interrupt to wake up sleep */
        uapi_lowpower_wakeup_config(EXT_LOWPOWER_ID_PHY, TD_TRUE);
        /* configure UART0 communication interrupt to wake up sleep */
        uapi_lowpower_wakeup_config(EXT_LOWPOWER_ID_UART0, TD_TRUE);
        /* configure UART1 communication interrupt to wake up sleep */
        uapi_lowpower_wakeup_config(EXT_LOWPOWER_ID_UART1, TD_TRUE);
        /* configure high precision timer interrupt to wake up sleep */
        uapi_lowpower_wakeup_config(EXT_LOWPOWER_ID_TIMER0, TD_TRUE);
        /* configure system timer interrupt to wake up sleep */
        uapi_lowpower_wakeup_config(EXT_LOWPOWER_ID_TIMER5, TD_TRUE);
        /* configure system timer interrupt to wake up sleep */
        uapi_lowpower_wakeup_config(EXT_LOWPOWER_ID_GPIO, TD_TRUE);
#if defined(PRODUCT_CFG_SUPPORT_DEEP_SLEEP)
        uapi_init_lowpower_deep_sleep_proc();
#endif
        (td_void) uapi_lowpower_init(cfg.lightsleep, cfg.deepsleep, EXT_LP_DELAY_DEFAULT_TIME);
    }
}

#ifdef __cplusplus
}
#endif

