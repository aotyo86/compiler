	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 14, 0	sdk_version 14, 4
	.globl	_main                           ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movl	$0, -4(%rbp)
	leaq	_check(%rip), %rax
	movq	%rax, -16(%rbp)
	movq	-16(%rbp), %rax
	movb	1(%rax), %al
	movb	%al, -5(%rbp)
	movsbl	-5(%rbp), %eax
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.section	__DATA,__data
	.globl	_check                          ## @check
	.p2align	2, 0x0
_check:
	.long	305419896                       ## 0x12345678

.subsections_via_symbols
