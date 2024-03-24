//
// Created by zkr on 2023/5/10.
//

#ifndef PL0_COMPILER23_VM_H
#define PL0_COMPILER23_VM_H

#include <utility>
#include <vector>
#include "code.h"

class VM
{
public:
    VM(std::vector<Code *> codeTable) : codeTable(std::move(codeTable))
    {}
    void run();

private:
    std::vector<Code*> codeTable;
    int stack[400010];  // 太大会导致栈溢出
    int top;        // SP，栈顶寄存器，指向运行栈的顶端
    int base;       // BP，基址寄存器，指向当前过程调用所分配的空间在栈中的起始地址
    int pc;         // IP，指令地址寄存器，存放下一条指令的地址
    Code* code;     // IR，指令寄存器，存放正在执行的指令
    int getBaseAddress(int nowBaseAddress, int levelDiff);
    void opr();
};

#endif //PL0_COMPILER23_VM_H
