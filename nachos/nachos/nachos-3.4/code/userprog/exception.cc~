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

#define MaxFileLength 32 // Do dai quy uoc cho file name
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
void IncrementPC() {
	int valuePC;
	valuePC = machine->ReadRegister(PCReg);
	machine->WriteRegister(PrevPCReg, valuePC);
	valuePC = machine->ReadRegister(NextPCReg);
	machine->WriteRegister(PCReg, valuePC);
	machine->WriteRegister(NextPCReg, valuePC + 4);
}

char* User2System(int virtAddr, int limit)
{
	int i;// index
	int oneChar;
	char* kernelBuf = NULL;
	kernelBuf = new char[limit + 1];//need for terminal string
	if (kernelBuf == NULL)
		return kernelBuf;
	memset(kernelBuf, 0, limit + 1);
	//printf("\n Filename u2s:");
	for (i = 0; i < limit; i++)
	{
		machine->ReadMem(virtAddr + i, 1, &oneChar);
		kernelBuf[i] = (char)oneChar;
		//printf("%c",kernelBuf[i]);
		if (oneChar == 0)
			break;
	}
	return kernelBuf;
}

int System2User(int virtAddr, int len, char* buffer)
{
	if (len < 0) return -1;
	if (len == 0)return len;
	int i = 0;
	int oneChar = 0;
	do {
		oneChar = (int)buffer[i];
		machine->WriteMem(virtAddr + i, 1, oneChar);
		i++;
	} while (i < len && oneChar != 0);
	return i;
}

