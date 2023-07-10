
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <math.h>
#include <string.h>

#include <readline/readline.h>
#include <readline/history.h>

#define MAX_LINE 100
#define MAX_AGRNUM 10
#define MAX_FILENAME 35
#define MAX_PIPELEN 10

#define CLOSE "\001\033[0m\002"                 // 关闭所有属性
#define BLOD  "\001\033[1m\002"                 // 强调、加粗、高亮
#define BEGIN(x,y) "\001\033["#x";"#y"m\002"    // x: 背景，y: 前景

/*
实验1创建子进程执行命令的基础上，
重定向输入输出至文件或者管道

保存最近30个命令，通过queue维护，方向按键进行交互

int creat (const char *__file, mode_t __mode)

*/

/*
配置使用readline库

命令解析架构：
1. 根据是否包含 {&, |, <, >} 等字符确定是否包含特定功能;
2. 使用空格进行语义分割；

重定向
> >> 2> &>
< << 
（目前仅支持>,<）

多级管道
只有第一、最后一级可以进行输入、输出重定向（暂不支持）

自定义信号处理？需要在终端开始运行程序
*/

// 从特定位置开始查看命令是否包含特定字符，并返回所在位置，否则返回-1
int get_char(char cmd[], int n, char c, int st);    // st = 0:C++缺省行参

void dfs(int u);   // 执行多级管道命令
int pip[MAX_PIPELEN][2];  // 管道读写标识符
int cnt_pip = 0;    // 管道级数
char *prog[MAX_PIPELEN] = { NULL };

void sigcat( void )
{
    printf("%d get SIGQUIT\n", getpid());
    exit(0);
}

