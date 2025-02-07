/*
    Very Simple Code Generator for Motolora MC-6809
	$Id$
 */

#include "s-compile.h"

int  label = 0;
char *comments = "*****";

static char *intro[] = {
    "        ORG     $100\n",
    "variable        EQU     0\n",
    "main\n",
    "        LDS     #$FF00\n",
    "        LDY     #4096\n",
    "        PSHS    U\n",
    "        LEAU    ,S\n",
    "        LDD     #3\n",
    NULL
};

static
char *ending[] = {
    "	     SYNC\n",
    "        LEAS    2,S\n",
    "        PULS    U,PC\n",
    "\n",
    NULL
};

void
emit_push()
{
    printf("\tPSHS    D\n");
}

void
emit_compare() 
{
    printf("\tTFR     D,X\n");
    printf("\tCLRA\n");
    printf("\tCLRB\n");
    printf("\tCMPX    ,S++\n");
    printf("\tBGE     L%d\n",label);
    printf("\tINCB\n");
    printf("L%d\n",label);
    label++;
}

void
emit_store(assign)
int assign;
{
    printf("\tSTD     %d,Y\n",assign*2);
}

static
char *opcode[] = {
    "",
    "SUBD",
    "ADDD",
    "",   /* no multiply for 6809 */
    ""    /* no division for 6809 */
};

void
emit_calc(enum opcode op)
{
    if(op==O_SUB) {   
	printf("\t%s ,S++\n",opcode[op]);
	printf("\tNEGA\n");
	printf("\tNEGB\n");
	printf("\tSBCA #0\n");
    } else if(op==O_ADD) {
	printf("\t%s ,S++\n",opcode[op]);
    } else if(op==O_SUB_R) {
	printf("\tsubd ,S++\n");
    } else if(op==O_MUL) {
	printf("\tLDX ,S++\n",opcode[op]);
	printf("\tLBSR  _MULTI\n");
    } else if(op==O_DIV) {
	printf("\tLDX ,S++\n",opcode[op]);
	printf("\tEXG D,X\n");
	printf("\tLBSR  _DIVIDE\n");
    } else if(op==O_DIV_R) {
	printf("\tLDX ,S++\n",opcode[op]);
	printf("\tLBSR  _DIVIDE\n");
    }
}

void
emit_value(d) 
int d;
{
    printf("\tLDD   #%d\n",d);
}

void
emit_load(d)
int d;
{
    printf("\tLDD    %d,Y\n",d*2);
}

void
emit_comment()
{
    if (before < ptr) {
	putchar('*'); putchar('*'); putchar(' ');
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
    printf("\tLBSR print\n");
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
    fflush(stdout);
    system("/bin/cat ending.09");
}


/* end */
