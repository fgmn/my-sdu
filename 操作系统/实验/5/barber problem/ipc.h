/*
*  Filename     :   ipc.h
*  copyright    : (C) 2022 by
*  Function     : 声明 IPC 机制的函数原型和全局变量
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>  // rand(), srand()
#include <time.h>    // time()
#include <sys/wait.h>

#define BUFSZ 256     // 缓冲区大小
#define MAXVAL 100    // count的最大值
#define STRSIZ 8      // 字符串大小
#define WRITERQUEST 1 // 写请求标识
#define READERQUEST 2 // 读请求标识
#define FINISHED 3    // 读写完成标识

#define FROMSOFA 11
#define FROMROOM 12

#define MAXSOFA 4
#define MAXROOM 13

/*信号量控制用的共同体*/
typedef union semuns {
    int val;
} Sem_uns;

/* 消 息 结 构 体 */
typedef struct msgbuf {
    long mtype;
    int mid;
} Msg_buf;



int get_ipc_id(char *proc_file, key_t key);

char *set_shm(key_t shm_key, int shm_num, int shm_flag);
int set_msq(key_t msq_key, int msq_flag);
int set_sem(key_t sem_key, int sem_val, int sem_flag);
int down(int sem_id);
int up(int sem_id);