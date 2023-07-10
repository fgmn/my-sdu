#include "ipc.h"

// 记账本互斥信号量
key_t acc_key;
int acc_val;
int acc_sem;

int sem_flg;

// 沙发消息队列
key_t sofa_key;
int sofa_id;

// 等候室消息队列
key_t room_key;
int room_id;

int q_flg;

int main(int argc, char *argv[]) {
    int rate;
    if (argv[1] != NULL) {
        rate = atoi(argv[1]);
    }
    else rate = 5;

    struct msqid_ds sofa_info;
    struct msqid_ds room_info;
    Msg_buf msg_arg;
    Msg_buf msg_arg1;

    // 设置记账本互斥信号量
    sem_flg = IPC_CREAT | 0644;
    acc_key = 100;
    acc_val = 1;
    acc_sem = set_sem(acc_key, acc_val, sem_flg);

    // 设置沙发消息队列，存储对理发服务的请求
    q_flg = IPC_CREAT | 0644;
    sofa_key = 200;
    sofa_id = set_msq(sofa_key, q_flg);

    // 设置等候室消息队列，存储对沙发位置的请求
    q_flg = IPC_CREAT | 0644;
    room_key = 201;
    room_id = set_msq(room_key, q_flg);

    int pid[3];
    for (int i = 0; i < 3; i++) {
        pid[i] = fork();
        if (pid[i] == 0) {  // 子进程为理发师进程
            while (1) {
                // 查看服务请求
                msgctl(sofa_id, IPC_STAT, &sofa_info);
                if (sofa_info.msg_qnum == 0) {
                    printf("%d号理发师睡眠\n", getpid());
                }
                // 以阻塞方式从沙发消息队列接受服务请求
                msgrcv(sofa_id, &msg_arg, sizeof(msg_arg), FROMSOFA, 0);

                // 并以非阻塞方式接受来自等候室对沙发位置的请求
                if (msgrcv(room_id, &msg_arg1, sizeof(msg_arg1), FROMROOM, IPC_NOWAIT) >= 0) {
                    msg_arg1.mtype = FROMSOFA;
                    printf("%d号顾客坐入沙发\n", msg_arg1.mid);
                    // 并发出服务请求
                    msgsnd(sofa_id, &msg_arg1, sizeof(msg_arg1), IPC_NOWAIT);
                }

                printf("%d理发师开始给%d号顾客理发\n", getpid(), msg_arg.mid);
                sleep(rate);    // 模拟理发过程
                printf("%d号理发师给%d号顾客理发完成\n", getpid(), msg_arg.mid);

                // 使用记账本结算
                down(acc_sem);
                printf("%d号理发师和%d号顾客使用记账本结算\n", getpid(), msg_arg.mid);
                up(acc_sem);
            }
        }
    }
    return EXIT_SUCCESS;
}