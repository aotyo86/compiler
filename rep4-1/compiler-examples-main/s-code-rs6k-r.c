/*
    Very Simple Code Generator for IBM PowerPC (with register)
	$Id$
 */

#include "s-compile.h"

int  label = 0;
char *comments = "#####";

static int  current_register;
static int  reg_sp;
static char *crn;  /* current register name */
static char *lrn;  /* last register name */

static char *reg_name[] = {
    "r0","r1","r2","r3","r4","r5","r6","r7",
    "r8","r9","r10","r11","r12","r13","r14","r15",
    "r16","r17","r18","r19","r20","r21","r22","r23",
    "r24","r25","r26","r27","r28","r29","r30","r31"
};
#define MAX_REGISTER 32
static char regs[MAX_REGISTER];
static char reg_stack[MAX_REGISTER];

static int
get_register() {
    int i;
    for(i=0;i<MAX_REGISTER;i++) {
	if (! regs[i]) {
	    regs[i]=1;
	    return i;
	}
    }
    error("Expression too complex (register overflow)");
    return -1;
}

static void
free_register(i) {
    regs[i]=0;
}

static char *
pop_register() {
    int i,j;

    j = current_register;
    i = reg_stack[--reg_sp];
    lrn = reg_name[i];

    regs[j]=0;

    current_register = i;
    crn = reg_name[i];
    return reg_name[j];
}


void
emit_push()
{
    reg_stack[reg_sp++] =  current_register;
    lrn = reg_name[current_register];
    current_register = get_register();
    crn = reg_name[current_register];
}

void
emit_compare() 
{
    char *orn;
    orn = pop_register();
    printf("\tcmp   0,%s,%s\n",lrn,orn);
    printf("\tcal   %s,1(r0)\n",crn);
    printf("\tbc    BO_IF,CR0_GT,__L%d\n",label);
    printf("\tcal   %s,0(r0)\n",crn);
    printf("__L%d:\n",label);
    label++;
}

void
emit_store(assign)
int assign;
{
    printf("\tl       r4,T.28.variable(RTOC)\n");
    printf("\tst      %s,%d(r4)\n",crn,assign*4);
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
    char *orn; 
    orn = pop_register();
    if(op==O_DIV || op==O_SUB_R) {
	printf("\t%s  %s,%s,%s\n",opcode[op],crn,lrn,orn);
    } else {
	printf("\t%s %s,%s,%s\n", opcode[op], crn, orn, lrn);
    }
}

void
emit_value(d) 
int d;
{
    printf("\tcal   %s,%d(r0)\n",crn,d&0xffff);
    if(d&0xffff0000) {
	printf("\tcau   %s,%s,%d\n",crn,crn,(d&0xffff0000)>>16);
    }

}

void
emit_load(assign)
int assign;
{
    printf("\tl       r4,T.28.variable(RTOC)\n");
    printf("\tl       %s,%d(r4)\n",crn,assign*4);
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
    if(current_register!=3) {
       printf("\tcal r3,0(%s)\n",crn);
    }
    printf("\tbl  .print\n");
}

void
emit_intro()
{
    int i;

    system("/bin/cat intro.rs6k");
    for(i=0;i<MAX_REGISTER;i++) regs[i]=0;
    regs[0]=regs[1]=regs[2]=regs[4]=1;   /* already used */
    current_register = get_register();
    crn = reg_name[current_register];
    reg_sp = 0;
}

void
emit_ending()
{
    fflush(stdout);	
    system("/bin/cat ending.rs6k");
}


/* end */
