//
// Created by zkr on 2023/5/10.
//

#include <iostream>
#include <cstring>
#include "vm.h"


int VM::getBaseAddress(int nowBaseAddress, int levelDiff)
{   // 获取对应层的基地址base
    // 过程调用栈中的数据分布如下：
    // base+4:变量b
    // base+3:变量a
    // base+2:返回地址
    // base+1:静态链
    // base:动态链
    while (levelDiff--)
        nowBaseAddress = stack[nowBaseAddress + 1]; // 通过静态链访问非局部量
    return nowBaseAddress;
}

void VM::run()
{
    top = pc = base = 0;
    memset(stack, 0, sizeof(stack));
    do
    {
        code = codeTable.at(pc++);
//        std::cerr << *code << '\n';
        switch (code->getF())
        {
            // 常量送栈顶
            case CODE::LIT:
                stack[top++] = code->getA();
                break;
                // 变量送栈顶
            case CODE::LOD:
                // 将于当前层层差为 l 的层，变量相对位置为 a 的变量复制到栈顶
                stack[top++] = stack[getBaseAddress(base, code->getL()) + code->getA()];
                break;
                // 栈顶送变量
            case CODE::STO:
                // 将栈顶内容复制到于当前层层差为 l 的层，变量相对位置为 a 的变量
                stack[getBaseAddress(base, code->getL()) + code->getA()] = stack[--top];
                break;
                // 调用
            case CODE::CAL:
                // 调用过程。l 标明层差，a 表明目标程序地址
                // 注意区分代码空间codeTable和栈式存储器空间stack
                // pc指向codeTable，top以及base指向stack
                stack[top] = base;   // 动态链
                // 静态链指向直接外层的基地址，要调用某个内层调用其外层一定在栈中
                if (code->getL() <= -1) {
                    exit(1);
                }
                else {
                    stack[top + 1] = getBaseAddress(base, code->getL()); // 静态链
//                    std::cout << base << ' ' << code->getL() << '\n';
//                    std::cout << "static link:" << stack[top + 1] << '\n';
//                    std::cout << "dynamic link:" << stack[top] << '\n';
                }
                stack[top + 2] = pc; // 返回地址
//                std::cout << "\nbegin\n";
//                for (int i = 0; i <= top + 2; i ++) {
//                    std::cout << i << ": " << stack[i] << '\n';
//                }   std::cout << '\n';
//                std::cout << "end\n";
                base = top;          // 不修改top，前面已将address+3，生成code后会产生INT语句，修改top值
                pc = code->getA();
                break;
                // 开辟空间
            case CODE::INT:
                top = top + code->getA(); // 3(静态链、动态链、返回地址)+变量+常量
                break;
                // 无条件跳转
            case CODE::JMP:
                pc = code->getA();
                break;
                // 有条件跳转
            case CODE::JPC:
                if (stack[top - 1] == 0)
                    pc = code->getA();
                break;
                // 操作
            case CODE::OPR:
                this->opr();
                break;
        }
        // 执行完指令之后显示栈的运行状态
    } while (pc != 0);
}

void VM::opr()
{
    switch (code->getA())
    {
        case OP::RET:
            top = base;
            pc = stack[base + 2];// 返回地址
            base = stack[base];  // 动态链
            break;
        case OP::NEG:
            stack[top - 1] = -stack[top - 1];
            break;
        case OP::ADD:
            stack[top - 2] = stack[top - 1] + stack[top - 2];
            --top;
            break;
        case OP::SUB:
            stack[top - 2] = stack[top - 2] - stack[top - 1];
            --top;
            break;
        case OP::MUL:
            stack[top - 2] = stack[top - 2] * stack[top - 1];
            --top;
            break;
        case OP::DIV:
            stack[top - 2] = stack[top - 2] / stack[top - 1];
            --top;
            break;
        case OP::LEQ:
            stack[top - 2] = stack[top - 2] <= stack[top - 1];
            --top;
            break;
        case OP::LS:
            stack[top - 2] = stack[top - 2] < stack[top - 1];
            --top;
            break;
        case OP::EQU:
            stack[top - 2] = (stack[top - 2] == stack[top - 1]);
            --top;
            break;
        case OP::NEQ:
            stack[top - 2] = (stack[top - 2] != stack[top - 1]);
            --top;
            break;
        case OP::GT:
            stack[top - 2] = stack[top - 2] > stack[top - 1];
            --top;
            break;
        case OP::GEQ:
            stack[top - 2] = stack[top - 2] >= stack[top - 1];
            --top;
            break;
        case OP::WRITE:
            std::cout << stack[top - 1] << '\t';
            break;
        case OP::LINE:
            std::cout << std::endl;
            break;
        case OP::READ:
//            std::cout << "Please Input: ";
            std::cin >> stack[top++];
            break;
        case OP::ODD:   // 奇数为真
            stack[top - 1] &= 1;
            break;
    }
}