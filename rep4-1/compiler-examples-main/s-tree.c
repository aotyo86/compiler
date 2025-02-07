/*
    Very Simple Calculator (Parser Part, Tree Printer)
	$Id$
 */

#include <stdio.h>
#include <stdlib.h>
#include "s-token.h"

// extern int  label;
// extern char *comments;

int  variable[48];

typedef struct node {
    struct node *left;
    struct node *right;
    int type;
    int value;
} node;

static node *expr();
static node *aexpr();
static node *mexpr();
static node *term();
static node *new_node();
static void print_node();

static node *
new_node(type,value,left,right) 
int type;
int value;
node *left;
node *right;
{
    node *d;
    d =  (node *)malloc(sizeof(node));
    d->type = type;
    d->value = value;
    d->left = left;
    d->right = right;
    return d;
}

static int tree_level = 0;

static void
print_node(d)
node *d;
{
    int i;
    for(i=0;i<tree_level;i++) { putchar(' '); }
    switch(d->type) {
    case '0':
	printf("value(%d",d->value);
	break;
    case 'v':
	printf("variable(%c",d->value+'a');
	break;
    default:
	printf("node(%c",d->type);
    }
    if(d->left) {
	tree_level++;
	printf(",(\n"); print_node(d->left); printf(")");
	tree_level--;
    }
    if(d->right) {
	tree_level++;
	printf(",(\n"); print_node(d->right); printf(")");
	tree_level--;
    }
    printf(")");
}

static node *
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

static node *
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

static node *
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

static node * 
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

    while (fgets(buf,BUFSIZ,stdin)) {
	printf("%s\n",buf);
	ptr = buf;
	d = expr();
	tree_level = 0;
	print_node(d);
	printf(".\n\n");
	fflush(stdout);
    }
    return 0;
}


/* end */
