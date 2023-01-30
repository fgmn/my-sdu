
#include<sstream>
#include "file.h"
#include "catalogTree.h"
using namespace std;

catalogTree::catalogTree() {
    root = new file;
    root->isDir = true;
    root->fileName = "";
    filePtr.push_back(root);
}

// 字典序，先输出文件后输出目录
// 有序维护
void catalogTree::dir() {
    file *p = filePtr.back();
    for (auto i : p->children) {
        cout << i.fileName;
        if (i.isDir == false) cout << '*';
        cout << '\n';
    }
}

// 列出当前目录的绝对路径
void catalogTree::cd() {

    if (filePtr.size() == 1) cout << '/';

    bool c = false;
    for (auto i : filePtr) {
        if (c) cout << '/';
        else {
            c = true;
            continue;   // 跳过root
        }
        cout << i->fileName;
    }

    cout << '\n';
}

void catalogTree::cd(ofstream &f) {

    if (filePtr.size() == 1) f << '/';

    bool c = false;
    for (auto i : filePtr) {
        if (c) f << '/';
        else {
            c = true;
            continue;   // 跳过root
        }
        f << i->fileName;
    }

    f << '\n';
}

// 返回父目录
void catalogTree::cd_() {
    filePtr.pop_back();
    // 可能已经返回根目录
    if (filePtr.empty()) filePtr.push_back(root);
}

// 跳转至str目录下
void catalogTree::cd_str(string &path) {
    if (path.find('/') == string::npos) {
        file *p = filePtr.back();
        for (auto &i : p->children) {
            if (path == i.fileName) {
                filePtr.push_back(&i);
                break;
            }
        }
    }
    else {
        findPath(parsePath(path));
    }
}

// 创建子目录
void catalogTree::mkdir(string &dirName) {
    file *p = filePtr.back();
    file *newDir = new file;
    newDir->isDir = true;
    newDir->fileName = dirName;

    // 插入至指定位置
    bool flag = false;
    for (auto i = p->children.begin(); i != p->children.end(); i++) {
        if (*newDir < *i) {
            p->children.insert(i, *newDir);
            flag = true;
            break;
        }
    }
    if (!flag) p->children.push_back(*newDir);
}

// 创建文件项
void catalogTree::mkfile(string &fileName) {
    file *p = filePtr.back();
    file *newDir = new file;
    newDir->isDir = false;
    newDir->fileName = fileName;

    // 插入至指定位置
    bool flag = false;
    for (auto i = p->children.begin(); i != p->children.end(); i++) {
        if (*newDir < *i) {
            p->children.insert(i, *newDir);
            flag = true;
            break;
        }
    }
    if (!flag) p->children.push_back(*newDir);
}

void catalogTree::delet(string &path) {
    file *p = filePtr.back();
    // if (!p->isDir) return;
    // 基于迭代器进行操作
    for (auto i = p->children.begin(); i != p->children.end(); i++) {
        if (path == i->fileName) {
            // 找到要删除文件
            p->children.erase(i);
            break;      // 立即返回否则丢失
        }
    }
}

vector<string> catalogTree::parsePath(string path) {
    stringstream ss(path);
    string fileName;
    vector<string> res;

    while (getline(ss, fileName, '/')) {
        res.push_back(fileName);
    }
    return res;
}

void catalogTree::findPath(vector<string> fileNames) {
    filePtr.clear();

    // 重新解析当前路径
    filePtr.push_back(root);
    for (int i = 1; i < fileNames.size(); i++) {
        file *p = filePtr.back();
        for (auto &j : p->children) {       // 引用，否则将形参地址加入filePtr
            if (j.fileName == fileNames[i]) {
                filePtr.push_back(&j);
                break;
            }
        }
    }
}

void catalogTree::dfs(ofstream &f, file *fp) {
    f << fp->fileName << ' ' << fp->isDir << ' ';
    if (fp->isDir) {
        f << "{ ";
        for (auto &i : fp->children) {
            dfs(f, &i);
        }
        f << "} ";
    }
}

// 存储文件名、文件属性及节点父子关系
// 存储文件指针
void catalogTree::save(string &path) {
    ofstream fout(path);
    cd(fout);
    dfs(fout, root);

    // 使用栈进行递归遍历
    /*
    stack<file *> stk;
    stk.push(root);

    while (!stk.empty()) {
        auto u = stk.top();
        stk.pop();

        fout << u->fileName << ' ' << u->isDir << ' ';

        if (u->isDir == true) {
            fout << "{ ";
            for (auto &i : u->children) {
                stk.push(&i);
            }
            fout << "} ";
        }

    }
    */
    
    fout.close();
}

// 重建树
void catalogTree::load(string &path) {
    ifstream fin(path);

    if (!fin.is_open()) {
        cout << "fail to open!\n";
        exit(0);
    } 

    string p;
    fin >> p;

    filePtr.push_back(root);
    string c, fn;
    bool isD;
    fin >> isD;
    fin >> c;       // 根目录特殊处理，直接将第一个'{'读出
    while (fin >> c) {
        if (c == "{") {     // 切换为子目录
            cd_str(fn);
        }
        else if (c == "}") {
            cd_();
        }
        else {
            fn = c;
            fin >> isD;
            if (isD) {
                mkdir(fn);
            }
            else {
                mkfile(fn);
            }
        }
    }

    // 还原之前保存路径
    cd_str(p);

    fin.close();
}

