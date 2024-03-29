#include "syscall.h"

int main() {
	int n;

	PrintString("Nhap so nguyen: ");
	n = ReadInt();
	PrintString("So vua nhap la: ");
	PrintInt(n);
	PrintChar('\n');
	return 0;
}
