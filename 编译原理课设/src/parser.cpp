//
// Created by zkr on 2023/5/7.
//

//#define DEBUG    // 调试模式

#include <iostream>
#include <sstream>
#include "parser.h"
#include "exceptionHandler.h"
#include "utils.h"
#include "lexer.h"

extern bool LexicalError;
// 语法分析不考虑符号表
// 同时关闭相关的ASSERT，但是不能关闭中间代码生成
// 由于中间代码还未使用，参数使用一个填充值

Parser::Parser(Lexer &lexer) : lexer(lexer) {
    advance();
}

// 取下一个 词
void Parser::advance() {
    if ("illegal" != nowSymbol.name)
        // 通过唯一标识符号name进行比较匹配
    {   // nowSymbol初始化为illegal
        if (!nowSymbol.value.empty())
            syntaxTree.merge(nowSymbol.value, 1);
        else if (-1 != nowSymbol.number)
            syntaxTree.merge(std::to_string(nowSymbol.number), 1);
        else {
            if (nowSymbol.id >= 15 && nowSymbol.id <= 24) {
                // 加减，乘除，关系运算符是叶子节点
                syntaxTree.merge(nowSymbol.name, 1);
            } else { syntaxTree.merge(nowSymbol.name); }
        }
    }
    // 原先在main函数中的取词逻辑
    nowSymbol = lexer.getSymbol();  // getSymbol函数中变量名过长也会导致词法错误
//    std::cerr << nowSymbol << '\n';
    // 发生词法错误之后不需要进行接下来的分析
    if (nowSymbol.name == "illegal") LexicalError = true;
    if (nowSymbol.name == "$") {}
    if (LexicalError) {
//        std::cout << "Lexical Error\n";
//        exit(0);
        exit(1);
    }
    logs("LOG[Parser: advance] ", nowSymbol);
}

// 每个过程的相对起始位置在 BLOCK 内置初值 DX=3
void Parser::addressReset() {
    address = 3;
}

int Parser::getCodeAddress() {
    return codeTable.size();
}

// 〈程序〉→〈分程序〉.
// "."是程序的结束符
void Parser::program() {
    logs("LOG[Parser: program]");
    syntaxTree.begin("PROGRAM");

    /* 语义分析 */
    subProbgram();                      // 〈分程序〉
    ASSERT("." == nowSymbol.name, EXCEPTION::MISSING_SEMICOLON); // 异常处理: 缺少 '.'
    advance();                          // 跳过 '.'
    ASSERT(lexer.isEOF(), EXCEPTION::EXTRA_CHARACTERS); // 异常处理: 文件尾 '.' 后还有字符

    syntaxTree.end();
}

// 〈分程序〉→ [〈常量说明部分〉][〈变量说明部分〉][〈过程说明部分〉]〈语句〉
void Parser::subProbgram() {
    logs("LOG[Parser: subProbgram]");
    syntaxTree.begin("SUBPROG");

    /* 中间代码生成 */
    // 递归调用时需要保存当前过程中的address
    int addressTemp = address; // 保存 address
    addressReset();
    Symbol *procedure = symbolTable.getLastProcedure(); // 得到当前〈分程序〉所在的过程 Proc
    Code *jmp = new Code(CODE::JMP);
    codeTable.push_back(jmp);   // 等待回填
    // 跳过所有说明部分，不予以执行

    /* 语义分析 */
    constDeclare(); // [〈常量说明部分〉]
    varDeclare();   // [〈变量说明部分〉]
    procDeclare();  // [〈过程说明部分〉]

    /* 中间代码生成 */
    jmp->setA(getCodeAddress());                            // 过程体的目标代码生成后, 返填过程体的入口地址
    codeTable.push_back(new Code(CODE::INT, address));      // 申请栈空间，大小即 address=3+常量声明（没有增加address）+变量声明
    if (procedure != nullptr)                                  // 不是主函数，那该过程的起始语句的地址要保存在符号表
    {
//        std::cerr << *procedure << ' ' << procedure->level << ' ' << procedure->address << '\n';
//        std::cerr << getCodeAddress() - 1 << '\n';

        // backpatch操作
        int t = procedure->address, nxt;
        while (t != -1) {
            nxt = codeTable[t]->a;
            codeTable[t]->a = getCodeAddress() - 1;
            t = nxt;
        }
        procedure->address = getCodeAddress() - 1;  // fixed
    }   // 起始语句即上一句INT指令

    /* 语义分析 */
    statement();   // 〈语句〉

    /* 中间代码生成 */
    codeTable.push_back(new Code(CODE::OPR, OP::RET)); // 过程结束, 返回调用点
    address = addressTemp;

    syntaxTree.end();
}

