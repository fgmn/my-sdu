#include<bits/stdc++.h>
#include "skipNode.h"
#include "skipList.h"
#include "skipList.cpp"
#define null NULL
using namespace std;
using ll = long long;


// O(n)
void calXor(skipList<int, int> &a) {
    int Xor = 0;
    auto it = a.begin();
    it++;

    for (; it != a.end(); it++) {
        Xor ^= it->first;
    }

    cout << Xor << '\n';
}

// O(1)
void outMin(skipList<int, int> &a) {
    auto it = a.begin();
    it++;

    cout << it->first << '\n';
    a.erase(it->first);
}

// O(n)
void outMax(skipList<int, int> &a) {
    auto it = a.begin();
    auto itt = it;
    
    while (it != a.end()) {
        itt = it;
        it++;
    }

    cout << itt->first << '\n';
    a.erase(itt->first);
}

void solve() {
    int m, n;
    cin >> m >> n;

    skipList<int, int> obj(INT32_MAX, 200, 0.5);

    // cout << "test for skiplist\n";

    int op, num;
    for (int i = 0; i < n; i++) {
        cin >> num;
        obj.insert({num, 0});
    }

    
    pair<int, int>* ret;
    while (m--) {
        cin >> op;
        switch (op)
        {
            case 1: 
                cin >> num;
                ret = obj.find(num);
                // cout << ret->first << " " << ret->second << "\n";
                if (ret == null) cout << "NO\n";
                else cout << "YES\n";
                break;
            case 2:
                cin >> num;
                obj.insert({num, Val});
                calXor(obj);
                break;
            case 3:
                cin >> num;
                obj.erase(num);
                calXor(obj);
                break;
            case 4:
                outMin(obj);
                break;
            case 5:
                outMax(obj);
                break;
            default:
                break;
        }
    }

}

/* 利用所给数据测试正确性 */
void testForCorrection() {
    const int numOfFiles = 10;
    clock_t timeStart, timeEnd;
    vector<double> timeCost(numOfFiles);


    for (int i = 0; i < numOfFiles; i++) {
        string s1 = "C:\\Users\\DELL\\Desktop\\Coder\\DSA\\1\\input\\input_" + to_string(i) + ".txt";
        string s2 = "C:\\Users\\DELL\\Desktop\\Coder\\DSA\\1\\output\\myoutput_" + to_string(i) +".txt";
        char f1[255];
        char f2[255];
        int len1 = s1.length(); 
        int len2 = s2.length();
        for (int j = 0; j < len1; j++) {
            f1[j] = s1[j];
        }
        for (int j = 0; j < len2; j++) {
            f2[j] = s2[j];
        }
        freopen(f1, "r", stdin);
        freopen(f2, "w", stdout);

        timeStart = clock();

        solve();    

        timeEnd = clock();

        timeCost[i] = (double)(timeEnd - timeStart) / CLOCKS_PER_SEC;  
    }

    freopen("staus.txt", "w", stdout);

    cout << "Test is over. The result is as follows:\n";
    for (int i = 0; i < numOfFiles; i++) {
        cout << "Test point " << i + 1 << " costs " << timeCost[i] << " s\n";
    }
}

/* 随机增删查改测试 */
void testForCapability() {

    freopen("test.txt", "w", stdout);

    int maxCapacity= 200;
    skipList<int, int> obj(INT32_MAX, 200, 0.5);
    double timeStart, timeEnd;
    ll loopSize = maxCapacity * 500000;     // 1e8

    for (int j = 1; j <= 10; j++) {
        timeStart = clock();
        for (int i = 1; i <= loopSize; i++) {
            
            int randomOption = rand() % 3;
            int randomNumber = rand() % 10000;

            switch (randomOption)
            {
            case 0:
                obj.insert({randomNumber, 0});
                break;
            case 1:
                obj.erase(randomNumber);
                break;
            default:
                obj.find(randomNumber);
                break;
            }
        }

        timeEnd = clock();
        cout << "Round " << j << " time cost " << (timeEnd - timeStart) / CLOCKS_PER_SEC * 1000 << " ms\n";       
    }

}

/* 
重构： 将有序序列初始化为理想跳表 
*/

int main() {
    // testForCorrection();
    testForCapability();
    return 0;
}
