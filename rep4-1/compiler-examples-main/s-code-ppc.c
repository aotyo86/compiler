/*
    Very Simple Code Generator for Mac OS X (PPC)
	$Id$
 */

#include "s-compile.h"

int  label = 0;
char *comments = "#####";

static
char *intro[] = {
"	.section __TEXT,__text,regular,pure_instructions",
"	.section __TEXT,__picsymbolstub1,symbol_stubs,pure_instructions,32",
"	.machine ppc",
"	.text",
"	.align 2",
"	.globl _main",
"_main:",
"	mflr r0",
"        stmw r29,-12(r1)",
"        stw r0,8(r1)",
"        stwu r1,-80(r1)",
"	bcl 20,31,\"L00000000001$pb\"",
"\"L00000000001$pb\":",
"	mflr r31",
    NULL
};


static
char *ending[] = {
"        addi r1,r1,80",
"        lwz r0,8(r1)",
"        mtlr r0",
"        lmw r29,-12(r1)",
"        blr",
"",
"	.cstring",
"	.align 2",
"LC0:",
"	.ascii \"= %d\\12\\0\"",
"	.text",
"	.align 2",
"	.globl _print",
"_print:",
"	mflr r0",
"	stw r31,-4(r1)",
"	stw r0,8(r1)",
"	stwu r1,-80(r1)",
"	bcl 20,31,\"L00000000002$pb\"",
"\"L00000000002$pb\":",
"	mflr r31",
"	mr r4,r3",
"	addis r3,r31,ha16(LC0-\"L00000000002$pb\")",
"	la r3,lo16(LC0-\"L00000000002$pb\")(r3)",
"	bl L_printf$stub",
"	addi r1,r1,80",
"	lwz r0,8(r1)",
"	mtlr r0",
"	lwz r31,-4(r1)",
"	mtlr r0",
"	lwz r31,-4(r1)",
"	blr",
    "\t.align 2",
    ".comm _variable,192",
"	.section __TEXT,__picsymbolstub1,symbol_stubs,pure_instructions,32",
"	.align 5",
"L_printf$stub:",
"	.indirect_symbol _printf",
"	mflr r0",
"	bcl 20,31,\"L00000000001$spb\"",
"\"L00000000001$spb\":",
"	mflr r11",
"	addis r11,r11,ha16(L_printf$lazy_ptr-\"L00000000001$spb\")",
"	mtlr r0",
"	lwzu r12,lo16(L_printf$lazy_ptr-\"L00000000001$spb\")(r11)",
"	mtctr r12",
"	bctr",
"	.lazy_symbol_pointer",
"L_printf$lazy_ptr:",
"	.indirect_symbol _printf",
"	.long   dyld_stub_binding_helper",
"        .non_lazy_symbol_pointer",
"L_variable$non_lazy_ptr:",
"        .indirect_symbol _variable",
"        .long   0",
"        .subsections_via_symbols",

    NULL
};

void
emit_push()
{
    printf("\taddis r1,r1,-4\n");
    printf("\tstw r3,0(r1)\n");
}

void
emit_compare() 
{
    printf("\tlwz r4,0(r1)\n");
    printf("\taddis r1,r1,4\n");
    printf("\tcmpw cr7,r4,r3\n");
    printf("\tmfcr r3\n");
    printf("\trlwinm r3,r3,30,1\n");
}

void
emit_store(assign)
int assign;
{
    printf("\taddis r2,r31,ha16(L_variable$non_lazy_ptr-\"L00000000001$pb\")\n");
    printf("\tlwz r2,lo16(L_variable$non_lazy_ptr-\"L00000000001$pb\")(r2)\n");
    printf("\tstw r3,%d(r2)\n",assign*4);
}

static
char *opcode[] = {
    "",
    "subf",
    "add",
    "mullw",
    "divw",
    "",
    "",
    "",
    "",
    "subf",
    "divw",
};

void
emit_calc(enum opcode op)
{
    printf("\tlwz r4,0(r1)\n");
    printf("\taddis r1,r1,4\n");
    if(op==O_DIV) {
	printf("\tdivw r3,r4,r3\n");
//    } else if(op==O_SUB) {
//	printf("\tsubf r3,r3,r4\n");
    } else {
	printf("\t%s r3,r3,r4\n",opcode[op]);
    }
}

void
emit_value(d) 
int d;
{
    unsigned int d0 = (d&0xffff);
    unsigned int d1 = ((d>>16)&0xffff);
    printf("\tlis r3,0x%x\n",d1);
    printf("\tori r3,r3,0x%x\n",d0);

}

void
emit_load(d)
int d;
{
    printf("\taddis r2,r31,ha16(L_variable$non_lazy_ptr-\"L00000000001$pb\")\n");
    printf("\tlwz r2,lo16(L_variable$non_lazy_ptr-\"L00000000001$pb\")(r2)\n");
    printf("\tlwz r3,%d(r2)\n",d*4);  //  -1024<d <1024
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
        printf("%s\n",*iptr);
    }
}

void
emit_ending()
{
    char **iptr;
    for(iptr=ending;*iptr;iptr++) {
        printf("%s\n",*iptr);
    }
}

/* end */
