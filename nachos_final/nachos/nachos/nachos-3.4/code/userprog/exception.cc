// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include <cstring>
#include <cmath>

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------
void IncrementPC(){
	int valuePC;
	valuePC = machine->ReadRegister(PCReg);
	machine->WriteRegister(PrevPCReg, valuePC);
	valuePC = machine->ReadRegister(NextPCReg);
	machine->WriteRegister(PCReg, valuePC);
	machine->WriteRegister(NextPCReg, valuePC + 4);
}

char* User2System(int virtAddr,int limit)
{
	int i;// index
	int oneChar;
	char* kernelBuf = NULL;
	kernelBuf = new char[limit + 1];//need for terminal string
	if (kernelBuf == NULL)
		return kernelBuf;
	memset(kernelBuf,0,limit+1);
	for (i = 0 ; i < limit ;i++)
	{
		machine->ReadMem(virtAddr+i,1,&oneChar);
		kernelBuf[i] = (char)oneChar;
		if (oneChar == 0)
			break;
	}
	return kernelBuf;
}

int System2User(int virtAddr,int len,char* buffer)
{
	if (len < 0) return -1;
	if (len == 0)return len;
	int i = 0;
	int oneChar = 0;
	do{
		oneChar= (int) buffer[i];
		machine->WriteMem(virtAddr+i,1,oneChar);
		i++;
	}while(i < len && oneChar != 0);
	return i;
}

