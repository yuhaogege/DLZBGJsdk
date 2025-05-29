    .syntax unified
    .cpu cortex-m3
    .thumb
#include <dw21_platform.h>
#include <soc_flash_boot.h>
    
.globl Reset_Handler

.extern __bss_begin__
.extern __bss_end__
.extern __bss_size__
.extern __data_begin__
.extern __data_end__
.extern __data_size__
.extern __SYSTEM_STACK_BEGIN__
.extern __SYSTEM_STACK_END__
.extern __SYSTEM_STACK_SIZE__

.extern start_fastboot

/******************************************************************************
*
* The heap for SYSTEM.
*
*******************************************************************************/
    .section .heap,"aw",%progbits
/******************************************************************************
*
* The stack for MSP of Cortex M3.
*
*******************************************************************************/
    .section .stacks,"aw",%progbits
    .set    STACK_SIZE_SYSTEM, 0x00002000
    .align  2
    .global stack_system
    .type   stack_system, %object
    .size   stack_system, STACK_SIZE_SYSTEM
stack_system_end:
    .space  STACK_SIZE_SYSTEM-4
stack_system:
    .space  4

/******************************************************************************
*
* The minimal vector table for a Cortex M3. Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
*
*******************************************************************************/
    .section  .text.isr_vector,"a",%progbits
    .type  g_pfnVectors, %object
    .size  g_pfnVectors, .-g_pfnVectors
g_pfnVectors:
    .word  stack_system
    .word  Reset_Handler
    .word  hard_reset
    .word  hard_reset
    .word  hard_reset
    .word  hard_reset
    .word  hard_reset
    .word  hard_reset
    .word  hard_reset
    .word  hard_reset
    .word  hard_reset
    .word  hard_reset
    .word  hard_reset
    .word  hard_reset
    .word  hard_reset
    .word  hard_reset


/******************************************************************************
*
* The entry Reset_Handler
*
*******************************************************************************/
    .section        .text.Reset_Handler,"ax",%progbits
    .weak           Reset_Handler
    .type           Reset_Handler, %function
    .thumb
    .func           Reset_Handler

Reset_Handler:
    /* Make sure SP is really at the start of the stack */
    ldr     r0, =stack_system
    msr     msp, r0

	ldr 	r0,=0x4003b05c
	mov 	r1, #1
	str 	r1, [r0]
