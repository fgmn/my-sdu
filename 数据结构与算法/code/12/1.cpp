#include<bits/stdc++.h>
#include<list>
using namespace std;

//list<int> lt;

class linkedGraph {
	protected:
		int n, e;
		list<int> *aList;
	public:
//		int counter;
		
		linkedGraph(int n) {
			this -> n = n;
			e = 0;
			aList = new list<int> [n + 1];
//			counter = 0;
		}
		
		~linkedGraph() { delete [] aList; }

		int numberOfVertices() const { return n; }	
		int numberOfEdges() const { return e; }	
		bool directed() const { return false; }
		bool weighted() const { return false; }
		
		void insert(int, int);
		void erase(int, int);
		
		int degree(int) const;
		
		void bfs(int v, int vis[], int label);
		void dfs(int v, bool vis[]);
		
		int BFS(int s, int t);
		
		void sort() {
			for (int i = 1; i <= n; i ++) {
				aList[i].sort();
			}
		} 
};

void linkedGraph::insert(int i, int j) {
//	aList[i].insert(aList[i].begin(), j);
//	aList[j].insert(aList[j].begin(), i);
	aList[i].push_front(j);
	aList[j].push_front(i);
	e ++;
}

void linkedGraph::erase(int i, int j) {
	aList[i].remove(j);
	aList[j].remove(i);
	e --;
}

int linkedGraph::degree(int v) const {
	return aList[v].size();
}

void linkedGraph::bfs(int v, int vis[], int label) {
	queue<int> q;
	vis[v] = label;
	q.push(v);
	
	while (!q.empty()) {
		int curNode = q.front();
		q.pop();
		
		if (label == -1) cout << curNode << " ";
		
		for (auto it = aList[curNode].begin(); it != aList[curNode].end(); it ++) {
			if (!vis[*it]) {
				q.push(*it);
				vis[*it] = label;
			}
		}
	}
	
}

void linkedGraph::dfs(int v, bool vis[]) {
	vis[v] = 1;
	cout << v << " "; 
	for (auto it = aList[v].begin(); it != aList[v].end(); it ++) {
		if (!vis[*it]) {
			dfs(*it, vis);
		}
	}
}

int linkedGraph::BFS(int s, int t) {
	int *piles = new int[n + 1];
	bool *visit = new bool[n + 1];
	
	for (int i = 1; i <= n; i ++) {
		piles[i] = 0;
		visit[i] = 0;
	}
	
	queue<int> q;
	
	q.push(s);
	piles[s] = 0;
	visit[s] = 0;
	
	while (!q.empty()) {
		int f = q.front(); q.pop();
		for (auto it = aList[f].begin(); it != aList[f].end(); it ++) {
			if (!visit[*it]) {
				piles[*it] = piles[f] + 1;
				visit[*it] = 1; 
				q.push(*it);
				
				if (*it == t) {
					return piles[*it];
				}
			}
		}
	} 
	return -1;
} 

void solve() {
	int n, m, s, t;
	cin >> n >> m >> s >> t;
	
	linkedGraph G(n);
	
	for (int i = 1, op, u, v; i <= m; i ++) {
		cin >> op >> u >> v;
		
		if (op) {
			G.erase(u, v);
		}
		else {
			G.insert(u, v);
		}
	}
	
	int *c = new int[n + 1];
	for (int i = 1; i <= n; i ++) c[i] = 0;
	
	int label = 0;
	for (int i = 1; i <= n; i++) {
		if (!c[i]) {
			label ++;
			G.bfs(i, c, label);
		}
	}
	
	// 连通分量 
	cout << label << '\n';
	
	// 每个连通子图中最小点编号
	int *over = new int[label + 1];
	for (int i = 1; i <= label; i ++) over[i] = 0;
	
	cout << 1 << " ";
	over[c[1]] ++;
	
	for (int i = 2; i <= n; i++) {
		if (!over[c[i]]) {
			over[c[i]] = 1;
			cout << i << " ";
		}
		else over[c[i]] ++;
	}
	cout << '\n';
	
	// 将每个节点的邻接点按字典序排序 
	G.sort();
	
	
	// 从 s 点开始的 dfs 序列的长度
	cout << over[c[s]] << '\n';
	
	// 从 s 点开始字典序最小的 dfs 序列  
	bool *over_ = new bool[n + 1];
	for (int i = 1; i <= n; i ++) over_[i] = 0;
	G.dfs(s, over_);
	cout << '\n';
	
	cout << over[c[t]] << '\n';
	int *over__ = new int[n + 1];
	for (int i = 1; i <= n; i ++) over__[i] = 0;
	G.bfs(t, over__, -1);
	cout << '\n';
	
	// s 到 t 的最短路径 BFS 
	cout <<  G.BFS(s, t) << '\n';
}


int main(){
	solve();
	return 0;
}

