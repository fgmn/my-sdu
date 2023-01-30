
#ifndef file_h
#define file_h

#include<list>
#include<string>
using ll = long long;
using namespace std;

struct file
{
    bool isDir;
    string fileName;
    list<file> children;
    ll filesize;

    bool operator < (const file &a) const {
        if (isDir != a.isDir) return a.isDir;
        return fileName < a.fileName;
    }
};

#endif