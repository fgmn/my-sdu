#include "syscall.h"
int main()
{
    SpaceId pid;
    int i;
    // 变量必须定义在函数体最前面
    pid = Exec("../test/yield1.noff");
    for (i = 0; i < 5; i++) {
        Yield();
    }
    Exit(99);
}