void
ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);
    int op1, op2, number, numbytes, i, result;
    int MaxFileLength = 32;
    int virtAddr;
    int length;
    float* p_float;
    int* p_int;
    float point;
    char* buffer;
    char* filename;
    char ch;
    int MaxLength = 256;
    bool isNegative = false;
    switch(which){
    	case NoException:
    		return;
    	case PageFaultException:
    		DEBUG('a', "PageFault dang xay ra.\n");
    		interrupt->Halt();
    	case ReadOnlyException:
    		DEBUG('a', "ReadOnly dang xay ra.\n");
    		interrupt->Halt();
    	case BusErrorException:
    		DEBUG('a', "BusError dang xay ra.\n");
    		interrupt->Halt();
    	case AddressErrorException:
    		DEBUG('a', "AddressError dang xay ra.\n");
    		interrupt->Halt();
    	case OverflowException:
    		DEBUG('a', "Overflow dang xay ra.\n");
    		interrupt->Halt();
    	case IllegalInstrException:
    		DEBUG('a', "IllegalInstr dang xay ra.\n");
    		interrupt->Halt();
    	case NumExceptionTypes:
    		DEBUG('a', "NumberExceptionTypes dang xay ra.\n");
    		interrupt->Halt();
    	case SyscallException:
    		switch(type){
    			case SC_Halt:
    				printf("Halt()\n");
    				DEBUG('a', "Shutdown, initiated by user program.\n");
   					interrupt->Halt();
   				case SC_Sub:
					op1 = machine->ReadRegister (4);
					op2 = machine->ReadRegister (5);
					result = op1 - op2;
					machine->WriteRegister (2, result);
					IncrementPC();
					return;
				case SC_ReadInt:
					buffer = new char[MaxLength + 1];
					numbytes = gSynchConsole->Read(buffer, MaxLength);//doc cac ky tu duoc nhap tren console vao buffer
					//doc console bi loi
					if (numbytes == -1){
						machine->WriteRegister(2, 0);//ghi ket qua vao thanh ghi so 2
						IncrementPC();//tang program counter
						delete[] buffer;
						return;
					}
					number = 0;//ket qua tra ve
					i = 0;//chi so cua buffer
					//kiem tra so am
					if (buffer[0] == '-'){
						isNegative = true;
						i++;
					}
					//kiem tra lan luot cac ky tu co phai chu so khong
					//neu la chu so thi them vao ket qua tra ve
					//neu khong la chu so thi break vong lap va ket qua tra ve la 0
					for (;i < numbytes;i++){
						if (buffer[i] < '0' || buffer[i] > '9'){
							DEBUG('a', "Invalid integer number.\n");
							number = 0;
							break;

						}
						else{
							number = number * 10 + (int)(buffer[i] - '0');
						}
					}
					if (isNegative){
						number = -number;
					}
					machine->WriteRegister(2, number);//ghi ket qua vao thanh ghi so 2
					IncrementPC();//tang program counter
					delete[] buffer;
					return;
				case SC_PrintInt:
					number = machine->ReadRegister(4);//doc tham so cua ham tu thanh ghi so 4
					//truong hop la so 0
					if (number == 0){
						gSynchConsole->Write("0", 1);
						IncrementPC();
						return;
					}
					isNegative = number < 0 ? true : false;//kiem tra so am
					number = number < 0 ? -number : number;//bien tam luu gia tri tuyet doi de lay tung chu so
					numbytes = log(number) + 1 + int(isNegative);//tinh so chu so va dau
					buffer = new char[numbytes + 1];//chuoi cac chu so duoc ghi len console
					i = numbytes;
					while(i > int(isNegative) && number > 0){
						buffer[i--] = char((number % 10) + '0');
						number /= 10;
					}
					if (isNegative){
						buffer[0] = '-';
					}
					gSynchConsole->Write(buffer, numbytes + 1);//ghi chuoi so len console
					IncrementPC();//tang program counter
					delete[] buffer;
					return;
				case SC_ReadFloat:
					virtAddr = machine->ReadRegister(2);
		            buffer = new char[MaxLength + 1];
		            numbytes = gSynchConsole->Read(buffer, MaxLength);
		            printf("numbytes: %d\n", numbytes);
		            p_float = new float;
		            *p_float = 0;
		            i = 0;
					if (buffer[0] == '-'){
						isNegative = true;
						i++;
					}
					for (;i < numbytes;i++){
						if (buffer[i] == '.'){
							i++;
							break;
						}
						else if (buffer[i] < '0' || buffer[i] > '9'){
							DEBUG('a', "Invalid floating point number.\n");
							*p_float = 0;
							i = numbytes;
							break;

						}
						else{
							*p_float = *p_float * 10 + (int)(buffer[i] - '0');
						}
					}
					point = 1;
					for (;i < numbytes;i++){
						point = point / 10;
						if (buffer[i] < '0' || buffer[i] > '9'){
							DEBUG('a', "Invalid floating point number.\n");
							*p_float = 0;
							break;

						}
						else{
							*p_float = *p_float + (int)(buffer[i] - '0') * point;
						}
					}
					if (isNegative){
						*p_float = -*p_float;
					}
					System2User(virtAddr, 8, (char*)p_float);
					//printf("result: %d\n", result);
					//machine->WriteRegister(2, *p_int);
					printf("Da ghi thanh cong\n");
					IncrementPC();
					delete p_float;
					delete[] buffer;
					//printf("return\n");
					return;
				case SC_ReadChar:
					buffer = new char[2];
					gSynchConsole->Read(buffer, 2);//doc ky tu tren console
					ch = buffer[0];	
					machine->WriteRegister(2, ch);//ghi ky tu tra ve vao thanh ghi so 2	
					IncrementPC();//tang program counter
					delete[] buffer;
					return;
				case SC_PrintChar:
					ch = machine->ReadRegister(4);//doc tham so cua ham tu thanh ghi so 4
					gSynchConsole->Write(&ch, 1);//ghi len console
					IncrementPC();//tang program counter
					return;
				case SC_ReadString:
				    virtAddr = machine->ReadRegister(4);//doc dia chi tham so buffer
				    length = machine->ReadRegister(5);//doc length
				    buffer = new char[MaxLength];
				    numbytes = gSynchConsole->Read(buffer, MaxLength);//doc chuoi ky tu tren console
				    System2User(virtAddr, length, buffer);//chuyen du lieu tu kernelspace sang userspace
				    delete[] buffer;
				    IncrementPC();//tang program counter
				    return;
				case SC_PrintString:
					virtAddr = machine->ReadRegister(4);//doc dia chi tham so buffer
					buffer = User2System(virtAddr, length);//chuyen du lieu tu userspace sang kernelspace 
					length = strlen(buffer);//tinh do dai buffer	
					gSynchConsole->Write(buffer, length + 1);//ghi chuoi len console
					IncrementPC();//tang program counter
					delete[] buffer;
					return;
				case SC_CreateFile:
					virtAddr = machine->ReadRegister(4);
					printf("virtAddr: %d\n", virtAddr);
					//DEBUG (dbgFile,"\n Reading filename.");
					filename = User2System(virtAddr,MaxFileLength+1); // MaxFileLength là = 32
					if (filename == NULL)
					{
						printf("\n Not enough memory in system");
						//DEBUG(dbgFile,"\n Not enough memory in system");
						machine->WriteRegister(2,-1); // trả về lỗi cho chương trình người dùng
						delete filename;
						return;
					}
					//DEBUG(dbgFile,"\n Finish reading filename.");
					//DEBUG(dbgFile,"\n File name : '"<<filename<<"'");
					// Create file with size = 0
					// Dùng đối tượng fileSystem của lớp OpenFile để tạo file, việc tạo file này là sử dụng các
					// thủ tục tạo file của hệ điều hành Linux, chúng ta không quản ly trực tiếp các block trên
					// đĩa cứng cấp phát cho file, việc quản ly các block của file trên ổ đĩa là một đồ án khác
					if (!fileSystem->Create(filename,0))
					{
					printf("\n Error create file '%s'",filename);
					machine->WriteRegister(2,-1);
					delete filename;
					return;
					}
					machine->WriteRegister(2,0); // trả về cho chương trình người dùng thành công
					delete filename;
					IncrementPC();//ghi ket qua vao thanh ghi so 2
					return;
				case SC_Open:
					int type;
					virtAddr = machine->ReadRegister(4);
					printf("virtAddr: %d\n", virtAddr);
					type = machine->ReadRegister(5);
					printf("Type: %d\n", type);
					filename = User2System(virtAddr,MaxFileLength+1);
					machine->WriteRegister(2, 2); 
					IncrementPC();//ghi ket qua vao thanh ghi so 2
					return;

			}
    }
}
