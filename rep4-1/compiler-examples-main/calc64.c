/*
    Simple Calculator
	$Id$
 */

#include <stdio.h>

char *ptr,*last_ptr;
long  value,lvalue;
long  last_token;
long  variable[48];

static long  expr();
static long  aexpr();
static long  mexpr();
static long  term();


#define T_EQUAL	  (0x100|'=')
#define T_NEQUAL  (0x100|'!')
#define T_LSHIFT  (0x100|'<')
#define T_RSHIFT  (0x100|'>')

static void
line_skip()
{
    while(*ptr++);
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

static long
token()
{
    long c,d;

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
    } else if ('0'==c && 'x'== *ptr) {       /* Hex */
	ptr++;
	d = 0;
	while((c= *ptr++)) {
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
	while((c= *ptr++)) {
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
	while((c= *ptr++)) {
	    if('\''!=c && c<=0x7f) {
		d = d*256 + c;
	    } else if(c>=0x80 && *ptr) {
		d = d*256*256 + c*256 + *ptr++;
	    } else {
		break;
	    }
	}
	c && ptr--;
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
	return last_token;
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
    }
}

static long
expr()
{
    long d,assign;

    d = aexpr();
    assign = lvalue;
    while(last_token!=EOF) {
	switch(last_token) {
	case '<': 
	    d = (d < aexpr()); 
	    break;
	case '>': 
	    d = (d > aexpr()); 
	    break;
	case T_EQUAL: 
	    d = (d == aexpr()); 
	    break;
	case T_NEQUAL: 
	    d = (d != aexpr()); 
	    break;
	case T_LSHIFT: 
	    d <<= aexpr(); 
	    break;
	case T_RSHIFT: 
	    d >>= aexpr(); 
	    break;
	case '?': 
	    {
		long true;
		true = expr(); 
		if(last_token != ':') {
		    error("? expression not terminated with :");
		    return true;
		} 
		if(d) {
		    expr(); 
		    return true;
		} else {
		    return expr(); 
		}
	    }
	    break;
	case '=': 
	    if(assign>=0) {
		d = expr(); 
		variable[assign] = d;
		return d;
	    } else {
		error("Bad assignment");
		return 0;
	    }
	    break;
	case ':': 
	case ')': 
	    return d;
	default:
	    error("Bad expression");
	    token();
	}
    }
    last_token = EOF;
    return d;
}

static long
aexpr()
{
    long d;

    d = mexpr();
    while(last_token!=EOF) {
	switch(last_token) {
	case '^': 
	    d ^= mexpr(); 
	    break;
	case '|': 
	    d |= mexpr(); 
	    break;
	case '&': 
	    d &= mexpr(); 
	    break;
	case '-': 
	    d -= mexpr(); 
	    break;
	case '+': 
	    d += mexpr(); 
	    break;
	default:
	    return d;
	}
    }
    return d;
}

static long
mexpr()
{
    long d;

    d = term();
    while(last_token!=EOF) {
	switch(last_token) {
	case '*': 
	    d *= term(); 
	    break;
	case '/': 
	    d /= term(); 
	    break;
	case '%': 
	    d %= term(); 
	    break;
	case '^': 
	    d ^= term(); 
	    break;
	default:
	    return d;
	}
    }
    return d;
}

static long term()
{
    long d;

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
    case '-':
	return - term();
    case '!':
	return ! term();
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

int
main(int ac,char *av[]) 
{
    long d;
    char buf[BUFSIZ];

    while (fgets(buf,BUFSIZ,stdin)) {
	ptr = buf;
	d = expr();
	printf("%s = 0x%016lx = %ld\n",buf,d,d);
	fflush(stdout);
    }
    return 0;
}

/* end */
