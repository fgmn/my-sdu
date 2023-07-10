#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

#define CLOSE "\001\033[0m\002"                 // 关闭所有属性
#define BLOD  "\001\033[1m\002"                 // 强调、加粗、高亮
#define BEGIN(x,y) "\001\033["#x";"#y"m\002"    // x: 背景，y: 前景

// https://blog.csdn.net/xuancbm/article/details/81436681

int main(void)
{
    while (1)
    {
        char *str = readline(BEGIN(49, 34)"COMMAND-> "CLOSE);
        add_history(str);
        free(str);
    }
}

