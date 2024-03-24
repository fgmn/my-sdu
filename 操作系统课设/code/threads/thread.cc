// thread.cc 
//	Routines to manage threads.  There are four main operations:
//
//	Fork -- create a thread to run a procedure concurrently
//		with the caller (this is done in two steps -- first
//		allocate the Thread object, then call Fork on it)
//	Finish -- called when the forked procedure finishes, to clean up
//	Yield -- relinquish control over the CPU to another ready thread
//	Sleep -- relinquish control over the CPU, but thread is now blocked.
//		In other words, it will not run again, until explicitly 
//		put back on the ready queue.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "thread.h"
#include "switch.h"
#include "synch.h"
#include "system.h"

#define STACK_FENCEPOST 0xdeadbeef	// this is put at the top of the
					// execution stack, for detecting 
					// stack overflows

//----------------------------------------------------------------------
// Thread::Thread
// 	Initialize a thread control block, so that we can then call
//	Thread::Fork.
//
//	"threadName" is an arbitrary string, useful for debugging.
//----------------------------------------------------------------------

Thread::Thread(char* threadName)
{
    name = threadName;
    stackTop = NULL;
    stack = NULL;
    status = JUST_CREATED;
#ifdef USER_PROGRAM
    space = NULL;
#endif
}

//----------------------------------------------------------------------
// Thread::~Thread
// 	De-allocate a thread.
//
// 	NOTE: the current thread *cannot* delete itself directly,
//	since it is still running on the stack that we need to delete.
//
//      NOTE: if this is the main thread, we can't delete the stack
//      because we didn't allocate it -- we got it automatically
//      as part of starting up Nachos.
//----------------------------------------------------------------------

Thread::~Thread()
{
    DEBUG('t', "Deleting thread \"%s\"\n", name);

    ASSERT(this != currentThread);
    if (stack != NULL)
		DeallocBoundedArray((char *) stack, StackSize * sizeof(_int));
}

//----------------------------------------------------------------------
// Thread::Fork
// 	Invoke (*func)(arg), allowing caller and callee to execute 
//	concurrently.
//
//	NOTE: although our definition allows only a single integer argument
//	to be passed to the procedure, it is possible to pass multiple
//	arguments by making them fields of a structure, and passing a pointer
//	to the structure as "arg".
//
// 	Implemented as the following steps:
//		1. Allocate a stack
//		2. Initialize the stack so that a call to SWITCH will
//		cause it to run the procedure
//		3. Put the thread on the ready queue
// 	
//	"func" is the procedure to run concurrently.
//	"arg" is a single argument to be passed to the procedure.
//----------------------------------------------------------------------

void 
Thread::Fork(VoidFunctionPtr func, _int arg)
{
#ifdef HOST_ALPHA
    DEBUG('t', "Forking thread \"%s\" with func = 0x%lx, arg = %ld\n",
	  name, (long) func, arg);
#else
    DEBUG('t', "Forking thread \"%s\" with func = 0x%x, arg = %d\n",
	  name, (int) func, arg);
#endif
    
    StackAllocate(func, arg);
    // 关中断，保证操作的原子性
    IntStatus oldLevel = interrupt->SetLevel(IntOff);
    scheduler->ReadyToRun(this);	// ReadyToRun assumes that interrupts 
					// are disabled!
    (void) interrupt->SetLevel(oldLevel);
}    

//----------------------------------------------------------------------
// Thread::CheckOverflow
// 	Check a thread's stack to see if it has overrun the space
//	that has been allocated for it.  If we had a smarter compiler,
//	we wouldn't need to worry about this, but we don't.
//
// 	NOTE: Nachos will not catch all stack overflow conditions.
//	In other words, your program may still crash because of an overflow.
//
// 	If you get bizarre results (such as seg faults where there is no code)
// 	then you *may* need to increase the stack size.  You can avoid stack
// 	overflows by not putting large data structures on the stack.
// 	Don't do this: void foo() { int bigArray[10000]; ... }
//----------------------------------------------------------------------

void
Thread::CheckOverflow()
{
    if (stack != NULL)
#ifdef HOST_SNAKE			// Stacks grow upward on the Snakes
	ASSERT((unsigned int)stack[StackSize - 1] == STACK_FENCEPOST);
#else
	ASSERT((unsigned int)*stack == STACK_FENCEPOST);
#endif
}

//----------------------------------------------------------------------
// Thread::Finish
// 	Called by ThreadRoot when a thread is done executing the 
//	forked procedure.
//
// 	NOTE: we don't immediately de-allocate the thread data structure 
//	or the execution stack, because we're still running in the thread 
//	and we're still on the stack!  Instead, we set "threadToBeDestroyed", 
//	so that Scheduler::Run() will call the destructor, once we're
//	running in the context of a different thread.
//
// 	NOTE: we disable interrupts, so that we don't get a time slice 
//	between setting threadToBeDestroyed, and going to sleep.
//----------------------------------------------------------------------

