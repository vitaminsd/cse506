int putchar(char c)
{
	volatile register char temp1, *temp2;
	temp1 =c;
	temp2 = (char *)0xb8000;
	*temp2 = temp1;
	return 0;
}
