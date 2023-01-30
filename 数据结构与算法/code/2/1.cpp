#include<bits/stdc++.h>
using namespace std;

// ������� ���鳤�� 
// 0 - n-1
template<class T>
class Sort {
	private:
		int n;
		T *p;
	public:
		Sort(T *P, int n) {		// T p[]
		 	this->p = P;
		 	this->n = n;
		}
		~Sort() {
			delete [] p;
		}
		void Rank_sort();
		void Selection_sort();
		void Bubble_sort();
		void Insection_sort();
		void output() {
			for (int i=0; i<n; i++) cout << p[i] << ' ';
			cout << '\n';
		}
};


template<class T>
void Sort<T>::Rank_sort() {
	// ���μ��� 
	int *r = new int [n];
	for (int i=0; i<n; i++) r[i] = 0;
	
	for (int i=1; i<n; i++) {
		for (int j=0; j<i; j++) {
			if(p[j] <= p[i]) r[i] ++;	// j < i �� p[j] == p[i] : �����ȶ���
			else r[j] ++;
		}
	}
	// ��������Ϊ[0, n-1]
	T *u = new T [n];
	for (int i=0; i<n; i++) u[r[i]] = p[i];
	for (int i=0; i<n; i++) p[i] = u[i];
	delete [] u;
	delete [] r;
}

template<class T>
void Sort<T>::Selection_sort() {
	bool sorted = false;
	for (int i = n; !sorted && (i > 1); i--) {
		int IndexOfMax = 0;
		sorted = true;
		for (int j = 1; j < i; j++) {
			if(p[IndexOfMax] <= p[j]) IndexOfMax = j;
			else sorted = false;
			// swap(p[IndexOfMax], p[i - 1]);
		}
		swap(p[IndexOfMax], p[i - 1]);
	}
}

template<class T>
void Sort<T>::Bubble_sort() {	
	for (int i = n, swaped; i > 1 && swaped; i--) {	// ����
		swaped = false;
		for (int j = 0; j < i - 1; j++) {
			if(p[j] > p[j+1]){	// �����ȶ���
				swap(p[j], p[j+1]);
				swaped = true;
			}
		}		
	}
}

template<class T>
void Sort<T>::Insection_sort() {
	for (int i = 1; i < n; i++) {
		// �� a[i] ���� a[0 : i-1]
		T t = p[i];
		int j;
		for (j = i - 1; j >= 0 && t < p[j]; j --)
			p[j+1] = p[j];
		p[j+1] = t;
	}
}

int main(){
	int n; cin >> n;
	int *a = new int [n];
	for (int i=0; i<n; i++) cin >> a[i];
	
	Sort<int> A(a, n);	// ���� a[]�׵�ַ 
//	A.Rank_sort();
//	A.Selection_sort();
//	A.Bubble_sort();
	A.Insection_sort();
	A.output();
	
	return 0;
}
