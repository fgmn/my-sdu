// synch.cc 
//	Routines for synchronizing threads.  Three kinds of
//	synchronization routines are defined here: semaphores, locks 
//   	and condition variables (the implementation of the last two
//	are left to the reader).
//
// Any implementation of a synchronization routine needs some
// primitive atomic operation.  We assume Nachos is running on
// a uniprocessor, and thus atomicity can be provided by
// turning off interrupts.  While interrupts are disabled, no
// context switch can occur, and thus the current thread is guaranteed
// to hold the CPU throughout, until interrupts are reenabled.
//
// Because some of these routines might be called with interrupts
// already disabled (Semaphore::V for one), instead of turning
// on interrupts at the end of the atomic operation, we always simply
// re-set the interrupt state back to its original value (whether
// that be disabled or enabled).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "synch.h"
#include "system.h"

//----------------------------------------------------------------------
// Semaphore::Semaphore
// 	Initialize a semaphore, so that it can be used for synchronization.
//
//	"debugName" is an arbitrary name, useful for debugging.
//	"initialValue" is the initial value of the semaphore.
//----------------------------------------------------------------------

Semaphore::Semaphore(char* debugName, int initialValue)
{
    name = debugName;
    value = initialValue;
    queue = new List;
}

//----------------------------------------------------------------------
// Semaphore::~Semaphore
// 	De-allocate semaphore, when no longer needed.  Assume no one
//	is still waiting on the semaphore!
//----------------------------------------------------------------------

Semaphore::~Semaphore()
{
    delete queue;
}

//----------------------------------------------------------------------
// Semaphore::P
// 	Wait until semaphore value > 0, then decrement.  Checking the
//	value and decrementing must be done atomically, so we
//	need to disable interrupts before checking the value.
//
//	Note that Thread::Sleep assumes that interrupts are disabled
//	when it is called.
//----------------------------------------------------------------------

void
Semaphore::P()
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts
    
    while (value == 0) { 			// semaphore not available
	queue->Append((void *)currentThread);	// so go to sleep
	currentThread->Sleep();
    } 
    value--; 					// semaphore available, 
						// consume its value
    
    (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
}

//----------------------------------------------------------------------
// Semaphore::V
// 	Increment semaphore value, waking up a waiter if necessary.
//	As with P(), this operation must be atomic, so we need to disable
//	interrupts.  Scheduler::ReadyToRun() assumes that threads
//	are disabled when it is called.
//----------------------------------------------------------------------

void
Semaphore::V()
{
    Thread *thread;
    IntStatus oldLevel = interrupt->SetLevel(IntOff);

    thread = (Thread *)queue->Remove();
    if (thread != NULL)	   // make thread ready, consuming the V immediately
	scheduler->ReadyToRun(thread);
    value++;
    (void) interrupt->SetLevel(oldLevel);
}


//----------------------------------------------------------------------
// Lock::Lock
// 	Initialize a lock, so that it can be used for synchronization.
//
//	"debugName" is an arbitrary name, useful for debugging.
//----------------------------------------------------------------------


Lock::Lock(char* debugName) 
{
    name = debugName;
    owner = NULL;
    lock = new Semaphore(name,1);
}


//----------------------------------------------------------------------
// Lock::~Lock
// 	De-allocate lock, when no longer needed.  As with semaphore,
//	assume no one is still waiting on the lock.
//----------------------------------------------------------------------
Lock::~Lock() 
{
    delete lock;
}

//----------------------------------------------------------------------
// Lock::Acquire
//      Use a binary semaphore to implement the lock.  Record which 
//      thread acquired the lock in order to assure that only the
//      same thread releases it.
//----------------------------------------------------------------------
void Lock::Acquire() 
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);  // disable interrupts

    lock->P();                            // procure the semaphore
    owner = currentThread;                // record the new owner of the lock
    (void) interrupt->SetLevel(oldLevel); // re-enable interrupts
}

