/*
 * Filename : dp.cc
 * copyright : (C) 2006 by zhonghonglie
 * Function : 哲学家就餐问题的模拟程序
 */

#include "dp.h"
using namespace std;

Sema::Sema(int id) { sem_id = id; }

Sema::~Sema() {}

/*
 * 信号量上的 down/up 操作
 * semid:信号量数组标识符
 * semnum:信号量数组下标
 * buf:操作信号量的结构
 */
int Sema::down() {
    struct sembuf buf;
    buf.sem_op = -1;
    buf.sem_num = 0;
    buf.sem_flg = SEM_UNDO;
    if ((semop(sem_id, &buf, 1)) < 0) {
        perror("down error ");
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}

int Sema::up() {
    Sem_uns arg;
    struct sembuf buf;
    buf.sem_op = 1;
    buf.sem_num = 0;
    buf.sem_flg = SEM_UNDO;
    if ((semop(sem_id, &buf, 1)) < 0) {
        perror("up error ");
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}

/* * 用于railway管程的互斥执行 */
Lock::Lock(Sema *s) { sema = s; }
Lock::~Lock() {}

//上锁
void Lock::close_lock() { sema->down(); }
//开锁
void Lock::open_lock() { sema->up(); }

Condition::Condition(char *st[], Sema *sm) {
    state = st;
    sema = sm;
}

void Condition::Wait(Lock *lock, int i) {
    if (*state[i ^ 1] == waiting) {
        *state[i] = running;
    }
    else {
        lock->open_lock();
        sema->down();
        lock->close_lock();
    }
}

void Condition::Signal(int i) {
    if ((*state[i ^ 1] == waiting) && (*state[i] == waiting)) {
        sema->up();
        *state[i] = running;
    }
}

/*
 * get_ipc_id() 从/proc/sysvipc/文件系统中获取 IPC 的 id 号
 * pfile: 对应/proc/sysvipc/目录中的 IPC 文件分别为
 * msg-消息队列,sem-信号量,shm-共享内存
 * key: 对应要获取的 IPC 的 id 号的键值
 */
int dp::get_ipc_id(char *proc_file, key_t key) {
#define BUFSZ 256

    FILE *pf;
    int i, j;
    char line[BUFSZ], colum[BUFSZ];
    if ((pf = fopen(proc_file, "r")) == NULL) {
        perror("Proc file not open");
        exit(EXIT_FAILURE);
    }

    fgets(line, BUFSZ, pf);
    while (!feof(pf)) {
        i = j = 0;

        fgets(line, BUFSZ, pf);
        while (line[i] == ' ')
            i++;
        while (line[i] != ' ')
            colum[j++] = line[i++];
        colum[j] = '\0';

        if (atoi(colum) != key)
            continue;

        j = 0;
        while (line[i] == ' ')
            i++;
        while (line[i] != ' ')
            colum[j++] = line[i++];
        colum[j] = '\0';

        i = atoi(colum);
        fclose(pf);
        return i;
    }
    fclose(pf);
    return -1;
}

/*
 * set_sem 函数建立一个具有 n 个信号量的信号量
 * 如果建立成功，返回 一个信号量的标识符 sem_id
 * 输入参数：
 * sem_key 信号量的键值
 * sem_val 信号量中信号量的个数
 * sem_flag 信号量的存取权限
 */
int dp::set_sem(key_t sem_key, int sem_val, int sem_flg) {
    int sem_id;
    Sem_uns sem_arg;

    //测试由 sem_key 标识的信号量是否已经建立
    if ((sem_id = get_ipc_id("/proc/sysvipc/sem", sem_key)) < 0) {
        // semget 新建一个信号量,其标号返回到sem_id
        if ((sem_id = semget(sem_key, 1, sem_flg)) < 0) {
            perror("semaphore create error");
            exit(EXIT_FAILURE);
        }
    }

    //设置信号量的初值
    sem_arg.val = sem_val;
    if (semctl(sem_id, 0, SETVAL, sem_arg) < 0) {
        perror("semaphore set error");
        exit(EXIT_FAILURE);
    }
    return sem_id;
}

/*
 * set_shm 函数建立一个具有 n 个字节 的共享内存区
 * 如果建立成功，返回 一个指向该内存区首地址的指针 shm_buf
 * 输入参数：
 * shm_key 共享内存的键值
 * shm_val 共享内存字节的长度
 * shm_flag 共享内存的存取权限
 */
char *dp::set_shm(key_t shm_key, int shm_num, int shm_flg) {

    int i, shm_id;
    char *shm_buf;

    //测试由 shm_key 标识的共享内存区是否已经建立
    if ((shm_id = get_ipc_id("/proc/sysvipc/shm", shm_key)) < 0) {
        // shmget 新建 一个长度为 shm_num 字节的共享内存
        if ((shm_id = shmget(shm_key, shm_num, shm_flg)) < 0) {
            perror("shareMemory set error");
            exit(EXIT_FAILURE);
        }
        // shmat 将由 shm_id 标识的共享内存附加给指针 shm_buf
        if ((shm_buf = (char *)shmat(shm_id, 0, 0)) < (char *)0) {
            perror("get shareMemory error");
            exit(EXIT_FAILURE);
        }
        for (i = 0; i < shm_num; i++)
            shm_buf[i] = 0; //初始为 0
    }
    //共享内存区已经建立,将由 shm_id 标识的共享内存附加给指针 shm_buf
    if ((shm_buf = (char *)shmat(shm_id, 0, 0)) < (char *)0) {
        perror("get shareMemory error");
        exit(EXIT_FAILURE);
    }
    return shm_buf;
}

//railway问题管程构造函数
dp::dp(int r, int m) {
    int ipc_flg = IPC_CREAT | 0644;
    int shm_key = 220;
    int shm_num = 1;
    int sem_key = 120;
    int sem_val = 0;
    int sem_id;
    Sema *sema;

    rate = r;
    max_trains = (int *)set_shm(sem_key++, 1, ipc_flg);
    num = (int *)set_shm(sem_key++, 1, ipc_flg);
    sum2east = (int *)set_shm(sem_key++, 1, ipc_flg);
    sum2west = (int *)set_shm(sem_key++, 1, ipc_flg);
    // init
    *num = 0;
    *sum2east = 0; *sum2west = 0;
    *max_trains = m;

    // 建立一个初值为 1 的用于锁的信号量
    if ((sem_id = set_sem(sem_key++, 1, ipc_flg)) < 0) {
        perror("Semaphor create error");
        exit(EXIT_FAILURE);
    }
    sema = new Sema(sem_id);
    lock = new Lock(sema);

    for (int i = 0; i < 2; i++) {
        //为每个车站建立一个条件变量和可共享的状态 //初始状态都为等待
        if ((state[i] = (char *)set_shm(shm_key++, shm_num, ipc_flg)) == NULL) {
            perror("Share memory create error");
            exit(EXIT_FAILURE);
        }
        *state[i] = waiting;
        //为每个车站建立初值为 0 的用于条件变量的信号量
        if ((sem_id = set_sem(sem_key++, sem_val, ipc_flg)) < 0) {
            perror("Semaphor create error");
            exit(EXIT_FAILURE);
        }
        sema = new Sema(sem_id);
        self[i] = new Condition(state, sema);
    }
}


void dp::depart(int i) {
    lock->close_lock(); //进入管程，上锁

    self[i]->Wait(lock, i); //测试是否发车
    if (!i) {
        *sum2east = *sum2east + 1;
        cout << getpid() << " depart train " << "E" << *num << " \n";
    }
    else {
        *sum2west = *sum2west + 1;
        cout << getpid() << " depart train " << "W" << *num << " \n";
    }
    *num = *num + 1;
    cnt[i]++;
    sleep(rate);       // 模拟行车时间
    lock->open_lock(); //离开管程，开锁
}

void dp::arrive(int i) {
    lock->close_lock(); //进入管程，上锁
    if (!i) {
        cout << getpid() << " train " << "E" << (*num - 1) << " arrive\n";
    }
    else {
        cout << getpid() << " train " << "W" << (*num - 1) << " arrive\n";
    }
    // 达到一次发车上限时停止发车，唤醒对方
    if (cnt[i] + (rand() % 5) >= *max_trains) {
        *state[i] = waiting;
        cnt[i] = 0;
        self[i ^ 1]->Signal(i ^ 1);
        cout << getpid() << " waiting\n";
    }
    // 唤醒对方，共同受益

    lock->open_lock();  //离开管程，开锁

    sleep(rate); //思考 rate 秒
}

dp::~dp() {}

dp *tdp;    //railway管程对象的指针

void sigcat( void )
{
    cout << "|sum2east-sum2west|=" << abs(*(tdp->sum2east) - *(tdp->sum2west)) << '\n';
    exit(0);
}


// railway问题并发执行的入口

int main(int argc, char *argv[]) {
    int pid[2]; // 2个车站进程的进程号
    int rate, max;

    rate = (argc > 1) ? atoi(argv[1]) : 3;
    max = (argc > 2) ? atoi(argv[2]) : 5;

    tdp = new dp(rate, max); //建立一个railway管程对象

    for (int i = 0; i < 2; i++) {
        pid[i] = fork();
        if (pid[i] == 0) {
            if (i) {
                signal(SIGINT, (__sighandler_t)sigcat);
            }
            while (1) {
                tdp->depart(i);
                tdp->arrive(i);
            }   // to be improved：多线程模拟列车行驶
            exit(0);
        }
    }
    for (int i = 0; i < 2; i++) {
        waitpid(pid[i], NULL, 0);
    }
    return 0;
}

