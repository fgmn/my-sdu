//
// Created by zkr on 2023/5/7.
//

#ifndef PL0_COMPILER23_SYNTAXTREE_H
#define PL0_COMPILER23_SYNTAXTREE_H

#include <list>
#include <utility>
#include "symbol.h"

class SyntaxTreeNode
{
public:
    std::string name;
    bool isLeaf;
    std::list<SyntaxTreeNode*> children;
    explicit SyntaxTreeNode(std::string name, bool isLeaf = 0): name(std::move(name)), isLeaf(isLeaf) {}
};

class SyntaxTree
{
    std::list<SyntaxTreeNode*> syntaxTreeStack;  // 语法树 栈
    SyntaxTreeNode* root;                        // 语法树 根
public:
    void merge(Symbol* symbol);
    void merge(std::string name,  bool isLeaf = 0); // 增加isLeaf标识叶子节点
    void begin(const std::string &name);
    void end();
    void print();
    void print(SyntaxTreeNode* now, const std::string& prefix, bool isLast);
};

#endif //PL0_COMPILER23_SYNTAXTREE_H
