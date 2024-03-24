// scheduler.cc 
//	Routines to choose the next thread to run, and to dispatch to
//	that thread.
//
// 	These routines assume that interrupts are already disabled.
//	If interrupts are disabled, we can assume mutual exclusion
//	(since we are on a uniprocessor).
//
// 	NOTE: We can't use Locks to provide mutual exclusion here, since
// 	if we needed to wait for a lock, and the lock was busy, we would 
//	end up calling FindNextToRun(), and that would put us in an 
//	infinite loop.
//
// 	Very simple implementation -- no priorities, straight FIFO.
//	Might need to be improved in later assignments.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "scheduler.h"
#include "system.h"

//----------------------------------------------------------------------
// Scheduler::Scheduler
// 	Initialize the list of ready but not running threads to empty.
//----------------------------------------------------------------------

Scheduler::Scheduler()
{ 
    readyList = new List; 
#ifdef USER_PROGRAM
    waitingList = new List;
    terminatedList = new List;
#endif
} 

//----------------------------------------------------------------------
// Scheduler::~Scheduler
// 	De-allocate the list of ready threads.
//----------------------------------------------------------------------

Scheduler::~Scheduler()
{ 
    delete readyList; 
} 

//----------------------------------------------------------------------
// Scheduler::ReadyToRun
// 	Mark a thread as ready, but not running.
//	Put it on the ready list, for later scheduling onto the CPU.
//
//	"thread" is the thread to be put on the ready list.
//----------------------------------------------------------------------

void
Scheduler::ReadyToRun (Thread *thread)
{
    DEBUG('t', "Putting thread %s on ready list.\n", thread->getName());

    thread->setStatus(READY);
    readyList->Append((void *)thread);
}

//----------------------------------------------------------------------
// Scheduler::FindNextToRun
// 	Return the next thread to be scheduled onto the CPU.
//	If there are no ready threads, return NULL.
// Side effect:
//	Thread is removed from the ready list.
//----------------------------------------------------------------------

Thread *
Scheduler::FindNextToRun ()
{
    return (Thread *)readyList->Remove();
}

//----------------------------------------------------------------------
// Scheduler::Run
// 	Dispatch the CPU to nextThread.  Save the state of the old thread,
//	and load the state of the new thread, by calling the machine
//	dependent context switch routine, SWITCH.
//
//      Note: we assume the state of the previously running thread has
//	already been changed from running to blocked or ready (depending).
// Side effect:
//	The global variable currentThread becomes nextThread.
//
//	"nextThread" is the thread to be put into the CPU.
//----------------------------------------------------------------------

void
Scheduler::Run (Thread *nextThread)
{   // 运行一个新线程

    Thread *oldThread = currentThread;
    
#ifdef USER_PROGRAM			// ignore until running user programs 
    // 1. 将当前CPU寄存器的内容保存到旧进程的用户寄存器中
    // 2. 保存用户页表
    if (currentThread->pcb->space != NULL) {	// if this thread is a user program,
        currentThread->SaveUserState(); // save the user's CPU registers
	    currentThread->pcb->space->SaveState();
    }
#endif
    
    oldThread->CheckOverflow();		    // check if the old thread
					    // had an undetected stack overflow

    currentThread = nextThread;		    // switch to the next thread
    currentThread->setStatus(RUNNING);      // nextThread is now running
    // 将线程指针指向新线程
    DEBUG('t', "Switching from thread \"%s\" to thread \"%s\"\n",
	  oldThread->getName(), nextThread->getName());
    
    // This is a machine-dependent assembly language routine defined 
    // in switch.s.  You may have to think
    // a bit to figure out what happens after this, both from the point
    // of view of the thread and from the perspective of the "outside world".
    
    // 从线程和外部世界的视角
    SWITCH(oldThread, nextThread);  
    // 此时寄存器（非通用数据寄存器，可能是PC，SP等状态寄存器）的状态还未保存和更新
    
    DEBUG('t', "Now in thread \"%s\"\n", currentThread->getName());

    // If the old thread gave up the processor because it was finishing,
    // we need to delete its carcass.  Note we cannot delete the thread
    // before now (for example, in Thread::Finish()), because up to this
    // point, we were still running on the old thread's stack!
    if (threadToBeDestroyed != NULL) {
        delete threadToBeDestroyed;     // 回收旧线程的堆栈
	    threadToBeDestroyed = NULL;
    }
    
#ifdef USER_PROGRAM
    // 1. 如果运行用户进程，需要将当前进程的用户寄存器内存加载到CPU寄存器中
    // 2. 并且加载用户页表
    if (currentThread->pcb->space != NULL) {		// if there is an address space
        currentThread->RestoreUserState();     // to restore, do it.
	    currentThread->pcb->space->RestoreState();
    }
#endif
}

//----------------------------------------------------------------------
// Scheduler::Print
// 	Print the scheduler state -- in other words, the contents of
//	the ready list.  For debugging.
//----------------------------------------------------------------------
void
Scheduler::Print()
{
    printf("Ready list contents:\n");
    readyList->Mapcar((VoidFunctionPtr) ThreadPrint);
}

#ifdef USER_PROGRAM

List *Scheduler::getTerminatedList()
{
    return terminatedList;
}

List *Scheduler::getWaitingList()
{
    return waitingList;
}

void Scheduler::deleteTerminatedThread(Thread *thread)
{
    // if (!terminatedList->RemoveByItem((void *)thread))
    // {
    //     DEBUG('t', "deleteTerminatedThread %s false\n", thread->getName());
    //     ASSERT(false);
    // }
    // delete thread;

    terminatedList->RemoveByItem((void *)thread);
    delete thread;
}

void Scheduler::deleteWaitingThread(Thread *thread)
{
    // if (!waitingList->RemoveByItem((void *)thread))
    // {
    //     DEBUG('t', "deleteWaitingThread %s false\n", thread->getName());
    //     ASSERT(false);
    // }
    // delete thread;

    waitingList->RemoveByItem((void *)thread);
}
#endif
