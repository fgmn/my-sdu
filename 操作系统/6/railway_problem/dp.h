/*
 * Filename : dp.h
 * copyright : (C) 2022
 * Function : 声明 IPC 机制的函数原型和哲学家管程类
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <math.h>

/*信号量控制用的共同体*/
typedef union semuns {
    int val;
} Sem_uns;

enum State { waiting, running };

//railway管程中使用的信号量
class Sema {
  public:
    Sema(int id);
    ~Sema();
    int down(); //信号量加 1
    int up();   //信号量减 1
  private:
    int sem_id; //信号量标识符
};

//railway管程中使用的锁
class Lock {
  public:
    Lock(Sema *lock);
    ~Lock();

    void close_lock();

    void open_lock();

  private:
    Sema *sema; //锁使用的信号量
};

//railway管程中使用的条件变量
class Condition {
  public:
    Condition(char *st[], Sema *sm);
    ~Condition();

    void Wait(Lock *lock, int i); //条件变量阻塞操作
    void Signal(int i);           //条件变量唤醒操作
  private:
    Sema *sema;   //railway信号量
    char **state; //railway当前的状态
};

//railway管程的定义
class dp {
  public:
    dp(int rate, int max); //管程构造函数
    ~dp();

    int *max_trains;    // 一次最大同向行车数量
    int *num;   // 车次
    int *sum2east;
    int *sum2west;  /* 共享变量 */ 
    // 通过|sum2east-sum2west|<c体现负载均衡

    void depart(int i);   // 发车
    void arrive(int i);   // 到站

    //建立或获取 ipc 信号量的一组函数的原型说明
    int get_ipc_id(char *proc_file, key_t key);
    int set_sem(key_t sem_key, int sem_val, int sem_flag);
    //创建共享内存，放车站状态
    char *set_shm(key_t shm_key, int shm_num, int shm_flag);

  private:
    int rate;           //车速
    Lock *lock;         //控制互斥进入管程的锁
    char *state[2];     // 2个车站当前的状态
    int cnt[2];     // 一次同向发车计数器
    Condition *self[2]; //控制2个车站状态的条件变量
};