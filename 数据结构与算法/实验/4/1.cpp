#include<bits/stdc++.h>
using namespace std;

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

    chainNode<T>* s = theList.fiNode;
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
    delete tar_;    // 回收指针所指向的空间
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
    Size++;
}

template<class T>
void chain<T>::reverse() {	
    // 1 -> 2 -> 3 -> 4 -> 5
    // pre  cur  nxt
    // 1 -> 2 -> NULL	
    // 1
	if(Size <= 1) return;
//	chainNode<T>* cur = fiNode -> next, pre = fiNode, nxt = cur -> next;
//invalid conversion from 'chainNode<int>*' to 'const int&' [-fpermissive]
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

void op() {

    chain<int> tt;

    int n, q; cin >> n >> q;
    for (int i = 0, in; i < n; i++) {
        cin >> in;
        tt.insert(i, in);
    }
    
    for (int i = 1, op, idx, val; i <= q; i++) {
    	cin >> op;
    	if (op == 1) {
    		cin >> idx >> val;
    		tt.insert(idx, val);
		}
		else if (op == 2) {
			cin >> val;
			int tarr = tt.indOf(val);
			if (tarr != -1) tt.erase(tarr);
			else cout << -1 << '\n';
		}
		else if (op == 3) {
			// 逆置 operation 
			tt.reverse();
		}
		else if (op == 4) {
			cin >> val;
			cout << tt.indOf(val) << '\n';
		}
		else if(op == 5) {
			int ans = 0, i = 0;
			for (chain<int>::iterator it = tt.begin(); it != tt.end(); it++, i++) {
				ans = ans + (i ^ (*it));
			}
			cout << ans << '\n';
		}
	}
	
	// check
//	for (chain<int>::iterator it = tt.begin(); it != tt.end(); it++) {
//		cout << *it << ' ';
//	}
//	cout << '\n';
}

int main(){
    op();
	return 0;
}

