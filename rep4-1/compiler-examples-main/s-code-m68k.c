/*
    Simple Code Generator for Motolora MC-68000
	$Id$
 */

#include "s-compile.h"

char *comments = "####";

static
char *intro[] = {
    "#APP\n",    /* To process comments by GNU as */
    "\t.file   \"output.s\"\n",
    ".text\n",
    "\t.align 1\n",
    ".globl _main\n_main:\n",
    "\tlink a6,#0\n",
    NULL
};

static
char *ending[] = {
    "\tunlk a6\n",
    "\trts\n",
    "_print:\n",
    "\tlink a6,#0\n",
    "\tmovel d0,sp@-\n",
    "\tmovel d0,sp@-\n",
    "\tpea _format\n",
    "\tjbsr _printf\n",
    "\taddqw #8,sp\n",
    "\tunlk a6\n",
    "\trts\n",
    "_format:\n",
    "\t.ascii \" = %08x %d\\12\\0\"\n",
    "\t.align 2\n",
    ".comm _variable,192\n",
    NULL
};

static
char *opcode[] = {
    "",
    "subl",
    "addl",
    "mulsl",
    "divsl",
    "andl",
    "orl",
    "eorl",
    "subl",
    "divsl",
/*
    "asll",
    "asrl",
    "subl",
    "divsl",
    "slt",
    "sgt",
    "seq",
    "sne",
 */
};

void
emit_push()
{
    printf("\tmovel d0,sp@-\n");
}

void
emit_not() 
{
    printf("\ttstl d0\n");
    printf("\tsne d0\n");
    printf("\textbl d0\n");
}

void
emit_compare() 
{
    printf("\tmovel sp@,d1\n");
    printf("\taddql #4,sp\n");
    printf("\tcmpl d1,d0\n");
    printf("\tsetg d0\n");
    printf("\textbl d0\n");
    printf("\tnegl d0\n");
}

void
emit_store(assign)
int assign;
{
    printf("\tmovel d0,_variable+%d\n",assign*4);
}


void
emit_calc(enum opcode op)
{
    if(op==O_DIV) {
	printf("\tmovel d0,d1\n");
	printf("\tmovel sp@,d0\n");
	printf("\taddql #4,sp\n");
	printf("\tdivsl d1,d0\n");
    } else if(op==O_MOD) {
	printf("\tmovel d0,d2\n");
	printf("\tmovel sp@,d0\n");
	printf("\taddql #4,sp\n");
	printf("\tdivsll d2,d0:d1\n");
    } else if(op==O_ADD||op==O_MUL||op==O_AND||op==O_OR
	    ||op==O_XOR) {
	/* commutable operator */
	printf("\tmovel sp@,d1\n");
	printf("\taddql #4,sp\n");
	printf("\t%s d1,d0\n",opcode[op]);
    } else if(op==O_SUB_R||op==O_DIV_R) {
	printf("\tmovel sp@,d1\n");
	printf("\taddql #4,sp\n");
	printf("\t%s d0,d1\n",opcode[op]);
	printf("\tmovel d1,d0\n");
    } else {
	printf("\tmovel d0,d1\n");
	printf("\tmovel sp@,d0\n");
	printf("\taddql #4,sp\n");
	printf("\t%s d1,d0\n",opcode[op]);
    }
}

void
emit_neg()
{
	printf("\tnegl d0\n");
}

void
emit_value(d) 
int d;
{
    if(-127<=d && d<=127) {
	printf("\tmoveq #%d,d0\n",d);
    } else {
	printf("\tmovel #%d,d0\n",d);
    }
}

void
emit_load(d)
int d;
{
    printf("\tmovel _variable+%d,d0\n",d*4);
}

void
emit_cond(lbl) 
{
    printf("\ttstl d0\n");
    printf("\tjeq L%d\n",lbl);
}

void
emit_jump(lbl)
{
    printf("\tjra L%d\n",lbl);
}

void
emit_label(lbl)
{
    printf("L%d:\n",lbl);
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
    printf("\tjbsr _print\n");
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
