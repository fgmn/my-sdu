#include<iostream>
#include<map>
#include<string>
#include<vector>
#include<sstream>
#include<List>
#include<fstream>
#include<cstring>
#include<stack>
#include "file.h"
#include "catalogTree.h"
#include "catalogTree.cpp"
#define D 0
using namespace std;
using ll = long long;

// 将目录视为具有后代的文件对象
// list存储后代
// 当前目录绝对路径用list维护
// 保存目录树结构

void run() {
    catalogTree tree;

    string s;

    // cin.sync();
    
    if (D)
    {string q;
    cin >> q;
    cout << q;
    cin >> q;
    cout << q;}
    cin.ignore();
    while (getline(cin, s, '\n')) {
        if (D) cout << s << '\n';
        string cmd, str;
        stringstream ss(s);
        ss >> cmd;
        ss >> str;

        if (cmd == "dir") {
            tree.dir();
        }
        else if (cmd == "cd") {
            if (str == "") {
                tree.cd();
            }
            else if (str == "..") {
                tree.cd_();
            }
            else {
                tree.cd_str(str);
            }
        }
        else if (cmd == "mkdir") {
            tree.mkdir(str);
        }
        else if (cmd == "mkfile") {
            tree.mkfile(str);
        }
        else if (cmd == "delete") {
            tree.delet(str);
        }
        else if (cmd == "save") {
            tree.save(str);
        }
        else if (cmd == "load") {
            tree.load(str);
        }
        else if (cmd == "quit") {
            // tree.quit();
            // cout << endl;
            return;
        }
    }
}



int main() {
    // to be fixed
    for (int i = 0; i < 10; i++) {
        // int i = 1;
        char f1[255];
        char f2[255];

        string s1 = "C:\\Users\\DELL\\Desktop\\Coder\\DSA\\3\\data\\output\\myout" + to_string(i) + ".txt";
        string s2 = "C:\\Users\\DELL\\Desktop\\Coder\\DSA\\3\\data\\input\\in" + to_string(i) + ".txt";

        int len1 = s1.length();
        int len2 = s2.length();
        for (int j = 0; j < len1; j++) f1[j] = s1[j];
        for (int j = 0; j < len2; j++) f2[j] = s2[j];
        freopen(f2, "r", stdin);
        freopen(f1, "w", stdout);
        // cout << i << '\n';
        run();
    }

    return 0;
}
