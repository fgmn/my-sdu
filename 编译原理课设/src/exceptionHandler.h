//
// Created by zkr on 2023/5/7.
//

#ifndef PL0_COMPILER23_EXCEPTIONHANDLER_H
#define PL0_COMPILER23_EXCEPTIONHANDLER_H

#include <iostream>
#include <unordered_map>

namespace EXCEPTION {
    enum ExceptionEnum{
        EXTRA_CHARACTERS,
        MISSING_SEMICOLON,
        MISSING_IDENTIFIER,
        DUPLICATE_IDENTIFIER,
        MISSING_EQUAL,
        MISSING_CEQUAL,
        MISSING_NUMBER,
        MISSING_THEN,
        MISSING_DO,
        MISSING_LBR,
        MISSING_RBR,
        MISSING_END,
        MISSING_SEMIC,
        NOT_A_VAR,
        NOT_A_PROCUDURE,
        NEVER_DECLARE,
        TOO_MUCH_NESTING,
        CALL_INDIRECTLY_CONTAIN,
        ERROR_USING_PROCEDURE
    };
    std::unordered_map<ExceptionEnum, std::string> EXCEPTION_ENUM_MAP({
        {EXTRA_CHARACTERS, "There is extra char after '.' !"},
        {EXTRA_CHARACTERS, "Here is missing a semicolon !"},
        {MISSING_SEMICOLON, "Here is missing a identifier !"},
        {MISSING_IDENTIFIER, "MISSING_IDENTIFIER !"},
        {DUPLICATE_IDENTIFIER, "DUPLICATE_IDENTIFIER"},
        {MISSING_EQUAL, "MISSING_EQUAL"},
        {MISSING_CEQUAL, "MISSING_CEQUAL"},
        {MISSING_NUMBER, "MISSING_NUMBER"},
        {MISSING_THEN, "MISSING_THEN"},
        {MISSING_DO, "MISSING_DO"},
        {MISSING_LBR, "MISSING_LBR"},
        {MISSING_RBR, "MISSING_RBR"},
        {MISSING_END, "MISSING_END"},
        {MISSING_SEMIC, "MISSING_SEMIC"},
        {NOT_A_VAR, "NOT_A_VAR"},
        {NOT_A_PROCUDURE, "NOT_A_PROCUDURE"},
        {NEVER_DECLARE, "NEVER_DECLARE"},
        {TOO_MUCH_NESTING, "TOO_MUCH_NESTING"},
        {CALL_INDIRECTLY_CONTAIN, "CALL_INDIRECTLY_CONTAIN,"},
        {ERROR_USING_PROCEDURE, "ERROR_USING_PROCEDURE"}
    });
};

void ASSERT(bool _, EXCEPTION::ExceptionEnum parserExceptionEnum)
{ // 更详细的异常信息、更方便的异常处理
    if (_) return;
    // 发生语法错误
#ifdef LOCAL_JUDGE
    std::cout << "-------------------" << EXCEPTION::EXCEPTION_ENUM_MAP[parserExceptionEnum] << std::endl;
#endif
//    std::cout << "Syntax Error\n";
//    exit(0);
    exit(1);
}

#endif //PL0_COMPILER23_EXCEPTIONHANDLER_H
