// ring.cc
//	Routines to implement a ring buffer for producer and consumer 
//      problem.
//	
// Copyright (c) 1995 The Regents of the University of Southern Queensland.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

extern "C" {
#include <stdio.h>
extern int exit(int st);
}

#include "ring.h"
#include "system.h"

//----------------------------------------------------------------------
// slot::slot
// 	The constructor for the slot class.  
//----------------------------------------------------------------------

slot::slot(int id, int number)
{
    thread_id = id;
    value = number;
}



//----------------------------------------------------------------------
// Ring::Ring
// 	The constructor for the Ring class.  Note that it doesn't have a
// 	return type.
//
// 	"sz" -- maximum number of elements in the ring buffer at any time
//----------------------------------------------------------------------

Ring::Ring(int sz)
{
    if (sz < 1) {
	fprintf(stderr, "Error: Ring: size %d too small\n", sz);
	exit(1);
    }

    // Initialize the data members of the ring object.
    size = sz;
    in = 0;
    out = 0;
    current = 0;
    buffer = new slot[size]; //allocate an array of slots.

    // Initialize condition variables
    notfull = new Condition_H("notfull");
    notempty = new Condition_H("notempty");

    // Initialize the samaphors of the montior ring
    mutex = new Semaphore("mutex", 1);
    next = new Semaphore("next", 0);
    next_count = 0;
}

//----------------------------------------------------------------------
// Ring::~Ring
// 	The destructor for the Ring class.  Just get rid of the array we
// 	allocated in the constructor.
//----------------------------------------------------------------------

Ring::~Ring()
{
    // Some compilers and books tell you to write this as:
    //     delete [size] stack;
    // but apparently G++ doesn't like that.

    delete [] buffer;

    delete notfull;
    delete notempty;

    delete mutex;
    delete next;
}

//----------------------------------------------------------------------
// Ring::Put
// 	Put a message into the next available empty slot. We assume the
//       caller has done necesaary synchronization.
//
//	"message" -- the message to be put in the buffer
//----------------------------------------------------------------------

void
Ring::Put(slot *message)
{

    mutex->P();
    
    if (current == size) 
    {

        notfull->Wait(mutex, next, &next_count);

    }

    buffer[in].thread_id = message->thread_id;
    buffer[in].value = message->value;
    current++;
    in = (in + 1) % size;

    notempty->Signal(next, &next_count);
    
    if (next_count > 0) 
	next->V();
    else 
	mutex->V();
}

//----------------------------------------------------------------------
// Ring::Get
// 	Get a message from the next full slot. We assume the
//       caller has done necesaary synchronization.
//
//	"message" -- the message from  the buffer
//----------------------------------------------------------------------

void
Ring::Get(slot *message)
{

    mutex->P();
	
    if (current == 0) 
    {
	
	notempty->Wait(mutex, next, &next_count);

    }
    
    message->thread_id = buffer[out].thread_id;
    message->value = buffer[out].value;
    current--;
    out = (out + 1) % size;

    notfull->Signal(next, &next_count);

    if (next_count > 0) 
	next->V();
    else 
	mutex->V();
}

int
Ring::Empty()
{
// to be implemented
}

int
Ring::Full()
{
// to be implemented
}


