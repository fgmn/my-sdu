/*
 *	Filename	:	reader.c
 *	copyright	: (C) 2006 by zhonghonglie
 *  Function	: 建立并模拟读者进程
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
    int i;
    int rate;
    Msg_buf msg_arg;

    //可在在命令行第一参数指定一个进程睡眠秒数，以调解进程执行速度
    if (argv[1] != NULL)
        rate = atoi(argv[1]);
    else
        rate = 3;
        
    //附加一个要读内容的共享内存
    buff_key = 101;
    buff_num = STRSIZ + 1;
    shm_flg = IPC_CREAT | 0644;
    buff_ptr = (char *)set_shm(buff_key, buff_num, shm_flg);

    //联系一个请求消息队列
    quest_flg = IPC_CREAT | 0644;
    quest_key = 201;
    quest_id = set_msq(quest_key, quest_flg);

    //联系一个响应消息队列
    respond_flg = IPC_CREAT | 0644;
    respond_key = 202;
    respond_id = set_msq(respond_key, respond_flg);

    //循环请求读
    msg_arg.mid = getpid();

    while (1) {
        msg_arg.mtype = READERQUEST; // 发读请求消息
        msgsnd(quest_id, &msg_arg, sizeof(msg_arg), 0);
        printf("%d reader quest\n", msg_arg.mid);

        // 等待接受允许读消息
        msgrcv(respond_id, &msg_arg, sizeof(msg_arg), msg_arg.mid, 0);
        printf("%d reading: %s\n", msg_arg.mid, buff_ptr);
        sleep(rate);

        // 发读完成消息
        msg_arg.mtype = FINISHED;
        msgsnd(quest_id, &msg_arg, sizeof(msg_arg), quest_flg);
    }
    return EXIT_SUCCESS;
}