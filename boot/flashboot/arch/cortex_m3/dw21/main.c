/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: boot start main file
 */

#include "main.h"
#include "soc_fixrom_sym.h"

EXT_PRV td_void soft_value_init(td_void)
{
    g_st_softvalue.cmd_burn_default_sram_addr = FIRMWARE_LOAD_ADDR;
    g_st_softvalue.cmd_burn_default_flash_addr = BURN_FLASH_OFFSET;
    g_st_softvalue.cmd_burn_default_flash_len = 464 * 1024; /* firmware code joint with nv :464*1024 */
    g_st_softvalue.cmd_loady_default_sram_addr = BOOTLOADER_LOAD_ADDR;
    g_st_softvalue.interrupt_char = CHAR_CTRL_C;
    g_st_softvalue.uart_interrupt_type = UART_AUTH_TYPE_CTRL_C;
    g_st_softvalue.malloc_start_addr = (td_u8 *)&__HEAP_BEGIN__;
    g_st_softvalue.malloc_end_addr = (td_u8 *)&__HEAP_END__;
    g_st_softvalue.flash_cmd_start = &__u_flash_boot_cmd_start;
    g_st_softvalue.flash_cmd_end = &__u_flash_boot_cmd_end;
    if (strcpy_s(g_st_softvalue.prompt, sizeof(CFG_PROMPT), CFG_PROMPT) != 0) {
        return;
    }
    /* By default, the flash boot function is enabled. */
}

void boot_kernel(uintptr_t kaddr)
{
    void (*entry)(void) = (void *)(kaddr + 1);
    entry();
}

void boot_firmware(void)
{
    unsigned int ret;
    boot_msg0("boot kernel ...");
    ext_flash_partition_table *flash_table = NULL;
    td_u32 flash_addr = FIRMWARE_FLASH_OFFSET;

    flash_table = (ext_flash_partition_table *)boot_malloc(sizeof(ext_flash_partition_table));
    if (flash_table != NULL) {
        ret = uapi_factory_nv_read(EXT_NV_FTM_FLASH_PARTIRION_TABLE_ID, flash_table, sizeof(ext_flash_partition_table));
        if (ret == EXT_ERR_SUCCESS) {
            if (!uint_2_multiply(flash_table->table[EXT_FLASH_PARTITON_KERNEL].addr_blk, BLOCK_SIZE, &flash_addr)) {
                return;
            }
        }
    }

    boot_free(flash_table);
    ret = uapi_flash_read(flash_addr, FIRMWARE_MAX_SIZE, (void *)FIRMWARE_LOAD_ADDR);
    if (ret) {
        boot_msg0("SPI flash read full kernel FLASH failed");
    }

    boot_kernel(FIRMWARE_LOAD_ADDR);
}

/* heap register and initialization */
EXT_PRV void fast_boot_malloc_funcs_init(void)
{
    uapi_malloc_func_s malloc_funcs = {
        0,
    };
    malloc_funcs.init = ROM_BOOT_MALLOC_INIT;
    malloc_funcs.boot_malloc = ROM_BOOT_MALLOC;
    malloc_funcs.boot_free = ROM_BOOT_FREE;
    uapi_register_malloc(&malloc_funcs);
    boot_malloc_init((uintptr_t)g_st_softvalue.malloc_start_addr, (uintptr_t)g_st_softvalue.malloc_end_addr);
}
/* debug serial initialization */
EXT_PRV void fastboot_debug_uart_init(void)
{
    uart_param_stru default_uart_param = { 0 };
    default_uart_param.baudrate = 115200; /* 115200:baudrate */
    default_uart_param.databit = 8;       /* 8: databit */
    default_uart_param.stopbit = 0;       /* 0:stop_bit 1; 1:when databit is 5, stop_bit:1.5, else stop_bit:2 */
    default_uart_param.parity = 2;        /* 2: even */
    default_uart_param.flowctrl = 0;
    default_uart_param.flow_gpio = 0;
    default_uart_param.fifoline_full = DEFAULT_FIFOLINE_FULL;
    default_uart_param.fifoline_empty = DEFAULT_FIFOLINE_EMPTY;
#ifdef PRODUCT_CFG_PRINT_UART0
    uapi_io_set_func(9, 1);  /* id:9, val:1 */
    uapi_io_set_func(10, 3); /* id:10, val:3 */
    serial_init(UART0, default_uart_param);
#else

    serial_init(UART1, default_uart_param);

#endif
}
EXT_PRV void fastboot_flash_funcs_init(void)
{
    EXT_FLASH_CMD_FUNC_S flash_funcs;
    td_u32 ret;

    flash_funcs.init = uapi_flash_init;
    flash_funcs.read = uapi_flash_read;
    flash_funcs.write = uapi_flash_write;
    flash_funcs.erase = uapi_flash_erase;
    uapi_cmd_regist_flash_cmd(&flash_funcs);
    ret = uapi_flash_init();
    if (ret != EXT_ERR_SUCCESS) {
        return;
    }
}
EXT_PRV void start_fast_boot_print_msg(td_u32 ret, td_u32 ret1, td_u32 ret2)
{
    boot_msg1("\nflash-boot ", BOOT_VER);
    boot_msg0(PRODUCT_CFG_BUILD_DATA_TIME);
    boot_msg1("burn addr", g_st_softvalue.cmd_burn_default_flash_addr);

    if (ret) {
        boot_msg1("flash_init_fail ret=", ret);
    }

    if (ret1) {
        boot_msg1("flash_partition_init ret=", ret1);
    }

    if (ret2) {
        boot_msg1("factory_nv_read ret=", ret2);
    }
}
EXT_PRV void start_fast_boot_init(void)
{
    uapi_watchdog_disable();
    uapi_watchdog_enable(WDG_TIME_US);
#ifdef BOOT_FROM_FLASH
#if BOOT_FROM_FLASH
    /* time_cnt initialization ,  using for record start-up time */
    uapi_time_count_enable();
#endif
#endif

    /* initialize some changeable value in this function, because of flashboot in roms */
    soft_value_init();
    /* heap register and initialization */
    fast_boot_malloc_funcs_init();

    /* debug serial initialization */
    fastboot_debug_uart_init();

    fastboot_flash_funcs_init();
}

