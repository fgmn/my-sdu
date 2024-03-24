#ifndef PCB_H
#define PCB_H

#include "addrspace.h"
#include "machine.h"
#include "filesys.h"
#define MaxFileId 20

class Pcb
{
public:
    int userRegisters[NumTotalRegs]; // user-level CPU register state 用户级CPU寄存器状态
    // 将地址空间移动到此处
    AddrSpace *space; // User code this thread is running. 此线程正在运行的用户代码。
    // Joiner
    int waitingProcessSpaceId;

    int waitProcessExitCode;

    Pcb();

    ~Pcb();

    void setExitStatus(int ExitStatus);

    int getExitStatus();

    int addFile(OpenFile *openfile);

    OpenFile *getFile(int fileId);

    void releaseFile(int fileId);

private:
    int exitStatus; // 退出码

    bool fileIdUse[MaxFileId];

    OpenFile *files[MaxFileId];
};

#endif // PCB_H