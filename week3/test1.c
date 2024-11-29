extern int printf(const char *,...);

unsigned char a[] = {
      0x01,
      0x02,
      0x03,
      0x04,
      0x05,
      0x06,
      0x07,
      0x08,
      0x55,
      0x12,
};

long
test(unsigned char *a, long j)
{
   return j;
}

int
main() 
{
    long x = 0;
    x = test(a,x);
    printf("x = %lx\n", x);
    return 0;
}
