# use normal make for this Makefile
#
# Makefile for building user programs to run on top of Nachos
#
# Several things to be aware of:
#
#    Nachos assumes that the location of the program startup routine (the
# 	location the kernel jumps to when the program initially starts up)
#       is at location 0.  This means: start.o must be the first .o passed 
# 	to ld, in order for the routine "Start" to be loaded at location 0
#

# if you are cross-compiling, you need to point to the right executables
# and change the flags to ld and the build procedure for as

GCCDIR = ../../../gnu-decstation-ultrix/decstation-ultrix/2.95.3/

LDFLAGS = -T script -N
ASFLAGS = -mips2
CPPFLAGS = $(INCDIR)

# if you aren't cross-compiling:
# GCCDIR =
# LDFLAGS = -N -T 0
# ASFLAGS =
# CPPFLAGS = -P $(INCDIR)

PATH = $(GCCDIR):/lib:/usr/bin:/bin

CC = $(GCCDIR)gcc -B../../../gnu-decstation-ultrix/
AS = $(GCCDIR)as
LD = $(GCCDIR)ld

CPP = gcc -E
INCDIR =-I../userprog -I../threads
CFLAGS = -G 0 -c $(INCDIR)

all: halt shell matmult sort ReadFloat PrintFloat ReadChar PrintString ReadInt PrintChar CreateFile PrintInt ReadString ascii

start.o: start.s ../userprog/syscall.h
	$(CPP) $(CPPFLAGS) start.c > strt.s
	$(AS) $(ASFLAGS) -o start.o strt.s
	rm strt.s

halt.o: halt.c
	$(CC) $(CFLAGS) -c halt.c
halt: halt.o start.o
	$(LD) $(LDFLAGS) start.o halt.o -o halt.coff
	../bin/coff2noff halt.coff halt

shell.o: shell.c
	$(CC) $(CFLAGS) -c shell.c
shell: shell.o start.o
	$(LD) $(LDFLAGS) start.o shell.o -o shell.coff
	../bin/coff2noff shell.coff shell

sort.o: sort.c
	$(CC) $(CFLAGS) -c sort.c
sort: sort.o start.o
	$(LD) $(LDFLAGS) start.o sort.o -o sort.coff
	../bin/coff2noff sort.coff sort

matmult.o: matmult.c
	$(CC) $(CFLAGS) -c matmult.c
matmult: matmult.o start.o
	$(LD) $(LDFLAGS) start.o matmult.o -o matmult.coff
	../bin/coff2noff matmult.coff matmult

ReadFloat.o: ReadFloat.c
	$(CC) $(CFLAGS) -c ReadFloat.c
ReadFloat: ReadFloat.o start.o
	$(LD) $(LDFLAGS) start.o ReadFloat.o -o ReadFloat.coff
	../bin/coff2noff ReadFloat.coff ReadFloat

PrintFloat.o: PrintFloat.c
	$(CC) $(CFLAGS) -c PrintFloat.c
PrintFloat: PrintFloat.o start.o
	$(LD) $(LDFLAGS) start.o PrintFloat.o -o PrintFloat.coff
	../bin/coff2noff PrintFloat.coff PrintFloat

ReadChar.o: ReadChar.c
	$(CC) $(CFLAGS) -c ReadChar.c
ReadChar: ReadChar.o start.o
	$(LD) $(LDFLAGS) start.o ReadChar.o -o ReadChar.coff
	../bin/coff2noff ReadChar.coff ReadChar

PrintString.o: PrintString.c
	$(CC) $(CFLAGS) -c PrintString.c
PrintString: PrintString.o start.o
	$(LD) $(LDFLAGS) start.o PrintString.o -o PrintString.coff
	../bin/coff2noff PrintString.coff PrintString

CreateFile.o: CreateFile.c
	$(CC) $(CFLAGS) -c CreateFile.c
CreateFile: CreateFile.o start.o
	$(LD) $(LDFLAGS) start.o CreateFile.o -oCreateFile.coff
	../bin/coff2noff CreateFile.coff CreateFile

PrintInt.o: PrintInt.c
	$(CC) $(CFLAGS) -c PrintInt.c
PrintInt: PrintInt.o start.o
	$(LD) $(LDFLAGS) start.o PrintInt.o -o PrintInt.coff
	../bin/coff2noff PrintInt.coff PrintInt

ReadString.o: ReadString.c
	$(CC) $(CFLAGS) -c ReadString.c
ReadString: ReadString.o start.o
	$(LD) $(LDFLAGS) start.o ReadString.o -o ReadString.coff
	../bin/coff2noff ReadString.coff ReadString

ReadInt.o: ReadInt.c
	$(CC) $(CFLAGS) -c ReadInt.c
ReadInt: ReadInt.o start.o
	$(LD) $(LDFLAGS) start.o ReadInt.o -o ReadInt.coff
	../bin/coff2noff ReadInt.coff ReadInt

PrintChar.o: PrintChar.c
	$(CC) $(CFLAGS) -c PrintChar.c
PrintChar: PrintChar.o start.o
	$(LD) $(LDFLAGS) start.o PrintChar.o -o PrintChar.coff
	../bin/coff2noff PrintChar.coff PrintChar

ascii.o: ascii.c
	$(CC) $(CFLAGS) -c ascii.c
ascii: ascii.o start.o
	$(LD) $(LDFLAGS) start.o ascii.o -o ascii.coff
	../bin/coff2noff ascii.coff ascii
