//
// Created by DELL on 2023/4/28.
//

#include <utility>
#include <unordered_map>
#include <string>
#include <iostream>
#include <cctype>
#include <vector>
#include <cassert>
#include <fstream>
#include <algorithm>
#include <sstream>

// # define DEBUG

// 调试begin
#ifdef DEBUG
void dbg() { std::cout << "\n"; }
template<typename T, typename... A>
void dbg(T a, A... x) { std::cout << a << ' '; dbg(x...); }
#define logv(x...) std::cout << #x << " -> "; dbg(x);
#define logs(x...) dbg(x);
int vectorToInt(std::vector<char> &v);
#else
#define logs(...)

#define logv(...)

#endif

int vectorToInt(std::vector<char> &v)
{
    int num = 0;
    for (char c : v)
    {
        assert(isdigit(c));
        num = num * 10 + (c - '0');
    }
    return num;
}
// 调试end

// 是否出现词法错误
bool LexicalError = 0;

/*
 * 符号类
 */
class Symbol
{
public:
    // explicit关键字声明，该方法只能用于初始化而不能用于隐式转换
    explicit Symbol(std::string _name, int _id): name(std::move(_name)), id(_id) {}
    explicit Symbol(std::string _name, int _id, std::string _value): name(std::move(_name)), id(_id), value(_value) {}

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

char Lexer::get()
{
    char ch = inputStream.get();
    // logs("LOG[Lexer: get]: ", ch);
    return ch;
}

char Lexer::peek()
{   // 查看但不提取该字符
    char ch = inputStream.peek();
    // logs("LOG[Lexer: peek]: ", ch);
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
        return Symbol("number", 33, std::to_string(number));
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

/* 词法分析测试代码 */
int main(int argc, char **argv)
{
//    std::freopen("e.out", "w", stdout);
//    std::ifstream fin("e.in");
   
    Lexer lexer(std::cin);
    // Lexer lexer(fin);
    // 输出到字符串中，判定没有词法错误才输出该字符串

    std::ostringstream os;
    while (!lexer.isEOF())
    {
        Symbol symbol = lexer.getSymbol();
        if (symbol.name == "$") break;
        if (symbol.name == "illegal") LexicalError = true;
        os << symbol;
        os << '\n';
    }

    if (LexicalError) {
        std::cout << "Lexical Error";
    }
    else {
        std::cout << os.str();
    }
    return 0;
}