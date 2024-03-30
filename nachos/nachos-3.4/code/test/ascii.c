#include "syscall.h"

int main()
{
	int i, id, cnt, temp;
	char fileName[] = "ascii.txt";
	char buffer[1000];

	if (CreateFile(fileName) == 0)
	{
		PrintString("Tao file thanh cong!\n");
		id = Open(fileName, 0);
	}
	PrintString("Bang ma ASCII la:\n");
	
	cnt = 0;
	for (i = 32; i < 127; i++)
	{
		temp = i;
		while (temp > 0)
		{
			buffer[cnt++] = (temp % 10) + '0';
			temp /= 10;
		}
		if (i >= 100)
		{
			temp = buffer[cnt - 1];
			buffer[cnt - 1] = buffer[cnt - 3];
			buffer[cnt - 3] = temp;
		}
		else
		{
			temp = buffer[cnt - 1];
			buffer[cnt - 1] = buffer[cnt - 2];
			buffer[cnt - 2] = temp;
		}
		buffer[cnt++] = ' ';
		buffer[cnt++] = i;
		buffer[cnt++] = '\n';
	}
	PrintString(buffer);
	Write(buffer, cnt, id);
	Close(id);
	return 0;
}
