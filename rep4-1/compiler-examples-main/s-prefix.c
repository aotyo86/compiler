/*
    Very Simple Compiler (Prefix form generator)
    Parser part
	$Id$
 */

#include "s-compile.h"

static void  expr();
static void  aexpr();
static void  mexpr();
static void  term();

static void
expr()
{
    int assign;

    emit_comment();
    aexpr();
    assign = lvalue;
    while(last_token!=EOF) {
	switch(last_token) {
	case '>': 
	    emit_compare();
	    aexpr();
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
	case ')': 
	    return;
	default:
	    error("Bad expression");
	    token();
	}
    }
    last_token = EOF;
}

static void
aexpr()
{

    emit_comment();
    mexpr();
    while(last_token!=EOF) {
	switch(last_token) {
	case '-': 
	    emit_calc(O_SUB);
	    mexpr(); 
	    break;
	case '+': 
	    emit_calc(O_ADD);
	    mexpr(); 
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
    emit_comment();
    term();
    while(last_token!=EOF) {
	switch(last_token) {
	case '*': 
	    emit_calc(O_MUL);
	    mexpr(); 
	    break;
	case '/': 
	    emit_calc(O_DIV); 
	    mexpr(); 
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


int
main() 
{
    char buf[BUFSIZ];

    emit_intro();
    while (fgets(buf,BUFSIZ,stdin)) {
	ptr = buf;
	before = buf;
	printf("%s %s",comments,buf);
	expr();
	emit_print();
	emit_comment();
    }
    emit_ending();
    return 0;
}


/* end */