// 〈常量说明部分〉 → const〈常量定义〉{ ,〈常量定义〉}；
void Parser::constDeclare() {
    logs("LOG[Parser: constDeclare]");
    syntaxTree.begin("CONSTANTDECLARE");

    if ("const" == nowSymbol.name) { // const
        advance();     // 跳过 'const'
        constDefine();
        while ("," == nowSymbol.name) {
            advance(); // 跳过 ','
            constDefine();
        }
        // 语句完成由分号结束
        semicolon();
    }

    syntaxTree.end();
}

// 〈常量定义〉 → 〈标识符〉=〈无符号整数〉
// 〈无符号整数〉 → 〈数字〉{〈数字〉}
void Parser::constDefine() {
    logs("LOG[Parser: constDefine]");
    syntaxTree.begin("CONSTANTDEFINE");

    ASSERT("identifier" == nowSymbol.name, EXCEPTION::MISSING_IDENTIFIER); // 异常处理: 缺少标识符
    std::string constName = nowSymbol.value;
    ASSERT(!symbolTable.inTable(constName), EXCEPTION::DUPLICATE_IDENTIFIER); // 特判标识符重复声明
    advance();                               // 跳过标识符
    ASSERT("=" == nowSymbol.name, EXCEPTION::MISSING_EQUAL); // 异常处理: 缺少 '='
    advance();                               // 跳过 '='
    ASSERT("number" == nowSymbol.name, EXCEPTION::MISSING_NUMBER); // 异常处理: 缺少无符号整数
//    std::cerr << nowSymbol.number << '\n';
    symbolTable.addSymbol("const", 6, constName, nowSymbol.number, level, address);
    // 符号表中添加该const变量
    advance();                               // 跳过 无符号整数

    syntaxTree.end();
}

// 〈变量说明部分〉 → var〈标识符〉{ ,〈标识符〉}；
void Parser::varDeclare() {
    logs("LOG[Parser: varDeclare]");
    syntaxTree.begin("VARIABLEDECLARE");

    if ("var" == nowSymbol.name) {
        do {
            advance();                                  // 跳过 'var' or ','
            ASSERT("identifier" == nowSymbol.name, EXCEPTION::MISSING_IDENTIFIER); // 异常处理: 缺少标识符
            std::string varName = nowSymbol.value;
            ASSERT(!symbolTable.inTable(varName), EXCEPTION::DUPLICATE_IDENTIFIER); // 特判标识符重复声明
            symbolTable.addSymbol("var", 8, varName, 0, level, address);    // 变量初始为0
            address++;                                  // 地址++
            advance();                                  // 跳过 标识符
        } while ("," == nowSymbol.name);       // 识别到逗号，one more variable
        semicolon();                                    // 跳过 ';'
    }

    syntaxTree.end();
}

// 〈过程说明部分〉 → 〈过程首部〉〈分程序〉；{〈过程说明部分〉}
// 〈过程首部〉 → procedure〈标识符〉；
void Parser::procDeclare() {
    logs("LOG[Parser: procDeclare]");
    syntaxTree.begin("PROCEDUREDECLARE");

    if ("procedure" == nowSymbol.name) {
        /* 过程首部 */
        syntaxTree.begin("PROCEDUREHEAD");
        advance();                               // 跳过 'procedure'

        ASSERT("identifier" == nowSymbol.name, EXCEPTION::MISSING_IDENTIFIER); // 异常处理: 缺少标识符
        std::string procName = nowSymbol.value;
        ASSERT(!symbolTable.inTable(procName), EXCEPTION::DUPLICATE_IDENTIFIER);    // 特判标识符重复声明

        // 将过程加入符号表中
//        std::cerr << procName << ' ' << 0 << ' ' << level << ' ' << address << '\n';
        symbolTable.addSymbol("procedure", 7, procName, 0, level, -1);  // 初始化为-1
        address++;
        advance();                               // 跳过 标识符
        semicolon();                             // 跳过 ';'
        syntaxTree.end();

        /* 进入〈分程序〉*/
        level++;
        // 嵌套定义过多，超过3层
        ASSERT(level <= 3, EXCEPTION::TOO_MUCH_NESTING);

        symbolTable.pushTable(); // 创建一张子符号表
        subProbgram(); // 分程序
        symbolTable.popTable();
        level--;
        semicolon();                            // 跳过 ';'
    }
    if ("procedure" == nowSymbol.name) {
        procDeclare();
    }   // 递归调用解析<过程说明部分>
    syntaxTree.end();
}

