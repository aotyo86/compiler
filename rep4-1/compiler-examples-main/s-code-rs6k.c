/*
    Very Simple Code Generator for IBM PowerPC (with stack)
	$Id$
 */

#include "s-compile.h"

int  label = 0;
char *comments = "#####";


void
emit_push()
{
    printf("\tstu   r3,-4(SP)\n");
}

void
emit_compare() 
{
    printf("\tl     r4,0(SP)\n");
    printf("\tcmp   0,r4,r3\n");
    printf("\tai    SP,SP,4\n");
    printf("\tcal   r3,1(r0)\n");
    printf("\tbc    BO_IF,CR0_GT,__L%d\n",label);
    printf("\tcal   r3,0(r0)\n");
    printf("__L%d:\n",label);
    label++;
}

void
emit_store(assign)
int assign;
{
    printf("\tl       r4,T.28.variable(RTOC)\n");
    printf("\tst      r3,%d(r4)\n",assign*4);
}

static
char *opcode[] = {
    "",
    "sf",
    "a",
    "muls",
    "divs",
    "",
    "",
    "",
    "",
    "sf",
    "divs",
};

void
emit_calc(enum opcode op)
{
    printf("\tl       r4,0(SP)\n");
    printf("\tai      SP,SP,4\n");
    if(op==O_DIV || op==O_SUB_R) {
	printf("\t%s  r3,r4,r3\n",opcode[op]);
    } else {
	printf("\t%s  r3,r3,r4\n",opcode[op]);
    }
}

void
emit_value(d) 
int d;
{
    printf("\tcal   r3,%d(r0)\n",d&0xffff);
    if(d&0xffff0000) {
	printf("\tcau   r3,r3,%d\n",(d&0xffff0000)>>16);
    }
}

void
emit_load(d)
int d;
{
    printf("\tl       r4,T.28.variable(RTOC)\n");
    printf("\tl       r3,%d(r4)\n",d*4);
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
    printf("\tbl  .print\n");
}

void 
emit_intro()
{
    system("/bin/cat intro.rs6k");
}

void 
emit_ending()
{
    fflush(stdout);	
    system("/bin/cat ending.rs6k");
}


/* end */
