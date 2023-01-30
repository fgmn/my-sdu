#include<bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ofstream f;
    f.open("p.in");
    srand(time(0));

    int n = 100, m = n * (n - 1) / 2; // (n - 1), ... , 1, 0
    int d = rand() % 10000;
    f << n << ' ' << m << ' ' << d <<'\n';
    for (int i = 1; i <= n; i++) {
        for (int j = i + 1; j <= n; j++) {
            f << i << ' ' << j << ' ' << rand() % d / 10 << '\n';
        }
    }
    f.close();
    return 0;
}
