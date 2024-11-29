#include <stdio.h>

long a[10];

long
test(long i, long j)
{
   return i*j;
}

int
main() 
{
    long x;
    x = test(1,2);
    printf("size =%ld x = %lx\n",sizeof(x), x);
    return a[4];
}

