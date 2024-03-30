
#include "syscall.h"

int main() {
	int x;
	int out;
	int len;
	char buffer[100];
	x = Open("hello.txt", 1);
	if (x != -1) {
		PrintString("Mo file thanh cong\n");
		len = Read(buffer, 100, x);

		if (len != -1 && len != -2) //Kiem tra co bi loi, hay co EOF hay khong
		{
			out = Open("out", 3); // Goi ham Open voi type = 3 de su dung out
			if (out != -1)
			{
				PrintString(" -> out: ");
				Write(buffer, len, out); // Goi ham Write de ghi noi dung doc duoc vao out
				Close(out); // Goi ham Close de dong out
			}
		}
		Close(x);
	}
	Halt();
	return 0;
}
