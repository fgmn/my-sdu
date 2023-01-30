#include<bits/stdc++.h>
using namespace std;
using ll = long long;

bool judge(string f1, string f2) {
    ifstream fs1(f1);
    ifstream fs2(f2);

    string A, B;
    while (fs1 >> A && fs2 >> B) {
        if (A != B) return false;
    }
    if (A != B) return false;
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int numOfFiles = 10;
    vector<bool> staus(numOfFiles);

    for (int i = 0; i < numOfFiles; i++) {
        string f1 = "C:\\Users\\DELL\\Desktop\\Coder\\DSA\\1\\output\\output_" + to_string(i) +".txt";
        string f2 = "C:\\Users\\DELL\\Desktop\\Coder\\DSA\\1\\output\\myoutput_" + to_string(i) +".txt";
        
        staus[i] = judge(f1, f2);    
    }

    cout << "The result is as follows:\n";
    for (int i = 0; i < numOfFiles; i++) {
        cout << "Test point " << i + 1 << " : " << (staus[i] ? "Accept" : "Wrong answer") << '\n';
    }
    
    return 0;
}
