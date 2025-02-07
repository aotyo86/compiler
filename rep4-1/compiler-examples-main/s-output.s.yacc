      .cstring
LC0:
  .ascii "%d\n\0"
    .text
.globl _print
_print:
LFB2:
    pushq   %rbp
LCFI0:
    movq    %rsp, %rbp
LCFI1:
    subq    $16, %rsp
LCFI2:
    movl    %edi, -4(%rbp)
    movl    -4(%rbp), %esi
    leaq    LC0(%rip), %rdi
    movl    $0, %eax
    call    _printf
    leave
    ret
LFE2:
.globl _main
_main:
LFB3:
    pushq   %rbp
LCFI3:
    movq    %rsp, %rbp
LCFI4:
    subq    $16, %rsp
LCFI5:


##### 1
	movq $1,%rax
	movq %rax, %rdi
	call _print
## 1
##### 1+2			//  expr
	movq $3,%rax
	movq %rax, %rdi
	call _print
## 1+2			//  expr
##### 2*3			//  mexpr
	movq $6,%rax
	movq %rax, %rdi
	call _print
## 2*3			//  mexpr
##### 1+(2*3)			// term
	movq $7,%rax
	movq %rax, %rdi
	call _print
## 1+(2*3)			// term
##### 1-(2-3)			// association
	movq $2,%rax
	movq %rax, %rdi
	call _print
## 1-(2-3)			// association
##### 1-2-3			// association
	movq $-4,%rax
	movq %rax, %rdi
	call _print
## 1-2-3			// association
##### 1+2*3			// expr order
	movq $7,%rax
	movq %rax, %rdi
	call _print
## 1+2*3			// expr order
##### 3*2-3			// expr order
	movq $3,%rax
	movq %rax, %rdi
	call _print
## 3*2-3			// expr order
##### 1+10*10			// token
	movq $101,%rax
	movq %rax, %rdi
	call _print
## 1+10*10			// token
##### (2+1030/2)-2
	movq $515,%rax
	movq %rax, %rdi
	call _print
## (2+1030/2)-2
##### (255*07)+256		// logical expression
	movq $2041,%rax
	movq %rax, %rdi
	call _print
## (255*07)+256		// logical expression
##### 0+(1+(2+(3+(4+(5+(6+(7+8)))))))-(0+(1+(2+(3+(4+(5+(6+(7+8))))))))
	movq $0,%rax
	movq %rax, %rdi
	call _print
## 0+(1+(2+(3+(4+(5+(6+(7+8)))))))-(0+(1+(2+(3+(4+(5+(6+(7+8))))))))
##### 100/10
	movq $10,%rax
	movq %rax, %rdi
	call _print
## 100/10
##### a=1*3
	movq $3,%rax
	movq _v@GOTPCREL(%rip), %rbx
	movq %rax, 0(%rbx)
	movq %rax, %rdi
	call _print
## a=1*3
##### b=2*3
	movq $6,%rax
	movq _v@GOTPCREL(%rip), %rbx
	movq %rax, 8(%rbx)
	movq %rax, %rdi
	call _print
## b=2*3
##### a+b
	movq _v@GOTPCREL(%rip), %rbx
	movq 8(%rbx),%rax
	movq _v@GOTPCREL(%rip), %rcx
	movq 0(%rcx),%rbx
	addq %rax,%rbx
	movq %rbx, %rdi
	call _print
## a+b
##### a=(b=3*2)
	movq $6,%rbx
	movq _v@GOTPCREL(%rip), %rax
	movq %rbx, 8(%rax)
	movq _v@GOTPCREL(%rip), %rax
	movq %rbx, 0(%rax)
	movq %rbx, %rdi
	call _print
## a=(b=3*2)
##### a>1
	movq _v@GOTPCREL(%rip), %rax
	movq 0(%rax),%rbx
	movq $1,%rax
	cmpq %rax,%rbx
	setg %dl
	movzbq %dl,%rax
	movq %rax, %rdi
	call _print
## a>1
##### b
	movq _v@GOTPCREL(%rip), %rbx
	movq 8(%rbx),%rax
	movq %rax, %rdi
	call _print
## b
##### b>a
	movq _v@GOTPCREL(%rip), %rbx
	movq 8(%rbx),%rax
	movq _v@GOTPCREL(%rip), %rcx
	movq 0(%rcx),%rbx
	cmpq %rbx,%rax
	setg %dl
	movzbq %dl,%rbx
	movq %rbx, %rdi
	call _print
## b>a
    movq    %rax, %rdi
    call    _print
    movq    $0, %rax
    leave
    ret
LFE3:
.comm _v,96,5
    .section __TEXT,__eh_frame,coalesced,no_toc+strip_static_syms+live_support
EH_frame1:
    .set L$set$0,LECIE1-LSCIE1
    .long L$set$0
LSCIE1:
    .long   0x0
    .byte   0x1
    .ascii "zR\0"
    .byte   0x1
    .byte   0x78
    .byte   0x10
    .byte   0x1
    .byte   0x10
    .byte   0xc
    .byte   0x7
    .byte   0x8
    .byte   0x90
    .byte   0x1
    .align 3
LECIE1:
    .globl _print.eh
_print.eh:
LSFDE1:
    .set L$set$1,LEFDE1-LASFDE1
    .long L$set$1
LASFDE1:
    .long   LASFDE1-EH_frame1
    .quad   LFB2-.
    .set L$set$2,LFE2-LFB2
    .quad L$set$2
    .byte   0x0
    .byte   0x4
    .set L$set$3,LCFI0-LFB2
    .long L$set$3
    .byte   0xe
    .byte   0x10
    .byte   0x86
    .byte   0x2
    .byte   0x4
    .set L$set$4,LCFI1-LCFI0
    .long L$set$4
    .byte   0xd
    .byte   0x6
    .align 3
LEFDE1:
    .globl _main.eh
_main.eh:
LSFDE3:
    .set L$set$5,LEFDE3-LASFDE3
    .long L$set$5
LASFDE3:
    .long   LASFDE3-EH_frame1
    .quad   LFB3-.
    .set L$set$6,LFE3-LFB3
    .quad L$set$6
    .byte   0x0
    .byte   0x4
    .set L$set$7,LCFI3-LFB3
    .long L$set$7
    .byte   0xe
    .byte   0x10
    .byte   0x86
    .byte   0x2
    .byte   0x4
    .set L$set$8,LCFI4-LCFI3
    .long L$set$8
    .byte   0xd
    .byte   0x6
    .align 3
LEFDE3:
    .subsections_via_symbols
