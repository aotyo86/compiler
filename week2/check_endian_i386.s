	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 14, 0	sdk_version 14, 4
	.globl	_main                           ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## %bb.0:
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset %ebp, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register %ebp
	subl	$12, %esp
	calll	L0$pb
L0$pb:
	popl	%eax
	movl	$0, -4(%ebp)
	leal	_check-L0$pb(%eax), %eax
	movl	%eax, -12(%ebp)
	movl	-12(%ebp), %eax
	movb	1(%eax), %al
	movb	%al, -5(%ebp)
	movsbl	-5(%ebp), %eax
	addl	$12, %esp
	popl	%ebp
	retl
	.cfi_endproc
                                        ## -- End function
	.section	__DATA,__data
	.globl	_check                          ## @check
	.p2align	2, 0x0
_check:
	.long	305419896                       ## 0x12345678

.subsections_via_symbols
