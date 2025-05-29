
OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(Reset_Handler)

MEMORY
{
	FLASH(rx) : ORIGIN = 0x00006000,LENGTH = 300K
	PERMANENT(rx) : ORIGIN = 0x02000000,LENGTH = 480K
	DYNSECTOR(rx) : ORIGIN = 0x02078000, LENGTH = 288K
}

SECTIONS
{
    .text1 :
    {
        __text_begin__ = .;
        KEEP(*(.text.Reset_Handler))
        liblitekernel.o (.text*)
        libsec.o (.text*)
		. = ALIGN(8);
    } > PERMANENT AT > FLASH
	

    .text2 :AT ((LOADADDR (.text1) + SIZEOF (.text1) + (8) - 1) & ~ ((8) - 1))
    {
        libc.o (.text*)
        libgcc.a (.text*)
        . = ALIGN(8);
    } > PERMANENT
	
 
    .text3 :AT ((LOADADDR (.text2) + SIZEOF (.text2) + (8) - 1) & ~ ((8) - 1))
    {

        libdrv.o (.text*)
		KEEP(libdrv_in_rom.o (.text*))
		KEEP(*(.bsp.rom.text))
		KEEP(*(.bsp.rom.rodata))
		*(.isr.text)
		*(.hot.text)
		libosa.o (.text*)
		libphyram.o (.text*)
		libphyrom.o (.text*)
		libcommon.o (.text*)
        . = ALIGN(8);	
    } > PERMANENT
	

    .text4 :AT ((LOADADDR (.text3) + SIZEOF (.text3) + (8) - 1) & ~ ((8) - 1))
    {
        *(.rodata)
        *(.rodata*)
		    . = ALIGN(8);	
    } > PERMANENT


    /* exidx and extab are debugging information to allow the unwinding of the
     * stack. See the "backtrace" module. exidx appears to be generated even if we
     * don't ask for full unwinding information (with the -funwind-tables option to
     * the compiler), so was present before backtracing was added. */
	 

    .ARM.exidx :AT ((LOADADDR (.text4) + SIZEOF (.text4) + (8) - 1) & ~ ((8) - 1))
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


    __text_end__ = .;
    __RAM_BEGIN__  = ORIGIN(PERMANENT);
    __RAM_SIZE__ = LENGTH(PERMANENT);

    /* Initialised data */
    .data : AT ((LOADADDR (.ARM.extab) + SIZEOF (.ARM.extab) + (256) - 1) & ~ ((256) - 1))
    {

        __data_begin__ = .;
        KEEP(*(.data.vector))
        *(.data)
        *(.data*)
		KEEP(*(.bsp.rom.data))
        . = ALIGN(4);
        INCLUDE system_config.ld
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
