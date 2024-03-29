#include "syscall.h"

int main() {
	float n;

	PrintString("Nhap so thuc: ");
	n = ReadFloat();
	PrintString("So vua nhap la: ");
	PrintFloat(n);
	PrintChar('\n');
	return 0;
}
