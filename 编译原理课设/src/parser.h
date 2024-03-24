//
// Created by zkr on 2023/5/7.
//

#ifndef PL0_COMPILER23_PARSER_H
#define PL0_COMPILER23_PARSER_H


#include "lexer.h"
#include "symbolTable.h"
#include "code.h"
#include "syntaxTree.h"

class Parser
{
    Lexer &lexer;
    int level = 0;
    int address = 3;
    Symbol nowSymbol = Symbol("illegal", 35);
public:
    SymbolTableManager symbolTable;
    std::vector<Code*> codeTable;
    SyntaxTree syntaxTree;
    explicit Parser(Lexer &lexer);
    void printTables() { symbolTable.printTables(); }
    void printCode();
    // 〈程序〉→〈分程序〉.
    void program();

private:
    void advance();
    void addressReset();
    int getCodeAddress();

    // 〈分程序〉→ [〈常量说明部分〉][〈变量说明部分〉][〈过程说明部分〉]〈语句〉
    void subProbgram();
    // 〈常量说明部分〉 → const〈常量定义〉{ ,〈常量定义〉}；
    void constDeclare();
    // 〈常量定义〉 → 〈标识符〉=〈无符号整数〉
    void constDefine();
    // 〈变量说明部分〉 → var〈标识符〉{ ,〈标识符〉}；
    void varDeclare();
    // 〈过程说明部分〉 → 〈过程首部〉〈分程序〉；{〈过程说明部分〉}
    void procDeclare();
    // 〈语句〉 → 〈赋值语句〉|〈条件语句〉|〈当型循环语句〉|〈过程调用语句〉|〈读语句〉|〈写语句〉|〈复合语句〉|〈空〉
    void statement();
    // 〈赋值语句〉 → 〈标识符〉:=〈表达式〉
    bool assignStatement();
    // 〈条件语句〉 → if〈条件〉then〈语句〉
    bool ifStatement();
    // 〈当型循环语句〉 → while〈条件〉do〈语句〉
    bool whileStatement();
    // 〈过程调用语句〉 → call〈标识符〉
    bool callStatement();
    // 〈读语句〉 → read(〈标识符〉{ ，〈标识符〉})
    bool readStatement();
    // 〈写语句〉 → write(〈表达式〉{，〈表达式〉})
    bool writeStatement();
    // 〈复合语句〉 → begin〈语句〉{ ；〈语句〉}〈end〉
    bool beginStatement();
    // 〈分号〉
    void semicolon();
    // ) 右括号
    void rightBracket();
    // 〈表达式〉 → [+|-]〈项〉{〈加减运算符〉〈项〉}
    void expression();
    // 〈项〉 → 〈因子〉{〈乘除运算符〉〈因子〉}
    void term();
    // 〈因子〉 → 〈标识符〉|〈无符号整数〉|(〈表达式〉)
    void factor();
    // 〈条件〉 → 〈表达式〉〈关系运算符〉〈表达式〉|odd〈表达式〉
    void condition();

};

#endif //PL0_COMPILER23_PARSER_H
