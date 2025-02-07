/*
    Very Simple Code Generator for Sun SPARC
	$Id$
 */

#include "s-compile.h"

int  label = 0;
char *comments = "#####";

static
char *reg_name[] = {
    "%g0","%g1","%g2","%g3","%g4","%g5","%g6","%g7",
    "%i0","%i1","%i2","%i3","%i4","%i5","%i6","%i7",
    "%l0","%l1","%l2","%l3","%l4","%l5","%l6","%l7",
    "%o0","%o1","%o2","%o3","%o4","%o5","%o6","%o7"
};
#define O0RN 24
#define MAX_REGISTER 32

static int  current_register;
static char reg_stack[MAX_REGISTER];
static int  reg_sp;
static char *crn;  /* current register name */
static char *lrn;  /* last register name */

static char regs[MAX_REGISTER];

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

/* simple version
static char *
pop_register() {
    int i;
    i = reg_stack[--reg_sp];
    regs[i]=0;
    lrn = reg_name[i];
    return crn;
}
 */

static char *
pop_register() {
    int i,j;

    j = current_register;
    i = reg_stack[--reg_sp];
    lrn = reg_name[i];

    free_register(j);

    current_register = i;
    crn = reg_name[i];
    return reg_name[j];
}

static
char *intro[] = {
    "gcc2_compiled.:\n",
    "___gnu_compiled_c:\n",
    ".text\n",
    "        .align 4\n",
    "        .global _main\n",
    "        .proc   04\n",
    "_main:\n",
    "        !#PROLOGUE# 0\n",
    "        save %sp,-104,%sp\n",
    "        !#PROLOGUE# 1\n",
    "        call ___main,0\n",
    "        nop\n",
    NULL
};

static
char *ending[] = {
    "        mov 0,%i0\n",
    "L1:\n",
    "        ret\n",
    "        restore\n",
    "        .align 8\n",
    "LC0:\n",
    "        .ascii \"0x%08x = %d\\12\\0\"\n",
    "        .align 4\n",
    "        .global _print\n",
    "        .proc   04\n",
    "_print:\n",
    "        !#PROLOGUE# 0\n",
    "        save %sp,-104,%sp\n",
    "        !#PROLOGUE# 1\n",
    "        st %i0,[%fp+68]\n",
    "        sethi %hi(LC0),%o1\n",
    "        or %o1,%lo(LC0),%o0\n",
    "        ld [%fp+68],%o1\n",
    "        ld [%fp+68],%o2\n",
    "        call _printf,0\n",
    "        nop\n",
    "L2:\n",
    "        ret\n",
    "        restore\n",
    "        .common _variable,256,\"bss\"\n",
    NULL
};

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
    printf("\tcmp %s,%s\n",lrn,orn);
    printf("\tbg,a LL%d\n",label);
    printf("\tmov   1,%s\n",crn);
    printf("\tmov   0,%s\n",crn);
    printf("LL%d:\n",label);
    label++;
}

void
emit_store(assign)
int assign;
{
    printf("\tsethi %%hi(_variable+%d),%%g2\n",assign*4);
    printf("\tst %s,[%%g2+%%lo(_variable+%d)]\n",crn,assign*4);
}

static
char *opcode[] = {
    "",
    "sub",
    "add",
    "mul",
    "div",
    "",
    "",
    "",
    "",
    "sub",
    "div",
};

void
emit_calc(enum opcode op)
{
    char *orn;
    orn = pop_register();
    if(op==O_MUL) {
	printf("\tmov %s,%%o1\n",lrn);
	printf("\tcall .umul,0\n");
	printf("\tmov %s,%%o0\n",orn);
	printf("\tmov %%o0,%s\n",crn);
    } else if(op==O_DIV) {
	printf("\tmov %s,%%o0\n",lrn);
	printf("\tcall .div,0\n");
	printf("\tmov %s,%%o1\n",orn);
	printf("\tmov %%o0,%s\n",crn);
    } else if(op==O_DIV_R) {
	printf("\tmov %s,%%o0\n",orn);
	printf("\tcall .div,0\n");
	printf("\tmov %s,%%o1\n",lrn);
	printf("\tmov %%o0,%s\n",crn);
    } else if(op==O_SUB) {
	printf("\t%s %s,%s,%s\n", opcode[op], lrn, orn, crn);
    } else {
	printf("\t%s %s,%s,%s\n", opcode[op], orn, lrn, crn);
    }
}

void
emit_value(d) 
int d;
{
    if(-4096 < d && d < 4096) {
	printf("\tmov %d,%s\n",d,crn);
    } else {
	printf("\tsethi %%hi(%d),%s\n",d,crn);
	printf("\tor %s,%%lo(%d),%s\n",crn,d,crn);
    }
}

void
emit_load(assign)
int assign;
{
    printf("\tsethi %%hi(_variable+%d),%%g2\n",assign*4);
    printf("\tld [%%g2+%%lo(_variable+%d)],%s\n",assign*4,crn);
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
    printf("\tcall _print,0\n");
    if(current_register!=O0RN) {
	printf("\tmov %s,%%o0\n",crn);
    } else {
	printf("\tnop\n");
    }
}

void
emit_intro()
{
    char **iptr;
    int i;
    for(iptr=intro;*iptr;iptr++) {
	printf("%s",*iptr);
    }
    for(i=0;i<MAX_REGISTER;i++) regs[i]=0;
    regs[0]=regs[1]=regs[2]=regs[24]=regs[25]=
       regs[8+6]=regs[16+6]=1;   /* already used */
    current_register = get_register();
    regs[current_register] = 1;
    crn = reg_name[current_register];
    reg_sp = 0;
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
