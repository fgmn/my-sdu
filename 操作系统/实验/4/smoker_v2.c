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

int type;

int main(int argc,char *argv[])
{
	int rate;
	// 可在在命令行第一参数指定一个进程睡眠秒数,以调解进程执行速度
	if(argv[1] != NULL) 
		rate = atoi(argv[1]);
	else 
		rate = 3; // 不指定为 3 秒
	// 增加参数指定吸烟者类型
	type = atoi(argv[2]);

	// 共享内存 使用的变量
	buff_key = 101; // 缓冲区任给的键值
	buff_num = 8; // 缓冲区任给的长度
	cget_key = 103; // 消费者取产品指针的键值
	cget_num = 1; // 指针数
	shm_flg = IPC_CREAT | 0644; //共享内存读写权限
	// 获取缓冲区使用的共享内存,buff_ptr 指向缓冲区首地址
	buff_ptr = (int *)set_shm(buff_key,buff_num,shm_flg);
	// 获取消费者取产品指针,cget_ptr 指向索引地址
	cget_ptr = (int *)set_shm(cget_key,cget_num,shm_flg);
	// 信号量使用的变量
	prod_key = 201; // 生产者同步信号灯键值
	pmtx_key = 202; // 生产者互斥信号灯键值
	// cons_key = 301; // 消费者同步信号灯键值
	sm1_key = 401;
	sm2_key = 402;
	sm3_key = 403;

	cmtx_key = 302; // 消费者互斥信号灯键值
	sem_flg = IPC_CREAT | 0644; // 信号灯操作权限
	// 生产者同步信号灯初值设为缓冲区最大可用量
	sem_val = buff_num;
	// 获取生产者同步信号灯,引用标识存 prod_sem
	prod_sem = set_sem(prod_key,sem_val,sem_flg);
	// 消费者初始无产品可取,同步信号灯初值设为 0
	sem_val = 0;
	// 获取消费者同步信号灯,引用标识存 cons_sem
	// cons_sem = set_sem(cons_key,sem_val,sem_flg);
	sm1_sem = set_sem(sm1_key,sem_val,sem_flg);
	sm2_sem = set_sem(sm2_key,sem_val,sem_flg);
	sm3_sem = set_sem(sm3_key,sem_val,sem_flg);

	// 消费者互斥信号灯初值为 1
	sem_val = 1;
	// 获取消费者互斥信号灯,引用标识存 pmtx_sem
	cmtx_sem = set_sem(cmtx_key,sem_val,sem_flg);
	// 循环执行模拟消费者不断取产品
	/*while(1)
	{
		// 如果无产品消费者阻塞
		down(cons_sem);
		// 如果另一消费者正在取产品,本消费者阻塞
		down(cmtx_sem);
		// 用读一字符的形式模拟消费者取产品,报告本进程号和获取的字符及读取的位置
		sleep(rate);
		printf("%d consumer get: %c from Buffer[%d]\n", getpid(), 
		buff_ptr[*cget_ptr], *cget_ptr);
		// 读取位置循环下移
		*cget_ptr = (*cget_ptr+1) % buff_num;
		// 唤醒阻塞的消费者
		up(cmtx_sem);
		// 唤醒阻塞的生产者
		up(prod_sem);
	}*/

	while (1) {
		if (type == 1) {
			down(sm1_sem);
			while (buff_ptr[*cget_ptr] != 1) {
				*cget_ptr = (*cget_ptr + 1) % buff_num;
			}
			buff_ptr[*cget_ptr] = 0;
			sleep(rate);
			printf("%d consumer get: 1 from Buffer[%d]\n", getpid(), *cget_ptr);
			*cget_ptr = (*cget_ptr + 1) % buff_num;
		}
		else if (type == 2) {
			down(sm2_sem);
			while (buff_ptr[*cget_ptr] != 2) {
				*cget_ptr = (*cget_ptr + 1) % buff_num;
			}
			buff_ptr[*cget_ptr] = 0;
			sleep(rate);
			printf("%d consumer get: 2 from Buffer[%d]\n", getpid(), *cget_ptr);
			*cget_ptr = (*cget_ptr + 1) % buff_num;
		}
		else if (type == 3) {
			down(sm3_sem);
			while (buff_ptr[*cget_ptr] != 3) {
				*cget_ptr = (*cget_ptr + 1) % buff_num;
			}
			buff_ptr[*cget_ptr] = 0;
			sleep(rate);
			printf("%d consumer get: 3 from Buffer[%d]\n", getpid(), *cget_ptr);
			*cget_ptr = (*cget_ptr + 1) % buff_num;
		}
		up(prod_sem);
	}

	return EXIT_SUCCESS;
}