// 〈语句〉 → 〈赋值语句〉|〈条件语句〉|〈当型循环语句〉|〈过程调用语句〉|〈读语句〉|〈写语句〉|〈复合语句〉|〈空语句〉
void Parser::statement() {
    logs("LOG[Parser: statement]");
    syntaxTree.begin("SENTENCE");
    if (assignStatement() ||
        ifStatement() ||
        whileStatement() ||
        callStatement() ||
        writeStatement() ||
        readStatement() ||
        beginStatement()) {}
    else {
        // 〈语句〉 → 〈空〉
        logs("LOG[Parser: empty]");
        syntaxTree.merge("empty");
    }
    syntaxTree.end();
}

// 〈赋值语句〉 → 〈标识符〉:=〈表达式〉
bool Parser::assignStatement() {
    logs("LOG[Parser: assignStatement]");
    if ("identifier" == nowSymbol.name) {
        syntaxTree.begin("ASSIGNMENT");
        std::string varName = nowSymbol.value;
        Symbol *symbol = symbolTable.getSymbol(varName);
        // 通过Symbol指针对符号变量进行赋值
        ASSERT(nullptr != symbol, EXCEPTION::MISSING_IDENTIFIER);          // 异常处理: 缺少标识符
        ASSERT("var" == symbol->name, EXCEPTION::NOT_A_VAR); // 异常处理: 并非变量
        advance();                                   // 跳过 标识符
        ASSERT(":=" == nowSymbol.name, EXCEPTION::MISSING_CEQUAL); // 异常处理: 缺少赋值号
        advance();                                   // 跳过 ':='
        expression();
        /* 中间代码生成 */
        // 对变量的修改最终一定是通过赋值语句！
        // 通过STO命令将变量存储至栈中用INT命令开辟出来的变量区域（在静态链、动态链、返回地址之后）
        codeTable.push_back(new Code(CODE::STO, level - symbol->level, symbol->address)); // 赋值
        syntaxTree.end();
        return true;
    }
    return false;
}

// 〈条件语句〉 → if〈条件〉then〈语句〉
bool Parser::ifStatement() {
    logs("LOG[Parser: ifStatement]");
    if ("if" == nowSymbol.name) {
        syntaxTree.begin("IFSENTENCE");

        /* 语义分析 */
        advance();                         // 跳过 'IF'
        condition();
        ASSERT("then" == nowSymbol.name, EXCEPTION::MISSING_THEN); // 缺少 THEN
        advance();                         // 跳过 'THEN'

        /* 中间代码生成 */
        Code *jpc = new Code(CODE::JPC);// 条件转移代码, 地址回填
        codeTable.push_back(jpc);

        /* 语义分析 */
        statement();

        /* 中间代码生成 */
        jpc->setA(getCodeAddress());   // 回填地址

        syntaxTree.end();
        return true;
    }
    return false;
}

