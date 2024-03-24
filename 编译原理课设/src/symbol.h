//
// Created by DELL on 2023/4/21.
//

#ifndef PL0_COMPILER23_SYMBOL_H
#define PL0_COMPILER23_SYMBOL_H

#include <utility>
#include <unordered_map>
#include <ostream>
#include <string>
#include <algorithm>

/*
 * 符号类
 */
class Symbol
{
public:
    // explicit关键字声明，该方法只能用于初始化而不能用于隐式转换
    explicit Symbol(std::string _name, int _id): name(std::move(_name)), id(_id) {}
    explicit Symbol(std::string _name, int _id, int _number): name(std::move(_name)), id(_id), number(_number) {}           // 常量声明
    explicit Symbol(std::string _name, int _id, std::string _value): name(std::move(_name)), id(_id), value(_value) {}      // 变量声明，value存储变量名

    Symbol(std::string _name, int _id, std::string _value, int _number, int _level, int _address):
            name(std::move(_name)), id(_id), value(std::move(_value)), number(_number), level(_level), address(_address) {}

    friend std::ostream &operator << (std::ostream &os, const Symbol &symbol)
    {
        std::string t = symbol.name;
        std::transform(t.begin(), t.end(), t.begin(), ::toupper);
        os << t;    // 将字符串转化为大写
        if (!symbol.value.empty()) {
            t = symbol.value;
            std::transform(t.begin(), t.end(), t.begin(), ::toupper);
            os << ' ' << t;
        }
        else if(-1 != symbol.number)
            os << '(' << symbol.number << ')';
        return os;
    }

    std::string name = "";
    int id = -1;
    std::string value = "";
    int number = -1;
    int level = -1;
    int address = -1;
};

namespace SYMBOL {
// name唯一标识一个词语，词语可能具有诸多属性，因此需要为它构造一个类Symbol
// 目前词法分析中仅仅使用了name
// 必须使用const修饰，否则报错重定义

const std::unordered_map<std::string, Symbol> KEYWORD_MAP({
    /* 关键字 */
    {"begin", Symbol("begin",1)},
    {"end", Symbol("end", 2)},
    {"if", Symbol("if", 3)},
    {"then", Symbol("then", 4)},
    {"else", Symbol("else", 5)},
    {"const", Symbol("const", 6)},
    {"procedure", Symbol("procedure", 7)},
    {"var", Symbol("var", 8)},
    {"do", Symbol("do", 9)},
    {"while", Symbol("while", 10)},
    {"call", Symbol("call", 11)},
    {"read", Symbol("read", 12)},
    {"write", Symbol("write", 13)},
    {"odd", Symbol("odd", 14)},
});

const std::unordered_map<std::string, Symbol> SIGN_MAP({
    /* 算符 */
    {"=", Symbol("=", 15)},
    {"<", Symbol("<", 16)},
    {"<=", Symbol("<=", 17)},
    {">", Symbol(">", 18)},
    {">=", Symbol(">=", 19)},
    {"#", Symbol("#", 20)},
    {"+", Symbol("+", 21)},
    {"-", Symbol("-", 22)},
    {"*", Symbol("*", 23)},
    {"/", Symbol("/", 24)},
    {":=", Symbol(":=", 26)},
    /* 界符 */
    {",", Symbol(",", 27)},
    {";", Symbol(";", 28)},
    {".", Symbol(".", 29)},
    {"(", Symbol("(", 30)},
    {")", Symbol(")", 31)},
});
}

#endif //PL0_COMPILER23_SYMBOL_H
