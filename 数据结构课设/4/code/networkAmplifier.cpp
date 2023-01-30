#include<iostream>
#include<vector>
#include<cstring>
#include<fstream>
#include<algorithm>
#include<queue>

#include<time.h>
#include <windows.h>
#include <psapi.h>
#pragma comment(lib, "psapi.lib")

using namespace std;
using ll = long long;

const int N = 100005;

struct edge
{
    int v, w;
    edge(int v, int w):v(v), w(w) {}
};

ifstream f1;
ofstream f2;

int n, m, d, ans;
vector<edge> g[N], g1[N];

vector<int> t;          // 存储拓扑序
bool vis[N];            // vis记录节点是否访问
bool vis1[N];           // vis1记录答案，是否放置网络放大器
int toLeaf[N];          // i到后代节点压力的最大衰减值
int in_deg[N];          // 节点入度，用于拓扑排序
int fa[N];              // 节点的最长入边，方便递推计算
int p[N];               // 节点压力值

// dfs回溯
// O(m)
// 递归深度：O(n)  图存储：O(m)
// 总空间复杂度为 O(m + n)
void dfs(int u) {
    vis[u] = 1;
    for (auto i : g[u]) {
        // cout << u << "->" << i.v << '\n';
        if (!vis[i.v]) dfs(i.v);
        toLeaf[u] = max(toLeaf[u], toLeaf[i.v] + i.w);
    }
    // -> 决策点u是否放置发大器
    if (fa[u] + toLeaf[u] > d) {
        vis1[u] = 1;
        ans++;
        toLeaf[u] = 0;
    }
}

void tsort() {
    queue<int> q;
    for (int i = 1; i <= n; i++) {
        if (!in_deg[i]) q.push(i);
    }
    while (!q.empty()) {
        int u = q.front();
        q.pop();

        t.push_back(u);
        
        for (auto i : g[u]) {
            if (--in_deg[i.v] == 0) {
                q.push(i.v);
            }
        }
    }
}

// 拓扑逆序进行递推
// O(所有节点的入度之和) = O(m)
// 空间： O(m + n)
void inferInTopo() {
    tsort();
    reverse(t.begin(), t.end());

    for (int i = 0; i < n; i++) {
        int u = t[i];
        if (toLeaf[u] + fa[u] > d) {
            ans++;
            toLeaf[u] = 0;
        }
        for (auto j : g1[u]) {
            toLeaf[j.v] = max(toLeaf[j.v], toLeaf[u] + j.w);
        }
    }
}

bool cmp(edge a, edge b) {
    return a.v > b.v;
}

void bfs() {
    memset(vis, 0, sizeof vis);
    queue<int> q;
    q.push(1);
    vis[1] = 1;
    p[1] = d;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        vis[u] = 0;

        if (vis1[u]) p[u] = d;
        for (auto i : g[u]) {
            p[i.v] = min(p[i.v], p[u] - i.w);
            if (!vis[i.v]) {
                q.push(i.v);
                vis[i.v] = 1;
            }
        }
    }
}

void solve(int op) {
    f1 >> n >> m >> d;

    int u, v, w;
    for (int i = 1; i <= m; i++) {
        f1 >> u >> v >> w;
        if (w > d) continue;
        g[u].push_back(edge(v, w));
        g1[v].push_back(edge(u, w));
        fa[v] = max(fa[v], w);
        in_deg[v]++;
    }

    if (!op) dfs(1);
    else inferInTopo();
    bfs();
    // f2 << ans << '\n';
}

void init() {
    for (int i = 0; i <= n; i++) g[i].clear(), g1[i].clear();
    memset(p, 0x3f3f3f3f, sizeof p);
    memset(fa, 0, sizeof fa);
    memset(vis, 0, sizeof vis);
    memset(vis1, 0, sizeof vis1);
    memset(toLeaf, 0, sizeof toLeaf);
    memset(in_deg, 0, sizeof in_deg);
    t.clear();
    ans = 0;
}

void readStandardAns() {
    for (int i = 1; i <= 100; i++) {
        string s1 = "C:\\Users\\DELL\\Desktop\\Coder\\DSA\\4\\Test\\output\\" + to_string(i) + ".out";
        f1.open(s1);
        int ans;
        f1 >> ans;
        f2 << ans << '\n';
        f1.close();   
    }
}

// 输出成dot文件，可以通过graphviz渲染
void viResult(int &n, vector<edge> g[], bool *vis, int *toLeaf, string &path) {
    ofstream f;
    f.open(path);
    if (!f.is_open()) { exit(-1); }

    f << "digraph example {\n";
    // 绘制有向图
    for (int i = 1; i <= n; i++) {
        for (auto j : g[i]) {
            f << i << "->" << j.v << "[label=" << j.w << "]\n";
        }
    }
    // 展示答案
    for (int i = 1; i <= n; i++) {
        f << i << "[label=\"(" << i << ", " << toLeaf[i] << ")\"";
        if (vis[i]) {
            f << ",style=filled,color=aquamarine";
        }
        f << "]\n";
    }
    f << "}\n";
    f.close();
}

void C() {
    string s2 = "C:\\Users\\DELL\\Desktop\\Coder\\DSA\\4\\Test\\output\\my.out";
    f2.open(s2);
    for (int i = 1; i <= 100; i++) {
        string s1 = "C:\\Users\\DELL\\Desktop\\Coder\\DSA\\4\\Test\\input\\input" + to_string(i) + ".in";
        string path = "C:\\Users\\DELL\\Desktop\\Coder\\DSA\\4\\Test\\output\\" + to_string(i) + ".dot";
        f1.open(s1);

        init();
        solve(1);
        viResult(n, g, vis1, p, path);
        f1.close(); 
    }
    // chk();
}

/*
Main() {
    f.open(path);    // 文件流读入/输出

    init();          // 变量初始化
    solve();         // 两种算法分别执行
        dfs();       // 记忆化搜索，回溯时进行求解
        topo();      // 对节点进行拓扑排序，按照拓扑序进行递推
    bfs();           // 根据算法结果进行图遍历，计算节点最终的压力值
    visualResult();  // 可视化计算结果

    f.close();       // 文件流关闭
}
*/

void P(int op) {
    f1.open("p.in");
    f2.open("p.out", ios::app);

    double s, t;

    if (op) {
        s = clock();
        init();
        solve(op);
        t = clock();
        f2 << "ans: " << ans << '\n';
        f2 << "method topo: " << (t - s) / CLOCKS_PER_SEC * 1000 << " ms\n";
    }
    else {
        s = clock();
        init();
        solve(op);
        t = clock();
        f2 << "ans: " << ans << '\n';
        f2 << "method dfs: " << (t - s) / CLOCKS_PER_SEC * 1000 << " ms\n";
    }



    // f1.seekg(ios::beg);

    HANDLE handle = GetCurrentProcess();
    PROCESS_MEMORY_COUNTERS pmc;
    GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));
    f2 << "memory usage: " << pmc.WorkingSetSize << " b\n";

    f1.close();
    f2.close();
}

int main() {
    // C();
    P(1);
    return 0;
}
