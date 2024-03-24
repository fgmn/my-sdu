// ring++.h
//	Data structures for a ring buffer to be used in producer and
//      consumer problem
//
// Copyright (c) 1995 The Regents of the University of Southern Queensland.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.


// The following defines the ring buffer class.  The functions are
// implemented in the file ring.cc.
//
// The constructor (initializer) for the ring burrer is passed with an
// integer for the size of the buffer (the number of slots). 

// class of the slot in the ring-buffer
class slot {
    public:
    slot(int id, int number);
    slot() { thread_id = 0; value = 0;};
    
    int thread_id;
    int value;
    };


class Ring {
  public:
    Ring(int sz);    // Constructor:  initialize variables, allocate space.
    ~Ring();         // Destructor:   deallocate space allocated above.

    
    void Put(slot *message); // Put a message the next empty slot.
    
    void Get(slot *message); // Get a message from the next  full slot.
                                            
    int Full();       // Returns non-0 if the ring is full, 0 otherwise.
    int Empty();      // Returns non-0 if the ring is empty, 0 otherwise.
    
  private:
    int size;         // The size of the ring buffer.
    int in, out;      // Index of 
    slot *buffer;       // A pointer to an array for the ring buffer.
};


