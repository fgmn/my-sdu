#include "ipc.h"

// 生产消费者共享缓冲区即其有关的变量
key_t buff_key;
int buff_num;
char *buff_ptr;

// 生产者放产品位置的共享指针
key_t pput_key;
int pput_num;
int *pput_ptr;

// 消费者取产品位置的共享指针
key_t cget_key;
int cget_num;
int *cget_ptr;

// 生产者有关的信号量
key_t prod_key;
key_t pmtx_key;
int prod_sem;
int pmtx_sem;

// 消费者有关的信号量
// key_t cons_key;
key_t cmtx_key;

key_t sm1_key;
key_t sm2_key;
key_t sm3_key;

// int cons_sem;
int cmtx_sem;

key_t sm1_sem;
key_t sm2_sem;
key_t sm3_sem;

int sem_val;
int sem_flg;
int shm_flg;

/*
保留生产者的同步和互斥信号
设置tobacco_paper(A), tobacco_matches(B), paper_matches(C)三个消费者同步信号
初始化桌子最多放两种材料，供给一次消费，因此消费者之间不存在并发 (-> 最多供给8次消费，存在并发)
产品具有多个种类 -> 无法顺次生产和消费
*/

int main(int argc, char *argv[])
{
	int rate;
	// 可在在命令行第一参数指定一个进程睡眠秒数,以调解进程执行速度
	if(argv[1] != NULL) 
		rate = atoi(argv[1]);
	else 
		rate = 3; // 不指定为 3 秒

	// 共享内存使用的变量
	buff_key = 101; // 缓冲区任给的键值
	buff_num = 1; // 缓冲区任给的长度
	pput_key = 102; // 生产者放产品指针的键值
	pput_num = 1; // 指针数
	shm_flg = IPC_CREAT | 0644; // 共享内存读写权限
	// 获取缓冲区使用的共享内存,buff_ptr 指向缓冲区首地址
	buff_ptr = (char*)set_shm(buff_key, buff_num, shm_flg);
	// 获取生产者放产品位置指针 pput_ptr
	pput_ptr = (int*)set_shm(pput_key, pput_num, shm_flg);
	// 信号量使用的变量
	prod_key = 201; // 生产者同步信号灯键值
	pmtx_key = 202; // 生产者互斥信号灯键值
	// cons_key = 301; // 消费者同步信号灯键值
	sm1_key = 401;
	sm2_key = 402;
	sm3_key = 403;

	cmtx_key = 302; // 消费者互斥信号灯键值
	sem_flg = IPC_CREAT | 0644;
	// 生产者同步信号灯初值设为缓冲区最大可用量
	sem_val = buff_num;
	// 获取生产者同步信号灯,引用标识存 prod_sem
	prod_sem = set_sem(prod_key,sem_val,sem_flg);
	// 消费者初始无产品可取,同步信号灯初值设为0
	sem_val = 0;
	// 获取消费者同步信号灯,引用标识存 cons_sem
	// cons_sem = set_sem(cons_key,sem_val,sem_flg);
	sm1_sem = set_sem(sm1_key,sem_val,sem_flg);
	sm2_sem = set_sem(sm2_key,sem_val,sem_flg);
	sm3_sem = set_sem(sm3_key,sem_val,sem_flg);
	// 生产者互斥信号灯初值为 1
	sem_val = 1;
	// 获取生产者互斥信号灯,引用标识存 pmtx_sem
	pmtx_sem = set_sem(pmtx_key,sem_val,sem_flg);
	// 循环执行模拟生产者不断放产品

	/*while(1)
	{
		// 如果缓冲区满则生产者阻塞
		down(prod_sem);		// wait
		// 如果另一生产者正在放产品,本生产者阻塞
		down(pmtx_sem);		// wait
		// 用写一字符的形式模拟生产者放产品,报告本进程号和放入的字符及存放的位置
		buff_ptr[*pput_ptr] = 'A' + *pput_ptr;
		sleep(rate);
		printf("%d producer put: %c to Buffer[%d]\n",getpid(),buff_ptr[*pput_ptr],*pput_ptr);
		// 存放位置循环下移
		*pput_ptr = (*pput_ptr+1) % buff_num;	// 共享内存
		// 唤醒阻塞的生产者
		up(pmtx_sem);		// signal
		// 唤醒阻塞的消费者
		up(cons_sem);		// signal
	}*/


	srand((unsigned int)time(0));
	while (1) {				
		down(prod_sem);
		down(pmtx_sem);
		int ra = rand() % 3;

		sleep(rate);
		printf("%d producer put: %d to Buffer\n", getpid(), ra);
		up(pmtx_sem);
		if (ra == 0) up(sm1_sem);
		else if (ra == 1) up(sm2_sem);
		else if (ra == 2) up(sm3_sem);
	}
	return EXIT_SUCCESS;
}
