/*
    Very Simple Compiler
    Tokenizer Tester 
	$Id$
 */

#include "s-compile.h"

int
main() 
{
    char buf[BUFSIZ];

    while (fgets(buf,BUFSIZ,stdin)) {
	ptr = buf;
	before = buf;
	printf("#### %s",buf);
	do {
	    token();
	    switch(last_token) {
	    case '0':
		printf("%d\tvalue\n",value);
		break;
	    case 'v':
		printf("%c\tvariable\n",value+'a');
		break;
	    case EOF:
		break;
	    default:
		printf("%c\ttoken\n",last_token);
		break;
	    }
	} while(last_token!=EOF);
    }
    return 0;
}



/* end */
