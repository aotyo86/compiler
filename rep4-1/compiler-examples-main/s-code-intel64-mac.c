/*
    Very Simple Code Generator for Intel x86
    $Id$
 */

#include "s-compile.h"

int  label = 0;
char *comments = "#####";
static
char *intro[] = {
//  "      .cstring\n",
  "LC0:\n",
  "  .ascii \"%d\\n\\0\"\n"
  "    .text\n",
  ".globl _print\n",
  "_print:\n",
  "LFB2:\n",
  "    pushq   %rbp\n",
  "LCFI0:\n",
  "    movq    %rsp, %rbp\n",
  "LCFI1:\n",
  "    subq    $16, %rsp\n",
  "LCFI2:\n",
  "    movl    %edi, -4(%rbp)\n",
  "    movl    -4(%rbp), %esi\n",
  "    leaq    LC0(%rip), %rdi\n",
  "    movl    $0, %eax\n",
  "    call    _printf\n",
  "    leave\n",
  "    ret\n",
  "LFE2:\n",
  ".globl _main\n",
  "_main:\n",
  "LFB3:\n",
  "    pushq   %rbp\n",
  "LCFI3:\n",
  "    movq    %rsp, %rbp\n",
  "LCFI4:\n",
  "    subq    $16, %rsp\n",
  "LCFI5:\n",
  "\n",
  "\n",
  NULL
};

static
char *ending[] = {
  "    movq    %rax, %rdi\n",
  "    call    _print\n",
  "    movq    $0,%rax\n",
  "    leave\n",
  "    ret\n",
  "LFE3:\n",
//   ".comm _v,96,5\n",
  "    .section __TEXT\n",
//   "    .section __TEXT,__eh_frame,coalesced,no_toc+strip_static_syms+live_support\n",
//  "EH_frame1:\n",
//  "    .set L$set$0,LECIE1-LSCIE1\n",
//  "    .long L$set$0\n",
  "LSCIE1:\n",
  "    .long   0x0\n",
  "    .byte   0x1\n",
  "    .ascii \"zR\\0\"\n",
  "    .byte   0x1\n",
  "    .byte   0x78\n",
  "    .byte   0x10\n",
  "    .byte   0x1\n",
  "    .byte   0x10\n",
  "    .byte   0xc\n",
  "    .byte   0x7\n",
  "    .byte   0x8\n",
  "    .byte   0x90\n",
  "    .byte   0x1\n",
//  "    .align 3\n",
#if 0
  "LECIE1:\n",
  "    .globl _print.eh\n",
  "_print.eh:\n",
  "LSFDE1:\n",
  "    .set L$set$1,LEFDE1-LASFDE1\n",
  "    .long L$set$1\n",
  "LASFDE1:\n",
  "    .long   LASFDE1-EH_frame1\n",
  "    .quad   LFB2-.\n",
  "    .set L$set$2,LFE2-LFB2\n",
  "    .quad L$set$2\n",
  "    .byte   0x0\n",
  "    .byte   0x4\n",
  "    .set L$set$3,LCFI0-LFB2\n",
  "    .long L$set$3\n",
  "    .byte   0xe\n",
  "    .byte   0x10\n",
  "    .byte   0x86\n",
  "    .byte   0x2\n",
  "    .byte   0x4\n",
  "    .set L$set$4,LCFI1-LCFI0\n",
  "    .long L$set$4\n",
  "    .byte   0xd\n",
  "    .byte   0x6\n",
//  "    .align 3\n",
  "LEFDE1:\n",
  "    .globl _main.eh\n",
  "_main.eh:\n",
  "LSFDE3:\n",
  "    .set L$set$5,LEFDE3-LASFDE3\n",
  "    .long L$set$5\n",
  "LASFDE3:\n",
  "    .long   LASFDE3-EH_frame1\n",
  "    .quad   LFB3-.\n",
  "    .set L$set$6,LFE3-LFB3\n",
  "    .quad L$set$6\n",
  "    .byte   0x0\n",
  "    .byte   0x4\n",
  "    .set L$set$7,LCFI3-LFB3\n",
  "    .long L$set$7\n",
  "    .byte   0xe\n",
  "    .byte   0x10\n",
  "    .byte   0x86\n",
  "    .byte   0x2\n",
  "    .byte   0x4\n",
  "    .set L$set$8,LCFI4-LCFI3\n",
  "    .long L$set$8\n",
  "    .byte   0xd\n",
  "    .byte   0x6\n",
//  "    .align 3\n",
//  "LEFDE3:\n",
//  "    .subsections_via_symbols\n",
#endif
 NULL
};

void
emit_push()
{
    printf("\tpushq %%rax\n");
}

void
emit_compare()
{
    printf("\tcmpq %%rax,(%%rsp)\n");
    printf("\t%s %%al\n","setg");
    printf("\tmovzbq %%al,%%rax\n");
    printf("\taddq $8,%%rsp\n");
}

void
emit_store( int d )
{
    printf("\tmovq _v@GOTPCREL(%%rip), %%rcx\n");
    printf("\tmovq %%rax, %d(%%rcx)\n" ,d*8);
}

void
emit_load(int d)
{
    printf("\tmovq _v@GOTPCREL(%%rip), %%rcx\n");
    printf("\tmovq %d(%%rcx),%%rax\n" ,d*8);
}


static
char *opcode[] = {
    "",
    "subq",
    "addq",
    "imulq",
    "idivq",
    "",
    "",
    "",
    "",
    "subq",
    "idivq",
};

void
emit_calc(enum opcode op)
{
    if(op==O_DIV ) {
        printf("\tmovq %%rax,%%rbx\n");
        printf("\tpopq %%rax\n");
        printf("\tcltd\n");
        printf("\tidivq %%rbx\n");
    } else if( op==O_DIV_R ) {
        printf("\tpopq %%rbx\n");
        printf("\tcltd\n");
        printf("\tidivq %%rbx\n");
    } else if(op==O_SUB) {
        printf("\tpopq %%rbx\n");
        printf("\t%s %%rbx,%%rax\n",opcode[op]);
        printf("\tnegq %%rax\n");
    } else {
        printf("\tpopq %%rbx\n");
        printf("\t%s %%rbx,%%rax\n",opcode[op]);
    }
}

void
emit_value(d)
int d;
{
    printf("\tmovq $%d,%%rax\n",d);
}

void
emit_comment()
{
    if (before < ptr) {
    putchar('#'); putchar('#'); putchar(' ');
    while (before < ptr) {
        if(*before && *before!='\n') {
        putchar(*before);
        }
        before++;
    }
    putchar('\n');
    }
}

void
emit_print()
{
    printf("\tmovq %%rax, %%rdi\n");
    printf("\tcall _print\n");
}


void
emit_intro()
{
    char **iptr;
    for(iptr=intro;*iptr;iptr++) {
        printf("%s",*iptr);
    }
}

void
emit_ending()
{
    char **iptr;
    for(iptr=ending;*iptr;iptr++) {
        printf("%s",*iptr);
    }
}

/* end */