// 〈当型循环语句〉 → while〈条件〉do〈语句〉
bool Parser::whileStatement() {
    logs("LOG[Parser: whileStatement]");
    if ("while" == nowSymbol.name) {
        syntaxTree.begin("WHILESENTENCE");

        /* 语义分析 */
        advance();                          // 跳过 'while'

        /* 中间代码生成 */
        int whileBeginAddress = getCodeAddress();

        /* 语义分析 */
        condition();
        ASSERT("do" == nowSymbol.name, EXCEPTION::MISSING_DO); // 缺少 DO
        advance(); // 跳过 'do'

        /* 中间代码生成 */
        Code *jpc = new Code(CODE::JPC);// 条件转移代码, 地址回填
        codeTable.push_back(jpc);

        /* 语义分析 */
        statement();
        // 执行完<语句>，无条件跳转回条件判断处
        codeTable.push_back(new Code(CODE::JMP, whileBeginAddress)); // 无条件跳转
        // <条件>不满足，跳转至<语句>之后
        jpc->setA(getCodeAddress());

        syntaxTree.end();
        return true;
    }
    return false;
}

// 〈过程调用语句〉 → call〈标识符〉
bool Parser::callStatement() {
    logs("LOG[Parser: callStatement]");
    if ("call" == nowSymbol.name) {
        syntaxTree.begin("CALLSENTENCE");

        /* 语义分析 */
        advance();                               // 跳过 'CALL'
        ASSERT("identifier" == nowSymbol.name, EXCEPTION::MISSING_IDENTIFIER);// 异常处理: 缺少标识符
        std::string procName = nowSymbol.value;
        // fixed
        Symbol *symbol = symbolTable.getSymbol(procName);
        ASSERT(nullptr != symbol, EXCEPTION::NOT_A_PROCUDURE);
        // CALL调用 标识符必须是procedure类型
        ASSERT("procedure" == symbol->name, EXCEPTION::NOT_A_PROCUDURE); // 并非过程
        advance();                               // 跳过 '标识符'

        /* 语义细节！主要通过符号表活动栈处理
         * 1.函数可以调用：
         * 比自己先声明的同层次函数
         * 被自己 直接 包含的函数
         * 函数本身
         * 包含自己的高层次函数
         * 2.不可以调用：
         * 被自己间接包含的函数：不会在自己的符号表中出现，因此不会导致问题
         * 在自己后面声明的同层次函数
         * */

        /* 中间代码生成 */
        // 如何执行CALL语句？参见vm.cpp
        // 执行CAL主要是构建该过程的活动链、静态链以及确定返回地址
        // 调用自己的孩子则level - symbol->level为0

//        std::cerr << "call " << symbol->value << ' ' << symbol->address << '\n';
//        std::cerr << level << ' ' << symbol->level << '\n';

        // 实际上该符号表已经从活动栈中弹出，不会访问到
        ASSERT(level - symbol->level >= 0, EXCEPTION::CALL_INDIRECTLY_CONTAIN);

        if (level - symbol->level >= 2) {
            int t = symbol->address;
            symbol->address = getCodeAddress(); // 指向下面将产生的CAL语句
            codeTable.push_back(new Code(CODE::CAL, level - symbol->level, t));     // 利用a区块构造链表
        }   // 如果是孩子调用祖先，此时祖先地址还未确定，等待回填
        else {
            codeTable.push_back(new Code(CODE::CAL, level - symbol->level, symbol->address));
        }
        syntaxTree.end();
        return true;
    }
    return false;
}

// 〈读语句〉 → read(〈标识符〉{ ，〈标识符〉})
bool Parser::readStatement() {
    logs("LOG[Parser: readStatement]");
    if ("read" == nowSymbol.name) {
        syntaxTree.begin("READSENTENCE");

        /* 语义分析 */
        advance();                                   // 跳过 'read'
        ASSERT("(" == nowSymbol.name, EXCEPTION::MISSING_LBR); // 异常处理: 缺少'('
        do {
            advance();                               // 跳过 '('、','
            ASSERT("identifier" == nowSymbol.name, EXCEPTION::MISSING_IDENTIFIER); // 异常处理: 缺少标识符
            std::string varName = nowSymbol.value;
            Symbol *symbol = symbolTable.getSymbol(varName);
            { ASSERT(nullptr != symbol && "var" == symbol->name, EXCEPTION::NOT_A_VAR); } // 并非变量
            /* 中间代码生成 */
            // 读入一个数，并且存储至指定变量
            codeTable.push_back(new Code(CODE::OPR, OP::READ));                                  // 读一个数到栈顶
            codeTable.push_back(new Code(CODE::STO, level - symbol->level, symbol->address));// 把栈顶送到变量
            /* 语义分析 */
            advance(); // 跳过 标识符
        } while ("," == nowSymbol.name);
        rightBracket(); // 跳过 ')'

        syntaxTree.end();
        return true;
    }
    return false;
}

