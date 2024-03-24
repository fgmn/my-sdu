//
// Created by zkr on 2023/5/12.
//

#include <iostream>
#include <fstream>
#include "lexer.h"
#include "vm.h"
#include "code.h"

/* 解释器 */
int main(int argc, char **argv)
{
    // 解释器需要从 program.code 读取机器码
    // 从标准输入读取 read 指令对应的数字，并将 write 指令对应的数字输出到标准输出
//    std::cout << "Here is Interpreter\n";
    std::vector<Code*> codeTable;
    std::ifstream fin("program.code");
    std::string str;
    while (fin >> str) {
        // 若int类型是有效的，可以强制类型转化为枚举类型
        Code* code = new Code(CODE::CODE_TYPE(CODE::RFIND_MAP.find(str)->second));
        fin >> code->l;
        fin >> code->a;
//        std::cerr << *code <<'\n';
        codeTable.push_back(code);
    }
    fin.close();
    VM vm(codeTable);
    vm.run();
    return 0;
}