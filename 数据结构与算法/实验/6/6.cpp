#include<bits/stdc++.h>
using namespace std;

template<class T>
class arrayStack { // : public stack<T>
	public:
		arrayStack(int iniCap = 1000);
		~arrayStack() { delete [] stack; }
		bool empty() const { return Top == -1; }
		int size() const { return Top + 1; }
		T& top() {
			if (Top == -1) {
				
			}
			return stack[Top];
		}
		void pop() {
			stack[Top--].~T();		//  3221226356
//			Top--;
		}
		void push(const T& tar); 
		void clear() {
			while (!(this -> empty())) {
				this -> pop();
			}
		}
	private:
		int Top;
		int Length;
		T* stack;
};

template<class T>
arrayStack<T>::arrayStack(int iniCap) {
	Length = iniCap;
	stack = new T[Length];
	Top = -1;
}

template<class T>
void arrayStack<T>::push(const T& tar) {
	// changeLength1D()
	stack[++Top] = tar;
}

arrayStack<double> Dig;
arrayStack<int> Sym;

string operand[] = {"+", "-", "*", "/"};


// 取数字栈的两个数字以及符号栈的一个符号进行运行
void Cal() {
	double b = Dig.top(); Dig.pop();
	double a = Dig.top(); Dig.pop();
	int op = Sym.top(); Sym.pop();
						
	double Res;
	switch(op) {
		case 0:
			Res = a + b;
			break;
		case 1:
			Res = a - b;
			break;
		case 2:
			Res = a * b;
			break;
		case 3:
			Res = a / b;
			break;
	}
//	cout << a << " " << operand[op] << " " << b << " = " << Res << '\n';
	Dig.push(Res);
}

void solve() {
	
	int n; cin >> n;
	string Exp;

	// 一共 n 条待计算的表达式， 数字为一位

	for (int i = 1; i <= n; i++) {
//		Dig.clear(); Sym.clear();		// 3221225477 
		cin >> Exp;
		int len = Exp.length();
		// Cal
		for (int j = 0; j < len; j++) {
			if (Exp[j] <= '9' && Exp[j] >= '0') Dig.push(Exp[j] - '0');
			else {
				// 遇到同级别的符号计算直到栈空或者是遇到低级符号，将当前符号入栈
				if (Exp[j] == '+') {	// 3 - 4 + 2
					while (!Sym.empty() && (Sym.top() <= 3)) {
						Cal();
					}
					Sym.push(0);
				}
				else if (Exp[j] == '-') {
					while (!Sym.empty() && (Sym.top() <= 3)) {
						Cal();
					}					
					Sym.push(1);
				}
				else if (Exp[j] == '*') {
					while (!Sym.empty() && (Sym.top() == 2 || Sym.top() == 3)) {
						Cal();
					}
					Sym.push(2);
				}
				else if (Exp[j] == '/' ) {
					while (!Sym.empty() && (Sym.top() == 2 || Sym.top() == 3)) {
						Cal();
					}
					Sym.push(3);
				}
				else if (Exp[j] == '(') {
					Sym.push(4);
				}
				else if (Exp[j] == ')') {	// 5
					while (Sym.top() != 4) {
						Cal();
					}
					Sym.pop();
				}
			}
		}
		// More
		// 最多剩下两次计算
		while (Dig.size() > 1) { // + *
//			cout << "Cal" << '\n';
			Cal();
		}
		double Ans = Dig.top(); Dig.pop();
		cout << Ans << '\n';
	}
}

int main() {
	cout << setiosflags(ios::fixed) << setprecision(2);
	solve();
	return 0;
}