//
void
Thread::Finish ()
{
    (void) interrupt->SetLevel(IntOff);		
    ASSERT(this == currentThread);
    
    DEBUG('t', "Finishing thread \"%s\"\n", getName());
    
    threadToBeDestroyed = currentThread;
    Sleep();					// invokes SWITCH
    // not reached
}

//----------------------------------------------------------------------
// Thread::Yield
// 	Relinquish the CPU if any other thread is ready to run.
//	If so, put the thread on the end of the ready list, so that
//	it will eventually be re-scheduled.
//
//	NOTE: returns immediately if no other thread on the ready queue.
//	Otherwise returns when the thread eventually works its way
//	to the front of the ready list and gets re-scheduled.
//
//	NOTE: we disable interrupts, so that looking at the thread
//	on the front of the ready list, and switching to it, can be done
//	atomically.  On return, we re-set the interrupt level to its
//	original state, in case we are called with interrupts disabled. 
//
// 	Similar to Thread::Sleep(), but a little different.
//----------------------------------------------------------------------

void
Thread::Yield ()
{
    Thread *nextThread;
    IntStatus oldLevel = interrupt->SetLevel(IntOff);
    
    ASSERT(this == currentThread);
    
    DEBUG('t', "Yielding thread \"%s\"\n", getName());
    
    nextThread = scheduler->FindNextToRun();
    if (nextThread != NULL) {
        scheduler->ReadyToRun(this);
        scheduler->Run(nextThread);
    }
    (void) interrupt->SetLevel(oldLevel);
}

//----------------------------------------------------------------------
// Thread::Sleep
// 	Relinquish the CPU, because the current thread is blocked
//	waiting on a synchronization variable (Semaphore, Lock, or Condition).
//	Eventually, some thread will wake this thread up, and put it
//	back on the ready queue, so that it can be re-scheduled.
//
//	NOTE: if there are no threads on the ready queue, that means
//	we have no thread to run.  "Interrupt::Idle" is called
//	to signify that we should idle the CPU until the next I/O interrupt
//	occurs (the only thing that could cause a thread to become
//	ready to run).
//
//	NOTE: we assume interrupts are already disabled, because it
//	is called from the synchronization routines which must
//	disable interrupts for atomicity.   We need interrupts off 
//	so that there can't be a time slice between pulling the first thread
//	off the ready list, and switching to it.
//----------------------------------------------------------------------
void
Thread::Sleep ()
{
    Thread *nextThread;
    
    ASSERT(this == currentThread);
    ASSERT(interrupt->getLevel() == IntOff);
    
    DEBUG('t', "Sleeping thread \"%s\"\n", getName());
    // 阻塞当前线程，寻找下一线程以执行
    status = BLOCKED;
    while ((nextThread = scheduler->FindNextToRun()) == NULL)
	interrupt->Idle();	// no one to run, wait for an interrupt
        
    scheduler->Run(nextThread); // returns when we've been signalled
}

//----------------------------------------------------------------------
// ThreadFinish, InterruptEnable, ThreadPrint
//	Dummy functions because C++ does not allow a pointer to a member
//	function.  So in order to do this, we create a dummy C function
//	(which we can pass a pointer to), that then simply calls the 
//	member function.
//----------------------------------------------------------------------

static void ThreadFinish()    { currentThread->Finish(); }
static void InterruptEnable() { interrupt->Enable(); }
void ThreadPrint(_int arg){ Thread *t = (Thread *)arg; t->Print(); }

//----------------------------------------------------------------------
// Thread::StackAllocate
//	Allocate and initialize an execution stack.  The stack is
//	initialized with an initial stack frame for ThreadRoot, which:
//		enables interrupts
//		calls (*func)(arg)
//		calls Thread::Finish
//
//	"func" is the procedure to be forked
//	"arg" is the parameter to be passed to the procedure
//----------------------------------------------------------------------

