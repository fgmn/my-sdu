#include<bits/stdc++.h>
using namespace std;

// 3221226356

// 节点
template<class T>
struct chainNode {
    T ele;
    chainNode<T>* next;
    
    chainNode() { }
    chainNode(const T& ele) { this -> ele = ele; }
    chainNode(const T& ele, chainNode<T>* next) {
        this -> ele = ele;
        this -> next = next;
    }
};


template<class T>
class chain {
    public:
        chain(int iniCap = 10);
        chain(const chain<T>&);
        ~chain();

        int indOf(const T& tar_ele) const;
        void erase(int ind);
        void insert(int tar_ind, const T& tar_ele);
        void reverse();
        bool empty() { return Size == 0; }
        int get_S() { return Size; }
//        chainNode<T>* get_F() { return fiNode; }
		void binSort(int r);
//		void clear();
        void Merge(chain<T> &c1, chain<T> &c2);		//  
        int Cal();
        void M_sort() {
        	fiNode = MergeSort(fiNode);
		}
        // 迭代器
    	class iterator;
    	iterator begin() { return iterator(fiNode); }
		iterator end() { return iterator(NULL); }
		
		class iterator {
		    public:
		        // typedef
		        // ......
				 
		        iterator(chainNode<T>* theNode = NULL) { node = theNode; }
		
		        // 解引用操作符
		        T& operator * () const { return node -> ele; }
		        T* operator -> () const { return &node -> ele; }
		
		        // 迭代器加法操作
		        iterator& operator ++ () { node = node -> next; return *this; }
		        iterator operator ++ (int) { iterator old = *this; node = node -> next; return old; }
		
		        // 相等检验
		        bool operator != (const iterator r) const { return node != r.node; }
		        bool operator == (const iterator r) const { return node == r.node; }
		    protected:
		        chainNode<T>* node;    
		};		 
    protected:
        chainNode<T>* fiNode;
        int Size;
        
};

// chain 实现
template<class T>
chain<T>::chain(int iniCap) {
    fiNode = NULL;
    Size = 0;
}

template<class T>
chain<T>::chain(const chain<T>& theList) {
    Size = theList.Size;
    if (Size == 0) { fiNode = NULL; return;}

    chainNode<T>* s = theList.fiNode;		//  
    fiNode = new chainNode<T>(s -> ele);
    s = s -> next;
    chainNode<T>* tarNode = fiNode;
    while (s != NULL) {
        tarNode -> next = new chainNode<T>(s -> ele);
        tarNode = tarNode -> next;
        s = s -> next;
    }
    tarNode -> next = NULL;
}

template<class T>
chain<T>::~chain() {
    while (fiNode != NULL) {
        chainNode<T>* nextNode = fiNode -> next;
        delete fiNode;
        fiNode = nextNode;
    }
}

template<class T>
int chain<T>::indOf(const T& tar_ele) const {	// 返回元素首次出现的索引 
    chainNode<T>* curNode = fiNode;
    int ind = 0;
    while(curNode != NULL && curNode -> ele != tar_ele) {
        curNode = curNode -> next;
        ind ++;
    }
    if (curNode == NULL) return -1;
    return ind;
}

template<class T>
void chain<T>::erase(int ind) {
    chainNode<T>* tar_;
    if (ind == 0) {
        tar_ = fiNode;
        fiNode = fiNode -> next;
    }
    else {
        chainNode<T>* p = fiNode;
        // 定位待删除节点的前驱 
        for (int i = 0; i < ind - 1; i++) p = p -> next;
        tar_ = p -> next;
        p -> next = p -> next -> next;
    }
    Size--;
    delete tar_;
}

template<class T>
void chain<T>::insert(int tar_ind, const T& tar_ele) {
    if (tar_ind == 0) {
        fiNode = new chainNode<T>(tar_ele, fiNode);
    }
    else {
        chainNode<T>* p = fiNode;
        for (int i = 0; i < tar_ind - 1; i++) p = p -> next;
        p -> next = new chainNode<T>(tar_ele, p -> next);
    }
    Size ++;
}

template<class T>
void chain<T>::reverse() {	// 1 -> 2 -> 3 -> 4 -> 5	// 1 -> 2 -> NULL	// 1
	if(Size <= 1) return;
//	chainNode<T>* cur = fiNode -> next, pre = fiNode, nxt = cur -> next;
//invalid conversion from 'chainNode<int>*' to 'const int&' [-fpermissive]
// 实际会声明 ： chainNode<T> pre = fiNode
	chainNode<T>* cur = fiNode -> next;
	chainNode<T>* pre = fiNode;
	chainNode<T>* nxt = cur -> next;

	while (nxt != NULL) {
		cur -> next = pre;
		pre = cur;
		cur = nxt;
		nxt = cur -> next;
	}
	cur -> next = pre;
	
	fiNode -> next = NULL;
	fiNode = cur; 
}

// 0. Cal
// 1. Sort
// 2. Merge
// 3. Cal 

