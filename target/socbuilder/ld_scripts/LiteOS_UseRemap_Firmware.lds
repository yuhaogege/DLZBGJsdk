
OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(Reset_Handler)

/* 
  * 1.FLASH reserves 0x00006000 space for boot here, so ORIGIN = 0x00006000, which can be modified according to the actual situation;
  * 2.512KB RAM debug version logic:
  *   1) The base address of the DYNSECTOR dynamic zone is 0x20700000. If the dynamic zone is configured to the last 32KB, the corresponding value should be written as 0x2078000 (0x2080000-32K);
  *   2) Modify the LENGTH of PERMANENT to a suitable size. Here you can use ORIGIN (DYNSECTOR) -ORIGIN (PERMANENT);
  *   3) Modify the macro DYN_BASE_ADDR in remap_driver.h, the corresponding 512KB logic should be 0x2070000;
  *   Recompile bsp / drivers and link.
  */

ROM_TEXT_SIZE = 0x12044;
ROM_DATA_SIZE = 0xa4c;
ROM_BSS_SIZE = 0x420;

MEMORY
{
    FLASH(rx) : ORIGIN = 0x00007000,LENGTH = 300K
    ROM_TEXT(rx) : ORIGIN = 0x02200000,LENGTH = 80K
    RESETHANDLER(rx) : ORIGIN = 0x02000000,LENGTH = 1K
    ROM_DATA(rw) : ORIGIN = 0x02000400,LENGTH = 3K
    ROM_BSS(rw) : ORIGIN = 0x02001000,LENGTH = 0x420
    PERMANENT(rx) : ORIGIN = 0x02001420,LENGTH = 0x36be0
    DYNSECTOR(rx) : ORIGIN = 0x02038000, LENGTH = 0xc0000
}

ASSERT(LENGTH(DYNSECTOR)<0x100000, "Dyn section should < 1MB.")

SECTIONS
{
    .text1 :
    {
        __rom_text_begin__ = .;
        . += ROM_TEXT_SIZE;
        __rom_text_end__ = .;
    } > ROM_TEXT AT > FLASH
    __rom_text_size__ = __rom_text_end__ - __rom_text_begin__;

    .text2 :AT ((LOADADDR (.text1) + LENGTH(ROM_TEXT)))
    {
        __reset_handler_start__ = .;
        KEEP(*(.text.Reset_Handler))
		KEEP(SORT(liblitekernelram.o)(.text))

		__reset_handler_end__ = .;
    } > RESETHANDLER
	__reset_handler_size__ = __reset_handler_end__ - __reset_handler_start__;

    .text3 :AT ((LOADADDR (.text2) + LENGTH(RESETHANDLER)))
    {
        __rom_data_begin__ = .;
        . += ROM_DATA_SIZE;
        __rom_data_end__ = .;
    } > ROM_DATA
    __rom_data_size__ = __rom_data_end__ - __rom_data_begin__;

    .text4 (NOLOAD):AT ((LOADADDR (.text3) + LENGTH(ROM_DATA)))
    {
        . = ALIGN(4);
        __rom_bss_begin__ = .;
        . += ROM_BSS_SIZE;
        __rom_bss_end__ = .;
    } > ROM_BSS
    __rom_bss_size__ = __rom_bss_end__ - __rom_bss_begin__;

    /* exidx and extab are debugging information to allow the unwinding of the
     * stack. See the "backtrace" module. exidx appears to be generated even if we
     * don't ask for full unwinding information (with the -funwind-tables option to
     * the compiler), so was present before backtracing was added. */

    .ARM.exidx :AT ((LOADADDR (.text4) + LENGTH (ROM_BSS)))
    {
        __exidx_start = .;
        SORT(*)(.ARM.exidx*)
        __exidx_end = .;
        . = ALIGN(8);
    } > PERMANENT


    .ARM.extab :AT ((LOADADDR (.ARM.exidx) + SIZEOF (.ARM.exidx) + (8) - 1) & ~ ((8) - 1))
    {
        SORT(*)(.ARM.extab*)
        . = ALIGN(8);
    } > PERMANENT

    .text5 :AT ((LOADADDR (.ARM.extab) + SIZEOF (.ARM.extab) + (8) - 1) & ~ ((8) - 1))
    {
        liblitekernelram.o (.text*)
        libadc.o (.text*)
        libraw_timer.o(.text*)
        libflash_ram.o(.text*)
        libfpb.o (.text*)
        libio_config.o (.text*)
        liblowpower.o (.text*)
        libmac_common.o (.text*)
        libpartition_table.o (.text*)
        libremap_dll.o (.text*)
        libsys_common.o (.text*)
        libuart.o (.text*)
        SORT(*)(.isr.text)
        SORT(*)(.hot.text)
        SORT(*)(.rodata)
        SORT(*)(.rodata*)
        . = ALIGN(8);
		__permanent_end__ = .;
    } > PERMANENT
	__permanent_size__ = __permanent_end__ - __exidx_start;
	
    __RAM_BEGIN__  = ORIGIN(PERMANENT);
    __RAM_SIZE__ = LENGTH(PERMANENT);

    /* Initialised data */
    .data : AT ((LOADADDR (.text5) + SIZEOF (.text5) + (8) - 1) & ~ ((8) - 1))
    {

        __data_begin__ = .;
        . = ALIGN(4);
        INCLUDE system_config.ld
        . = ALIGN(4);
        SORT(*)(.data)
        SORT(*)(.data*)
        . = ALIGN(4);
        __data_end__ = .;
    } > PERMANENT
    __data_size__ = __data_end__ - __data_begin__;


    .dyn_loader0 :AT ((LOADADDR (.data) + SIZEOF (.data) + (4096) - 1) & ~ ((4096) - 1))
    {
        _dyn_vir_mem_start_ = .;
		SORT(*)(.init.text)
        SORT(*)(.text)
        SORT(*)(.text*)
		SORT(*)(.exit.text)
        SORT(*)(.dynamic1_loader*)
        _dyn_vir_mem_end_ = .;
    } > DYNSECTOR
	__dyn_size__ = _dyn_vir_mem_end_ - _dyn_vir_mem_start_;
	
    __DYN_START_FLASH__ = LOADADDR(.dyn_loader0) -  LENGTH(ROM_TEXT);

    /* Stack in SRAM at lowest addresses */
    .stacks (NOLOAD) :
    {
        __SYSTEM_STACK_BEGIN__ = .;
        KEEP(*(.stacks))
        __SYSTEM_STACK_END__ = .;
    } > PERMANENT
    __SYSTEM_STACK_SIZE__ = __SYSTEM_STACK_END__ - __SYSTEM_STACK_BEGIN__;


    /* Uninitialised data */
    .bss (NOLOAD) :
    {
        . = ALIGN(4);
        __bss_begin__ = .;
        SORT(*)(.bss)
        SORT(*)(.bss*)
        SORT(*)(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
    } > PERMANENT
    __bss_size__ = __bss_end__ - __bss_begin__;

    /* Like Uninitialised data, but we don't want to 0 it. Stores backtrace among other things. */
    .preserve (NOLOAD):
    {
        . = ALIGN(4);
        *(preserve)
    } > PERMANENT

    .heap (NOLOAD) :
    {
        . = ALIGN(64);
        __HEAP_BEGIN__ = ABSOLUTE(.);
    } > PERMANENT

    __HEAP_SIZE__  = (__RAM_BEGIN__ + __RAM_SIZE__) - __HEAP_BEGIN__ - 1;
    . = ALIGN(4);
    end = . ;

}