//----------------------------------------------------------------------
// Lock::Release
//      Set the lock to be free (i.e. vanquish the semaphore).  Check
//      that the currentThread is allowed to release this lock.
//----------------------------------------------------------------------
void Lock::Release() 
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);  // disable interrupts

    // Ensure: a) lock is BUSY  b) this thread is the same one that acquired it.
    ASSERT(currentThread == owner);        
    owner = NULL;                          // clear the owner
    lock->V();                             // vanquish the semaphore
    (void) interrupt->SetLevel(oldLevel);
}


//----------------------------------------------------------------------
// Lock::isHeldByCurrentThread
//----------------------------------------------------------------------
bool Lock::isHeldByCurrentThread()
{
    bool result;
    IntStatus oldLevel = interrupt->SetLevel(IntOff);

    result = currentThread == owner;
    (void) interrupt->SetLevel(oldLevel);
    return(result);
}

//----------------------------------------------------------------------
// Condition::Condition
// 	Initialize a condition variable, so that it can be used for 
//      synchronization.
//
//	"debugName" is an arbitrary name, useful for debugging.
//----------------------------------------------------------------------
Condition::Condition(char* debugName) 
{ 
    name = debugName;
    queue = new List;
    lock = NULL;
}

//----------------------------------------------------------------------
// Condition::~Condition
// 	De-allocate a condition variable, when no longer needed.  As
//      with semaphore, assume no one is still waiting on the condition.
//----------------------------------------------------------------------

Condition::~Condition() 
{ 
    delete queue;
}

//----------------------------------------------------------------------
// Condition::Wait
//
//      Release the lock, relinquish the CPU until signaled, then
//      re-acquire the lock.
//
//      Pre-conditions:  currentThread is holding the lock; threads in
//      the queue are waiting on the same lock.
//----------------------------------------------------------------------
void Condition::Wait(Lock* conditionLock) 
{ 
    IntStatus oldLevel = interrupt->SetLevel(IntOff);

    ASSERT(conditionLock->isHeldByCurrentThread());  // check pre-condition
    if(queue->IsEmpty()) {
	lock = conditionLock;  // helps to enforce pre-condition
    } 
    ASSERT(lock == conditionLock); // another pre-condition
    queue->Append(currentThread);  // add this thread to the waiting list
    conditionLock->Release();      // release the lock
    currentThread->Sleep();        // goto sleep
    conditionLock->Acquire();      // awaken: re-acquire the lock
    (void) interrupt->SetLevel(oldLevel);
}

//----------------------------------------------------------------------
// Condition::Signal
//      Wake up a thread, if there are any waiting on the condition.
//   
//      Pre-conditions:  currentThread is holding the lock; threads in
//      the queue are waiting on the same lock.
//----------------------------------------------------------------------
void Condition::Signal(Lock* conditionLock) 
{ 
    Thread *nextThread;
    IntStatus oldLevel = interrupt->SetLevel(IntOff);

    ASSERT(conditionLock->isHeldByCurrentThread());
    if(!queue->IsEmpty()) {
	ASSERT(lock == conditionLock);
	nextThread = (Thread *)queue->Remove();
	scheduler->ReadyToRun(nextThread);      // wake up the thread
    } 
    (void) interrupt->SetLevel(oldLevel);
}

//----------------------------------------------------------------------
// Condition::Broadcast
//      Wake up all threads waiting on the condition.   
//
//      Pre-conditions:  currentThread is holding the lock; threads in
//      the queue are waiting on the same lock.
//----------------------------------------------------------------------
void Condition::Broadcast(Lock* conditionLock) 
{ 
    Thread *nextThread;
    IntStatus oldLevel = interrupt->SetLevel(IntOff);

    ASSERT(conditionLock->isHeldByCurrentThread());
    if(!queue->IsEmpty()) {
	ASSERT(lock == conditionLock);
	while((nextThread = (Thread *)queue->Remove()) != NULL) {
	    scheduler->ReadyToRun(nextThread);  // wake up the thread
	}
    } 
    (void) interrupt->SetLevel(oldLevel);
}