// Change〈写语句〉 → write(〈表达式〉{，〈表达式〉})
// To <写语句> → WRITE(<标识符>{,<标识符>})
bool Parser::writeStatement() {
    logs("LOG[Parser: writeStatement]");
    if ("write" == nowSymbol.name) {
        syntaxTree.begin("WRITESENTENCE");

        /* 语义分析 */
        advance();                       // 跳过 'write'
        ASSERT("(" == nowSymbol.name, EXCEPTION::MISSING_LBR); // 异常处理: 缺少'('
        do {
            advance(); // 跳过 '('、','

            // 读取一个标识符begin
            ASSERT("identifier" == nowSymbol.name, EXCEPTION::MISSING_IDENTIFIER); // 异常处理: 缺少标识符
            // 从factor()中将产生标识符的逻辑提取至此处
            std::string name = nowSymbol.value;
            advance();// 跳过标识符
            Symbol *symbol = symbolTable.getSymbol(name);
            ASSERT(nullptr != symbol, EXCEPTION::NEVER_DECLARE);  // 异常处理: 未声明
            ASSERT(symbol->name != "procedure", EXCEPTION::ERROR_USING_PROCEDURE);  // 异常处理: 这个是函数！
            if ("const" == symbol->name)
                codeTable.push_back(new Code(CODE::LIT, symbol->number));        // 常量放入栈顶
            else if ("var" == symbol->name)
                codeTable.push_back(new Code(CODE::LOD, level - symbol->level, symbol->address)); // 变量放入栈顶
            // 读取一个标识符end

//            expression();
            /* 中间代码生成 */
            codeTable.push_back(new Code(CODE::OPR, OP::WRITE)); // 栈顶 输出到屏幕
        } while ("," == nowSymbol.name);
        codeTable.push_back(new Code(CODE::OPR, OP::LINE)); // 输出 换行
        /* 语义分析 */
        rightBracket(); // 跳过 ')'

        syntaxTree.end();
        return true;
    }
    return false;
}

// 〈复合语句〉 → begin〈语句〉{ ；〈语句〉}〈end〉
bool Parser::beginStatement() {
    logs("LOG[Parser: beginStatement]");
    if ("begin" == nowSymbol.name) {
        syntaxTree.begin("COMBINED");

        do {
            advance();                    // 跳过 'begin' 或 ';'
            statement();
        } while (";" == nowSymbol.name);
        ASSERT("end" == nowSymbol.name, EXCEPTION::MISSING_END); // 异常处理: 缺少'end'
        advance();                        // 跳过 'end'

        syntaxTree.end();
        return true;
    }
    return false;
}

// 〈分号〉
void Parser::semicolon() {
    logs("LOG[Parser: semicolon]");
    ASSERT(";" == nowSymbol.name, EXCEPTION::MISSING_SEMIC); // 异常处理: 缺少';'
    advance();                          // 跳过 ';'
}

void Parser::rightBracket() {
    logs("LOG[Parser: rightBracket]");
    ASSERT(")" == nowSymbol.name, EXCEPTION::MISSING_RBR); // 异常处理: 缺少')'
    advance();                         // 跳过 ')'
}

// 〈表达式〉 → [+|-]〈项〉{〈加减运算符〉〈项〉}
// 〈加减运算符〉 → +|-
void Parser::expression() {
    logs("LOG[Parser: expression]");
    syntaxTree.begin("EXPRESSION");

    // 第一项可以是带符号数
    std::string op = nowSymbol.name;
    if ("+" == nowSymbol.name || "-" == nowSymbol.name)
        advance();         // 跳过 '+' / '-'
    term();
    if ("-" == op) // 栈顶 取负
        codeTable.push_back(new Code(CODE::OPR, OP::NEG));
    while ("+" == nowSymbol.name || "-" == nowSymbol.name) {
        op = nowSymbol.name;
        advance();             // 跳过 '+' / '-'
        term();
        if ("+" == op) // 栈顶两个元素弹出相加结果进栈
            codeTable.push_back(new Code(CODE::OPR, OP::ADD));
        else                   // 栈顶两个元素弹出，次栈顶减去栈顶结果进栈
            codeTable.push_back(new Code(CODE::OPR, OP::SUB));
    }

    syntaxTree.end();
}

