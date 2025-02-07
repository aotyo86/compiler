/*
    Simple Compiler
	$Id$
 */

#include <stdio.h>

char *ptr,*last_ptr,*before;
int  value,lvalue;
int  last_token;
int  label;

const int   T_EQUAL	= (0x100|'=');
const int   T_NEQUAL	= (0x100|'!');
const int   T_LSHIFT	= (0x100|'<');
const int   T_RSHIFT	= (0x100|'>');

enum opcode {
  O_SUB  = 1;
  O_ADD  = 2;
  O_MUL  = 3;
  O_DIV  = 4;
  O_AND  = 5;
  O_OR   = 6;
  O_XOR  = 7;
  O_ASL  = 8;
  O_ASR  = 9;
  O_MOD  = 10;
  O_LT   = 11;
  O_GT   = 12;
  O_EQ   = 13;
  O_NE   = 14;
};

static void error(char *);
static void  expr();
static void  aexpr();
static void  mexpr();
static void  term();
static int  token();

static void emit_push();
static void emit_compare() ;
static void emit_store(int);
static void emit_push();
static void emit_calc(enum opcode);
static void emit_value(int) ;
static void emit_load(int);
static void emit_comment();
static void emit_neg();
static void emit_not();
static void emit_label(int);
static void emit_jump(int);
static void emit_cond(int);


static void
line_skip()
{
    while(*ptr++);
    ptr--;
    last_ptr = ptr;  
    last_token = EOF;
}

static void
error(description)
char *description;
{
    fprintf(stderr,"%s on %s\n",description,last_ptr);
    line_skip();
}

static int
token()
{
    int c,d;

    last_ptr = ptr;  /* for error position */
    c= *ptr;
    while(c<=' ' && c) {
	c = *++ptr;
    }
    if(!c) {
	last_token = EOF;
	return last_token;
    }
    ptr++;
    if ('/'==c && ('/'== *ptr || '*'== *ptr)) {       /* comment */ 
	line_skip();
	return EOF;
    }
    if('1'<=c && c<='9') {     /* Decimal */
	d = c-'0';
	while(c= *ptr++) {
	    if('0'<=c && c<='9') {
		d = d*10 + (c - '0');
	    } else {
		break;
	    }
	}
	c && ptr--;
	value = d;
	last_token = '0';
	return last_token;
    } else if ('0'==c && 'x'== *ptr) {       /* Hex */
	ptr++;
	d = 0;
	while(c= *ptr++) {
	    if('0'<=c && c<='9') {
		d = d*16 + (c - '0');
	    } else if('a'<=c && c<='f') {
		d = d*16 + (c - 'a' + 10);
	    } else if('A'<=c && c<='F') {
		d = d*16 + (c - 'a' + 10);
	    } else {
		break;
	    }
	}
	c && ptr--;
	value = d;
	last_token = '0';
	return last_token;
    } else if ('0'==c) {       /* Octal */
	d = c-'0';
	while(c= *ptr++) {
	    if('0'<=c && c<='7') {
		d = d*8 + (c - '0');
	    } else {
		break;
	    }
	}
	c && ptr--;
	value = d;
	last_token = '0';
	return last_token;
    } else if ('\''==c) {      /* Ascii */
	d = 0;
	while(c= *ptr++) {
	    if('\''!=c && c<=0x7f) {
		d = d*256 + c;
	    } else if(c>=0x80 && *ptr) {
		d = d*256*256 + c*256 + *ptr++;
	    } else {
		break;
	    }
	}
	if(c!='\''|| c==0) { ptr--; }
	value = d;
	last_token = '0';
	return last_token;
    } else if (('a'<=c && c<='z')  ||   /* variable */
		('A'<=c && c<='Z')) {
	value = ((c>'Z')?c-'a'+'Z'-'A'+1:c-'A');
	last_token = 'v';
	return last_token;
    } else if ('='==c && '='== *ptr) {       /* equal */
	ptr++;
	last_token = T_EQUAL;
	return last_token;
    } else if ('!'==c && '='== *ptr) {       /* equal */
	ptr++;
	last_token = T_NEQUAL;
    } else if ('<'==c && '<'== *ptr) {       /* shift */
	ptr++;
	last_token = T_LSHIFT;
	return last_token;
    } else if ('>'==c && '>'== *ptr) {       /* shift */
	ptr++;
	last_token = T_RSHIFT;
	return last_token;
    } else {
	last_token = c;
	return last_token;
	return c;
    }
}

static void
expr()
{
    int d,assign;

    emit_comment();
    aexpr();
    assign = lvalue;
    while(last_token!=EOF) {
	switch(last_token) {
	case '<': 
            emit_push();
	    aexpr(); 
            emit_compare(O_LT);
	    break;
	case '>': 
            emit_push();
	    aexpr(); 
            emit_compare(O_GT);
	    break;
	case T_EQUAL: 
            emit_push();
	    aexpr(); 
            emit_compare(O_EQ);
	    break;
	case T_NEQUAL: 
            emit_push();
	    aexpr(); 
            emit_compare(O_NE);
	    break;
	case T_LSHIFT: 
            emit_push();
	    aexpr(); 
            emit_calc(O_ASL);
	    break;
	case T_RSHIFT: 
            emit_push();
	    aexpr(); 
            emit_calc(O_ASR);
	    break;
	case '?': 
	    {
		int l_false,l_end;
		l_false = label++;
		l_end = label++;
		emit_cond(l_false);
		expr(); 
		if(last_token != ':') {
		    error("? expression not terminated with :");
		    return;
		} 
		emit_jump(l_end);
		emit_label(l_false);
		expr(); 
		emit_label(l_end);
		return;
	    }
	    break;
	case '=': 
	    if(assign>=0) {
		expr(); 
		emit_store(assign);
		return;
	    } else {
		error("Bad assignment");
		return;
	    }
	    break;
	case ':': 
	case ')': 
	    return;
	default:
	    error("Bad expression");
	    token();
	}
    }
    last_token = EOF;
    return;
}

