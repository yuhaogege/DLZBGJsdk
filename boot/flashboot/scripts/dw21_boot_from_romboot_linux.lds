OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(Reset_Handler)


MEMORY
{
	/* RAM occupation distribution */
	TEMP(xrw): ORIGIN = 0x02000000,LENGTH = 92K         /* remain 92K, using for temporary or system startup loading ,0x17000 */
	HEAP(xrw): ORIGIN = 0x02017000,LENGTH = 136K        /* heap space when runing ,0x22000 */
	SRAM(xrw) : ORIGIN = 0x02039000,LENGTH = 16K        /*RAM usage for FLASH code section ,data section, BSS section when running, 0x4000*/
	STACK(xrw) : ORIGIN = 0x0203D000,LENGTH = 8K        /* stack space when runing,0x2000*/
	FIXED_ROM_SRAM(xrw): ORIGIN = 0x0203f000,LENGTH = 4K /*FIX_ROM bss and data ram ,0x1000*/

	/* FLASH occupied */
	FLASH(rx): ORIGIN = 0x01000c00,LENGTH = 16K			/* FLASH space occupied */
	FIXED_ROM_ADDR(rx): ORIGIN = 0x00010800,LENGTH = 30K/* share space occupied by ROM */
}

SECTIONS
{
      /* The startup code goes first into FLASH */
	.text.isr_vector :
	{
	. = ALIGN(4);
	KEEP(*(.text.isr_vector))
	. = ALIGN(4);
	KEEP(*(.text.Reset_Handler))
	} >FLASH

	.rom.text :
	{
	. = ALIGN(4);
	KEEP(SORT(librom.o)(.text*))
	KEEP(SORT(librom.o)(.rodata*))
	. = ALIGN(4);
	} >FIXED_ROM_ADDR

	.u_boot_cmd :
    {
	. = ALIGN(4);
	__rom_copy_start = LOADADDR(.u_boot_cmd); /*FLASH addr*/
	. = ALIGN(4);
	__rom_copy_ram_start = .;
        __u_boot_cmd_start = .;
        KEEP(SORT(*)(.u_boot_cmd*))
        __u_boot_cmd_end = .;

    }>FIXED_ROM_SRAM  AT>FIXED_ROM_ADDR

    .rom.data :
     {
	. = ALIGN(4);
	KEEP(SORT(librom.o) (.data*))
	. = ALIGN(4);
	__rom_copy_ram_end = .;
     }>FIXED_ROM_SRAM AT>FIXED_ROM_ADDR
	 __rom_copy_size = __rom_copy_ram_end - __rom_copy_ram_start;

        .rom.bss :
    {
		. = ALIGN(4);
		__rom_bss_start = .;
		KEEP(librom.o (.bss))
		KEEP(librom.o (.bss*))
		KEEP(librom.o (COMMON))
		. = ALIGN(4);
		__rom_bss_end = .;
    }>FIXED_ROM_SRAM AT>FIXED_ROM_ADDR

    .text :AT ((LOADADDR (.text.isr_vector) + SIZEOF (.text.isr_vector) + (4) - 1) & ~ ((4) - 1))
    {
        . = ALIGN(4);
	__ram_copy_start = LOADADDR(.text); /*FLASH addr*/
	. = ALIGN(4);
	__ram_copy_ram_start = .;
        *(.text)
        *(.text*)
        *(.rodata)
        *(.rodata*)
        KEEP(*(.rom_addition.text))
        . = ALIGN(4);
    }>SRAM

	.u_flash_boot_cmd : AT ((LOADADDR (.text) + SIZEOF (.text) + (4) - 1) & ~ ((4) - 1))
    {
	. = ALIGN(4);
        __u_flash_boot_cmd_start = .;
        KEEP(SORT(*) (.u_flash_boot_cmd*))
	. = ALIGN(4);
        __u_flash_boot_cmd_end = .;
    }>SRAM

    /* Initialised data */
    .data :  AT ((LOADADDR (.u_flash_boot_cmd) + SIZEOF (.u_flash_boot_cmd) + (4) - 1) & ~ ((4) - 1))
    {
        FILL(0xFF)
	. = ALIGN(4);
        __data_begin__ = .;
        __data_flash_begin__ = LOADADDR(.data);
        *(.data)
        *(.data*)
        . = ALIGN(4);
        __data_end__ = .;
	__ram_copy_ram_end = .;
    } >SRAM
    __ram_copy_size = __ram_copy_ram_end - __ram_copy_ram_start;

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
    } >SRAM
    __bss_size__ = __bss_end__ - __bss_begin__;

    /* Stack in SRAM at lowest addresses */
    .stacks (NOLOAD) :
    {
	. = ALIGN(4);
        __SYSTEM_STACK_BEGIN__ = ORIGIN(STACK);
        KEEP(*(.stacks))
	__SYSTEM_STACK_END__ = ORIGIN(STACK) + LENGTH(STACK);
    } >STACK
    __SYSTEM_STACK_SIZE__ = __SYSTEM_STACK_END__ - __SYSTEM_STACK_BEGIN__;


    .heap (NOLOAD) :
    {
        . = ALIGN(4);
        __HEAP_BEGIN__ = ORIGIN(HEAP);
	KEEP(*(.keap))
        __HEAP_END__ =  __HEAP_BEGIN__ + LENGTH(HEAP);
    } >HEAP

}
