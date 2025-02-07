/*
    Very Simple Code Generator for Reverse Polish Notation
	$Id$
 */

#include "s-compile.h"

char *comments = "#####";

void
emit_push()
{
}

void
emit_compare() 
{
    printf("> ");
}

void
emit_store(assign)
int assign;
{
    printf("->%c ",assign+'a');
}


char *opcode[] = {
    "",
    "-",
    "+",
    "*",   
    "/",
    "",
    "",
    "",
    "",
    "-(r)",
    "/(r)",
};

void
emit_calc(enum opcode op)
{
    printf("%s ",opcode[op]);
}

void
emit_value(d) 
int d;
{
    printf("%d ",d);
}

void
emit_load(d)
int d;
{
    printf("%c\n",d+'a');
}

void
emit_comment()
{
}

void
emit_print()
{
    printf("print\n");
}


void
emit_intro()
{
}

void
emit_ending()
{
}


/* end */
