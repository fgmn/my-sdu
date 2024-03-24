cmake_minimum_required(VERSION 3.24)
project(PL0_Compiler23)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
        src/lexer.cpp
        src/lexer.h
        src/symbol.h
        src/utils.cpp
        src/utils.h
        src/symbolTable.h
        src/parser.h
        src/syntaxTree.cpp
        src/code.h
        src/parser.cpp
        src/exceptionHandler.h
        src/symbolTable.cpp
        src/vm.h
        src/vm.cpp)

add_definitions(-DLOCAL_JUDGE) # 本地调试宏

add_executable(PL0_Compiler23 ${SOURCE_FILES} src/main.cpp)

#add_executable(PL0_Compiler23 ${SOURCE_FILES} src/interpreter.cpp)