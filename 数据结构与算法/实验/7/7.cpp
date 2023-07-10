#include<bits/stdc++.h>
using namespace std;

// 循环队列元素位置的映射方式和arrayList不同,不能继承！
template<class T>
class arrayQueue {
	public:
		arrayQueue(int iniCap = 10);
		~arrayQueue() { delete [] queue; }
		bool empty() const { return Front == Back; }
		int size() const { return (Back - Front + Length) % Length; }
		T& front() {
			if (Front == Back) { cout << "The queue is empty.\n"; return; }
			return queue[(Front + 1) % Length];
		}
		T& back() { 
			if (Front == Back) { cout << "The queue is empty.\n"; return; }
			return queue[Back];
		}
		void pop() {
			Front = (Front + 1) % Length;	// 采用循环队列，且 Front 处不存储信息
			queue[Front].~T();
		}
		void push(const T& Ele); 
	private:
		int Front, Back, Length;
		T* queue;	
};

template<class T>
arrayQueue<T>::arrayQueue(int iniCap) {
	Length = iniCap;
	queue = new T[Length];
	Front = 0;
	Back = 0;
}

template<class T>
void arrayQueue<T>::push(const T& Ele) {
	// CL
	if ((Back + 1) % Length == Front) {
		T* newQ = new T[2 * Length];
		int st = (Front + 1) % Length;
		// 由于队列的最大容量为 arrayLength - 1， 因此扩容时数组 Front 处有唯一空位
		if (st < 2) {	// _ 1 2 3 4 or 1 2 3 4 _
			copy(queue + st, queue + st + Length - 1, newQ);
		}
		else {	// 3 4 5 _ 1 2 -> 1 2 3 4 5 _ _ _ _ ...
			copy(queue + st, queue + Length, newQ);
			copy(queue, queue + Back + 1, queue + Length - st);
		}
		// 0 位置为队首
		Front = 2 * Length - 1;
		Back = Length - 2;
		Length *= 2;
		queue = newQ;
	}
	Back = (Back + 1) % Length;
	queue[Back] = Ele;

	// 题目 ： 使容量达到 arrayLength
	// int Back = (Front + Size++) % arrayLength;
	// queue[Back] = theElement;
}

arrayQueue<int> Q;

// 1 2 3 4 5 6 7 8 9 10
// 3 4 5 6 7 8 9 10 2
// 5 6 7 8 9 10 2 4
// 2 4 6 8 10
// 6 8 10 4
// 10 4 8
// 8 4
// 4

int main(){
	int n; cin >> n;
	for (int i = 1; i <= n; i++) {
		Q.push(i);
	}
	while (Q.size() >= 2) {
		Q.pop();
		auto f = Q.front();
		Q.pop();
		Q.push(f);
	}
	int Ans = Q.front(); Q.pop();
	cout << Ans << '\n';
	
	return 0;
}