void
Thread::StackAllocate (VoidFunctionPtr func, _int arg)
{   // 栈分配
    stack = (int *) AllocBoundedArray(StackSize * sizeof(_int));

#ifdef HOST_SNAKE
    // HP stack works from low addresses to high addresses
    stackTop = stack + 16;	// HP requires 64-byte frame marker
    stack[StackSize - 1] = STACK_FENCEPOST;
#else
    // i386 & MIPS & SPARC & ALPHA stack works from high addresses to low addresses
#ifdef HOST_SPARC
    // SPARC stack must contains at least 1 activation record to start with.
    stackTop = stack + StackSize - 96;
#else  // HOST_MIPS  || HOST_i386 || HOST_ALPHA
    stackTop = stack + StackSize - 4;	// -4 to be on the safe side!
#ifdef HOST_i386
    // the 80386 passes the return address on the stack.  In order for
    // SWITCH() to go to ThreadRoot when we switch to this thread, the
    // return addres used in SWITCH() must be the starting address of
    // ThreadRoot.
    // 在此处将返回地址设置为ThreadBoot地址

    //    *(--stackTop) = (int)ThreadRoot;
    // This statement can be commented out after a bug in SWITCH function
    // of i386 has been fixed: The current last three instruction of 
    // i386 SWITCH is as follows: 
    // movl    %eax,4(%esp)            # copy over the ret address on the stack
    // movl    _eax_save,%eax
    // ret
    // Here "movl    %eax,4(%esp)" should be "movl   %eax,0(%esp)". 
    // After this bug is fixed, the starting address of ThreadRoot,
    // which is stored in machineState[PCState] by the next stament, 
    // will be put to the location pointed by %esp when the SWITCH function
    // "return" to ThreadRoot.
    // It seems that this statement was used to get around that bug in SWITCH.
    //
    // However, this statement will be needed, if SWITCH for i386 is
    // further simplified. In fact, the code to save and 
    // retore the return address are all redundent, because the
    // return address is already in the stack (pointed by %esp).
    // That is, the following four instructions can be removed:
    // ...
    // movl    0(%esp),%ebx            # get return address from stack into ebx
    // movl    %ebx,_PC(%eax)          # save it into the pc storage
    // ...
    // movl    _PC(%eax),%eax          # restore return address into eax
    // movl    %eax,0(%esp)            # copy over the ret address on the stack#    

    // The SWITCH function can be as follows:
//         .comm   _eax_save,4

//         .globl  SWITCH
// SWITCH:
//         movl    %eax,_eax_save          # save the value of eax
//         movl    4(%esp),%eax            # move pointer to t1 into eax
//         movl    %ebx,_EBX(%eax)         # save registers
//         movl    %ecx,_ECX(%eax)
//         movl    %edx,_EDX(%eax)
//         movl    %esi,_ESI(%eax)
//         movl    %edi,_EDI(%eax)
//         movl    %ebp,_EBP(%eax)
//         movl    %esp,_ESP(%eax)         # save stack pointer
//         movl    _eax_save,%ebx          # get the saved value of eax
//         movl    %ebx,_EAX(%eax)         # store it

//         movl    8(%esp),%eax            # move pointer to t2 into eax

//         movl    _EAX(%eax),%ebx         # get new value for eax into ebx
//         movl    %ebx,_eax_save          # save it
//         movl    _EBX(%eax),%ebx         # retore old registers
//         movl    _ECX(%eax),%ecx
//         movl    _EDX(%eax),%edx
//         movl    _ESI(%eax),%esi
//         movl    _EDI(%eax),%edi
//         movl    _EBP(%eax),%ebp
//         movl    _ESP(%eax),%esp         # restore stack pointer
	
//         movl    _eax_save,%eax

//         ret

    //In this case the above statement 
    //    *(--stackTop) = (int)ThreadRoot;
    // is necesssary. But, the following statement
    //    machineState[PCState] = (_int) ThreadRoot;
    // becomes redundant.

    // Peiyi Tang, ptang@titus.compsci.ualr.edu
    // Department of Computer Science
    // University of Arkansas at Little Rock
    // Sep 1, 2003



#endif
#endif  // HOST_SPARC
    *stack = STACK_FENCEPOST;
#endif  // HOST_SNAKE
    
    machineState[PCState] = (_int) ThreadRoot;
    machineState[StartupPCState] = (_int) InterruptEnable;
    machineState[InitialPCState] = (_int) func;
    machineState[InitialArgState] = arg;
    machineState[WhenDonePCState] = (_int) ThreadFinish;
}

#ifdef USER_PROGRAM
#include "machine.h"

//----------------------------------------------------------------------
// Thread::SaveUserState
//	Save the CPU state of a user program on a context switch.
//
//	Note that a user program thread has *two* sets of CPU registers -- 
//	one for its state while executing user code, one for its state 
//	while executing kernel code.  This routine saves the former.
//----------------------------------------------------------------------

void
Thread::SaveUserState()
{
    for (int i = 0; i < NumTotalRegs; i++)
	userRegisters[i] = machine->ReadRegister(i);
}

//----------------------------------------------------------------------
// Thread::RestoreUserState
//	Restore the CPU state of a user program on a context switch.
//
//	Note that a user program thread has *two* sets of CPU registers -- 
//	one for its state while executing user code, one for its state 
//	while executing kernel code.  This routine restores the former.
//----------------------------------------------------------------------

void
Thread::RestoreUserState()
{
    for (int i = 0; i < NumTotalRegs; i++)
	machine->WriteRegister(i, userRegisters[i]);
}
#endif
