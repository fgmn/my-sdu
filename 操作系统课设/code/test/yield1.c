#include "syscall.h"
int main()
{
    int i;
    for (i = 0; i < 5; i++) {
        Yield();
    }
    Exit(1);
}