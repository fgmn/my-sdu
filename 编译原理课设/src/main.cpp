#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer.h"
#include "parser.h"
#include "vm.h"

//extern bool LexicalError;

/* 词法分析测试代码 */
//int main(int argc, char **argv)
//{
//    std::freopen("out", "w", stdout);
//    std::ifstream fin("../test/PL0_code6.in");
//    Lexer lexer(fin);
//
//    // 输出到字符串中，判定没有词法错误才输出该字符串
//    std::ostringstream os;
//    while (!lexer.isEOF())
//    {
//        Symbol symbol = lexer.getSymbol();
//        if (symbol.name == "$") break;
//        if (symbol.name == "illegal") LexicalError = true;
//        os << symbol;
//        os << '\n';
//    }   // 在语法分析中，取词逻辑在advance()中实现，取词变成一种主动行为，对将要取的词有一定的期望
//
//    if (LexicalError) {
//        std::cout << "Lexical Error";
//    }
//    else {
//        std::cout << os.str();
//    }
//    return 0;
//}

/* 语法分析测试代码 */
//int main(int argc, char **argv)
//{
//    // 提交注意事项：
//    // 1.修改输入，现通过ONLINE_JUDGE定义实现
//    // 2.打开SUMBIT_VERSION宏定义
//    // 3.关闭onSymbolTable
////#define ONLINE_JUDGE
//#ifdef ONLINE_JUDGE
//    Lexer lexer(std::cin);
//#else
//    std::freopen("out", "w", stdout);
//    std::ifstream fin("../test/spfa/program.txt");
//    Lexer lexer(fin);
//#endif
//
//    Parser parser(lexer);
//    parser.program();
////    parser.printCode();
////    parser.printTables();
//    parser.syntaxTree.print();
//    return 0;
//}

/* 编译器 */
int main(int argc, char **argv)
{
#ifdef LOCAL_JUDGE
//    std::freopen("out", "w", stdout);
//    std::freopen("program.code", "w", stdout);
    std::ifstream fin("../test/my/recursion_indirect.in");
    Lexer lexer(fin);
#else
    Lexer lexer(std::cin);
#endif
    Parser parser(lexer);
    parser.program();

#ifdef LOCAL_JUDGE
    parser.printCode();
    parser.printTables();
//    parser.syntaxTree.print();
    VM vm(parser.codeTable);
    vm.run();
#else
    // Here is Compiler
    // 编译器从标准输入读取 PL/0 源程序，并将编译后的机器码输出到标准输出
    parser.printCode();
#endif

    return 0;
}