// 〈项〉 → 〈因子〉{〈乘除运算符〉〈因子〉}
// 〈乘除运算符〉 → *|/
void Parser::term() {   // 乘除运算在一项内处理
    logs("LOG[Parser: term]");
    syntaxTree.begin("ITEM");

    factor();
    while ("*" == nowSymbol.name || "/" == nowSymbol.name) {
        std::string op = nowSymbol.name;
        advance(); // 跳过 '*' / '/'
        factor();
        if ("*" == op) // 栈顶两个元素弹出相乘结果进栈
            codeTable.push_back(new Code(CODE::OPR, OP::MUL));
        else                   // 栈顶两个元素弹出，次栈顶除以栈顶结果进栈
            codeTable.push_back(new Code(CODE::OPR, OP::DIV));
    }
    syntaxTree.end();
}

// 〈因子〉 → 〈标识符〉|〈无符号整数〉| (〈表达式〉)
void Parser::factor() {
    logs("LOG[Parser: factor]");
    syntaxTree.begin("FACTOR");

    if ("identifier" == nowSymbol.name) {
        std::string name = nowSymbol.value;
        advance();// 跳过标识符
        Symbol *symbol = symbolTable.getSymbol(name);
        ASSERT(nullptr != symbol, EXCEPTION::NEVER_DECLARE);  // 异常处理: 未声明
        ASSERT(symbol->name != "procedure", EXCEPTION::ERROR_USING_PROCEDURE);  // 异常处理: 这个是函数！
        if ("const" == symbol->name)    // 编译过程中访问符号表获得
            codeTable.push_back(new Code(CODE::LIT, symbol->number));        // 常量放入栈顶
        else if ("var" == symbol->name)
            codeTable.push_back(new Code(CODE::LOD, level - symbol->level, symbol->address)); // 变量放入栈顶
    } else if ("number" == nowSymbol.name) {
        codeTable.push_back(new Code(CODE::LIT, nowSymbol.number));      // 常量放入栈顶
        advance(); // 跳过 '数字'
    } else if ("(" == nowSymbol.name) {
        advance(); // 跳过 '('
        expression();
        rightBracket(); // 跳过 ')'
    }

    syntaxTree.end();
}

// 〈条件〉 → 〈表达式〉〈关系运算符〉〈表达式〉| odd〈表达式〉
// 〈关系运算符〉 → =|#|<|<=|>|>=
void Parser::condition() {
    logs("LOG[Parser: condition]");
    syntaxTree.begin("CONDITION");

    if ("odd" == nowSymbol.name) {
        advance(); // 跳过 'odd'
        expression();
        codeTable.push_back(new Code(CODE::OPR, OP::ODD));
    } else {
        expression();   // 解析表达式
        std::string op = nowSymbol.name; // 保存关系算符
        advance();                     // 跳过关系运算符
        expression();
        // 6种关系运算符:=,#,<,<=,>,>=
        if ("=" == op)
            codeTable.push_back(new Code(CODE::OPR, OP::EQU));
        else if ("#" == op)
            codeTable.push_back(new Code(CODE::OPR, OP::NEQ));
        else if ("<" == op)
            codeTable.push_back(new Code(CODE::OPR, OP::LS));
        else if ("<=" == op)
            codeTable.push_back(new Code(CODE::OPR, OP::LEQ));
        else if (">" == op)
            codeTable.push_back(new Code(CODE::OPR, OP::GT));
        else if (">=" == op)
            codeTable.push_back(new Code(CODE::OPR, OP::GEQ));
    }

    syntaxTree.end();
}

void Parser::printCode() {
    int index = 0;
    for (auto x: codeTable)
#ifdef LOCAL_JUDGE
        std::cout << index++ << '\t' << (*x) << std::endl;
#else
    std::cout << (*x) << std::endl;
#endif
}


