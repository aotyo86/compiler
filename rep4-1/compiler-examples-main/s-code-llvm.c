/*
    Very Simple Code Generator for Intel x86
    $Id$
 */

 #include "s-compile.h"

int  label = 0;
char *comments = "; ";
static
char *intro[] = {
    "; ModuleID = 'tmp.c'\n",
    "source_filename = \"tmp.c\"\n",
    "target datalayout = \"e-m:o-i64:64-i128:128-n32:64-S128\"\n",
    // "target triple = \"arm64-apple-macosx14.0.0\"\n",
    "\n",
    "@a = global i64 4, align 8\n",
    "@b = global i64 3, align 8\n",
    "@p = global i64 3, align 8\n",
    "@.str = private unnamed_addr constant [5 x i8] c\"%ld\\0A\\00\", align 1\n",
    "@variable = common global [100 x i32] zeroinitializer, align 8\n",
    "\n",
    "; Function Attrs: noinline nounwind optnone ssp uwtable(sync)\n",
    "define void @print(i64 noundef %0) #0 {\n",
    "  %2 = alloca i64, align 8\n",
    "  store i64 %0, i64*  %2, align 8\n",
    "  %3 = load i64, i64*  %2, align 8\n",
    "  %4 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([5 x i8], [5 x i8]* @.str, i64 0, i64 0), i64 noundef %3)\n",
    "  ret void\n",
    "}\n",
    "\n",
    "declare i32 @printf(i8*  noundef, ...) #1\n",
    "\n",
    "; Function Attrs: noinline nounwind optnone ssp uwtable(sync)\n",
    "define i32 @main() #0 {\n",
    "  %1 = alloca i32, align 4\n",
    "  store i32 0, i32*  %1, align 4\n",
    "  %2 = load i64, i64*  @a, align 8\n",
    "  %3 = add nsw i64 %2, 1\n",
    "  %4 = load i64, i64*  @b, align 8\n",
    "  %5 = sub nsw i64 %4, 123\n",
    "  %6 = sub nsw i64 %3, %5\n",
    "",
  NULL
};


static
char *ending[] = {
  // "  %7 = add i32 0, 0\n",
  // "  ret i32 %7\n",
  "}\n",
  "\n",
//  "attributes #0 = { noinline nounwind optnone ssp uwtable(sync) \"frame-pointer\"=\"non-leaf\" \"min-legal-vector-width\"=\"0\" \"no-trapping-math\"=\"true\" \"probe-stack\"=\"__chkstk_darwin\" \"stack-protector-buffer-size\"=\"8\" \"target-cpu\"=\"apple-m1\" \"target-features\"=\"+aes,+crc,+crypto,+dotprod,+fp-armv8,+fp16fml,+fullfp16,+lse,+neon,+ras,+rcpc,+rdm,+sha2,+sha3,+sm4,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8.5a,+v8a,+zcm,+zcz\" }\n",
//   "attributes #1 = { \"frame-pointer\"=\"non-leaf\" \"no-trapping-math\"=\"true\" \"probe-stack\"=\"__chkstk_darwin\" \"stack-protector-buffer-size\"=\"8\" \"target-cpu\"=\"apple-m1\" \"target-features\"=\"+aes,+crc,+crypto,+dotprod,+fp-armv8,+fp16fml,+fullfp16,+lse,+neon,+ras,+rcpc,+rdm,+sha2,+sha3,+sm4,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8.5a,+v8a,+zcm,+zcz\" }\n",
  "\n",
  "!llvm.module.flags = !{!0, !1, !2, !3, !4}\n",
  "!llvm.ident = !{!5}\n",
  "\n",
  "!0 = !{i32 2, !\"SDK Version\", [2 x i32] [i32 14, i32 5]}\n",
  "!1 = !{i32 1, !\"wchar_size\", i32 4}\n",
  "!2 = !{i32 8, !\"PIC Level\", i32 2}\n",
  "!3 = !{i32 7, !\"uwtable\", i32 1}\n",
  "!4 = !{i32 7, !\"frame-pointer\", i32 1}\n",
  "!5 = !{!\"Apple clang version 15.0.0 (clang-1500.3.9.4)\"}\n",
  "\n",
     NULL
};

static int  creg;     /* current register */
static int  lreg;     /* operand register */
static int  xreg;     /* operand register  */
static int  reg_sp;   /* REGister Stack-Pointer */

