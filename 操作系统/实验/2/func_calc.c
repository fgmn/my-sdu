#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

void task1(int *);
void task2(int *);
void task3(int *);

int pipe1[2], pipe2[2];
pthread_t thrd1, thrd2, thrd3;

int main(int argc, char *arg[]) {
    int ret;
    int x, y;

    if (pipe(pipe1) < 0) {
        perror("pipe1 not create");
        exit(EXIT_FAILURE);
    } 
    if (pipe(pipe2) < 0) {
        perror("pipe2 not create");
        exit(EXIT_FAILURE);
    } 

    x = 2, y = 3;
    printf("x=%d,y=%d\n", x, y);
    ret = pthread_create(&thrd1, NULL, (void *)task1, (void *)&x);
    if (ret) {
        perror("pthread_create: task1");
        exit(EXIT_FAILURE);
    }
    ret = pthread_create(&thrd2, NULL, (void *)task2, (void *)&y);
    if (ret) {
        perror("pthread_create: task2");
        exit(EXIT_FAILURE);
    }
    // ret = pthread_create(&thrd3, NULL, (void *)task3, (void *)&x);
    ret = pthread_create(&thrd3, NULL, (void *)task3, NULL);
    if (ret) {
        perror("pthread_create: task3");
        exit(EXIT_FAILURE);
    }
    pthread_join(thrd1, NULL);
    pthread_join(thrd2, NULL);
    pthread_join(thrd3, NULL);
    exit(EXIT_SUCCESS);
}

// f(x)
void task1(int *x) {
    int fx = 1;
    for (int i = 1; i <= *x; i++) fx *= i;
    printf("f(x)=%d\n", fx);
    write(pipe1[1], &fx, sizeof(int));
    close(pipe1[1]);
}

// f(y)
void task2(int *y) {
    double c1 = (1 + sqrt(5)) / 2, c2 = (1 - sqrt(5)) / 2;
    int fy = (pow(c1, *y) - pow(c2, *y)) / sqrt(5); // 直接使用fibonacci通项公式
    printf("f(y)=%d\n", fy);
    write(pipe2[1], &fy, sizeof(int));
    close(pipe2[1]);
}

// f(x, y)
void task3(int *x) {
    int fx, fy;
    read(pipe1[0], &fx, sizeof(int));
    read(pipe2[0], &fy, sizeof(int));
    printf("f(x+y)=%d\n", fx + fy);
    close(pipe1[0]);
    close(pipe2[0]);
}