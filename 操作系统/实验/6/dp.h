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

/*信号量控制用的共同体*/
typedef union semuns {
    int val;
} Sem_uns;

//哲学家的 3 个状态（思考、饥俄、就餐）
enum State { thinking, hungry, eating };

//哲学家管程中使用的信号量
class Sema {
  public:
    Sema(int id);
    ~Sema();
    int down(); //信号量加 1
    int up();   //信号量减 1
  private:
    int sem_id; //信号量标识符
};

//哲学家管程中使用的锁
class Lock {
  public:
    Lock(Sema *lock);
    ~Lock();

    void close_lock();

    void open_lock();

  private:
    Sema *sema; //锁使用的信号量
};

//哲学家管程中使用的条件变量
class Condition {
  public:
    Condition(char *st[], Sema *sm);
    ~Condition();

    void Wait(Lock *lock, int i); //条件变量阻塞操作
    void Signal(int i);           //条件变量唤醒操作
  private:
    Sema *sema;   //哲学家信号量
    char **state; //哲学家当前的状态
};

//哲学家管程的定义
class dp {
  public:
    dp(int rate); //管程构造函数
    ~dp();
    void pickup(int i);  //获取筷子
    void putdown(int i); //放下筷子
    //建立或获取 ipc 信号量的一组函数的原型说明
    int get_ipc_id(char *proc_file, key_t key);
    int set_sem(key_t sem_key, int sem_val, int sem_flag);
    //创建共享内存，放哲学家状态
    char *set_shm(key_t shm_key, int shm_num, int shm_flag);

  private:
    int rate;           //控制执行速度
    Lock *lock;         //控制互斥进入管程的锁
    char *state[5];     // 5 个哲学家当前的状态
    Condition *self[5]; //控制 5 个哲学家状态的条件变量
};