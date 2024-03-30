
#include "syscall.h"
#define MAX_LENGTH 255

int main() {
	int fileID;
	int out;
	int len;
	char buffer[100];
	fileID = Open("hello.txt", 1);

	if (fileID != -1) {
		len = Read(buffer, MAX_LENGTH, fileID);
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
		Close(fileID);
	}
	Halt();
	return 0;
}
