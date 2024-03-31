#include "syscall.h"

int main(){
	//int success;
	//success = CreateFile("test.txt");
	//if (success == 0){
	//	PrintChar('0');
	//}
	//else {
		//PrintChar('-1');
	//}
	OpenFileId o;
	o = Open("test.txt", 1);
	if (o != -1){
		PrintChar('0');
	}
	Halt();
}