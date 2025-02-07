int printf(const char *,...);

void print(int i)
{
   printf("%d\n",i);
}

int a = 3;
int b = 4;

int main()
{

   print(a+b);
}
