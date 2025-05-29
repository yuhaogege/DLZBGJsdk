    .syntax unified
    .arch armv7-m
    .cpu cortex-m3
    .thumb


.global Reset_Handler

.extern __bss_begin__
.extern __bss_end__
.extern __bss_size__
.extern __data_begin__
.extern __SYSTEM_STACK_BEGIN__
.extern __SYSTEM_STACK_END__

#ifdef LIB_CONFIGURABLE
.extern __rom_bss_begin__
.extern __rom_bss_end__
.extern __rom_bss_size__
#endif
.extern main
 .equ SHCSR,              0xE000ED24
 .equ BUSFAULTSET,        0x00020000
 .equ SCBCCR,             0xE000ED14
 .equ DIV0TRPMSK,         0x00000010
/******************************************************************************
*
* The stack for MSP of Cortex M3.
*
*******************************************************************************/
    .section .stacks,"aw",%progbits
    .set    STACK_SIZE_SYSTEM, 0x00000900   /* Set the stack size for MSP */
    .global stack_system
    .type   stack_system, %object
    .align    3
stack_system_end:
    .space  STACK_SIZE_SYSTEM
    .size   stack_system_end, . - stack_system_end
stack_system:
    .size   stack_system, . - stack_system

/******************************************************************************
*
* The entry Reset_Handler
*
*******************************************************************************/
    .section        .text.Reset_Handler,"ax",%progbits
    .weak           Reset_Handler
    .type           Reset_Handler, %function
    .func           Reset_Handler
    .align            2
    .thumb

Reset_Handler:
    cpsid   i

    /* Make sure SP is really at the start of the stack */
    ldr     r0, =stack_system
    msr     msp, r0

    /* Prefill the system stack with 0xefbeadde (or deafbeef in little endian) */
    ldr     r1, =__SYSTEM_STACK_BEGIN__
    ldr     r3, =__SYSTEM_STACK_END__
stack_fill:
    subs    r3, r3, r1
    beq     stack_fill_loop_end
    ldr     r2, =0xefbeadde
stack_fill_loop:
    str     r2, [r1, #0]           /* Store the quad octet initialisation value in r2 into address in r1 */
    adds    r1, r1, #4             /* Increased address in r1 by a quad octet */
     subs    r3, r3, #4             /* Decrease the number of bytes to do by a quad octet */
    bgt     stack_fill_loop        /* Keep going until it is all done */
stack_fill_loop_end:

    /* Clear bss section */
    ldr     r1, =__bss_begin__
    ldr     r3, =__bss_end__
    subs    r3, r3, r1

    beq     end_clear_bss_loop
    ldr     r2, =#00000000

clear_bss_loop:
    str     r2, [r1, #0]           /* Store the octet initialisation value in r2 into address in r1 */
    adds    r1, r1, #4             /* Increased address in r1 by an octet */
    subs    r3, r3, #4             /* Decrease the number of bytes to do by an octet */
    bgt     clear_bss_loop         /* Keep going until it is all done */
end_clear_bss_loop:
#ifdef LIB_CONFIGURABLE
    /* Clear rom bss section */
    ldr     r1, =__rom_bss_begin__
    ldr     r3, =__rom_bss_end__
    subs    r3, r3, r1

    beq     end_clear_rom_bss_loop
    ldr     r2, =#00000000

clear_rom_bss_loop:
    str     r2, [r1, #0]           /* Store the octet initialisation value in r2 into address in r1 */
    adds    r1, r1, #4             /* Increased address in r1 by an octet */
    subs    r3, r3, #4             /* Decrease the number of bytes to do by an octet */
    bgt     clear_rom_bss_loop         /* Keep going until it is all done */
end_clear_rom_bss_loop:
#endif

#ifdef USE_REMAP
/*enable busfault int*/
    ldr r4, =SHCSR
    ldr r5, =BUSFAULTSET
    str r5, [r4]
#endif

/*enable div 0 trigger*/
    ldr r4, =SCBCCR
    ldr r5, =DIV0TRPMSK
    ldr r3, [r4]
    orr r3, r3, r5
    str r3, [r4]

    cpsie     i
    ldr     pc, =main

    .size  Reset_Handler, .-Reset_Handler
    .endfunc

