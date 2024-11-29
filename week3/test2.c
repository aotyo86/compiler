.section    __TEXT,__text,regular,pure_instructions
    .build_version macos, 12, 0 sdk_version 12, 0
    .globl  _test                           ## -- Begin function test
    .p2align    4, 0x90
_test:                                  ## @test
    .cfi_startproc
## %bb.0:
    pushq   %rbp
    .cfi_def_cfa_offset 16
    .cfi_offset %rbp, -16
    movq    %rsp, %rbp
    .cfi_def_cfa_register %rbp
    movq    %rdi, -8(%rbp)
    movq    %rsi, -16(%rbp)
    movq    (%rdi),%rax
    popq    %rbp
    retq
    .cfi_endproc
                                        ## -- End function
    .globl  _main                           ## -- Begin function main
    .p2align    4, 0x90
_main:                                  ## @main
    .cfi_startproc
## %bb.0:
    pushq   %rbp
    .cfi_def_cfa_offset 16
    .cfi_offset %rbp, -16
    movq    %rsp, %rbp
    .cfi_def_cfa_register %rbp
    subq    $16, %rsp
    movl    $0, -4(%rbp)
    movq    $0, -16(%rbp)
    movq    -16(%rbp), %rsi
    leaq    _a(%rip), %rdi
    callq   _test
    movq    %rax, -16(%rbp)
    movq    -16(%rbp), %rsi
    leaq    L_.str(%rip), %rdi
    movb    $0, %al
    callq   _printf
    xorl    %eax, %eax
    addq    $16, %rsp
    popq    %rbp
    retq
    .cfi_endproc
                                        ## -- End function
    .section    __DATA,__data
    .globl  _a                              ## @a
_a:
    .ascii  "\001\002\003\004\005\006\007\bU\022"

    .section    __TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
    .asciz  "x = %lx\n"

.subsections_via_symbols
