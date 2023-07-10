#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *arg[]) {
    int pid, x;
    int pipe1[2], pipe2[2];

    if (pipe(pipe1) < 0) {
        perror("pipe1 not create");
        exit(EXIT_FAILURE);
    } 
    if (pipe(pipe2) < 0) {
        perror("pipe2 not create");
        exit(EXIT_FAILURE);
    } 

    if ((pid = fork()) < 0) {
        perror("process not create");
        exit(EXIT_FAILURE);
    }
    else {
        if (pid == 0) {     // 子进程
            close(pipe1[1]);
            close(pipe2[0]);

            do {
                read(pipe1[0], &x, sizeof(int));
                printf("child %d read: %d\n", getpid(), x++);        
                write(pipe2[1], &x, sizeof(int));
            } while (x <= 9);

            close(pipe1[0]);
            close(pipe2[1]);
            exit(EXIT_SUCCESS);
        }
        else {  // 父进程
            close(pipe1[0]);
            close(pipe2[1]);
            x = 1;
            do {
                write(pipe1[1], &x, sizeof(int));
                read(pipe2[0], &x, sizeof(int));
                printf("parent %d read: %d\n", getpid(), x++);        
            } while (x <= 9);

            close(pipe1[1]);
            close(pipe2[0]);
        }
    }
    return EXIT_SUCCESS;
}

