#include<bits/stdc++.h>
using namespace std;

// 7�����
 
struct stu {
	string name, phone;
	int clas, dor;
//	stu(string s1, string s2, int a1, int a2) {
//		name = s1; phone = s2; clas = a1; dor = a2;
//	}
};

// ��������������� Find its index 
// index < listSize

template<class T>
void cl(T*& a, int ol, int nl) {
	T* t = new T[nl];
	int num = min(ol, nl);
	copy(a, a + num, t);
	delete [] a;
	a = t;
}


/* ʵ�ֵò��Ǻܺ�,Ӧ����ʵ�����Ա�arrayList�࣬��������ͨѶ¼�� */
template<class T>
class Alist {
	public:
		Alist(int C = 1000);
		Alist(const Alist<T>&);
		~Alist() { delete [] ele; }
		void Push(T a);
		// not ADT
		int Find(string name);	// ������������
		int cal(int tar);		// ������ general�� 
		void Del(string Dtar);
		// set
		void setP(int ind, string np) {	ele[ind].phone = np; }
		void setC(int ind, int nc) { ele[ind].clas = nc; }
		void setD(int ind, int nd) { ele[ind].dor = nd; }
//		void chk() {
//			for (int i = 0; i < Size; i++) {
//				cout << ele[i].name << ' ' << ele[i].phone << ' ' << ele[i].clas << ' ' << ele[i].dor;
//				cout << '\n';
//			}
//		}	
		
	private:
		T* ele;
		int Cap, Size;	// ��������ǰ��С 
}; 

// ʵ��
template<class T>
Alist<T>::Alist(int C) {
	Cap = C;
	ele = new T[Cap];
	Size = 0;
}

template<class T>
Alist<T>:: Alist(const Alist<T>& a) {
	Cap = a.Cap;
	Size = a.Size;
	ele = new T[Cap];
	copy(a.ele, a.ele + Size, ele);		// (���ƶ��� ���Ƴ��ȣ� Ŀ��ռ�) 
}

template<class T>
void Alist<T>::Push(T a) {
	// �ж��Ƿ�����
	if (Size == Cap) {
		cl(ele, Cap, 2*Cap);
		Cap *= 2;
	}
	
	ele[Size] = a;		// a: student 's example
	Size++; 
}

template<class T>
int Alist<T>::Find(string name) {
	for (int i = 0; i < Size; i++) {
		if (ele[i].name == name) return i;
	}
	return -1;
}

template<class T>
int Alist<T>::cal(int tar) {
	int res = -1, cnt = 0;		// res = 0
	for (int i = 0; i < Size; i++) {
		if(ele[i].clas == tar){
			if(res == -1) res = ele[i].dor;
			else res = res ^ ele[i].dor;
			cnt ++;
		}
	}
//	if(res == -1) res = 0;
//	if(cnt == 1) res = 0;
//	return res;
	return res == -1 ? 0 : res;

}

template<class T>
void Alist<T>::Del(string Dtar) {
	int ind = Find(Dtar);
	if(ind == -1) return;
	copy(ele + ind + 1, ele + Size, ele + ind);
//	ele[--Size].~T();	// ������������
	--Size;
}

Alist<stu> abk;		// 

void solve() {
	int N; cin >> N;	// �������� 
	string n, p; int c, d;
	for (int i = 1, op, ind; i <= N; i++) {
		cin >> op;
		if (op == 0) {
			cin >> n >> p >> c >> d;
//			abk.Push(stu(n, p, c, d));
			abk.Push({n, p, c, d});		// ��Ԫ�� 
		}
		else if (op == 1) {
			cin >> n;
			abk.Del(n);
		}
		else if (op == 2) {
			int op1;
			cin >> n >> op1;
			ind = abk.Find(n);		
			if(op1 == 1) {
				cin >> p;
				if(ind == -1) continue;	
				abk.setP(ind, p);
			}
			else if (op1 == 2) {
				cin >> c;	
				if(ind == -1) continue;	
				abk.setC(ind, c);		
			}
			else if (op1 == 3) {
				cin >> d;
				if(ind == -1) continue;		
				abk.setD(ind, d);				
			}
		}
		else if (op == 3) {
			cin >> n;
			ind = abk.Find(n);
			if(ind == -1) cout << 0 << '\n';
			else cout << 1 << '\n';				
		}
		else if (op == 4) {
			cin >> c;
			cout << abk.cal(c) << '\n';
		}
	}
//	abk.chk();
}

int main(){
//	freopen("out.txt", "w", stdout);
	
	solve();  
	return 0;
}

//struct Axis{
//	float x, y, z, rx, ry, rz;
//};
