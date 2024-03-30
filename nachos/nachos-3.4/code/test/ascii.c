#include "syscall.h"

int main()
{
	int i;

	PrintString("Bang ma ASCII la: ");
	
	for (i = 0; i <= 127; i++)
	{
		PrintChar((char)i);
		PrintChar(' ');
	}
	PrintChar('\n');
	PrintString("_______________");

	return 0;
}
