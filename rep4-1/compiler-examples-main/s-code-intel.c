/*
    Very Simple Code Generator for Intel x86
	$Id$
 */

#include "s-compile.h"

int  label = 0;
char *comments = "#####";

static
char *intro[] = {
    "#APP\n",    /* To process comments by GNU as */
    "\t.file   \"s-calc.c\"\n",
    ".text\n",
    "\t.align 2\n",
    ".globl main\nmain:\n",
    "\tpushl %ebp\n",
    "\tmovl %esp,%ebp\n",
    "\tsubl $1028,%esp\n",
/*
#ifndef NEXTSTEP
    "\tcall ___main\n",
#endif
 */
    NULL
};

static
char *ending[] = {
    "\txorl %eax,%eax\n",
    "\tleave\n",
    "\tret\n",
    "_print:\n",
    "\tpushl %ebp\n",
    "\tmovl %esp,%ebp\n",
    "\tpushl %eax\n",
    "\tpushl $_format\n",
    "\tcall printf\n",
    "\tleave\n",
    "\tret\n",
    "_format:\n",
    "\t.ascii \" = %d\\12\\0\"\n",
    "\t.align 2\n",
    ".comm _variable,192\n",
    NULL
};

void
emit_push()
{
    printf("\tpushl %%eax\n");
}

void
emit_compare() 
{
    printf("\tcmpl %%eax,(%%esp)\n");
    printf("\t%s %%al\n","setg");
    printf("\tmovzbl %%al,%%eax\n");
    printf("\taddl $4,%%esp\n");
}

void
emit_store(assign)
int assign;
{
    printf("\tmovl %%eax,_variable+%d\n",assign*4);
}

static
char *opcode[] = {
    "",
    "subl",
    "addl",
    "imull",
    "idiv",
    "",
    "",
    "",
    "",
    "subl",
    "idiv",
};

void
emit_calc(enum opcode op)
{
    if(op==O_DIV) {
	printf("\tmovl %%eax,%%ebx\n");
	printf("\tpopl %%eax\n");
	printf("\tcltd\n");
	printf("\tidiv %%ebx\n");
    } else if(op==O_SUB) {
	printf("\tpopl %%ebx\n");
	printf("\t%s %%ebx,%%eax\n",opcode[op]);
	printf("\tnegl %%eax\n");
    } else {
	printf("\tpopl %%ebx\n");
	printf("\t%s %%ebx,%%eax\n",opcode[op]);
    }
}

void
emit_value(d) 
int d;
{
    printf("\tmovl $%d,%%eax\n",d);
}

void
emit_load(d)
int d;
{
    printf("\tmovl _variable+%d,%%eax\n",d*4);
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
