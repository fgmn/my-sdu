#include "syscall.h"

float InvSqrt(float x) {
    float xhalf = 0.5f * x;
    int i = *(int*)&x;
    i = 0x5f375a86 - (i >> 1);
    x = *(float*)&i;
    x = x * (1.5f - xhalf * x * x);
    return x;
}

int main()
{
    // Halt();
    float x = 15;
    int y = InvSqrt(x * x);
    Exit(y);
}