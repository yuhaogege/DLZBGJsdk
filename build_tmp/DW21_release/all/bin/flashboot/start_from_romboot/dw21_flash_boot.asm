
/home/code/output/DW21_release/mk_image/image/sdk/build_tmp/DW21_release/all/bin/flashboot/start_from_romboot/dw21_flash_boot.elf:     file format elf32-littlearm


Disassembly of section .text.isr_vector:

01000c00 <g_pfnVectors>:
 1000c00:	fc ef 03 02 41 0c 00 01 01 90 03 02 01 90 03 02     ....A...........
 1000c10:	01 90 03 02 01 90 03 02 01 90 03 02 01 90 03 02     ................
 1000c20:	01 90 03 02 01 90 03 02 01 90 03 02 01 90 03 02     ................
 1000c30:	01 90 03 02 01 90 03 02 01 90 03 02 01 90 03 02     ................

01000c40 <Reset_Handler>:
 1000c40:	4824      	ldr	r0, [pc, #144]	; (1000cd4 <end_set_ram_loop+0x4>)
 1000c42:	f380 8808 	msr	MSP, r0
 1000c46:	4824      	ldr	r0, [pc, #144]	; (1000cd8 <end_set_ram_loop+0x8>)
 1000c48:	f04f 0101 	mov.w	r1, #1
 1000c4c:	6001      	str	r1, [r0, #0]
 1000c4e:	4823      	ldr	r0, [pc, #140]	; (1000cdc <end_set_ram_loop+0xc>)
 1000c50:	4923      	ldr	r1, [pc, #140]	; (1000ce0 <end_set_ram_loop+0x10>)
 1000c52:	6001      	str	r1, [r0, #0]
 1000c54:	4823      	ldr	r0, [pc, #140]	; (1000ce4 <end_set_ram_loop+0x14>)
 1000c56:	f240 2199 	movw	r1, #665	; 0x299
 1000c5a:	6001      	str	r1, [r0, #0]
 1000c5c:	481f      	ldr	r0, [pc, #124]	; (1000cdc <end_set_ram_loop+0xc>)
 1000c5e:	4922      	ldr	r1, [pc, #136]	; (1000ce8 <end_set_ram_loop+0x18>)
 1000c60:	6001      	str	r1, [r0, #0]
 1000c62:	4922      	ldr	r1, [pc, #136]	; (1000cec <end_set_ram_loop+0x1c>)
 1000c64:	4b22      	ldr	r3, [pc, #136]	; (1000cf0 <end_set_ram_loop+0x20>)

01000c66 <stack_fill>:
 1000c66:	1a5b      	subs	r3, r3, r1
 1000c68:	d004      	beq.n	1000c74 <stack_fill_loop_end>
 1000c6a:	4a22      	ldr	r2, [pc, #136]	; (1000cf4 <end_set_ram_loop+0x24>)

01000c6c <stack_fill_loop>:
 1000c6c:	600a      	str	r2, [r1, #0]
 1000c6e:	3104      	adds	r1, #4
 1000c70:	3b04      	subs	r3, #4
 1000c72:	dcfb      	bgt.n	1000c6c <stack_fill_loop>

01000c74 <stack_fill_loop_end>:
 1000c74:	4920      	ldr	r1, [pc, #128]	; (1000cf8 <end_set_ram_loop+0x28>)
 1000c76:	4b21      	ldr	r3, [pc, #132]	; (1000cfc <end_set_ram_loop+0x2c>)
 1000c78:	4299      	cmp	r1, r3
 1000c7a:	d006      	beq.n	1000c8a <end_clear_bss_loop>
 1000c7c:	f04f 0200 	mov.w	r2, #0

01000c80 <clear_bss_loop>:
 1000c80:	600a      	str	r2, [r1, #0]
 1000c82:	3104      	adds	r1, #4
 1000c84:	4299      	cmp	r1, r3
 1000c86:	d1fb      	bne.n	1000c80 <clear_bss_loop>
 1000c88:	600a      	str	r2, [r1, #0]

01000c8a <end_clear_bss_loop>:
 1000c8a:	491d      	ldr	r1, [pc, #116]	; (1000d00 <end_set_ram_loop+0x30>)
 1000c8c:	4b1d      	ldr	r3, [pc, #116]	; (1000d04 <end_set_ram_loop+0x34>)
 1000c8e:	4299      	cmp	r1, r3
 1000c90:	d006      	beq.n	1000ca0 <end_clear_rom_bss_loop>
 1000c92:	f04f 0200 	mov.w	r2, #0

01000c96 <clear_rom_bss_loop>:
 1000c96:	600a      	str	r2, [r1, #0]
 1000c98:	3104      	adds	r1, #4
 1000c9a:	4299      	cmp	r1, r3
 1000c9c:	d1fb      	bne.n	1000c96 <clear_rom_bss_loop>
 1000c9e:	600a      	str	r2, [r1, #0]

01000ca0 <end_clear_rom_bss_loop>:
 1000ca0:	4919      	ldr	r1, [pc, #100]	; (1000d08 <end_set_ram_loop+0x38>)
 1000ca2:	4a1a      	ldr	r2, [pc, #104]	; (1000d0c <end_set_ram_loop+0x3c>)
 1000ca4:	4b1a      	ldr	r3, [pc, #104]	; (1000d10 <end_set_ram_loop+0x40>)
 1000ca6:	2b00      	cmp	r3, #0
 1000ca8:	d006      	beq.n	1000cb8 <end_rom_data_loop>

01000caa <start_rom_data_loop>:
 1000caa:	6814      	ldr	r4, [r2, #0]
 1000cac:	600c      	str	r4, [r1, #0]
 1000cae:	3204      	adds	r2, #4
 1000cb0:	3104      	adds	r1, #4
 1000cb2:	3b04      	subs	r3, #4
 1000cb4:	2b00      	cmp	r3, #0
 1000cb6:	d1f8      	bne.n	1000caa <start_rom_data_loop>

01000cb8 <end_rom_data_loop>:
 1000cb8:	4916      	ldr	r1, [pc, #88]	; (1000d14 <end_set_ram_loop+0x44>)
 1000cba:	4a17      	ldr	r2, [pc, #92]	; (1000d18 <end_set_ram_loop+0x48>)
 1000cbc:	4b17      	ldr	r3, [pc, #92]	; (1000d1c <end_set_ram_loop+0x4c>)
 1000cbe:	2b00      	cmp	r3, #0
 1000cc0:	d006      	beq.n	1000cd0 <end_set_ram_loop>

01000cc2 <set_ram_loop>:
 1000cc2:	6814      	ldr	r4, [r2, #0]
 1000cc4:	600c      	str	r4, [r1, #0]
 1000cc6:	3204      	adds	r2, #4
 1000cc8:	3104      	adds	r1, #4
 1000cca:	3b04      	subs	r3, #4
 1000ccc:	2b00      	cmp	r3, #0
 1000cce:	d1f8      	bne.n	1000cc2 <set_ram_loop>

01000cd0 <end_set_ram_loop>:
 1000cd0:	f000 f826 	bl	1000d20 <__start_fastboot_veneer>
 1000cd4:	0203effc 	.word	0x0203effc
 1000cd8:	4003b05c 	.word	0x4003b05c
 1000cdc:	4003d300 	.word	0x4003d300
 1000ce0:	1a802fb3 	.word	0x1a802fb3
 1000ce4:	4003d304 	.word	0x4003d304
 1000ce8:	1a802fb8 	.word	0x1a802fb8
 1000cec:	0203d000 	.word	0x0203d000
 1000cf0:	0203f000 	.word	0x0203f000
 1000cf4:	deadbeef 	.word	0xdeadbeef
 1000cf8:	0203bf08 	.word	0x0203bf08
 1000cfc:	0203c5bc 	.word	0x0203c5bc
 1000d00:	0203f224 	.word	0x0203f224
 1000d04:	0203f828 	.word	0x0203f828
 1000d08:	0203f000 	.word	0x0203f000
 1000d0c:	00017d7c 	.word	0x00017d7c
 1000d10:	00000224 	.word	0x00000224
 1000d14:	02039000 	.word	0x02039000
 1000d18:	01000d28 	.word	0x01000d28
 1000d1c:	00002f08 	.word	0x00002f08

01000d20 <__start_fastboot_veneer>:
 1000d20:	f85f f000 	ldr.w	pc, [pc]	; 1000d24 <__start_fastboot_veneer+0x4>
 1000d24:	02039205 	.word	0x02039205

Disassembly of section .text:

02039000 <hard_reset>:
 2039000:	4a01      	ldr	r2, [pc, #4]	; (2039008 <hard_reset+0x8>)
 2039002:	f04f 0101 	mov.w	r1, #1
 2039006:	6011      	str	r1, [r2, #0]
 2039008:	40030004 	.word	0x40030004

0203900c <start_fast_boot_print_msg>:
 203900c:	b570      	push	{r4, r5, r6, lr}
 203900e:	4606      	mov	r6, r0
 2039010:	460d      	mov	r5, r1
 2039012:	4812      	ldr	r0, [pc, #72]	; (203905c <start_fast_boot_print_msg+0x50>)
 2039014:	4912      	ldr	r1, [pc, #72]	; (2039060 <start_fast_boot_print_msg+0x54>)
 2039016:	4614      	mov	r4, r2
 2039018:	f002 feea 	bl	203bdf0 <__boot_msg1_veneer>
 203901c:	4811      	ldr	r0, [pc, #68]	; (2039064 <start_fast_boot_print_msg+0x58>)
 203901e:	f002 fe6f 	bl	203bd00 <__boot_msg0_veneer>
 2039022:	4b11      	ldr	r3, [pc, #68]	; (2039068 <start_fast_boot_print_msg+0x5c>)
 2039024:	4811      	ldr	r0, [pc, #68]	; (203906c <start_fast_boot_print_msg+0x60>)
 2039026:	6859      	ldr	r1, [r3, #4]
 2039028:	f002 fee2 	bl	203bdf0 <__boot_msg1_veneer>
 203902c:	b976      	cbnz	r6, 203904c <start_fast_boot_print_msg+0x40>
 203902e:	b90d      	cbnz	r5, 2039034 <start_fast_boot_print_msg+0x28>
 2039030:	b934      	cbnz	r4, 2039040 <start_fast_boot_print_msg+0x34>
 2039032:	bd70      	pop	{r4, r5, r6, pc}
 2039034:	4629      	mov	r1, r5
 2039036:	480e      	ldr	r0, [pc, #56]	; (2039070 <start_fast_boot_print_msg+0x64>)
 2039038:	f002 feda 	bl	203bdf0 <__boot_msg1_veneer>
 203903c:	2c00      	cmp	r4, #0
 203903e:	d0f8      	beq.n	2039032 <start_fast_boot_print_msg+0x26>
 2039040:	4621      	mov	r1, r4
 2039042:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
 2039046:	480b      	ldr	r0, [pc, #44]	; (2039074 <start_fast_boot_print_msg+0x68>)
 2039048:	f002 bed2 	b.w	203bdf0 <__boot_msg1_veneer>
 203904c:	4631      	mov	r1, r6
 203904e:	480a      	ldr	r0, [pc, #40]	; (2039078 <start_fast_boot_print_msg+0x6c>)
 2039050:	f002 fece 	bl	203bdf0 <__boot_msg1_veneer>
 2039054:	2d00      	cmp	r5, #0
 2039056:	d0eb      	beq.n	2039030 <start_fast_boot_print_msg+0x24>
 2039058:	e7ec      	b.n	2039034 <start_fast_boot_print_msg+0x28>
 203905a:	bf00      	nop
 203905c:	0203b4e8 	.word	0x0203b4e8
 2039060:	8000000c 	.word	0x8000000c
 2039064:	0203b4f8 	.word	0x0203b4f8
 2039068:	0203f228 	.word	0x0203f228
 203906c:	0203b50c 	.word	0x0203b50c
 2039070:	0203b530 	.word	0x0203b530
 2039074:	0203b54c 	.word	0x0203b54c
 2039078:	0203b518 	.word	0x0203b518

0203907c <soft_value_init>:
 203907c:	f44f 21e8 	mov.w	r1, #475136	; 0x74000
 2039080:	b430      	push	{r4, r5}
 2039082:	f04f 7200 	mov.w	r2, #33554432	; 0x2000000
 2039086:	f44f 45c0 	mov.w	r5, #24576	; 0x6000
 203908a:	2003      	movs	r0, #3
 203908c:	2404      	movs	r4, #4
 203908e:	4b0b      	ldr	r3, [pc, #44]	; (20390bc <soft_value_init+0x40>)
 2039090:	6099      	str	r1, [r3, #8]
 2039092:	490b      	ldr	r1, [pc, #44]	; (20390c0 <soft_value_init+0x44>)
 2039094:	e9c3 2500 	strd	r2, r5, [r3]
 2039098:	e9c3 4104 	strd	r4, r1, [r3, #16]
 203909c:	4d09      	ldr	r5, [pc, #36]	; (20390c4 <soft_value_init+0x48>)
 203909e:	4c0a      	ldr	r4, [pc, #40]	; (20390c8 <soft_value_init+0x4c>)
 20390a0:	f883 0024 	strb.w	r0, [r3, #36]	; 0x24
 20390a4:	4809      	ldr	r0, [pc, #36]	; (20390cc <soft_value_init+0x50>)
 20390a6:	60da      	str	r2, [r3, #12]
 20390a8:	e9c3 5006 	strd	r5, r0, [r3, #24]
 20390ac:	621c      	str	r4, [r3, #32]
 20390ae:	210d      	movs	r1, #13
 20390b0:	bc30      	pop	{r4, r5}
 20390b2:	4a07      	ldr	r2, [pc, #28]	; (20390d0 <soft_value_init+0x54>)
 20390b4:	f103 0025 	add.w	r0, r3, #37	; 0x25
 20390b8:	f002 be4a 	b.w	203bd50 <__strcpy_s_veneer>
 20390bc:	0203f228 	.word	0x0203f228
 20390c0:	02017000 	.word	0x02017000
 20390c4:	02039000 	.word	0x02039000
 20390c8:	0203be08 	.word	0x0203be08
 20390cc:	0203be08 	.word	0x0203be08
 20390d0:	0203b564 	.word	0x0203b564

020390d4 <fast_boot_malloc_funcs_init>:
 20390d4:	b500      	push	{lr}
 20390d6:	4a09      	ldr	r2, [pc, #36]	; (20390fc <fast_boot_malloc_funcs_init+0x28>)
 20390d8:	b085      	sub	sp, #20
 20390da:	4909      	ldr	r1, [pc, #36]	; (2039100 <fast_boot_malloc_funcs_init+0x2c>)
 20390dc:	4b09      	ldr	r3, [pc, #36]	; (2039104 <fast_boot_malloc_funcs_init+0x30>)
 20390de:	a801      	add	r0, sp, #4
 20390e0:	e9cd 1201 	strd	r1, r2, [sp, #4]
 20390e4:	9303      	str	r3, [sp, #12]
 20390e6:	f002 fe5f 	bl	203bda8 <__uapi_register_malloc_veneer>
 20390ea:	4b07      	ldr	r3, [pc, #28]	; (2039108 <fast_boot_malloc_funcs_init+0x34>)
 20390ec:	e9d3 0105 	ldrd	r0, r1, [r3, #20]
 20390f0:	f002 fe0a 	bl	203bd08 <__boot_malloc_init_veneer>
 20390f4:	b005      	add	sp, #20
 20390f6:	f85d fb04 	ldr.w	pc, [sp], #4
 20390fa:	bf00      	nop
 20390fc:	00016459 	.word	0x00016459
 2039100:	000163dd 	.word	0x000163dd
 2039104:	0001652d 	.word	0x0001652d
 2039108:	0203f228 	.word	0x0203f228

0203910c <fastboot_debug_uart_init>:
 203910c:	f44f 32e1 	mov.w	r2, #115200	; 0x1c200
 2039110:	2302      	movs	r3, #2
 2039112:	2001      	movs	r0, #1
 2039114:	4905      	ldr	r1, [pc, #20]	; (203912c <fastboot_debug_uart_init+0x20>)
 2039116:	b084      	sub	sp, #16
 2039118:	e9cd 2101 	strd	r2, r1, [sp, #4]
 203911c:	9303      	str	r3, [sp, #12]
 203911e:	ab04      	add	r3, sp, #16
 2039120:	e913 000e 	ldmdb	r3, {r1, r2, r3}
 2039124:	b004      	add	sp, #16
 2039126:	f002 be1b 	b.w	203bd60 <__serial_init_veneer>
 203912a:	bf00      	nop
 203912c:	00020008 	.word	0x00020008

02039130 <fastboot_flash_funcs_init>:
 2039130:	b510      	push	{r4, lr}
 2039132:	4908      	ldr	r1, [pc, #32]	; (2039154 <fastboot_flash_funcs_init+0x24>)
 2039134:	b084      	sub	sp, #16
 2039136:	4a08      	ldr	r2, [pc, #32]	; (2039158 <fastboot_flash_funcs_init+0x28>)
 2039138:	4b08      	ldr	r3, [pc, #32]	; (203915c <fastboot_flash_funcs_init+0x2c>)
 203913a:	4c09      	ldr	r4, [pc, #36]	; (2039160 <fastboot_flash_funcs_init+0x30>)
 203913c:	4668      	mov	r0, sp
 203913e:	e9cd 4100 	strd	r4, r1, [sp]
 2039142:	e9cd 2302 	strd	r2, r3, [sp, #8]
 2039146:	f002 fe43 	bl	203bdd0 <__uapi_cmd_regist_flash_cmd_veneer>
 203914a:	f000 f94d 	bl	20393e8 <uapi_flash_init>
 203914e:	b004      	add	sp, #16
 2039150:	bd10      	pop	{r4, pc}
 2039152:	bf00      	nop
 2039154:	020393b5 	.word	0x020393b5
 2039158:	02039371 	.word	0x02039371
 203915c:	02039341 	.word	0x02039341
 2039160:	020393e9 	.word	0x020393e9

02039164 <start_fast_boot_init>:
 2039164:	b508      	push	{r3, lr}
 2039166:	f002 fdd7 	bl	203bd18 <__uapi_watchdog_disable_veneer>
 203916a:	4806      	ldr	r0, [pc, #24]	; (2039184 <start_fast_boot_init+0x20>)
 203916c:	f002 fe2c 	bl	203bdc8 <__uapi_watchdog_enable_veneer>
 2039170:	f7ff ff84 	bl	203907c <soft_value_init>
 2039174:	f7ff ffae 	bl	20390d4 <fast_boot_malloc_funcs_init>
 2039178:	f7ff ffc8 	bl	203910c <fastboot_debug_uart_init>
 203917c:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
 2039180:	f7ff bfd6 	b.w	2039130 <fastboot_flash_funcs_init>
 2039184:	014fb180 	.word	0x014fb180

02039188 <uint_2_multiply.constprop.0>:
 2039188:	b109      	cbz	r1, 203918e <uint_2_multiply.constprop.0+0x6>
 203918a:	0300      	lsls	r0, r0, #12
 203918c:	6008      	str	r0, [r1, #0]
 203918e:	2001      	movs	r0, #1
 2039190:	4770      	bx	lr
 2039192:	bf00      	nop

02039194 <boot_kernel>:
 2039194:	3001      	adds	r0, #1
 2039196:	4700      	bx	r0

02039198 <boot_firmware>:
 2039198:	b510      	push	{r4, lr}
 203919a:	4818      	ldr	r0, [pc, #96]	; (20391fc <boot_firmware+0x64>)
 203919c:	b082      	sub	sp, #8
 203919e:	f002 fdaf 	bl	203bd00 <__boot_msg0_veneer>
 20391a2:	f44f 43e0 	mov.w	r3, #28672	; 0x7000
 20391a6:	203c      	movs	r0, #60	; 0x3c
 20391a8:	9301      	str	r3, [sp, #4]
 20391aa:	f002 fdbd 	bl	203bd28 <__boot_malloc_veneer>
 20391ae:	4604      	mov	r4, r0
 20391b0:	b128      	cbz	r0, 20391be <boot_firmware+0x26>
 20391b2:	4601      	mov	r1, r0
 20391b4:	223c      	movs	r2, #60	; 0x3c
 20391b6:	2004      	movs	r0, #4
 20391b8:	f000 fd98 	bl	2039cec <uapi_factory_nv_read>
 20391bc:	b180      	cbz	r0, 20391e0 <boot_firmware+0x48>
 20391be:	4620      	mov	r0, r4
 20391c0:	f002 fdba 	bl	203bd38 <__boot_free_veneer>
 20391c4:	f04f 7200 	mov.w	r2, #33554432	; 0x2000000
 20391c8:	f44f 31c8 	mov.w	r1, #102400	; 0x19000
 20391cc:	9801      	ldr	r0, [sp, #4]
 20391ce:	f000 f8f1 	bl	20393b4 <uapi_flash_read>
 20391d2:	b970      	cbnz	r0, 20391f2 <boot_firmware+0x5a>
 20391d4:	f04f 7000 	mov.w	r0, #33554432	; 0x2000000
 20391d8:	f7ff ffdc 	bl	2039194 <boot_kernel>
 20391dc:	b002      	add	sp, #8
 20391de:	bd10      	pop	{r4, pc}
 20391e0:	8a20      	ldrh	r0, [r4, #16]
 20391e2:	a901      	add	r1, sp, #4
 20391e4:	f3c0 000b 	ubfx	r0, r0, #0, #12
 20391e8:	f7ff ffce 	bl	2039188 <uint_2_multiply.constprop.0>
 20391ec:	2800      	cmp	r0, #0
 20391ee:	d1e6      	bne.n	20391be <boot_firmware+0x26>
 20391f0:	e7f4      	b.n	20391dc <boot_firmware+0x44>
 20391f2:	4803      	ldr	r0, [pc, #12]	; (2039200 <boot_firmware+0x68>)
 20391f4:	f002 fd84 	bl	203bd00 <__boot_msg0_veneer>
 20391f8:	e7ec      	b.n	20391d4 <boot_firmware+0x3c>
 20391fa:	bf00      	nop
 20391fc:	0203b574 	.word	0x0203b574
 2039200:	0203b584 	.word	0x0203b584

02039204 <start_fastboot>:
 2039204:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
 2039208:	b083      	sub	sp, #12
 203920a:	f7ff ffab 	bl	2039164 <start_fast_boot_init>
 203920e:	f44f 4080 	mov.w	r0, #16384	; 0x4000
 2039212:	f8df 80dc 	ldr.w	r8, [pc, #220]	; 20392f0 <start_fastboot+0xec>
 2039216:	4b37      	ldr	r3, [pc, #220]	; (20392f4 <start_fastboot+0xf0>)
 2039218:	f8c8 3044 	str.w	r3, [r8, #68]	; 0x44
 203921c:	f8d8 704c 	ldr.w	r7, [r8, #76]	; 0x4c
 2039220:	f007 477f 	and.w	r7, r7, #4278190080	; 0xff000000
 2039224:	ea47 0300 	orr.w	r3, r7, r0
 2039228:	f8c8 304c 	str.w	r3, [r8, #76]	; 0x4c
 203922c:	f000 fd5a 	bl	2039ce4 <uapi_factory_nv_init>
 2039230:	4604      	mov	r4, r0
 2039232:	b310      	cbz	r0, 203927a <start_fastboot+0x76>
 2039234:	2500      	movs	r5, #0
 2039236:	46a9      	mov	r9, r5
 2039238:	2128      	movs	r1, #40	; 0x28
 203923a:	2001      	movs	r0, #1
 203923c:	f002 fd70 	bl	203bd20 <__uart_auth_process_veneer>
 2039240:	b9a0      	cbnz	r0, 203926c <start_fastboot+0x68>
 2039242:	462a      	mov	r2, r5
 2039244:	4649      	mov	r1, r9
 2039246:	4620      	mov	r0, r4
 2039248:	f7ff fee0 	bl	203900c <start_fast_boot_print_msg>
 203924c:	a801      	add	r0, sp, #4
 203924e:	f002 fdcb 	bl	203bde8 <__uapi_time_counter_getms_veneer>
 2039252:	9901      	ldr	r1, [sp, #4]
 2039254:	4828      	ldr	r0, [pc, #160]	; (20392f8 <start_fastboot+0xf4>)
 2039256:	f002 fdcb 	bl	203bdf0 <__boot_msg1_veneer>
 203925a:	f002 fc9d 	bl	203bb98 <uapi_get_hw_chip_type>
 203925e:	2801      	cmp	r0, #1
 2039260:	d021      	beq.n	20392a6 <start_fastboot+0xa2>
 2039262:	f001 f951 	bl	203a508 <boot_start_main>
 2039266:	f7ff ff97 	bl	2039198 <boot_firmware>
 203926a:	e7fe      	b.n	203926a <start_fastboot+0x66>
 203926c:	f002 fd54 	bl	203bd18 <__uapi_watchdog_disable_veneer>
 2039270:	b003      	add	sp, #12
 2039272:	e8bd 43f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, lr}
 2039276:	f002 bd77 	b.w	203bd68 <__cli_loop_veneer>
 203927a:	203c      	movs	r0, #60	; 0x3c
 203927c:	f002 fd54 	bl	203bd28 <__boot_malloc_veneer>
 2039280:	4606      	mov	r6, r0
 2039282:	b168      	cbz	r0, 20392a0 <start_fastboot+0x9c>
 2039284:	f000 ff64 	bl	203a150 <uapi_flash_partition_init>
 2039288:	223c      	movs	r2, #60	; 0x3c
 203928a:	4681      	mov	r9, r0
 203928c:	4631      	mov	r1, r6
 203928e:	2004      	movs	r0, #4
 2039290:	f000 fd2c 	bl	2039cec <uapi_factory_nv_read>
 2039294:	4605      	mov	r5, r0
 2039296:	b170      	cbz	r0, 20392b6 <start_fastboot+0xb2>
 2039298:	4630      	mov	r0, r6
 203929a:	f002 fd4d 	bl	203bd38 <__boot_free_veneer>
 203929e:	e7cb      	b.n	2039238 <start_fastboot+0x34>
 20392a0:	4605      	mov	r5, r0
 20392a2:	4681      	mov	r9, r0
 20392a4:	e7f8      	b.n	2039298 <start_fastboot+0x94>
 20392a6:	4815      	ldr	r0, [pc, #84]	; (20392fc <start_fastboot+0xf8>)
 20392a8:	f002 fd2a 	bl	203bd00 <__boot_msg0_veneer>
 20392ac:	f001 f92c 	bl	203a508 <boot_start_main>
 20392b0:	f7ff ff72 	bl	2039198 <boot_firmware>
 20392b4:	e7d9      	b.n	203926a <start_fastboot+0x66>
 20392b6:	89b0      	ldrh	r0, [r6, #12]
 20392b8:	4629      	mov	r1, r5
 20392ba:	f3c0 000b 	ubfx	r0, r0, #0, #12
 20392be:	f7ff ff63 	bl	2039188 <uint_2_multiply.constprop.0>
 20392c2:	2800      	cmp	r0, #0
 20392c4:	d0e8      	beq.n	2039298 <start_fastboot+0x94>
 20392c6:	8930      	ldrh	r0, [r6, #8]
 20392c8:	f3c0 000b 	ubfx	r0, r0, #0, #12
 20392cc:	f7ff ff5c 	bl	2039188 <uint_2_multiply.constprop.0>
 20392d0:	2800      	cmp	r0, #0
 20392d2:	d0e1      	beq.n	2039298 <start_fastboot+0x94>
 20392d4:	8933      	ldrh	r3, [r6, #8]
 20392d6:	89b2      	ldrh	r2, [r6, #12]
 20392d8:	f3c3 030b 	ubfx	r3, r3, #0, #12
 20392dc:	ea47 3303 	orr.w	r3, r7, r3, lsl #12
 20392e0:	f8c8 304c 	str.w	r3, [r8, #76]	; 0x4c
 20392e4:	f3c2 020b 	ubfx	r2, r2, #0, #12
 20392e8:	4b05      	ldr	r3, [pc, #20]	; (2039300 <start_fastboot+0xfc>)
 20392ea:	0312      	lsls	r2, r2, #12
 20392ec:	605a      	str	r2, [r3, #4]
 20392ee:	e7d3      	b.n	2039298 <start_fastboot+0x94>
 20392f0:	40030000 	.word	0x40030000
 20392f4:	8000000c 	.word	0x8000000c
 20392f8:	0203b5ac 	.word	0x0203b5ac
 20392fc:	0203b5c0 	.word	0x0203b5c0
 2039300:	0203f228 	.word	0x0203f228

02039304 <uapi_flash_protect_cancel>:
 2039304:	4b03      	ldr	r3, [pc, #12]	; (2039314 <uapi_flash_protect_cancel+0x10>)
 2039306:	7818      	ldrb	r0, [r3, #0]
 2039308:	b900      	cbnz	r0, 203930c <uapi_flash_protect_cancel+0x8>
 203930a:	4770      	bx	lr
 203930c:	2100      	movs	r1, #0
 203930e:	4608      	mov	r0, r1
 2039310:	f002 bc84 	b.w	203bc1c <spi_flash_set_protect>
 2039314:	0203c35c 	.word	0x0203c35c

02039318 <uapi_flash_protect_enable>:
 2039318:	4b07      	ldr	r3, [pc, #28]	; (2039338 <uapi_flash_protect_enable+0x20>)
 203931a:	4a08      	ldr	r2, [pc, #32]	; (203933c <uapi_flash_protect_enable+0x24>)
 203931c:	7018      	strb	r0, [r3, #0]
 203931e:	6cd3      	ldr	r3, [r2, #76]	; 0x4c
 2039320:	b120      	cbz	r0, 203932c <uapi_flash_protect_enable+0x14>
 2039322:	f043 4300 	orr.w	r3, r3, #2147483648	; 0x80000000
 2039326:	2000      	movs	r0, #0
 2039328:	64d3      	str	r3, [r2, #76]	; 0x4c
 203932a:	4770      	bx	lr
 203932c:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
 2039330:	2000      	movs	r0, #0
 2039332:	64d3      	str	r3, [r2, #76]	; 0x4c
 2039334:	4770      	bx	lr
 2039336:	bf00      	nop
 2039338:	0203c35c 	.word	0x0203c35c
 203933c:	40030000 	.word	0x40030000

02039340 <uapi_flash_erase>:
 2039340:	b538      	push	{r3, r4, r5, lr}
 2039342:	4604      	mov	r4, r0
 2039344:	460d      	mov	r5, r1
 2039346:	2302      	movs	r3, #2
 2039348:	4621      	mov	r1, r4
 203934a:	462a      	mov	r2, r5
 203934c:	4807      	ldr	r0, [pc, #28]	; (203936c <uapi_flash_erase+0x2c>)
 203934e:	f002 fd23 	bl	203bd98 <__sfc_check_para_veneer>
 2039352:	b100      	cbz	r0, 2039356 <uapi_flash_erase+0x16>
 2039354:	bd38      	pop	{r3, r4, r5, pc}
 2039356:	f7ff ffd5 	bl	2039304 <uapi_flash_protect_cancel>
 203935a:	2800      	cmp	r0, #0
 203935c:	d1fa      	bne.n	2039354 <uapi_flash_erase+0x14>
 203935e:	462a      	mov	r2, r5
 2039360:	4621      	mov	r1, r4
 2039362:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 2039366:	4801      	ldr	r0, [pc, #4]	; (203936c <uapi_flash_erase+0x2c>)
 2039368:	f002 bd02 	b.w	203bd70 <__flash_erase_prv_veneer>
 203936c:	0203c308 	.word	0x0203c308

02039370 <uapi_flash_write>:
 2039370:	b1da      	cbz	r2, 20393aa <uapi_flash_write+0x3a>
 2039372:	b5f0      	push	{r4, r5, r6, r7, lr}
 2039374:	4605      	mov	r5, r0
 2039376:	460e      	mov	r6, r1
 2039378:	4614      	mov	r4, r2
 203937a:	461f      	mov	r7, r3
 203937c:	460a      	mov	r2, r1
 203937e:	b083      	sub	sp, #12
 2039380:	4601      	mov	r1, r0
 2039382:	2301      	movs	r3, #1
 2039384:	480a      	ldr	r0, [pc, #40]	; (20393b0 <uapi_flash_write+0x40>)
 2039386:	f002 fd07 	bl	203bd98 <__sfc_check_para_veneer>
 203938a:	b108      	cbz	r0, 2039390 <uapi_flash_write+0x20>
 203938c:	b003      	add	sp, #12
 203938e:	bdf0      	pop	{r4, r5, r6, r7, pc}
 2039390:	f7ff ffb8 	bl	2039304 <uapi_flash_protect_cancel>
 2039394:	2800      	cmp	r0, #0
 2039396:	d1f9      	bne.n	203938c <uapi_flash_write+0x1c>
 2039398:	4633      	mov	r3, r6
 203939a:	4622      	mov	r2, r4
 203939c:	4629      	mov	r1, r5
 203939e:	9700      	str	r7, [sp, #0]
 20393a0:	4803      	ldr	r0, [pc, #12]	; (20393b0 <uapi_flash_write+0x40>)
 20393a2:	f002 fcd9 	bl	203bd58 <__flash_write_prv_veneer>
 20393a6:	b003      	add	sp, #12
 20393a8:	bdf0      	pop	{r4, r5, r6, r7, pc}
 20393aa:	f640 607a 	movw	r0, #3706	; 0xe7a
 20393ae:	4770      	bx	lr
 20393b0:	0203c308 	.word	0x0203c308

020393b4 <uapi_flash_read>:
 20393b4:	b19a      	cbz	r2, 20393de <uapi_flash_read+0x2a>
 20393b6:	b570      	push	{r4, r5, r6, lr}
 20393b8:	2300      	movs	r3, #0
 20393ba:	4605      	mov	r5, r0
 20393bc:	460e      	mov	r6, r1
 20393be:	4614      	mov	r4, r2
 20393c0:	460a      	mov	r2, r1
 20393c2:	4601      	mov	r1, r0
 20393c4:	4807      	ldr	r0, [pc, #28]	; (20393e4 <uapi_flash_read+0x30>)
 20393c6:	f002 fce7 	bl	203bd98 <__sfc_check_para_veneer>
 20393ca:	b100      	cbz	r0, 20393ce <uapi_flash_read+0x1a>
 20393cc:	bd70      	pop	{r4, r5, r6, pc}
 20393ce:	4633      	mov	r3, r6
 20393d0:	4622      	mov	r2, r4
 20393d2:	4629      	mov	r1, r5
 20393d4:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
 20393d8:	4802      	ldr	r0, [pc, #8]	; (20393e4 <uapi_flash_read+0x30>)
 20393da:	f002 bce9 	b.w	203bdb0 <__flash_read_prv_veneer>
 20393de:	f640 607a 	movw	r0, #3706	; 0xe7a
 20393e2:	4770      	bx	lr
 20393e4:	0203c308 	.word	0x0203c308

020393e8 <uapi_flash_init>:
 20393e8:	2300      	movs	r3, #0
 20393ea:	b530      	push	{r4, r5, lr}
 20393ec:	4c13      	ldr	r4, [pc, #76]	; (203943c <uapi_flash_init+0x54>)
 20393ee:	b083      	sub	sp, #12
 20393f0:	6822      	ldr	r2, [r4, #0]
 20393f2:	e9cd 3300 	strd	r3, r3, [sp]
 20393f6:	2a01      	cmp	r2, #1
 20393f8:	d018      	beq.n	203942c <uapi_flash_init+0x44>
 20393fa:	f44f 6580 	mov.w	r5, #1024	; 0x400
 20393fe:	4910      	ldr	r1, [pc, #64]	; (2039440 <uapi_flash_init+0x58>)
 2039400:	2304      	movs	r3, #4
 2039402:	63a1      	str	r1, [r4, #56]	; 0x38
 2039404:	4620      	mov	r0, r4
 2039406:	4669      	mov	r1, sp
 2039408:	4a0e      	ldr	r2, [pc, #56]	; (2039444 <uapi_flash_init+0x5c>)
 203940a:	6365      	str	r5, [r4, #52]	; 0x34
 203940c:	f002 fcd4 	bl	203bdb8 <__spi_flash_basic_info_probe_veneer>
 2039410:	4605      	mov	r5, r0
 2039412:	b110      	cbz	r0, 203941a <uapi_flash_init+0x32>
 2039414:	4628      	mov	r0, r5
 2039416:	b003      	add	sp, #12
 2039418:	bd30      	pop	{r4, r5, pc}
 203941a:	f002 fbbd 	bl	203bb98 <uapi_get_hw_chip_type>
 203941e:	2801      	cmp	r0, #1
 2039420:	d009      	beq.n	2039436 <uapi_flash_init+0x4e>
 2039422:	2301      	movs	r3, #1
 2039424:	4628      	mov	r0, r5
 2039426:	6023      	str	r3, [r4, #0]
 2039428:	b003      	add	sp, #12
 203942a:	bd30      	pop	{r4, r5, pc}
 203942c:	f640 6589 	movw	r5, #3721	; 0xe89
 2039430:	4628      	mov	r0, r5
 2039432:	b003      	add	sp, #12
 2039434:	bd30      	pop	{r4, r5, pc}
 2039436:	f7ff ff6f 	bl	2039318 <uapi_flash_protect_enable>
 203943a:	e7f2      	b.n	2039422 <uapi_flash_init+0x3a>
 203943c:	0203c308 	.word	0x0203c308
 2039440:	0203bf08 	.word	0x0203bf08
 2039444:	0203b5fc 	.word	0x0203b5fc

02039448 <crc32_no_comp>:
 2039448:	078b      	lsls	r3, r1, #30
 203944a:	b410      	push	{r4}
 203944c:	d00e      	beq.n	203946c <crc32_no_comp+0x24>
 203944e:	b15a      	cbz	r2, 2039468 <crc32_no_comp+0x20>
 2039450:	4b23      	ldr	r3, [pc, #140]	; (20394e0 <crc32_no_comp+0x98>)
 2039452:	440a      	add	r2, r1
 2039454:	f811 4b01 	ldrb.w	r4, [r1], #1
 2039458:	4044      	eors	r4, r0
 203945a:	b2e4      	uxtb	r4, r4
 203945c:	f853 4024 	ldr.w	r4, [r3, r4, lsl #2]
 2039460:	4291      	cmp	r1, r2
 2039462:	ea84 2010 	eor.w	r0, r4, r0, lsr #8
 2039466:	d1f5      	bne.n	2039454 <crc32_no_comp+0xc>
 2039468:	bc10      	pop	{r4}
 203946a:	4770      	bx	lr
 203946c:	f002 0403 	and.w	r4, r2, #3
 2039470:	0892      	lsrs	r2, r2, #2
 2039472:	f1a1 0c04 	sub.w	ip, r1, #4
 2039476:	d031      	beq.n	20394dc <crc32_no_comp+0x94>
 2039478:	f102 4280 	add.w	r2, r2, #1073741824	; 0x40000000
 203947c:	3a01      	subs	r2, #1
 203947e:	4b18      	ldr	r3, [pc, #96]	; (20394e0 <crc32_no_comp+0x98>)
 2039480:	eb01 0182 	add.w	r1, r1, r2, lsl #2
 2039484:	f85c 2f04 	ldr.w	r2, [ip, #4]!
 2039488:	4050      	eors	r0, r2
 203948a:	b2c2      	uxtb	r2, r0
 203948c:	f853 2022 	ldr.w	r2, [r3, r2, lsl #2]
 2039490:	458c      	cmp	ip, r1
 2039492:	ea82 2210 	eor.w	r2, r2, r0, lsr #8
 2039496:	b2d0      	uxtb	r0, r2
 2039498:	f853 0020 	ldr.w	r0, [r3, r0, lsl #2]
 203949c:	ea80 2212 	eor.w	r2, r0, r2, lsr #8
 20394a0:	b2d0      	uxtb	r0, r2
 20394a2:	f853 0020 	ldr.w	r0, [r3, r0, lsl #2]
 20394a6:	ea80 2012 	eor.w	r0, r0, r2, lsr #8
 20394aa:	b2c2      	uxtb	r2, r0
 20394ac:	f853 2022 	ldr.w	r2, [r3, r2, lsl #2]
 20394b0:	ea82 2010 	eor.w	r0, r2, r0, lsr #8
 20394b4:	d1e6      	bne.n	2039484 <crc32_no_comp+0x3c>
 20394b6:	2c00      	cmp	r4, #0
 20394b8:	d0d6      	beq.n	2039468 <crc32_no_comp+0x20>
 20394ba:	3403      	adds	r4, #3
 20394bc:	4b08      	ldr	r3, [pc, #32]	; (20394e0 <crc32_no_comp+0x98>)
 20394be:	f101 0c03 	add.w	ip, r1, #3
 20394c2:	4421      	add	r1, r4
 20394c4:	f81c 2f01 	ldrb.w	r2, [ip, #1]!
 20394c8:	4042      	eors	r2, r0
 20394ca:	b2d2      	uxtb	r2, r2
 20394cc:	f853 2022 	ldr.w	r2, [r3, r2, lsl #2]
 20394d0:	458c      	cmp	ip, r1
 20394d2:	ea82 2010 	eor.w	r0, r2, r0, lsr #8
 20394d6:	d1f5      	bne.n	20394c4 <crc32_no_comp+0x7c>
 20394d8:	bc10      	pop	{r4}
 20394da:	4770      	bx	lr
 20394dc:	4661      	mov	r1, ip
 20394de:	e7ea      	b.n	20394b6 <crc32_no_comp+0x6e>
 20394e0:	0203b64c 	.word	0x0203b64c

020394e4 <uapi_crc32>:
 20394e4:	b508      	push	{r3, lr}
 20394e6:	43c0      	mvns	r0, r0
 20394e8:	f7ff ffae 	bl	2039448 <crc32_no_comp>
 20394ec:	43c0      	mvns	r0, r0
 20394ee:	bd08      	pop	{r3, pc}

020394f0 <lzma_free>:
 20394f0:	4608      	mov	r0, r1
 20394f2:	b109      	cbz	r1, 20394f8 <lzma_free+0x8>
 20394f4:	f002 bc20 	b.w	203bd38 <__boot_free_veneer>
 20394f8:	4770      	bx	lr
 20394fa:	bf00      	nop

020394fc <lzma_alloc>:
 20394fc:	b538      	push	{r3, r4, r5, lr}
 20394fe:	4608      	mov	r0, r1
 2039500:	460d      	mov	r5, r1
 2039502:	f002 fc11 	bl	203bd28 <__boot_malloc_veneer>
 2039506:	4604      	mov	r4, r0
 2039508:	b108      	cbz	r0, 203950e <lzma_alloc+0x12>
 203950a:	4620      	mov	r0, r4
 203950c:	bd38      	pop	{r3, r4, r5, pc}
 203950e:	4629      	mov	r1, r5
 2039510:	4802      	ldr	r0, [pc, #8]	; (203951c <lzma_alloc+0x20>)
 2039512:	f002 fc6d 	bl	203bdf0 <__boot_msg1_veneer>
 2039516:	4620      	mov	r0, r4
 2039518:	bd38      	pop	{r3, r4, r5, pc}
 203951a:	bf00      	nop
 203951c:	0203ba4c 	.word	0x0203ba4c

02039520 <uapi_lzma_get_uncompress_len>:
 2039520:	b1a8      	cbz	r0, 203954e <uapi_lzma_get_uncompress_len+0x2e>
 2039522:	b1a2      	cbz	r2, 203954e <uapi_lzma_get_uncompress_len+0x2e>
 2039524:	b19b      	cbz	r3, 203954e <uapi_lzma_get_uncompress_len+0x2e>
 2039526:	290d      	cmp	r1, #13
 2039528:	d811      	bhi.n	203954e <uapi_lzma_get_uncompress_len+0x2e>
 203952a:	4684      	mov	ip, r0
 203952c:	f8d0 0009 	ldr.w	r0, [r0, #9]
 2039530:	b968      	cbnz	r0, 203954e <uapi_lzma_get_uncompress_len+0x2e>
 2039532:	b410      	push	{r4}
 2039534:	f8dc 1001 	ldr.w	r1, [ip, #1]
 2039538:	f8dc 4005 	ldr.w	r4, [ip, #5]
 203953c:	f5b1 5f80 	cmp.w	r1, #4096	; 0x1000
 2039540:	bf38      	it	cc
 2039542:	f44f 5180 	movcc.w	r1, #4096	; 0x1000
 2039546:	6019      	str	r1, [r3, #0]
 2039548:	6014      	str	r4, [r2, #0]
 203954a:	bc10      	pop	{r4}
 203954c:	4770      	bx	lr
 203954e:	f04f 30ff 	mov.w	r0, #4294967295	; 0xffffffff
 2039552:	4770      	bx	lr

02039554 <uapi_lzma_decompress>:
 2039554:	b570      	push	{r4, r5, r6, lr}
 2039556:	b08e      	sub	sp, #56	; 0x38
 2039558:	930b      	str	r3, [sp, #44]	; 0x2c
 203955a:	2300      	movs	r3, #0
 203955c:	260d      	movs	r6, #13
 203955e:	4614      	mov	r4, r2
 2039560:	4605      	mov	r5, r0
 2039562:	e9cd 3305 	strd	r3, r3, [sp, #20]
 2039566:	9307      	str	r3, [sp, #28]
 2039568:	930c      	str	r3, [sp, #48]	; 0x30
 203956a:	4b0f      	ldr	r3, [pc, #60]	; (20395a8 <uapi_lzma_decompress+0x54>)
 203956c:	9a12      	ldr	r2, [sp, #72]	; 0x48
 203956e:	9308      	str	r3, [sp, #32]
 2039570:	4b0e      	ldr	r3, [pc, #56]	; (20395ac <uapi_lzma_decompress+0x58>)
 2039572:	920d      	str	r2, [sp, #52]	; 0x34
 2039574:	9309      	str	r3, [sp, #36]	; 0x24
 2039576:	aa06      	add	r2, sp, #24
 2039578:	ab07      	add	r3, sp, #28
 203957a:	960a      	str	r6, [sp, #40]	; 0x28
 203957c:	f7ff ffd0 	bl	2039520 <uapi_lzma_get_uncompress_len>
 2039580:	9906      	ldr	r1, [sp, #24]
 2039582:	aa0c      	add	r2, sp, #48	; 0x30
 2039584:	ab0a      	add	r3, sp, #40	; 0x28
 2039586:	3c0d      	subs	r4, #13
 2039588:	e9cd 3200 	strd	r3, r2, [sp]
 203958c:	e9cd 4102 	strd	r4, r1, [sp, #8]
 2039590:	4628      	mov	r0, r5
 2039592:	2105      	movs	r1, #5
 2039594:	ab08      	add	r3, sp, #32
 2039596:	aa05      	add	r2, sp, #20
 2039598:	f002 fc22 	bl	203bde0 <__LzmaDecode2_veneer>
 203959c:	3800      	subs	r0, #0
 203959e:	bf18      	it	ne
 20395a0:	2001      	movne	r0, #1
 20395a2:	4240      	negs	r0, r0
 20395a4:	b00e      	add	sp, #56	; 0x38
 20395a6:	bd70      	pop	{r4, r5, r6, pc}
 20395a8:	020394fd 	.word	0x020394fd
 20395ac:	020394f1 	.word	0x020394f1

020395b0 <uapi_lzma_mem_detect>:
 20395b0:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
 20395b4:	2400      	movs	r4, #0
 20395b6:	b08c      	sub	sp, #48	; 0x30
 20395b8:	ad07      	add	r5, sp, #28
 20395ba:	466a      	mov	r2, sp
 20395bc:	ab01      	add	r3, sp, #4
 20395be:	4607      	mov	r7, r0
 20395c0:	e9cd 4401 	strd	r4, r4, [sp, #4]
 20395c4:	e9cd 4403 	strd	r4, r4, [sp, #12]
 20395c8:	e9cd 4405 	strd	r4, r4, [sp, #20]
 20395cc:	e9c5 4401 	strd	r4, r4, [r5, #4]
 20395d0:	e9c5 4403 	strd	r4, r4, [r5, #12]
 20395d4:	9400      	str	r4, [sp, #0]
 20395d6:	9407      	str	r4, [sp, #28]
 20395d8:	f7ff ffa2 	bl	2039520 <uapi_lzma_get_uncompress_len>
 20395dc:	4606      	mov	r6, r0
 20395de:	2800      	cmp	r0, #0
 20395e0:	d143      	bne.n	203966a <uapi_lzma_mem_detect+0xba>
 20395e2:	7838      	ldrb	r0, [r7, #0]
 20395e4:	28e0      	cmp	r0, #224	; 0xe0
 20395e6:	d844      	bhi.n	2039672 <uapi_lzma_mem_detect+0xc2>
 20395e8:	4b25      	ldr	r3, [pc, #148]	; (2039680 <uapi_lzma_mem_detect+0xd0>)
 20395ea:	f44f 5180 	mov.w	r1, #4096	; 0x1000
 20395ee:	fba3 2300 	umull	r2, r3, r3, r0
 20395f2:	085a      	lsrs	r2, r3, #1
 20395f4:	4613      	mov	r3, r2
 20395f6:	eb02 02c2 	add.w	r2, r2, r2, lsl #3
 20395fa:	1a82      	subs	r2, r0, r2
 20395fc:	f44f 7040 	mov.w	r0, #768	; 0x300
 2039600:	f44f 6480 	mov.w	r4, #1024	; 0x400
 2039604:	e9cd 1103 	strd	r1, r1, [sp, #12]
 2039608:	491e      	ldr	r1, [pc, #120]	; (2039684 <uapi_lzma_mem_detect+0xd4>)
 203960a:	b2d2      	uxtb	r2, r2
 203960c:	fba1 1c03 	umull	r1, ip, r1, r3
 2039610:	9901      	ldr	r1, [sp, #4]
 2039612:	9406      	str	r4, [sp, #24]
 2039614:	9105      	str	r1, [sp, #20]
 2039616:	f00c 011c 	and.w	r1, ip, #28
 203961a:	eb01 0c9c 	add.w	ip, r1, ip, lsr #2
 203961e:	eba3 030c 	sub.w	r3, r3, ip
 2039622:	b2db      	uxtb	r3, r3
 2039624:	4413      	add	r3, r2
 2039626:	4098      	lsls	r0, r3
 2039628:	f200 7336 	addw	r3, r0, #1846	; 0x736
 203962c:	0058      	lsls	r0, r3, #1
 203962e:	462f      	mov	r7, r5
 2039630:	f002 fb7a 	bl	203bd28 <__boot_malloc_veneer>
 2039634:	ac0c      	add	r4, sp, #48	; 0x30
 2039636:	f10d 080c 	add.w	r8, sp, #12
 203963a:	f847 0b04 	str.w	r0, [r7], #4
 203963e:	b148      	cbz	r0, 2039654 <uapi_lzma_mem_detect+0xa4>
 2039640:	42a7      	cmp	r7, r4
 2039642:	d009      	beq.n	2039658 <uapi_lzma_mem_detect+0xa8>
 2039644:	f858 0b04 	ldr.w	r0, [r8], #4
 2039648:	f002 fb6e 	bl	203bd28 <__boot_malloc_veneer>
 203964c:	f847 0b04 	str.w	r0, [r7], #4
 2039650:	2800      	cmp	r0, #0
 2039652:	d1f5      	bne.n	2039640 <uapi_lzma_mem_detect+0x90>
 2039654:	f04f 36ff 	mov.w	r6, #4294967295	; 0xffffffff
 2039658:	ac0b      	add	r4, sp, #44	; 0x2c
 203965a:	6820      	ldr	r0, [r4, #0]
 203965c:	4627      	mov	r7, r4
 203965e:	3c04      	subs	r4, #4
 2039660:	b108      	cbz	r0, 2039666 <uapi_lzma_mem_detect+0xb6>
 2039662:	f002 fb69 	bl	203bd38 <__boot_free_veneer>
 2039666:	42af      	cmp	r7, r5
 2039668:	d1f7      	bne.n	203965a <uapi_lzma_mem_detect+0xaa>
 203966a:	4630      	mov	r0, r6
 203966c:	b00c      	add	sp, #48	; 0x30
 203966e:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 2039672:	f04f 36ff 	mov.w	r6, #4294967295	; 0xffffffff
 2039676:	4630      	mov	r0, r6
 2039678:	b00c      	add	sp, #48	; 0x30
 203967a:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 203967e:	bf00      	nop
 2039680:	38e38e39 	.word	0x38e38e39
 2039684:	cccccccd 	.word	0xcccccccd

02039688 <nv_init_data_check_all_success>:
 2039688:	b430      	push	{r4, r5}
 203968a:	68c4      	ldr	r4, [r0, #12]
 203968c:	68cd      	ldr	r5, [r1, #12]
 203968e:	42ac      	cmp	r4, r5
 2039690:	d90a      	bls.n	20396a8 <nv_init_data_check_all_success+0x20>
 2039692:	f502 5180 	add.w	r1, r2, #4096	; 0x1000
 2039696:	e9c3 2100 	strd	r2, r1, [r3]
 203969a:	8942      	ldrh	r2, [r0, #10]
 203969c:	301c      	adds	r0, #28
 203969e:	609c      	str	r4, [r3, #8]
 20396a0:	825a      	strh	r2, [r3, #18]
 20396a2:	6158      	str	r0, [r3, #20]
 20396a4:	bc30      	pop	{r4, r5}
 20396a6:	4770      	bx	lr
 20396a8:	4608      	mov	r0, r1
 20396aa:	f502 5180 	add.w	r1, r2, #4096	; 0x1000
 20396ae:	462c      	mov	r4, r5
 20396b0:	e9c3 1200 	strd	r1, r2, [r3]
 20396b4:	e7f1      	b.n	203969a <nv_init_data_check_all_success+0x12>
 20396b6:	bf00      	nop

020396b8 <nv_init_data_prepare>:
 20396b8:	b570      	push	{r4, r5, r6, lr}
 20396ba:	4606      	mov	r6, r0
 20396bc:	f44f 5080 	mov.w	r0, #4096	; 0x1000
 20396c0:	460d      	mov	r5, r1
 20396c2:	f002 fb31 	bl	203bd28 <__boot_malloc_veneer>
 20396c6:	b158      	cbz	r0, 20396e0 <nv_init_data_prepare+0x28>
 20396c8:	4602      	mov	r2, r0
 20396ca:	4604      	mov	r4, r0
 20396cc:	f44f 5180 	mov.w	r1, #4096	; 0x1000
 20396d0:	4628      	mov	r0, r5
 20396d2:	f7ff fe6f 	bl	20393b4 <uapi_flash_read>
 20396d6:	4605      	mov	r5, r0
 20396d8:	b930      	cbnz	r0, 20396e8 <nv_init_data_prepare+0x30>
 20396da:	4628      	mov	r0, r5
 20396dc:	6034      	str	r4, [r6, #0]
 20396de:	bd70      	pop	{r4, r5, r6, pc}
 20396e0:	f241 35ef 	movw	r5, #5103	; 0x13ef
 20396e4:	4628      	mov	r0, r5
 20396e6:	bd70      	pop	{r4, r5, r6, pc}
 20396e8:	4620      	mov	r0, r4
 20396ea:	f002 fb25 	bl	203bd38 <__boot_free_veneer>
 20396ee:	4628      	mov	r0, r5
 20396f0:	bd70      	pop	{r4, r5, r6, pc}
 20396f2:	bf00      	nop

020396f4 <nv_check_file>:
 20396f4:	b538      	push	{r3, r4, r5, lr}
 20396f6:	4c11      	ldr	r4, [pc, #68]	; (203973c <nv_check_file+0x48>)
 20396f8:	4d11      	ldr	r5, [pc, #68]	; (2039740 <nv_check_file+0x4c>)
 20396fa:	2a01      	cmp	r2, #1
 20396fc:	bf18      	it	ne
 20396fe:	462c      	movne	r4, r5
 2039700:	6802      	ldr	r2, [r0, #0]
 2039702:	42a2      	cmp	r2, r4
 2039704:	d116      	bne.n	2039734 <nv_check_file+0x40>
 2039706:	8942      	ldrh	r2, [r0, #10]
 2039708:	4603      	mov	r3, r0
 203970a:	f5b2 7f80 	cmp.w	r2, #256	; 0x100
 203970e:	d80e      	bhi.n	203972e <nv_check_file+0x3a>
 2039710:	f1a1 0208 	sub.w	r2, r1, #8
 2039714:	2000      	movs	r0, #0
 2039716:	f103 0108 	add.w	r1, r3, #8
 203971a:	685c      	ldr	r4, [r3, #4]
 203971c:	f7ff fee2 	bl	20394e4 <uapi_crc32>
 2039720:	f241 33f1 	movw	r3, #5105	; 0x13f1
 2039724:	4284      	cmp	r4, r0
 2039726:	bf14      	ite	ne
 2039728:	4618      	movne	r0, r3
 203972a:	2000      	moveq	r0, #0
 203972c:	bd38      	pop	{r3, r4, r5, pc}
 203972e:	f241 30ee 	movw	r0, #5102	; 0x13ee
 2039732:	bd38      	pop	{r3, r4, r5, pc}
 2039734:	f241 30f2 	movw	r0, #5106	; 0x13f2
 2039738:	bd38      	pop	{r3, r4, r5, pc}
 203973a:	bf00      	nop
 203973c:	23564e46 	.word	0x23564e46
 2039740:	24564e48 	.word	0x24564e48

02039744 <nv_repair_from_backup>:
 2039744:	b570      	push	{r4, r5, r6, lr}
 2039746:	4604      	mov	r4, r0
 2039748:	460d      	mov	r5, r1
 203974a:	4610      	mov	r0, r2
 203974c:	f44f 5180 	mov.w	r1, #4096	; 0x1000
 2039750:	4622      	mov	r2, r4
 2039752:	f7ff fe2f 	bl	20393b4 <uapi_flash_read>
 2039756:	b100      	cbz	r0, 203975a <nv_repair_from_backup+0x16>
 2039758:	bd70      	pop	{r4, r5, r6, pc}
 203975a:	f44f 5180 	mov.w	r1, #4096	; 0x1000
 203975e:	4628      	mov	r0, r5
 2039760:	f7ff fdee 	bl	2039340 <uapi_flash_erase>
 2039764:	2800      	cmp	r0, #0
 2039766:	d1f7      	bne.n	2039758 <nv_repair_from_backup+0x14>
 2039768:	4603      	mov	r3, r0
 203976a:	4622      	mov	r2, r4
 203976c:	4628      	mov	r0, r5
 203976e:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
 2039772:	f44f 5180 	mov.w	r1, #4096	; 0x1000
 2039776:	f7ff bdfb 	b.w	2039370 <uapi_flash_write>
 203977a:	bf00      	nop

0203977c <nv_init_data_check_all_fail>:
 203977c:	b570      	push	{r4, r5, r6, lr}
 203977e:	2a01      	cmp	r2, #1
 2039780:	9e04      	ldr	r6, [sp, #16]
 2039782:	d012      	beq.n	20397aa <nv_init_data_check_all_fail+0x2e>
 2039784:	4605      	mov	r5, r0
 2039786:	460a      	mov	r2, r1
 2039788:	4601      	mov	r1, r0
 203978a:	4630      	mov	r0, r6
 203978c:	461c      	mov	r4, r3
 203978e:	f7ff ffd9 	bl	2039744 <nv_repair_from_backup>
 2039792:	b948      	cbnz	r0, 20397a8 <nv_init_data_check_all_fail+0x2c>
 2039794:	f505 5380 	add.w	r3, r5, #4096	; 0x1000
 2039798:	8972      	ldrh	r2, [r6, #10]
 203979a:	e9c4 5300 	strd	r5, r3, [r4]
 203979e:	68f3      	ldr	r3, [r6, #12]
 20397a0:	361c      	adds	r6, #28
 20397a2:	8262      	strh	r2, [r4, #18]
 20397a4:	60a3      	str	r3, [r4, #8]
 20397a6:	6166      	str	r6, [r4, #20]
 20397a8:	bd70      	pop	{r4, r5, r6, pc}
 20397aa:	f241 30f0 	movw	r0, #5104	; 0x13f0
 20397ae:	bd70      	pop	{r4, r5, r6, pc}

020397b0 <nv_write_update_data>:
 20397b0:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
 20397b4:	460e      	mov	r6, r1
 20397b6:	4604      	mov	r4, r0
 20397b8:	f44f 5180 	mov.w	r1, #4096	; 0x1000
 20397bc:	6830      	ldr	r0, [r6, #0]
 20397be:	b082      	sub	sp, #8
 20397c0:	4615      	mov	r5, r2
 20397c2:	461f      	mov	r7, r3
 20397c4:	f894 8004 	ldrb.w	r8, [r4, #4]
 20397c8:	f7ff fdf4 	bl	20393b4 <uapi_flash_read>
 20397cc:	4684      	mov	ip, r0
 20397ce:	b118      	cbz	r0, 20397d8 <nv_write_update_data+0x28>
 20397d0:	4660      	mov	r0, ip
 20397d2:	b002      	add	sp, #8
 20397d4:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 20397d8:	4642      	mov	r2, r8
 20397da:	6821      	ldr	r1, [r4, #0]
 20397dc:	f7ff fe82 	bl	20394e4 <uapi_crc32>
 20397e0:	9001      	str	r0, [sp, #4]
 20397e2:	f1b8 0f00 	cmp.w	r8, #0
 20397e6:	d12f      	bne.n	2039848 <nv_write_update_data+0x98>
 20397e8:	2304      	movs	r3, #4
 20397ea:	8878      	ldrh	r0, [r7, #2]
 20397ec:	eb0d 0203 	add.w	r2, sp, r3
 20397f0:	f5c0 5180 	rsb	r1, r0, #4096	; 0x1000
 20397f4:	4440      	add	r0, r8
 20397f6:	eba1 0108 	sub.w	r1, r1, r8
 20397fa:	4428      	add	r0, r5
 20397fc:	f002 fafc 	bl	203bdf8 <__memcpy_s_veneer>
 2039800:	4604      	mov	r4, r0
 2039802:	bb58      	cbnz	r0, 203985c <nv_write_update_data+0xac>
 2039804:	68eb      	ldr	r3, [r5, #12]
 2039806:	f640 72f8 	movw	r2, #4088	; 0xff8
 203980a:	3301      	adds	r3, #1
 203980c:	60eb      	str	r3, [r5, #12]
 203980e:	f105 0108 	add.w	r1, r5, #8
 2039812:	f7ff fe67 	bl	20394e4 <uapi_crc32>
 2039816:	4603      	mov	r3, r0
 2039818:	f44f 5180 	mov.w	r1, #4096	; 0x1000
 203981c:	6870      	ldr	r0, [r6, #4]
 203981e:	606b      	str	r3, [r5, #4]
 2039820:	f7ff fd8e 	bl	2039340 <uapi_flash_erase>
 2039824:	4684      	mov	ip, r0
 2039826:	4623      	mov	r3, r4
 2039828:	462a      	mov	r2, r5
 203982a:	f44f 5180 	mov.w	r1, #4096	; 0x1000
 203982e:	6870      	ldr	r0, [r6, #4]
 2039830:	4664      	mov	r4, ip
 2039832:	f7ff fd9d 	bl	2039370 <uapi_flash_write>
 2039836:	eb14 0c00 	adds.w	ip, r4, r0
 203983a:	d10f      	bne.n	203985c <nv_write_update_data+0xac>
 203983c:	4660      	mov	r0, ip
 203983e:	9b08      	ldr	r3, [sp, #32]
 2039840:	601d      	str	r5, [r3, #0]
 2039842:	b002      	add	sp, #8
 2039844:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 2039848:	8878      	ldrh	r0, [r7, #2]
 203984a:	4643      	mov	r3, r8
 203984c:	f5c0 5180 	rsb	r1, r0, #4096	; 0x1000
 2039850:	6822      	ldr	r2, [r4, #0]
 2039852:	4428      	add	r0, r5
 2039854:	f002 fad0 	bl	203bdf8 <__memcpy_s_veneer>
 2039858:	2800      	cmp	r0, #0
 203985a:	d0c5      	beq.n	20397e8 <nv_write_update_data+0x38>
 203985c:	f04f 3cff 	mov.w	ip, #4294967295	; 0xffffffff
 2039860:	4660      	mov	r0, ip
 2039862:	b002      	add	sp, #8
 2039864:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}

02039868 <nv_write_para_check.constprop.0>:
 2039868:	b180      	cbz	r0, 203988c <nv_write_para_check.constprop.0+0x24>
 203986a:	3901      	subs	r1, #1
 203986c:	b2c9      	uxtb	r1, r1
 203986e:	29fb      	cmp	r1, #251	; 0xfb
 2039870:	d80c      	bhi.n	203988c <nv_write_para_check.constprop.0+0x24>
 2039872:	4908      	ldr	r1, [pc, #32]	; (2039894 <nv_write_para_check.constprop.0+0x2c>)
 2039874:	eb02 0242 	add.w	r2, r2, r2, lsl #1
 2039878:	eb01 02c2 	add.w	r2, r1, r2, lsl #3
 203987c:	7c11      	ldrb	r1, [r2, #16]
 203987e:	f241 30ec 	movw	r0, #5100	; 0x13ec
 2039882:	2901      	cmp	r1, #1
 2039884:	bf08      	it	eq
 2039886:	2000      	moveq	r0, #0
 2039888:	601a      	str	r2, [r3, #0]
 203988a:	4770      	bx	lr
 203988c:	f241 30ee 	movw	r0, #5102	; 0x13ee
 2039890:	4770      	bx	lr
 2039892:	bf00      	nop
 2039894:	0203c360 	.word	0x0203c360

02039898 <nv_find_item.constprop.0>:
 2039898:	4b0e      	ldr	r3, [pc, #56]	; (20398d4 <nv_find_item.constprop.0+0x3c>)
 203989a:	eb01 0141 	add.w	r1, r1, r1, lsl #1
 203989e:	eb03 03c1 	add.w	r3, r3, r1, lsl #3
 20398a2:	b430      	push	{r4, r5}
 20398a4:	8a5d      	ldrh	r5, [r3, #18]
 20398a6:	695c      	ldr	r4, [r3, #20]
 20398a8:	b18d      	cbz	r5, 20398ce <nv_find_item.constprop.0+0x36>
 20398aa:	4601      	mov	r1, r0
 20398ac:	2300      	movs	r3, #0
 20398ae:	4620      	mov	r0, r4
 20398b0:	e003      	b.n	20398ba <nv_find_item.constprop.0+0x22>
 20398b2:	429d      	cmp	r5, r3
 20398b4:	f100 0004 	add.w	r0, r0, #4
 20398b8:	d006      	beq.n	20398c8 <nv_find_item.constprop.0+0x30>
 20398ba:	f814 2023 	ldrb.w	r2, [r4, r3, lsl #2]
 20398be:	3301      	adds	r3, #1
 20398c0:	428a      	cmp	r2, r1
 20398c2:	d1f6      	bne.n	20398b2 <nv_find_item.constprop.0+0x1a>
 20398c4:	bc30      	pop	{r4, r5}
 20398c6:	4770      	bx	lr
 20398c8:	2000      	movs	r0, #0
 20398ca:	bc30      	pop	{r4, r5}
 20398cc:	4770      	bx	lr
 20398ce:	4628      	mov	r0, r5
 20398d0:	e7f8      	b.n	20398c4 <nv_find_item.constprop.0+0x2c>
 20398d2:	bf00      	nop
 20398d4:	0203c360 	.word	0x0203c360

020398d8 <nv_repair_file.constprop.0>:
 20398d8:	b538      	push	{r3, r4, r5, lr}
 20398da:	f44f 5380 	mov.w	r3, #4096	; 0x1000
 20398de:	4615      	mov	r5, r2
 20398e0:	460a      	mov	r2, r1
 20398e2:	4619      	mov	r1, r3
 20398e4:	4604      	mov	r4, r0
 20398e6:	f002 fa87 	bl	203bdf8 <__memcpy_s_veneer>
 20398ea:	b100      	cbz	r0, 20398ee <nv_repair_file.constprop.0+0x16>
 20398ec:	bd38      	pop	{r3, r4, r5, pc}
 20398ee:	f640 72f8 	movw	r2, #4088	; 0xff8
 20398f2:	60e5      	str	r5, [r4, #12]
 20398f4:	f104 0108 	add.w	r1, r4, #8
 20398f8:	f7ff fdf4 	bl	20394e4 <uapi_crc32>
 20398fc:	6060      	str	r0, [r4, #4]
 20398fe:	bd38      	pop	{r3, r4, r5, pc}

02039900 <nv_init_data_check_once_fail>:
 2039900:	b570      	push	{r4, r5, r6, lr}
 2039902:	460e      	mov	r6, r1
 2039904:	4605      	mov	r5, r0
 2039906:	4611      	mov	r1, r2
 2039908:	9c04      	ldr	r4, [sp, #16]
 203990a:	b1ae      	cbz	r6, 2039938 <nv_init_data_check_once_fail+0x38>
 203990c:	f503 5280 	add.w	r2, r3, #4096	; 0x1000
 2039910:	e9c4 2300 	strd	r2, r3, [r4]
 2039914:	68ca      	ldr	r2, [r1, #12]
 2039916:	894b      	ldrh	r3, [r1, #10]
 2039918:	3201      	adds	r2, #1
 203991a:	8263      	strh	r3, [r4, #18]
 203991c:	f101 031c 	add.w	r3, r1, #28
 2039920:	4628      	mov	r0, r5
 2039922:	60a2      	str	r2, [r4, #8]
 2039924:	6163      	str	r3, [r4, #20]
 2039926:	f7ff ffd7 	bl	20398d8 <nv_repair_file.constprop.0>
 203992a:	f44f 5180 	mov.w	r1, #4096	; 0x1000
 203992e:	6860      	ldr	r0, [r4, #4]
 2039930:	f7ff fd06 	bl	2039340 <uapi_flash_erase>
 2039934:	b138      	cbz	r0, 2039946 <nv_init_data_check_once_fail+0x46>
 2039936:	bd70      	pop	{r4, r5, r6, pc}
 2039938:	460d      	mov	r5, r1
 203993a:	f503 5180 	add.w	r1, r3, #4096	; 0x1000
 203993e:	e9c4 3100 	strd	r3, r1, [r4]
 2039942:	4601      	mov	r1, r0
 2039944:	e7e6      	b.n	2039914 <nv_init_data_check_once_fail+0x14>
 2039946:	4603      	mov	r3, r0
 2039948:	462a      	mov	r2, r5
 203994a:	f44f 5180 	mov.w	r1, #4096	; 0x1000
 203994e:	6860      	ldr	r0, [r4, #4]
 2039950:	f7ff fd0e 	bl	2039370 <uapi_flash_write>
 2039954:	2800      	cmp	r0, #0
 2039956:	d1ee      	bne.n	2039936 <nv_init_data_check_once_fail+0x36>
 2039958:	e9d4 2300 	ldrd	r2, r3, [r4]
 203995c:	e9c4 3200 	strd	r3, r2, [r4]
 2039960:	bd70      	pop	{r4, r5, r6, pc}
 2039962:	bf00      	nop

02039964 <nv_init_data_check>:
 2039964:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
 2039968:	9c08      	ldr	r4, [sp, #32]
 203996a:	4607      	mov	r7, r0
 203996c:	6820      	ldr	r0, [r4, #0]
 203996e:	b340      	cbz	r0, 20399c2 <nv_init_data_check+0x5e>
 2039970:	461e      	mov	r6, r3
 2039972:	6863      	ldr	r3, [r4, #4]
 2039974:	b32b      	cbz	r3, 20399c2 <nv_init_data_check+0x5e>
 2039976:	4689      	mov	r9, r1
 2039978:	f44f 5180 	mov.w	r1, #4096	; 0x1000
 203997c:	4615      	mov	r5, r2
 203997e:	f7ff feb9 	bl	20396f4 <nv_check_file>
 2039982:	462a      	mov	r2, r5
 2039984:	4682      	mov	sl, r0
 2039986:	f44f 5180 	mov.w	r1, #4096	; 0x1000
 203998a:	6860      	ldr	r0, [r4, #4]
 203998c:	f7ff feb2 	bl	20396f4 <nv_check_file>
 2039990:	4680      	mov	r8, r0
 2039992:	f1ba 0f00 	cmp.w	sl, #0
 2039996:	d009      	beq.n	20399ac <nv_init_data_check+0x48>
 2039998:	b9c0      	cbnz	r0, 20399cc <nv_init_data_check+0x68>
 203999a:	e9d4 0200 	ldrd	r0, r2, [r4]
 203999e:	463b      	mov	r3, r7
 20399a0:	4651      	mov	r1, sl
 20399a2:	9608      	str	r6, [sp, #32]
 20399a4:	e8bd 47f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
 20399a8:	f7ff bfaa 	b.w	2039900 <nv_init_data_check_once_fail>
 20399ac:	2800      	cmp	r0, #0
 20399ae:	d1f4      	bne.n	203999a <nv_init_data_check+0x36>
 20399b0:	e9d4 0100 	ldrd	r0, r1, [r4]
 20399b4:	4633      	mov	r3, r6
 20399b6:	463a      	mov	r2, r7
 20399b8:	f7ff fe66 	bl	2039688 <nv_init_data_check_all_success>
 20399bc:	4640      	mov	r0, r8
 20399be:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
 20399c2:	f04f 38ff 	mov.w	r8, #4294967295	; 0xffffffff
 20399c6:	4640      	mov	r0, r8
 20399c8:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
 20399cc:	6824      	ldr	r4, [r4, #0]
 20399ce:	4633      	mov	r3, r6
 20399d0:	462a      	mov	r2, r5
 20399d2:	4649      	mov	r1, r9
 20399d4:	4638      	mov	r0, r7
 20399d6:	9408      	str	r4, [sp, #32]
 20399d8:	e8bd 47f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
 20399dc:	f7ff bece 	b.w	203977c <nv_init_data_check_all_fail>

020399e0 <uint_2_multiply.constprop.0>:
 20399e0:	4603      	mov	r3, r0
 20399e2:	009b      	lsls	r3, r3, #2
 20399e4:	2001      	movs	r0, #1
 20399e6:	600b      	str	r3, [r1, #0]
 20399e8:	4770      	bx	lr
 20399ea:	bf00      	nop

020399ec <nv_init_index_space>:
 20399ec:	b530      	push	{r4, r5, lr}
 20399ee:	b083      	sub	sp, #12
 20399f0:	4604      	mov	r4, r0
 20399f2:	a901      	add	r1, sp, #4
 20399f4:	8a40      	ldrh	r0, [r0, #18]
 20399f6:	f7ff fff3 	bl	20399e0 <uint_2_multiply.constprop.0>
 20399fa:	b180      	cbz	r0, 2039a1e <nv_init_index_space+0x32>
 20399fc:	9801      	ldr	r0, [sp, #4]
 20399fe:	f002 f993 	bl	203bd28 <__boot_malloc_veneer>
 2039a02:	4605      	mov	r5, r0
 2039a04:	b178      	cbz	r0, 2039a26 <nv_init_index_space+0x3a>
 2039a06:	8a63      	ldrh	r3, [r4, #18]
 2039a08:	6962      	ldr	r2, [r4, #20]
 2039a0a:	9901      	ldr	r1, [sp, #4]
 2039a0c:	009b      	lsls	r3, r3, #2
 2039a0e:	f002 f9f3 	bl	203bdf8 <__memcpy_s_veneer>
 2039a12:	2301      	movs	r3, #1
 2039a14:	2000      	movs	r0, #0
 2039a16:	6165      	str	r5, [r4, #20]
 2039a18:	7423      	strb	r3, [r4, #16]
 2039a1a:	b003      	add	sp, #12
 2039a1c:	bd30      	pop	{r4, r5, pc}
 2039a1e:	f04f 30ff 	mov.w	r0, #4294967295	; 0xffffffff
 2039a22:	b003      	add	sp, #12
 2039a24:	bd30      	pop	{r4, r5, pc}
 2039a26:	f241 30ef 	movw	r0, #5103	; 0x13ef
 2039a2a:	b003      	add	sp, #12
 2039a2c:	bd30      	pop	{r4, r5, pc}
 2039a2e:	bf00      	nop

02039a30 <nv_init_common>:
 2039a30:	2300      	movs	r3, #0
 2039a32:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
 2039a36:	4e26      	ldr	r6, [pc, #152]	; (2039ad0 <nv_init_common+0xa0>)
 2039a38:	b086      	sub	sp, #24
 2039a3a:	e9cd 3302 	strd	r3, r3, [sp, #8]
 2039a3e:	eb02 0342 	add.w	r3, r2, r2, lsl #1
 2039a42:	eb06 06c3 	add.w	r6, r6, r3, lsl #3
 2039a46:	7c33      	ldrb	r3, [r6, #16]
 2039a48:	2b01      	cmp	r3, #1
 2039a4a:	d02a      	beq.n	2039aa2 <nv_init_common+0x72>
 2039a4c:	4607      	mov	r7, r0
 2039a4e:	4688      	mov	r8, r1
 2039a50:	4601      	mov	r1, r0
 2039a52:	a802      	add	r0, sp, #8
 2039a54:	4615      	mov	r5, r2
 2039a56:	f7ff fe2f 	bl	20396b8 <nv_init_data_prepare>
 2039a5a:	4604      	mov	r4, r0
 2039a5c:	b138      	cbz	r0, 2039a6e <nv_init_common+0x3e>
 2039a5e:	9802      	ldr	r0, [sp, #8]
 2039a60:	b108      	cbz	r0, 2039a66 <nv_init_common+0x36>
 2039a62:	f002 f969 	bl	203bd38 <__boot_free_veneer>
 2039a66:	4620      	mov	r0, r4
 2039a68:	b006      	add	sp, #24
 2039a6a:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 2039a6e:	f507 5180 	add.w	r1, r7, #4096	; 0x1000
 2039a72:	a803      	add	r0, sp, #12
 2039a74:	f7ff fe20 	bl	20396b8 <nv_init_data_prepare>
 2039a78:	4604      	mov	r4, r0
 2039a7a:	bb08      	cbnz	r0, 2039ac0 <nv_init_common+0x90>
 2039a7c:	9b02      	ldr	r3, [sp, #8]
 2039a7e:	462a      	mov	r2, r5
 2039a80:	9304      	str	r3, [sp, #16]
 2039a82:	9b03      	ldr	r3, [sp, #12]
 2039a84:	4641      	mov	r1, r8
 2039a86:	9305      	str	r3, [sp, #20]
 2039a88:	ab04      	add	r3, sp, #16
 2039a8a:	9300      	str	r3, [sp, #0]
 2039a8c:	4638      	mov	r0, r7
 2039a8e:	4633      	mov	r3, r6
 2039a90:	f7ff ff68 	bl	2039964 <nv_init_data_check>
 2039a94:	4604      	mov	r4, r0
 2039a96:	b150      	cbz	r0, 2039aae <nv_init_common+0x7e>
 2039a98:	9803      	ldr	r0, [sp, #12]
 2039a9a:	f002 f94d 	bl	203bd38 <__boot_free_veneer>
 2039a9e:	9802      	ldr	r0, [sp, #8]
 2039aa0:	e7df      	b.n	2039a62 <nv_init_common+0x32>
 2039aa2:	f241 34ed 	movw	r4, #5101	; 0x13ed
 2039aa6:	4620      	mov	r0, r4
 2039aa8:	b006      	add	sp, #24
 2039aaa:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 2039aae:	8a73      	ldrh	r3, [r6, #18]
 2039ab0:	f5b3 7f80 	cmp.w	r3, #256	; 0x100
 2039ab4:	d8f0      	bhi.n	2039a98 <nv_init_common+0x68>
 2039ab6:	4630      	mov	r0, r6
 2039ab8:	f7ff ff98 	bl	20399ec <nv_init_index_space>
 2039abc:	4604      	mov	r4, r0
 2039abe:	e7eb      	b.n	2039a98 <nv_init_common+0x68>
 2039ac0:	9802      	ldr	r0, [sp, #8]
 2039ac2:	f002 f939 	bl	203bd38 <__boot_free_veneer>
 2039ac6:	9803      	ldr	r0, [sp, #12]
 2039ac8:	2800      	cmp	r0, #0
 2039aca:	d1ca      	bne.n	2039a62 <nv_init_common+0x32>
 2039acc:	e7cb      	b.n	2039a66 <nv_init_common+0x36>
 2039ace:	bf00      	nop
 2039ad0:	0203c360 	.word	0x0203c360

02039ad4 <nv_read_common>:
 2039ad4:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 2039ad8:	f04f 0800 	mov.w	r8, #0
 2039adc:	b083      	sub	sp, #12
 2039ade:	f8cd 8004 	str.w	r8, [sp, #4]
 2039ae2:	b309      	cbz	r1, 2039b28 <nv_read_common+0x54>
 2039ae4:	4615      	mov	r5, r2
 2039ae6:	b1fa      	cbz	r2, 2039b28 <nv_read_common+0x54>
 2039ae8:	461c      	mov	r4, r3
 2039aea:	f8df b0a8 	ldr.w	fp, [pc, #168]	; 2039b94 <nv_read_common+0xc0>
 2039aee:	eb03 0343 	add.w	r3, r3, r3, lsl #1
 2039af2:	eb0b 03c3 	add.w	r3, fp, r3, lsl #3
 2039af6:	f893 a010 	ldrb.w	sl, [r3, #16]
 2039afa:	ea4f 0944 	mov.w	r9, r4, lsl #1
 2039afe:	f1ba 0f01 	cmp.w	sl, #1
 2039b02:	d10b      	bne.n	2039b1c <nv_read_common+0x48>
 2039b04:	460e      	mov	r6, r1
 2039b06:	4621      	mov	r1, r4
 2039b08:	f7ff fec6 	bl	2039898 <nv_find_item.constprop.0>
 2039b0c:	4607      	mov	r7, r0
 2039b0e:	b988      	cbnz	r0, 2039b34 <nv_read_common+0x60>
 2039b10:	f241 34f3 	movw	r4, #5107	; 0x13f3
 2039b14:	4620      	mov	r0, r4
 2039b16:	b003      	add	sp, #12
 2039b18:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 2039b1c:	f241 34ec 	movw	r4, #5100	; 0x13ec
 2039b20:	4620      	mov	r0, r4
 2039b22:	b003      	add	sp, #12
 2039b24:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 2039b28:	f241 34ee 	movw	r4, #5102	; 0x13ee
 2039b2c:	4620      	mov	r0, r4
 2039b2e:	b003      	add	sp, #12
 2039b30:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 2039b34:	7843      	ldrb	r3, [r0, #1]
 2039b36:	42ab      	cmp	r3, r5
 2039b38:	d003      	beq.n	2039b42 <nv_read_common+0x6e>
 2039b3a:	429d      	cmp	r5, r3
 2039b3c:	bf28      	it	cs
 2039b3e:	461d      	movcs	r5, r3
 2039b40:	46c2      	mov	sl, r8
 2039b42:	f04f 0803 	mov.w	r8, #3
 2039b46:	44a1      	add	r9, r4
 2039b48:	eb0b 09c9 	add.w	r9, fp, r9, lsl #3
 2039b4c:	8878      	ldrh	r0, [r7, #2]
 2039b4e:	f8d9 3000 	ldr.w	r3, [r9]
 2039b52:	4632      	mov	r2, r6
 2039b54:	4629      	mov	r1, r5
 2039b56:	4418      	add	r0, r3
 2039b58:	f7ff fc2c 	bl	20393b4 <uapi_flash_read>
 2039b5c:	4604      	mov	r4, r0
 2039b5e:	b998      	cbnz	r0, 2039b88 <nv_read_common+0xb4>
 2039b60:	f1ba 0f01 	cmp.w	sl, #1
 2039b64:	d1d6      	bne.n	2039b14 <nv_read_common+0x40>
 2039b66:	8878      	ldrh	r0, [r7, #2]
 2039b68:	f8d9 3000 	ldr.w	r3, [r9]
 2039b6c:	2104      	movs	r1, #4
 2039b6e:	4418      	add	r0, r3
 2039b70:	4428      	add	r0, r5
 2039b72:	aa01      	add	r2, sp, #4
 2039b74:	f7ff fc1e 	bl	20393b4 <uapi_flash_read>
 2039b78:	b930      	cbnz	r0, 2039b88 <nv_read_common+0xb4>
 2039b7a:	462a      	mov	r2, r5
 2039b7c:	4631      	mov	r1, r6
 2039b7e:	f7ff fcb1 	bl	20394e4 <uapi_crc32>
 2039b82:	9b01      	ldr	r3, [sp, #4]
 2039b84:	4298      	cmp	r0, r3
 2039b86:	d0c5      	beq.n	2039b14 <nv_read_common+0x40>
 2039b88:	f1b8 0801 	subs.w	r8, r8, #1
 2039b8c:	d1de      	bne.n	2039b4c <nv_read_common+0x78>
 2039b8e:	f241 34f4 	movw	r4, #5108	; 0x13f4
 2039b92:	e7bf      	b.n	2039b14 <nv_read_common+0x40>
 2039b94:	0203c360 	.word	0x0203c360

02039b98 <nv_write_cmp_readback>:
 2039b98:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
 2039b9c:	4605      	mov	r5, r0
 2039b9e:	20fc      	movs	r0, #252	; 0xfc
 2039ba0:	460f      	mov	r7, r1
 2039ba2:	4616      	mov	r6, r2
 2039ba4:	4698      	mov	r8, r3
 2039ba6:	f002 f8bf 	bl	203bd28 <__boot_malloc_veneer>
 2039baa:	b1e0      	cbz	r0, 2039be6 <nv_write_cmp_readback+0x4e>
 2039bac:	4604      	mov	r4, r0
 2039bae:	4643      	mov	r3, r8
 2039bb0:	4628      	mov	r0, r5
 2039bb2:	4632      	mov	r2, r6
 2039bb4:	4621      	mov	r1, r4
 2039bb6:	f7ff ff8d 	bl	2039ad4 <nv_read_common>
 2039bba:	4605      	mov	r5, r0
 2039bbc:	b968      	cbnz	r0, 2039bda <nv_write_cmp_readback+0x42>
 2039bbe:	4632      	mov	r2, r6
 2039bc0:	4621      	mov	r1, r4
 2039bc2:	4638      	mov	r0, r7
 2039bc4:	f002 f894 	bl	203bcf0 <__memcmp_veneer>
 2039bc8:	b138      	cbz	r0, 2039bda <nv_write_cmp_readback+0x42>
 2039bca:	f241 35f3 	movw	r5, #5107	; 0x13f3
 2039bce:	4620      	mov	r0, r4
 2039bd0:	f002 f8b2 	bl	203bd38 <__boot_free_veneer>
 2039bd4:	4628      	mov	r0, r5
 2039bd6:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 2039bda:	4620      	mov	r0, r4
 2039bdc:	f002 f8ac 	bl	203bd38 <__boot_free_veneer>
 2039be0:	4628      	mov	r0, r5
 2039be2:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 2039be6:	f241 35ef 	movw	r5, #5103	; 0x13ef
 2039bea:	4628      	mov	r0, r5
 2039bec:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}

02039bf0 <nv_write_common>:
 2039bf0:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
 2039bf4:	461d      	mov	r5, r3
 2039bf6:	4688      	mov	r8, r1
 2039bf8:	4617      	mov	r7, r2
 2039bfa:	2400      	movs	r4, #0
 2039bfc:	b087      	sub	sp, #28
 2039bfe:	2d01      	cmp	r5, #1
 2039c00:	4681      	mov	r9, r0
 2039c02:	462a      	mov	r2, r5
 2039c04:	4639      	mov	r1, r7
 2039c06:	4640      	mov	r0, r8
 2039c08:	ab02      	add	r3, sp, #8
 2039c0a:	bf0c      	ite	eq
 2039c0c:	2602      	moveq	r6, #2
 2039c0e:	2601      	movne	r6, #1
 2039c10:	e9cd 4402 	strd	r4, r4, [sp, #8]
 2039c14:	f7ff fe28 	bl	2039868 <nv_write_para_check.constprop.0>
 2039c18:	b110      	cbz	r0, 2039c20 <nv_write_common+0x30>
 2039c1a:	b007      	add	sp, #28
 2039c1c:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
 2039c20:	462b      	mov	r3, r5
 2039c22:	463a      	mov	r2, r7
 2039c24:	4641      	mov	r1, r8
 2039c26:	4648      	mov	r0, r9
 2039c28:	f7ff ffb6 	bl	2039b98 <nv_write_cmp_readback>
 2039c2c:	f241 33f3 	movw	r3, #5107	; 0x13f3
 2039c30:	4298      	cmp	r0, r3
 2039c32:	4604      	mov	r4, r0
 2039c34:	d148      	bne.n	2039cc8 <nv_write_common+0xd8>
 2039c36:	4629      	mov	r1, r5
 2039c38:	4648      	mov	r0, r9
 2039c3a:	f7ff fe2d 	bl	2039898 <nv_find_item.constprop.0>
 2039c3e:	4605      	mov	r5, r0
 2039c40:	2800      	cmp	r0, #0
 2039c42:	d041      	beq.n	2039cc8 <nv_write_common+0xd8>
 2039c44:	f8cd 8010 	str.w	r8, [sp, #16]
 2039c48:	f88d 7014 	strb.w	r7, [sp, #20]
 2039c4c:	7842      	ldrb	r2, [r0, #1]
 2039c4e:	42ba      	cmp	r2, r7
 2039c50:	d004      	beq.n	2039c5c <nv_write_common+0x6c>
 2039c52:	42ba      	cmp	r2, r7
 2039c54:	bf28      	it	cs
 2039c56:	463a      	movcs	r2, r7
 2039c58:	f88d 2014 	strb.w	r2, [sp, #20]
 2039c5c:	f44f 5080 	mov.w	r0, #4096	; 0x1000
 2039c60:	f002 f862 	bl	203bd28 <__boot_malloc_veneer>
 2039c64:	f10d 080c 	add.w	r8, sp, #12
 2039c68:	4607      	mov	r7, r0
 2039c6a:	9902      	ldr	r1, [sp, #8]
 2039c6c:	e00c      	b.n	2039c88 <nv_write_common+0x98>
 2039c6e:	e9dd 1302 	ldrd	r1, r3, [sp, #8]
 2039c72:	e9d1 2400 	ldrd	r2, r4, [r1]
 2039c76:	e9c1 4200 	strd	r4, r2, [r1]
 2039c7a:	b113      	cbz	r3, 2039c82 <nv_write_common+0x92>
 2039c7c:	68db      	ldr	r3, [r3, #12]
 2039c7e:	9003      	str	r0, [sp, #12]
 2039c80:	608b      	str	r3, [r1, #8]
 2039c82:	2e01      	cmp	r6, #1
 2039c84:	d024      	beq.n	2039cd0 <nv_write_common+0xe0>
 2039c86:	2601      	movs	r6, #1
 2039c88:	2400      	movs	r4, #0
 2039c8a:	462b      	mov	r3, r5
 2039c8c:	463a      	mov	r2, r7
 2039c8e:	a804      	add	r0, sp, #16
 2039c90:	f8cd 8000 	str.w	r8, [sp]
 2039c94:	f7ff fd8c 	bl	20397b0 <nv_write_update_data>
 2039c98:	2800      	cmp	r0, #0
 2039c9a:	d0e8      	beq.n	2039c6e <nv_write_common+0x7e>
 2039c9c:	3401      	adds	r4, #1
 2039c9e:	2c03      	cmp	r4, #3
 2039ca0:	d11d      	bne.n	2039cde <nv_write_common+0xee>
 2039ca2:	e9dd 3202 	ldrd	r3, r2, [sp, #8]
 2039ca6:	e9d3 0100 	ldrd	r0, r1, [r3]
 2039caa:	e9c3 1000 	strd	r1, r0, [r3]
 2039cae:	b11a      	cbz	r2, 2039cb8 <nv_write_common+0xc8>
 2039cb0:	2100      	movs	r1, #0
 2039cb2:	68d2      	ldr	r2, [r2, #12]
 2039cb4:	9103      	str	r1, [sp, #12]
 2039cb6:	609a      	str	r2, [r3, #8]
 2039cb8:	4638      	mov	r0, r7
 2039cba:	f002 f83d 	bl	203bd38 <__boot_free_veneer>
 2039cbe:	f241 30f4 	movw	r0, #5108	; 0x13f4
 2039cc2:	b007      	add	sp, #28
 2039cc4:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
 2039cc8:	4620      	mov	r0, r4
 2039cca:	b007      	add	sp, #28
 2039ccc:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
 2039cd0:	4638      	mov	r0, r7
 2039cd2:	f002 f831 	bl	203bd38 <__boot_free_veneer>
 2039cd6:	2000      	movs	r0, #0
 2039cd8:	b007      	add	sp, #28
 2039cda:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
 2039cde:	9902      	ldr	r1, [sp, #8]
 2039ce0:	e7d3      	b.n	2039c8a <nv_write_common+0x9a>
 2039ce2:	bf00      	nop

02039ce4 <uapi_factory_nv_init>:
 2039ce4:	2201      	movs	r2, #1
 2039ce6:	2100      	movs	r1, #0
 2039ce8:	f7ff bea2 	b.w	2039a30 <nv_init_common>

02039cec <uapi_factory_nv_read>:
 2039cec:	2301      	movs	r3, #1
 2039cee:	f7ff bef1 	b.w	2039ad4 <nv_read_common>
 2039cf2:	bf00      	nop

02039cf4 <uapi_factory_nv_write>:
 2039cf4:	2301      	movs	r3, #1
 2039cf6:	f7ff bf7b 	b.w	2039bf0 <nv_write_common>
 2039cfa:	bf00      	nop

02039cfc <uapi_temp_nv_init>:
 2039cfc:	2202      	movs	r2, #2
 2039cfe:	f7ff be97 	b.w	2039a30 <nv_init_common>
 2039d02:	bf00      	nop

02039d04 <uapi_common_nv_read>:
 2039d04:	f7ff bee6 	b.w	2039ad4 <nv_read_common>

02039d08 <uapi_common_nv_write>:
 2039d08:	f7ff bf72 	b.w	2039bf0 <nv_write_common>

02039d0c <uapi_nv_check_file>:
 2039d0c:	2300      	movs	r3, #0
 2039d0e:	b570      	push	{r4, r5, r6, lr}
 2039d10:	b088      	sub	sp, #32
 2039d12:	211c      	movs	r1, #28
 2039d14:	aa01      	add	r2, sp, #4
 2039d16:	4605      	mov	r5, r0
 2039d18:	e9cd 3302 	strd	r3, r3, [sp, #8]
 2039d1c:	e9cd 3304 	strd	r3, r3, [sp, #16]
 2039d20:	e9cd 3306 	strd	r3, r3, [sp, #24]
 2039d24:	9301      	str	r3, [sp, #4]
 2039d26:	f7ff fb45 	bl	20393b4 <uapi_flash_read>
 2039d2a:	4604      	mov	r4, r0
 2039d2c:	b110      	cbz	r0, 2039d34 <uapi_nv_check_file+0x28>
 2039d2e:	4620      	mov	r0, r4
 2039d30:	b008      	add	sp, #32
 2039d32:	bd70      	pop	{r4, r5, r6, pc}
 2039d34:	9806      	ldr	r0, [sp, #24]
 2039d36:	f001 fff7 	bl	203bd28 <__boot_malloc_veneer>
 2039d3a:	4606      	mov	r6, r0
 2039d3c:	b190      	cbz	r0, 2039d64 <uapi_nv_check_file+0x58>
 2039d3e:	4602      	mov	r2, r0
 2039d40:	9906      	ldr	r1, [sp, #24]
 2039d42:	4628      	mov	r0, r5
 2039d44:	f7ff fb36 	bl	20393b4 <uapi_flash_read>
 2039d48:	4604      	mov	r4, r0
 2039d4a:	b928      	cbnz	r0, 2039d58 <uapi_nv_check_file+0x4c>
 2039d4c:	4602      	mov	r2, r0
 2039d4e:	9906      	ldr	r1, [sp, #24]
 2039d50:	4630      	mov	r0, r6
 2039d52:	f7ff fccf 	bl	20396f4 <nv_check_file>
 2039d56:	4604      	mov	r4, r0
 2039d58:	4630      	mov	r0, r6
 2039d5a:	f001 ffed 	bl	203bd38 <__boot_free_veneer>
 2039d5e:	4620      	mov	r0, r4
 2039d60:	b008      	add	sp, #32
 2039d62:	bd70      	pop	{r4, r5, r6, pc}
 2039d64:	f241 34ef 	movw	r4, #5103	; 0x13ef
 2039d68:	e7e1      	b.n	2039d2e <uapi_nv_check_file+0x22>
 2039d6a:	bf00      	nop

02039d6c <uapi_nv_get_ver>:
 2039d6c:	b151      	cbz	r1, 2039d84 <uapi_nv_get_ver+0x18>
 2039d6e:	460a      	mov	r2, r1
 2039d70:	b508      	push	{r3, lr}
 2039d72:	2104      	movs	r1, #4
 2039d74:	3010      	adds	r0, #16
 2039d76:	f7ff fb1d 	bl	20393b4 <uapi_flash_read>
 2039d7a:	3800      	subs	r0, #0
 2039d7c:	bf18      	it	ne
 2039d7e:	2001      	movne	r0, #1
 2039d80:	4240      	negs	r0, r0
 2039d82:	bd08      	pop	{r3, pc}
 2039d84:	f241 30ee 	movw	r0, #5102	; 0x13ee
 2039d88:	4770      	bx	lr
 2039d8a:	bf00      	nop

02039d8c <flash_partition_get_ver>:
 2039d8c:	b570      	push	{r4, r5, r6, lr}
 2039d8e:	4606      	mov	r6, r0
 2039d90:	203c      	movs	r0, #60	; 0x3c
 2039d92:	f001 ffc9 	bl	203bd28 <__boot_malloc_veneer>
 2039d96:	4604      	mov	r4, r0
 2039d98:	2301      	movs	r3, #1
 2039d9a:	223c      	movs	r2, #60	; 0x3c
 2039d9c:	2004      	movs	r0, #4
 2039d9e:	4621      	mov	r1, r4
 2039da0:	f7ff ffb0 	bl	2039d04 <uapi_common_nv_read>
 2039da4:	4605      	mov	r5, r0
 2039da6:	b908      	cbnz	r0, 2039dac <flash_partition_get_ver+0x20>
 2039da8:	6823      	ldr	r3, [r4, #0]
 2039daa:	6033      	str	r3, [r6, #0]
 2039dac:	4620      	mov	r0, r4
 2039dae:	f001 ffc3 	bl	203bd38 <__boot_free_veneer>
 2039db2:	4628      	mov	r0, r5
 2039db4:	bd70      	pop	{r4, r5, r6, pc}
 2039db6:	bf00      	nop

02039db8 <flash_partition_check_ver>:
 2039db8:	7803      	ldrb	r3, [r0, #0]
 2039dba:	07db      	lsls	r3, r3, #31
 2039dbc:	d520      	bpl.n	2039e00 <flash_partition_check_ver+0x48>
 2039dbe:	b510      	push	{r4, lr}
 2039dc0:	b082      	sub	sp, #8
 2039dc2:	4604      	mov	r4, r0
 2039dc4:	a801      	add	r0, sp, #4
 2039dc6:	f7ff ffe1 	bl	2039d8c <flash_partition_get_ver>
 2039dca:	b9b0      	cbnz	r0, 2039dfa <flash_partition_check_ver+0x42>
 2039dcc:	f89d 3005 	ldrb.w	r3, [sp, #5]
 2039dd0:	f013 0ff8 	tst.w	r3, #248	; 0xf8
 2039dd4:	d011      	beq.n	2039dfa <flash_partition_check_ver+0x42>
 2039dd6:	8862      	ldrh	r2, [r4, #2]
 2039dd8:	f8bd 1006 	ldrh.w	r1, [sp, #6]
 2039ddc:	4211      	tst	r1, r2
 2039dde:	d00c      	beq.n	2039dfa <flash_partition_check_ver+0x42>
 2039de0:	7862      	ldrb	r2, [r4, #1]
 2039de2:	08d1      	lsrs	r1, r2, #3
 2039de4:	ebb1 0fd3 	cmp.w	r1, r3, lsr #3
 2039de8:	d307      	bcc.n	2039dfa <flash_partition_check_ver+0x42>
 2039dea:	4053      	eors	r3, r2
 2039dec:	f013 0ff8 	tst.w	r3, #248	; 0xf8
 2039df0:	bf14      	ite	ne
 2039df2:	2001      	movne	r0, #1
 2039df4:	2000      	moveq	r0, #0
 2039df6:	b002      	add	sp, #8
 2039df8:	bd10      	pop	{r4, pc}
 2039dfa:	2002      	movs	r0, #2
 2039dfc:	b002      	add	sp, #8
 2039dfe:	bd10      	pop	{r4, pc}
 2039e00:	2002      	movs	r0, #2
 2039e02:	4770      	bx	lr

02039e04 <uapi_flash_partition_refresh_free_memery>:
 2039e04:	b538      	push	{r3, r4, r5, lr}
 2039e06:	460d      	mov	r5, r1
 2039e08:	4604      	mov	r4, r0
 2039e0a:	4610      	mov	r0, r2
 2039e0c:	f001 ff94 	bl	203bd38 <__boot_free_veneer>
 2039e10:	4628      	mov	r0, r5
 2039e12:	f001 ff91 	bl	203bd38 <__boot_free_veneer>
 2039e16:	4620      	mov	r0, r4
 2039e18:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 2039e1c:	f001 bf8c 	b.w	203bd38 <__boot_free_veneer>

02039e20 <uapi_flash_self_check>:
 2039e20:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
 2039e24:	4605      	mov	r5, r0
 2039e26:	4617      	mov	r7, r2
 2039e28:	4698      	mov	r8, r3
 2039e2a:	2238      	movs	r2, #56	; 0x38
 2039e2c:	2301      	movs	r3, #1
 2039e2e:	2006      	movs	r0, #6
 2039e30:	460e      	mov	r6, r1
 2039e32:	4629      	mov	r1, r5
 2039e34:	f7ff ff66 	bl	2039d04 <uapi_common_nv_read>
 2039e38:	4604      	mov	r4, r0
 2039e3a:	b918      	cbnz	r0, 2039e44 <uapi_flash_self_check+0x24>
 2039e3c:	f995 3007 	ldrsb.w	r3, [r5, #7]
 2039e40:	2b00      	cmp	r3, #0
 2039e42:	db02      	blt.n	2039e4a <uapi_flash_self_check+0x2a>
 2039e44:	4620      	mov	r0, r4
 2039e46:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 2039e4a:	2301      	movs	r3, #1
 2039e4c:	4631      	mov	r1, r6
 2039e4e:	223c      	movs	r2, #60	; 0x3c
 2039e50:	2005      	movs	r0, #5
 2039e52:	f7ff ff57 	bl	2039d04 <uapi_common_nv_read>
 2039e56:	4604      	mov	r4, r0
 2039e58:	2800      	cmp	r0, #0
 2039e5a:	d1f3      	bne.n	2039e44 <uapi_flash_self_check+0x24>
 2039e5c:	88a8      	ldrh	r0, [r5, #4]
 2039e5e:	f3c0 030b 	ubfx	r3, r0, #0, #12
 2039e62:	b92b      	cbnz	r3, 2039e70 <uapi_flash_self_check+0x50>
 2039e64:	2301      	movs	r3, #1
 2039e66:	f888 3000 	strb.w	r3, [r8]
 2039e6a:	4620      	mov	r0, r4
 2039e6c:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 2039e70:	8939      	ldrh	r1, [r7, #8]
 2039e72:	0318      	lsls	r0, r3, #12
 2039e74:	f3c1 010b 	ubfx	r1, r1, #0, #12
 2039e78:	0309      	lsls	r1, r1, #12
 2039e7a:	f7ff ff3f 	bl	2039cfc <uapi_temp_nv_init>
 2039e7e:	b108      	cbz	r0, 2039e84 <uapi_flash_self_check+0x64>
 2039e80:	4604      	mov	r4, r0
 2039e82:	e7df      	b.n	2039e44 <uapi_flash_self_check+0x24>
 2039e84:	2302      	movs	r3, #2
 2039e86:	2238      	movs	r2, #56	; 0x38
 2039e88:	4629      	mov	r1, r5
 2039e8a:	2006      	movs	r0, #6
 2039e8c:	f7ff ff3a 	bl	2039d04 <uapi_common_nv_read>
 2039e90:	2800      	cmp	r0, #0
 2039e92:	d1f5      	bne.n	2039e80 <uapi_flash_self_check+0x60>
 2039e94:	2302      	movs	r3, #2
 2039e96:	e7e6      	b.n	2039e66 <uapi_flash_self_check+0x46>

02039e98 <uapi_flash_partition_refresh_malloc.constprop.0>:
 2039e98:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
 2039e9a:	4605      	mov	r5, r0
 2039e9c:	2038      	movs	r0, #56	; 0x38
 2039e9e:	460e      	mov	r6, r1
 2039ea0:	4617      	mov	r7, r2
 2039ea2:	f001 ff41 	bl	203bd28 <__boot_malloc_veneer>
 2039ea6:	6028      	str	r0, [r5, #0]
 2039ea8:	b168      	cbz	r0, 2039ec6 <uapi_flash_partition_refresh_malloc.constprop.0+0x2e>
 2039eaa:	203c      	movs	r0, #60	; 0x3c
 2039eac:	f001 ff3c 	bl	203bd28 <__boot_malloc_veneer>
 2039eb0:	4604      	mov	r4, r0
 2039eb2:	6030      	str	r0, [r6, #0]
 2039eb4:	b158      	cbz	r0, 2039ece <uapi_flash_partition_refresh_malloc.constprop.0+0x36>
 2039eb6:	f44f 5080 	mov.w	r0, #4096	; 0x1000
 2039eba:	f001 ff35 	bl	203bd28 <__boot_malloc_veneer>
 2039ebe:	4604      	mov	r4, r0
 2039ec0:	6038      	str	r0, [r7, #0]
 2039ec2:	2001      	movs	r0, #1
 2039ec4:	b104      	cbz	r4, 2039ec8 <uapi_flash_partition_refresh_malloc.constprop.0+0x30>
 2039ec6:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
 2039ec8:	6830      	ldr	r0, [r6, #0]
 2039eca:	f001 ff35 	bl	203bd38 <__boot_free_veneer>
 2039ece:	6828      	ldr	r0, [r5, #0]
 2039ed0:	f001 ff32 	bl	203bd38 <__boot_free_veneer>
 2039ed4:	4620      	mov	r0, r4
 2039ed6:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}

02039ed8 <uapi_get_partition_table>:
 2039ed8:	4800      	ldr	r0, [pc, #0]	; (2039edc <uapi_get_partition_table+0x4>)
 2039eda:	4770      	bx	lr
 2039edc:	0203c3a8 	.word	0x0203c3a8

02039ee0 <uapi_flash_partition_refresh_proc55>:
 2039ee0:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 2039ee4:	4683      	mov	fp, r0
 2039ee6:	b089      	sub	sp, #36	; 0x24
 2039ee8:	9106      	str	r1, [sp, #24]
 2039eea:	f7ff fff5 	bl	2039ed8 <uapi_get_partition_table>
 2039eee:	f8db 4004 	ldr.w	r4, [fp, #4]
 2039ef2:	9007      	str	r0, [sp, #28]
 2039ef4:	f414 3ff8 	tst.w	r4, #126976	; 0x1f000
 2039ef8:	f000 80a5 	beq.w	203a046 <uapi_flash_partition_refresh_proc55+0x166>
 2039efc:	4699      	mov	r9, r3
 2039efe:	2300      	movs	r3, #0
 2039f00:	4692      	mov	sl, r2
 2039f02:	9304      	str	r3, [sp, #16]
 2039f04:	9b04      	ldr	r3, [sp, #16]
 2039f06:	3302      	adds	r3, #2
 2039f08:	9305      	str	r3, [sp, #20]
 2039f0a:	eb0b 0383 	add.w	r3, fp, r3, lsl #2
 2039f0e:	78df      	ldrb	r7, [r3, #3]
 2039f10:	06bb      	lsls	r3, r7, #26
 2039f12:	f140 808e 	bpl.w	203a032 <uapi_flash_partition_refresh_proc55+0x152>
 2039f16:	f007 071f 	and.w	r7, r7, #31
 2039f1a:	2f0b      	cmp	r7, #11
 2039f1c:	f200 8089 	bhi.w	203a032 <uapi_flash_partition_refresh_proc55+0x152>
 2039f20:	9b06      	ldr	r3, [sp, #24]
 2039f22:	eb03 0187 	add.w	r1, r3, r7, lsl #2
 2039f26:	9b07      	ldr	r3, [sp, #28]
 2039f28:	f8b1 8004 	ldrh.w	r8, [r1, #4]
 2039f2c:	eb03 0287 	add.w	r2, r3, r7, lsl #2
 2039f30:	8893      	ldrh	r3, [r2, #4]
 2039f32:	6855      	ldr	r5, [r2, #4]
 2039f34:	684a      	ldr	r2, [r1, #4]
 2039f36:	79ce      	ldrb	r6, [r1, #7]
 2039f38:	f3c3 030b 	ubfx	r3, r3, #0, #12
 2039f3c:	f3c8 080b 	ubfx	r8, r8, #0, #12
 2039f40:	f3c5 350b 	ubfx	r5, r5, #12, #12
 2039f44:	f3c2 320b 	ubfx	r2, r2, #12, #12
 2039f48:	1958      	adds	r0, r3, r5
 2039f4a:	eb08 0102 	add.w	r1, r8, r2
 2039f4e:	09f6      	lsrs	r6, r6, #7
 2039f50:	ea4f 3303 	mov.w	r3, r3, lsl #12
 2039f54:	ea4f 3000 	mov.w	r0, r0, lsl #12
 2039f58:	ea4f 3808 	mov.w	r8, r8, lsl #12
 2039f5c:	ea4f 3101 	mov.w	r1, r1, lsl #12
 2039f60:	d173      	bne.n	203a04a <uapi_flash_partition_refresh_proc55+0x16a>
 2039f62:	4543      	cmp	r3, r8
 2039f64:	d065      	beq.n	203a032 <uapi_flash_partition_refresh_proc55+0x152>
 2039f66:	4295      	cmp	r5, r2
 2039f68:	bf94      	ite	ls
 2039f6a:	032a      	lslls	r2, r5, #12
 2039f6c:	0312      	lslhi	r2, r2, #12
 2039f6e:	9202      	str	r2, [sp, #8]
 2039f70:	9a05      	ldr	r2, [sp, #20]
 2039f72:	3702      	adds	r7, #2
 2039f74:	eb0b 0282 	add.w	r2, fp, r2, lsl #2
 2039f78:	f83b 5027 	ldrh.w	r5, [fp, r7, lsl #2]
 2039f7c:	78d2      	ldrb	r2, [r2, #3]
 2039f7e:	f3c5 050b 	ubfx	r5, r5, #0, #12
 2039f82:	0992      	lsrs	r2, r2, #6
 2039f84:	032d      	lsls	r5, r5, #12
 2039f86:	2a01      	cmp	r2, #1
 2039f88:	462e      	mov	r6, r5
 2039f8a:	d06c      	beq.n	203a066 <uapi_flash_partition_refresh_proc55+0x186>
 2039f8c:	4543      	cmp	r3, r8
 2039f8e:	d368      	bcc.n	203a062 <uapi_flash_partition_refresh_proc55+0x182>
 2039f90:	f44f 5280 	mov.w	r2, #4096	; 0x1000
 2039f94:	9203      	str	r2, [sp, #12]
 2039f96:	441d      	add	r5, r3
 2039f98:	eba8 0303 	sub.w	r3, r8, r3
 2039f9c:	9301      	str	r3, [sp, #4]
 2039f9e:	e02f      	b.n	203a000 <uapi_flash_partition_refresh_proc55+0x120>
 2039fa0:	4652      	mov	r2, sl
 2039fa2:	f44f 5180 	mov.w	r1, #4096	; 0x1000
 2039fa6:	4628      	mov	r0, r5
 2039fa8:	f7ff fa04 	bl	20393b4 <uapi_flash_read>
 2039fac:	4604      	mov	r4, r0
 2039fae:	2800      	cmp	r0, #0
 2039fb0:	d14e      	bne.n	203a050 <uapi_flash_partition_refresh_proc55+0x170>
 2039fb2:	9b03      	ldr	r3, [sp, #12]
 2039fb4:	4642      	mov	r2, r8
 2039fb6:	4629      	mov	r1, r5
 2039fb8:	482f      	ldr	r0, [pc, #188]	; (203a078 <uapi_flash_partition_refresh_proc55+0x198>)
 2039fba:	441d      	add	r5, r3
 2039fbc:	f001 fe9c 	bl	203bcf8 <__boot_msg2_veneer>
 2039fc0:	f44f 5180 	mov.w	r1, #4096	; 0x1000
 2039fc4:	4640      	mov	r0, r8
 2039fc6:	f7ff f9bb 	bl	2039340 <uapi_flash_erase>
 2039fca:	4601      	mov	r1, r0
 2039fcc:	4623      	mov	r3, r4
 2039fce:	4652      	mov	r2, sl
 2039fd0:	460c      	mov	r4, r1
 2039fd2:	4640      	mov	r0, r8
 2039fd4:	f44f 5180 	mov.w	r1, #4096	; 0x1000
 2039fd8:	f7ff f9ca 	bl	2039370 <uapi_flash_write>
 2039fdc:	4304      	orrs	r4, r0
 2039fde:	ea4f 3116 	mov.w	r1, r6, lsr #12
 2039fe2:	d135      	bne.n	203a050 <uapi_flash_partition_refresh_proc55+0x170>
 2039fe4:	f83b 2027 	ldrh.w	r2, [fp, r7, lsl #2]
 2039fe8:	464b      	mov	r3, r9
 2039fea:	f361 020b 	bfi	r2, r1, #0, #12
 2039fee:	f82b 2027 	strh.w	r2, [fp, r7, lsl #2]
 2039ff2:	4659      	mov	r1, fp
 2039ff4:	2238      	movs	r2, #56	; 0x38
 2039ff6:	2006      	movs	r0, #6
 2039ff8:	f7ff fe84 	bl	2039d04 <uapi_common_nv_read>
 2039ffc:	4604      	mov	r4, r0
 2039ffe:	bb38      	cbnz	r0, 203a050 <uapi_flash_partition_refresh_proc55+0x170>
 203a000:	9b01      	ldr	r3, [sp, #4]
 203a002:	eb03 0805 	add.w	r8, r3, r5
 203a006:	9b02      	ldr	r3, [sp, #8]
 203a008:	429e      	cmp	r6, r3
 203a00a:	f506 5680 	add.w	r6, r6, #4096	; 0x1000
 203a00e:	d3c7      	bcc.n	2039fa0 <uapi_flash_partition_refresh_proc55+0xc0>
 203a010:	9b05      	ldr	r3, [sp, #20]
 203a012:	4659      	mov	r1, fp
 203a014:	eb0b 0383 	add.w	r3, fp, r3, lsl #2
 203a018:	78da      	ldrb	r2, [r3, #3]
 203a01a:	2006      	movs	r0, #6
 203a01c:	f36f 1245 	bfc	r2, #5, #1
 203a020:	70da      	strb	r2, [r3, #3]
 203a022:	464b      	mov	r3, r9
 203a024:	2238      	movs	r2, #56	; 0x38
 203a026:	f7ff fe6f 	bl	2039d08 <uapi_common_nv_write>
 203a02a:	4604      	mov	r4, r0
 203a02c:	b980      	cbnz	r0, 203a050 <uapi_flash_partition_refresh_proc55+0x170>
 203a02e:	f8db 4004 	ldr.w	r4, [fp, #4]
 203a032:	9b04      	ldr	r3, [sp, #16]
 203a034:	3301      	adds	r3, #1
 203a036:	461a      	mov	r2, r3
 203a038:	9304      	str	r3, [sp, #16]
 203a03a:	b2d2      	uxtb	r2, r2
 203a03c:	f3c4 3304 	ubfx	r3, r4, #12, #5
 203a040:	4293      	cmp	r3, r2
 203a042:	f63f af5f 	bhi.w	2039f04 <uapi_flash_partition_refresh_proc55+0x24>
 203a046:	2400      	movs	r4, #0
 203a048:	e007      	b.n	203a05a <uapi_flash_partition_refresh_proc55+0x17a>
 203a04a:	4288      	cmp	r0, r1
 203a04c:	d18b      	bne.n	2039f66 <uapi_flash_partition_refresh_proc55+0x86>
 203a04e:	e7f0      	b.n	203a032 <uapi_flash_partition_refresh_proc55+0x152>
 203a050:	4652      	mov	r2, sl
 203a052:	4658      	mov	r0, fp
 203a054:	9906      	ldr	r1, [sp, #24]
 203a056:	f7ff fed5 	bl	2039e04 <uapi_flash_partition_refresh_free_memery>
 203a05a:	4620      	mov	r0, r4
 203a05c:	b009      	add	sp, #36	; 0x24
 203a05e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 203a062:	2a02      	cmp	r2, #2
 203a064:	d194      	bne.n	2039f90 <uapi_flash_partition_refresh_proc55+0xb0>
 203a066:	4a05      	ldr	r2, [pc, #20]	; (203a07c <uapi_flash_partition_refresh_proc55+0x19c>)
 203a068:	426d      	negs	r5, r5
 203a06a:	f5a0 5380 	sub.w	r3, r0, #4096	; 0x1000
 203a06e:	f5a1 5880 	sub.w	r8, r1, #4096	; 0x1000
 203a072:	9203      	str	r2, [sp, #12]
 203a074:	e78f      	b.n	2039f96 <uapi_flash_partition_refresh_proc55+0xb6>
 203a076:	bf00      	nop
 203a078:	0203ba60 	.word	0x0203ba60
 203a07c:	fffff000 	.word	0xfffff000

0203a080 <flash_partition_reset>:
 203a080:	b510      	push	{r4, lr}
 203a082:	f7ff ff29 	bl	2039ed8 <uapi_get_partition_table>
 203a086:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 203a08a:	4601      	mov	r1, r0
 203a08c:	2301      	movs	r3, #1
 203a08e:	223c      	movs	r2, #60	; 0x3c
 203a090:	2004      	movs	r0, #4
 203a092:	f7ff be37 	b.w	2039d04 <uapi_common_nv_read>
 203a096:	bf00      	nop

0203a098 <uapi_flash_after_move>:
 203a098:	b510      	push	{r4, lr}
 203a09a:	2301      	movs	r3, #1
 203a09c:	4604      	mov	r4, r0
 203a09e:	223c      	movs	r2, #60	; 0x3c
 203a0a0:	2004      	movs	r0, #4
 203a0a2:	f7ff fe31 	bl	2039d08 <uapi_common_nv_write>
 203a0a6:	b100      	cbz	r0, 203a0aa <uapi_flash_after_move+0x12>
 203a0a8:	bd10      	pop	{r4, pc}
 203a0aa:	f894 c007 	ldrb.w	ip, [r4, #7]
 203a0ae:	2301      	movs	r3, #1
 203a0b0:	f360 1cc7 	bfi	ip, r0, #7, #1
 203a0b4:	2238      	movs	r2, #56	; 0x38
 203a0b6:	4621      	mov	r1, r4
 203a0b8:	2006      	movs	r0, #6
 203a0ba:	f884 c007 	strb.w	ip, [r4, #7]
 203a0be:	f7ff fe23 	bl	2039d08 <uapi_common_nv_write>
 203a0c2:	2800      	cmp	r0, #0
 203a0c4:	d1f0      	bne.n	203a0a8 <uapi_flash_after_move+0x10>
 203a0c6:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 203a0ca:	f7ff bfd9 	b.w	203a080 <flash_partition_reset>
 203a0ce:	bf00      	nop

0203a0d0 <uapi_flash_partition_check_ver>:
 203a0d0:	b108      	cbz	r0, 203a0d6 <uapi_flash_partition_check_ver+0x6>
 203a0d2:	f7ff be71 	b.w	2039db8 <flash_partition_check_ver>
 203a0d6:	2002      	movs	r0, #2
 203a0d8:	4770      	bx	lr
 203a0da:	bf00      	nop

0203a0dc <uapi_flash_partition_refresh>:
 203a0dc:	2300      	movs	r3, #0
 203a0de:	b510      	push	{r4, lr}
 203a0e0:	b084      	sub	sp, #16
 203a0e2:	aa03      	add	r2, sp, #12
 203a0e4:	a902      	add	r1, sp, #8
 203a0e6:	a801      	add	r0, sp, #4
 203a0e8:	e9cd 3301 	strd	r3, r3, [sp, #4]
 203a0ec:	9303      	str	r3, [sp, #12]
 203a0ee:	f7ff fed3 	bl	2039e98 <uapi_flash_partition_refresh_malloc.constprop.0>
 203a0f2:	2801      	cmp	r0, #1
 203a0f4:	d004      	beq.n	203a100 <uapi_flash_partition_refresh+0x24>
 203a0f6:	f241 34ef 	movw	r4, #5103	; 0x13ef
 203a0fa:	4620      	mov	r0, r4
 203a0fc:	b004      	add	sp, #16
 203a0fe:	bd10      	pop	{r4, pc}
 203a100:	f7ff feea 	bl	2039ed8 <uapi_get_partition_table>
 203a104:	f10d 0303 	add.w	r3, sp, #3
 203a108:	4602      	mov	r2, r0
 203a10a:	e9dd 0101 	ldrd	r0, r1, [sp, #4]
 203a10e:	f7ff fe87 	bl	2039e20 <uapi_flash_self_check>
 203a112:	4604      	mov	r4, r0
 203a114:	9801      	ldr	r0, [sp, #4]
 203a116:	b91c      	cbnz	r4, 203a120 <uapi_flash_partition_refresh+0x44>
 203a118:	f990 3007 	ldrsb.w	r3, [r0, #7]
 203a11c:	2b00      	cmp	r3, #0
 203a11e:	db06      	blt.n	203a12e <uapi_flash_partition_refresh+0x52>
 203a120:	e9dd 1202 	ldrd	r1, r2, [sp, #8]
 203a124:	f7ff fe6e 	bl	2039e04 <uapi_flash_partition_refresh_free_memery>
 203a128:	4620      	mov	r0, r4
 203a12a:	b004      	add	sp, #16
 203a12c:	bd10      	pop	{r4, pc}
 203a12e:	f89d 3003 	ldrb.w	r3, [sp, #3]
 203a132:	e9dd 1202 	ldrd	r1, r2, [sp, #8]
 203a136:	f7ff fed3 	bl	2039ee0 <uapi_flash_partition_refresh_proc55>
 203a13a:	4604      	mov	r4, r0
 203a13c:	b108      	cbz	r0, 203a142 <uapi_flash_partition_refresh+0x66>
 203a13e:	9801      	ldr	r0, [sp, #4]
 203a140:	e7ee      	b.n	203a120 <uapi_flash_partition_refresh+0x44>
 203a142:	e9dd 0101 	ldrd	r0, r1, [sp, #4]
 203a146:	f7ff ffa7 	bl	203a098 <uapi_flash_after_move>
 203a14a:	4604      	mov	r4, r0
 203a14c:	e7f7      	b.n	203a13e <uapi_flash_partition_refresh+0x62>
 203a14e:	bf00      	nop

0203a150 <uapi_flash_partition_init>:
 203a150:	b508      	push	{r3, lr}
 203a152:	f44f 4080 	mov.w	r0, #16384	; 0x4000
 203a156:	f7ff fdc5 	bl	2039ce4 <uapi_factory_nv_init>
 203a15a:	f7ff febd 	bl	2039ed8 <uapi_get_partition_table>
 203a15e:	223c      	movs	r2, #60	; 0x3c
 203a160:	4601      	mov	r1, r0
 203a162:	2004      	movs	r0, #4
 203a164:	f7ff fdc2 	bl	2039cec <uapi_factory_nv_read>
 203a168:	b100      	cbz	r0, 203a16c <uapi_flash_partition_init+0x1c>
 203a16a:	bd08      	pop	{r3, pc}
 203a16c:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
 203a170:	f7ff bfb4 	b.w	203a0dc <uapi_flash_partition_refresh>

0203a174 <uapi_is_secure_start>:
 203a174:	2300      	movs	r3, #0
 203a176:	b510      	push	{r4, lr}
 203a178:	b082      	sub	sp, #8
 203a17a:	2010      	movs	r0, #16
 203a17c:	f10d 0107 	add.w	r1, sp, #7
 203a180:	f88d 3007 	strb.w	r3, [sp, #7]
 203a184:	f001 fdf8 	bl	203bd78 <__uapi_efuse_read_veneer>
 203a188:	b958      	cbnz	r0, 203a1a2 <uapi_is_secure_start+0x2e>
 203a18a:	f89d 4007 	ldrb.w	r4, [sp, #7]
 203a18e:	f014 0401 	ands.w	r4, r4, #1
 203a192:	bf14      	ite	ne
 203a194:	4805      	ldrne	r0, [pc, #20]	; (203a1ac <uapi_is_secure_start+0x38>)
 203a196:	4806      	ldreq	r0, [pc, #24]	; (203a1b0 <uapi_is_secure_start+0x3c>)
 203a198:	f001 fdb2 	bl	203bd00 <__boot_msg0_veneer>
 203a19c:	4620      	mov	r0, r4
 203a19e:	b002      	add	sp, #8
 203a1a0:	bd10      	pop	{r4, pc}
 203a1a2:	2401      	movs	r4, #1
 203a1a4:	4620      	mov	r0, r4
 203a1a6:	b002      	add	sp, #8
 203a1a8:	bd10      	pop	{r4, pc}
 203a1aa:	bf00      	nop
 203a1ac:	0203ba68 	.word	0x0203ba68
 203a1b0:	0203ba74 	.word	0x0203ba74

0203a1b4 <uapi_secure_save_key_pos>:
 203a1b4:	4b03      	ldr	r3, [pc, #12]	; (203a1c4 <uapi_secure_save_key_pos+0x10>)
 203a1b6:	4804      	ldr	r0, [pc, #16]	; (203a1c8 <uapi_secure_save_key_pos+0x14>)
 203a1b8:	4904      	ldr	r1, [pc, #16]	; (203a1cc <uapi_secure_save_key_pos+0x18>)
 203a1ba:	4a05      	ldr	r2, [pc, #20]	; (203a1d0 <uapi_secure_save_key_pos+0x1c>)
 203a1bc:	4403      	add	r3, r0
 203a1be:	1a5b      	subs	r3, r3, r1
 203a1c0:	6493      	str	r3, [r2, #72]	; 0x48
 203a1c2:	4770      	bx	lr
 203a1c4:	01003b30 	.word	0x01003b30
 203a1c8:	0103be08 	.word	0x0103be08
 203a1cc:	0203be08 	.word	0x0203be08
 203a1d0:	40030000 	.word	0x40030000

0203a1d4 <boot_start_check_tbl_secure_info>:
 203a1d4:	b5f0      	push	{r4, r5, r6, r7, lr}
 203a1d6:	4d31      	ldr	r5, [pc, #196]	; (203a29c <boot_start_check_tbl_secure_info+0xc8>)
 203a1d8:	f5ad 7d4d 	sub.w	sp, sp, #820	; 0x334
 203a1dc:	69ec      	ldr	r4, [r5, #28]
 203a1de:	2c00      	cmp	r4, #0
 203a1e0:	d057      	beq.n	203a292 <boot_start_check_tbl_secure_info+0xbe>
 203a1e2:	f44f 7241 	mov.w	r2, #772	; 0x304
 203a1e6:	2100      	movs	r1, #0
 203a1e8:	a80b      	add	r0, sp, #44	; 0x2c
 203a1ea:	f001 fde9 	bl	203bdc0 <__memset_veneer>
 203a1ee:	2300      	movs	r3, #0
 203a1f0:	e9cd 3303 	strd	r3, r3, [sp, #12]
 203a1f4:	f88d 3008 	strb.w	r3, [sp, #8]
 203a1f8:	f7ff ffbc 	bl	203a174 <uapi_is_secure_start>
 203a1fc:	b158      	cbz	r0, 203a216 <boot_start_check_tbl_secure_info+0x42>
 203a1fe:	69a3      	ldr	r3, [r4, #24]
 203a200:	b163      	cbz	r3, 203a21c <boot_start_check_tbl_secure_info+0x48>
 203a202:	69e2      	ldr	r2, [r4, #28]
 203a204:	b152      	cbz	r2, 203a21c <boot_start_check_tbl_secure_info+0x48>
 203a206:	68e8      	ldr	r0, [r5, #12]
 203a208:	f44f 7141 	mov.w	r1, #772	; 0x304
 203a20c:	4418      	add	r0, r3
 203a20e:	aa0b      	add	r2, sp, #44	; 0x2c
 203a210:	f7ff f8d0 	bl	20393b4 <uapi_flash_read>
 203a214:	b138      	cbz	r0, 203a226 <boot_start_check_tbl_secure_info+0x52>
 203a216:	f50d 7d4d 	add.w	sp, sp, #820	; 0x334
 203a21a:	bdf0      	pop	{r4, r5, r6, r7, pc}
 203a21c:	f241 30c2 	movw	r0, #5058	; 0x13c2
 203a220:	f50d 7d4d 	add.w	sp, sp, #820	; 0x334
 203a224:	bdf0      	pop	{r4, r5, r6, r7, pc}
 203a226:	f44f 7380 	mov.w	r3, #256	; 0x100
 203a22a:	9304      	str	r3, [sp, #16]
 203a22c:	f89d 302e 	ldrb.w	r3, [sp, #46]	; 0x2e
 203a230:	af8c      	add	r7, sp, #560	; 0x230
 203a232:	f3c3 0305 	ubfx	r3, r3, #0, #6
 203a236:	f88d 301c 	strb.w	r3, [sp, #28]
 203a23a:	4b19      	ldr	r3, [pc, #100]	; (203a2a0 <boot_start_check_tbl_secure_info+0xcc>)
 203a23c:	ae05      	add	r6, sp, #20
 203a23e:	f88d 0008 	strb.w	r0, [sp, #8]
 203a242:	9305      	str	r3, [sp, #20]
 203a244:	9703      	str	r7, [sp, #12]
 203a246:	2201      	movs	r2, #1
 203a248:	af0c      	add	r7, sp, #48	; 0x30
 203a24a:	9600      	str	r6, [sp, #0]
 203a24c:	ab02      	add	r3, sp, #8
 203a24e:	a904      	add	r1, sp, #16
 203a250:	a803      	add	r0, sp, #12
 203a252:	9706      	str	r7, [sp, #24]
 203a254:	f001 f900 	bl	203b458 <uapi_rsa_verify_flash_content>
 203a258:	2800      	cmp	r0, #0
 203a25a:	d1dc      	bne.n	203a216 <boot_start_check_tbl_secure_info+0x42>
 203a25c:	2201      	movs	r2, #1
 203a25e:	6a20      	ldr	r0, [r4, #32]
 203a260:	68e9      	ldr	r1, [r5, #12]
 203a262:	f89d 302e 	ldrb.w	r3, [sp, #46]	; 0x2e
 203a266:	4401      	add	r1, r0
 203a268:	9103      	str	r1, [sp, #12]
 203a26a:	6a61      	ldr	r1, [r4, #36]	; 0x24
 203a26c:	ad8c      	add	r5, sp, #560	; 0x230
 203a26e:	f3c3 0305 	ubfx	r3, r3, #0, #6
 203a272:	ac08      	add	r4, sp, #32
 203a274:	9104      	str	r1, [sp, #16]
 203a276:	9508      	str	r5, [sp, #32]
 203a278:	f88d 3028 	strb.w	r3, [sp, #40]	; 0x28
 203a27c:	ad4c      	add	r5, sp, #304	; 0x130
 203a27e:	a904      	add	r1, sp, #16
 203a280:	a803      	add	r0, sp, #12
 203a282:	9400      	str	r4, [sp, #0]
 203a284:	ab02      	add	r3, sp, #8
 203a286:	f88d 2008 	strb.w	r2, [sp, #8]
 203a28a:	9509      	str	r5, [sp, #36]	; 0x24
 203a28c:	f001 f8e4 	bl	203b458 <uapi_rsa_verify_flash_content>
 203a290:	e7c1      	b.n	203a216 <boot_start_check_tbl_secure_info+0x42>
 203a292:	f04f 30ff 	mov.w	r0, #4294967295	; 0xffffffff
 203a296:	f50d 7d4d 	add.w	sp, sp, #820	; 0x334
 203a29a:	bdf0      	pop	{r4, r5, r6, r7, pc}
 203a29c:	0203c3e4 	.word	0x0203c3e4
 203a2a0:	0203be08 	.word	0x0203be08

0203a2a4 <boot_upg_check_secure_head>:
 203a2a4:	b5f0      	push	{r4, r5, r6, r7, lr}
 203a2a6:	4e4a      	ldr	r6, [pc, #296]	; (203a3d0 <boot_upg_check_secure_head+0x12c>)
 203a2a8:	f5ad 7d4f 	sub.w	sp, sp, #828	; 0x33c
 203a2ac:	f44f 7241 	mov.w	r2, #772	; 0x304
 203a2b0:	2100      	movs	r1, #0
 203a2b2:	a80d      	add	r0, sp, #52	; 0x34
 203a2b4:	6a35      	ldr	r5, [r6, #32]
 203a2b6:	f001 fd83 	bl	203bdc0 <__memset_veneer>
 203a2ba:	2300      	movs	r3, #0
 203a2bc:	e9cd 3303 	strd	r3, r3, [sp, #12]
 203a2c0:	e9cd 3305 	strd	r3, r3, [sp, #20]
 203a2c4:	f8ad 3008 	strh.w	r3, [sp, #8]
 203a2c8:	f7ff ff54 	bl	203a174 <uapi_is_secure_start>
 203a2cc:	2800      	cmp	r0, #0
 203a2ce:	d07a      	beq.n	203a3c6 <boot_upg_check_secure_head+0x122>
 203a2d0:	6cab      	ldr	r3, [r5, #72]	; 0x48
 203a2d2:	2b00      	cmp	r3, #0
 203a2d4:	d071      	beq.n	203a3ba <boot_upg_check_secure_head+0x116>
 203a2d6:	6cea      	ldr	r2, [r5, #76]	; 0x4c
 203a2d8:	2a00      	cmp	r2, #0
 203a2da:	d06e      	beq.n	203a3ba <boot_upg_check_secure_head+0x116>
 203a2dc:	69b0      	ldr	r0, [r6, #24]
 203a2de:	f44f 7141 	mov.w	r1, #772	; 0x304
 203a2e2:	4418      	add	r0, r3
 203a2e4:	aa0d      	add	r2, sp, #52	; 0x34
 203a2e6:	f7ff f865 	bl	20393b4 <uapi_flash_read>
 203a2ea:	4604      	mov	r4, r0
 203a2ec:	2800      	cmp	r0, #0
 203a2ee:	d166      	bne.n	203a3be <boot_upg_check_secure_head+0x11a>
 203a2f0:	f89d 3137 	ldrb.w	r3, [sp, #311]	; 0x137
 203a2f4:	f89d 2039 	ldrb.w	r2, [sp, #57]	; 0x39
 203a2f8:	f89d 1038 	ldrb.w	r1, [sp, #56]	; 0x38
 203a2fc:	4835      	ldr	r0, [pc, #212]	; (203a3d4 <boot_upg_check_secure_head+0x130>)
 203a2fe:	9300      	str	r3, [sp, #0]
 203a300:	f89d 3136 	ldrb.w	r3, [sp, #310]	; 0x136
 203a304:	f001 fd68 	bl	203bdd8 <__boot_msg4_veneer>
 203a308:	f89d 3237 	ldrb.w	r3, [sp, #567]	; 0x237
 203a30c:	f89d 2139 	ldrb.w	r2, [sp, #313]	; 0x139
 203a310:	f89d 1138 	ldrb.w	r1, [sp, #312]	; 0x138
 203a314:	4830      	ldr	r0, [pc, #192]	; (203a3d8 <boot_upg_check_secure_head+0x134>)
 203a316:	9300      	str	r3, [sp, #0]
 203a318:	f89d 3236 	ldrb.w	r3, [sp, #566]	; 0x236
 203a31c:	f001 fd5c 	bl	203bdd8 <__boot_msg4_veneer>
 203a320:	f89d 3337 	ldrb.w	r3, [sp, #823]	; 0x337
 203a324:	f89d 2239 	ldrb.w	r2, [sp, #569]	; 0x239
 203a328:	f89d 1238 	ldrb.w	r1, [sp, #568]	; 0x238
 203a32c:	482b      	ldr	r0, [pc, #172]	; (203a3dc <boot_upg_check_secure_head+0x138>)
 203a32e:	9300      	str	r3, [sp, #0]
 203a330:	f89d 3336 	ldrb.w	r3, [sp, #822]	; 0x336
 203a334:	f001 fd50 	bl	203bdd8 <__boot_msg4_veneer>
 203a338:	f44f 7380 	mov.w	r3, #256	; 0x100
 203a33c:	9305      	str	r3, [sp, #20]
 203a33e:	f89d 3036 	ldrb.w	r3, [sp, #54]	; 0x36
 203a342:	f88d 4008 	strb.w	r4, [sp, #8]
 203a346:	f3c3 0305 	ubfx	r3, r3, #0, #6
 203a34a:	f88d 3024 	strb.w	r3, [sp, #36]	; 0x24
 203a34e:	4b24      	ldr	r3, [pc, #144]	; (203a3e0 <boot_upg_check_secure_head+0x13c>)
 203a350:	ac07      	add	r4, sp, #28
 203a352:	af8e      	add	r7, sp, #568	; 0x238
 203a354:	9400      	str	r4, [sp, #0]
 203a356:	9307      	str	r3, [sp, #28]
 203a358:	9703      	str	r7, [sp, #12]
 203a35a:	2201      	movs	r2, #1
 203a35c:	ab02      	add	r3, sp, #8
 203a35e:	a905      	add	r1, sp, #20
 203a360:	a803      	add	r0, sp, #12
 203a362:	af0e      	add	r7, sp, #56	; 0x38
 203a364:	9708      	str	r7, [sp, #32]
 203a366:	f001 f877 	bl	203b458 <uapi_rsa_verify_flash_content>
 203a36a:	4604      	mov	r4, r0
 203a36c:	bb38      	cbnz	r0, 203a3be <boot_upg_check_secure_head+0x11a>
 203a36e:	e9d5 1312 	ldrd	r1, r3, [r5, #72]	; 0x48
 203a372:	2401      	movs	r4, #1
 203a374:	69b2      	ldr	r2, [r6, #24]
 203a376:	6a68      	ldr	r0, [r5, #36]	; 0x24
 203a378:	440b      	add	r3, r1
 203a37a:	1ac0      	subs	r0, r0, r3
 203a37c:	f89d 6036 	ldrb.w	r6, [sp, #54]	; 0x36
 203a380:	4413      	add	r3, r2
 203a382:	3224      	adds	r2, #36	; 0x24
 203a384:	3924      	subs	r1, #36	; 0x24
 203a386:	9203      	str	r2, [sp, #12]
 203a388:	f88d 4008 	strb.w	r4, [sp, #8]
 203a38c:	f88d 4009 	strb.w	r4, [sp, #9]
 203a390:	aa8e      	add	r2, sp, #568	; 0x238
 203a392:	ac0a      	add	r4, sp, #40	; 0x28
 203a394:	9105      	str	r1, [sp, #20]
 203a396:	9006      	str	r0, [sp, #24]
 203a398:	9304      	str	r3, [sp, #16]
 203a39a:	920a      	str	r2, [sp, #40]	; 0x28
 203a39c:	9400      	str	r4, [sp, #0]
 203a39e:	2202      	movs	r2, #2
 203a3a0:	f3c6 0605 	ubfx	r6, r6, #0, #6
 203a3a4:	a905      	add	r1, sp, #20
 203a3a6:	a803      	add	r0, sp, #12
 203a3a8:	ab02      	add	r3, sp, #8
 203a3aa:	ad4e      	add	r5, sp, #312	; 0x138
 203a3ac:	f88d 6030 	strb.w	r6, [sp, #48]	; 0x30
 203a3b0:	950b      	str	r5, [sp, #44]	; 0x2c
 203a3b2:	f001 f851 	bl	203b458 <uapi_rsa_verify_flash_content>
 203a3b6:	4604      	mov	r4, r0
 203a3b8:	e001      	b.n	203a3be <boot_upg_check_secure_head+0x11a>
 203a3ba:	f241 34c1 	movw	r4, #5057	; 0x13c1
 203a3be:	4620      	mov	r0, r4
 203a3c0:	f50d 7d4f 	add.w	sp, sp, #828	; 0x33c
 203a3c4:	bdf0      	pop	{r4, r5, r6, r7, pc}
 203a3c6:	4604      	mov	r4, r0
 203a3c8:	4620      	mov	r0, r4
 203a3ca:	f50d 7d4f 	add.w	sp, sp, #828	; 0x33c
 203a3ce:	bdf0      	pop	{r4, r5, r6, r7, pc}
 203a3d0:	0203c3e4 	.word	0x0203c3e4
 203a3d4:	0203ba84 	.word	0x0203ba84
 203a3d8:	0203ba90 	.word	0x0203ba90
 203a3dc:	0203ba9c 	.word	0x0203ba9c
 203a3e0:	0203be08 	.word	0x0203be08

0203a3e4 <boot_upg_check_secure_boot>:
 203a3e4:	b508      	push	{r3, lr}
 203a3e6:	f7ff fec5 	bl	203a174 <uapi_is_secure_start>
 203a3ea:	b168      	cbz	r0, 203a408 <boot_upg_check_secure_boot+0x24>
 203a3ec:	4b07      	ldr	r3, [pc, #28]	; (203a40c <boot_upg_check_secure_boot+0x28>)
 203a3ee:	6a5a      	ldr	r2, [r3, #36]	; 0x24
 203a3f0:	b142      	cbz	r2, 203a404 <boot_upg_check_secure_boot+0x20>
 203a3f2:	699b      	ldr	r3, [r3, #24]
 203a3f4:	6850      	ldr	r0, [r2, #4]
 203a3f6:	f103 7380 	add.w	r3, r3, #16777216	; 0x1000000
 203a3fa:	4418      	add	r0, r3
 203a3fc:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
 203a400:	f000 beae 	b.w	203b160 <flash_boot_security_boot_verify>
 203a404:	f04f 30ff 	mov.w	r0, #4294967295	; 0xffffffff
 203a408:	bd08      	pop	{r3, pc}
 203a40a:	bf00      	nop
 203a40c:	0203c3e4 	.word	0x0203c3e4

0203a410 <boot_upg_is_upgrade_boot_reason>:
 203a410:	f1a0 0006 	sub.w	r0, r0, #6
 203a414:	fab0 f080 	clz	r0, r0
 203a418:	0940      	lsrs	r0, r0, #5
 203a41a:	4770      	bx	lr

0203a41c <boot_upg_process_phase1.constprop.0>:
 203a41c:	b538      	push	{r3, r4, r5, lr}
 203a41e:	4b0f      	ldr	r3, [pc, #60]	; (203a45c <boot_upg_process_phase1.constprop.0+0x40>)
 203a420:	4604      	mov	r4, r0
 203a422:	6a1d      	ldr	r5, [r3, #32]
 203a424:	f883 002a 	strb.w	r0, [r3, #42]	; 0x2a
 203a428:	f000 fbf6 	bl	203ac18 <boot_upg_check_upg_phase2>
 203a42c:	b938      	cbnz	r0, 203a43e <boot_upg_process_phase1.constprop.0+0x22>
 203a42e:	f895 3033 	ldrb.w	r3, [r5, #51]	; 0x33
 203a432:	2b01      	cmp	r3, #1
 203a434:	d00d      	beq.n	203a452 <boot_upg_process_phase1.constprop.0+0x36>
 203a436:	2b03      	cmp	r3, #3
 203a438:	d002      	beq.n	203a440 <boot_upg_process_phase1.constprop.0+0x24>
 203a43a:	f241 309a 	movw	r0, #5018	; 0x139a
 203a43e:	bd38      	pop	{r3, r4, r5, pc}
 203a440:	4620      	mov	r0, r4
 203a442:	f7ff ffe5 	bl	203a410 <boot_upg_is_upgrade_boot_reason>
 203a446:	2801      	cmp	r0, #1
 203a448:	d1f7      	bne.n	203a43a <boot_upg_process_phase1.constprop.0+0x1e>
 203a44a:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 203a44e:	f000 bc07 	b.w	203ac60 <boot_upg_process_boot>
 203a452:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 203a456:	f000 bc9d 	b.w	203ad94 <boot_upg_kernel_process>
 203a45a:	bf00      	nop
 203a45c:	0203c3e4 	.word	0x0203c3e4

0203a460 <boot_upg_process>:
 203a460:	b570      	push	{r4, r5, r6, lr}
 203a462:	4604      	mov	r4, r0
 203a464:	4d13      	ldr	r5, [pc, #76]	; (203a4b4 <boot_upg_process+0x54>)
 203a466:	460a      	mov	r2, r1
 203a468:	4813      	ldr	r0, [pc, #76]	; (203a4b8 <boot_upg_process+0x58>)
 203a46a:	4621      	mov	r1, r4
 203a46c:	69ee      	ldr	r6, [r5, #28]
 203a46e:	f001 fc43 	bl	203bcf8 <__boot_msg2_veneer>
 203a472:	4620      	mov	r0, r4
 203a474:	f7ff ffd2 	bl	203a41c <boot_upg_process_phase1.constprop.0>
 203a478:	4604      	mov	r4, r0
 203a47a:	4810      	ldr	r0, [pc, #64]	; (203a4bc <boot_upg_process+0x5c>)
 203a47c:	4621      	mov	r1, r4
 203a47e:	f001 fcb7 	bl	203bdf0 <__boot_msg1_veneer>
 203a482:	b184      	cbz	r4, 203a4a6 <boot_upg_process+0x46>
 203a484:	f895 3028 	ldrb.w	r3, [r5, #40]	; 0x28
 203a488:	06db      	lsls	r3, r3, #27
 203a48a:	d405      	bmi.n	203a498 <boot_upg_process+0x38>
 203a48c:	4d0c      	ldr	r5, [pc, #48]	; (203a4c0 <boot_upg_process+0x60>)
 203a48e:	4621      	mov	r1, r4
 203a490:	4628      	mov	r0, r5
 203a492:	f001 fcad 	bl	203bdf0 <__boot_msg1_veneer>
 203a496:	e7fa      	b.n	203a48e <boot_upg_process+0x2e>
 203a498:	68e8      	ldr	r0, [r5, #12]
 203a49a:	6873      	ldr	r3, [r6, #4]
 203a49c:	4418      	add	r0, r3
 203a49e:	f000 fea9 	bl	203b1f4 <boot_firmware_new>
 203a4a2:	2000      	movs	r0, #0
 203a4a4:	bd70      	pop	{r4, r5, r6, pc}
 203a4a6:	f000 fb37 	bl	203ab18 <boot_upg_clear_upg_magic_all>
 203a4aa:	f000 fec5 	bl	203b238 <boot_reset>
 203a4ae:	2000      	movs	r0, #0
 203a4b0:	bd70      	pop	{r4, r5, r6, pc}
 203a4b2:	bf00      	nop
 203a4b4:	0203c3e4 	.word	0x0203c3e4
 203a4b8:	0203baac 	.word	0x0203baac
 203a4bc:	0203bac8 	.word	0x0203bac8
 203a4c0:	0203bae8 	.word	0x0203bae8

0203a4c4 <boot_start_pre_check>:
 203a4c4:	b510      	push	{r4, lr}
 203a4c6:	f000 f8a5 	bl	203a614 <boot_start_check_read_tbl_info>
 203a4ca:	b920      	cbnz	r0, 203a4d6 <boot_start_pre_check+0x12>
 203a4cc:	f000 f8f0 	bl	203a6b0 <boot_start_check_kernel>
 203a4d0:	4604      	mov	r4, r0
 203a4d2:	b970      	cbnz	r0, 203a4f2 <boot_start_pre_check+0x2e>
 203a4d4:	bd10      	pop	{r4, pc}
 203a4d6:	4604      	mov	r4, r0
 203a4d8:	f000 fb46 	bl	203ab68 <boot_upg_check_upg_phase0>
 203a4dc:	f000 fb58 	bl	203ab90 <boot_upg_check_upg_phase1>
 203a4e0:	4621      	mov	r1, r4
 203a4e2:	2001      	movs	r0, #1
 203a4e4:	f7ff ffbc 	bl	203a460 <boot_upg_process>
 203a4e8:	f000 f8e2 	bl	203a6b0 <boot_start_check_kernel>
 203a4ec:	4604      	mov	r4, r0
 203a4ee:	2800      	cmp	r0, #0
 203a4f0:	d0f0      	beq.n	203a4d4 <boot_start_pre_check+0x10>
 203a4f2:	f000 fb39 	bl	203ab68 <boot_upg_check_upg_phase0>
 203a4f6:	f000 fb4b 	bl	203ab90 <boot_upg_check_upg_phase1>
 203a4fa:	4621      	mov	r1, r4
 203a4fc:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 203a500:	2002      	movs	r0, #2
 203a502:	f7ff bfad 	b.w	203a460 <boot_upg_process>
 203a506:	bf00      	nop

0203a508 <boot_start_main>:
 203a508:	b530      	push	{r4, r5, lr}
 203a50a:	493b      	ldr	r1, [pc, #236]	; (203a5f8 <boot_start_main+0xf0>)
 203a50c:	b083      	sub	sp, #12
 203a50e:	483b      	ldr	r0, [pc, #236]	; (203a5fc <boot_start_main+0xf4>)
 203a510:	f001 fc6e 	bl	203bdf0 <__boot_msg1_veneer>
 203a514:	f7ff fe4e 	bl	203a1b4 <uapi_secure_save_key_pos>
 203a518:	f000 f920 	bl	203a75c <boot_upg_init_phase1>
 203a51c:	b130      	cbz	r0, 203a52c <boot_start_main+0x24>
 203a51e:	4604      	mov	r4, r0
 203a520:	4d37      	ldr	r5, [pc, #220]	; (203a600 <boot_start_main+0xf8>)
 203a522:	4621      	mov	r1, r4
 203a524:	4628      	mov	r0, r5
 203a526:	f001 fc63 	bl	203bdf0 <__boot_msg1_veneer>
 203a52a:	e7fa      	b.n	203a522 <boot_start_main+0x1a>
 203a52c:	4c35      	ldr	r4, [pc, #212]	; (203a604 <boot_start_main+0xfc>)
 203a52e:	f000 f979 	bl	203a824 <boot_upg_init_phash2>
 203a532:	f7ff ffc7 	bl	203a4c4 <boot_start_pre_check>
 203a536:	f894 3028 	ldrb.w	r3, [r4, #40]	; 0x28
 203a53a:	f043 0310 	orr.w	r3, r3, #16
 203a53e:	f884 3028 	strb.w	r3, [r4, #40]	; 0x28
 203a542:	f000 f8db 	bl	203a6fc <boot_start_check_nvfile>
 203a546:	4605      	mov	r5, r0
 203a548:	bb18      	cbnz	r0, 203a592 <boot_start_main+0x8a>
 203a54a:	f000 fb0d 	bl	203ab68 <boot_upg_check_upg_phase0>
 203a54e:	f894 3028 	ldrb.w	r3, [r4, #40]	; 0x28
 203a552:	f894 1029 	ldrb.w	r1, [r4, #41]	; 0x29
 203a556:	f3c3 1380 	ubfx	r3, r3, #6, #1
 203a55a:	9300      	str	r3, [sp, #0]
 203a55c:	f001 0201 	and.w	r2, r1, #1
 203a560:	f3c1 0382 	ubfx	r3, r1, #2, #3
 203a564:	4828      	ldr	r0, [pc, #160]	; (203a608 <boot_start_main+0x100>)
 203a566:	f3c1 0140 	ubfx	r1, r1, #1, #1
 203a56a:	f001 fc35 	bl	203bdd8 <__boot_msg4_veneer>
 203a56e:	f894 3029 	ldrb.w	r3, [r4, #41]	; 0x29
 203a572:	0799      	lsls	r1, r3, #30
 203a574:	d504      	bpl.n	203a580 <boot_start_main+0x78>
 203a576:	07da      	lsls	r2, r3, #31
 203a578:	d514      	bpl.n	203a5a4 <boot_start_main+0x9c>
 203a57a:	f013 0f1c 	tst.w	r3, #28
 203a57e:	d127      	bne.n	203a5d0 <boot_start_main+0xc8>
 203a580:	69e2      	ldr	r2, [r4, #28]
 203a582:	68e3      	ldr	r3, [r4, #12]
 203a584:	6850      	ldr	r0, [r2, #4]
 203a586:	4418      	add	r0, r3
 203a588:	f000 fe34 	bl	203b1f4 <boot_firmware_new>
 203a58c:	2000      	movs	r0, #0
 203a58e:	b003      	add	sp, #12
 203a590:	bd30      	pop	{r4, r5, pc}
 203a592:	f000 fae9 	bl	203ab68 <boot_upg_check_upg_phase0>
 203a596:	f000 fafb 	bl	203ab90 <boot_upg_check_upg_phase1>
 203a59a:	4629      	mov	r1, r5
 203a59c:	2004      	movs	r0, #4
 203a59e:	f7ff ff5f 	bl	203a460 <boot_upg_process>
 203a5a2:	e7d2      	b.n	203a54a <boot_start_main+0x42>
 203a5a4:	f000 faf4 	bl	203ab90 <boot_upg_check_upg_phase1>
 203a5a8:	4601      	mov	r1, r0
 203a5aa:	4818      	ldr	r0, [pc, #96]	; (203a60c <boot_start_main+0x104>)
 203a5ac:	f001 fc20 	bl	203bdf0 <__boot_msg1_veneer>
 203a5b0:	f894 3028 	ldrb.w	r3, [r4, #40]	; 0x28
 203a5b4:	f003 0360 	and.w	r3, r3, #96	; 0x60
 203a5b8:	2b60      	cmp	r3, #96	; 0x60
 203a5ba:	d1e1      	bne.n	203a580 <boot_start_main+0x78>
 203a5bc:	f000 fb9e 	bl	203acfc <boot_upg_is_do_upg_from_nv>
 203a5c0:	2800      	cmp	r0, #0
 203a5c2:	d0dd      	beq.n	203a580 <boot_start_main+0x78>
 203a5c4:	f894 102b 	ldrb.w	r1, [r4, #43]	; 0x2b
 203a5c8:	2006      	movs	r0, #6
 203a5ca:	f7ff ff49 	bl	203a460 <boot_upg_process>
 203a5ce:	e7d7      	b.n	203a580 <boot_start_main+0x78>
 203a5d0:	f000 fa7a 	bl	203aac8 <boot_upg_clear_upg_magic_one>
 203a5d4:	f000 fadc 	bl	203ab90 <boot_upg_check_upg_phase1>
 203a5d8:	69a2      	ldr	r2, [r4, #24]
 203a5da:	4601      	mov	r1, r0
 203a5dc:	480c      	ldr	r0, [pc, #48]	; (203a610 <boot_start_main+0x108>)
 203a5de:	f001 fb8b 	bl	203bcf8 <__boot_msg2_veneer>
 203a5e2:	f894 3028 	ldrb.w	r3, [r4, #40]	; 0x28
 203a5e6:	065b      	lsls	r3, r3, #25
 203a5e8:	d5ca      	bpl.n	203a580 <boot_start_main+0x78>
 203a5ea:	f894 102b 	ldrb.w	r1, [r4, #43]	; 0x2b
 203a5ee:	2006      	movs	r0, #6
 203a5f0:	f7ff ff36 	bl	203a460 <boot_upg_process>
 203a5f4:	e7c4      	b.n	203a580 <boot_start_main+0x78>
 203a5f6:	bf00      	nop
 203a5f8:	12121213 	.word	0x12121213
 203a5fc:	0203baf4 	.word	0x0203baf4
 203a600:	0203bae8 	.word	0x0203bae8
 203a604:	0203c3e4 	.word	0x0203c3e4
 203a608:	0203bb10 	.word	0x0203bb10
 203a60c:	0203bb1c 	.word	0x0203bb1c
 203a610:	0203bb28 	.word	0x0203bb28

0203a614 <boot_start_check_read_tbl_info>:
 203a614:	2300      	movs	r3, #0
 203a616:	b5f0      	push	{r4, r5, r6, r7, lr}
 203a618:	4c24      	ldr	r4, [pc, #144]	; (203a6ac <boot_start_check_read_tbl_info+0x98>)
 203a61a:	b083      	sub	sp, #12
 203a61c:	69e5      	ldr	r5, [r4, #28]
 203a61e:	212c      	movs	r1, #44	; 0x2c
 203a620:	462a      	mov	r2, r5
 203a622:	68e0      	ldr	r0, [r4, #12]
 203a624:	9301      	str	r3, [sp, #4]
 203a626:	f7fe fec5 	bl	20393b4 <uapi_flash_read>
 203a62a:	bb40      	cbnz	r0, 203a67e <boot_start_check_read_tbl_info+0x6a>
 203a62c:	f643 73fc 	movw	r3, #16380	; 0x3ffc
 203a630:	6869      	ldr	r1, [r5, #4]
 203a632:	3904      	subs	r1, #4
 203a634:	4299      	cmp	r1, r3
 203a636:	d827      	bhi.n	203a688 <boot_start_check_read_tbl_info+0x74>
 203a638:	68e0      	ldr	r0, [r4, #12]
 203a63a:	aa01      	add	r2, sp, #4
 203a63c:	3004      	adds	r0, #4
 203a63e:	f000 fe67 	bl	203b310 <upg_crc_block>
 203a642:	682a      	ldr	r2, [r5, #0]
 203a644:	9b01      	ldr	r3, [sp, #4]
 203a646:	4607      	mov	r7, r0
 203a648:	429a      	cmp	r2, r3
 203a64a:	d10d      	bne.n	203a668 <boot_start_check_read_tbl_info+0x54>
 203a64c:	b960      	cbnz	r0, 203a668 <boot_start_check_read_tbl_info+0x54>
 203a64e:	f7ff fdc1 	bl	203a1d4 <boot_start_check_tbl_secure_info>
 203a652:	4606      	mov	r6, r0
 203a654:	bb18      	cbnz	r0, 203a69e <boot_start_check_read_tbl_info+0x8a>
 203a656:	f894 3028 	ldrb.w	r3, [r4, #40]	; 0x28
 203a65a:	f043 0302 	orr.w	r3, r3, #2
 203a65e:	f884 3028 	strb.w	r3, [r4, #40]	; 0x28
 203a662:	4630      	mov	r0, r6
 203a664:	b003      	add	sp, #12
 203a666:	bdf0      	pop	{r4, r5, r6, r7, pc}
 203a668:	232c      	movs	r3, #44	; 0x2c
 203a66a:	f241 368d 	movw	r6, #5005	; 0x138d
 203a66e:	2200      	movs	r2, #0
 203a670:	4628      	mov	r0, r5
 203a672:	4619      	mov	r1, r3
 203a674:	f001 fb5c 	bl	203bd30 <__memset_s_veneer>
 203a678:	4630      	mov	r0, r6
 203a67a:	b003      	add	sp, #12
 203a67c:	bdf0      	pop	{r4, r5, r6, r7, pc}
 203a67e:	f241 368a 	movw	r6, #5002	; 0x138a
 203a682:	4630      	mov	r0, r6
 203a684:	b003      	add	sp, #12
 203a686:	bdf0      	pop	{r4, r5, r6, r7, pc}
 203a688:	232c      	movs	r3, #44	; 0x2c
 203a68a:	f241 368b 	movw	r6, #5003	; 0x138b
 203a68e:	4602      	mov	r2, r0
 203a690:	4619      	mov	r1, r3
 203a692:	4628      	mov	r0, r5
 203a694:	f001 fb4c 	bl	203bd30 <__memset_s_veneer>
 203a698:	4630      	mov	r0, r6
 203a69a:	b003      	add	sp, #12
 203a69c:	bdf0      	pop	{r4, r5, r6, r7, pc}
 203a69e:	232c      	movs	r3, #44	; 0x2c
 203a6a0:	463a      	mov	r2, r7
 203a6a2:	4628      	mov	r0, r5
 203a6a4:	4619      	mov	r1, r3
 203a6a6:	f001 fb43 	bl	203bd30 <__memset_s_veneer>
 203a6aa:	e7da      	b.n	203a662 <boot_start_check_read_tbl_info+0x4e>
 203a6ac:	0203c3e4 	.word	0x0203c3e4

0203a6b0 <boot_start_check_kernel>:
 203a6b0:	2300      	movs	r3, #0
 203a6b2:	b530      	push	{r4, r5, lr}
 203a6b4:	4d10      	ldr	r5, [pc, #64]	; (203a6f8 <boot_start_check_kernel+0x48>)
 203a6b6:	b083      	sub	sp, #12
 203a6b8:	69ec      	ldr	r4, [r5, #28]
 203a6ba:	9301      	str	r3, [sp, #4]
 203a6bc:	b1bc      	cbz	r4, 203a6ee <boot_start_check_kernel+0x3e>
 203a6be:	6a23      	ldr	r3, [r4, #32]
 203a6c0:	68e8      	ldr	r0, [r5, #12]
 203a6c2:	aa01      	add	r2, sp, #4
 203a6c4:	4418      	add	r0, r3
 203a6c6:	6a61      	ldr	r1, [r4, #36]	; 0x24
 203a6c8:	f000 fe22 	bl	203b310 <upg_crc_block>
 203a6cc:	6aa2      	ldr	r2, [r4, #40]	; 0x28
 203a6ce:	9b01      	ldr	r3, [sp, #4]
 203a6d0:	429a      	cmp	r2, r3
 203a6d2:	d108      	bne.n	203a6e6 <boot_start_check_kernel+0x36>
 203a6d4:	b938      	cbnz	r0, 203a6e6 <boot_start_check_kernel+0x36>
 203a6d6:	f895 3028 	ldrb.w	r3, [r5, #40]	; 0x28
 203a6da:	f043 0304 	orr.w	r3, r3, #4
 203a6de:	f885 3028 	strb.w	r3, [r5, #40]	; 0x28
 203a6e2:	b003      	add	sp, #12
 203a6e4:	bd30      	pop	{r4, r5, pc}
 203a6e6:	f241 308e 	movw	r0, #5006	; 0x138e
 203a6ea:	b003      	add	sp, #12
 203a6ec:	bd30      	pop	{r4, r5, pc}
 203a6ee:	f04f 30ff 	mov.w	r0, #4294967295	; 0xffffffff
 203a6f2:	b003      	add	sp, #12
 203a6f4:	bd30      	pop	{r4, r5, pc}
 203a6f6:	bf00      	nop
 203a6f8:	0203c3e4 	.word	0x0203c3e4

0203a6fc <boot_start_check_nvfile>:
 203a6fc:	2200      	movs	r2, #0
 203a6fe:	b530      	push	{r4, r5, lr}
 203a700:	4c14      	ldr	r4, [pc, #80]	; (203a754 <boot_start_check_nvfile+0x58>)
 203a702:	b083      	sub	sp, #12
 203a704:	69e5      	ldr	r5, [r4, #28]
 203a706:	9201      	str	r2, [sp, #4]
 203a708:	7b2b      	ldrb	r3, [r5, #12]
 203a70a:	07da      	lsls	r2, r3, #31
 203a70c:	d40a      	bmi.n	203a724 <boot_start_check_nvfile+0x28>
 203a70e:	079b      	lsls	r3, r3, #30
 203a710:	d40f      	bmi.n	203a732 <boot_start_check_nvfile+0x36>
 203a712:	2000      	movs	r0, #0
 203a714:	f894 3028 	ldrb.w	r3, [r4, #40]	; 0x28
 203a718:	f043 0308 	orr.w	r3, r3, #8
 203a71c:	f884 3028 	strb.w	r3, [r4, #40]	; 0x28
 203a720:	b003      	add	sp, #12
 203a722:	bd30      	pop	{r4, r5, pc}
 203a724:	6920      	ldr	r0, [r4, #16]
 203a726:	f7ff faf1 	bl	2039d0c <uapi_nv_check_file>
 203a72a:	b978      	cbnz	r0, 203a74c <boot_start_check_nvfile+0x50>
 203a72c:	7b2b      	ldrb	r3, [r5, #12]
 203a72e:	079b      	lsls	r3, r3, #30
 203a730:	d5ef      	bpl.n	203a712 <boot_start_check_nvfile+0x16>
 203a732:	6920      	ldr	r0, [r4, #16]
 203a734:	a901      	add	r1, sp, #4
 203a736:	f7ff fb19 	bl	2039d6c <uapi_nv_get_ver>
 203a73a:	b918      	cbnz	r0, 203a744 <boot_start_check_nvfile+0x48>
 203a73c:	68aa      	ldr	r2, [r5, #8]
 203a73e:	9b01      	ldr	r3, [sp, #4]
 203a740:	429a      	cmp	r2, r3
 203a742:	d0e6      	beq.n	203a712 <boot_start_check_nvfile+0x16>
 203a744:	f241 3090 	movw	r0, #5008	; 0x1390
 203a748:	b003      	add	sp, #12
 203a74a:	bd30      	pop	{r4, r5, pc}
 203a74c:	f241 308f 	movw	r0, #5007	; 0x138f
 203a750:	e7e6      	b.n	203a720 <boot_start_check_nvfile+0x24>
 203a752:	bf00      	nop
 203a754:	0203c3e4 	.word	0x0203c3e4

0203a758 <uint_2_multiply.constprop.0>:
 203a758:	2001      	movs	r0, #1
 203a75a:	4770      	bx	lr

0203a75c <boot_upg_init_phase1>:
 203a75c:	b530      	push	{r4, r5, lr}
 203a75e:	b097      	sub	sp, #92	; 0x5c
 203a760:	223c      	movs	r2, #60	; 0x3c
 203a762:	2100      	movs	r1, #0
 203a764:	a807      	add	r0, sp, #28
 203a766:	f001 fb2b 	bl	203bdc0 <__memset_veneer>
 203a76a:	2300      	movs	r3, #0
 203a76c:	223c      	movs	r2, #60	; 0x3c
 203a76e:	a907      	add	r1, sp, #28
 203a770:	2004      	movs	r0, #4
 203a772:	e9cd 3303 	strd	r3, r3, [sp, #12]
 203a776:	e9cd 3305 	strd	r3, r3, [sp, #20]
 203a77a:	f7ff fab7 	bl	2039cec <uapi_factory_nv_read>
 203a77e:	2210      	movs	r2, #16
 203a780:	4604      	mov	r4, r0
 203a782:	a903      	add	r1, sp, #12
 203a784:	2003      	movs	r0, #3
 203a786:	f7ff fab1 	bl	2039cec <uapi_factory_nv_read>
 203a78a:	4304      	orrs	r4, r0
 203a78c:	d002      	beq.n	203a794 <boot_upg_init_phase1+0x38>
 203a78e:	4620      	mov	r0, r4
 203a790:	b017      	add	sp, #92	; 0x5c
 203a792:	bd30      	pop	{r4, r5, pc}
 203a794:	f8bd 102c 	ldrh.w	r1, [sp, #44]	; 0x2c
 203a798:	f3c1 010b 	ubfx	r1, r1, #0, #12
 203a79c:	4608      	mov	r0, r1
 203a79e:	f7ff ffdb 	bl	203a758 <uint_2_multiply.constprop.0>
 203a7a2:	b3a0      	cbz	r0, 203a80e <boot_upg_init_phase1+0xb2>
 203a7a4:	f8bd 2028 	ldrh.w	r2, [sp, #40]	; 0x28
 203a7a8:	f3c2 020b 	ubfx	r2, r2, #0, #12
 203a7ac:	4610      	mov	r0, r2
 203a7ae:	f7ff ffd3 	bl	203a758 <uint_2_multiply.constprop.0>
 203a7b2:	b360      	cbz	r0, 203a80e <boot_upg_init_phase1+0xb2>
 203a7b4:	f8bd 3030 	ldrh.w	r3, [sp, #48]	; 0x30
 203a7b8:	f3c3 030b 	ubfx	r3, r3, #0, #12
 203a7bc:	4618      	mov	r0, r3
 203a7be:	f7ff ffcb 	bl	203a758 <uint_2_multiply.constprop.0>
 203a7c2:	b320      	cbz	r0, 203a80e <boot_upg_init_phase1+0xb2>
 203a7c4:	f8bd 5020 	ldrh.w	r5, [sp, #32]
 203a7c8:	f3c5 050b 	ubfx	r5, r5, #0, #12
 203a7cc:	4628      	mov	r0, r5
 203a7ce:	f7ff ffc3 	bl	203a758 <uint_2_multiply.constprop.0>
 203a7d2:	b1e0      	cbz	r0, 203a80e <boot_upg_init_phase1+0xb2>
 203a7d4:	980c      	ldr	r0, [sp, #48]	; 0x30
 203a7d6:	032d      	lsls	r5, r5, #12
 203a7d8:	f3c0 300b 	ubfx	r0, r0, #12, #12
 203a7dc:	4403      	add	r3, r0
 203a7de:	480e      	ldr	r0, [pc, #56]	; (203a818 <boot_upg_init_phase1+0xbc>)
 203a7e0:	9500      	str	r5, [sp, #0]
 203a7e2:	6085      	str	r5, [r0, #8]
 203a7e4:	f890 5028 	ldrb.w	r5, [r0, #40]	; 0x28
 203a7e8:	0309      	lsls	r1, r1, #12
 203a7ea:	f045 0501 	orr.w	r5, r5, #1
 203a7ee:	f880 5028 	strb.w	r5, [r0, #40]	; 0x28
 203a7f2:	9d03      	ldr	r5, [sp, #12]
 203a7f4:	0312      	lsls	r2, r2, #12
 203a7f6:	6045      	str	r5, [r0, #4]
 203a7f8:	4d08      	ldr	r5, [pc, #32]	; (203a81c <boot_upg_init_phase1+0xc0>)
 203a7fa:	031b      	lsls	r3, r3, #12
 203a7fc:	e9c0 1203 	strd	r1, r2, [r0, #12]
 203a800:	e9c0 3405 	strd	r3, r4, [r0, #20]
 203a804:	6005      	str	r5, [r0, #0]
 203a806:	4806      	ldr	r0, [pc, #24]	; (203a820 <boot_upg_init_phase1+0xc4>)
 203a808:	f001 fae6 	bl	203bdd8 <__boot_msg4_veneer>
 203a80c:	e7bf      	b.n	203a78e <boot_upg_init_phase1+0x32>
 203a80e:	f04f 34ff 	mov.w	r4, #4294967295	; 0xffffffff
 203a812:	4620      	mov	r0, r4
 203a814:	b017      	add	sp, #92	; 0x5c
 203a816:	bd30      	pop	{r4, r5, pc}
 203a818:	0203c3e4 	.word	0x0203c3e4
 203a81c:	12121213 	.word	0x12121213
 203a820:	0203bb34 	.word	0x0203bb34

0203a824 <boot_upg_init_phash2>:
 203a824:	4b04      	ldr	r3, [pc, #16]	; (203a838 <boot_upg_init_phash2+0x14>)
 203a826:	4805      	ldr	r0, [pc, #20]	; (203a83c <boot_upg_init_phash2+0x18>)
 203a828:	4905      	ldr	r1, [pc, #20]	; (203a840 <boot_upg_init_phash2+0x1c>)
 203a82a:	4a06      	ldr	r2, [pc, #24]	; (203a844 <boot_upg_init_phash2+0x20>)
 203a82c:	e9c3 0108 	strd	r0, r1, [r3, #32]
 203a830:	61da      	str	r2, [r3, #28]
 203a832:	2000      	movs	r0, #0
 203a834:	4770      	bx	lr
 203a836:	bf00      	nop
 203a838:	0203c3e4 	.word	0x0203c3e4
 203a83c:	0203c43c 	.word	0x0203c43c
 203a840:	0203c498 	.word	0x0203c498
 203a844:	0203c410 	.word	0x0203c410

0203a848 <uapi_product_get_type>:
 203a848:	4b01      	ldr	r3, [pc, #4]	; (203a850 <uapi_product_get_type+0x8>)
 203a84a:	6858      	ldr	r0, [r3, #4]
 203a84c:	4770      	bx	lr
 203a84e:	bf00      	nop
 203a850:	0203c3e4 	.word	0x0203c3e4

0203a854 <upg_check_chip_maigc>:
 203a854:	f641 5356 	movw	r3, #7510	; 0x1d56
 203a858:	4a02      	ldr	r2, [pc, #8]	; (203a864 <upg_check_chip_maigc+0x10>)
 203a85a:	4290      	cmp	r0, r2
 203a85c:	bf14      	ite	ne
 203a85e:	4618      	movne	r0, r3
 203a860:	2000      	moveq	r0, #0
 203a862:	4770      	bx	lr
 203a864:	57555557 	.word	0x57555557

0203a868 <upg_check_file_len>:
 203a868:	f641 5359 	movw	r3, #7513	; 0x1d59
 203a86c:	f1b0 7f80 	cmp.w	r0, #16777216	; 0x1000000
 203a870:	bf8c      	ite	hi
 203a872:	4618      	movhi	r0, r3
 203a874:	2000      	movls	r0, #0
 203a876:	4770      	bx	lr

0203a878 <boot_upg_check_by_file_type>:
 203a878:	b510      	push	{r4, lr}
 203a87a:	f890 3033 	ldrb.w	r3, [r0, #51]	; 0x33
 203a87e:	4c19      	ldr	r4, [pc, #100]	; (203a8e4 <boot_upg_check_by_file_type+0x6c>)
 203a880:	2b01      	cmp	r3, #1
 203a882:	69e1      	ldr	r1, [r4, #28]
 203a884:	6a62      	ldr	r2, [r4, #36]	; 0x24
 203a886:	f884 302b 	strb.w	r3, [r4, #43]	; 0x2b
 203a88a:	d01c      	beq.n	203a8c6 <boot_upg_check_by_file_type+0x4e>
 203a88c:	2b03      	cmp	r3, #3
 203a88e:	d00f      	beq.n	203a8b0 <boot_upg_check_by_file_type+0x38>
 203a890:	f890 103c 	ldrb.w	r1, [r0, #60]	; 0x3c
 203a894:	69a4      	ldr	r4, [r4, #24]
 203a896:	6c00      	ldr	r0, [r0, #64]	; 0x40
 203a898:	eb01 0141 	add.w	r1, r1, r1, lsl #1
 203a89c:	0089      	lsls	r1, r1, #2
 203a89e:	4420      	add	r0, r4
 203a8a0:	f7fe fd88 	bl	20393b4 <uapi_flash_read>
 203a8a4:	f241 3398 	movw	r3, #5016	; 0x1398
 203a8a8:	2800      	cmp	r0, #0
 203a8aa:	bf18      	it	ne
 203a8ac:	4618      	movne	r0, r3
 203a8ae:	bd10      	pop	{r4, pc}
 203a8b0:	6a81      	ldr	r1, [r0, #40]	; 0x28
 203a8b2:	6823      	ldr	r3, [r4, #0]
 203a8b4:	4299      	cmp	r1, r3
 203a8b6:	d010      	beq.n	203a8da <boot_upg_check_by_file_type+0x62>
 203a8b8:	f894 3028 	ldrb.w	r3, [r4, #40]	; 0x28
 203a8bc:	f043 0320 	orr.w	r3, r3, #32
 203a8c0:	f884 3028 	strb.w	r3, [r4, #40]	; 0x28
 203a8c4:	e7e4      	b.n	203a890 <boot_upg_check_by_file_type+0x18>
 203a8c6:	688b      	ldr	r3, [r1, #8]
 203a8c8:	6a81      	ldr	r1, [r0, #40]	; 0x28
 203a8ca:	4299      	cmp	r1, r3
 203a8cc:	d1f4      	bne.n	203a8b8 <boot_upg_check_by_file_type+0x40>
 203a8ce:	8d23      	ldrh	r3, [r4, #40]	; 0x28
 203a8d0:	f403 7381 	and.w	r3, r3, #258	; 0x102
 203a8d4:	2b02      	cmp	r3, #2
 203a8d6:	d1ef      	bne.n	203a8b8 <boot_upg_check_by_file_type+0x40>
 203a8d8:	e7da      	b.n	203a890 <boot_upg_check_by_file_type+0x18>
 203a8da:	f894 3029 	ldrb.w	r3, [r4, #41]	; 0x29
 203a8de:	07db      	lsls	r3, r3, #31
 203a8e0:	d4ea      	bmi.n	203a8b8 <boot_upg_check_by_file_type+0x40>
 203a8e2:	e7d5      	b.n	203a890 <boot_upg_check_by_file_type+0x18>
 203a8e4:	0203c3e4 	.word	0x0203c3e4

0203a8e8 <upg_check_product_list.constprop.0>:
 203a8e8:	4602      	mov	r2, r0
 203a8ea:	b508      	push	{r3, lr}
 203a8ec:	f7ff ffac 	bl	203a848 <uapi_product_get_type>
 203a8f0:	8811      	ldrh	r1, [r2, #0]
 203a8f2:	4603      	mov	r3, r0
 203a8f4:	4288      	cmp	r0, r1
 203a8f6:	d302      	bcc.n	203a8fe <upg_check_product_list.constprop.0+0x16>
 203a8f8:	8851      	ldrh	r1, [r2, #2]
 203a8fa:	4288      	cmp	r0, r1
 203a8fc:	d909      	bls.n	203a912 <upg_check_product_list.constprop.0+0x2a>
 203a8fe:	8891      	ldrh	r1, [r2, #4]
 203a900:	f641 505a 	movw	r0, #7514	; 0x1d5a
 203a904:	428b      	cmp	r3, r1
 203a906:	d303      	bcc.n	203a910 <upg_check_product_list.constprop.0+0x28>
 203a908:	88d2      	ldrh	r2, [r2, #6]
 203a90a:	4293      	cmp	r3, r2
 203a90c:	bf98      	it	ls
 203a90e:	2000      	movls	r0, #0
 203a910:	bd08      	pop	{r3, pc}
 203a912:	2000      	movs	r0, #0
 203a914:	bd08      	pop	{r3, pc}
 203a916:	bf00      	nop

0203a918 <upg_check_manufactuer>:
 203a918:	f245 7244 	movw	r2, #22340	; 0x5744
 203a91c:	f641 5357 	movw	r3, #7511	; 0x1d57
 203a920:	4290      	cmp	r0, r2
 203a922:	bf14      	ite	ne
 203a924:	4618      	movne	r0, r3
 203a926:	2000      	moveq	r0, #0
 203a928:	4770      	bx	lr
 203a92a:	bf00      	nop

0203a92c <upg_check_file_type>:
 203a92c:	f641 5258 	movw	r2, #7512	; 0x1d58
 203a930:	f000 03fd 	and.w	r3, r0, #253	; 0xfd
 203a934:	2b01      	cmp	r3, #1
 203a936:	bf14      	ite	ne
 203a938:	4610      	movne	r0, r2
 203a93a:	2000      	moveq	r0, #0
 203a93c:	4770      	bx	lr
 203a93e:	bf00      	nop

0203a940 <upg_conver_tail_addr_2_head_addr>:
 203a940:	2300      	movs	r3, #0
 203a942:	b530      	push	{r4, r5, lr}
 203a944:	b087      	sub	sp, #28
 203a946:	460d      	mov	r5, r1
 203a948:	4604      	mov	r4, r0
 203a94a:	2114      	movs	r1, #20
 203a94c:	aa01      	add	r2, sp, #4
 203a94e:	3814      	subs	r0, #20
 203a950:	e9cd 3301 	strd	r3, r3, [sp, #4]
 203a954:	e9cd 3303 	strd	r3, r3, [sp, #12]
 203a958:	9305      	str	r3, [sp, #20]
 203a95a:	f7fe fd2b 	bl	20393b4 <uapi_flash_read>
 203a95e:	b998      	cbnz	r0, 203a988 <upg_conver_tail_addr_2_head_addr+0x48>
 203a960:	4b23      	ldr	r3, [pc, #140]	; (203a9f0 <upg_conver_tail_addr_2_head_addr+0xb0>)
 203a962:	9a05      	ldr	r2, [sp, #20]
 203a964:	429a      	cmp	r2, r3
 203a966:	d111      	bne.n	203a98c <upg_conver_tail_addr_2_head_addr+0x4c>
 203a968:	9b04      	ldr	r3, [sp, #16]
 203a96a:	42a3      	cmp	r3, r4
 203a96c:	d23d      	bcs.n	203a9ea <upg_conver_tail_addr_2_head_addr+0xaa>
 203a96e:	4a21      	ldr	r2, [pc, #132]	; (203a9f4 <upg_conver_tail_addr_2_head_addr+0xb4>)
 203a970:	9901      	ldr	r1, [sp, #4]
 203a972:	4291      	cmp	r1, r2
 203a974:	4a20      	ldr	r2, [pc, #128]	; (203a9f8 <upg_conver_tail_addr_2_head_addr+0xb8>)
 203a976:	d00d      	beq.n	203a994 <upg_conver_tail_addr_2_head_addr+0x54>
 203a978:	f892 1029 	ldrb.w	r1, [r2, #41]	; 0x29
 203a97c:	f360 0100 	bfi	r1, r0, #0, #1
 203a980:	f882 1029 	strb.w	r1, [r2, #41]	; 0x29
 203a984:	1ae3      	subs	r3, r4, r3
 203a986:	602b      	str	r3, [r5, #0]
 203a988:	b007      	add	sp, #28
 203a98a:	bd30      	pop	{r4, r5, pc}
 203a98c:	f241 30c9 	movw	r0, #5065	; 0x13c9
 203a990:	b007      	add	sp, #28
 203a992:	bd30      	pop	{r4, r5, pc}
 203a994:	f64d 6c72 	movw	ip, #56946	; 0xde72
 203a998:	f892 1029 	ldrb.w	r1, [r2, #41]	; 0x29
 203a99c:	f041 0101 	orr.w	r1, r1, #1
 203a9a0:	f882 1029 	strb.w	r1, [r2, #41]	; 0x29
 203a9a4:	f8bd 100e 	ldrh.w	r1, [sp, #14]
 203a9a8:	4561      	cmp	r1, ip
 203a9aa:	d105      	bne.n	203a9b8 <upg_conver_tail_addr_2_head_addr+0x78>
 203a9ac:	f892 1029 	ldrb.w	r1, [r2, #41]	; 0x29
 203a9b0:	f041 0110 	orr.w	r1, r1, #16
 203a9b4:	f882 1029 	strb.w	r1, [r2, #41]	; 0x29
 203a9b8:	f64d 6172 	movw	r1, #56946	; 0xde72
 203a9bc:	f8bd c00c 	ldrh.w	ip, [sp, #12]
 203a9c0:	458c      	cmp	ip, r1
 203a9c2:	d105      	bne.n	203a9d0 <upg_conver_tail_addr_2_head_addr+0x90>
 203a9c4:	f892 1029 	ldrb.w	r1, [r2, #41]	; 0x29
 203a9c8:	f041 0108 	orr.w	r1, r1, #8
 203a9cc:	f882 1029 	strb.w	r1, [r2, #41]	; 0x29
 203a9d0:	f64d 6172 	movw	r1, #56946	; 0xde72
 203a9d4:	f8bd c00a 	ldrh.w	ip, [sp, #10]
 203a9d8:	458c      	cmp	ip, r1
 203a9da:	d1d3      	bne.n	203a984 <upg_conver_tail_addr_2_head_addr+0x44>
 203a9dc:	f892 1029 	ldrb.w	r1, [r2, #41]	; 0x29
 203a9e0:	f041 0104 	orr.w	r1, r1, #4
 203a9e4:	f882 1029 	strb.w	r1, [r2, #41]	; 0x29
 203a9e8:	e7cc      	b.n	203a984 <upg_conver_tail_addr_2_head_addr+0x44>
 203a9ea:	f241 30ca 	movw	r0, #5066	; 0x13ca
 203a9ee:	e7cb      	b.n	203a988 <upg_conver_tail_addr_2_head_addr+0x48>
 203a9f0:	57555557 	.word	0x57555557
 203a9f4:	a509b84e 	.word	0xa509b84e
 203a9f8:	0203c3e4 	.word	0x0203c3e4

0203a9fc <upg_get_head_from_flash.constprop.0>:
 203a9fc:	b510      	push	{r4, lr}
 203a9fe:	460c      	mov	r4, r1
 203aa00:	4906      	ldr	r1, [pc, #24]	; (203aa1c <upg_get_head_from_flash.constprop.0+0x20>)
 203aa02:	f7ff ff9d 	bl	203a940 <upg_conver_tail_addr_2_head_addr>
 203aa06:	b938      	cbnz	r0, 203aa18 <upg_get_head_from_flash.constprop.0+0x1c>
 203aa08:	4b05      	ldr	r3, [pc, #20]	; (203aa20 <upg_get_head_from_flash.constprop.0+0x24>)
 203aa0a:	4622      	mov	r2, r4
 203aa0c:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 203aa10:	215c      	movs	r1, #92	; 0x5c
 203aa12:	6998      	ldr	r0, [r3, #24]
 203aa14:	f7fe bcce 	b.w	20393b4 <uapi_flash_read>
 203aa18:	bd10      	pop	{r4, pc}
 203aa1a:	bf00      	nop
 203aa1c:	0203c3fc 	.word	0x0203c3fc
 203aa20:	0203c3e4 	.word	0x0203c3e4

0203aa24 <upg_check_head_common_info>:
 203aa24:	b510      	push	{r4, lr}
 203aa26:	4604      	mov	r4, r0
 203aa28:	6a40      	ldr	r0, [r0, #36]	; 0x24
 203aa2a:	f7ff ff1d 	bl	203a868 <upg_check_file_len>
 203aa2e:	b9b8      	cbnz	r0, 203aa60 <upg_check_head_common_info+0x3c>
 203aa30:	6820      	ldr	r0, [r4, #0]
 203aa32:	f7ff ff0f 	bl	203a854 <upg_check_chip_maigc>
 203aa36:	b9c8      	cbnz	r0, 203aa6c <upg_check_head_common_info+0x48>
 203aa38:	8e20      	ldrh	r0, [r4, #48]	; 0x30
 203aa3a:	f7ff ff6d 	bl	203a918 <upg_check_manufactuer>
 203aa3e:	b990      	cbnz	r0, 203aa66 <upg_check_head_common_info+0x42>
 203aa40:	f894 0033 	ldrb.w	r0, [r4, #51]	; 0x33
 203aa44:	7008      	strb	r0, [r1, #0]
 203aa46:	f7ff ff71 	bl	203a92c <upg_check_file_type>
 203aa4a:	b990      	cbnz	r0, 203aa72 <upg_check_head_common_info+0x4e>
 203aa4c:	f104 0034 	add.w	r0, r4, #52	; 0x34
 203aa50:	f7ff ff4a 	bl	203a8e8 <upg_check_product_list.constprop.0>
 203aa54:	f641 535a 	movw	r3, #7514	; 0x1d5a
 203aa58:	2800      	cmp	r0, #0
 203aa5a:	bf18      	it	ne
 203aa5c:	4618      	movne	r0, r3
 203aa5e:	bd10      	pop	{r4, pc}
 203aa60:	f641 5059 	movw	r0, #7513	; 0x1d59
 203aa64:	bd10      	pop	{r4, pc}
 203aa66:	f641 5057 	movw	r0, #7511	; 0x1d57
 203aa6a:	bd10      	pop	{r4, pc}
 203aa6c:	f641 5056 	movw	r0, #7510	; 0x1d56
 203aa70:	bd10      	pop	{r4, pc}
 203aa72:	f641 5058 	movw	r0, #7512	; 0x1d58
 203aa76:	bd10      	pop	{r4, pc}

0203aa78 <uapi_upg_check_head>:
 203aa78:	2300      	movs	r3, #0
 203aa7a:	b530      	push	{r4, r5, lr}
 203aa7c:	b083      	sub	sp, #12
 203aa7e:	f10d 0107 	add.w	r1, sp, #7
 203aa82:	4605      	mov	r5, r0
 203aa84:	f88d 3007 	strb.w	r3, [sp, #7]
 203aa88:	f7ff ffcc 	bl	203aa24 <upg_check_head_common_info>
 203aa8c:	4604      	mov	r4, r0
 203aa8e:	b110      	cbz	r0, 203aa96 <uapi_upg_check_head+0x1e>
 203aa90:	4620      	mov	r0, r4
 203aa92:	b003      	add	sp, #12
 203aa94:	bd30      	pop	{r4, r5, pc}
 203aa96:	f105 0058 	add.w	r0, r5, #88	; 0x58
 203aa9a:	f7ff fb19 	bl	203a0d0 <uapi_flash_partition_check_ver>
 203aa9e:	f89d 3007 	ldrb.w	r3, [sp, #7]
 203aaa2:	2b01      	cmp	r3, #1
 203aaa4:	d007      	beq.n	203aab6 <uapi_upg_check_head+0x3e>
 203aaa6:	f641 525c 	movw	r2, #7516	; 0x1d5c
 203aaaa:	2b03      	cmp	r3, #3
 203aaac:	bf18      	it	ne
 203aaae:	4614      	movne	r4, r2
 203aab0:	4620      	mov	r0, r4
 203aab2:	b003      	add	sp, #12
 203aab4:	bd30      	pop	{r4, r5, pc}
 203aab6:	f641 535b 	movw	r3, #7515	; 0x1d5b
 203aaba:	2800      	cmp	r0, #0
 203aabc:	bf18      	it	ne
 203aabe:	461c      	movne	r4, r3
 203aac0:	4620      	mov	r0, r4
 203aac2:	b003      	add	sp, #12
 203aac4:	bd30      	pop	{r4, r5, pc}
 203aac6:	bf00      	nop

0203aac8 <boot_upg_clear_upg_magic_one>:
 203aac8:	2200      	movs	r2, #0
 203aaca:	b500      	push	{lr}
 203aacc:	4911      	ldr	r1, [pc, #68]	; (203ab14 <boot_upg_clear_upg_magic_one+0x4c>)
 203aace:	b083      	sub	sp, #12
 203aad0:	f891 3029 	ldrb.w	r3, [r1, #41]	; 0x29
 203aad4:	9201      	str	r2, [sp, #4]
 203aad6:	f013 001c 	ands.w	r0, r3, #28
 203aada:	d009      	beq.n	203aaf0 <boot_upg_clear_upg_magic_one+0x28>
 203aadc:	f3c3 0082 	ubfx	r0, r3, #2, #3
 203aae0:	075b      	lsls	r3, r3, #29
 203aae2:	d408      	bmi.n	203aaf6 <boot_upg_clear_upg_magic_one+0x2e>
 203aae4:	0783      	lsls	r3, r0, #30
 203aae6:	d412      	bmi.n	203ab0e <boot_upg_clear_upg_magic_one+0x46>
 203aae8:	1080      	asrs	r0, r0, #2
 203aaea:	bf18      	it	ne
 203aaec:	2002      	movne	r0, #2
 203aaee:	d103      	bne.n	203aaf8 <boot_upg_clear_upg_magic_one+0x30>
 203aaf0:	b003      	add	sp, #12
 203aaf2:	f85d fb04 	ldr.w	pc, [sp], #4
 203aaf6:	4610      	mov	r0, r2
 203aaf8:	694a      	ldr	r2, [r1, #20]
 203aafa:	2300      	movs	r3, #0
 203aafc:	f1a2 0c0e 	sub.w	ip, r2, #14
 203ab00:	2102      	movs	r1, #2
 203ab02:	aa01      	add	r2, sp, #4
 203ab04:	eb0c 0040 	add.w	r0, ip, r0, lsl #1
 203ab08:	f7fe fc32 	bl	2039370 <uapi_flash_write>
 203ab0c:	e7f0      	b.n	203aaf0 <boot_upg_clear_upg_magic_one+0x28>
 203ab0e:	2001      	movs	r0, #1
 203ab10:	e7f2      	b.n	203aaf8 <boot_upg_clear_upg_magic_one+0x30>
 203ab12:	bf00      	nop
 203ab14:	0203c3e4 	.word	0x0203c3e4

0203ab18 <boot_upg_clear_upg_magic_all>:
 203ab18:	b5f0      	push	{r4, r5, r6, r7, lr}
 203ab1a:	2400      	movs	r4, #0
 203ab1c:	4e11      	ldr	r6, [pc, #68]	; (203ab64 <boot_upg_clear_upg_magic_all+0x4c>)
 203ab1e:	b083      	sub	sp, #12
 203ab20:	f896 0029 	ldrb.w	r0, [r6, #41]	; 0x29
 203ab24:	9401      	str	r4, [sp, #4]
 203ab26:	f010 051c 	ands.w	r5, r0, #28
 203ab2a:	d010      	beq.n	203ab4e <boot_upg_clear_upg_magic_all+0x36>
 203ab2c:	4625      	mov	r5, r4
 203ab2e:	6977      	ldr	r7, [r6, #20]
 203ab30:	3f0e      	subs	r7, #14
 203ab32:	f896 3029 	ldrb.w	r3, [r6, #41]	; 0x29
 203ab36:	f3c3 0382 	ubfx	r3, r3, #2, #3
 203ab3a:	4123      	asrs	r3, r4
 203ab3c:	f013 0f01 	tst.w	r3, #1
 203ab40:	f104 0401 	add.w	r4, r4, #1
 203ab44:	d106      	bne.n	203ab54 <boot_upg_clear_upg_magic_all+0x3c>
 203ab46:	2c03      	cmp	r4, #3
 203ab48:	f107 0702 	add.w	r7, r7, #2
 203ab4c:	d1f1      	bne.n	203ab32 <boot_upg_clear_upg_magic_all+0x1a>
 203ab4e:	4628      	mov	r0, r5
 203ab50:	b003      	add	sp, #12
 203ab52:	bdf0      	pop	{r4, r5, r6, r7, pc}
 203ab54:	2300      	movs	r3, #0
 203ab56:	4638      	mov	r0, r7
 203ab58:	2102      	movs	r1, #2
 203ab5a:	aa01      	add	r2, sp, #4
 203ab5c:	f7fe fc08 	bl	2039370 <uapi_flash_write>
 203ab60:	4305      	orrs	r5, r0
 203ab62:	e7f0      	b.n	203ab46 <boot_upg_clear_upg_magic_all+0x2e>
 203ab64:	0203c3e4 	.word	0x0203c3e4

0203ab68 <boot_upg_check_upg_phase0>:
 203ab68:	b510      	push	{r4, lr}
 203ab6a:	4c08      	ldr	r4, [pc, #32]	; (203ab8c <boot_upg_check_upg_phase0+0x24>)
 203ab6c:	6a21      	ldr	r1, [r4, #32]
 203ab6e:	b151      	cbz	r1, 203ab86 <boot_upg_check_upg_phase0+0x1e>
 203ab70:	6960      	ldr	r0, [r4, #20]
 203ab72:	f7ff ff43 	bl	203a9fc <upg_get_head_from_flash.constprop.0>
 203ab76:	b928      	cbnz	r0, 203ab84 <boot_upg_check_upg_phase0+0x1c>
 203ab78:	f894 3029 	ldrb.w	r3, [r4, #41]	; 0x29
 203ab7c:	f043 0302 	orr.w	r3, r3, #2
 203ab80:	f884 3029 	strb.w	r3, [r4, #41]	; 0x29
 203ab84:	bd10      	pop	{r4, pc}
 203ab86:	f241 30ee 	movw	r0, #5102	; 0x13ee
 203ab8a:	bd10      	pop	{r4, pc}
 203ab8c:	0203c3e4 	.word	0x0203c3e4

0203ab90 <boot_upg_check_upg_phase1>:
 203ab90:	2000      	movs	r0, #0
 203ab92:	b530      	push	{r4, r5, lr}
 203ab94:	4c1e      	ldr	r4, [pc, #120]	; (203ac10 <boot_upg_check_upg_phase1+0x80>)
 203ab96:	b089      	sub	sp, #36	; 0x24
 203ab98:	6a25      	ldr	r5, [r4, #32]
 203ab9a:	e9cd 0000 	strd	r0, r0, [sp]
 203ab9e:	e9cd 0002 	strd	r0, r0, [sp, #8]
 203aba2:	e9cd 0004 	strd	r0, r0, [sp, #16]
 203aba6:	e9cd 0006 	strd	r0, r0, [sp, #24]
 203abaa:	b34d      	cbz	r5, 203ac00 <boot_upg_check_upg_phase1+0x70>
 203abac:	f894 3029 	ldrb.w	r3, [r4, #41]	; 0x29
 203abb0:	079b      	lsls	r3, r3, #30
 203abb2:	d525      	bpl.n	203ac00 <boot_upg_check_upg_phase1+0x70>
 203abb4:	4b17      	ldr	r3, [pc, #92]	; (203ac14 <boot_upg_check_upg_phase1+0x84>)
 203abb6:	781a      	ldrb	r2, [r3, #0]
 203abb8:	b10a      	cbz	r2, 203abbe <boot_upg_check_upg_phase1+0x2e>
 203abba:	b009      	add	sp, #36	; 0x24
 203abbc:	bd30      	pop	{r4, r5, pc}
 203abbe:	2201      	movs	r2, #1
 203abc0:	4628      	mov	r0, r5
 203abc2:	701a      	strb	r2, [r3, #0]
 203abc4:	f7ff ff58 	bl	203aa78 <uapi_upg_check_head>
 203abc8:	2800      	cmp	r0, #0
 203abca:	d1f6      	bne.n	203abba <boot_upg_check_upg_phase1+0x2a>
 203abcc:	6a69      	ldr	r1, [r5, #36]	; 0x24
 203abce:	69a0      	ldr	r0, [r4, #24]
 203abd0:	466a      	mov	r2, sp
 203abd2:	3924      	subs	r1, #36	; 0x24
 203abd4:	3024      	adds	r0, #36	; 0x24
 203abd6:	f000 fbc7 	bl	203b368 <upg_hash_block>
 203abda:	b9a8      	cbnz	r0, 203ac08 <boot_upg_check_upg_phase1+0x78>
 203abdc:	2220      	movs	r2, #32
 203abde:	4669      	mov	r1, sp
 203abe0:	1d28      	adds	r0, r5, #4
 203abe2:	f001 f885 	bl	203bcf0 <__memcmp_veneer>
 203abe6:	b978      	cbnz	r0, 203ac08 <boot_upg_check_upg_phase1+0x78>
 203abe8:	4628      	mov	r0, r5
 203abea:	f7ff fe45 	bl	203a878 <boot_upg_check_by_file_type>
 203abee:	2800      	cmp	r0, #0
 203abf0:	d1e3      	bne.n	203abba <boot_upg_check_upg_phase1+0x2a>
 203abf2:	f894 3028 	ldrb.w	r3, [r4, #40]	; 0x28
 203abf6:	f043 0340 	orr.w	r3, r3, #64	; 0x40
 203abfa:	f884 3028 	strb.w	r3, [r4, #40]	; 0x28
 203abfe:	e7dc      	b.n	203abba <boot_upg_check_upg_phase1+0x2a>
 203ac00:	f04f 30ff 	mov.w	r0, #4294967295	; 0xffffffff
 203ac04:	b009      	add	sp, #36	; 0x24
 203ac06:	bd30      	pop	{r4, r5, pc}
 203ac08:	f241 3097 	movw	r0, #5015	; 0x1397
 203ac0c:	e7d5      	b.n	203abba <boot_upg_check_upg_phase1+0x2a>
 203ac0e:	bf00      	nop
 203ac10:	0203c3e4 	.word	0x0203c3e4
 203ac14:	0203c4b0 	.word	0x0203c4b0

0203ac18 <boot_upg_check_upg_phase2>:
 203ac18:	b570      	push	{r4, r5, r6, lr}
 203ac1a:	4d10      	ldr	r5, [pc, #64]	; (203ac5c <boot_upg_check_upg_phase2+0x44>)
 203ac1c:	f895 3028 	ldrb.w	r3, [r5, #40]	; 0x28
 203ac20:	065b      	lsls	r3, r3, #25
 203ac22:	d510      	bpl.n	203ac46 <boot_upg_check_upg_phase2+0x2e>
 203ac24:	6a2e      	ldr	r6, [r5, #32]
 203ac26:	f7ff fb3d 	bl	203a2a4 <boot_upg_check_secure_head>
 203ac2a:	4604      	mov	r4, r0
 203ac2c:	b948      	cbnz	r0, 203ac42 <boot_upg_check_upg_phase2+0x2a>
 203ac2e:	f896 3033 	ldrb.w	r3, [r6, #51]	; 0x33
 203ac32:	2b03      	cmp	r3, #3
 203ac34:	d00b      	beq.n	203ac4e <boot_upg_check_upg_phase2+0x36>
 203ac36:	f895 3028 	ldrb.w	r3, [r5, #40]	; 0x28
 203ac3a:	f043 0380 	orr.w	r3, r3, #128	; 0x80
 203ac3e:	f885 3028 	strb.w	r3, [r5, #40]	; 0x28
 203ac42:	4620      	mov	r0, r4
 203ac44:	bd70      	pop	{r4, r5, r6, pc}
 203ac46:	f241 349d 	movw	r4, #5021	; 0x139d
 203ac4a:	4620      	mov	r0, r4
 203ac4c:	bd70      	pop	{r4, r5, r6, pc}
 203ac4e:	f7ff fbc9 	bl	203a3e4 <boot_upg_check_secure_boot>
 203ac52:	2800      	cmp	r0, #0
 203ac54:	d0ef      	beq.n	203ac36 <boot_upg_check_upg_phase2+0x1e>
 203ac56:	4604      	mov	r4, r0
 203ac58:	e7f3      	b.n	203ac42 <boot_upg_check_upg_phase2+0x2a>
 203ac5a:	bf00      	nop
 203ac5c:	0203c3e4 	.word	0x0203c3e4

0203ac60 <boot_upg_process_boot>:
 203ac60:	2300      	movs	r3, #0
 203ac62:	b5f0      	push	{r4, r5, r6, r7, lr}
 203ac64:	4e24      	ldr	r6, [pc, #144]	; (203acf8 <boot_upg_process_boot+0x98>)
 203ac66:	b089      	sub	sp, #36	; 0x24
 203ac68:	6a75      	ldr	r5, [r6, #36]	; 0x24
 203ac6a:	9302      	str	r3, [sp, #8]
 203ac6c:	b1f5      	cbz	r5, 203acac <boot_upg_process_boot+0x4c>
 203ac6e:	782a      	ldrb	r2, [r5, #0]
 203ac70:	07d2      	lsls	r2, r2, #31
 203ac72:	d41f      	bmi.n	203acb4 <boot_upg_process_boot+0x54>
 203ac74:	68a9      	ldr	r1, [r5, #8]
 203ac76:	9102      	str	r1, [sp, #8]
 203ac78:	b1c1      	cbz	r1, 203acac <boot_upg_process_boot+0x4c>
 203ac7a:	f601 71ff 	addw	r1, r1, #4095	; 0xfff
 203ac7e:	f421 617f 	bic.w	r1, r1, #4080	; 0xff0
 203ac82:	f021 010f 	bic.w	r1, r1, #15
 203ac86:	68b0      	ldr	r0, [r6, #8]
 203ac88:	f7fe fb5a 	bl	2039340 <uapi_flash_erase>
 203ac8c:	782b      	ldrb	r3, [r5, #0]
 203ac8e:	4604      	mov	r4, r0
 203ac90:	69b7      	ldr	r7, [r6, #24]
 203ac92:	6868      	ldr	r0, [r5, #4]
 203ac94:	f003 0301 	and.w	r3, r3, #1
 203ac98:	68aa      	ldr	r2, [r5, #8]
 203ac9a:	68b1      	ldr	r1, [r6, #8]
 203ac9c:	9300      	str	r3, [sp, #0]
 203ac9e:	4438      	add	r0, r7
 203aca0:	9b02      	ldr	r3, [sp, #8]
 203aca2:	f000 fb85 	bl	203b3b0 <upg_copy_flash_2_flash>
 203aca6:	4320      	orrs	r0, r4
 203aca8:	b009      	add	sp, #36	; 0x24
 203acaa:	bdf0      	pop	{r4, r5, r6, r7, pc}
 203acac:	f04f 30ff 	mov.w	r0, #4294967295	; 0xffffffff
 203acb0:	b009      	add	sp, #36	; 0x24
 203acb2:	bdf0      	pop	{r4, r5, r6, r7, pc}
 203acb4:	e9cd 3304 	strd	r3, r3, [sp, #16]
 203acb8:	9306      	str	r3, [sp, #24]
 203acba:	f88d 301c 	strb.w	r3, [sp, #28]
 203acbe:	69b0      	ldr	r0, [r6, #24]
 203acc0:	9303      	str	r3, [sp, #12]
 203acc2:	686b      	ldr	r3, [r5, #4]
 203acc4:	210d      	movs	r1, #13
 203acc6:	4418      	add	r0, r3
 203acc8:	aa04      	add	r2, sp, #16
 203acca:	f7fe fb73 	bl	20393b4 <uapi_flash_read>
 203acce:	2800      	cmp	r0, #0
 203acd0:	d1ea      	bne.n	203aca8 <boot_upg_process_boot+0x48>
 203acd2:	210d      	movs	r1, #13
 203acd4:	ab03      	add	r3, sp, #12
 203acd6:	aa02      	add	r2, sp, #8
 203acd8:	a804      	add	r0, sp, #16
 203acda:	f7fe fc21 	bl	2039520 <uapi_lzma_get_uncompress_len>
 203acde:	2800      	cmp	r0, #0
 203ace0:	d1e2      	bne.n	203aca8 <boot_upg_process_boot+0x48>
 203ace2:	210d      	movs	r1, #13
 203ace4:	a804      	add	r0, sp, #16
 203ace6:	f7fe fc63 	bl	20395b0 <uapi_lzma_mem_detect>
 203acea:	2800      	cmp	r0, #0
 203acec:	d1dc      	bne.n	203aca8 <boot_upg_process_boot+0x48>
 203acee:	9902      	ldr	r1, [sp, #8]
 203acf0:	2900      	cmp	r1, #0
 203acf2:	d1c2      	bne.n	203ac7a <boot_upg_process_boot+0x1a>
 203acf4:	e7da      	b.n	203acac <boot_upg_process_boot+0x4c>
 203acf6:	bf00      	nop
 203acf8:	0203c3e4 	.word	0x0203c3e4

0203acfc <boot_upg_is_do_upg_from_nv>:
 203acfc:	b500      	push	{lr}
 203acfe:	b089      	sub	sp, #36	; 0x24
 203ad00:	2220      	movs	r2, #32
 203ad02:	2100      	movs	r1, #0
 203ad04:	4668      	mov	r0, sp
 203ad06:	f001 f85b 	bl	203bdc0 <__memset_veneer>
 203ad0a:	2220      	movs	r2, #32
 203ad0c:	4669      	mov	r1, sp
 203ad0e:	2007      	movs	r0, #7
 203ad10:	f7fe ffec 	bl	2039cec <uapi_factory_nv_read>
 203ad14:	b918      	cbnz	r0, 203ad1e <boot_upg_is_do_upg_from_nv+0x22>
 203ad16:	4b0d      	ldr	r3, [pc, #52]	; (203ad4c <boot_upg_is_do_upg_from_nv+0x50>)
 203ad18:	9a00      	ldr	r2, [sp, #0]
 203ad1a:	429a      	cmp	r2, r3
 203ad1c:	d003      	beq.n	203ad26 <boot_upg_is_do_upg_from_nv+0x2a>
 203ad1e:	2001      	movs	r0, #1
 203ad20:	b009      	add	sp, #36	; 0x24
 203ad22:	f85d fb04 	ldr.w	pc, [sp], #4
 203ad26:	f89d 3004 	ldrb.w	r3, [sp, #4]
 203ad2a:	f89d 2005 	ldrb.w	r2, [sp, #5]
 203ad2e:	429a      	cmp	r2, r3
 203ad30:	d3f6      	bcc.n	203ad20 <boot_upg_is_do_upg_from_nv+0x24>
 203ad32:	3301      	adds	r3, #1
 203ad34:	2220      	movs	r2, #32
 203ad36:	4669      	mov	r1, sp
 203ad38:	2007      	movs	r0, #7
 203ad3a:	f88d 3004 	strb.w	r3, [sp, #4]
 203ad3e:	f7fe ffd9 	bl	2039cf4 <uapi_factory_nv_write>
 203ad42:	2001      	movs	r0, #1
 203ad44:	b009      	add	sp, #36	; 0x24
 203ad46:	f85d fb04 	ldr.w	pc, [sp], #4
 203ad4a:	bf00      	nop
 203ad4c:	31564e55 	.word	0x31564e55

0203ad50 <boot_upg_kernel_lzma_detect>:
 203ad50:	468c      	mov	ip, r1
 203ad52:	2300      	movs	r3, #0
 203ad54:	b530      	push	{r4, r5, lr}
 203ad56:	6984      	ldr	r4, [r0, #24]
 203ad58:	f8dc 0004 	ldr.w	r0, [ip, #4]
 203ad5c:	b087      	sub	sp, #28
 203ad5e:	4615      	mov	r5, r2
 203ad60:	210d      	movs	r1, #13
 203ad62:	aa02      	add	r2, sp, #8
 203ad64:	4420      	add	r0, r4
 203ad66:	e9cd 3302 	strd	r3, r3, [sp, #8]
 203ad6a:	9301      	str	r3, [sp, #4]
 203ad6c:	9304      	str	r3, [sp, #16]
 203ad6e:	f88d 3014 	strb.w	r3, [sp, #20]
 203ad72:	f7fe fb1f 	bl	20393b4 <uapi_flash_read>
 203ad76:	210d      	movs	r1, #13
 203ad78:	4604      	mov	r4, r0
 203ad7a:	462a      	mov	r2, r5
 203ad7c:	a802      	add	r0, sp, #8
 203ad7e:	ab01      	add	r3, sp, #4
 203ad80:	f7fe fbce 	bl	2039520 <uapi_lzma_get_uncompress_len>
 203ad84:	4320      	orrs	r0, r4
 203ad86:	d103      	bne.n	203ad90 <boot_upg_kernel_lzma_detect+0x40>
 203ad88:	210d      	movs	r1, #13
 203ad8a:	a802      	add	r0, sp, #8
 203ad8c:	f7fe fc10 	bl	20395b0 <uapi_lzma_mem_detect>
 203ad90:	b007      	add	sp, #28
 203ad92:	bd30      	pop	{r4, r5, pc}

0203ad94 <boot_upg_kernel_process>:
 203ad94:	b5f0      	push	{r4, r5, r6, r7, lr}
 203ad96:	4e37      	ldr	r6, [pc, #220]	; (203ae74 <boot_upg_kernel_process+0xe0>)
 203ad98:	b085      	sub	sp, #20
 203ad9a:	6a74      	ldr	r4, [r6, #36]	; 0x24
 203ad9c:	2c00      	cmp	r4, #0
 203ad9e:	d064      	beq.n	203ae6a <boot_upg_kernel_process+0xd6>
 203ada0:	7823      	ldrb	r3, [r4, #0]
 203ada2:	07da      	lsls	r2, r3, #31
 203ada4:	d453      	bmi.n	203ae4e <boot_upg_kernel_process+0xba>
 203ada6:	68a3      	ldr	r3, [r4, #8]
 203ada8:	9302      	str	r3, [sp, #8]
 203adaa:	2b00      	cmp	r3, #0
 203adac:	d059      	beq.n	203ae62 <boot_upg_kernel_process+0xce>
 203adae:	7b23      	ldrb	r3, [r4, #12]
 203adb0:	07db      	lsls	r3, r3, #31
 203adb2:	d43d      	bmi.n	203ae30 <boot_upg_kernel_process+0x9c>
 203adb4:	6963      	ldr	r3, [r4, #20]
 203adb6:	9303      	str	r3, [sp, #12]
 203adb8:	2b00      	cmp	r3, #0
 203adba:	d044      	beq.n	203ae46 <boot_upg_kernel_process+0xb2>
 203adbc:	9d02      	ldr	r5, [sp, #8]
 203adbe:	4f2e      	ldr	r7, [pc, #184]	; (203ae78 <boot_upg_kernel_process+0xe4>)
 203adc0:	f605 75ff 	addw	r5, r5, #4095	; 0xfff
 203adc4:	f603 73ff 	addw	r3, r3, #4095	; 0xfff
 203adc8:	403d      	ands	r5, r7
 203adca:	462a      	mov	r2, r5
 203adcc:	401f      	ands	r7, r3
 203adce:	68f1      	ldr	r1, [r6, #12]
 203add0:	482a      	ldr	r0, [pc, #168]	; (203ae7c <boot_upg_kernel_process+0xe8>)
 203add2:	f000 ff91 	bl	203bcf8 <__boot_msg2_veneer>
 203add6:	463a      	mov	r2, r7
 203add8:	4828      	ldr	r0, [pc, #160]	; (203ae7c <boot_upg_kernel_process+0xe8>)
 203adda:	6931      	ldr	r1, [r6, #16]
 203addc:	f000 ff8c 	bl	203bcf8 <__boot_msg2_veneer>
 203ade0:	4629      	mov	r1, r5
 203ade2:	68f0      	ldr	r0, [r6, #12]
 203ade4:	f7fe faac 	bl	2039340 <uapi_flash_erase>
 203ade8:	4639      	mov	r1, r7
 203adea:	4605      	mov	r5, r0
 203adec:	6930      	ldr	r0, [r6, #16]
 203adee:	f7fe faa7 	bl	2039340 <uapi_flash_erase>
 203adf2:	4601      	mov	r1, r0
 203adf4:	7823      	ldrb	r3, [r4, #0]
 203adf6:	68a2      	ldr	r2, [r4, #8]
 203adf8:	f003 0301 	and.w	r3, r3, #1
 203adfc:	6860      	ldr	r0, [r4, #4]
 203adfe:	9300      	str	r3, [sp, #0]
 203ae00:	69b3      	ldr	r3, [r6, #24]
 203ae02:	430d      	orrs	r5, r1
 203ae04:	4418      	add	r0, r3
 203ae06:	68f1      	ldr	r1, [r6, #12]
 203ae08:	9b02      	ldr	r3, [sp, #8]
 203ae0a:	f000 fad1 	bl	203b3b0 <upg_copy_flash_2_flash>
 203ae0e:	7b23      	ldrb	r3, [r4, #12]
 203ae10:	6921      	ldr	r1, [r4, #16]
 203ae12:	4607      	mov	r7, r0
 203ae14:	69b0      	ldr	r0, [r6, #24]
 203ae16:	f003 0301 	and.w	r3, r3, #1
 203ae1a:	6962      	ldr	r2, [r4, #20]
 203ae1c:	4408      	add	r0, r1
 203ae1e:	9300      	str	r3, [sp, #0]
 203ae20:	6931      	ldr	r1, [r6, #16]
 203ae22:	9b03      	ldr	r3, [sp, #12]
 203ae24:	f000 fac4 	bl	203b3b0 <upg_copy_flash_2_flash>
 203ae28:	433d      	orrs	r5, r7
 203ae2a:	4328      	orrs	r0, r5
 203ae2c:	b005      	add	sp, #20
 203ae2e:	bdf0      	pop	{r4, r5, r6, r7, pc}
 203ae30:	4810      	ldr	r0, [pc, #64]	; (203ae74 <boot_upg_kernel_process+0xe0>)
 203ae32:	aa03      	add	r2, sp, #12
 203ae34:	f104 010c 	add.w	r1, r4, #12
 203ae38:	f7ff ff8a 	bl	203ad50 <boot_upg_kernel_lzma_detect>
 203ae3c:	2800      	cmp	r0, #0
 203ae3e:	d1f5      	bne.n	203ae2c <boot_upg_kernel_process+0x98>
 203ae40:	9b03      	ldr	r3, [sp, #12]
 203ae42:	2b00      	cmp	r3, #0
 203ae44:	d1ba      	bne.n	203adbc <boot_upg_kernel_process+0x28>
 203ae46:	f241 309c 	movw	r0, #5020	; 0x139c
 203ae4a:	b005      	add	sp, #20
 203ae4c:	bdf0      	pop	{r4, r5, r6, r7, pc}
 203ae4e:	4621      	mov	r1, r4
 203ae50:	4630      	mov	r0, r6
 203ae52:	aa02      	add	r2, sp, #8
 203ae54:	f7ff ff7c 	bl	203ad50 <boot_upg_kernel_lzma_detect>
 203ae58:	2800      	cmp	r0, #0
 203ae5a:	d1e7      	bne.n	203ae2c <boot_upg_kernel_process+0x98>
 203ae5c:	9b02      	ldr	r3, [sp, #8]
 203ae5e:	2b00      	cmp	r3, #0
 203ae60:	d1a5      	bne.n	203adae <boot_upg_kernel_process+0x1a>
 203ae62:	f241 309b 	movw	r0, #5019	; 0x139b
 203ae66:	b005      	add	sp, #20
 203ae68:	bdf0      	pop	{r4, r5, r6, r7, pc}
 203ae6a:	f04f 30ff 	mov.w	r0, #4294967295	; 0xffffffff
 203ae6e:	b005      	add	sp, #20
 203ae70:	bdf0      	pop	{r4, r5, r6, r7, pc}
 203ae72:	bf00      	nop
 203ae74:	0203c3e4 	.word	0x0203c3e4
 203ae78:	fffff000 	.word	0xfffff000
 203ae7c:	0203bb3c 	.word	0x0203bb3c

0203ae80 <flash_boot_verify_sign_alg>:
 203ae80:	8803      	ldrh	r3, [r0, #0]
 203ae82:	b943      	cbnz	r3, 203ae96 <flash_boot_verify_sign_alg+0x16>
 203ae84:	7883      	ldrb	r3, [r0, #2]
 203ae86:	f003 033f 	and.w	r3, r3, #63	; 0x3f
 203ae8a:	2b01      	cmp	r3, #1
 203ae8c:	bf8c      	ite	hi
 203ae8e:	f44f 509e 	movhi.w	r0, #5056	; 0x13c0
 203ae92:	2000      	movls	r0, #0
 203ae94:	4770      	bx	lr
 203ae96:	f44f 509e 	mov.w	r0, #5056	; 0x13c0
 203ae9a:	4770      	bx	lr

0203ae9c <flash_boot_verify_root_pub_key>:
 203ae9c:	2320      	movs	r3, #32
 203ae9e:	b510      	push	{r4, lr}
 203aea0:	4604      	mov	r4, r0
 203aea2:	b090      	sub	sp, #64	; 0x40
 203aea4:	2200      	movs	r2, #0
 203aea6:	4619      	mov	r1, r3
 203aea8:	4668      	mov	r0, sp
 203aeaa:	f000 ff41 	bl	203bd30 <__memset_s_veneer>
 203aeae:	466a      	mov	r2, sp
 203aeb0:	f44f 6180 	mov.w	r1, #1024	; 0x400
 203aeb4:	4620      	mov	r0, r4
 203aeb6:	f000 ff17 	bl	203bce8 <__uapi_hash_sha256_veneer>
 203aeba:	b9c8      	cbnz	r0, 203aef0 <flash_boot_verify_root_pub_key+0x54>
 203aebc:	2320      	movs	r3, #32
 203aebe:	4602      	mov	r2, r0
 203aec0:	4619      	mov	r1, r3
 203aec2:	eb0d 0003 	add.w	r0, sp, r3
 203aec6:	f000 ff33 	bl	203bd30 <__memset_s_veneer>
 203aeca:	2002      	movs	r0, #2
 203aecc:	a908      	add	r1, sp, #32
 203aece:	f000 ff53 	bl	203bd78 <__uapi_efuse_read_veneer>
 203aed2:	b988      	cbnz	r0, 203aef8 <flash_boot_verify_root_pub_key+0x5c>
 203aed4:	2220      	movs	r2, #32
 203aed6:	4668      	mov	r0, sp
 203aed8:	eb0d 0102 	add.w	r1, sp, r2
 203aedc:	f000 ff08 	bl	203bcf0 <__memcmp_veneer>
 203aee0:	f241 33b3 	movw	r3, #5043	; 0x13b3
 203aee4:	2800      	cmp	r0, #0
 203aee6:	bf0c      	ite	eq
 203aee8:	2000      	moveq	r0, #0
 203aeea:	4618      	movne	r0, r3
 203aeec:	b010      	add	sp, #64	; 0x40
 203aeee:	bd10      	pop	{r4, pc}
 203aef0:	f241 30b1 	movw	r0, #5041	; 0x13b1
 203aef4:	b010      	add	sp, #64	; 0x40
 203aef6:	bd10      	pop	{r4, pc}
 203aef8:	f241 30b2 	movw	r0, #5042	; 0x13b2
 203aefc:	b010      	add	sp, #64	; 0x40
 203aefe:	bd10      	pop	{r4, pc}

0203af00 <flash_boot_verify_sub_key>:
 203af00:	f04f 33ff 	mov.w	r3, #4294967295	; 0xffffffff
 203af04:	b5f0      	push	{r4, r5, r6, r7, lr}
 203af06:	b08d      	sub	sp, #52	; 0x34
 203af08:	4606      	mov	r6, r0
 203af0a:	460c      	mov	r4, r1
 203af0c:	2009      	movs	r0, #9
 203af0e:	a903      	add	r1, sp, #12
 203af10:	9303      	str	r3, [sp, #12]
 203af12:	f000 ff31 	bl	203bd78 <__uapi_efuse_read_veneer>
 203af16:	2800      	cmp	r0, #0
 203af18:	d139      	bne.n	203af8e <flash_boot_verify_sub_key+0x8e>
 203af1a:	68a2      	ldr	r2, [r4, #8]
 203af1c:	9b03      	ldr	r3, [sp, #12]
 203af1e:	429a      	cmp	r2, r3
 203af20:	d130      	bne.n	203af84 <flash_boot_verify_sub_key+0x84>
 203af22:	6923      	ldr	r3, [r4, #16]
 203af24:	f5b3 7f80 	cmp.w	r3, #256	; 0x100
 203af28:	d12c      	bne.n	203af84 <flash_boot_verify_sub_key+0x84>
 203af2a:	6967      	ldr	r7, [r4, #20]
 203af2c:	f5b7 7f00 	cmp.w	r7, #512	; 0x200
 203af30:	d128      	bne.n	203af84 <flash_boot_verify_sub_key+0x84>
 203af32:	1d20      	adds	r0, r4, #4
 203af34:	f7ff ffa4 	bl	203ae80 <flash_boot_verify_sign_alg>
 203af38:	4605      	mov	r5, r0
 203af3a:	bb28      	cbnz	r0, 203af88 <flash_boot_verify_sub_key+0x88>
 203af3c:	68e3      	ldr	r3, [r4, #12]
 203af3e:	2b1f      	cmp	r3, #31
 203af40:	d82a      	bhi.n	203af98 <flash_boot_verify_sub_key+0x98>
 203af42:	2320      	movs	r3, #32
 203af44:	4602      	mov	r2, r0
 203af46:	4619      	mov	r1, r3
 203af48:	a804      	add	r0, sp, #16
 203af4a:	f000 fef1 	bl	203bd30 <__memset_s_veneer>
 203af4e:	f44f 7106 	mov.w	r1, #536	; 0x218
 203af52:	4620      	mov	r0, r4
 203af54:	aa04      	add	r2, sp, #16
 203af56:	f000 fec7 	bl	203bce8 <__uapi_hash_sha256_veneer>
 203af5a:	bb00      	cbnz	r0, 203af9e <flash_boot_verify_sub_key+0x9e>
 203af5c:	ab04      	add	r3, sp, #16
 203af5e:	9300      	str	r3, [sp, #0]
 203af60:	79a3      	ldrb	r3, [r4, #6]
 203af62:	f504 7406 	add.w	r4, r4, #536	; 0x218
 203af66:	f003 033f 	and.w	r3, r3, #63	; 0x3f
 203af6a:	463a      	mov	r2, r7
 203af6c:	4630      	mov	r0, r6
 203af6e:	9401      	str	r4, [sp, #4]
 203af70:	f506 7100 	add.w	r1, r6, #512	; 0x200
 203af74:	f000 ff04 	bl	203bd80 <__uapi_rsa_verify_veneer>
 203af78:	f241 33b8 	movw	r3, #5048	; 0x13b8
 203af7c:	2800      	cmp	r0, #0
 203af7e:	bf18      	it	ne
 203af80:	461d      	movne	r5, r3
 203af82:	e001      	b.n	203af88 <flash_boot_verify_sub_key+0x88>
 203af84:	f241 35b5 	movw	r5, #5045	; 0x13b5
 203af88:	4628      	mov	r0, r5
 203af8a:	b00d      	add	sp, #52	; 0x34
 203af8c:	bdf0      	pop	{r4, r5, r6, r7, pc}
 203af8e:	f241 35b4 	movw	r5, #5044	; 0x13b4
 203af92:	4628      	mov	r0, r5
 203af94:	b00d      	add	sp, #52	; 0x34
 203af96:	bdf0      	pop	{r4, r5, r6, r7, pc}
 203af98:	f241 35b6 	movw	r5, #5046	; 0x13b6
 203af9c:	e7f4      	b.n	203af88 <flash_boot_verify_sub_key+0x88>
 203af9e:	f241 35b7 	movw	r5, #5047	; 0x13b7
 203afa2:	e7f1      	b.n	203af88 <flash_boot_verify_sub_key+0x88>

0203afa4 <flash_boot_verify_maintenance_key>:
 203afa4:	2318      	movs	r3, #24
 203afa6:	b5f0      	push	{r4, r5, r6, r7, lr}
 203afa8:	b091      	sub	sp, #68	; 0x44
 203afaa:	4606      	mov	r6, r0
 203afac:	2200      	movs	r2, #0
 203afae:	460c      	mov	r4, r1
 203afb0:	a802      	add	r0, sp, #8
 203afb2:	4619      	mov	r1, r3
 203afb4:	f000 febc 	bl	203bd30 <__memset_s_veneer>
 203afb8:	2001      	movs	r0, #1
 203afba:	a902      	add	r1, sp, #8
 203afbc:	f000 fedc 	bl	203bd78 <__uapi_efuse_read_veneer>
 203afc0:	2800      	cmp	r0, #0
 203afc2:	d140      	bne.n	203b046 <flash_boot_verify_maintenance_key+0xa2>
 203afc4:	2218      	movs	r2, #24
 203afc6:	a902      	add	r1, sp, #8
 203afc8:	f104 0010 	add.w	r0, r4, #16
 203afcc:	f000 fe90 	bl	203bcf0 <__memcmp_veneer>
 203afd0:	2800      	cmp	r0, #0
 203afd2:	d13d      	bne.n	203b050 <flash_boot_verify_maintenance_key+0xac>
 203afd4:	68e3      	ldr	r3, [r4, #12]
 203afd6:	2b18      	cmp	r3, #24
 203afd8:	d130      	bne.n	203b03c <flash_boot_verify_maintenance_key+0x98>
 203afda:	6aa3      	ldr	r3, [r4, #40]	; 0x28
 203afdc:	f5b3 7f00 	cmp.w	r3, #512	; 0x200
 203afe0:	d12c      	bne.n	203b03c <flash_boot_verify_maintenance_key+0x98>
 203afe2:	f8d4 722c 	ldr.w	r7, [r4, #556]	; 0x22c
 203afe6:	f5b7 7f80 	cmp.w	r7, #256	; 0x100
 203afea:	d127      	bne.n	203b03c <flash_boot_verify_maintenance_key+0x98>
 203afec:	1d20      	adds	r0, r4, #4
 203afee:	f7ff ff47 	bl	203ae80 <flash_boot_verify_sign_alg>
 203aff2:	4605      	mov	r5, r0
 203aff4:	bb88      	cbnz	r0, 203b05a <flash_boot_verify_maintenance_key+0xb6>
 203aff6:	2320      	movs	r3, #32
 203aff8:	4602      	mov	r2, r0
 203affa:	4619      	mov	r1, r3
 203affc:	eb0d 0003 	add.w	r0, sp, r3
 203b000:	f000 fe96 	bl	203bd30 <__memset_s_veneer>
 203b004:	f44f 710c 	mov.w	r1, #560	; 0x230
 203b008:	4620      	mov	r0, r4
 203b00a:	aa08      	add	r2, sp, #32
 203b00c:	f000 fe6c 	bl	203bce8 <__uapi_hash_sha256_veneer>
 203b010:	bb30      	cbnz	r0, 203b060 <flash_boot_verify_maintenance_key+0xbc>
 203b012:	ab08      	add	r3, sp, #32
 203b014:	9300      	str	r3, [sp, #0]
 203b016:	79a3      	ldrb	r3, [r4, #6]
 203b018:	f504 740c 	add.w	r4, r4, #560	; 0x230
 203b01c:	f003 033f 	and.w	r3, r3, #63	; 0x3f
 203b020:	463a      	mov	r2, r7
 203b022:	9401      	str	r4, [sp, #4]
 203b024:	f506 718c 	add.w	r1, r6, #280	; 0x118
 203b028:	f106 0018 	add.w	r0, r6, #24
 203b02c:	f000 fea8 	bl	203bd80 <__uapi_rsa_verify_veneer>
 203b030:	f241 33bd 	movw	r3, #5053	; 0x13bd
 203b034:	2800      	cmp	r0, #0
 203b036:	bf18      	it	ne
 203b038:	461d      	movne	r5, r3
 203b03a:	e001      	b.n	203b040 <flash_boot_verify_maintenance_key+0x9c>
 203b03c:	f241 35bb 	movw	r5, #5051	; 0x13bb
 203b040:	4628      	mov	r0, r5
 203b042:	b011      	add	sp, #68	; 0x44
 203b044:	bdf0      	pop	{r4, r5, r6, r7, pc}
 203b046:	f241 35b9 	movw	r5, #5049	; 0x13b9
 203b04a:	4628      	mov	r0, r5
 203b04c:	b011      	add	sp, #68	; 0x44
 203b04e:	bdf0      	pop	{r4, r5, r6, r7, pc}
 203b050:	f241 35ba 	movw	r5, #5050	; 0x13ba
 203b054:	4628      	mov	r0, r5
 203b056:	b011      	add	sp, #68	; 0x44
 203b058:	bdf0      	pop	{r4, r5, r6, r7, pc}
 203b05a:	f44f 559e 	mov.w	r5, #5056	; 0x13c0
 203b05e:	e7ef      	b.n	203b040 <flash_boot_verify_maintenance_key+0x9c>
 203b060:	f241 35bc 	movw	r5, #5052	; 0x13bc
 203b064:	e7ec      	b.n	203b040 <flash_boot_verify_maintenance_key+0x9c>
 203b066:	bf00      	nop

0203b068 <flash_boot_verify_bootloader>:
 203b068:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
 203b06c:	461d      	mov	r5, r3
 203b06e:	2320      	movs	r3, #32
 203b070:	4604      	mov	r4, r0
 203b072:	b08a      	sub	sp, #40	; 0x28
 203b074:	4690      	mov	r8, r2
 203b076:	460e      	mov	r6, r1
 203b078:	2200      	movs	r2, #0
 203b07a:	4619      	mov	r1, r3
 203b07c:	a802      	add	r0, sp, #8
 203b07e:	f89d 7040 	ldrb.w	r7, [sp, #64]	; 0x40
 203b082:	f000 fe55 	bl	203bd30 <__memset_s_veneer>
 203b086:	e9d4 0100 	ldrd	r0, r1, [r4]
 203b08a:	aa02      	add	r2, sp, #8
 203b08c:	f000 fe2c 	bl	203bce8 <__uapi_hash_sha256_veneer>
 203b090:	b990      	cbnz	r0, 203b0b8 <flash_boot_verify_bootloader+0x50>
 203b092:	ac02      	add	r4, sp, #8
 203b094:	463b      	mov	r3, r7
 203b096:	4641      	mov	r1, r8
 203b098:	4630      	mov	r0, r6
 203b09a:	f44f 7280 	mov.w	r2, #256	; 0x100
 203b09e:	9501      	str	r5, [sp, #4]
 203b0a0:	9400      	str	r4, [sp, #0]
 203b0a2:	f000 fe6d 	bl	203bd80 <__uapi_rsa_verify_veneer>
 203b0a6:	f241 33bf 	movw	r3, #5055	; 0x13bf
 203b0aa:	2800      	cmp	r0, #0
 203b0ac:	bf0c      	ite	eq
 203b0ae:	2000      	moveq	r0, #0
 203b0b0:	4618      	movne	r0, r3
 203b0b2:	b00a      	add	sp, #40	; 0x28
 203b0b4:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 203b0b8:	f241 30be 	movw	r0, #5054	; 0x13be
 203b0bc:	b00a      	add	sp, #40	; 0x28
 203b0be:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 203b0c2:	bf00      	nop

0203b0c4 <flash_boot_verify_proc>:
 203b0c4:	690a      	ldr	r2, [r1, #16]
 203b0c6:	f5b2 6f80 	cmp.w	r2, #1024	; 0x400
 203b0ca:	d002      	beq.n	203b0d2 <flash_boot_verify_proc+0xe>
 203b0cc:	f241 30a9 	movw	r0, #5033	; 0x13a9
 203b0d0:	4770      	bx	lr
 203b0d2:	b5f0      	push	{r4, r5, r6, r7, lr}
 203b0d4:	68cd      	ldr	r5, [r1, #12]
 203b0d6:	4606      	mov	r6, r0
 203b0d8:	4405      	add	r5, r0
 203b0da:	b085      	sub	sp, #20
 203b0dc:	4628      	mov	r0, r5
 203b0de:	460c      	mov	r4, r1
 203b0e0:	f7ff fedc 	bl	203ae9c <flash_boot_verify_root_pub_key>
 203b0e4:	b928      	cbnz	r0, 203b0f2 <flash_boot_verify_proc+0x2e>
 203b0e6:	69e3      	ldr	r3, [r4, #28]
 203b0e8:	f5b3 6f83 	cmp.w	r3, #1048	; 0x418
 203b0ec:	d003      	beq.n	203b0f6 <flash_boot_verify_proc+0x32>
 203b0ee:	f241 30ab 	movw	r0, #5035	; 0x13ab
 203b0f2:	b005      	add	sp, #20
 203b0f4:	bdf0      	pop	{r4, r5, r6, r7, pc}
 203b0f6:	69a2      	ldr	r2, [r4, #24]
 203b0f8:	4628      	mov	r0, r5
 203b0fa:	18b5      	adds	r5, r6, r2
 203b0fc:	4629      	mov	r1, r5
 203b0fe:	f7ff feff 	bl	203af00 <flash_boot_verify_sub_key>
 203b102:	bb48      	cbnz	r0, 203b158 <flash_boot_verify_proc+0x94>
 203b104:	6a27      	ldr	r7, [r4, #32]
 203b106:	b9cf      	cbnz	r7, 203b13c <flash_boot_verify_proc+0x78>
 203b108:	f105 0118 	add.w	r1, r5, #24
 203b10c:	f505 728c 	add.w	r2, r5, #280	; 0x118
 203b110:	6ae3      	ldr	r3, [r4, #44]	; 0x2c
 203b112:	a802      	add	r0, sp, #8
 203b114:	4433      	add	r3, r6
 203b116:	9302      	str	r3, [sp, #8]
 203b118:	6aa3      	ldr	r3, [r4, #40]	; 0x28
 203b11a:	9303      	str	r3, [sp, #12]
 203b11c:	6b63      	ldr	r3, [r4, #52]	; 0x34
 203b11e:	f894 4032 	ldrb.w	r4, [r4, #50]	; 0x32
 203b122:	4433      	add	r3, r6
 203b124:	f004 043f 	and.w	r4, r4, #63	; 0x3f
 203b128:	9400      	str	r4, [sp, #0]
 203b12a:	f7ff ff9d 	bl	203b068 <flash_boot_verify_bootloader>
 203b12e:	f241 33ae 	movw	r3, #5038	; 0x13ae
 203b132:	2800      	cmp	r0, #0
 203b134:	bf18      	it	ne
 203b136:	4618      	movne	r0, r3
 203b138:	b005      	add	sp, #20
 203b13a:	bdf0      	pop	{r4, r5, r6, r7, pc}
 203b13c:	4437      	add	r7, r6
 203b13e:	4639      	mov	r1, r7
 203b140:	4628      	mov	r0, r5
 203b142:	f7ff ff2f 	bl	203afa4 <flash_boot_verify_maintenance_key>
 203b146:	b920      	cbnz	r0, 203b152 <flash_boot_verify_proc+0x8e>
 203b148:	f107 012c 	add.w	r1, r7, #44	; 0x2c
 203b14c:	f507 7296 	add.w	r2, r7, #300	; 0x12c
 203b150:	e7de      	b.n	203b110 <flash_boot_verify_proc+0x4c>
 203b152:	f241 30ad 	movw	r0, #5037	; 0x13ad
 203b156:	e7cc      	b.n	203b0f2 <flash_boot_verify_proc+0x2e>
 203b158:	f241 30ac 	movw	r0, #5036	; 0x13ac
 203b15c:	e7c9      	b.n	203b0f2 <flash_boot_verify_proc+0x2e>
 203b15e:	bf00      	nop

0203b160 <flash_boot_security_boot_verify>:
 203b160:	b570      	push	{r4, r5, r6, lr}
 203b162:	4604      	mov	r4, r0
 203b164:	2300      	movs	r3, #0
 203b166:	b082      	sub	sp, #8
 203b168:	4621      	mov	r1, r4
 203b16a:	481e      	ldr	r0, [pc, #120]	; (203b1e4 <flash_boot_security_boot_verify+0x84>)
 203b16c:	f88d 3003 	strb.w	r3, [sp, #3]
 203b170:	f000 fe3e 	bl	203bdf0 <__boot_msg1_veneer>
 203b174:	4b1c      	ldr	r3, [pc, #112]	; (203b1e8 <flash_boot_security_boot_verify+0x88>)
 203b176:	6822      	ldr	r2, [r4, #0]
 203b178:	429a      	cmp	r2, r3
 203b17a:	d103      	bne.n	203b184 <flash_boot_security_boot_verify+0x24>
 203b17c:	4b1b      	ldr	r3, [pc, #108]	; (203b1ec <flash_boot_security_boot_verify+0x8c>)
 203b17e:	6be2      	ldr	r2, [r4, #60]	; 0x3c
 203b180:	429a      	cmp	r2, r3
 203b182:	d004      	beq.n	203b18e <flash_boot_security_boot_verify+0x2e>
 203b184:	f241 35a7 	movw	r5, #5031	; 0x13a7
 203b188:	4628      	mov	r0, r5
 203b18a:	b002      	add	sp, #8
 203b18c:	bd70      	pop	{r4, r5, r6, pc}
 203b18e:	2010      	movs	r0, #16
 203b190:	f10d 0103 	add.w	r1, sp, #3
 203b194:	f000 fdf0 	bl	203bd78 <__uapi_efuse_read_veneer>
 203b198:	4605      	mov	r5, r0
 203b19a:	b9e0      	cbnz	r0, 203b1d6 <flash_boot_security_boot_verify+0x76>
 203b19c:	f89d 3003 	ldrb.w	r3, [sp, #3]
 203b1a0:	2b01      	cmp	r3, #1
 203b1a2:	d1f1      	bne.n	203b188 <flash_boot_security_boot_verify+0x28>
 203b1a4:	4621      	mov	r1, r4
 203b1a6:	4620      	mov	r0, r4
 203b1a8:	9501      	str	r5, [sp, #4]
 203b1aa:	6ae6      	ldr	r6, [r4, #44]	; 0x2c
 203b1ac:	f7ff ff8a 	bl	203b0c4 <flash_boot_verify_proc>
 203b1b0:	4605      	mov	r5, r0
 203b1b2:	2800      	cmp	r0, #0
 203b1b4:	d1e8      	bne.n	203b188 <flash_boot_security_boot_verify+0x28>
 203b1b6:	200a      	movs	r0, #10
 203b1b8:	a901      	add	r1, sp, #4
 203b1ba:	f000 fddd 	bl	203bd78 <__uapi_efuse_read_veneer>
 203b1be:	b968      	cbnz	r0, 203b1dc <flash_boot_security_boot_verify+0x7c>
 203b1c0:	480b      	ldr	r0, [pc, #44]	; (203b1f0 <flash_boot_security_boot_verify+0x90>)
 203b1c2:	f000 fd9d 	bl	203bd00 <__boot_msg0_veneer>
 203b1c6:	f241 33b0 	movw	r3, #5040	; 0x13b0
 203b1ca:	59a2      	ldr	r2, [r4, r6]
 203b1cc:	9901      	ldr	r1, [sp, #4]
 203b1ce:	4211      	tst	r1, r2
 203b1d0:	bf18      	it	ne
 203b1d2:	461d      	movne	r5, r3
 203b1d4:	e7d8      	b.n	203b188 <flash_boot_security_boot_verify+0x28>
 203b1d6:	f241 35a8 	movw	r5, #5032	; 0x13a8
 203b1da:	e7d5      	b.n	203b188 <flash_boot_security_boot_verify+0x28>
 203b1dc:	f241 35af 	movw	r5, #5039	; 0x13af
 203b1e0:	e7d2      	b.n	203b188 <flash_boot_security_boot_verify+0x28>
 203b1e2:	bf00      	nop
 203b1e4:	0203bb4c 	.word	0x0203bb4c
 203b1e8:	55aa55aa 	.word	0x55aa55aa
 203b1ec:	33cc33cc 	.word	0x33cc33cc
 203b1f0:	0203bb58 	.word	0x0203bb58

0203b1f4 <boot_firmware_new>:
 203b1f4:	b510      	push	{r4, lr}
 203b1f6:	4604      	mov	r4, r0
 203b1f8:	480d      	ldr	r0, [pc, #52]	; (203b230 <boot_firmware_new+0x3c>)
 203b1fa:	4621      	mov	r1, r4
 203b1fc:	f000 fdf8 	bl	203bdf0 <__boot_msg1_veneer>
 203b200:	f04f 7200 	mov.w	r2, #33554432	; 0x2000000
 203b204:	f44f 31c8 	mov.w	r1, #102400	; 0x19000
 203b208:	4620      	mov	r0, r4
 203b20a:	f7fe f8d3 	bl	20393b4 <uapi_flash_read>
 203b20e:	b928      	cbnz	r0, 203b21c <boot_firmware_new+0x28>
 203b210:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 203b214:	f04f 7000 	mov.w	r0, #33554432	; 0x2000000
 203b218:	f7fd bfbc 	b.w	2039194 <boot_kernel>
 203b21c:	4805      	ldr	r0, [pc, #20]	; (203b234 <boot_firmware_new+0x40>)
 203b21e:	f000 fd6f 	bl	203bd00 <__boot_msg0_veneer>
 203b222:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 203b226:	f04f 7000 	mov.w	r0, #33554432	; 0x2000000
 203b22a:	f7fd bfb3 	b.w	2039194 <boot_kernel>
 203b22e:	bf00      	nop
 203b230:	0203bb64 	.word	0x0203bb64
 203b234:	0203b584 	.word	0x0203b584

0203b238 <boot_reset>:
 203b238:	2300      	movs	r3, #0
 203b23a:	461a      	mov	r2, r3
 203b23c:	4619      	mov	r1, r3
 203b23e:	4618      	mov	r0, r3
 203b240:	f000 bdae 	b.w	203bda0 <__do_reset_veneer>

0203b244 <kernel_comprss_flash_read>:
 203b244:	468c      	mov	ip, r1
 203b246:	b508      	push	{r3, lr}
 203b248:	4b04      	ldr	r3, [pc, #16]	; (203b25c <kernel_comprss_flash_read+0x18>)
 203b24a:	4611      	mov	r1, r2
 203b24c:	681b      	ldr	r3, [r3, #0]
 203b24e:	4662      	mov	r2, ip
 203b250:	4418      	add	r0, r3
 203b252:	f7fe f8af 	bl	20393b4 <uapi_flash_read>
 203b256:	2000      	movs	r0, #0
 203b258:	bd08      	pop	{r3, pc}
 203b25a:	bf00      	nop
 203b25c:	0203c4b8 	.word	0x0203c4b8

0203b260 <kernel_comprss_flash_write>:
 203b260:	b510      	push	{r4, lr}
 203b262:	4614      	mov	r4, r2
 203b264:	460a      	mov	r2, r1
 203b266:	4904      	ldr	r1, [pc, #16]	; (203b278 <kernel_comprss_flash_write+0x18>)
 203b268:	2300      	movs	r3, #0
 203b26a:	6809      	ldr	r1, [r1, #0]
 203b26c:	4408      	add	r0, r1
 203b26e:	4621      	mov	r1, r4
 203b270:	f7fe f87e 	bl	2039370 <uapi_flash_write>
 203b274:	4620      	mov	r0, r4
 203b276:	bd10      	pop	{r4, pc}
 203b278:	0203c4b4 	.word	0x0203c4b4

0203b27c <upg_hash_block_2>:
 203b27c:	e92d 4ff8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
 203b280:	4614      	mov	r4, r2
 203b282:	4681      	mov	r9, r0
 203b284:	4688      	mov	r8, r1
 203b286:	461f      	mov	r7, r3
 203b288:	f8dd b028 	ldr.w	fp, [sp, #40]	; 0x28
 203b28c:	f000 fd5c 	bl	203bd48 <__uapi_hash_start_veneer>
 203b290:	b3b4      	cbz	r4, 203b300 <upg_hash_block_2+0x84>
 203b292:	4e1e      	ldr	r6, [pc, #120]	; (203b30c <upg_hash_block_2+0x90>)
 203b294:	f1a8 0804 	sub.w	r8, r8, #4
 203b298:	eb07 0a04 	add.w	sl, r7, r4
 203b29c:	f858 3f04 	ldr.w	r3, [r8, #4]!
 203b2a0:	2500      	movs	r5, #0
 203b2a2:	b983      	cbnz	r3, 203b2c6 <upg_hash_block_2+0x4a>
 203b2a4:	e027      	b.n	203b2f6 <upg_hash_block_2+0x7a>
 203b2a6:	f8d9 0000 	ldr.w	r0, [r9]
 203b2aa:	4632      	mov	r2, r6
 203b2ac:	4621      	mov	r1, r4
 203b2ae:	4428      	add	r0, r5
 203b2b0:	f7fe f880 	bl	20393b4 <uapi_flash_read>
 203b2b4:	4621      	mov	r1, r4
 203b2b6:	4630      	mov	r0, r6
 203b2b8:	f000 fd6a 	bl	203bd90 <__uapi_hash_update_veneer>
 203b2bc:	f8d8 3000 	ldr.w	r3, [r8]
 203b2c0:	4425      	add	r5, r4
 203b2c2:	42ab      	cmp	r3, r5
 203b2c4:	d917      	bls.n	203b2f6 <upg_hash_block_2+0x7a>
 203b2c6:	1b5c      	subs	r4, r3, r5
 203b2c8:	783a      	ldrb	r2, [r7, #0]
 203b2ca:	f5b4 7f80 	cmp.w	r4, #256	; 0x100
 203b2ce:	bf28      	it	cs
 203b2d0:	f44f 7480 	movcs.w	r4, #256	; 0x100
 203b2d4:	2a00      	cmp	r2, #0
 203b2d6:	d1e6      	bne.n	203b2a6 <upg_hash_block_2+0x2a>
 203b2d8:	f8d9 2000 	ldr.w	r2, [r9]
 203b2dc:	f44f 7180 	mov.w	r1, #256	; 0x100
 203b2e0:	4630      	mov	r0, r6
 203b2e2:	4623      	mov	r3, r4
 203b2e4:	442a      	add	r2, r5
 203b2e6:	f000 fd87 	bl	203bdf8 <__memcpy_s_veneer>
 203b2ea:	2800      	cmp	r0, #0
 203b2ec:	d0e2      	beq.n	203b2b4 <upg_hash_block_2+0x38>
 203b2ee:	f04f 30ff 	mov.w	r0, #4294967295	; 0xffffffff
 203b2f2:	e8bd 8ff8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
 203b2f6:	3701      	adds	r7, #1
 203b2f8:	4557      	cmp	r7, sl
 203b2fa:	f109 0904 	add.w	r9, r9, #4
 203b2fe:	d1cd      	bne.n	203b29c <upg_hash_block_2+0x20>
 203b300:	4658      	mov	r0, fp
 203b302:	e8bd 4ff8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
 203b306:	f000 bd1b 	b.w	203bd40 <__uapi_hash_final_veneer>
 203b30a:	bf00      	nop
 203b30c:	0203c4bc 	.word	0x0203c4bc

0203b310 <upg_crc_block>:
 203b310:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
 203b314:	460f      	mov	r7, r1
 203b316:	4692      	mov	sl, r2
 203b318:	b1f1      	cbz	r1, 203b358 <upg_crc_block+0x48>
 203b31a:	2600      	movs	r6, #0
 203b31c:	4681      	mov	r9, r0
 203b31e:	4635      	mov	r5, r6
 203b320:	f8df 8040 	ldr.w	r8, [pc, #64]	; 203b364 <upg_crc_block+0x54>
 203b324:	1b7c      	subs	r4, r7, r5
 203b326:	f5b4 7f80 	cmp.w	r4, #256	; 0x100
 203b32a:	bf28      	it	cs
 203b32c:	f44f 7480 	movcs.w	r4, #256	; 0x100
 203b330:	4642      	mov	r2, r8
 203b332:	4621      	mov	r1, r4
 203b334:	eb09 0005 	add.w	r0, r9, r5
 203b338:	f7fe f83c 	bl	20393b4 <uapi_flash_read>
 203b33c:	4630      	mov	r0, r6
 203b33e:	4622      	mov	r2, r4
 203b340:	4641      	mov	r1, r8
 203b342:	f7fe f8cf 	bl	20394e4 <uapi_crc32>
 203b346:	4425      	add	r5, r4
 203b348:	42af      	cmp	r7, r5
 203b34a:	4606      	mov	r6, r0
 203b34c:	d8ea      	bhi.n	203b324 <upg_crc_block+0x14>
 203b34e:	2000      	movs	r0, #0
 203b350:	f8ca 6000 	str.w	r6, [sl]
 203b354:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
 203b358:	460e      	mov	r6, r1
 203b35a:	2000      	movs	r0, #0
 203b35c:	f8ca 6000 	str.w	r6, [sl]
 203b360:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
 203b364:	0203c4bc 	.word	0x0203c4bc

0203b368 <upg_hash_block>:
 203b368:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
 203b36c:	460e      	mov	r6, r1
 203b36e:	4680      	mov	r8, r0
 203b370:	4691      	mov	r9, r2
 203b372:	f000 fce9 	bl	203bd48 <__uapi_hash_start_veneer>
 203b376:	b1a6      	cbz	r6, 203b3a2 <upg_hash_block+0x3a>
 203b378:	2400      	movs	r4, #0
 203b37a:	4f0c      	ldr	r7, [pc, #48]	; (203b3ac <upg_hash_block+0x44>)
 203b37c:	1b35      	subs	r5, r6, r4
 203b37e:	f5b5 7f80 	cmp.w	r5, #256	; 0x100
 203b382:	bf28      	it	cs
 203b384:	f44f 7580 	movcs.w	r5, #256	; 0x100
 203b388:	463a      	mov	r2, r7
 203b38a:	4629      	mov	r1, r5
 203b38c:	eb08 0004 	add.w	r0, r8, r4
 203b390:	f7fe f810 	bl	20393b4 <uapi_flash_read>
 203b394:	442c      	add	r4, r5
 203b396:	4629      	mov	r1, r5
 203b398:	4638      	mov	r0, r7
 203b39a:	f000 fcf9 	bl	203bd90 <__uapi_hash_update_veneer>
 203b39e:	42a6      	cmp	r6, r4
 203b3a0:	d8ec      	bhi.n	203b37c <upg_hash_block+0x14>
 203b3a2:	4648      	mov	r0, r9
 203b3a4:	e8bd 43f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
 203b3a8:	f000 bcca 	b.w	203bd40 <__uapi_hash_final_veneer>
 203b3ac:	0203c4bc 	.word	0x0203c4bc

0203b3b0 <upg_copy_flash_2_flash>:
 203b3b0:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
 203b3b4:	2500      	movs	r5, #0
 203b3b6:	b087      	sub	sp, #28
 203b3b8:	f89d 4038 	ldrb.w	r4, [sp, #56]	; 0x38
 203b3bc:	4680      	mov	r8, r0
 203b3be:	4689      	mov	r9, r1
 203b3c0:	461e      	mov	r6, r3
 203b3c2:	e9cd 5502 	strd	r5, r5, [sp, #8]
 203b3c6:	9504      	str	r5, [sp, #16]
 203b3c8:	f88d 5014 	strb.w	r5, [sp, #20]
 203b3cc:	b9dc      	cbnz	r4, 203b406 <upg_copy_flash_2_flash+0x56>
 203b3ce:	b1b3      	cbz	r3, 203b3fe <upg_copy_flash_2_flash+0x4e>
 203b3d0:	4f1b      	ldr	r7, [pc, #108]	; (203b440 <upg_copy_flash_2_flash+0x90>)
 203b3d2:	1b35      	subs	r5, r6, r4
 203b3d4:	f5b5 7f80 	cmp.w	r5, #256	; 0x100
 203b3d8:	bf28      	it	cs
 203b3da:	f44f 7580 	movcs.w	r5, #256	; 0x100
 203b3de:	463a      	mov	r2, r7
 203b3e0:	4629      	mov	r1, r5
 203b3e2:	eb08 0004 	add.w	r0, r8, r4
 203b3e6:	f7fd ffe5 	bl	20393b4 <uapi_flash_read>
 203b3ea:	eb09 0004 	add.w	r0, r9, r4
 203b3ee:	2300      	movs	r3, #0
 203b3f0:	463a      	mov	r2, r7
 203b3f2:	4629      	mov	r1, r5
 203b3f4:	442c      	add	r4, r5
 203b3f6:	f7fd ffbb 	bl	2039370 <uapi_flash_write>
 203b3fa:	42a6      	cmp	r6, r4
 203b3fc:	d8e9      	bhi.n	203b3d2 <upg_copy_flash_2_flash+0x22>
 203b3fe:	2000      	movs	r0, #0
 203b400:	b007      	add	sp, #28
 203b402:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
 203b406:	4617      	mov	r7, r2
 203b408:	9300      	str	r3, [sp, #0]
 203b40a:	460b      	mov	r3, r1
 203b40c:	4601      	mov	r1, r0
 203b40e:	480d      	ldr	r0, [pc, #52]	; (203b444 <upg_copy_flash_2_flash+0x94>)
 203b410:	f000 fce2 	bl	203bdd8 <__boot_msg4_veneer>
 203b414:	aa02      	add	r2, sp, #8
 203b416:	210d      	movs	r1, #13
 203b418:	4640      	mov	r0, r8
 203b41a:	f7fd ffcb 	bl	20393b4 <uapi_flash_read>
 203b41e:	4b0a      	ldr	r3, [pc, #40]	; (203b448 <upg_copy_flash_2_flash+0x98>)
 203b420:	490a      	ldr	r1, [pc, #40]	; (203b44c <upg_copy_flash_2_flash+0x9c>)
 203b422:	f8c3 8000 	str.w	r8, [r3]
 203b426:	4b0a      	ldr	r3, [pc, #40]	; (203b450 <upg_copy_flash_2_flash+0xa0>)
 203b428:	463a      	mov	r2, r7
 203b42a:	9300      	str	r3, [sp, #0]
 203b42c:	a802      	add	r0, sp, #8
 203b42e:	f8c1 9000 	str.w	r9, [r1]
 203b432:	4b08      	ldr	r3, [pc, #32]	; (203b454 <upg_copy_flash_2_flash+0xa4>)
 203b434:	210d      	movs	r1, #13
 203b436:	f7fe f88d 	bl	2039554 <uapi_lzma_decompress>
 203b43a:	b007      	add	sp, #28
 203b43c:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
 203b440:	0203c4bc 	.word	0x0203c4bc
 203b444:	0203bb88 	.word	0x0203bb88
 203b448:	0203c4b8 	.word	0x0203c4b8
 203b44c:	0203c4b4 	.word	0x0203c4b4
 203b450:	0203b261 	.word	0x0203b261
 203b454:	0203b245 	.word	0x0203b245

0203b458 <uapi_rsa_verify_flash_content>:
 203b458:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 203b45c:	2400      	movs	r4, #0
 203b45e:	b08b      	sub	sp, #44	; 0x2c
 203b460:	4607      	mov	r7, r0
 203b462:	f44f 7080 	mov.w	r0, #256	; 0x100
 203b466:	4688      	mov	r8, r1
 203b468:	4692      	mov	sl, r2
 203b46a:	469b      	mov	fp, r3
 203b46c:	e9cd 4402 	strd	r4, r4, [sp, #8]
 203b470:	e9cd 4404 	strd	r4, r4, [sp, #16]
 203b474:	e9cd 4406 	strd	r4, r4, [sp, #24]
 203b478:	e9cd 4408 	strd	r4, r4, [sp, #32]
 203b47c:	f8dd 9050 	ldr.w	r9, [sp, #80]	; 0x50
 203b480:	f000 fc52 	bl	203bd28 <__boot_malloc_veneer>
 203b484:	b350      	cbz	r0, 203b4dc <uapi_rsa_verify_flash_content+0x84>
 203b486:	f44f 7380 	mov.w	r3, #256	; 0x100
 203b48a:	4605      	mov	r5, r0
 203b48c:	4622      	mov	r2, r4
 203b48e:	2401      	movs	r4, #1
 203b490:	4619      	mov	r1, r3
 203b492:	ae02      	add	r6, sp, #8
 203b494:	f000 fc4c 	bl	203bd30 <__memset_s_veneer>
 203b498:	f885 40ff 	strb.w	r4, [r5, #255]	; 0xff
 203b49c:	f885 40fd 	strb.w	r4, [r5, #253]	; 0xfd
 203b4a0:	465b      	mov	r3, fp
 203b4a2:	4652      	mov	r2, sl
 203b4a4:	4641      	mov	r1, r8
 203b4a6:	4638      	mov	r0, r7
 203b4a8:	9600      	str	r6, [sp, #0]
 203b4aa:	f7ff fee7 	bl	203b27c <upg_hash_block_2>
 203b4ae:	4604      	mov	r4, r0
 203b4b0:	b968      	cbnz	r0, 203b4ce <uapi_rsa_verify_flash_content+0x76>
 203b4b2:	f8d9 1004 	ldr.w	r1, [r9, #4]
 203b4b6:	f899 3008 	ldrb.w	r3, [r9, #8]
 203b4ba:	f8d9 0000 	ldr.w	r0, [r9]
 203b4be:	f44f 7280 	mov.w	r2, #256	; 0x100
 203b4c2:	e9cd 6100 	strd	r6, r1, [sp]
 203b4c6:	4629      	mov	r1, r5
 203b4c8:	f000 fc5a 	bl	203bd80 <__uapi_rsa_verify_veneer>
 203b4cc:	4604      	mov	r4, r0
 203b4ce:	4628      	mov	r0, r5
 203b4d0:	f000 fc32 	bl	203bd38 <__boot_free_veneer>
 203b4d4:	4620      	mov	r0, r4
 203b4d6:	b00b      	add	sp, #44	; 0x2c
 203b4d8:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 203b4dc:	f04f 34ff 	mov.w	r4, #4294967295	; 0xffffffff
 203b4e0:	4620      	mov	r0, r4
 203b4e2:	b00b      	add	sp, #44	; 0x2c
 203b4e4:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 203b4e8:	616c660a 	.word	0x616c660a
 203b4ec:	622d6873 	.word	0x622d6873
 203b4f0:	20746f6f 	.word	0x20746f6f
 203b4f4:	00000000 	.word	0x00000000
 203b4f8:	32323032 	.word	0x32323032
 203b4fc:	2d34302d 	.word	0x2d34302d
 203b500:	31203932 	.word	0x31203932
 203b504:	30303a30 	.word	0x30303a30
 203b508:	0030303a 	.word	0x0030303a
 203b50c:	6e727562 	.word	0x6e727562
 203b510:	64646120 	.word	0x64646120
 203b514:	00000072 	.word	0x00000072
 203b518:	73616c66 	.word	0x73616c66
 203b51c:	6e695f68 	.word	0x6e695f68
 203b520:	665f7469 	.word	0x665f7469
 203b524:	206c6961 	.word	0x206c6961
 203b528:	3d746572 	.word	0x3d746572
 203b52c:	00000000 	.word	0x00000000
 203b530:	73616c66 	.word	0x73616c66
 203b534:	61705f68 	.word	0x61705f68
 203b538:	74697472 	.word	0x74697472
 203b53c:	5f6e6f69 	.word	0x5f6e6f69
 203b540:	74696e69 	.word	0x74696e69
 203b544:	74657220 	.word	0x74657220
 203b548:	0000003d 	.word	0x0000003d
 203b54c:	74636166 	.word	0x74636166
 203b550:	5f79726f 	.word	0x5f79726f
 203b554:	725f766e 	.word	0x725f766e
 203b558:	20646165 	.word	0x20646165
 203b55c:	3d746572 	.word	0x3d746572
 203b560:	00000000 	.word	0x00000000
 203b564:	65746e69 	.word	0x65746e69
 203b568:	70757272 	.word	0x70757272
 203b56c:	20242074 	.word	0x20242074
 203b570:	00000000 	.word	0x00000000
 203b574:	746f6f62 	.word	0x746f6f62
 203b578:	72656b20 	.word	0x72656b20
 203b57c:	206c656e 	.word	0x206c656e
 203b580:	002e2e2e 	.word	0x002e2e2e
 203b584:	20495053 	.word	0x20495053
 203b588:	73616c66 	.word	0x73616c66
 203b58c:	65722068 	.word	0x65722068
 203b590:	66206461 	.word	0x66206461
 203b594:	206c6c75 	.word	0x206c6c75
 203b598:	6e72656b 	.word	0x6e72656b
 203b59c:	46206c65 	.word	0x46206c65
 203b5a0:	4853414c 	.word	0x4853414c
 203b5a4:	69616620 	.word	0x69616620
 203b5a8:	0064656c 	.word	0x0064656c
 203b5ac:	746f6f62 	.word	0x746f6f62
 203b5b0:	65737520 	.word	0x65737520
 203b5b4:	6d282064 	.word	0x6d282064
 203b5b8:	69202973 	.word	0x69202973
 203b5bc:	00002073 	.word	0x00002073
 203b5c0:	70707553 	.word	0x70707553
 203b5c4:	2074726f 	.word	0x2074726f
 203b5c8:	73616c46 	.word	0x73616c46
 203b5cc:	72702068 	.word	0x72702068
 203b5d0:	6365746f 	.word	0x6365746f
 203b5d4:	6e6f6974 	.word	0x6e6f6974
 203b5d8:	0000002e 	.word	0x0000002e
 203b5dc:	4e4b4e55 	.word	0x4e4b4e55
 203b5e0:	004e574f 	.word	0x004e574f
 203b5e4:	51353257 	.word	0x51353257
 203b5e8:	00003034 	.word	0x00003034
 203b5ec:	35324447 	.word	0x35324447
 203b5f0:	00303451 	.word	0x00303451
 203b5f4:	51353257 	.word	0x51353257
 203b5f8:	00003631 	.word	0x00003631

0203b5fc <g_flash_usr_info_tbl>:
 203b5fc:	0203b5dc 00000000 00000000 01000000     ................
 203b60c:	0000001b 0203b5e4 001340ef 00000000     .........@......
 203b61c:	00080000 0003001a 0203b5ec 001340c8     .............@..
 203b62c:	00000000 00080000 0003001a 0203b5f4     ................
 203b63c:	001540ef 00000000 00200000 0003001a     .@........ .....

0203b64c <g_crc_table>:
 203b64c:	00000000 77073096 ee0e612c 990951ba     .....0.w,a...Q..
 203b65c:	076dc419 706af48f e963a535 9e6495a3     ..m...jp5.c...d.
 203b66c:	0edb8832 79dcb8a4 e0d5e91e 97d2d988     2......y........
 203b67c:	09b64c2b 7eb17cbd e7b82d07 90bf1d91     +L...|.~.-......
 203b68c:	1db71064 6ab020f2 f3b97148 84be41de     d.... .jHq...A..
 203b69c:	1adad47d 6ddde4eb f4d4b551 83d385c7     }......mQ.......
 203b6ac:	136c9856 646ba8c0 fd62f97a 8a65c9ec     V.l...kdz.b...e.
 203b6bc:	14015c4f 63066cd9 fa0f3d63 8d080df5     O\...l.cc=......
 203b6cc:	3b6e20c8 4c69105e d56041e4 a2677172     . n;^.iL.A`.rqg.
 203b6dc:	3c03e4d1 4b04d447 d20d85fd a50ab56b     ...<G..K....k...
 203b6ec:	35b5a8fa 42b2986c dbbbc9d6 acbcf940     ...5l..B....@...
 203b6fc:	32d86ce3 45df5c75 dcd60dcf abd13d59     .l.2u\.E....Y=..
 203b70c:	26d930ac 51de003a c8d75180 bfd06116     .0.&:..Q.Q...a..
 203b71c:	21b4f4b5 56b3c423 cfba9599 b8bda50f     ...!#..V........
 203b72c:	2802b89e 5f058808 c60cd9b2 b10be924     ...(..._....$...
 203b73c:	2f6f7c87 58684c11 c1611dab b6662d3d     .|o/.LhX..a.=-f.
 203b74c:	76dc4190 01db7106 98d220bc efd5102a     .A.v.q... ..*...
 203b75c:	71b18589 06b6b51f 9fbfe4a5 e8b8d433     ...q........3...
 203b76c:	7807c9a2 0f00f934 9609a88e e10e9818     ...x4...........
 203b77c:	7f6a0dbb 086d3d2d 91646c97 e6635c01     ..j.-=m..ld..\c.
 203b78c:	6b6b51f4 1c6c6162 856530d8 f262004e     .Qkkbal..0e.N.b.
 203b79c:	6c0695ed 1b01a57b 8208f4c1 f50fc457     ...l{.......W...
 203b7ac:	65b0d9c6 12b7e950 8bbeb8ea fcb9887c     ...eP.......|...
 203b7bc:	62dd1ddf 15da2d49 8cd37cf3 fbd44c65     ...bI-...|..eL..
 203b7cc:	4db26158 3ab551ce a3bc0074 d4bb30e2     Xa.M.Q.:t....0..
 203b7dc:	4adfa541 3dd895d7 a4d1c46d d3d6f4fb     A..J...=m.......
 203b7ec:	4369e96a 346ed9fc ad678846 da60b8d0     j.iC..n4F.g...`.
 203b7fc:	44042d73 33031de5 aa0a4c5f dd0d7cc9     s-.D...3_L...|..
 203b80c:	5005713c 270241aa be0b1010 c90c2086     <q.P.A.'..... ..
 203b81c:	5768b525 206f85b3 b966d409 ce61e49f     %.hW..o ..f...a.
 203b82c:	5edef90e 29d9c998 b0d09822 c7d7a8b4     ...^...)".......
 203b83c:	59b33d17 2eb40d81 b7bd5c3b c0ba6cad     .=.Y....;\...l..
 203b84c:	edb88320 9abfb3b6 03b6e20c 74b1d29a      ..............t
 203b85c:	ead54739 9dd277af 04db2615 73dc1683     9G...w...&.....s
 203b86c:	e3630b12 94643b84 0d6d6a3e 7a6a5aa8     ..c..;d.>jm..Zjz
 203b87c:	e40ecf0b 9309ff9d 0a00ae27 7d079eb1     ........'......}
 203b88c:	f00f9344 8708a3d2 1e01f268 6906c2fe     D.......h......i
 203b89c:	f762575d 806567cb 196c3671 6e6b06e7     ]Wb..ge.q6l...kn
 203b8ac:	fed41b76 89d32be0 10da7a5a 67dd4acc     v....+..Zz...J.g
 203b8bc:	f9b9df6f 8ebeeff9 17b7be43 60b08ed5     o.......C......`
 203b8cc:	d6d6a3e8 a1d1937e 38d8c2c4 4fdff252     ....~......8R..O
 203b8dc:	d1bb67f1 a6bc5767 3fb506dd 48b2364b     .g..gW.....?K6.H
 203b8ec:	d80d2bda af0a1b4c 36034af6 41047a60     .+..L....J.6`z.A
 203b8fc:	df60efc3 a867df55 316e8eef 4669be79     ..`.U.g...n1y.iF
 203b90c:	cb61b38c bc66831a 256fd2a0 5268e236     ..a...f...o%6.hR
 203b91c:	cc0c7795 bb0b4703 220216b9 5505262f     .w...G....."/&.U
 203b92c:	c5ba3bbe b2bd0b28 2bb45a92 5cb36a04     .;..(....Z.+.j.\
 203b93c:	c2d7ffa7 b5d0cf31 2cd99e8b 5bdeae1d     ....1......,...[
 203b94c:	9b64c2b0 ec63f226 756aa39c 026d930a     ..d.&.c...ju..m.
 203b95c:	9c0906a9 eb0e363f 72076785 05005713     ....?6...g.r.W..
 203b96c:	95bf4a82 e2b87a14 7bb12bae 0cb61b38     .J...z...+.{8...
 203b97c:	92d28e9b e5d5be0d 7cdcefb7 0bdbdf21     ...........|!...
 203b98c:	86d3d2d4 f1d4e242 68ddb3f8 1fda836e     ....B......hn...
 203b99c:	81be16cd f6b9265b 6fb077e1 18b74777     ....[&...w.owG..
 203b9ac:	88085ae6 ff0f6a70 66063bca 11010b5c     .Z..pj...;.f\...
 203b9bc:	8f659eff f862ae69 616bffd3 166ccf45     ..e.i.b...kaE.l.
 203b9cc:	a00ae278 d70dd2ee 4e048354 3903b3c2     x.......T..N...9
 203b9dc:	a7672661 d06016f7 4969474d 3e6e77db     a&g...`.MGiI.wn>
 203b9ec:	aed16a4a d9d65adc 40df0b66 37d83bf0     Jj...Z..f..@.;.7
 203b9fc:	a9bcae53 debb9ec5 47b2cf7f 30b5ffe9     S..........G...0
 203ba0c:	bdbdf21c cabac28a 53b39330 24b4a3a6     ........0..S...$
 203ba1c:	bad03605 cdd70693 54de5729 23d967bf     .6......)W.T.g.#
 203ba2c:	b3667a2e c4614ab8 5d681b02 2a6f2b94     .zf..Ja...h].+o*
 203ba3c:	b40bbe37 c30c8ea1 5a05df1b 2d02ef8d     7..........Z...-
 203ba4c:	5a4c200a 415f414d 636f6c6c 69616620     . LZMA_Alloc fai
 203ba5c:	00203d6c 72646461 00000020 75636573     l= .addr ...secu
 203ba6c:	62206572 00746f6f 65736e75 65727563     re boot.unsecure
 203ba7c:	6f6f6220 00000074 20677075 5f676973      boot...upg sig_
 203ba8c:	00002031 20677075 5f676973 00002032     1 ..upg sig_2 ..
 203ba9c:	20677075 5f79656b 20325f6e 00000000     upg key_n_2 ....
 203baac:	746f6f62 6770755f 6f72705f 73736563     boot_upg_process
 203babc:	61657220 3d6e6f73 00000000 746f6f62      reason=....boot
 203bacc:	6770755f 6f72705f 73736563 6168705f     _upg_process_pha
 203badc:	20316573 3d746572 00000000 544f4f42     se1 ret=....BOOT
 203baec:	4e41505f 003d4349 52505b0a 5345434f     _PANIC=..[PROCES
 203bafc:	6f625d53 735f746f 74726174 67616d20     S]boot_start mag
 203bb0c:	003d6369 5f677075 6f666e69 00000000     ic=.upg_info....
 203bb1c:	31616870 74657220 0000003d 20316870     pha1 ret=...ph1 
 203bb2c:	3d746572 00000000 3d766e65 00000020     ret=....env= ...
 203bb3c:	474f4c5b 50555b5d 72655d47 00657361     [LOG][UPG]erase.
 203bb4c:	69726576 3d317966 00000000 69726576     verify1=....veri
 203bb5c:	31317966 00000000 746f6f62 72656b20     fy11....boot ker
 203bb6c:	206c656e 3d3d7461 3d3d3d3d 3d3d3d3d     nel at==========
 203bb7c:	3d3d3d3d 3d3d3d3d 00000000 6f63200a     ========..... co
 203bb8c:	207a7970 61726170 0000006d              pyz param...

0203bb98 <uapi_get_hw_chip_type>:
 203bb98:	4b02      	ldr	r3, [pc, #8]	; (203bba4 <uapi_get_hw_chip_type+0xc>)
 203bb9a:	6a58      	ldr	r0, [r3, #36]	; 0x24
 203bb9c:	f3c0 0040 	ubfx	r0, r0, #1, #1
 203bba0:	4770      	bx	lr
 203bba2:	bf00      	nop
 203bba4:	40030000 	.word	0x40030000

0203bba8 <spi_flash_write_sr_reg>:
 203bba8:	b5f0      	push	{r4, r5, r6, r7, lr}
 203bbaa:	2700      	movs	r7, #0
 203bbac:	b083      	sub	sp, #12
 203bbae:	4605      	mov	r5, r0
 203bbb0:	461e      	mov	r6, r3
 203bbb2:	4614      	mov	r4, r2
 203bbb4:	9701      	str	r7, [sp, #4]
 203bbb6:	bb2a      	cbnz	r2, 203bc04 <spi_flash_write_sr_reg+0x5c>
 203bbb8:	b9b6      	cbnz	r6, 203bbe8 <spi_flash_write_sr_reg+0x40>
 203bbba:	2001      	movs	r0, #1
 203bbbc:	f000 f8a8 	bl	203bd10 <__spif_write_enable_veneer>
 203bbc0:	b980      	cbnz	r0, 203bbe4 <spi_flash_write_sr_reg+0x3c>
 203bbc2:	f04f 4380 	mov.w	r3, #1073741824	; 0x40000000
 203bbc6:	3c01      	subs	r4, #1
 203bbc8:	f8c3 5308 	str.w	r5, [r3, #776]	; 0x308
 203bbcc:	0264      	lsls	r4, r4, #9
 203bbce:	9a01      	ldr	r2, [sp, #4]
 203bbd0:	f404 34ff 	and.w	r4, r4, #130560	; 0x1fe00
 203bbd4:	f044 0483 	orr.w	r4, r4, #131	; 0x83
 203bbd8:	f8c3 2400 	str.w	r2, [r3, #1024]	; 0x400
 203bbdc:	f8c3 4300 	str.w	r4, [r3, #768]	; 0x300
 203bbe0:	f000 f90e 	bl	203be00 <__spif_wait_config_start_veneer>
 203bbe4:	b003      	add	sp, #12
 203bbe6:	bdf0      	pop	{r4, r5, r6, r7, pc}
 203bbe8:	f04f 4380 	mov.w	r3, #1073741824	; 0x40000000
 203bbec:	2150      	movs	r1, #80	; 0x50
 203bbee:	2203      	movs	r2, #3
 203bbf0:	f8c3 1308 	str.w	r1, [r3, #776]	; 0x308
 203bbf4:	f8c3 2300 	str.w	r2, [r3, #768]	; 0x300
 203bbf8:	f000 f902 	bl	203be00 <__spif_wait_config_start_veneer>
 203bbfc:	2800      	cmp	r0, #0
 203bbfe:	d0e0      	beq.n	203bbc2 <spi_flash_write_sr_reg+0x1a>
 203bc00:	b003      	add	sp, #12
 203bc02:	bdf0      	pop	{r4, r5, r6, r7, pc}
 203bc04:	460a      	mov	r2, r1
 203bc06:	2104      	movs	r1, #4
 203bc08:	4623      	mov	r3, r4
 203bc0a:	eb0d 0001 	add.w	r0, sp, r1
 203bc0e:	f000 f8f3 	bl	203bdf8 <__memcpy_s_veneer>
 203bc12:	2800      	cmp	r0, #0
 203bc14:	d0d0      	beq.n	203bbb8 <spi_flash_write_sr_reg+0x10>
 203bc16:	4638      	mov	r0, r7
 203bc18:	b003      	add	sp, #12
 203bc1a:	bdf0      	pop	{r4, r5, r6, r7, pc}

0203bc1c <spi_flash_set_protect>:
 203bc1c:	b5f0      	push	{r4, r5, r6, r7, lr}
 203bc1e:	2400      	movs	r4, #0
 203bc20:	b083      	sub	sp, #12
 203bc22:	4605      	mov	r5, r0
 203bc24:	460e      	mov	r6, r1
 203bc26:	2201      	movs	r2, #1
 203bc28:	2005      	movs	r0, #5
 203bc2a:	a901      	add	r1, sp, #4
 203bc2c:	f8ad 4004 	strh.w	r4, [sp, #4]
 203bc30:	f000 f8aa 	bl	203bd88 <__spi_flash_read_reg_veneer>
 203bc34:	4604      	mov	r4, r0
 203bc36:	b110      	cbz	r0, 203bc3e <spi_flash_set_protect+0x22>
 203bc38:	4620      	mov	r0, r4
 203bc3a:	b003      	add	sp, #12
 203bc3c:	bdf0      	pop	{r4, r5, r6, r7, pc}
 203bc3e:	2201      	movs	r2, #1
 203bc40:	2035      	movs	r0, #53	; 0x35
 203bc42:	f10d 0105 	add.w	r1, sp, #5
 203bc46:	f000 f89f 	bl	203bd88 <__spi_flash_read_reg_veneer>
 203bc4a:	4604      	mov	r4, r0
 203bc4c:	2800      	cmp	r0, #0
 203bc4e:	d1f3      	bne.n	203bc38 <spi_flash_set_protect+0x1c>
 203bc50:	f89d 2004 	ldrb.w	r2, [sp, #4]
 203bc54:	f005 031f 	and.w	r3, r5, #31
 203bc58:	f002 017c 	and.w	r1, r2, #124	; 0x7c
 203bc5c:	ebb1 0f83 	cmp.w	r1, r3, lsl #2
 203bc60:	ea4f 0783 	mov.w	r7, r3, lsl #2
 203bc64:	f3c5 1540 	ubfx	r5, r5, #5, #1
 203bc68:	f89d 3005 	ldrb.w	r3, [sp, #5]
 203bc6c:	d104      	bne.n	203bc78 <spi_flash_set_protect+0x5c>
 203bc6e:	f003 0140 	and.w	r1, r3, #64	; 0x40
 203bc72:	ebb1 1f85 	cmp.w	r1, r5, lsl #6
 203bc76:	d0df      	beq.n	203bc38 <spi_flash_set_protect+0x1c>
 203bc78:	f022 027c 	bic.w	r2, r2, #124	; 0x7c
 203bc7c:	f023 0340 	bic.w	r3, r3, #64	; 0x40
 203bc80:	433a      	orrs	r2, r7
 203bc82:	ea43 1385 	orr.w	r3, r3, r5, lsl #6
 203bc86:	f88d 2004 	strb.w	r2, [sp, #4]
 203bc8a:	f88d 3005 	strb.w	r3, [sp, #5]
 203bc8e:	2202      	movs	r2, #2
 203bc90:	4633      	mov	r3, r6
 203bc92:	2001      	movs	r0, #1
 203bc94:	a901      	add	r1, sp, #4
 203bc96:	f7ff ff87 	bl	203bba8 <spi_flash_write_sr_reg>
 203bc9a:	b118      	cbz	r0, 203bca4 <spi_flash_set_protect+0x88>
 203bc9c:	4604      	mov	r4, r0
 203bc9e:	4620      	mov	r0, r4
 203bca0:	b003      	add	sp, #12
 203bca2:	bdf0      	pop	{r4, r5, r6, r7, pc}
 203bca4:	2201      	movs	r2, #1
 203bca6:	2005      	movs	r0, #5
 203bca8:	a901      	add	r1, sp, #4
 203bcaa:	f000 f86d 	bl	203bd88 <__spi_flash_read_reg_veneer>
 203bcae:	2800      	cmp	r0, #0
 203bcb0:	d1f4      	bne.n	203bc9c <spi_flash_set_protect+0x80>
 203bcb2:	2201      	movs	r2, #1
 203bcb4:	2035      	movs	r0, #53	; 0x35
 203bcb6:	f10d 0105 	add.w	r1, sp, #5
 203bcba:	f000 f865 	bl	203bd88 <__spi_flash_read_reg_veneer>
 203bcbe:	2800      	cmp	r0, #0
 203bcc0:	d1ec      	bne.n	203bc9c <spi_flash_set_protect+0x80>
 203bcc2:	f89d 3004 	ldrb.w	r3, [sp, #4]
 203bcc6:	f003 037c 	and.w	r3, r3, #124	; 0x7c
 203bcca:	429f      	cmp	r7, r3
 203bccc:	d106      	bne.n	203bcdc <spi_flash_set_protect+0xc0>
 203bcce:	f89d 3005 	ldrb.w	r3, [sp, #5]
 203bcd2:	f003 0340 	and.w	r3, r3, #64	; 0x40
 203bcd6:	ebb3 1f85 	cmp.w	r3, r5, lsl #6
 203bcda:	d0ad      	beq.n	203bc38 <spi_flash_set_protect+0x1c>
 203bcdc:	f8bd 4004 	ldrh.w	r4, [sp, #4]
 203bce0:	e7aa      	b.n	203bc38 <spi_flash_set_protect+0x1c>
 203bce2:	bf00      	nop
 203bce4:	0000      	movs	r0, r0
	...

0203bce8 <__uapi_hash_sha256_veneer>:
 203bce8:	f85f f000 	ldr.w	pc, [pc]	; 203bcec <__uapi_hash_sha256_veneer+0x4>
 203bcec:	000145a5 	.word	0x000145a5

0203bcf0 <__memcmp_veneer>:
 203bcf0:	f85f f000 	ldr.w	pc, [pc]	; 203bcf4 <__memcmp_veneer+0x4>
 203bcf4:	00016739 	.word	0x00016739

0203bcf8 <__boot_msg2_veneer>:
 203bcf8:	f85f f000 	ldr.w	pc, [pc]	; 203bcfc <__boot_msg2_veneer+0x4>
 203bcfc:	000143ad 	.word	0x000143ad

0203bd00 <__boot_msg0_veneer>:
 203bd00:	f85f f000 	ldr.w	pc, [pc]	; 203bd04 <__boot_msg0_veneer+0x4>
 203bd04:	00014379 	.word	0x00014379

0203bd08 <__boot_malloc_init_veneer>:
 203bd08:	f85f f000 	ldr.w	pc, [pc]	; 203bd0c <__boot_malloc_init_veneer+0x4>
 203bd0c:	000163b9 	.word	0x000163b9

0203bd10 <__spif_write_enable_veneer>:
 203bd10:	f85f f000 	ldr.w	pc, [pc]	; 203bd14 <__spif_write_enable_veneer+0x4>
 203bd14:	00012e19 	.word	0x00012e19

0203bd18 <__uapi_watchdog_disable_veneer>:
 203bd18:	f85f f000 	ldr.w	pc, [pc]	; 203bd1c <__uapi_watchdog_disable_veneer+0x4>
 203bd1c:	00014b01 	.word	0x00014b01

0203bd20 <__uart_auth_process_veneer>:
 203bd20:	f85f f000 	ldr.w	pc, [pc]	; 203bd24 <__uart_auth_process_veneer+0x4>
 203bd24:	00016d59 	.word	0x00016d59

0203bd28 <__boot_malloc_veneer>:
 203bd28:	f85f f000 	ldr.w	pc, [pc]	; 203bd2c <__boot_malloc_veneer+0x4>
 203bd2c:	000163c5 	.word	0x000163c5

0203bd30 <__memset_s_veneer>:
 203bd30:	f85f f000 	ldr.w	pc, [pc]	; 203bd34 <__memset_s_veneer+0x4>
 203bd34:	000165a9 	.word	0x000165a9

0203bd38 <__boot_free_veneer>:
 203bd38:	f85f f000 	ldr.w	pc, [pc]	; 203bd3c <__boot_free_veneer+0x4>
 203bd3c:	000163d1 	.word	0x000163d1

0203bd40 <__uapi_hash_final_veneer>:
 203bd40:	f85f f000 	ldr.w	pc, [pc]	; 203bd44 <__uapi_hash_final_veneer+0x4>
 203bd44:	00014559 	.word	0x00014559

0203bd48 <__uapi_hash_start_veneer>:
 203bd48:	f85f f000 	ldr.w	pc, [pc]	; 203bd4c <__uapi_hash_start_veneer+0x4>
 203bd4c:	00014459 	.word	0x00014459

0203bd50 <__strcpy_s_veneer>:
 203bd50:	f85f f000 	ldr.w	pc, [pc]	; 203bd54 <__strcpy_s_veneer+0x4>
 203bd54:	000165fd 	.word	0x000165fd

0203bd58 <__flash_write_prv_veneer>:
 203bd58:	f85f f000 	ldr.w	pc, [pc]	; 203bd5c <__flash_write_prv_veneer+0x4>
 203bd5c:	00013389 	.word	0x00013389

0203bd60 <__serial_init_veneer>:
 203bd60:	f85f f000 	ldr.w	pc, [pc]	; 203bd64 <__serial_init_veneer+0x4>
 203bd64:	000142ed 	.word	0x000142ed

0203bd68 <__cli_loop_veneer>:
 203bd68:	f85f f000 	ldr.w	pc, [pc]	; 203bd6c <__cli_loop_veneer+0x4>
 203bd6c:	0001199d 	.word	0x0001199d

0203bd70 <__flash_erase_prv_veneer>:
 203bd70:	f85f f000 	ldr.w	pc, [pc]	; 203bd74 <__flash_erase_prv_veneer+0x4>
 203bd74:	00013455 	.word	0x00013455

0203bd78 <__uapi_efuse_read_veneer>:
 203bd78:	f85f f000 	ldr.w	pc, [pc]	; 203bd7c <__uapi_efuse_read_veneer+0x4>
 203bd7c:	00012689 	.word	0x00012689

0203bd80 <__uapi_rsa_verify_veneer>:
 203bd80:	f85f f000 	ldr.w	pc, [pc]	; 203bd84 <__uapi_rsa_verify_veneer+0x4>
 203bd84:	00013b65 	.word	0x00013b65

0203bd88 <__spi_flash_read_reg_veneer>:
 203bd88:	f85f f000 	ldr.w	pc, [pc]	; 203bd8c <__spi_flash_read_reg_veneer+0x4>
 203bd8c:	00012cb5 	.word	0x00012cb5

0203bd90 <__uapi_hash_update_veneer>:
 203bd90:	f85f f000 	ldr.w	pc, [pc]	; 203bd94 <__uapi_hash_update_veneer+0x4>
 203bd94:	000144b1 	.word	0x000144b1

0203bd98 <__sfc_check_para_veneer>:
 203bd98:	f85f f000 	ldr.w	pc, [pc]	; 203bd9c <__sfc_check_para_veneer+0x4>
 203bd9c:	000131e1 	.word	0x000131e1

0203bda0 <__do_reset_veneer>:
 203bda0:	f85f f000 	ldr.w	pc, [pc]	; 203bda4 <__do_reset_veneer+0x4>
 203bda4:	00010801 	.word	0x00010801

0203bda8 <__uapi_register_malloc_veneer>:
 203bda8:	f85f f000 	ldr.w	pc, [pc]	; 203bdac <__uapi_register_malloc_veneer+0x4>
 203bdac:	00016399 	.word	0x00016399

0203bdb0 <__flash_read_prv_veneer>:
 203bdb0:	f85f f000 	ldr.w	pc, [pc]	; 203bdb4 <__flash_read_prv_veneer+0x4>
 203bdb4:	00013461 	.word	0x00013461

0203bdb8 <__spi_flash_basic_info_probe_veneer>:
 203bdb8:	f85f f000 	ldr.w	pc, [pc]	; 203bdbc <__spi_flash_basic_info_probe_veneer+0x4>
 203bdbc:	000134bd 	.word	0x000134bd

0203bdc0 <__memset_veneer>:
 203bdc0:	f85f f000 	ldr.w	pc, [pc]	; 203bdc4 <__memset_veneer+0x4>
 203bdc4:	000166ed 	.word	0x000166ed

0203bdc8 <__uapi_watchdog_enable_veneer>:
 203bdc8:	f85f f000 	ldr.w	pc, [pc]	; 203bdcc <__uapi_watchdog_enable_veneer+0x4>
 203bdcc:	00014b21 	.word	0x00014b21

0203bdd0 <__uapi_cmd_regist_flash_cmd_veneer>:
 203bdd0:	f85f f000 	ldr.w	pc, [pc]	; 203bdd4 <__uapi_cmd_regist_flash_cmd_veneer+0x4>
 203bdd4:	00011289 	.word	0x00011289

0203bdd8 <__boot_msg4_veneer>:
 203bdd8:	f85f f000 	ldr.w	pc, [pc]	; 203bddc <__boot_msg4_veneer+0x4>
 203bddc:	000143e1 	.word	0x000143e1

0203bde0 <__LzmaDecode2_veneer>:
 203bde0:	f85f f000 	ldr.w	pc, [pc]	; 203bde4 <__LzmaDecode2_veneer+0x4>
 203bde4:	000161cd 	.word	0x000161cd

0203bde8 <__uapi_time_counter_getms_veneer>:
 203bde8:	f85f f000 	ldr.w	pc, [pc]	; 203bdec <__uapi_time_counter_getms_veneer+0x4>
 203bdec:	00014a71 	.word	0x00014a71

0203bdf0 <__boot_msg1_veneer>:
 203bdf0:	f85f f000 	ldr.w	pc, [pc]	; 203bdf4 <__boot_msg1_veneer+0x4>
 203bdf4:	0001438d 	.word	0x0001438d

0203bdf8 <__memcpy_s_veneer>:
 203bdf8:	f85f f000 	ldr.w	pc, [pc]	; 203bdfc <__memcpy_s_veneer+0x4>
 203bdfc:	00016565 	.word	0x00016565

0203be00 <__spif_wait_config_start_veneer>:
 203be00:	f85f f000 	ldr.w	pc, [pc]	; 203be04 <__spif_wait_config_start_veneer+0x4>
 203be04:	00012c45 	.word	0x00012c45
