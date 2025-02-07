/*
    Very Simple Compiler (Parser part, Tree Genertor)
	$Id$
 */

#include "s-compile.h"

#include <stdlib.h>    // for malloc

static int optimize = 1;

#define NEW(type) ((type *)malloc(sizeof(type)))

typedef struct node {
    struct node *left;
    struct node *right;
    int type;
    int value;
} node, *node_ptr;

static node *expr();
static node *aexpr();
static node *mexpr();
static node *term();
static node *new_node();

static node_ptr
new_node(int type,int value,node_ptr left,node_ptr right) 
{
    node *d;
    if (optimize && (left  &&  left->type =='0') &&
        (right && right->type =='0')) {
	switch(type) {
	case '>':
	    right->value = (left->value > right->value); 
	    free(left); return right;
	    break;
	case '+':
	    right->value = left->value + right->value; 
	    free(left); return right;
	    break;
	case '-':
	    right->value = left->value - right->value; 
	    free(left); return right;
	    break;
	case '*':
	    right->value = right->value * left->value;
	    free(left); return right;
	    break;
	case '/':
	    if(right->value==0) {
		error("zero divide in compile time");
	    } else {
		right->value = left->value / right->value;
	    }
	    free(left); return right;
	}
    }
    // d =  (node *)malloc(sizeof(node));
    d =  NEW(node);
    d->type = type;
    d->value = value;
    d->left = left;
    d->right = right;
    return d;
}

static void
free_node(d)
node_ptr d;
{
    if(d->left) {
	free_node(d->left);
    } 
    if(d->right) {
	free_node(d->right);
    } 
    free(d);
}

static void
code_generate(d)
node_ptr d;
{
    int assign;
    switch(d->type) {
    case '0':
	emit_value(d->value);
	return;
    case 'v':
	emit_load(d->value);
	return;
    case '=':
	if(!d->left || d->left->type != 'v') {
	    error("Bad assignment");
	    code_generate(d->right);
	    return;
	}
	assign = d->left->value;
	code_generate(d->right);
	emit_store(assign);
	return;
    case '>': 
	code_generate(d->left);
	emit_push();
	code_generate(d->right);
	emit_compare(); 
	break;
    default:   /* calculation */
	code_generate(d->right);
	emit_push();
	code_generate(d->left);
	switch(d->type) {
	case '+': emit_calc(O_ADD); break;
	case '-': emit_calc(O_SUB_R); break;
	case '/': emit_calc(O_DIV_R); break;
	case '*': emit_calc(O_MUL); break;
	default:
	    error("Internal Error, unknown opecode");
	}
	return;
    }
}

static node_ptr 
expr()
{
    node *d;

    d = aexpr();
    while(last_token!=EOF) {
	switch(last_token) {
	case '>': 
	    d = new_node('>',0,d,aexpr()); 
	    break;
	case '=': 
	    d = new_node('=',0,d,aexpr()); 
	    break;
	case ')': 
	    return d;
	default:
	    error("Bad expression");
	    return d;
	}
    }
    return d;
}

static node_ptr 
aexpr()
{
    node *d;

    d = mexpr();
    while(last_token!=EOF) {
	switch(last_token) {
	case '-': 
	    d = new_node('-',0,d,mexpr());
	    break;
	case '+': 
	    d = new_node('+',0,d,mexpr());
	    break;
	default:
	    return d;
	}
    }
    return d;
}

static node_ptr
mexpr()
{
    node *d;

    d = term();
    while(last_token!=EOF) {
	switch(last_token) {
	case '*': 
	    d = new_node('*',0,d,term());
	    break;
	case '/': 
	    d = new_node('/',0,d,term());
	    break;
	default:
	    return d;
	}
    }
    return d;
}

static node_ptr
term()
{
    node *d;

    lvalue= -1;
    token();
    if(last_token==EOF) {
	error("Term expected");
    }
    switch(last_token) {
    case '0':
	d = new_node('0',value,NULL,NULL);
	token();
	return d;
    case 'v':
	d = new_node('v',value,NULL,NULL);
	token();
	return d;
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
main() 
{
    node *d;
    char buf[BUFSIZ];

    emit_intro();
    while (fgets(buf,BUFSIZ,stdin)) {
	ptr = buf;
	before = buf;
	printf("%s %s",comments,buf);
	d = expr();
	code_generate(d);
	free_node(d);
	emit_print();
	emit_comment();
    }
    emit_ending();
    return 0;
}


/* end */
