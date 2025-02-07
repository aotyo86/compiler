/*
    Very Simple Code Generator for Intel x86
	$Id$
 */

#include "s-compile.h"

int  label = 0;
char *comments = "#####";

static
char *intro[] = {
".globl _variable\n",
"	.data\n",
"	.align 2\n",
"_variable:\n",
"	.space	512\n",
".globl _main\n",
"_main:\n",
"	pushl	%ebp\n",
"	movl	%esp, %ebp\n",
"	pushl	%ebx\n",
"	subl	$20, %esp\n",
"	call	___i686.get_pc_thunk.bx\n",
"\"L00000000002$pb\":\n",
"	leal	_variable-\"L00000000002$pb\"(%ebx), %esi\n",
 NULL
};

static
char *ending[] = {
"	movl	$0, %eax\n",
"	addl	$20, %esp\n",
"	popl	%ebx\n",
"	popl	%ebp\n",
"	ret\n",
"LC0:\n",
"	.ascii \"%d\\12\\0\"\n",
"	.text\n",
".globl _print\n",
"_print:\n",
"	pushl	%ebp\n",
"	movl	%esp, %ebp\n",
"	pushl	%ebx\n",
"	subl	$20, %esp\n",
"	call	___i686.get_pc_thunk.bx\n",
"\"L00000000001$pb\":\n",
"	movl	%eax, 4(%esp)\n",
"	leal	LC0-\"L00000000001$pb\"(%ebx), %eax\n",
"	movl	%eax, (%esp)\n",
"	call	L_printf$stub\n",
"	addl	$20, %esp\n",
"	popl	%ebx\n",
"	popl	%ebp\n",
"	ret\n",
"	.section __IMPORT,__jump_table,symbol_stubs,self_modifying_code+pure_instructions,5\n",
"L_printf$stub:\n",
"	.indirect_symbol _printf\n",
"	hlt ; hlt ; hlt ; hlt ; hlt\n",
"	.subsections_via_symbols\n",
"	.section __TEXT,__textcoal_nt,coalesced,pure_instructions\n",
".weak_definition	___i686.get_pc_thunk.bx\n",
".private_extern	___i686.get_pc_thunk.bx\n",
"___i686.get_pc_thunk.bx:\n",
"	movl	(%esp), %ebx\n",
"	ret\n",
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
    printf("\tmovl %%eax,%d(%%esi)\n",assign*4);
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
    printf("\tmovl %d(%%esi),%%eax\n",d*4);
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
