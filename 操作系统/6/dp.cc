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

/* * 用于哲学家管程的互斥执行 */
Lock::Lock(Sema *s) { sema = s; }
Lock::~Lock() {}

//上锁
void Lock::close_lock() { sema->down(); }
//开锁
void Lock::open_lock() { sema->up(); }

//用于哲学家就餐问题的条件变量
Condition::Condition(char *st[], Sema *sm) {
    state = st;
    sema = sm;
}

/*
 * 左右邻居不在就餐，条件成立，状态变为就餐
 * 否则睡眠，等待条件成立
 */
void Condition::Wait(Lock *lock, int i) {
    if ((*state[(i + 4) % 5] != eating) && (*state[i] == hungry) &&
        (*state[(i + 1) % 5] != eating)) {  // 通过同时拿起左右两只筷子的方式避免死锁
        *state[i] = eating; //拿到筷子，进就餐态
    } else {
        cout << "p" << i + 1 << ":" << getpid() << " hungry\n";
        lock->open_lock();  //开锁
        sema->down();       //没拿到，以饥饿态等待 -> 等待邻居signal唤醒自己
        lock->close_lock(); //上锁
    }
}

/*
 *左右邻居不在就餐，则置其状态为就餐，
 *将其从饥俄中唤醒。否则什么也不作。
 */
void Condition::Signal(int i) {
    if ((*state[(i + 4) % 5] != eating) && (*state[i] == hungry) &&
        (*state[(i + 1) % 5] != eating)) {
        //可拿到筷子，从饥饿态唤醒进就餐态
        sema->up();
        *state[i] = eating;
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

//哲学家就餐问题管程构造函数
dp::dp(int r) {
    int ipc_flg = IPC_CREAT | 0644;
    int shm_key = 220;
    int shm_num = 1;
    int sem_key = 120;
    int sem_val = 0;
    int sem_id;
    Sema *sema;

    rate = r;

    //为防止两个相邻的哲学家同时就餐，导致共有同一只筷子，允许5 个中只有 1 个
    // 在就餐，建立一个初值为 1 的用于锁的信号量
    if ((sem_id = set_sem(sem_key++, 1, ipc_flg)) < 0) {
        perror("Semaphor create error");
        exit(EXIT_FAILURE);
    }
    sema = new Sema(sem_id);
    lock = new Lock(sema);

    for (int i = 0; i < 5; i++) {
        //为每个哲学家建立一个条件变量和可共享的状态 //初始状态都为思考
        if ((state[i] = (char *)set_shm(shm_key++, shm_num, ipc_flg)) == NULL) {
            perror("Share memory create error");
            exit(EXIT_FAILURE);
        }
        *state[i] = thinking;
        //为每个哲学家建立初值为 0 的用于条件变量的信号量
        if ((sem_id = set_sem(sem_key++, sem_val, ipc_flg)) < 0) {
            perror("Semaphor create error");
            exit(EXIT_FAILURE);
        }
        sema = new Sema(sem_id);
        self[i] = new Condition(state, sema);
    }
}

//获取筷子的操作
//如果左右邻居都在就餐，则以饥俄状态阻塞
//否则可以进入就餐状态
void dp::pickup(int i) {
    lock->close_lock(); //进入管程，上锁

    *state[i] = hungry; //进饥饿态

    self[i]->Wait(lock, i); //测试是否能拿到两只筷子
    cout << "p" << i + 1 << ":" << getpid() << " eating\n";
    sleep(rate);       //拿到，吃 rate 秒
    lock->open_lock(); //离开管程，开锁
}

//放下筷子的操作
//状态改变为思考，如左右邻居有阻塞者则唤醒它
void dp::putdown(int i) {
    int j;
    lock->close_lock(); //进入管程，上锁

    *state[i] = thinking; //进思考态

    j = (i + 4) % 5;
    self[j]->Signal(j); //唤醒左邻居
    j = (i + 1) % 5;
    self[j]->Signal(j); //唤醒右邻居
    // 此时邻居处于饥饿，唤醒对方，共同受益

    lock->open_lock();  //离开管程，开锁

    cout << "p" << i + 1 << ":" << getpid() << " thinking\n";
    sleep(rate); //思考 rate 秒
}

dp::~dp() {}

// 哲学家就餐问题并发执行的入口

int main(int argc, char *argv[]) {
    dp *tdp;    //哲学家就餐管程对象的指针
    int pid[5]; // 5 个哲学家进程的进程号
    int rate;

    rate = (argc > 1) ? atoi(argv[1]) : 3;

    tdp = new dp(rate); //建立一个哲学家就餐的管程对象

    pid[0] = fork(); //建立第一个哲学家进程
    if (pid[0] < 0) {
        perror("p1 create error");
        exit(EXIT_FAILURE);
    } else if (pid[0] == 0) { //利用管程模拟第一个哲学家就餐的过程
        while (1) {
            tdp->pickup(0);  //拿起筷子
            tdp->putdown(0); //放下筷子
        }
    }

    pid[1] = fork(); //建立第二个哲学家进程
    if (pid[1] < 0) {
        perror("p2 create error");
        exit(EXIT_FAILURE);
    } else if (pid[1] == 0) { //利用管程模拟第二个哲学家就餐的过程
        while (1) {
            tdp->pickup(1);  //拿起筷子
            tdp->putdown(1); //放下筷子
        }
    }

    pid[2] = fork(); //建立第三个哲学家进程
    if (pid[2] < 0) {
        perror("p3 create error");
        exit(EXIT_FAILURE);
    } else if (pid[2] == 0) { //利用管程模拟第三个哲学家就餐的过程
        while (1) {
            tdp->pickup(2);  //拿起筷子
            tdp->putdown(2); //放下筷子
        }
    }

    pid[3] = fork(); //建立第四个哲学家进程
    if (pid[3] < 0) {
        perror("p4 create error");
        exit(EXIT_FAILURE);
    } else if (pid[3] == 0) { //利用管程模拟第四个哲学家就餐的过程
        while (1) {
            tdp->pickup(3);  //拿起筷子
            tdp->putdown(3); //放下筷子
        }
    }

    pid[4] = fork(); //建立第五个哲学家进程
    if (pid[4] < 0) {
        perror("p5 create error");
        exit(EXIT_FAILURE);
    } else if (pid[4] == 0) { //利用管程模拟第五个哲学家就餐的过程
        while (1) {
            tdp->pickup(4);  //拿起筷子
            tdp->putdown(4); //放下筷子
        }
    }
    return 0;
}

