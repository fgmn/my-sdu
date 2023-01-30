#include<bits/stdc++.h>
using namespace std;

int ans = 0;

// ������ 
template<class T>
void permutations(T list[], int k, int m) {
	// ����list[k:m]ȫ���� 
	if(k == m) {
//		copy(list, list+m+1, ostream_iterator<T>(cout, " "));
//		cout << '\n';
		int res = 0;
		for (int i = 1; i <= k; i++){
			res += i ^ list[i];		// ע��������� �� res = res + (i ^ list[i]);
		}
		ans |= res;		
	}
	else {
		for (int i = k; i <= m; i++){
			swap(list[k], list[i]);
			permutations(list, k+1, m);
			swap(list[k], list[i]);
		}
	}
}


// DFS: �ֵ���

int main(){
	int n; cin >> n;
	int A[11];
	
	for (int i = 1; i <= n; i++){
		cin >> A[i];
	} 
	permutations(A, 1, n);
	
	cout << ans << '\n';
	
	return 0;
}

