#include "syscall.h"
int main(){
	float f;
	ReadFloat(&f);
	PrintChar('h');
	Halt();
	PrintChar('h');
}