/* the entry of C. */
void start_fastboot(void)
{
    td_u32 ret;
    td_u32 ret1 = 0;
    td_u32 ret2 = 0;
    ext_flash_partition_table *flash_table = NULL;
    td_u32 reg_val;

    start_fast_boot_init();

    /* confiurate boot version */
    UAPI_REG_WRITE((EXT_SYSCTRL_REG_BASE + REG_BOOT_VER), BOOT_VER);

    /* nv initialization and refresh glabal variables */
    UAPI_REG_READ((EXT_SYSCTRL_REG_BASE + REG_FNV_ADDR), reg_val);
    reg_val &= ~MASK_FNV_ADDR;
    UAPI_REG_WRITE((EXT_SYSCTRL_REG_BASE + REG_FNV_ADDR), reg_val | (MASK_FNV_ADDR & FACTORY_NV_ADDR));
    ret = uapi_factory_nv_init(FACTORY_NV_ADDR);
    if (ret == EXT_ERR_SUCCESS) {
        flash_table = (ext_flash_partition_table *)boot_malloc(sizeof(ext_flash_partition_table));
        if (flash_table != NULL) {
            ret1 = uapi_flash_partition_init();
            ret2 = uapi_factory_nv_read(EXT_NV_FTM_FLASH_PARTIRION_TABLE_ID, flash_table,
                sizeof(ext_flash_partition_table));
            if (ret2 == EXT_ERR_SUCCESS &&
                uint_2_multiply(flash_table->table[EXT_FLASH_PARTITON_NV_BACKUP].addr_blk, BLOCK_SIZE, TD_NULL) &&
                uint_2_multiply(flash_table->table[EXT_FLASH_PARTITON_FACTORY_NV].addr_blk, BLOCK_SIZE, TD_NULL)) {
                g_st_softvalue.cmd_burn_default_flash_addr =
                    flash_table->table[EXT_FLASH_PARTITON_NV_BACKUP].addr_blk * BLOCK_SIZE;
                UAPI_REG_WRITE((EXT_SYSCTRL_REG_BASE + REG_FNV_ADDR), reg_val |
                    (MASK_FNV_ADDR & (flash_table->table[EXT_FLASH_PARTITON_FACTORY_NV].addr_blk * BLOCK_SIZE)));
            }
        }
        boot_free(flash_table);
    }

    /* to avoid affecting serial communication.  don't print data before serial connected safely */
    if (uart_auth_process(UART1, UART_FLASHBOOT_INTERRUPT_TIMEOUT) == TD_FALSE) {
        td_u32 temp;
        start_fast_boot_print_msg(ret, ret1, ret2);
        uapi_time_counter_getms(&temp);
        boot_msg1("boot used (ms) is ", temp);

#ifdef USE_FOTA
        (void)los_upgrade_app();
#endif
        /* only S-chip support flash protection.
         * E-chip need set by users according to specific model, because of replaceable flash */
        if (EXT_CHIP_TYPE_21SV == uapi_get_hw_chip_type()) {
            boot_msg0("Support Flash protection.");
        }

        boot_start_main();
        boot_firmware();
        while (1) { }
    }

    /* entering command mode, disable watch dog */
    uapi_watchdog_disable();
    cli_loop();
}
