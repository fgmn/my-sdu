#include "syscall.h"
int main()
{
    SpaceId pid;
    pid = Exec("../test/halt.noff");   //利用你所实现的Exec()执行../test/halt.noff
    // pid = Exec("../test/halt.noff");
    Exit(99);
    // Halt();
    /* not reached */
}