void
ExceptionHandler(ExceptionType which)
{
	int type = machine->ReadRegister(2);
	int op1, op2, number, numbytes, i, temp, result;
	char* buffer;
	char ch;
	int MaxLength = 256;
	bool isNegative = false;
	float value;
	int virtAddr, charcount, id, OldPos, NewPos;
	switch (which) {
	case SyscallException:
		switch (type) {
		case SC_Halt:
			DEBUG('a', "Shutdown, initiated by user program.\n");
			interrupt->Halt();
		case SC_Sub:
			op1 = machine->ReadRegister(4);
			op2 = machine->ReadRegister(5);
			result = op1 - op2;
			machine->WriteRegister(2, result);
			IncrementPC();
			return;
		case SC_ReadInt:
			buffer = new char[MaxLength + 1];
			numbytes = gSynchConsole->Read(buffer, MaxLength);
			//printf("numbytes: %d\n", numbytes);
			number = 0;
			i = 0;
			if (buffer[0] == '-') {
				isNegative = true;
				i++;
			}
			for (; i < numbytes; i++) {
				if (buffer[i] < '0' || buffer[i] > '9') {
					DEBUG('a', "Invalid integer number.\n");
					number = 0;
					break;

				}
				else {
					number = number * 10 + (int)(buffer[i] - '0');
				}
			}
			if (isNegative) {
				number = -number;
			}
			//printf("result: %d\n", number);
			machine->WriteRegister(2, number);
			IncrementPC();
			delete[] buffer;
			return;
		case SC_PrintInt:
			number = machine->ReadRegister(4);
			//printf("number: %d\n", number);
			if (number == 0) {
				gSynchConsole->Write("0", 1);
				IncrementPC();
				return;
			}
			isNegative = number < 0 ? true : false;
			numbytes = 0;
			temp = number < 0 ? -number : number;
			while (temp > 0) {
				temp /= 10;
				numbytes++;
			}
			numbytes += int(isNegative);
			//printf("numbytes: %d\n", numbytes);
			buffer = new char[numbytes + 1];
			temp = number < 0 ? -number : number;
			i = numbytes;
			while (i > int(isNegative) && temp > 0) {
				buffer[i--] = char((temp % 10) + 48);
				temp /= 10;
			}
			if (isNegative) {
				buffer[0] = '-';
			}
			gSynchConsole->Write(buffer, numbytes + 1);
			IncrementPC();
			delete[] buffer;
			return;
			//case SC_PrintFloat:
				//number = machine->ReadRegister(4);
				//value = *(float*)&number;
				//printf("%f\n", value);
				//IncrementPC();
				//return;
		case SC_ReadChar:
			buffer = new char[2];
			gSynchConsole->Read(buffer, 2);
			ch = buffer[0];
			//printf("result: %c\n", ch);	
			machine->WriteRegister(2, ch);
			IncrementPC();
			delete buffer;
			return;
		case SC_PrintChar:
			ch = machine->ReadRegister(4);
			gSynchConsole->Write(&ch, 1);
			IncrementPC();
			return;
		case SC_PrintString:
			//void PrintString (char[] buffer
			virtAddr = machine->ReadRegister(4); //Lay dia chi cua buffer tu thanh ghi r4
			buffer = User2System(virtAddr, 255); //Copy chuoi tu vung nho User Space sang System Space
			int length;
			length = 0;
			//Dem do dai chuoi
			while (buffer[length] != '\0')
				length++;
			gSynchConsole->Write(buffer, length + 1); //Goi ham Write cua SynchConsole de in chuoi
			IncrementPC();
			delete[] buffer;
			return;
		case SC_Read:
			virtAddr = machine->ReadRegister(4);
			charcount = machine->ReadRegister(5);
			id = machine->ReadRegister(6);

			// Kiem tra id cua file truyen vao co nam ngoai bang mo ta file khong
			if (id < 0 || id > 14)
			{
				printf("\nKhong the read vi id nam ngoai bang mo ta file.");
				machine->WriteRegister(2, -1);
				IncrementPC();
				return;
			}
			// Kiem tra file co ton tai khong
			if (fileSystem->openf[id] == NULL)
			{
				printf("\nKhong the read vi file nay khong ton tai.");
				machine->WriteRegister(2, -1);
				IncrementPC();
				return;
			}
			if (fileSystem->openf[id]->type == 3) // Xet truong hop doc file stdout (type quy uoc la 3) thi tra ve -1
			{
				printf("\nKhong the read file stdout.");
				machine->WriteRegister(2, -1);
				IncrementPC();
				return;
			}
			OldPos = fileSystem->openf[id]->GetCurrentPos(); // Kiem tra thanh cong thi lay vi tri OldPos
			buffer = User2System(virtAddr, charcount); // Copy chuoi tu vung nho User Space sang System Space voi bo dem buffer dai charcount
			// Xet truong hop doc file stdin (type quy uoc la 2)
			if (fileSystem->openf[id]->type == 2)
			{
				// Su dung ham Read cua lop SynchConsole de tra ve so byte thuc su doc duoc
				int size = gSynchConsole->Read(buffer, charcount);
				System2User(virtAddr, size, buffer); // Copy chuoi tu vung nho System Space sang User Space voi bo dem buffer co do dai la so byte thuc su
				machine->WriteRegister(2, size); // Tra ve so byte thuc su doc duoc
				delete buffer;
				IncrementPC();
				return;
			}
			// Xet truong hop doc file binh thuong thi tra ve so byte thuc su
			if ((fileSystem->openf[id]->Read(buffer, charcount)) > 0)
			{
				// So byte thuc su = NewPos - OldPos
				NewPos = fileSystem->openf[id]->GetCurrentPos();
				// Copy chuoi tu vung nho System Space sang User Space voi bo dem buffer co do dai la so byte thuc su 
				System2User(virtAddr, NewPos - OldPos, buffer);
				machine->WriteRegister(2, NewPos - OldPos);
			}
			else
			{
				// Truong hop con lai la doc file co noi dung la NULL tra ve -2
				//printf("\nDoc file rong.");
				machine->WriteRegister(2, -2);
			}
			delete buffer;
			IncrementPC();
			return;
		case SC_Write:
			virtAddr = machine->ReadRegister(4);
			charcount = machine->ReadRegister(5);
			id = machine->ReadRegister(6);

			// Kiem tra id cua file truyen vao co nam ngoai bang mo ta file khong
			if (id < 0 || id > 14)
			{
				printf("\nKhong the write vi id nam ngoai bang mo ta file.");
				machine->WriteRegister(2, -1);
				IncrementPC();
				return;
			}
			// Kiem tra file co ton tai khong
			if (fileSystem->openf[id] == NULL)
			{
				printf("\nKhong the write vi file nay khong ton tai.");
				machine->WriteRegister(2, -1);
				IncrementPC();
				return;
			}
			// Xet truong hop ghi file only read (type quy uoc la 1) hoac file stdin (type quy uoc la 2) thi tra ve -1
			if (fileSystem->openf[id]->type == 1 || fileSystem->openf[id]->type == 2)
			{
				printf("\nKhong the write file stdin hoac file only read.");
				machine->WriteRegister(2, -1);
				IncrementPC();
				return;
			}
			OldPos = fileSystem->openf[id]->GetCurrentPos(); // Kiem tra thanh cong thi lay vi tri OldPos
			buffer = User2System(virtAddr, charcount);  // Copy chuoi tu vung nho User Space sang System Space voi bo dem buffer dai charcount
			// Xet truong hop ghi file read & write (type quy uoc la 0) thi tra ve so byte thuc su
			if (fileSystem->openf[id]->type == 0)
			{
				if ((fileSystem->openf[id]->Write(buffer, charcount)) > 0)
				{
					// So byte thuc su = NewPos - OldPos
					NewPos = fileSystem->openf[id]->GetCurrentPos();
					machine->WriteRegister(2, NewPos - OldPos);
					delete buffer;
					IncrementPC();
					return;
				}
			}
			if (fileSystem->openf[id]->type == 3) // Xet truong hop con lai ghi file stdout (type quy uoc la 3)
			{
				int i = 0;
				while (buffer[i] != 0 && buffer[i] != '\n') // Vong lap de write den khi gap ky tu '\n'
				{
					gSynchConsole->Write(buffer + i, 1); // Su dung ham Write cua lop SynchConsole 
					i++;
				}
				buffer[i] = '\n';
				gSynchConsole->Write(buffer + i, 1); // Write ky tu '\n'
				machine->WriteRegister(2, i - 1); // Tra ve so byte thuc su write duoc
				delete buffer;
				IncrementPC();
				return;
			}
		case SC_Close:
			//Input id cua file(OpenFileID)
			// Output: 0: thanh cong, -1 that bai
			int fid;
			fid = machine->ReadRegister(4); // Lay id cua file tu thanh ghi so 4
			if (fid >= 0 && fid <= 14) //Chi xu li khi fid nam trong [0, 14]
			{
				if (fileSystem->openf[fid]) //neu mo file thanh cong
				{
					delete fileSystem->openf[fid]; //Xoa vung nho luu tru file
					fileSystem->openf[fid] = NULL; //Gan vung nho NULL
					machine->WriteRegister(2, 0);
					printf("\nClose file thanh cong.");
					break;
				}
			}
			machine->WriteRegister(2, -1);
			return;
		case SC_Open:
			// Input: arg1: Dia chi cua chuoi name, arg2: type
			// Output: Tra ve OpenFileID neu thanh, -1 neu loi
			// Chuc nang: Tra ve ID cua file.

			//OpenFileID Open(char *name, int type)
			virtAddr = machine->ReadRegister(4); // Lay dia chi cua tham so name tu thanh ghi so 4
			type = machine->ReadRegister(5); // Lay tham so type tu thanh ghi so 5
			char* filename;
			filename = User2System(virtAddr, MaxFileLength); // Copy chuoi tu vung nho User Space sang System Space voi bo dem name dai MaxFileLength
			//Kiem tra xem OS con mo dc file khong
			int freeSlot;
			freeSlot = fileSystem->FindFreeSlot();
			if (freeSlot != -1) //Chi xu li khi con slot trong
			{
				if (type == 0 || type == 1) //chi xu li khi type = 0 hoac 1
				{
					if ((fileSystem->openf[freeSlot] = fileSystem->Open(filename, type)) != NULL) //Mo file thanh cong
					{
						machine->WriteRegister(2, freeSlot); //tra ve OpenFileID
					}
				}
				else if (type == 2) // xu li stdin voi type quy uoc la 2
				{
					machine->WriteRegister(2, 0); //tra ve OpenFileID
				}
				else // xu li stdout voi type quy uoc la 3
				{
					machine->WriteRegister(2, 1); //tra ve OpenFileID
				}
				IncrementPC();
				delete[] filename;
				return;
			}
			machine->WriteRegister(2, -1); //Khong mo duoc file return -1
			IncrementPC();
			delete[] filename;
			return;
		}
	}
}
