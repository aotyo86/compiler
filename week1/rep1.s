	.text
	.syntax unified
	.eabi_attribute	67, "2.09"	@ Tag_conformance
	.cpu	arm7tdmi
	.eabi_attribute	6, 2	@ Tag_CPU_arch
	.eabi_attribute	8, 1	@ Tag_ARM_ISA_use
	.eabi_attribute	9, 1	@ Tag_THUMB_ISA_use
	.eabi_attribute	34, 0	@ Tag_CPU_unaligned_access
	.eabi_attribute	17, 1	@ Tag_ABI_PCS_GOT_use
	.eabi_attribute	20, 1	@ Tag_ABI_FP_denormal
	.eabi_attribute	21, 0	@ Tag_ABI_FP_exceptions
	.eabi_attribute	23, 3	@ Tag_ABI_FP_number_model
	.eabi_attribute	24, 1	@ Tag_ABI_align_needed
	.eabi_attribute	25, 1	@ Tag_ABI_align_preserved
	.eabi_attribute	38, 1	@ Tag_ABI_FP_16bit_format
	.eabi_attribute	18, 4	@ Tag_ABI_PCS_wchar_t
	.eabi_attribute	26, 2	@ Tag_ABI_enum_size
	.eabi_attribute	14, 0	@ Tag_ABI_PCS_R9_use
	.file	"rep1.c"
	.globl	f                               @ -- Begin function f
	.p2align	2
	.type	f,%function
	.code	32                              @ @f
f:
	.fnstart
@ %bb.0:
	sub	sp, sp, #8
	str	r0, [sp, #4]
	str	r1, [sp]
	ldr	r0, [sp, #4]
	ldr	r1, [sp]
	add	r0, r0, r1
	add	sp, sp, #8
	bx	lr
.Lfunc_end0:
	.size	f, .Lfunc_end0-f
	.cantunwind
	.fnend
                                        @ -- End function
	.globl	main                            @ -- Begin function main
	.p2align	2
	.type	main,%function
	.code	32                              @ @main
main:
	.fnstart
@ %bb.0:
	push	{r11, lr}
	mov	r11, sp
	sub	sp, sp, #16
	mov	r0, #0
	str	r0, [sp]                        @ 4-byte Spill
	str	r0, [r11, #-4]
	mov	r0, #1
	str	r0, [sp, #8]
	mov	r0, #2
	str	r0, [sp, #4]
	ldr	r0, [sp, #8]
	ldr	r1, [sp, #4]
	bl	f
	mov	r1, r0
	ldr	r2, [sp, #8]
	ldr	r3, [sp, #4]
	ldr	r0, .LCPI1_0
	bl	printf
                                        @ kill: def $r1 killed $r0
	ldr	r0, [sp]                        @ 4-byte Reload
	mov	sp, r11
	pop	{r11, lr}
	bx	lr
	.p2align	2
@ %bb.1:
.LCPI1_0:
	.long	.L.str
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cantunwind
	.fnend
                                        @ -- End function
	.type	.L.str,%object                  @ @.str
	.section	.rodata.str1.1,"aMS",%progbits,1
.L.str:
	.asciz	"%x = %x + %x \n"
	.size	.L.str, 15

	.ident	"Apple clang version 15.0.0 (clang-1500.3.9.4)"
	.section	".note.GNU-stack","",%progbits
	.addrsig
	.addrsig_sym f
	.addrsig_sym printf
