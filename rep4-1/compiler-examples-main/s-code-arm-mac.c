/*
    Very Simple Code Generator for Intel x86
    $Id$
 */

#include "s-compile.h"

int  label = 0;
char *comments = "#####";
static
char *intro[] = {
"            .section        __TEXT,__text,regular,pure_instructions\n"
"            .build_version macos, 12, 0     sdk_version 12, 3\n"
"        .globl  _print                          ; -- Begin function print\n"
"        .p2align        2\n"
"_print:                                 ; @print\n"
"        .cfi_startproc\n"
"; %bb.0:\n"
"        sub     sp, sp, #32\n"
"        stp     x29, x30, [sp, #16]             ; 16-byte Folded Spill\n"
"        add     x29, sp, #16\n"
"        .cfi_def_cfa w29, 16\n"
"        .cfi_offset w30, -8\n"
"        .cfi_offset w29, -16\n"
"        str     x0, [sp]\n"
"Lloh0:\n"
"        adrp    x0, l_.str@PAGE\n"
"Lloh1:\n"
"        add     x0, x0, l_.str@PAGEOFF\n"
"        bl      _printf\n"
"        ldp     x29, x30, [sp, #16]             ; 16-byte Folded Reload\n"
"        add     sp, sp, #32\n"
"        ret\n"
"        .loh AdrpAdd    Lloh0, Lloh1\n"
"        .cfi_endproc\n"
"\n"
"            .globl  _main                           ; -- Begin function main\n"
"            .p2align        2\n"
"    _main:                                  ; @main\n"
"            .cfi_startproc\n"
"    ; %bb.0:\n"
"            adrp    x8, _v@PAGE\n"
"\n\n",
  NULL
};

static
char *ending[] = {
"            bl _exit\n"
"            .loh AdrpLdr    Lloh0, Lloh1\n"
"            .cfi_endproc\n"
"                                            ; -- End function\n"
"        .section        __TEXT,__cstring,cstring_literals\n"
"l_.str:                                 ; @.str\n"
"        .asciz  \"%ld\\n\"\n"
"            .section        __DATA,__data\n"
"            .globl  _v                              ; @a\n"
"            .p2align        3\n"
"    _a:\n"
"            .quad   128                               ; 128\n"
"            .quad   128                               ; 128\n"
"    _v:\n"
"            .quad   64*100                               ; 64\n"
"            .quad   64*100                               ; 64\n"
"            .quad   64*100                               ; 64\n"
"            .quad   64*100                               ; 64\n"
"\n"
"    .subsections_via_symbols\n",
 NULL
};

void
emit_push()
{
    printf("\tstr x0,[sp, -16]!\n");
}

void
emit_compare()
{
    printf("\tldr x1,[sp]\n");
    printf("\tadd sp,sp,#16\n");
    printf("\tcmp x0,x1\n");
}

void
emit_store( int d )
{
    printf("    adrp    x9, _v@GOTPAGE\n");
    printf("    ldr     x9, [x9, _v@GOTPAGEOFF]\n");
    printf("str     x0, [x9, %d ]\n",d * 0x10);

}

void
emit_load(int d)
{
    printf("    adrp    x9, _v@GOTPAGE\n");
    printf("    ldr     x9, [x9, _v@GOTPAGEOFF]\n");
    printf("ldr     x0, [x9, %d ]\n",d * 0x10);
}


static
char *opcode[] = {
    "",
    "sub",
    "add",
    "mul",
    "sdiv",
    "",
    "",
    "",
    "",
    "sub",
    "sdiv",
};

void
emit_calc(enum opcode op)
{
    if(op==O_DIV ) {
        printf("\tldr x1,[sp]\n");
        printf("\tadd sp,sp,16\n");
        printf("\tsdiv x0 ,x1 , x0\n");
    } else if( op==O_DIV_R ) {
        printf("\tldr x1,[sp]\n");
        printf("\tadd sp,sp,#16\n");
        printf("\tsdiv x1 ,x0 , x0\n");
    } else if(op==O_SUB) {
        printf("\tldr x1,[sp]\n");
        printf("\tadd sp,sp,#16\n");
        printf("\tsub x1 ,x0 , x0\n");
    } else {
        printf("\tldr x1,[sp]\n");
        printf("\tadd sp,sp,#16\n");
        printf("\t%s x1 ,x0 , x0\n",opcode[op]);
    }
}

void
emit_value(int d)
{
    printf("\tmov x0, #%d\n",d);  // 16bit ?
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
    printf("\tbl _print\n");
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
