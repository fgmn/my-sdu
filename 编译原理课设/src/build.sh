#! /bin/bash
g++ -c lexer.cpp -o lexer.o -O2
g++ -c utils.cpp -o utils.o -O2
g++ -c syntaxTree.cpp -o syntaxTree.o -O2
g++ -c parser.cpp -o parser.o -O2
g++ -c symbolTable.cpp -o symbolTable.o -O2
g++ -c vm.cpp -o vm.o -O2

# Compiler
g++ lexer.o utils.o syntaxTree.o parser.o symbolTable.o vm.o main.cpp -o Compiler -lm

# Interpreter
g++ lexer.o utils.o syntaxTree.o parser.o symbolTable.o vm.o interpreter.cpp -o Interpreter -lm
