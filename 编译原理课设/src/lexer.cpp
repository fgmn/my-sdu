//
// Created by DELL on 2023/4/21.
//

//#define DEBUG    // 调试模式

#include <iostream>
#include <cctype>
#include "utils.h"
#include "lexer.h"

// 是否出现词法错误
bool LexicalError = 0;

char Lexer::get()
{
    char ch = inputStream.get();
//    logs("LOG[Lexer: get]: ", ch);
    return ch;
}

char Lexer::peek()
{   // 查看但不提取该字符
    char ch = inputStream.peek();
//    logs("LOG[Lexer: peek]: ", ch);
    return ch;
}

bool Lexer::isEOF()
{
    return inputStream.eof();
}

Symbol Lexer::getSymbol()
{
    if (isEOF())
        return Symbol("$", 34);

    // 滤掉单词间空格，Tab以及换行符
    while(isspace(peek()) || peek() == '\t' || peek() == '\n')
        get();

    // 最后可能是空行，滤掉空格之后到达文件末尾
    if (isEOF())
        return Symbol("$", 34);

    std::vector<char> strToken;
    if (isalpha(peek()))
    {   // 首字符是字母，可能是标识符或者关键字类型
        do
        {
            strToken.emplace_back(get());
        } while(isalpha(peek()) || isdigit(peek()));
        if (strToken.size() > 10) LexicalError = true;
        // 拼接得到字符串，并转换为小写进行比较
        std::string value(strToken.begin(), strToken.end());
        std::transform(value.begin(), value.end(), value.begin(), ::tolower);
        auto it = SYMBOL::KEYWORD_MAP.find(value);
        // 如果在关键字表中查找到，则返回对应Symbol；否则识别为标识符
        return it != SYMBOL::KEYWORD_MAP.end() ? it->second : Symbol("identifier", 32, value);
    } else if (isdigit(peek()))
    {   // 首字符是数字，为常量类型
        do
        {
            strToken.emplace_back(get());
            if (isalpha(peek())) { LexicalError = true; }
        } while(isdigit(peek()));
        if (strToken.size() > 10) LexicalError = true;
        // 拼接为一个数
        int number = vectorToInt(strToken);
        return Symbol("number", 33, number);    // fixed
    }
    // 可能是算符，界符
    std::string str(1, get());
    char pk = peek();
    switch (str[0])
    {
        case '<': case '>': case ':':
            if(pk == '=') str += get();
    }
    auto it = SYMBOL::SIGN_MAP.find(str);
    logs("LOG[getSymbol: ToFind]", str, "\n");
    return it != SYMBOL::SIGN_MAP.end() ? it->second : Symbol("illegal", 35);
}

/*
 * Symbol("identifier", 32, value)
 * Symbol("number", 33, std::to_string(number))
 * Symbol("$", 34)
 * Symbol("illegal", 35)
 */