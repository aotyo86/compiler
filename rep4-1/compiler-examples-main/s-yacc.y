/*
    Very Simple Compiler (Parser Part)
    Parser part
	$Id$
 */

%{
#include "s-compile.h"

void yyerror(char *);
int yylex();

typedef struct node {
    struct node *left;
    struct node *right;
    int type;
    int value;
} node,*nodeptr;

static int optimize = 0;

#define YYSTYPE nodeptr

static node *new_node();
static void free_node(node *d);
static void code_generate(node *d);
%}

%%

top : expr { 
	code_generate($1);
	free_node($1);
	emit_print();
	emit_comment();
    }
    ;

expr :  expr '>' aexpr  { $$ = new_node('>',0,$1,$3); }
    |   expr '=' aexpr  { $$ = new_node('=',0,$1,$3); }
    |   aexpr {$$ = $1; }
    ;

aexpr :  aexpr '+' mexpr { $$ = new_node('+',0,$1,$3); }
    |    aexpr '-' mexpr { $$ = new_node('-',0,$1,$3); }
    |    mexpr {$$ = $1; }
    ;

mexpr :  mexpr '*' term { $$ = new_node('*',0,$1,$3); }
    |    mexpr '/' term { $$ = new_node('/',0,$1,$3); }
    |    term {$$ = $1; }
    ;

term :  '0' { $$ = new_node('0',value,NULL,NULL); }
    |   'v' { $$ = new_node('v',value,NULL,NULL); }
    |   '(' expr ')'  {$$ = $2; }
    ;

%%

static void print_node();
int  variable[48];

static node *
new_node(type,value,left,right) 
int type;
int value;
node *left;
node *right;
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
    d =  (node *)malloc(sizeof(node));
    d->type = type;
    d->value = value;
    d->left = left;
    d->right = right;
    return d;
}

static void
free_node(node *d)
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
code_generate(node *d)
{
    int assign;
    if(!d) return; /* compiler internal error.. */
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

int
yylex() {
    return token();
}

int
main() 
{
    nodeptr d;
    char buf[BUFSIZ];

    emit_intro();
    while (fgets(buf,BUFSIZ,stdin)) {
	ptr = buf;
	before = buf;
	printf("%s %s",comments,buf);
	yyparse();
    }
    emit_ending();
    return 0;
}

void 
yyerror(s)
char *s;
{
    error(s?s:"error");
}


/* end */
