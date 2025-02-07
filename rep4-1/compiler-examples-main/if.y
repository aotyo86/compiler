%{
int yylex();
void yyerror(const char *);
%}

%%

statement :  'i' 'f' '(' expr ')' statement
          |  'i' 'f' '(' expr ')' statement 'e' 'l' 's' 'e' statement
          |  'a'| 'b' | 'c'
          ;

expr : '0'| '1'| '2';

%%

#include <stdio.h>

void yyerror(const char *s ) { fprintf(stderr,"error on %s\n", s); }

int yylex()
{
    for(;;) {
        int c = getchar();
        if (c<0) return 0;
        if (c>' ') return c;
    }
}