int main(void) {

    // 自定义信号处理Start
    signal(SIGINT, SIG_IGN);    // 父进程忽略SIGINT
    perror("SIGINT");
    signal(SIGQUIT, (__sighandler_t)sigcat);    // 自定义父进程退出信号
    // 自定义信号处理End

    int bg;     // 命令是否放在后台执行
    while (1) {
        bg = 0;
        cnt_pip = 0;

        // 调用readline接口实现命令获取、历史保存
        char *cmd = readline(BEGIN(49, 34)"COMMAND-> "CLOSE);
        add_history(cmd);
        int n = strlen(cmd);
        printf("%s (%d)\n", cmd, n);


        // 参数区Start
        int fd;
        char outfile[MAX_FILENAME];      // 重定向输出文件
        char inputfile[MAX_FILENAME];    // 重定向输入文件（静态分配）

        char *arg[MAX_AGRNUM] = { NULL };   // 动态分配空间
        int cnt_arg = 0;
        

        int sym, blk;    // 记录功能符号位置
        int re_obj;      // 重定向对象
        int vis[MAX_LINE];  // 标记命令已解析部分
        memset(vis, 0, sizeof vis);
        // 参数区End

        // 参数解析Start
        // >
        sym = get_char(cmd, n, '>', 0);
        blk = get_char(cmd, n, ' ', sym);
        if (blk < 0) blk = n;
        blk--;
        if (sym > 0) {
            strncpy(outfile, cmd + sym + 1, blk - sym); // [sym+1,blk]
            for (int i = sym; i <= blk; i++) vis[i] = 1;
            outfile[blk - sym] = '\0';
            printf("redirect output to %s\n", outfile);            
        }

        // <
        sym = get_char(cmd, n, '<', 0);
        blk = get_char(cmd, n, ' ', sym);
        if (blk < 0) blk = n;
        blk--;
        if (sym > 0) {
            strncpy(inputfile, cmd + sym + 1, blk - sym);
            for (int i = sym; i <= blk; i++) vis[i] = 1;
            inputfile[blk - sym] = '\0';
            printf("redirect input to %s\n", inputfile);            
        }

        // |
        int t = 0;  // 查找起点
        while (1) {
            sym = get_char(cmd, n, '|', t);
            if (sym < 0) break;
            prog[cnt_pip] = (char *)malloc(sizeof(char) * MAX_FILENAME);
            memset(prog[cnt_pip], 0, sizeof prog[cnt_pip]);
            strncpy(prog[cnt_pip], cmd + t, sym - t - 1);   // [t,sym-1]
            // printf("t:%d, sym:%d\n", t, sym);
            for (int i = t; i <= sym; i++) vis[i] = 1;
            t = sym + 1; cnt_pip++;
        }
        // printf("pipe: ");
        // for (int i = 0; i < cnt_pip; i++) {
        //     printf("%s ", prog[i]);
        // } printf("(%d)\n", cnt_pip);
        // 参数解析End


        if (get_char(cmd, n, '&', 0) == n - 1) { bg = 1; vis[n - 1] = 1; }
        if (fork() == 0) {  // 子进程

            signal(SIGINT, SIG_DFL);    // 恢复默认方式

            if (get_char(cmd, n, '>', 0) >= 0) {
                // for (int i = sym; i < blk; i++) vis[i] = 1;     // 标记已解析
                // err: vis[]不被父子进程所共享！
                // sol:1.提前解析
                // 2.写入文件再读出来

                // todo: >> 2> &>

                fd = creat(outfile, S_IRWXU);
                close(1);
                dup(fd);
                close(fd);
            }   /* 输出重定向 */

            if (get_char(cmd, n, '<', 0) >= 0) {
                fd = open(inputfile, O_RDONLY);
                if (fd < 0) {
                    fprintf(stderr, "can't find the file!\n");
                    exit(1);
                }
                close(0);
                dup(fd);
                close(fd);
            }   /* 输入重定向 */


            /* 是否有形如 c1|c2|...|cn 的管道 */
            if (cnt_pip) {
                dfs(cnt_pip - 1);
            }   /* 管道 */
        
            // 解析命令剩余部分，根据空格进行语义分割
            int arg_cnt = 0;    // 参数个数
            for (int i = 0; i < n; i++) {
                if (vis[i] | (cmd[i] == ' ')) continue;

                arg[arg_cnt] = (char *)malloc(sizeof(char) * 100);
                blk = get_char(cmd, n, ' ', i);
                if (blk < 0) blk = n;
                // printf("i:%d blk: %d\n", i, blk);

                strncpy(arg[arg_cnt], cmd + i, blk - i);   // 参数范围：[i, blk-1]
                arg[arg_cnt][blk - i] = '\0';
                i = blk; arg_cnt++;
            }
            arg[arg_cnt] = NULL;

            // printf("arg: ");
            // for (int i = 0; arg[i] != NULL; i++) {
            //     printf("%s ", arg[i]);
            // }   printf("(%d)\n", arg_cnt);

            execvp(arg[0], arg);
            // for (int i = 0; i < arg_cnt; i++) free(arg[i]);            
        }

        if (bg == 0) {
            int status;
            wait(&status);
        }
        free(cmd);  // readline为cmd分配动态内存
        for (int i = 0; i < cnt_arg; i++) { free(prog[i]); }
    }   /* while(1) */
}


int get_char(char cmd[], int n, char c, int st) {
    for (int i = st; i < n; i++) {
        if (cmd[i] == c) { return i; }
    }
    return -1;
}


void dfs(int u) {
    pipe(pip[u]);  // 建立无名管道pip

    if (fork() == 0) {
        // 标准输出至管道（架设管道）
        close(1);
        dup(pip[u][1]);
        close(pip[u][1]);
        close(pip[u][0]);

        if (u) dfs(u - 1);
        else {
            // fprintf(stderr, "%s output to pip[%d]\n", prog[u], u);
            execvp(prog[0], NULL);
        }
    }
    // 标准输入来自管道
    close(0);
    dup(pip[u][0]);
    close(pip[u][0]);
    close(pip[u][1]);

    if (cnt_pip - 1 != u) {
        // fprintf(stderr, "%s input from pip[%d]\n", prog[u + 1], u);
        execvp(prog[u + 1], NULL);
    }
}



/* 测试
1. 带参数命令
/bin/ls -l
/bin/ls -a

2. 重定向
/bin/echo nihao >hi.txt
/bin/cat hi.txt
./read_output <hi.txt
./read_output <./hi.txt >./file.txt

sqr <x.txt
./sqr <x.txt
./sqr <./x.txt

3. 管道
./sqr |add
(5*5)+5=30
sqr |add |multi
(5*5+5)*3=90
(1*1+5)*3=18
./sqr |./add |./multi

4.信号处理以及后台命令
while_1
while_1 &
/bin/ps
/bin/kill pid
*/



