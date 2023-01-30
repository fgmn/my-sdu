#include<bits/stdc++.h>
#include "LoserTree.h"
#include "DiskControler.h"
#include "LoserTree.cpp"
using namespace std;

#define DEBUG 0

#define FAIL_TO_ACCESS_FILE "Fail to access file."
#define MYSEG0 "Segments/mySeg0-"
#define MYSEG1 "Segments/mySeg1-"
#define MYSEG "Segments/mySeg"
#define TXT ".txt"

// 最小输者树

/*
1. 应用LoserTree实现外排序；
2. 实现testForCorrection();
3. 获得外排序访问磁盘次数；
*/

/*
文件比对： 文件数量不定，构造路径的方式不可行
使用CMake编译项目

顺串的生成以及K路归并中涉及磁盘模拟读写，用面向过程方法实现
*/


void test() {
    int a[11] = {0, 2, 7, 1, 3, 9, 8, 10, 6, 4, 5};
    LoserTree<int> tree(a, 10);

    cout << tree.winner() << '\n';
    tree.output();

    tree.replay(9, -1);
    cout << tree.winner() << '\n';
    tree.replay(8, -2);
    cout << tree.winner() << '\n';
    tree.replay(7, -3);
    cout << tree.winner() << '\n';

}

struct Player
{
    int serialNum, val;
    bool operator >= (const Player &a) const {
        if (serialNum == a.serialNum) return val >= a.val;
        return serialNum > a.serialNum;
    }
};

int n;  // 元素总数
int K;  // 归并路数
Player players[200010];
DiskControler mIN, mOUT;    // 磁盘读写控制

void ConstructRuns() {
    // 模拟磁盘，频繁进行文件读写

    ifstream f(mIN.filePath);
    if (!f.is_open()) {
        cout << FAIL_TO_ACCESS_FILE << '\n';
        exit(0);
    }

    f >> n;
    f >> mIN.memorySize;
    f >> K;

    for (int i = 1; i <= mIN.memorySize; i++) {
        f >> players[i].val;
        players[i].serialNum = 1;
    }
    // 连续存储于磁盘同一区域
    mIN.numOfDiskAccess++;

    LoserTree<Player> tree(players, mIN.memorySize);

    int num;
    for (int i = 1; i <= n; i++) {
        if (!(f >> num))
            num = INT_MIN;
        else mIN.numOfDiskAccess++;

        Player mWinner = players[tree.winner()];
        Player mPlayer;
        mPlayer.val = num;

        if (num >= mWinner.val) {
            mPlayer.serialNum = mWinner.serialNum;
        }
        else if (num == INT32_MIN) {
            mPlayer.serialNum = INT32_MAX;
        } 
        else mPlayer.serialNum = mWinner.serialNum + 1;

        tree.replay(tree.winner(), mPlayer);
        
        string seg0File = mOUT.Path + MYSEG0 + to_string(mWinner.serialNum) + TXT;
        mOUT.numOfiles = max(mOUT.numOfiles, mWinner.serialNum);
        ofstream fout(seg0File, ios::app);   // 追加
        fout << mWinner.val << " ";
        fout.close();
        mOUT.numOfDiskAccess++;
    }
}

void KRoadMerge(int mRound) {
    // (i + K - 1) / K
    if (DEBUG) cout << "mIN.numOfiles: " << mIN.numOfiles << '\n';
    for (int i = 1; i <= mIN.numOfiles; i += K) {
        int *FP = new int [K + 1];
        int *pplayers = new int [K + 1];
        
        // 初始化 && 不足K路
        int overCnt = 0;        // 已结束顺串
        for (int j = 1; j <= K; j++) {
            if ((i + j - 1) > mIN.numOfiles) {
                pplayers[j] = INT32_MAX;
                overCnt++;
                continue;
            }
            string seg0File = mIN.Path + MYSEG + to_string(mRound - 1) + "-" + to_string(i + j - 1) + TXT;
            ifstream f(seg0File);
            f >> pplayers[j];
            if (DEBUG) cout << "pplayers " << j << " : " << pplayers[j] << '\n';

            mIN.numOfDiskAccess++;
            FP[j] = f.tellg();
            f.close();
        }
        
        string seg1File = mOUT.Path + MYSEG + to_string(mRound) + "-" + to_string((i + K - 1) / K) + TXT;
        ofstream fout(seg1File);

        // K路归并
        LoserTree<int> tree(pplayers, K);
        if (DEBUG) cout << "tree: \n";
        if (DEBUG) tree.output();

        do {
            int mWinner = pplayers[tree.winner()];
            fout << mWinner << " ";
            mOUT.numOfDiskAccess++;

            int segNum = i + tree.winner() - 1;
            string seg0File = mIN.Path + MYSEG + to_string(mRound - 1) + "-" + to_string(segNum) + TXT;

            ifstream f(seg0File);
            f.clear();

            // 定位至上次将读位置
            int FPPos = FP[tree.winner()] + 1;
            f.seekg(FPPos);

            int num;
            if (FPPos == 0) {
                num = INT_MAX;
                overCnt++;
            }
            else {
                f >> num;
                FP[tree.winner()] = f.tellg();
                if (FP[tree.winner()] + 1 == 0) {     // ?
                    num = INT_MAX;
                    overCnt++;
                }
            }
            mIN.numOfDiskAccess++;
            tree.replay(tree.winner(), num);
            f.close();
        } while (overCnt < K);

        fout.close();
        delete [] FP;
        delete [] pplayers;
    }
}

void testForCorrection(int testPoint) {
    
    mIN.Path = "C:/Users/DELL/Desktop/Coder/DSA/2/data/input/data" + to_string(testPoint) + "/";
    mIN.fileName = "data" + to_string(testPoint) + ".in";
    mIN.filePath = mIN.Path + mIN.fileName;
    mIN.numOfiles = 1;

    mOUT.Path = "C:/Users/DELL/Desktop/Coder/DSA/2/data/output/ans" + to_string(testPoint) + "/";
    mOUT.numOfiles = 0;

    mIN.numOfDiskAccess = mOUT.numOfDiskAccess = 0;

    ifstream f(mIN.filePath);
    if (!f.is_open()) {
        cout << FAIL_TO_ACCESS_FILE << '\n';
        exit(0);
    }
    f >> n;
    f >> mIN.memorySize;
    f >> K;
    f.close();

    ConstructRuns();

    mIN.Path = "C:/Users/DELL/Desktop/Coder/DSA/2/data/output/ans" + to_string(testPoint) + "/";
    mIN.numOfiles = mOUT.numOfiles;

    int mRound = 1;
    do {
        KRoadMerge(mRound);
        mIN.numOfiles = (mIN.numOfiles + K - 1) / K;
        mRound++;
    } while (mIN.numOfiles > 1);
    
}

int main() {

    freopen("status", "w", stdout);

    // test();

    for (int i = 1; i <= 7; i++) {
        testForCorrection(i);
        cout << "Test" << to_string(i) << "\n";
        cout << "Number of disk read: " << mIN.numOfDiskAccess << '\n';
        cout << "Number of disk write: " << mOUT.numOfDiskAccess << '\n';
    }
    
    return 0;
}
