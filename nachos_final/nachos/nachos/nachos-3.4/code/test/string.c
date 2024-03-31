#include "syscall.h"
int main(){
	char str[100];
	int length;
	length = 5;
	ReadString(str, 5);
	PrintString(str);
	Halt();
}