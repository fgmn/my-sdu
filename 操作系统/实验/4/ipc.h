#ifndef _IPC_H
#define _IPC_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <time.h>
#define BUFSZ 256

// 建立或获取 ipc 的一组函数的原型说明
int get_ipc_id(char *proc_file,key_t key);
char *set_shm(key_t shm_key,int shm_num,int shm_flag);
int set_msq(key_t msq_key,int msq_flag);
int set_sem(key_t sem_key,int sem_val,int sem_flag);
int down(int sem_id);
int up(int sem_id);

/*信号灯控制用的共同体*/
typedef union semuns 
{
	int val;
} Sem_uns;

/* 消息结构体 */
typedef struct msgbuf 
{
	long mtype;
	char mtext[1];
} Msg_buf;



#endif