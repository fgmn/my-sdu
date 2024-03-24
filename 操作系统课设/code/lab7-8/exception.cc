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

extern void StartProcess(int spaceId);

void AdvancePC()
{
    machine->WriteRegister(PCReg, machine->ReadRegister(PCReg) + 4);
    machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg) + 4);
}

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

void ExceptionHandler(ExceptionType which)
{
    // $2保存系统调用号
    int type = machine->ReadRegister(2);

    if (which == SyscallException)
    {
        switch (type)
        {
        case SC_Halt:
        {
            interrupt->Halt();
        }
        case SC_Join:
        {
            int spaceId = machine->ReadRegister(4);
            DEBUG('x', "thread %s join %d\n", currentThread->getName(), spaceId);
            currentThread->Join(spaceId);
            // 返回 Joinee 的退出码 waitProcessExitCode
            machine->WriteRegister(2, currentThread->pcb->waitProcessExitCode);
            AdvancePC();
            break;
        }
        case SC_Exec:
        {
            // read argument
            char filename[50];
            int addr = machine->ReadRegister(4);
            int i = 0;
            do
            {
                // read filname from mainMemory
                machine->ReadMem(addr + i, 1, (int *)&filename[i]);
            } while (filename[i++] != '\0');

            // 为halt.noff创建相应的进程以及相应的核心线程
            DEBUG('x', "thread %s Exec(%s)\n", currentThread->getName(), filename);

            OpenFile *executable = fileSystem->Open(filename);
            AddrSpace *space;

            if (executable == NULL)
            {
                printf("Unable to open file %s\n", filename);
                ASSERT(false);
            }
            space = new AddrSpace(executable);
            delete executable;

            Thread *thread = new Thread(filename);
            thread->Fork(StartProcess, space->getSpaceId());
            // 将该进程映射至新建的核心线程上执行
            thread->pcb->space = space;

            // return spaceID
            machine->WriteRegister(2, space->getSpaceId());
            AdvancePC();

            // currentThread->Yield();
            break;
        }
        case SC_Exit:
        {
            int ExitStatus = machine->ReadRegister(4);
            DEBUG('x', "thread %s Exit(%d)\n", currentThread->getName(), ExitStatus);
            currentThread->pcb->setExitStatus(ExitStatus);

            List *terminatedList = scheduler->getTerminatedList();
            if (ExitStatus == 99)
            {
                DEBUG('x', "thread %s parent delete terminatedList\n", currentThread->getName());
                Thread *thread;
                // 清空终止队列
                while ((thread = (Thread *)(terminatedList->Remove())) != NULL) {
                    DEBUG('x', "remove %s from tList\n", thread->getName());
                    delete thread;
                }
            }
            // 一个类的指针成员必须手动释放，在Thread的析构函数中手动释放Pcb对象
            // delete currentThread->pcb->space;
            currentThread->Finish();
            AdvancePC();
        }
        case SC_Yield:
        {
            DEBUG('x', "thread %s Yield\n", currentThread->getName());
            currentThread->Yield();
            AdvancePC();
            break;
        }
#ifdef FILESYS_STUB
        case SC_Create:
        {
            int base = machine->ReadRegister(4);
            int value;
            int count = 0;
            char *FileName = new char[128];
            do
            {
                machine->ReadMem(base + count, 1, &value);
                FileName[count] = *(char *)&value;
                count++;
            } while (*(char *)&value != '\0' && count < 128);

            int fileDescriptor = OpenForWrite(FileName);
            if (fileDescriptor == -1)
            {
                printf("create file %s failed!\n", FileName);
            }
            else
            {
                printf("create file %s succeed! the file id is %d\n", FileName, fileDescriptor);
            }
            Close(fileDescriptor);

            AdvancePC();
            break;
        }
        case SC_Open:
        {
            int base = machine->ReadRegister(4);
            int value;
            int count = 0;
            char *FileName = new char[128];
            do
            {
                machine->ReadMem(base + count, 1, &value);
                FileName[count] = *(char *)&value;
                count++;
            } while (*(char *)&value != '\0' && count < 128);

            int fileDescriptor = OpenForReadWrite(FileName, FALSE);
            if (fileDescriptor == -1)
            {
                printf("Open file %s failed!\n", FileName);
            }
            else
            {
                printf("Open file %s succeed! the file id is %d\n", FileName, fileDescriptor);
            }

            machine->WriteRegister(2, fileDescriptor);
            AdvancePC();
            break;
        }
        case SC_Write:
        {
            int base = machine->ReadRegister(4);
            int size = machine->ReadRegister(5);
            // bytes written to file   
            int fileId = machine->ReadRegister(6); // fd   

            int value;
            int count = 0;

            if (fileId == 0)
            {
                printf("thread %s stdin cannot write!\n", currentThread->getName());
                ASSERT(false);
            }

            OpenFile *openfile = new OpenFile(fileId);
            ASSERT(openfile != NULL);

            char *buffer = new char[128];
            do
            {
                machine->ReadMem(base + count, 1, &value);
                buffer[count] = *(char *)&value;
                count++;
            } while (*(char *)&value != '\0' && count < size);
            buffer[size] = '\0';
            

            int WritePosition;
            if (fileId == 1)
            {
                // WritePosition = 0;
                printf("%s", buffer);
                AdvancePC();
                break;
            }
            else
            {
                WritePosition = openfile->Length();
            }
            int WrittenBytes = openfile->WriteAt(buffer, size, WritePosition);

            if (WrittenBytes == 0)
            {
                printf("write file failed!\n");
            }
            else
            {
                printf("\"%s\" has wrote in file %d succeed!\n", buffer, fileId);
            }
            AdvancePC();
            break;
        }
        case SC_Read:
        {
            int base = machine->ReadRegister(4);
            int size = machine->ReadRegister(5);
            // bytes written to file   
            int fileId = machine->ReadRegister(6); // fd   

            OpenFile *openfile = new OpenFile(fileId);

            char buffer[size];
            int readnum = 0;

            if (fileId == 0) {  // to fix
                readnum = scanf("%s", buffer);
                machine->WriteRegister(2, readnum);
                AdvancePC();
                break;
            }
            readnum = openfile->Read(buffer, size);
            
            for (int i = 0; i < readnum; i++)
                if (!machine->WriteMem(base, 1, buffer[i]))
                    printf("This is something wrong.\n");
            buffer[readnum] = '\0';
            printf("read succeed! the content is \"%s\" , the length is %d\n", buffer, readnum);
            machine->WriteRegister(2, readnum);

            AdvancePC();
            break;
        }
        case SC_Close:
        {
            int fileId = machine->ReadRegister(4);

            Close(fileId);
            DEBUG('x', "thread %s fileId %d close success\n", currentThread->getName(), fileId);
            AdvancePC();
        }
#endif // FILESYS_STUB

        default:
        {
            printf("Unexpected syscall %d %d\n", which, type);
            break;
        }
        }
    }
    else
    {
        printf("Unexpected user mode exception %d %d\n", which, type);
        ASSERT(FALSE);
    }
}