// 使用箱子排序发现题目数据有负数！runtime error
template<class T>
void chain<T>::binSort(int r) {
	chainNode<T> **bm, **tp;
	bm = new chainNode<T>* [r + 1];
	tp = new chainNode<T>* [r + 1];
	for (int b = 0; b <= r; b++) bm[b] = NULL;
	
	
	for (; fiNode != NULL; fiNode = fiNode -> next) {
		int Bin = fiNode -> ele;
		if (bm[Bin] == NULL) bm[Bin] = tp[Bin] = fiNode;
		else {
			tp[Bin] -> next = fiNode;
			tp[Bin] = fiNode;
		}
	}
	
	chainNode<T>* y = NULL;
	for (int Bin = 0; Bin <= r; Bin++) {
		if (bm[Bin] != NULL) {
			if (y == NULL) fiNode = bm[Bin];
			else y -> next = bm[Bin];
			y = tp[Bin];
		}
	}
	if (y != NULL) y -> next = NULL;
	
	delete [] bm;
	delete [] tp; 
}

template<class T>
void chain<T>::Merge(chain<T> &c, chain<T> &c_) {
	// 归并 
	// clear() != ~
	// iterator != pointer
	 
	fiNode = new chainNode<T>;
	chainNode<T>* ptr;
	if(c_.get_S() == 0) {
//		cout << "1" << '\n';
		Size = c.get_S();
		chain<int>::iterator it = c.begin();
		this -> fiNode -> ele = *it;
		ptr = fiNode;
		it++;
		while (it != c.end()) {
			chainNode<T>* newNode = new chainNode<T>;
			newNode -> ele = *it;
			ptr -> next = newNode;
			ptr = newNode;
			it++;
		}
		ptr -> next = NULL;
		// clear
		return;
	} 
	
	// error
	chain<int>::iterator it = c.begin();
	chain<int>::iterator it_ = c_.begin();
	
	if ((*it) < (*it_)) fiNode -> ele = *it, it++;
	else{
		fiNode -> ele = *it_;
		it_++; 
	}
	Size ++;
	
	// 维护链表尾节点
	chainNode<T>* p = this -> fiNode;
	
	while (it != c.end() && it_ != c_.end()) {
//		cout << "2" << '\n';
		chainNode<T>* newNode = new chainNode<T>;
		if (*it < *it_) {
			newNode -> ele = *it;
			it ++;
		}
		else {
			newNode -> ele = *it_;
			it_ ++;	
		}
		p -> next = newNode;
		p = newNode;
		Size ++;
	}
	
	while (it != c.end()) {
		chainNode<T>* newNode = new chainNode<T>;
		newNode -> ele = *it;
		it ++;
		p -> next = newNode;
		p = newNode;
		Size ++;
	}
	while (it_ != c_.end()) {
		chainNode<T>* newNode = new chainNode<T>;
		newNode -> ele = *it_;
		it_ ++;
		p -> next = newNode;
		p = newNode;
		Size ++;
	}
	
//	c.~chain();
//	c_.~chain();
	
}

template<class T>
int chain<T>::Cal() {
	int ind = 0, res = 0;
	chainNode<T>* p = fiNode;
	while (p != NULL) {
		res += ind ^ (p -> ele);
		p = p -> next;
		ind ++;
	}
	return res;
}

template<class T>
chainNode<T>* MergeTwoList(chainNode<T>* l1, chainNode<T>* l2) {
	chainNode<T>* dummpy = new chainNode<T>;		// 哑节点 
	chainNode<T>* tail = dummpy;
	while (l1 != NULL && l2 != NULL) {
		if ((l1 -> ele) < (l2 -> ele)) {
			tail -> next = l1;
			l1 = l1 -> next;
		}
		else {
			tail -> next = l2;
			l2 = l2 -> next;			
		}
		tail = tail -> next;
	} 
	
	while (l1 != NULL) {
		tail -> next = l1;
		tail = tail -> next;
		l1 = l1 -> next;
	}
	while (l2 != NULL) {
		tail -> next = l2;
		tail = tail -> next;
		l2 = l2 -> next;		
	}
	
	return dummpy -> next;						// 不影响数据的组织 
}


template<class T>
chainNode<T>* MergeSort(chainNode<T>* head) {
	if (head == NULL || head -> next == NULL) return head;
	chainNode<T>* slow = head;
	chainNode<T>* fast = head -> next;
	while (fast != NULL && fast -> next != NULL) {
		slow = slow -> next;
		fast = fast -> next -> next;
	}
	chainNode<T>* last = slow -> next;
	slow -> next = NULL;
	chainNode<T>* fi = MergeSort(head);
	chainNode<T>* se = MergeSort(last); 
	chainNode<T>* res = MergeTwoList(fi, se);
	return res;
}
 
void solve() {
	chain<int> t1, t2, T;
	int n, m, in; cin >> n >> m;
	for (int i = 0; i < n; i++) {
		cin >> in;
		t1.insert(i, in);
	}
	for (int i = 0; i < m; i++) {
		cin >> in;
		t2.insert(i, in);
	}  
	
	// insertSort
	// ......
	
	// MergeSort
	t1.M_sort();
	t2.M_sort();
	
//	t1.binSort(1e7);
//	t2.binSort(1e7);
//	cout << "ok" << '\n';
	
	cout << t1.Cal() << '\n';
	cout << t2.Cal() << '\n';

	T.Merge(t1, t2);
	
	cout << T.Cal() << '\n';
	// check
//	for (chain<int>::iterator it = T.begin(); it != T.end(); it++) {
//		cout << *it << ' ';
//	}
//	cout << '\n';
	
} 

int main(){
	solve();
	return 0;
}

