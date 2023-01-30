#include<bits/stdc++.h>
using namespace std;

template<class T>
class minHeap {
	public:
		minHeap(int ini = 10);
		~minHeap() { delete [] heap; }
		bool empty() const { return Size == 0; }
		int size() const { return Size; }
		const T& top() {
			if (Size == 0) { }
			return heap[1];
		}
		
		void pop();
		void push(const T&);
		void initialize(T *, int);
		
		void deactivate() {
			heap = NULL; Length = Size = 0;
		}
		void output(ostream& out) const;
	private:
		int Size;
		int Length;
		T * heap;
}; 

template<class T>
minHeap<T>::minHeap(int iniCap) {
//	if 
	Length = iniCap + 1; // **
	heap = new T[Length];
	Size = 0;
}

template<class T>
void changeLength1D(T*& a, int oldLength, int newLength)
{
//   if (newLength < 0)
//      throw illegalParameterValue("new length must be >= 0");

   T* temp = new T[newLength];              // new array
   int number = min(oldLength, newLength);  // number to copy
   copy(a, a + number, temp);
   delete [] a;                             // deallocate old memory
   a = temp;
}

template<class T>
void minHeap<T>::push(const T& ele) {
	if (Size == Length - 1) {
		changeLength1D(heap, Length, 2 * Length);
		Length <<= 1;
	}
	
	int cur = ++Size;
	
	// 向上起泡 
	while (cur != 1 && heap[cur / 2] > ele) {
		heap[cur] = heap[cur / 2];
		cur /= 2;
	}
	heap[cur] = ele;
}

template<class T>
void minHeap<T>::pop() {
	if (Size == 0) { return; }
	heap[1].~T();
	
	T last = heap[Size--];
	int cur = 1, child = 2;
	
	// 向下遍历 
	while (child <= Size) {
		if (child < Size && heap[child] > heap[child + 1]) child++;	
		
		if (last <= heap[child]) break;
		heap[cur] = heap[child];
		cur = child;
		child *= 2;	// cur = child / 2 
	}
	heap[cur] = last;

}

template<class T>
void minHeap<T>::initialize(T *theheap, int theSize) {
	delete [] heap;
	heap = theheap;
	Size = theSize;
	// How about Length ?
	// it assums: theheap.Length == heap.Length

	// 堆化
	for (int root = Size / 2; root >= 1; root--) {
		T rEle = heap[root];
		
		int child = 2 * root ;
		while (child <= Size) {
			if (child < Size && heap[child] > heap[child + 1]) child++;	
			if (rEle <= heap[child]) break;
			
			heap[child / 2] = heap[child];
			child *= 2;
		}
		heap[child / 2] = rEle;
	}    
}

template<class T>
void heapSort(T a[], int n) {	// ** 原地重排  **
	minHeap<T> heap(n);
	heap.initialize(a, n);
	
	for (int i = n - 1; i >= 1; i--) {
		T x = heap.top(); heap.pop(); // heap.Size = i then we can put the min ele on the (i + 1) th.
		a[i + 1] = x;
	}
	heap.deactivate();
}

void solve() {
	int n; cin >> n;
	int *a = new int[n + 1];
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
	}
	
	minHeap<int> hp(n);
	hp.initialize(a, n);
	cout << hp.top() << endl;
	
	int m; cin >> m;
	for (int i = 0, opt, num; i < m; i++) {
		cin >> opt;
		if (opt == 1) {
			cin >> num;
			hp.push(num);
			cout << hp.top() << endl;
		}
		else if (opt == 2) {
			hp.pop();
			cout << hp.top() << '\n';
		}
		else if (opt == 3) {
			int sz; cin >> sz;
			int *b = new int[sz + 1];
			for (int j = 1; j <= sz; j++) cin >> b[j];
			heapSort(b, sz);
			for (int j = sz; j >= 1; j--) cout << b[j] << " ";
			cout << endl;
		}
	}
}

int main() {
	solve();
	return 0;
}






