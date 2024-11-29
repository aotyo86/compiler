    extern int printf(const char *,...);
    #define TYPE int
    TYPE f(TYPE a, TYPE b) {
        return a + b;
    }
    int main() 
    {
        TYPE a = 1;
        TYPE b = 2;
        printf("%x = %x + %x \n",f(a,b),a,b);
        return 0;
    }	