static void
aexpr()
{
    int d;

    emit_comment();
    mexpr();
    while(last_token!=EOF) {
	switch(last_token) {
	case '^': 
	    emit_push();
	    mexpr(); 
	    emit_calc(O_XOR);
	    break;
	case '|': 
	    emit_push();
	    mexpr(); 
	    emit_calc(O_OR);
	    break;
	case '&': 
	    emit_push();
	    mexpr(); 
	    emit_calc(O_AND);
	    break;
	case '-': 
	    emit_push();
	    mexpr(); 
	    emit_calc(O_SUB);
	    break;
	case '+': 
	    emit_push();
	    mexpr(); 
	    emit_calc(O_ADD);
	    break;
	default:
	    return;
	}
    }
    return;
}

static void
mexpr()
{
    int d;

    emit_comment();
    term();
    while(last_token!=EOF) {
	switch(last_token) {
	case '*': 
	    emit_push();
	    term();
	    emit_calc(O_MUL);
	    break;
	case '/': 
	    emit_push();
	    term();
	    emit_calc(O_DIV);
	    break;
	case '%': 
	    emit_push();
	    term();
	    emit_calc(O_MOD);
	    break;
	default:
	    return;
	}
    }
    return;
}

static void
term()
{
    int d;

    lvalue= -1;
    emit_comment();

    token();
    if(last_token==EOF) {
	error("Term expected");
    }
    switch(last_token) {
    case '0':
	d = value;
        emit_value(d);
	token();
	return;
    case 'v':
	d = lvalue = value;
        emit_load(d);
	token();
	return;
    case '-':
	term();
        emit_neg();
	return;
    case '!':
	term();
        emit_not();
	return;
    case '(':
	expr();
	if(last_token != ')') {
	    error("Unbalanced parenthsis");
	} 
	token(); 
	return;
    default:
	token();
	error("Unknown term");
	return;
    }
}

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
    "asll",
    "asrl",
    "idiv",
    "slt",
    "sgt",
    "seq",
    "sne",
};

static void
emit_push()
{
    printf("\tmovel d0,sp@-\n");
}

static void
emit_not() 
{
    printf("\ttstl d0\n");
    printf("\tsne d0\n");
    printf("\textbl d0\n");
}

static void
emit_compare(op) 
enum opcode op;
{
    printf("\tmovel sp@,d1\n");
    printf("\taddql #4,sp\n");
    printf("\tcmpl d1,d0\n");
    printf("\t%s d0\n",opcode[op]);
    printf("\textbl d0\n");
    printf("\tnegl d0\n");
}

static void
emit_store(assign)
int assign;
{
    printf("\tmovel d0,_variable+%d\n",assign*4);
}


static void
emit_calc(op)
int op;
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
    } else if(op==O_ADD||op==O_MUL||op==O_AND||op==O_OR||op==O_XOR) {
	/* commutable operator */
	printf("\tmovel sp@,d1\n");
	printf("\taddql #4,sp\n");
	printf("\t%s d1,d0\n",opcode[op]);
    } else {
	printf("\tmovel d0,d1\n");
	printf("\tmovel sp@,d0\n");
	printf("\taddql #4,sp\n");
	printf("\t%s d1,d0\n",opcode[op]);
    }
}

static void
emit_neg()
{
	printf("\tnegl d0\n");
}

static void
emit_value(d) 
int d;
{
    if(-127<=d && d<=127) {
	printf("\tmoveq #%d,d0\n",d);
    } else {
	printf("\tmovel #%d,d0\n",d);
    }
}

static void
emit_load(d)
int d;
{
    printf("\tmovel _variable+%d,d0\n",d*4);
}

static void
emit_cond(lbl) 
{
    printf("\ttstl d0\n");
    printf("\tjeq L%d\n",lbl);
}

static void
emit_jump(lbl)
{
    printf("\tjra L%d\n",lbl);
}

static void
emit_label(lbl)
{
    printf("L%d:\n",lbl);
}

static void
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

static void
emit_print()
{
    printf("\tjbsr _print\n");
}

int
main(int ac,char *av[]) 
{
    int d;
    char **iptr,buf[BUFSIZ];

    for(iptr=intro;*iptr;iptr++) {
	printf("%s",*iptr);
    }
    while (fgets(buf,BUFSIZ,stdin)) {
	ptr = buf;
	before = buf;
	printf("##### %s",buf);
	fflush(stdout);
	expr();
	emit_print();
	emit_comment();
    }
    for(iptr=ending;*iptr;iptr++) {
	printf("%s",*iptr);
    }
    return 0;
}


/* end */
