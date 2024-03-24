// addrspace.cc 
//	Routines to manage address spaces (executing user programs).
//
//	In order to run a user program, you must:
//
//	1. link with the -N -T 0 option 
//	2. run coff2noff to convert the object file to Nachos format
//		(Nachos object code format is essentially just a simpler
//		version of the UNIX executable object code format)
//	3. load the NOFF file into the Nachos file system
//		(if you haven't implemented the file system yet, you
//		don't need to do this last step)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"    // 引入内存
#include "addrspace.h"
#include "noff.h"
#include "machine.h"

//----------------------------------------------------------------------
// SwapHeader
// 	Do little endian to big endian conversion on the bytes in the 
//	object file header, in case the file was generated on a little
//	endian machine, and we're now running on a big endian machine.
//----------------------------------------------------------------------

static void 
SwapHeader (NoffHeader *noffH)
{
	noffH->noffMagic = WordToHost(noffH->noffMagic);
	noffH->code.size = WordToHost(noffH->code.size);
	noffH->code.virtualAddr = WordToHost(noffH->code.virtualAddr);
	noffH->code.inFileAddr = WordToHost(noffH->code.inFileAddr);
	noffH->initData.size = WordToHost(noffH->initData.size);
	noffH->initData.virtualAddr = WordToHost(noffH->initData.virtualAddr);
	noffH->initData.inFileAddr = WordToHost(noffH->initData.inFileAddr);
	noffH->uninitData.size = WordToHost(noffH->uninitData.size);
	noffH->uninitData.virtualAddr = WordToHost(noffH->uninitData.virtualAddr);
	noffH->uninitData.inFileAddr = WordToHost(noffH->uninitData.inFileAddr);
}

//----------------------------------------------------------------------
// AddrSpace::AddrSpace
// 	Create an address space to run a user program.
//	Load the program from a file "executable", and set everything
//	up so that we can start executing user instructions.
//
//	Assumes that the object code file is in NOFF format.
//
//	First, set up the translation from program memory to physical 
//	memory.  For now, this is really simple (1:1), since we are
//	only uniprogramming, and we have a single unsegmented page table
//
//	"executable" is the file containing the object code to load into memory
//----------------------------------------------------------------------

AddrSpace::AddrSpace(OpenFile *executable)
{
    char *tempMainMemory;
    NoffHeader noffH;
    unsigned int i, size;
    // 读取文件头进行识别
    executable->ReadAt((char *)&noffH, sizeof(noffH), 0);
    if ((noffH.noffMagic != NOFFMAGIC) && 
		(WordToHost(noffH.noffMagic) == NOFFMAGIC))
    	SwapHeader(&noffH);
    ASSERT(noffH.noffMagic == NOFFMAGIC);

// how big is address space?
    size = noffH.code.size + noffH.initData.size + noffH.uninitData.size 
			+ UserStackSize;	// we need to increase the size
						// to leave room for the stack
    numPages = divRoundUp(size, PageSize);  // 页表大小定义为扇区大小
    size = numPages * PageSize; // 程序实际的页表大小总和

    ASSERT(numPages <= NumPhysPages);		// check we're not trying
						// to run anything too big --
						// at least until we have
						// virtual memory
    // 检查确保拥有足够的虚拟内存空间
    DEBUG('a', "Initializing address space, num pages %d, size %d\n", 
					numPages, size);

    // 1. 检查是否有足够的空闲帧(不支持页面替换)
    if (numPages > bitmap->NumClear()) {    
        // 似乎没有初始化bitmap? fixed
        printf("Not enough pages.\n");
        ASSERT(FALSE);
        return;
    }
    // 2. 检查是否有可用pid（并发进程是否达到上限）
    bool hasAvailabePid = false;
    for (int i = 100; i < MAX_USERPROCESSES; i++)
    {
        if (!ThreadMap[i])
        {
            ThreadMap[i] = true;
            spaceId = i; //may be should reserved 0-99 for kernel Process,
                         //even though there is no any process at present
            // 将0-99保留给核心进程
            addrspaces[i] = this;
            hasAvailabePid = true; //ther is available Pid for new process
            break;
        }
    }                    //for
    if (!hasAvailabePid) //no available Pid for new process
    {
        printf("Too many processes in Nachos !\n");
        return;
    }
    ASSERT(hasAvailabePid);
    
    
// first, set up the translation 
// 初始化页表
    pageTable = new TranslationEntry[numPages];

    for (i = 0; i < numPages; i++) {
        pageTable[i].virtualPage = i;	// for now, virtual page # = phys page #
        // 分配空闲物理内存帧
        pageTable[i].physicalPage = bitmap->Find();
        pageTable[i].valid = TRUE;
        pageTable[i].use = FALSE;
        pageTable[i].dirty = FALSE;
        pageTable[i].readOnly = FALSE;  // if the code segment was entirely on 
                        // a separate page, we could set its 
                        // pages to be read-only
    }
    
// zero out the entire address space, to zero the unitialized data segment 
// and the stack segment 将整个地址空间归零
    // bzero(machine->mainMemory, size);

    tempMainMemory = new char[MemorySize];

// then, copy in the code and data segments into memory
    if (noffH.code.size > 0) {  // 将数据段和代码段拷贝进内存
        DEBUG('a', "Initializing code segment, at 0x%x, size %d\n", 
			noffH.code.virtualAddr, noffH.code.size);
        executable->ReadAt(&(tempMainMemory[noffH.code.virtualAddr]),
			noffH.code.size, noffH.code.inFileAddr);
    }
    if (noffH.initData.size > 0) {
        DEBUG('a', "Initializing data segment, at 0x%x, size %d\n", 
			noffH.initData.virtualAddr, noffH.initData.size);
        executable->ReadAt(&(tempMainMemory[noffH.initData.virtualAddr]),
			noffH.initData.size, noffH.initData.inFileAddr);
    }

    for (int i = 0; i < numPages; i++)
        memcpy(&(machine->mainMemory[pageTable[i].physicalPage * PageSize]), &(tempMainMemory[i * PageSize]), PageSize);
        // tempMainMemory仅仅使用了前面一块空间？
        // 要求编译器使用这块空间进行指令生成
    delete[] tempMainMemory;
}