#if BOOT_FROM_FLASH
	/* config wdg phase1,10.74s based on 12.5M,1.34s based on 100M*/
	ldr 	r0, =0x40030000
	ldr 	r1, [r0]
	ldr     r2, =0x00800000
	orr 	r1, r1,r2
	str 	r1, [r0]

	ldr 	r0, =0x4003b028
	ldr 	r1, [r0]
	orr 	r1, r1,#0x1
	str 	r1, [r0]

	ldr 	r0, =0x40031000
	ldr 	r1, =0xB
	str 	r1, [r0, #4]
	ldr 	r1, =0x1D
	str 	r1, [r0, #0]
	ldr 	r1, =0x76
	str 	r1, [r0, #0xC]

	/* LDO_config enable 0x40034104 = 3 */
	ldr 	r0,=0x40034104
	mov 	r1, #3
	str 	r1, [r0]
	
	/*wait 100us for LDO steady, 25M calculate clock */
	ldr r0,=0x384/* (25/3) * 100us = 900*/
loop1:
	subs r0,r0,#1
	bne loop1
	
	/* enable pll; configurate the frequency division coefficient of frequency doubling */
	ldr 	r0,=0x4003b000
	ldr 	r1, =0x50f0   
	str 	r1, [r0,#0x8]
wait_for_pll_lock:
	ldr     r1,[r0]
	and     r2,r1,#0x1 
	cmp     r2,#0x1
	bne     wait_for_pll_lock
	
	/*slow to normal*/
	/*0x40030000[2:0] = 4*/
	ldr 	r0,=0x40030000
	ldr 	r1,[r0]
	bic 	r1,r1,#0x7
	orr 	r1,r1,#0x4
	str 	r1,[r0]
wait_for_normal:
    ldr     r1,[r0]
    and     r2,r1,#0x78 
    cmp     r2,#0x20
    bne     wait_for_normal
#endif

  /* advise: do not set osc here */
	/* start osc for 32.768k  freqency adjust*/
	ldr 	r0,=0x4003d300
	ldr 	r1, =0x1a802fb3
	str 	r1, [r0]

	/*invalided power down*/
	ldr 	r0,=0x4003d304
	ldr 	r1, =0x299
	str 	r1, [r0]
	
	ldr 	r0,=0x4003d300
	ldr 	r1, =0x1a802fb8
	str 	r1, [r0]

    /* Prefill the system stack with 0xefbeadde (or deadbeef in little endian) */
        /* start and ending address must make 4B aligned */
    ldr     r1, =__SYSTEM_STACK_BEGIN__
    ldr     r3, =__SYSTEM_STACK_END__
stack_fill:
    subs    r3, r3, r1
    beq     stack_fill_loop_end
    ldr     r2, =0xdeadbeef
stack_fill_loop:
    str     r2, [r1, #0]           /* Store the quad octet initialisation value in r2 into address in r1 */
    adds    r1, r1, #4             /* Increased address in r1 by a quad octet */
    subs    r3, r3, #4             /* Decrease the number of bytes to do by a quad octet */
    bgt     stack_fill_loop        /* Keep going until it is all done */
stack_fill_loop_end:

    /* Clear bss section */
        /* start and ending address must make 4B aligned */
    ldr     r1, =__bss_begin__
    ldr     r3, =__bss_end__
    cmp     r1,r3
    beq     end_clear_bss_loop

    ldr     r2, =#00000000
clear_bss_loop:
    str     r2, [r1, #0]          /* Store the  initialisation value in r2 into address in r1 */
    adds    r1, r1, #4             /* Increased address in r1 by one word */
    cmp     r1, r3         
    bne     clear_bss_loop
    str     r2, [r1, #0]
end_clear_bss_loop:


    /* Clear rom bss section */
    ldr     r1, =__rom_bss_start
    ldr     r3, =__rom_bss_end
    cmp     r1,r3
    beq     end_clear_rom_bss_loop

    ldr     r2, =#00000000
clear_rom_bss_loop:
    str     r2, [r1, #0]          /* Store the  initialisation value in r2 into address in r1 */
    adds    r1, r1, #4             /* Increased address in r1 by one word */
    cmp     r1, r3         
    bne     clear_rom_bss_loop
    str     r2, [r1, #0]
end_clear_rom_bss_loop:

/* copy rom .data section  from  FLASH to SRAM*/
    ldr     r1, =__rom_copy_ram_start     /*SRAM addr*/
    ldr     r2, =__rom_copy_start    /*FLASH addr*/
    ldr     r3, =__rom_copy_size
    cmp     r3, #0
    beq     end_rom_data_loop
start_rom_data_loop:
    ldr     r4, [r2]           /* Load the  quadoctet value into r4 from address in r2 */
    str     r4, [r1]           /* Store the quad octet value in r4 to address in r1 */
    adds    r2, r2, #4             /* Move onto next quad octet */
    adds    r1, r1, #4
    subs    r3, r3, #4             /* Decrease the number of bytes to do by a quad octet */
    cmp     r3, 0
    bne     start_rom_data_loop          /* Keep going until it is all done */
end_rom_data_loop:

    /* copy  .data section  from  FLASH to SRAM*/
    ldr     r1, =__ram_copy_ram_start     /*SRAM addr*/
    ldr     r2, =__ram_copy_start    /*FLASH addr*/
    ldr     r3, =__ram_copy_size
    cmp     r3, #0
    beq     end_set_ram_loop

set_ram_loop:
    ldr     r4, [r2]           /* Load the  quadoctet value into r4 from address in r2 */
    str     r4, [r1]           /* Store the quad octet value in r4 to address in r1 */
    adds    r2, r2, #4             /* Move onto next quad octet */
    adds    r1, r1, #4
    subs    r3, r3, #4             /* Decrease the number of bytes to do by a quad octet */
    cmp     r3, 0
    bne     set_ram_loop          /* Keep going until it is all done */
end_set_ram_loop:

    bl      start_fastboot

    .size  Reset_Handler, .-Reset_Handler
    .endfunc

/******************************************************************************
*
* The entry Reset_Handler
*
*******************************************************************************/
    .section        .text.hard_reset,"ax",%progbits
    .weak           hard_reset
    .type           hard_reset, %function
    .thumb
    .func           hard_reset
hard_reset:
    ldr     r2, =(EXT_SYSCTRL_REG_BASE + REG_SC_SYSRES)
    mov     r1, #1
    str     r1, [r2]

    .size  hard_reset, .-hard_reset
    .endfunc