#define MAX_MAX  200000
static int regs[MAX_MAX];       /* 使われているレジスタを示すフラグ */
static int reg_stack[MAX_MAX];  /* 実際のレジスタの領域 */

const int	MAX_REGISTER=100000;

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
free_register(int i) {    /* いらなくなったレジスタを開放 */
    // regs[i]=0;
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
    reg_sp = 0;
}

static int
pop_register()
{     /* レジスタから値を取り出す */
    int i;

    // j = creg;
    i = reg_stack[--reg_sp];

    if(i<0) {
	return i;
    } else {
	lreg = i;
	// regs[i]=0;
	return lreg;
    }
}

void
emit_push()
{
    // int new_reg;
    // new_reg = get_register();
    // if(new_reg<0) {                     /* もうレジスタがない */
        // printf("Compilation error: too complex expression\n");
    // } else {
	reg_stack[reg_sp++] = creg;     /* push するかわりにレジスタを使う */
	// creg = new_reg;
    // }
}

int
emit_pop()
{
    pop_register();
    xreg = lreg;
    return xreg;
}


void
emit_compare()
{
    int oreg;
    oreg = emit_pop();
    creg = get_register();
    int creg0 = creg ;
    printf("\t%%%d = icmp sgt i32 %%%d, %%%d\n",creg,oreg,oreg);
    creg = get_register();
    printf("\t%%%d = zext i1 %%%d to i32\n",creg,creg0);
}

void
emit_store(int assign)
{
    int tmp = get_register();

    // printf("\t%%%d = getelementptr inbounds [48 x i32], i32*  @variable, i32 0, i32 %d\n",tmp,assign);
    printf("\t%%%d = getelementptr inbounds [100 x i32], [100 x i32]* @variable, i32 0, i32 %d\n",tmp,assign);
    printf("\tstore i32  %%%d, i32*  %%%d, align 4\n",creg,tmp);

    free_register(tmp);
}

void
emit_load(int d)
{
    int tmp = get_register();
    // printf("\t%%%d = getelementptr inbounds [48 x i32], i32*  @variable, i32 0, i32 %d\n", tmp, d);
    printf("\t%%%d = getelementptr inbounds [100 x i32], [100 x i32]* @variable, i32 0, i32 %d\n",tmp, d);
    creg = get_register();
    printf("\t%%%d = load i32, i32*  %%%d, align 4\n", creg, tmp);

    free_register(tmp);
}


static
char *opcode[] = {
    "",
    "sub nsw i32",
    "add nsw i32",
    "mul nsw i32",
    "sdiv i32",
    "",
    "",
    "",
    "",
    "sub ndw i32",
    "sdiv i32",
};

void
emit_calc(enum opcode op)
{
    int oreg;
    oreg = emit_pop();
    creg = get_register();
    if(op==O_DIV) {
        printf("\t%%%d = %s %%%d, %%%d\n",creg,opcode[op],oreg,oreg);
    } else if(op==O_SUB) {
        printf("\t%%%d = %s %%%d, %%%d\n",creg,opcode[op],oreg,oreg);
    } else {
        printf("\t%%%d = %s %%%d, %%%d\n",creg,opcode[op],oreg,oreg);
    }
}

void
emit_value(int d)
{
    creg = get_register();
    printf("\t%%%d = add i32 0, %d\n",creg, d);
    // printf("\t%%%d =  %d\n",creg,d);
}

void
emit_comment()
{
    if (before < ptr) {
    putchar(';');  putchar(' ');
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
    // printf("\t%%call = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([13 x i8]* @.str, i32 %%%d, i32 0))\n", creg);
    // printf("\tcall i32 (ptr , ...) @printf(i8* noundef @.str, i32 noundef %%%d)\n", creg);
    printf("\tcall i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([5 x i8], [5 x i8]* @.str, i32 0 , i32 0), i32 noundef %%%d)\n",creg);

    creg = get_register();
}


void
emit_intro()
{
    char **iptr;
    for(iptr=intro;*iptr;iptr++) {
        printf("%s",*iptr);
    }
    int i ;
    for(i=0;i<MAX_REGISTER;i++) regs[i]=0;
    for(i=0;i<7;i++) regs[i]=1;
}

void
emit_ending()
{
    creg = get_register();
    printf("\t%%%d = add i32 0, 0\n",creg);
    printf("\tret i32 %%%d\n",creg);
    char **iptr;
    for(iptr=ending;*iptr;iptr++) {
        printf("%s",*iptr);
    }
}

/* end */
