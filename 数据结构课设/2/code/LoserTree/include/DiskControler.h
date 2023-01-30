
#ifndef DiskControler_
#define DiskControler_

#include<string>
using namespace std;

struct DiskControler
{
    // 默认输入输出缓冲区大小为1
    int numOfDiskAccess;  // 访问次数
    int memorySize;       // 内存大小
    int numOfiles;        // 操作文件数量
    DiskControler():numOfDiskAccess(0), memorySize(0), numOfiles(0) {}
    string Path;
    string fileName;
    string filePath;
};

#endif