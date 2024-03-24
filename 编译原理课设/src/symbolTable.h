//
// Created by zkr on 2023/5/7.
//

#ifndef PL0_COMPILER23_SYMBOLTABLE_H
#define PL0_COMPILER23_SYMBOLTABLE_H

#include <unordered_map>
#include <list>
#include "symbol.h"

class SymbolTable
{   // 符号表
public:
    std::unordered_map<std::string, Symbol*> mp;
    std::list<Symbol*> lst;
    bool inTable(const std::string& name);  // 判断符号是否在符号表中出现
    void addSymbol(Symbol* symbol);         // 添加符号项
};

class SymbolTableManager
{
    std::list<SymbolTable*> symbolTableStack;  // 符号表 栈
    std::list<SymbolTable*> symbolTableList;   // 符号表 数组
public:
    SymbolTableManager()
    {
        pushTable();
    }

    void addSymbol(const std::string &symbolName, int id, const std::string &name, int number, int level, int address);
    Symbol *getLastProcedure();
    bool inTable(const std::string& name);
    Symbol *getSymbol(const std::string &name);
    void pushTable();
    void popTable();
    void printTables();
};

#endif //PL0_COMPILER23_SYMBOLTABLE_H
