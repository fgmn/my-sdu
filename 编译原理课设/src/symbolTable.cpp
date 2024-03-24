//
// Created by zkr on 2023/5/8.
//


#include <iostream>
#include <iomanip>
#include "symbolTable.h"

bool SymbolTable::inTable(const std::string &name)
{
    return mp.find(name) != mp.end();
}

void SymbolTable::addSymbol(Symbol *symbol)
{
    mp[symbol->value] = symbol; // 用于快速查找
    lst.push_back(symbol);   // 用于遍历访问
}

void SymbolTableManager::addSymbol(const std::string &symbolName, int id, const std::string &name, int number, int level, int address)
{   // 往top(tblptr)添加符号
    symbolTableStack.back()->addSymbol(new Symbol(symbolName, id, name, number, level, address));
}

Symbol *SymbolTableManager::getLastProcedure()
{   // 获取最近出现的“程序”关键字
//    Symbol *ret = nullptr;   // 一个指向Symbol*类型的迭代器
//    std::cerr << "begin\n";
    for (auto it = symbolTableStack.rbegin(); it != symbolTableStack.rend(); it++) {    // fixed
        for (auto it2 =  (*it)->lst.rbegin(); it2 != (*it)->lst.rend(); it2++) {
//            std::cerr << *(*it2) << '\n';
//            if ("procedure" == (*it2)->name && ret == nullptr)
//                ret = *it2;
            if ("procedure" == (*it2)->name) {
                return *it2;
            }
        }
//        std::cerr << '\n';
    }
//    std::cerr << "end\n\n";
//    if (ret != nullptr) return ret;
    return nullptr;
}

bool SymbolTableManager::inTable(const std::string &name)
{   // 在当前符号表（symbolTableStack.back()）中查找符号
    return symbolTableStack.back()->inTable(name);
}


Symbol *SymbolTableManager::getSymbol(const std::string &name)
{   // lookup，在<有效>符号表中查找某一个变量
//    for (auto *t : symbolTableStack)
//    {
//        auto it = t->mp.find(name);
//        if (it != t->mp.end())
//            return it->second;
//    }


    // 保证：访问的变量是递归过程中最接近的变量 fixed
    for (auto it = symbolTableStack.rbegin(); it != symbolTableStack.rend(); it++) {
        auto res = (*it)->mp.find(name);
        if (res != (*it)->mp.end()) {
            return res->second;
        }
    }

    return nullptr;
}

void SymbolTableManager::pushTable()
{
    symbolTableStack.push_back(new SymbolTable());
    symbolTableList.push_back(symbolTableStack.back());
}

void SymbolTableManager::popTable()
{
    symbolTableStack.pop_back();
}

void SymbolTableManager::printTables()
{
    int idx = 0;
    for (auto *t : symbolTableList)
    {
        std::cout << "TX" << idx++ << "->" << std::endl;
        for (auto *symbol : t->lst)
        {
            std::cout << "NAME: ";
            std::cout << std::setiosflags(std::ios::left) << std::setfill(' ') << std::setw(10) << symbol->value;
            std::cout << "KIND: ";
            std::cout << std::setiosflags(std::ios::left) << std::setfill(' ') << std::setw(10)
                      << symbol->name;
            if ("const" == symbol->name)
            {
                std::cout << "VAL: ";
                std::cout << std::setiosflags(std::ios::left) << std::setfill(' ') << std::setw(10)
                          << symbol->number;
            } else
            {
                std::cout << "LEVEL: ";
                std::cout << std::setiosflags(std::ios::left) << std::setfill(' ') << std::setw(10)
                          << symbol->level;
                std::cout << "ADR: ";
                std::cout << std::setiosflags(std::ios::left) << std::setfill(' ') << std::setw(10)
                          << symbol->address;
            }
            std::cout << std::endl;
        }
    }
}

