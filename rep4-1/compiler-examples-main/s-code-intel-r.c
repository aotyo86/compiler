/*
    Very Simple Code Generator for Intel x86
    Register Version
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


static int  creg;     /* current register */
static char *crn;     /* current register name */
static int  lreg;     /* operand register */
static char *lrn;     /* operand register name */
static int  dreg;     /* temporary register */
static char *drn;     /* temporary register name */
static int  xreg;     /* operand register  */
static char *xrn;     /* operand register name */
static int  reg_sp;   /* REGister Stack-Pointer */


const int REG_EAX    = 0;
const int REG_EBX    = 1;
const int REG_ECX    = 2;
const int REG_EDX    = 3;
const int REG_ESI    = 4;
const int REG_EDI    = 5;
const int REG_EBP    = 6;
const int REG_ESP    = 7;


const int DATA_REG  = 0    ;
const int POINTER_REG  = 3    ;

/*
    creg   currrent register
    lreg   operand  register
    dreg   spare    register

    regs[]          register usage

    reg_name[creg]  register name
 */

#define MAX_MAX  20
static int regs[MAX_MAX];       /* 使われているレジスタを示すフラグ */
static int reg_stack[MAX_MAX];  /* 実際のレジスタの領域 */

const int	MAX_REGISTER=6;
const int	MAX_DATA_REG=4;    
const int	MAX_POINTER=3;    

static char    *reg_name[] = {"%eax","%ebx","%ecx","%edx","%esi",
		      "%edi","%ebp","%esp"};
// static char    *reg_name_l[] = {"%al","%bl","%cl","%dl"};

//static char *
//regster(i)
//int i;
//{
//    return reg_name[i];
//}

static int 
get_register() 
{    /* 使われていないレジスタを調べる */
    int i;
    for(i=0;i<MAX_REGISTER;i++) {
	if (! regs[i]) {    /* 使われていないなら */
	    regs[i]=1;      /* そのレジスタを使うことを宣言し */
	    return i;       /* その場所を表す番号を返す */
	}
    }
    return -1;    /* 空いている場所がないなら、それを表す -1 を返す */
}

static void 
free_register(i) {    /* いらなくなったレジスタを開放 */
    regs[i]=0;
}

/*
static int
register_full()
{
    int i;
    for(i=0;i<MAX_REGISTER;i++) {
	if (! regs[i]) { 
	    return 0;  
	}
    }
    return 1;    
}
 */

void 
emit_init() 
{
    int i;
    for(i=0;i<MAX_REGISTER;i++) regs[i]=0;
    creg = get_register();
    crn = reg_name[creg];
    dreg = REG_EDX; /* これは割算などで特別に使われる */
    regs[dreg]=1;
    drn = reg_name[dreg];
    reg_sp = 0;
}

static int 
pop_register() 
{     /* レジスタから値を取り出す */
    int i,j;

    j = creg;
    i = reg_stack[--reg_sp];

    if(i<0) {
	return i;
    } else {
	lreg = i;
	lrn = reg_name[lreg];
        free_register(i);
	return lreg;
    }
}

void 
emit_push()
{
    int new_reg;
    new_reg = get_register();
    if(new_reg<0) {                     /* もうレジスタがない */
	if (reg_sp==MAX_MAX-1) {
	    printf("Compilation error: too complex expression\n");
	} else {
	    reg_stack[reg_sp++] =  -1;
	    printf("\tpushl %s\n",crn);
	}
    } else {
	reg_stack[reg_sp++] = creg;     /* push するかわりにレジスタを使う */
	creg = new_reg;
	crn = reg_name[creg];
    }
}

char *
emit_pop()
{
    if (pop_register()==-1) {
	printf("\tpopl %s\n",drn);
	xrn = drn;
	xreg = dreg;
    } else {
	xrn = lrn;
	xreg = lreg;
    }
    return xrn;
}


void
emit_compare() 
{
    char *orn;
    orn = emit_pop();
    printf("\tcmpl %s,%s\n",crn,orn);
    printf("\t%s %%dl\n","setg");
    printf("\tmovzbl %%dl,%s\n",crn);
}

void
emit_store(assign)
int assign;
{
    printf("\tmovl %s,_variable+%d\n",crn,assign*4);
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
    char *orn;
    orn = emit_pop();
    if(op==O_DIV) {
	if (xreg==REG_EAX) {
	    printf("\tcltd\n");
	    printf("\tdivl %s\n",crn);
	    printf("\tmovl %s,%s\n",xrn,crn);
	} else if (creg==REG_EAX) {
	    printf("\txchg %s,%s\n",xrn,crn);
	    printf("\tcltd\n");
	    printf("\tdivl %s\n",xrn);
	} else {
	    if (regs[REG_EAX]) {
		printf("\tpushl %%eax\n");
	    }
	    printf("\tmovl %s,%%eax\n",xrn);
	    printf("\tcltd\n");
	    printf("\tdivl %s\n",crn);
	    printf("\tmovl %%eax,%s\n",crn);
	    if (regs[REG_EAX]) {
		printf("\tpopl %%eax\n");
	    }
	}
    } else {
	printf("\t%s %s,%s\n",opcode[op],orn,crn);
    }
}

void
emit_value(d) 
int d;
{
    printf("\tmovl $%d,%s\n",d,crn);
}

void
emit_load(d)
int d;
{
    printf("\tmovl _variable+%d,%s\n",d*4,crn);
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
    if (creg!=REG_EAX) {
        printf("\tmovl %s,%%eax\n",crn);
    }
    printf("\tcall _print\n");
}


void
emit_intro()
{
    char **iptr;
    for(iptr=intro;*iptr;iptr++) {
        printf("%s",*iptr);
    }
    emit_init() ;
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
