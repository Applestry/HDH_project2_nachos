#include "syscall.h"

int main() {
	char c;
	c = ReadChar();
	PrintChar(c);
	PrintChar('\n');
	return 0;
}
