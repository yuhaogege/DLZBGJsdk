
OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(Reset_Handler)

MEMORY
{
    RAM(xrw) : ORIGIN = 0x02000000,LENGTH = 256K
}

SECTIONS
{
    .text :
    {
        . = ALIGN(4);
        __text_begin__ = .;
        KEEP(*(.text.env))
        KEEP(*(.text.Reset_Handler))
        *(.text)
        *(.text*)
        liblitekernelram.o(.text*)
        libphyrom.o(.text*)
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
        __text_end__ = .;
    } > RAM

    /* exidx and extab are debugging information to allow the unwinding of the
     * stack. See the "backtrace" module. exidx appears to be generated even if we
     * don't ask for full unwinding information (with the -funwind-tables option to
     * the compiler), so was present before backtracing was added. */
    .ARM.exidx :
    {
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
    } > RAM

    .ARM.extab :
    {
        *(.ARM.extab*)
    } > RAM

    __RAM_BEGIN__ = ORIGIN(RAM);
    __RAM_SIZE__ = LENGTH(RAM);

    /* Stack in SRAM at lowest addresses */
    .stacks (NOLOAD) :
    {
        __SYSTEM_STACK_BEGIN__ = .;
        KEEP(*(.stacks))
        __SYSTEM_STACK_END__ = .;
    } > RAM
    __SYSTEM_STACK_SIZE__ = __SYSTEM_STACK_END__ - __SYSTEM_STACK_BEGIN__;


    /* Initialised data */
    .data :
    {
        . = ALIGN(256);
        __data_load__ = LOADADDR(.data);
        __data_begin__ = .;
        KEEP(*(.data.vector))
        *(.data)
        *(.data*)
		libphyrom.o(.data*)
        . = ALIGN(4);
        INCLUDE system_config.ld
        . = ALIGN(4);

    } > RAM
    
    .qShellFunTab :
    {
        __qShellFunTab_start = .;
        *(.qShellFunTab)
        __qShellFunTab_end = .;
        . = ALIGN(4);
    } > RAM
    
    .qShellVarTab :
    {
        __qShellVarTab_start = .;
        *(.qShellVarTab)
        __qShellVarTab_end = .;
        . = ALIGN(4);
        __data_end__ = .;
    } >RAM
    
    __data_size__ = __data_end__ - __data_begin__;


    /* Uninitialised data */
    .bss (NOLOAD) :
    {
        . = ALIGN(4);
        __bss_begin__ = .;
        *(.bss)
        *(.bss*)
		libphyrom.o(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
    } > RAM
    __bss_size__ = __bss_end__ - __bss_begin__;

    /* Like Uninitialised data, but we don't want to 0 it. Stores backtrace among other things. */
    .preserve (NOLOAD):
    {
        . = ALIGN(4);
        *(preserve)
    } > RAM
     
    .heap (NOLOAD) :
    {
        . = ALIGN(64);
        __HEAP_BEGIN__ = ABSOLUTE(.);
    } > RAM

    __HEAP_SIZE__  = (__RAM_BEGIN__ + __RAM_SIZE__) - __HEAP_BEGIN__;
    . = ALIGN(4);
    end = . ;
	
}