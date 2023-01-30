#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <signal.h>
#include <stdlib.h>

// 缺省执行命令
char *args1[] = {"/bin/ls", "-a", NULL};
char *args2[] = {"/bin/ps", "-l", NULL};

void sigcat()
{
  printf("%d Process continue\n", getpid());
}

void work() {
    int status, t = 0;
    // 每隔3s嵌套创建两个子进程
    TAG: int pid1 = fork();
    if (pid1 < 0) {
        printf("Create Process fail!\n");
        exit(EXIT_FAILURE);
    }
    else if (pid1 == 0) {
        printf("child <1> %d\n", getpid());
        // 睡眠，期望通过第二个子进程唤醒
        pause();
        // 首先创建的子进程执行ls命令
        for (int i = 0; args1[i] != NULL; i++)
            printf("%s ", args1[i]);
        printf("\n");
        execve(args1[0], args1, NULL);
    }
    else {
        printf("parent poccess <2>\n");
        // 创建第二个子进程
        int pid2 = fork();
        if (pid2 < 0) {
            printf("Create Process fail!\n");
            exit(EXIT_FAILURE);
        }
        else if (pid2 == 0) {
            printf("child <2> %d\n", getpid());
            // 之后创建的子进程执行ps命令
            for (int i = 0; args2[i] != NULL; i++)
                printf("%s ", args2[i]);
            printf("\n");
            execve(args2[0], args2, NULL);
            // err！执行execve后不再继续执行
            // exec用新程序覆盖了原进程的地址空间，不会再返回到调用进程
        }
        else {
            printf("parent poccess <3>\n");
            sleep(5);   // 等待子进程1进入阻塞态
            waitpid(pid2, &status, 0);
            if (kill(pid1, SIGINT) >= 0)
                printf("%d Wakeup %d child.\n", getpid(), pid1);            
            waitpid(pid1, &status, 0);
            sleep(3);
            if (t < 5) { printf("\n\n\n"); t++; goto TAG; }
            // 父进程保留，2个子进程结束
        }
    }
}

int main() {
    // fork
    // int pid = fork();
    // if (pid < 0) {
    //     printf("fork failed\n");
    // }
    // else if (pid == 0) {
    //     printf("child poccess\n");
    // }
    // else {
    //     wait(NULL); // block parent poccess until child's is over
    //     printf("parent poccess\n");
    // }

    // exec
    // char *argv[] = {"ls", "-al", "/etc/passwd", NULL};
    // execvp("ls", argv);



    // 注册中断信号
    signal(SIGINT, (__sighandler_t)sigcat);
    perror("SIGINT");

    work();

    return 0;
}





