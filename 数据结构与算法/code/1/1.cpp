#include<bits/stdc++.h>
using namespace std;

int ans = -1, A[25];

// pos ��λ���Ӽ��е�λ�� cur �� ��ǰ������a���е�λ�� v �� ��ǰ�ۻ���ֵ n �� a���г��� 
void G_subset(int pos, int cur, int v, int n){
	if(cur == n){
		if(ans == -1) ans = v;
		else ans ^= v;
//		cout << ans << '\n';
		ans ^= v + pos * A[cur];
//		cout << ans << '\n';
	}
	else {
		G_subset(pos, cur + 1, v, n);
		G_subset(pos + 1, cur + 1, v + pos * A[cur], n);
	}
} 

int main(){
	
	int n; cin >> n;
		
	for(int i = 1; i <= n; i++) {
		cin >> A[i];
	}
	
	// ������ģ�������Ӽ�
//	for (int i = 0, res, cnt; i <= (1<<n) - 1; i++){
//		res = 0, cnt = 1;
//		for (int j = 1; j <= n; j++){
//			if(i & (1<<(j-1))) res += cnt * A[j], cnt ++;
//		}
//		if(!i) ans = res;
//		else ans ^= res;
//		cout << res << " " << ans << '\n';
//	} 

	// �ݹ�
	G_subset(1, 1, 0, n);
	
	 
	cout << ans << '\n';
	return 0;
}

