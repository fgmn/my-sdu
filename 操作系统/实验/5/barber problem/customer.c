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
    else rate = 5;  // 平均客流量

    srand((unsigned)time(NULL));
    struct msqid_ds sofa_info;
    struct msqid_ds room_info;
    Msg_buf msg_arg;

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

    int num = 0;    // 顾客编号
    srand((unsigned)time(NULL));

    while (1) {
        num++;
        int t = rate + (rand() % 5 - 2);    // 实时客流量
        if (t < 0) t = 0;

        // 查看沙发人数
        msgctl(sofa_id, IPC_STAT, &sofa_info);
        if (sofa_info.msg_qnum < MAXSOFA) {
            msg_arg.mtype = FROMSOFA;
            msg_arg.mid = num;
            printf("%d号顾客坐入沙发\n", msg_arg.mid);
            msgsnd(sofa_id, &msg_arg, sizeof(msg_arg), IPC_NOWAIT);
        }
        else {  // 沙发坐满，考虑进入等候室
            // 查看等候室人数
            msgctl(room_id, IPC_STAT, &room_info);
            if (room_info.msg_qnum < MAXROOM) {
                msg_arg.mtype = FROMROOM;
                msg_arg.mid = num;
                printf("沙发座满,%d号顾客进入等候室\n", num);
                // 并发出位置请求
                msgsnd(room_id, &msg_arg, sizeof(msg_arg), IPC_NOWAIT);
            }
            else {
                printf("等候室满，%d号顾客没有进入理发店\n", num);
            }
        }
        sleep(t);    // fix：模拟客流量
    }
    return EXIT_SUCCESS;
}