/*
    Very Simple Code Generator for Intel x86
    $Id$
 */

 #include "s-compile.h"

int  label = 0;
char *comments = "#####";
static
char *intro[] = {
  "      .cstring\n",
  "LC0:\n",
  "  .ascii \"%d\\n\\0\"\n"
  "    .text\n",
  ".globl _print\n",
  "_print:\n",
  "LFB2:\n",
  "    pushq   %rbp\n",
  "LCFI0:\n",
  "    movq    %rsp, %rbp\n",
  "LCFI1:\n",
  "    subq    $16, %rsp\n",
  "LCFI2:\n",
  "    movl    %edi, -4(%rbp)\n",
  "    movl    -4(%rbp), %esi\n",
  "    leaq    LC0(%rip), %rdi\n",
  "    movl    $0, %eax\n",
  "    call    _printf\n",
  "    leave\n",
  "    ret\n",
  "LFE2:\n",
  ".globl _main\n",
  "_main:\n",
  "LFB3:\n",
  "    pushq   %rbp\n",
  "LCFI3:\n",
  "    movq    %rsp, %rbp\n",
  "LCFI4:\n",
  "    subq    $16, %rsp\n",
  "LCFI5:\n",
  "\n",
  "\n",
  NULL
};

static
char *ending[] = {
  "    movq    %rax, %rdi\n",
  "    call    _print\n",
  "    movq    $0, %rax\n",
  "    leave\n",
  "    ret\n",
  "LFE3:\n",
  ".comm _v,96,5\n",
  "    .section __TEXT,__eh_frame,coalesced,no_toc+strip_static_syms+live_support\n",
  "EH_frame1:\n",
  "    .set L$set$0,LECIE1-LSCIE1\n",
  "    .long L$set$0\n",
  "LSCIE1:\n",
  "    .long   0x0\n",
  "    .byte   0x1\n",
  "    .ascii \"zR\\0\"\n",
  "    .byte   0x1\n",
  "    .byte   0x78\n",
  "    .byte   0x10\n",
  "    .byte   0x1\n",
  "    .byte   0x10\n",
  "    .byte   0xc\n",
  "    .byte   0x7\n",
  "    .byte   0x8\n",
  "    .byte   0x90\n",
  "    .byte   0x1\n",
  "    .align 3\n",
  "LECIE1:\n",
  "    .globl _print.eh\n",
  "_print.eh:\n",
  "LSFDE1:\n",
  "    .set L$set$1,LEFDE1-LASFDE1\n",
  "    .long L$set$1\n",
  "LASFDE1:\n",
  "    .long   LASFDE1-EH_frame1\n",
  "    .quad   LFB2-.\n",
  "    .set L$set$2,LFE2-LFB2\n",
  "    .quad L$set$2\n",
  "    .byte   0x0\n",
  "    .byte   0x4\n",
  "    .set L$set$3,LCFI0-LFB2\n",
  "    .long L$set$3\n",
  "    .byte   0xe\n",
  "    .byte   0x10\n",
  "    .byte   0x86\n",
  "    .byte   0x2\n",
  "    .byte   0x4\n",
  "    .set L$set$4,LCFI1-LCFI0\n",
  "    .long L$set$4\n",
  "    .byte   0xd\n",
  "    .byte   0x6\n",
  "    .align 3\n",
  "LEFDE1:\n",
  "    .globl _main.eh\n",
  "_main.eh:\n",
  "LSFDE3:\n",
  "    .set L$set$5,LEFDE3-LASFDE3\n",
  "    .long L$set$5\n",
  "LASFDE3:\n",
  "    .long   LASFDE3-EH_frame1\n",
  "    .quad   LFB3-.\n",
  "    .set L$set$6,LFE3-LFB3\n",
  "    .quad L$set$6\n",
  "    .byte   0x0\n",
  "    .byte   0x4\n",
  "    .set L$set$7,LCFI3-LFB3\n",
  "    .long L$set$7\n",
  "    .byte   0xe\n",
  "    .byte   0x10\n",
  "    .byte   0x86\n",
  "    .byte   0x2\n",
  "    .byte   0x4\n",
  "    .set L$set$8,LCFI4-LCFI3\n",
  "    .long L$set$8\n",
  "    .byte   0xd\n",
  "    .byte   0x6\n",
  "    .align 3\n",
  "LEFDE3:\n",
  "    .subsections_via_symbols\n",
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

const int REG_RAX    = 0;
const int REG_RBX    = 1;
const int REG_RCX    = 2;
const int REG_RDX    = 3;
const int REG_RSI    = 4;
const int REG_RDI    = 5;
const int REG_RBP    = 6;
const int REG_R07    = 7;
const int REG_R08    = 8;
const int REG_R09    = 9;
const int REG_R10    = 10;
const int REG_R11    = 11;
const int REG_R12    = 12;
const int REG_R13    = 13;
const int REG_R14    = 14;
const int REG_R15    = 15;

const int REG_RSP    = 16;

#define MAX_MAX  20
static int regs[MAX_MAX];       /* 使われているレジスタを示すフラグ */
static int reg_stack[MAX_MAX];  /* 実際のレジスタの領域 */

const int	MAX_REGISTER=10;

static char    *reg_name[] = {"%rax","%rbx","%rcx","%rdx", "%rsi", "%rdi", 
                      "%r8", "%r9", "%r10", "%r12", "%r13", "%r14", "%r15",
                      "%rip", "%rbp", "%rsp" };

char *
regster(i)
int i;
{
    return reg_name[i];
}

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

int
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

void 
emit_init() 
{
    int i;
    for(i=0;i<MAX_REGISTER;i++) regs[i]=0;
    creg = get_register();
    crn = reg_name[creg];
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
	regs[i]=0;
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
	    printf("\tpushq %s\n",crn);
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
	printf("\tpopq %s\n",drn);
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
    printf("\tcmpq %s,%s\n",crn,orn);
    printf("\t%s %%dl\n","setg");
    printf("\tmovzbq %%dl,%s\n",crn);
}

void
emit_store(assign)
int assign;
{
    int tmp = get_register();
    printf("\tmovq _v@GOTPCREL(%%rip), %s\n",reg_name[tmp] );
    printf("\tmovq %s, %d(%s)\n" ,crn, assign*8, reg_name[tmp]);
    free_register(tmp);
}

void
emit_load(int d)
{
    int tmp = get_register();
    printf("\tmovq _v@GOTPCREL(%%rip), %s\n",reg_name[tmp]);
    printf("\tmovq %d(%s),%s\n" ,d*8, reg_name[tmp],crn);
    free_register(tmp);
}


static
char *opcode[] = {
    "",
    "subq",
    "addq",
    "imulq",
    "idivq",
    "",
    "",
    "",
    "",
    "subq",
    "idivq",
};

void
emit_calc(enum opcode op)
{
    char *orn;
    orn = emit_pop();
    if(op==O_DIV) {
        if (orn[2]=='a') {
            printf("\tcltd\n");
            printf("\tidivq %s\n",crn);
            printf("\txchg %s,%%rax\n",crn);
        } else if ( crn[2]=='a' ) {
            printf("\txchg %s,%%rax\n",crn);
            printf("\tcltd\n");
            printf("\tidivq %s\n",orn);
            printf("\txchg %s,%%rax\n",crn);
        } else {
            printf("\txchg %s,%%rax\n",orn);
            printf("\tcltd\n");
            printf("\tidivq %s\n",crn);
            printf("\txchg %s,%%rax\n",crn);
        }
    } else if(op==O_DIV_R) {
        if (crn[2]=='a') {
            printf("\tcltd\n");
            printf("\tidivq %s\n",orn);
        } else if ( orn[2]=='a' ) {
            printf("\txchg %s,%%rax\n",crn);
            printf("\tcltd\n");
            printf("\tidivq %s\n",crn);
            printf("\txchg %s,%%rax\n",crn);
        } else {
            printf("\txchg %s,%%rax\n",crn);
            printf("\tcltd\n");
            printf("\tidivq %s\n",orn);
            printf("\txchg %s,%%rax\n",crn);
        }
    } else if(op==O_SUB) {
        printf("\t%s %s,%s\n",opcode[op],orn,crn);
        printf("\tnegq %s\n",crn);
    } else {
        printf("\t%s %s,%s\n",opcode[op],orn,crn);
    }
}

void
emit_value(d)
int d;
{
    printf("\tmovq $%d,%s\n",d,crn);
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
    printf("\tmovq %s, %%rdi\n",crn);
    printf("\tcall _print\n");
}


void
emit_intro()
{
    char **iptr;
    for(iptr=intro;*iptr;iptr++) {
        printf("%s",*iptr);
    }
    emit_init();
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
