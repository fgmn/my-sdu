#include<bits/stdc++.h>
using namespace std;


template<class T>
struct node
{
	T element;
	int leftSize;
	
	node<T> *leftChild,   // left subtree
	        *rightChild;  // right subtree
	
	node() { leftChild = rightChild = NULL; leftSize = 0; }
	node(const T& theElement):element(theElement)
	{
		leftChild = rightChild = NULL;
		leftSize = 0;
	}
	node(const T& theElement,
				  int sz,
	              node<T> *theLeftChild,
	              node<T> *theRightChild):element(theElement),leftSize(sz)
	{
		leftChild = theLeftChild;
		rightChild = theRightChild;
	}
	
	bool operator == (const node& ano) const {
		return element == ano.element;
	}
};

template<class T>
class IBStree {
	public:
		IBStree() {
			root = NULL;
			Size = 0;
		}
		
		node<T>* find(const T& ele);
		void insert(const T& ele);
		void erase(const T& ele);
		
		void getbyRank(int, int);
		
	private:
		int Size;
		node<T> *root;
};

template<class T>
node<T>* IBStree<T>::find(const T& ele) {
	int Xor = 0;
	node<T> *p = root;
	
	while (p != NULL) {
		Xor ^= p->element;
		
		if (ele < p->element) p = p->leftChild;
		else {
			if (ele > p->element) p = p->rightChild;
			else {
				// match
				cout << Xor << '\n';
				return p;
			}
		}
	}
	
	// not match
	cout << 0 << '\n';
	return NULL;
	
}

template<class T>
void IBStree<T>::insert(const T& ele) {
	int Xor = 0;
	node<T> *p = root, *pp = NULL;
	
	stack<node<T>* > path;
	
	while (p != NULL) {
		path.push(p);
		Xor ^= p->element;
		
		pp = p;
		
		if (ele < p->element) p = p->leftChild;
		else {
			if (ele > p->element) p = p->rightChild;
			else {
				cout << 0 << '\n';
				return;
			}
		}
	} 
	
	cout << Xor << '\n';
	
	node<T> *newNode = new node<T>(ele);
	if (root != NULL) {
		if (ele < pp->element) pp->leftChild = newNode;
		else pp->rightChild = newNode;
	}
	else {
		root = newNode;
		Size++;
		return;
	}
	Size++;
	
	if (path.empty()) return;
	auto pre = newNode;
	
	while (!path.empty()) {
		auto now = path.top();
		path.pop();
		
		if (now->leftChild != NULL && now->leftChild->element == pre->element) {
			now->leftSize++;
		}
		pre = now; 
	}
}
	
template<class T>
void IBStree<T>::erase(const T& ele) {
	int Xor = 0;
	node<T> *p = root, *pp = NULL;
	
	stack<node<T>* > path;
	
	while (p != NULL && p->element != ele) {
		path.push(p);		// push at least one or return on 145 
		Xor ^= p->element;
		pp = p;
		if (ele < p->element) p = p->leftChild;
		else p = p->rightChild; 
	}
	
	if (p == NULL) {
		cout << 0 << '\n';
		return;
	}
	
	// have found ele
	Xor ^= p->element;
	path.push(p);
	cout << Xor << '\n';
	
	// path1
	auto pre = path.top();
	path.pop();

	while (!path.empty()) {
		auto now = path.top();
		path.pop();
		if (now->leftChild != NULL && now->leftChild->element == pre->element) now->leftSize--;
		pre = now;
	}
	
	// 2º¢×Ó
	if (p->leftChild != NULL && p->rightChild != NULL) {
		node<T> *s = p->rightChild, *ps = p;
		while (s->leftChild != NULL) {
			path.push(s);
			ps = s;
			s = s->leftChild;
		} 
		
		// path2
		while (!path.empty()) {
			auto now = path.top();
			path.pop();
			now->leftSize -= 1;
		}
		// Ìæ»» 
		node<T> *q = new node<T>(s->element, p->leftSize, p->leftChild, p->rightChild);
		if (pp == NULL) {
			root = q;
		}
		else if (p == pp->leftChild) {
			pp->leftChild = q;
		}
		else pp->rightChild = q;
		
		// ×ª»»ÎªÉ¾³ýs 
		if (ps == p) pp = q;
		else pp = ps;
		
		delete p;
		p = s; 
	}
	
	// 1º¢×Ó
	node<T> *c;
	if (p->leftChild != NULL) {
		c = p->leftChild;
	} 
	else c = p->rightChild;

	if (p == root) {
		root = c;
	}
	else {
		if (p == pp->leftChild) {
			pp->leftChild = c;
		}
		else pp->rightChild = c; 
	}
	Size--;
	delete p;
} 

template<class T>
void IBStree<T>::getbyRank(int r, int op) {
	if (r <= 0 || r > Size) {
		cout << 0 << '\n';
		return;
	}

	node<T> *now = root;
	while (now->leftSize != r - 1) {
		if (now->leftSize > r - 1) {
			now = now->leftChild;
		}
		else if (now->leftSize < r - 1)	{
			r -= now->leftSize + 1; 
			now = now->rightChild;
		}
	}
	
	if (op == 0) {
//		cout << "finding... " << now->element << '\n'; 
		find(now->element);
	}
	else if (op == 1) {
		erase(now->element);
	}
}	

void solve() {
	IBStree<int> tree;
	
	int m; cin >> m;
	for (int i = 1, a, b; i <= m; i++) {
		cin >> a >> b;
		switch (a) {
			case 0:
				tree.insert(b);
				break;
			case 1:
				tree.find(b);
				break;
			case 2:
				tree.erase(b);
				break;
			case 3:
				tree.getbyRank(b, 0);
				break;
			case 4:
				tree.getbyRank(b, 1);
				break;			
		}
	}	
}

int main(){
	solve();
	return 0;
}

