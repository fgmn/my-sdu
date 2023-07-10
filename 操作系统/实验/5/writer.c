/*
 *	Filename	:	writer.c
 *	copyright	: (C) 2006 by zhonghonglie
 * Function	: 建立并模拟写者进程
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
    int i, j = 0; // i写入指针，j控制写入内容的自增量
    int rate;
    Msg_buf msg_arg;

    //可在在命令行第一参数指定一个进程睡眠秒数，以调解进程执行速度
    if (argv[1] != NULL)
        rate = atoi(argv[1]);
    else
        rate = 3;

    //附加一个要读内容的共享内存
    buff_key = 101; // 先前被注释掉了
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

    //循环请求写
    msg_arg.mid = getpid();
    
    while (1) {
        msg_arg.mtype = WRITERQUEST; // 发写请求消息
        msgsnd(quest_id, &msg_arg, sizeof(msg_arg), 0);
        printf("%d writer quest\n", msg_arg.mid);

        // 等待允许写消息
        msgrcv(respond_id, &msg_arg, sizeof(msg_arg), msg_arg.mid, 0);
        
        // 写入 STRSIZ 个相同的字符
        for (i = 0; i < STRSIZ; i++)
            buff_ptr[i] = 'A' + j;
        j = (j + 1) % STRSIZ; //按 STRSIZ 循环变换每轮写入的字符

        printf("%d writing: %s\n", msg_arg.mid, buff_ptr);
        sleep(rate);

        // 发写完成消息
        msg_arg.mtype = FINISHED;
        msgsnd(quest_id, &msg_arg, sizeof(msg_arg), 0);
    }
    return EXIT_SUCCESS;
}