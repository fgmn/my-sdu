/*
 *	Filename	:	control.c
 *	copyright	: (C) 2006 by zhonghonglie
 *  Function	: 建立并模拟控制者进程
 */
#include "ipc.h"

// 共享缓冲区
key_t buff_key;
int buff_num;
char *buff_ptr;
int shm_flg;

// 请求
int quest_flg;
key_t quest_key;
int quest_id;

// 回应
int respond_flg;
key_t respond_key;
int respond_id;

int main(int argc, char *argv[]) {
    int i; // 字符串指针
    int rate; // 休眠速率
    int w_mid; // 写者请求
    int count = MAXVAL; // count计数器
    Msg_buf msg_arg; // 消息结构体
    struct msqid_ds msg_inf; // 消息信息

    // 建立一个共享内存先写入一串 A 字符模拟要读写的内容
    buff_key = 101; // 共享内存键值
    buff_num = STRSIZ + 1; // 缓冲区长度
    shm_flg = IPC_CREAT | 0644; // 共享内存读写权限
    buff_ptr = (char *)set_shm(buff_key, buff_num, shm_flg); // 共享内存首地址指针
    for (i = 0; i < STRSIZ; i++)
        buff_ptr[i] = 'A';
    buff_ptr[i] = '\0';

    // 建立一条请求消息队列
    quest_flg = IPC_CREAT | 0644; // 请求消息队列读写权限
    quest_key = 201; // 请求消息队列键值
    quest_id = set_msq(quest_key, quest_flg); // 请求消息队列id

    // 建立一条响应消息队列
    respond_flg = IPC_CREAT | 0644; // 响应消息队列读写权限
    respond_key = 202; // 响应消息队列键值
    respond_id = set_msq(respond_key, respond_flg); // 响应消息队列id

    // 控制进程准备接收和响应读写者的消息
    printf("Wait quest \n");
    while (1) {
        if (count > 0) { // 当 count 大于 0 时说明没有新的读写请求，查询是否有任何新请求
            quest_flg = IPC_NOWAIT; // 以非阻塞方式接收请求消息，当满了就直接退出返回错误
            if (msgrcv(quest_id, &msg_arg, sizeof(msg_arg), FINISHED,
                       quest_flg) >= 0) { // 接收到读者完成的消息请求
                count++; // 有读者完成
                printf("%d reader finished\n", msg_arg.mid); // 输出完成的读者信息
            }
            else if (msgrcv(quest_id, &msg_arg, sizeof(msg_arg), READERQUEST,
                              quest_flg) >= 0) { // 接收到读者请求的消息请求
                count--; // 有读者请求，允许读者读
                msg_arg.mtype = msg_arg.mid;
                msgsnd(respond_id, &msg_arg, sizeof(msg_arg), 0);
                printf("%d quest read\n", msg_arg.mid); // 读请求
            }
            else if (msgrcv(quest_id, &msg_arg, sizeof(msg_arg), WRITERQUEST,
                              quest_flg) >= 0) {  // 接收到写者请求的消息请求
                w_mid = msg_arg.mid; // 有写者请求，记录有写者写
                count -= MAXVAL; // count直接减100

                while (count < 0) { // 如果有读者正在读，则等待所有读者读完
                    // 以阻塞方式接收读完成消息
                    msgrcv(quest_id, &msg_arg, sizeof(msg_arg), FINISHED, 0);
                    count++;
                    printf("%d reader finish\n", msg_arg.mid); // 读完成
                }

                // 允许写者写
                msg_arg.mtype = w_mid;
                msg_arg.mid = w_mid;
                msgsnd(respond_id, &msg_arg, sizeof(msg_arg), 0);
                printf("%d quest write \n", msg_arg.mid); // 写请求
            }
        }

        // 当 count 等于 0 时说明写者正在写，等待写完成
        if (count == 0) {
            // 以阻塞方式接收消息
            msgrcv(quest_id, &msg_arg, sizeof(msg_arg), FINISHED, 0); // 接收到了写完成
            count = MAXVAL;
            printf("%d write finished\n", msg_arg.mid); // 写者写完成

            if (msgrcv(quest_id, &msg_arg, sizeof(msg_arg), READERQUEST,
                       quest_flg) >= 0) { // 有读者请求
                count--; // 允许读者读
                msg_arg.mtype = msg_arg.mid;
                msgsnd(respond_id, &msg_arg, sizeof(msg_arg), 0); // 送读者可读消息
                printf("%d quest read\n", msg_arg.mid);
            }
        }
    }
    return EXIT_SUCCESS;
}
