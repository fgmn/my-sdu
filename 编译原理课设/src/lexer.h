//
// Created by DELL on 2023/4/21.
//

#ifndef PL0_COMPILER23_LEXER_H
#define PL0_COMPILER23_LEXER_H

#include <istream>
#include <vector>
#include "symbol.h"

/**
 * 词法分析器
 */
class Lexer
{
public:
    Lexer(std::istream &_inputStream) : inputStream(_inputStream)
    {}
    Symbol getSymbol();
    bool isEOF();

private:
    std::istream &inputStream;
    char get();
    char peek();
};

#endif //PL0_COMPILER23_LEXER_H
