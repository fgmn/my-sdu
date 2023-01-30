
#ifndef catalogTree_h
#define catalogTree_h

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include "file.h"
using namespace std;

class catalogTree {
    public:
        catalogTree();
        ~catalogTree() { delete root; }

        void dir();
        void cd();
        void cd(ofstream &);
        void cd_();
        void cd_str(string &);
        void mkdir(string &);
        void mkfile(string &);
        void delet(string &);
        void save(string &);
        void load(string &);
        void quit() { exit(0); }

    private:
        file *root;                 // 根目录文件
        list<file *> filePtr;     // 保存当前路径
        vector<string> parsePath(string);
        void findPath(vector<string>);
        void dfs(ofstream &, file *);
};

#endif