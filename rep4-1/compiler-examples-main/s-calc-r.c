/*
    Very Simple Calculator
 */

#include <stdio.h>

static char *ptr,*last_ptr;
static int  value,lvalue;
static int  last_token;
static int  variable[48];

static int  expr();
static int  aexpr();
static int  mexpr();
static int  term();
static int  token();
static void  error(char *);


static int
token()
{
    int c,d;

    last_ptr = ptr;  /* for error position */
    c= *ptr;
    if(!c) {
	last_token = EOF;
	return last_token;
    }
    ptr++;
    if (c<=' ') {       /* comment */ 
	while(*ptr++);
	ptr--;
	last_token = EOF;
	last_ptr = ptr;
	return last_token;
    }
    if('0'<=c && c<='9') {     /* Decimal */
	d = c-'0';
	while((c= *ptr++)) {
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
    } else if ('a'<=c && c<='z') {    /* variable */
	value = c-'a';                /* return variable reference */
	last_token = 'v';
	return last_token;
    } else {
	last_token = c;
	return last_token;
	return c;
    }
}

static int
expr()
{
    int d,assign;

    d = aexpr();
    assign = lvalue;
    switch(last_token) {
    case '>': 
	d = (d > aexpr()); 
	return d;
    case '=': 
	if(assign>=0) {
	    d = expr(); 
	    variable[assign] = d;
	    return d;
	} else {
	    error("Bad assignment");
	    return 0;
	}
    case ')': 
	return d;
    case EOF: 
	return d;
    default:
	error("Bad expression");
	return d;
    }
}

static int
aexpr()
{
    int d;

    d = mexpr();
    switch(last_token) {
    case '-': 
	d -= aexpr(); 
	return d;
    case '+': 
	d += aexpr(); 
	return d;
    default:
	return d;
    }
}

static int
mexpr()
{
    int d;

    d = term();
    switch(last_token) {
    case '*': 
	d *= mexpr(); 
	return d;
    case '/': 
	d /= mexpr(); 
	return d;
    default:
	return d;
    }
}

static int 
term()
{
    int d;

    lvalue= -1;
    token();
    if(last_token==EOF) {
	error("Term expected");
    }
    switch(last_token) {
    case '0':
	d = value;
	token();
	return d;
    case 'v':
	d = lvalue = value;
	token();
	return variable[d];
    case '(':
	d = expr();
	if(last_token != ')') {
	    error("Unbalanced parenthsis");
	} 
	token(); 
	return d;
    default:
	token();
	error("Unknown term");
	return 0;
    }
}

static int lineno = 0;

void
error(char *msg) 
{
    fprintf(stderr,"%s on line %d\n",msg, lineno);
}

int
main() 
{
    int d;
    char buf[BUFSIZ];

    while (fgets(buf,BUFSIZ,stdin)) {
	ptr = buf;
	d = expr();
	printf("%s = 0x%08x = %d\n",buf,d,d);
	fflush(stdout);
	lineno++;
    }
    return 0;
}

/* end */
