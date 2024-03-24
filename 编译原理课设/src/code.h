//
// Created by zkr on 2023/5/7.
//

#ifndef PL0_COMPILER23_CODE_H
#define PL0_COMPILER23_CODE_H

#include <ostream>
#include <unordered_map>

namespace CODE
{
    enum CODE_TYPE
    {
        LIT, // 将常数放到运栈顶，a 域为常数
        LOD, // 将变量放到栈顶。a 域为变量在所说明层中的相对位置，l 为调用层与说明层的层差值
        STO, // 将栈顶的内容送到某变量单元中。a 域为变量在所说明层中的相对位置，l 为调用层与说明层的层差值
        CAL, // 调用过程的指令。a 为被调用过程的目标程序的入口地址，l 为层差
        INT, // 为被调用的过程（或主程序）在运行栈中开辟数据区。a 域为开辟的个数
        JMP, // 无条件转移指令，a 为转向地址
        JPC, // 条件转移指令，当栈顶的布尔值为非真时，转向 a 域的地址，否则顺序执行
        OPR  // 关系和算术运算。具体操作由 a 域给出。运算对象为栈顶和次顶的内容进行运算，结果存放在次顶。a 域为 0 时是退出数据区
    };
    const std::unordered_map<int, std::string> CODE_TYPE_MAP({
             {LIT, "LIT"},
             {LOD, "LOD"},
             {STO, "STO"},
             {CAL, "CAL"},
             {INT, "INT"},
             {JMP, "JMP"},
             {JPC, "JPC"},
             {OPR, "OPR"}
    });
    // 通过字符串反向确定操作类型，在Interpreter中使用
    const std::unordered_map<std::string, int> RFIND_MAP({
             {"LIT", LIT},
             {"LOD", LOD},
             {"STO", STO},
             {"CAL", CAL},
             {"INT", INT},
             {"JMP", JMP},
             {"JPC", JPC},
             {"OPR", OPR}
     });
}

namespace OP
{
    enum OP_TYPE
    {
        RET = 0,   // 过程调用结束后, 返回调用点并退栈
        NEG = 1,   // 栈顶元素取反
        ADD = 2,   // 弹出次栈顶与栈顶相加，结果进栈
        SUB = 3,   // 弹出次栈顶减去栈顶，结果进栈
        MUL = 4,   // 弹出次栈顶、栈顶，相乘结果进栈
        DIV = 5,   // 弹出次栈顶除以栈顶，结果值进栈
        LEQ = 6,   // 栈顶两个元素弹出，判次栈顶小于等于栈顶结果进栈
        LS = 7,    // 栈顶两个元素弹出，判次栈顶小于栈顶结果进栈
        EQU = 8,   // 弹出栈顶两元素判相等结果入栈顶
        NEQ = 9,   // 弹出栈顶两元素判不等结果入栈顶
        GT = 10,   // 栈顶两个元素弹出，判次栈顶大于栈顶结果进栈
        GEQ = 11,  // 栈顶两个元素弹出，判次栈顶小于等于栈顶结果进栈
        WRITE = 14,// 栈顶值输出至屏幕
        LINE = 15, // 屏幕输出换行
        READ = 16, // 从命令行读一个数到栈顶
        ODD = 17,  // 弹出栈顶，判断是否为奇数，结果进栈
    };
    const std::unordered_map<int, std::string> OP_TYPE_MAP({
           {RET, "RET"},
           {NEG, "NEG"},
           {ADD, "ADD"},
           {SUB, "SUB"},
           {MUL, "MUL"},
           {DIV, "DIV"},
           {LEQ, "LEQ"},
           {LS, "LS"},
           {EQU, "EQU"},
           {NEQ, "NEQ"},
           {GT, "GT"},
           {GEQ, "GEQ"},
           {WRITE, "WRITE"},
           {LINE, "LINE"},
           {READ, "READ"},
           {ODD, "ODD"}
   });
}


class Code
{
public:
    CODE::CODE_TYPE f;
    int l = 0;
    int a = 0;
    void setL(int _l)
    {
        this->l = _l;
    }

    void setA(int _a)
    {
        this->a = _a;
    }

    explicit Code(CODE::CODE_TYPE _f) : f(_f)
    {}

    Code(CODE::CODE_TYPE _f, int _l, int _a) : f(_f), l(_l), a(_a)
    {}

    Code(CODE::CODE_TYPE _f, int _a) : f(_f), a(_a)
    {}

    friend std::ostream &operator<<(std::ostream &os, const Code &code)
    {
#ifdef LOCAL_JUDGE
        os << CODE::CODE_TYPE_MAP.find(code.f)->second << "\t" << code.l << "\t" << code.a;
        if (CODE::OPR == code.f)
            os << '(' << OP::OP_TYPE_MAP.find(code.a)->second << ')';
#else
        // 生成的目标代码是一种假想栈式计算机的汇编语言，格式为f l a
        os << CODE::CODE_TYPE_MAP.find(code.f)->second << " " << code.l << " " << code.a;
#endif
        return os;
    }

    CODE::CODE_TYPE getF() const
    {
        return f;
    }

    int getL() const
    {
        return l;
    }

    int getA() const
    {
        return a;
    }
};


#endif //PL0_COMPILER23_CODE_H
