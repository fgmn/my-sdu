#include "syscall.h"
int main()
{
    SpaceId pid;
    pid = Exec("../test/halt.noff");
    Join(pid);
    Exit(99);
    /* not reached */
}