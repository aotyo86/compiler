int check = 0x12345678;
    int main()
    {
	char i, *ptr;
	
	ptr = (char *)&check; 
	i = ptr[1];
	return i;
    }
