//
// Created by zkr on 2023/5/7.
//

#include "syntaxTree.h"
#include <iostream>


void SyntaxTree::begin(const std::string &name)
{   // 往语法树中添加一个语法单元
    SyntaxTreeNode *syntaxTreeNode = new SyntaxTreeNode(name);
    if (syntaxTreeStack.empty())
        this->root = syntaxTreeNode;    // 若整个语法树为空，则设置当前语法单元为根
    else    // 否则将该节点作为当前栈顶的儿子节点加入
        syntaxTreeStack.back()->children.push_back(syntaxTreeNode);
    syntaxTreeStack.push_back(syntaxTreeNode);
}

void SyntaxTree::end()
{
    if (syntaxTreeStack.back()->children.empty())
    {   // 若该节点没有儿子，则将该节点弹出，打印时将不再显示
        syntaxTreeStack.pop_back();
        syntaxTreeStack.back()->children.pop_back();
    } else
    {
        syntaxTreeStack.pop_back();
    }
}

void SyntaxTree::merge(Symbol *symbol)
{
    syntaxTreeStack.back()->children.push_back(new SyntaxTreeNode(symbol->value));
}

void SyntaxTree::merge(std::string name, bool isLeaf)
{
    syntaxTreeStack.back()->children.push_back(new SyntaxTreeNode(std::move(name), isLeaf));
}
#ifndef LOCAL_JUDGE
# define SUMBIT_VERSION
#endif

void SyntaxTree::print()
{   // 从根开始打印整棵树
#ifdef SUMBIT_VERSION
    std::cout << root->name;
    int n = root->children.size();
    if (!root->children.empty()) std::cout << "(";
    for (auto x : root->children) {
        print(x, "", --n==0);
#ifdef SUMBIT_VERSION
        if (x != root->children.back()) {
            // 这是一个叶子节点且并非最后一个儿子
            std::cout << ",";
        }
#endif
    }
    if (!root->children.empty()) std::cout << ")";
#else
    std::cout << root->name;
    std::cout << '\n';
    int n = root->children.size();
    for (auto x : root->children)
        print(x, "", --n==0);
#endif
}

void SyntaxTree::print(SyntaxTreeNode *now, const std::string &prefix, bool isLast)
{   // 打印当前节点
#ifdef SUMBIT_VERSION
    // 叶子节点不输出括号，输出其本身以及一个分隔“，”(这个逻辑在遍历儿子节点时处理)
    if (now->name == "(") {
        std::cout << "LP";
    }
    else if (now->name == ")") {
        std::cout << "RP";
    }
    else if (now->name == ",") {
        std::cout << "COMMA";
    }
    else {
        // 转换为大写之后输出
        std::string t = now->name;
        std::transform(t.begin(), t.end(), t.begin(), ::toupper);
        std::cout << t;
    }
#else
    std::cout << prefix;
    std::cout << (isLast ? "|__" : "|--");
//    std::cout << "(";
    if (now->name == "(") {
        std::cout << "LP";
    }
    else if (now->name == ")") {
        std::cout << "RP";
    }
    else if (now->name == ",") {
        std::cout << "COMMA";
    }
    else {
        // 转换为大写之后输出
        std::string t = now->name;
        std::transform(t.begin(), t.end(), t.begin(), ::toupper);
        std::cout << t;
    }
    std::cout << '\n';
#endif

#ifdef SUMBIT_VERSION
    if (!now->children.empty()) std::cout << "(";
#endif
    // 打印儿子节点
    int n = now->children.size();
    for (auto x : now->children) {
        print(x, prefix + (isLast ? "   " : "|   "), --n == 0);
#ifdef SUMBIT_VERSION
        if (x != now->children.back()) {
            // 这是一个叶子节点且并非最后一个儿子
            std::cout << ",";
        }
#endif
    }
#ifdef SUMBIT_VERSION
    if (!now->children.empty()) std::cout << ")";
#endif
}
