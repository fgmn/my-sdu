// synchtest.cc
//      An other test case for threads.  Uses locks and condition variables 
//      to implement a Bridge (see question 7).
//     
//      SynchThread just sends a car back and forth across the bridge.  By
//      forking a bunch of these, you can simulate traffic at the bridge!
//


#include "copyright.h"
#include "system.h"
#include "synch.h"

// See question 7.  The bridge can hold a maximum of 3 cars.  It is
// one-lane, so cars may cross in one direction at a time only--otherwise
// there is a head-on collision.    //  
class Bridge {      
  private:
    int numCars;
    int currentDirec;
    Condition *bridgeFull;
    Lock *lock;
  public:
    Bridge();
    ~Bridge();
    void Arrive(int direc);   // Returns when it is OK for the car to cross
    void Cross(int direc);    // Doesn't do anything, except maybe print messages
    void Exit(int direc);     // Leave the bridge
};

//----------------------------------------------------------------------
// Bridge::Bridge
//     Initialise the bridge to its initial state
//----------------------------------------------------------------------
Bridge::Bridge()
{
    numCars = 0;
    currentDirec = 0;
    bridgeFull = new Condition("bridge");
    lock = new Lock("bridge");
}

//----------------------------------------------------------------------
// Bridge::~Bridge
//----------------------------------------------------------------------
Bridge::~Bridge()
{
    delete bridgeFull;
    delete lock;
}

//----------------------------------------------------------------------
// Bridge::Arrive
//         Car arrives at the bridge, wishing to cross in direction
//         direc.  
//----------------------------------------------------------------------
void
Bridge::Arrive(int direc)
{
    DEBUG('t', "Arriving at bridge.  Direction [%d]", direc);
    lock->Acquire();

    // We can proceed if:  
    //       a) the bridge is empty
    //    or b) the bridge has less than 3 cars and traffic is flowing
    //          in the direction we want
    // Otherwise, wait
    while((numCars > 0) && ((numCars >= 3) || (direc != currentDirec))) {
	bridgeFull->Wait(lock);
    }
    numCars++;               // reserve a spot on the bridge
    currentDirec = direc;    // make sure the direction matches 
    lock->Release();
    DEBUG('t', "Direction [%d], ready to cross bridge now", direc);
}

//----------------------------------------------------------------------
// Bridge::Exit
//         Car leaves bridge.
//----------------------------------------------------------------------
void
Bridge::Exit(int direc)
{
    lock->Acquire();
    numCars--;              // vacate our spot on the bridge
    DEBUG('t', "Direction [%d], bridge exit", direc);
    bridgeFull->Broadcast(lock);  // signal all those waiting for the bridge
    lock->Release();
}

//----------------------------------------------------------------------
// Bridge::Cross
//         Car crosses bridge.
//----------------------------------------------------------------------
void
Bridge::Cross(int direc)
{
    DEBUG('t', "Direction [%d], crossing bridge", direc);
}


//  A testcase based on the bridge class.  
//
//
Bridge *bridge = new Bridge;

//----------------------------------------------------------------------
// SynchThread
//      Simulates a car that crosses back and forth across the bridge
//      repeatedly--must have a nice view! :).
//
//----------------------------------------------------------------------
void
SynchThread(_int which)
{
    int num;
    int direc;
    
    for (num = 0; num < 5; num++) {
        direc = num % 2;  // set direction (alternates)
// The following printf's need fixing for the different types of _int of which
	printf("Direction [%d], Car [%d], Arriving...\n", direc, which);
	bridge->Arrive(direc);
	currentThread->Yield();
	printf("Direction [%d], Car [%d], Crossing...\n", direc, which);
	bridge->Cross(direc);
	currentThread->Yield();
        printf("Direction [%d], Car [%d], Exiting...\n", direc, which);
	bridge->Exit(direc);
	currentThread->Yield();
    }
}

void
SynchTest()
{
    const int maxCars = 7;  // How much traffic?
    int i = 0;
    Thread *ts[maxCars];

    for(i=0; i < maxCars; i++) {
	ts[i] = new Thread("forked thread");
	ts[i]->Fork(SynchThread, i);
    }
}
