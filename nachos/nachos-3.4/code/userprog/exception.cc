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
// Input: - User space address (int)
// - Limit of buffer (int)
// Output:- Buffer (char*)
// Purpose: Copy buffer from User memory space to System memory space
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

// Input: - User space address (int)
// - Limit of buffer (int)
// - Buffer (char[])
// Output:- Number of bytes copied (int)
// Purpose: Copy buffer from System memory space to User memory space
int System2User(int virtAddr, int len, char* buffer)
{
    if (len < 0) return -1;
    if (len == 0) return len;
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

    switch (which)
    {
    case NoException:
        return;

    case PageFaultException:
        DEBUG('a', "\n No valid translation found");
        printf("\n\n No valid translation found");
        interrupt->Halt();
        break;

    case ReadOnlyException:
        DEBUG('a', "\n Write attempted to page marked read-only");
        printf("\n\n Write attempted to page marked read-only");
        interrupt->Halt();
        break;

    case BusErrorException:
        DEBUG('a', "\n Translation resulted invalid physical address");
        printf("\n\n Translation resulted invalid physical address");
        interrupt->Halt();
        break;

    case AddressErrorException:
        DEBUG('a', "\n Unaligned reference or one that was beyond the end of the address space");
        printf("\n\n Unaligned reference or one that was beyond the end of the address space");
        interrupt->Halt();
        break;

    case OerflowException:
        DEBUG('a', "\nInteger overflow in add or sub.");
        printf("\n\n Integer overflow in add or sub.");
        interrupt->Halt();
        break;
    case IllegalInstrException:
        DEBUG('a', "\n Unimplemented or reserved instr.");
        printf("\n\n Unimplemented or reserved instr.");
        interrupt->Halt();
        break;

    case NumExceptionTypes:
        DEBUG('a', "\n Number exception types");
        printf("\n\n Number exception types");
        interrupt->Halt();
        break;

    case SyscallException:
        switch (type)
        {
        case SC_Halt:
            DEBUG('a', "\n Shutdown, initiated by user program.");
            printf("\n\n Shutdown, initiated by user program.");
            interrupt->Halt();
            break;

        case SC_ReadFloat:
            char* buffer;
            int max_buffer = 255;
            buffer = new char[max_buffer + 1];
            int numBytes = gSynchConsole->Read(buffer, max_buffer);
            float value = 0.0;
            int firstIndex = 0, lastIndex = 0;
            int decimalIndex = -1;
            bool isNegative = false;
            if (buffer[0] == '-')
            {
                isNegative  true;
                firstIndex = 1;
                lastIndex = 1;
            }
            for (int i = firstIndex; i < numBytes; i++)
            {
                if ((buffer[i] >= '0' && buffer[i] <= '9') || buffer[i] == '.')
                {
                    if (buffer[i] == '.')
                    {
                        decimalIndex = i;
                    }
                }
                else
                {
                    printf("\n\n The float number is not valid");
                    DEBUG('a', "\n The float number is not valid");
                    machine->WriteRegister(2, 0);
                    //IncreasePC();
                    delete buffer;
                    return;
                }
                lastIndex = i;
            }
            for (int i = firstIndex; i < decimalIndex; i++)
            {
                value = value * 10 + (buffer[i] - '0');
            }
            if (decimalIndex != -1)
            {
                float decimalPart = 0.0;
                int decimalDigits = 0;
                for (int i = decimalIndex + 1; i <= lastIndex; i++)
                {
                    decimalPart = decimalPart * 10 + (buffer[i] - '0');
                    decimalDigits++;
                }
                value += decimalPart / pow(10, decimalDigits);
            }
            if (isNegative)
            {
                value = value * -1;
            }
            machine->WriteRegister(2, value);
            //IncreasePC();
            delete buffer;
            return;

        case SC_PrintString:
            //void PrintString (char[] buffer
            int virtAddr;
            char* buffer;
            virtAdd = machine->ReadRegister(4); //Lay dia chi cua buffer tu thanh ghi r4
            buffer = User2System(virtAddr, 255); //Copy chuoi tu vung nho User Space sang System Space
            int length = 0;
            //Dem do dai chuoi
            while (buffer[length] != '\0')
                length++;
            gSynchConsole->Write(buffer, length + 1); //Goi ham Write cua SynchConsole de in chuoi
            delete buffer;
            break;
        case SC_CreateFile:
        {
            int virtAddr;
            char* filename;

            DEBUG('a', "\n SC_CreateFile call ...");
            DEBUG('a', "\n Reading virtual address of filename");
            //Lay tham so ten tap tin tu thanh ghi r4
            virtAddr = machine->ReadRegister(4);
            DEBUG('a', "\n Reading filename.");
            //MaxFileLength = 32
            filename = User2System(virtAddr, MaxFileLength + 1);
            if (filename == NULL)
            {
                printf("\n Not enough memory in system");
                DEBUG('a', "\n Not enough memory in system");
                machine->WriteRegister(2, -1); //tra ve loi cho chuong trinh nguoi dung
                delete filename;
                return;
            }
            DEBUG('a', "\n Finish reading filename.");
            //DEBUG('a',"\n File name: '" << filename << "'");
            //Create file with size = 0
            //Dung doi tuong fileSystem cua class OpenFile de tao file
            //viec tao file nay la su dung cac thu tuc tao file cua he dieu hanh Linux
            //chung ta khong quan ly truc tiep ca block tren dia cung cap phat cho file
            //viec quan ly cac block cua file tren o dia la mot do an khac
            if (!fileSystem->CreateFile(filename, 0))
            {
                printf("\n Error create file '%s'", filename);
                machine->WriteRegister(2, -1);
                delete filename;
                return;
            }
            machine->WriteRegister(2, 0); //tra ve chuong trinh nguoi dung thanh cong
            delete filename;
            break;
        }
        default:
            printf("\n Unexpected user mode exception (%d %d)", which, type);
            interrupt->Halt();
        }
    }
}