//----------------------------------------------------------------------
// AddrSpace::~AddrSpace
// 	Dealloate an address space.  Nothing for now!
//----------------------------------------------------------------------

AddrSpace::~AddrSpace()
{
    ThreadMap[spaceId] = 0;
    addrspaces[spaceId] = NULL;

    for (int i = 0; i < numPages; i++)
        bitmap->Clear(pageTable[i].physicalPage);
        // 进程结束，释放占用的内存空间
   delete [] pageTable;
}

//----------------------------------------------------------------------
// AddrSpace::InitRegisters
// 	Set the initial values for the user-level register set.
//
// 	We write these directly into the "machine" registers, so
//	that we can immediately jump to user code.  Note that these
//	will be saved/restored into the currentThread->userRegisters
//	when this thread is context switched out.
//----------------------------------------------------------------------

void
AddrSpace::InitRegisters()
{
    int i;

    for (i = 0; i < NumTotalRegs; i++)
	machine->WriteRegister(i, 0);

    // Initial program counter -- must be location of "Start"
    machine->WriteRegister(PCReg, 0);	

    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    // 告诉MIPS下一条指令的位置，因为可能存在分支延迟
    machine->WriteRegister(NextPCReg, 4);

   // Set the stack register to the end of the address space, where we
   // allocated the stack; but subtract off a bit, to make sure we don't
   // accidentally reference off the end!
   // 将堆栈寄存器设置到地址空间的末尾
   // 安全起见，最后留余空间
    machine->WriteRegister(StackReg, numPages * PageSize - 16);
    DEBUG('a', "Initializing stack register to %d\n", numPages * PageSize - 16);
}

//----------------------------------------------------------------------
// AddrSpace::SaveState
// 	On a context switch, save any machine state, specific
//	to this address space, that needs saving.
//
//	For now, nothing!
//----------------------------------------------------------------------

void AddrSpace::SaveState() 
{}

//----------------------------------------------------------------------
// AddrSpace::RestoreState
// 	On a context switch, restore the machine state so that
//	this address space can run.
//
//      For now, tell the machine where to find the page table.
//----------------------------------------------------------------------

void AddrSpace::RestoreState() 
{    // 除以下赋值操作外没有显式对页表的操作
    machine->pageTable = pageTable;
    machine->pageTableSize = numPages;
}

void AddrSpace::Print() {
    printf("page table dump:  %d pages  in total\n", numPages); 
    printf("=============================\n"); 
    printf("\tVirtPage, \tPhysPage\n");

    for (int i=0; i < numPages;  i++) {
        printf("\t %d, \t\t%d\n", pageTable[i].virtualPage, pageTable[i].physicalPage);
    }
    printf("============================================\n\n");
}

int AddrSpace::getSpaceId() {
    return spaceId;
}