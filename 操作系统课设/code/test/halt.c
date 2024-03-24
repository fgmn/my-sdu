/* halt.c
 *	Simple program to test whether running a user program works.
 *	
 *	Just do a "syscall" that shuts down the OS.
 *
 * 	NOTE: for some reason, user programs with global data structures 
 *	sometimes haven't worked in the Nachos environment.  So be careful
 *	out there!  One option is to allocate data structures as 
 * 	automatics within a procedure, but if you do this, you have to
 *	be careful to allocate a big enough stack to hold the automatics!
 */

#include "syscall.h"

int
main()
{
    // int i;
    // long long sum = 0;
    // char wd[1];
    // OpenFileId output = ConsoleOutput;
    // while (i < 1e8)
    // {
    //     sum += i;
    // }
    // wd[0] = '1';
    // Write(wd, 2, output);
    Exit(1);
    /* not reached */
}