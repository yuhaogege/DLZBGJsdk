
OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(Reset_Handler)

MEMORY
{
    FLASH(rx) : ORIGIN = 0x00006000,LENGTH = 300K
    RESETHANDLER(rx) : ORIGIN = 0x02000000,LENGTH = 1K
    ROM_TEXT(rx) : ORIGIN = 0x02000400,LENGTH = 80K
    ROM_DATA(rw) : ORIGIN = 0x02014400,LENGTH = 3K
    ROM_BSS(rw) : ORIGIN = 0x02015000,LENGTH = 2K
    PERMANENT(rx) : ORIGIN = 0x02015800,LENGTH = 394K
    DYNSECTOR(rx) : ORIGIN = 0x02078000, LENGTH = 288K
}

SECTIONS
{
    .text1 :
    {
        __text_begin__ = .;
        KEEP(*(.text.Reset_Handler))
    } > RESETHANDLER AT > FLASH

    .text2 :AT (LOADADDR (.text1) + LENGTH (RESETHANDLER))
    {
        KEEP(SORT(*)(.text.kernel.dispatch))
        KEEP(SORT(*)(.text.kernel.rom))
        KEEP(SORT(liblitekernel.o)(.rodata*))
        KEEP(SORT(libgcc.o)(.text*))
        KEEP(SORT(*)(.text.libc.rom))
        KEEP(SORT(*)(.text.libsec.rom))
        KEEP(SORT(libphyrom.o)(.text*))
        KEEP(SORT(libdrv_in_rom.o)(.text*))
        KEEP(SORT(*)(.bsp.rom.text))
        KEEP(SORT(*)(.bsp.rom.rodata))
        KEEP(SORT(libosa.o)(.text*))
        KEEP(SORT(libalg.o)(.text*))
    } > ROM_TEXT

    .text3 :AT ((LOADADDR (.text2) + LENGTH(ROM_TEXT)))
    {
        KEEP(SORT(*)(.data.vector))
        KEEP(SORT(*)(.data.kernel))
        KEEP(SORT(*)(.kernel.rodata))
        KEEP(SORT(libc.o)(.data*))
        KEEP(SORT(libsec.o)(.data*))
        KEEP(SORT(libc.o)(.rodata*))
        KEEP(SORT(libsec.o)(.rodata*))
        KEEP(SORT(libgcc.o)(.rodata*))
        KEEP(SORT(libphyrom.o)(.data*))
        KEEP(SORT(libdrv_in_rom.o)(.data*))
        KEEP(SORT(*)(.bsp.rom.data))
        KEEP(SORT(libosa.o)(.data*))
        KEEP(SORT(libalg.o)(.data*))
        KEEP(SORT(libphyrom.o)(.rodata*))
        KEEP(SORT(libdrv_in_rom.o)(.rodata*))
        KEEP(SORT(libosa.o)(.rodata*))
        KEEP(SORT(libalg.o)(.rodata*))
    } > ROM_DATA

    .text4 :AT ((LOADADDR (.text3) + LENGTH(ROM_DATA)))
    {
        . = ALIGN(4);
        __rom_bss_begin__ = .;
        KEEP(SORT(*)(.bss.kernel))
        KEEP(SORT(*)(.COMMON.kernel))
        KEEP(SORT(libc.o)(.bss*))
        KEEP(SORT(libsec.o)(.bss*))
        KEEP(SORT(libc.o)(COMMON))
        KEEP(SORT(libsec.o)(COMMON))
        KEEP(SORT(libphyrom.o)(.bss*))
        KEEP(SORT(libphyrom.o)(COMMON))
        KEEP(SORT(libdrv_in_rom.o)(.bss*))
        KEEP(SORT(libdrv_in_rom.o)(COMMON))
        KEEP(SORT(libosa.o)(.bss*))
        KEEP(SORT(libalg.o)(.bss*))
        KEEP(SORT(libosa.o)(COMMON))
        KEEP(SORT(libalg.o)(COMMON))
        . = ALIGN(4);
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
        *(.ARM.exidx*)
        __exidx_end = .;
        . = ALIGN(8);
    } > PERMANENT


    .ARM.extab :AT ((LOADADDR (.ARM.exidx) + SIZEOF (.ARM.exidx) + (8) - 1) & ~ ((8) - 1))
    {
        *(.ARM.extab*)
        . = ALIGN(256);
    } > PERMANENT

    .text5 :AT ((LOADADDR (.ARM.extab) + SIZEOF (.ARM.extab) + (256) - 1) & ~ ((256) - 1))
    {
        liblitekernelram.o (.text*)
        libdrv.o (.text*)
        *(.isr.text)
        *(.hot.text)
        libphyram.o (.text*)
        libcommon.o (.text*)
        *(.rodata)
        *(.rodata*)
        . = ALIGN(8);
    } > PERMANENT

    __text_end__ = .;
    __RAM_BEGIN__  = ORIGIN(PERMANENT);
    __RAM_SIZE__ = LENGTH(PERMANENT);

    /* Initialised data */
    .data : AT ((LOADADDR (.text5) + SIZEOF (.text5) + (8) - 1) & ~ ((8) - 1))
    {

        __data_begin__ = .;
        . = ALIGN(4);
        INCLUDE system_config.ld
        . = ALIGN(4);
        *(.data)
        *(.data*)
        . = ALIGN(4);
        __data_end__ = .;
    } > PERMANENT

    __data_size__ = __data_end__ - __data_begin__;


    .dyn_loader0 :AT ((LOADADDR (.data) + SIZEOF (.data) + (4096) - 1) & ~ ((4096) - 1))
    {
        _dyn_vir_mem_start_ = .;
        *(.text)
        *(.text*)
        *(.dynamic1_loader*)
        . = ALIGN(4096);
        _dyn_vir_mem_end_ = .;
    } > DYNSECTOR

    __DYN_START_FLASH__ = LOADADDR(.dyn_loader0);

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
        *(.bss)
        *(.bss*)
        *(COMMON)
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
