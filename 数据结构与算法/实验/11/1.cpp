#include<bits/stdc++.h>
using namespace std;


/* 二叉搜索树 */ 
template <class T>
struct binaryTreeNode
{
	/* 节点元素 */ 
   T element;
   binaryTreeNode<T> *leftChild,   // left subtree
                     *rightChild;  // right subtree

   binaryTreeNode() {leftChild = rightChild = NULL;}
   binaryTreeNode(const T& theElement):element(theElement)
   {
      leftChild = rightChild = NULL;
   }
   binaryTreeNode(const T& theElement,
                  binaryTreeNode *theLeftChild,
                  binaryTreeNode *theRightChild)
                  :element(theElement)
   {
      leftChild = theLeftChild;
      rightChild = theRightChild;
   }
};

// 不支持修改操作 
template<class E>
class linkedBinaryTree // : public binaryTree<binaryTreeNode<E> >
{
	public:
		linkedBinaryTree() { root = NULL; Size = 0; }
		~linkedBinaryTree() { erase(); root = NULL; Size = 0; }
		bool empty() const { return Size == 0; }
		int size() const { return Size; }
		E* rootele() const;
		void make(int n);
		
		void inOrder(void(*theVisit)(binaryTreeNode<E>*)) { visit = theVisit; inOrder(root); }
		void postOrder(void(*theVisit)(binaryTreeNode<E>*)) { visit = theVisit; postOrder(root); }
		void levelOrder(void(*theVisit)(binaryTreeNode<E>*)) { visit = theVisit; levelOrder(root); }
		
		void inOut() { inOrder(Output); cout << '\n'; }
		void postOut() { postOrder(Output); cout << '\n'; }
		void levelOut() { levelOrder(Output); cout << '\n'; }

		void Count();
		
		int getnum(binaryTreeNode<E>* t) {
			if (t == NULL) return 0;
			int num = getnum(t->leftChild) + getnum(t->rightChild) + 1;
			return num;
		}
		int getheight(binaryTreeNode<E>* t) {
			if (t == NULL) return 0;
			int h = max(getheight(t->leftChild), getheight(t->rightChild)) + 1;
			return h;
		}
		binaryTreeNode<E>* getnode(int ind) { return node[ind]; }
		void erase();
		
		// rebuild tree
		void rebuild(int preord[], int inord[], int n) { root = crea(1, n, 1, n, preord, inord); }
		binaryTreeNode<E>* crea(int h1, int t1, int h2, int t2, int s1[], int s2[]);
	
//	private:
		binaryTreeNode<E> *root;
		binaryTreeNode<E> **node;
		
		static void (*visit)(binaryTreeNode<E> *t);		// 访问函数 
		static void preOrder(binaryTreeNode<E> *t);
		static void inOrder(binaryTreeNode<E> *t);
		static void postOrder(binaryTreeNode<E> *t);
		static void levelOrder(binaryTreeNode<E> *t);
		static void Output(binaryTreeNode<E> *t) { cout << t->element << " "; }
		static void dispose(binaryTreeNode<E> *t) { delete t; }
		
		int Size;
};

template <class E> void (*linkedBinaryTree<E>::visit)(binaryTreeNode<E>*);

template<class E>
binaryTreeNode<E>* linkedBinaryTree<E>::crea(int h1, int t1, int h2, int t2, int s1[], int s2[]) {
	if (h1 > t1 || h2 > t2) return NULL;
	binaryTreeNode<E> *head = new binaryTreeNode<E>;
	head->element = s1[h1];
	int i;
	for (i = h2; i <= t2; i++) {
		if (s2[i] == s1[h1]) break;
	}
	
	// i - h2 - 1
	// h1 + 1 + i - h2 - 1
	
	head->leftChild = crea(h1 + 1, h1 - h2 + i, h2, i - 1, s1, s2);
	head->rightChild = crea(h1 - h2 + i + 1, t1, i + 1, t2, s1, s2);
	return head;
}

template<class E>
E* linkedBinaryTree<E>::rootele() const {	// 返回指向根元素的指针 
	if (!Size) return NULL;
	else return &root->element;
} 
	
template<class E>
void linkedBinaryTree<E>::make(int n) {
	
	root = new binaryTreeNode<E>(1); 		// 根节点保证为 1 
	node = new binaryTreeNode<E> *[n + 1]; 
	 
	for (int i = 1; i <= n; i++) node[i] = NULL;	// **
	
	node[1] = root;
	for (int i = 1, a, b; i <= n; i++) { 
		cin >> a >> b;
		if (node[i] == NULL) node[i] = new binaryTreeNode<E>(i);
		if (a != -1) {
			if (node[a] == NULL) {
				node[a] = new binaryTreeNode<E>(a);
				node[i]->leftChild = node[a];
			}
			else {
				cout << node[a]->element << '\n';
				node[i]->leftChild = node[a];
			}
		}
		if (b != -1) {
			if (node[b] == NULL) {
				node[b] = new binaryTreeNode<E>(b);
				node[i]->rightChild = node[b];
			}
			else {
				node[i]->rightChild = node[b];
			}
		}
	}
	Size = n;
}

template<class E>
void linkedBinaryTree<E>::erase() {
	postOrder(dispose);
}
	
template<class E>
void linkedBinaryTree<E>::preOrder(binaryTreeNode<E> *t)	// 根节点作为递归的起始点 
{// Preorder traversal.
   if (t != NULL)
   {
      linkedBinaryTree<E>::visit(t);
      preOrder(t->leftChild);
      preOrder(t->rightChild);
   }
}

template<class E>
void linkedBinaryTree<E>::inOrder(binaryTreeNode<E> *t)
{// Inorder traversal.
   if (t != NULL)
   {
      inOrder(t->leftChild);
	  linkedBinaryTree<E>::visit(t);
      inOrder(t->rightChild);
   }
}

template<class E>
void linkedBinaryTree<E>::postOrder(binaryTreeNode<E> *t)
{// Postorder traversal.
   if (t != NULL)
   {
      postOrder(t->leftChild);
      postOrder(t->rightChild);
	  linkedBinaryTree<E>::visit(t);
   }
}

template <class E>
void linkedBinaryTree<E>::levelOrder(binaryTreeNode<E> *t)
{// Level-order traversal.
   queue<binaryTreeNode<E>*> q;
   
   while (t != NULL)	// 若根节点不为空 
   {
      visit(t);  // visit t   *** theVisit -> visit
      // put t's children on queue
      if (t->leftChild != NULL)
         q.push(t->leftChild);
      if (t->rightChild != NULL)
         q.push(t->rightChild);

      // get next node to visit
//      try {t = q.front();}
//      catch (queueEmpty) {return;}
		if (q.empty()) {
			return;
		}
		t = q.front(); 
    	q.pop();
   }
}

template <class E>
void linkedBinaryTree<E>::Count() {
	int *sz = new int[Size + 1];
	bool *vis = new bool[Size + 1];
	
	for (int i = 1; i <= Size; i++) {
		vis[i] = 0;
		sz[i] = 1;
	}
	
	// start from root
	//  recurve through stack
	stack<int> stk;
	stk.push(1);
	
	while (!stk.empty()) {
		// element domain: nodeID
		int cur = stk.top(); vis[cur] = true;
		binaryTreeNode<E>* curNode = node[cur];
		
		int lc = -1, rc = -1;
		if (curNode->leftChild != NULL) lc = curNode->leftChild->element;
		if (curNode->rightChild != NULL) rc = curNode->rightChild->element;
		
		if (lc < 0 and rc < 0) {
			stk.pop();
		}
		else if (lc > 0 and rc > 0 and vis[lc] and vis[rc]) {
			stk.pop();
			sz[cur] += sz[lc] + sz[rc];
		}
		else if (lc > 0 and vis[lc]) {
			stk.pop();
			sz[cur] += sz[lc];
		}	
		else if (rc > 0 and vis[rc]) {
			stk.pop();
			sz[cur] += sz[rc];
		}
		
		if (lc > 0 and !vis[lc]) stk.push(lc);			// 操作以 lc 为根的子树 
		if (rc > 0 and !vis[rc]) stk.push(rc);
	}
	
	for (int i = 1; i <= Size; i++) cout << sz[i] << " ";
	cout << '\n';
}


/***************************************************************************/ 
/********************************new part***********************************/
/***************************************************************************/ 

/* 节点拓延 */ 
template<class K, class E>
struct node {
	int leftSize;
	K key;
	E value;
}; 

template<class K, class E>
class binarySearchTree : public linkedBinaryTree<node<K, E> >
{
   public:
      // methods of dictionary
      bool empty() const {return Size == 0;}
      int size() const {return Size;}
      node<K, E>* find(const K& theKey) const;
      void insert(const node<K, E>& thePair);
      void erase(const K& theKey);

	  // RANK start from 1
//	  node* findbyRank(int rank) const;	
//	  void erasebyRank(int rank);
	  
      // additional method of bsTree
      void ascend() {inOut();}
	private:
//		int Size;
};

template<class K, class E>
node<K, E>* binarySearchTree<K,E>::find(const K& theKey) const
{// Return pointer to matching pair.
 // Return NULL if no matching pair.
   // p starts at the root and moves through
   // the tree looking for an element with key theKey
   int Xor = 0;
   binaryTreeNode<node<K, E> > *p = root;
   while (p != NULL) {
   	  Xor = Xor ^ p->element.key;
      // examine p->element
      if (theKey < p->element.key)
         p = p->leftChild;
      else
         if (theKey > p->element.key)
            p = p->rightChild;
         else {
		 	// found matching pair
		 	cout << Xor << '\n';
            return &p->element;
         }   	
   }


   // no matching pair
   cout << 0 << '\n';
   return NULL;
}

template<class K, class E>
void binarySearchTree<K,E>::insert(const node& thePair)
{// Insert thePair into the tree. Overwrite existing
 // pair, if any, with same key.
   // find place to insert
   
   int Xor = 0;
   binaryTreeNode<node> *p = root, *pp = NULL;
   stack<binaryTreeNode<node> > path;
   
   while (p != NULL)
   {// examine p->element
      path.push(*p);
      Xor = Xor ^ p->element.key;
   		
      pp = p;
      // move p to a child
      if (thePair.key < p->element.key)
         p = p->leftChild;
      else
         if (thePair.key > p->element.key)
            p = p->rightChild;
         else
         {// replace old value
            p->element.value = thePair.value;
            cout << 0 << '\n';
            return;
         }
   }
	
   cout << Xor << '\n';
	
   // get a node for thePair and attach to pp
   binaryTreeNode<node> *newNode
                 = new binaryTreeNode<node> (thePair);
   if (root != NULL) // the tree is not empty
      if (thePair.key < pp->element.key)
         pp->leftChild = newNode;
      else
         pp->rightChild = newNode;
   else
      root = newNode; // insertion into empty tree
   treeSize++;
   
   // insert successfully
   // 更新根节点至新插入节点的路径上修改 leftSize 域 
   // 1. 存储路径上的节点 
   // 2. once again
	auto pre = path.top();
	path.pop();   
	while (!path.empty()) {		// last one : root
		auto now = path.top();
		path.pop();
		
		if (*now.leftChild == pre) {
			now.element.leftSize ++;
		}
//		else break;
		
		pre = now;
	} 
}

template<class K, class E>
void binarySearchTree<K,E>::erase(const K& theKey)
{// Delete the pair, if any, whose key equals theKey.

   // search for node with key theKey
   int Xor = 0;
   binaryTreeNode<node> *p = root, *pp = NULL;
   stack<binaryTreeNode<node> > path1, path2;
   // path1: 从root至删除节点
   // path2: 从删除节点的右孩子至右子树最大节点
   // 若删除节点有2孩子，回溯 path1 + path2；
   // 0 or 1, 回溯 path1 
   
   while (p != NULL && p->element.key != theKey)
   {// move to a child of p
      path1.push(*p);
   	  Xor = Xor ^ p->element.key;
      pp = p;
      if (theKey < p->element.key)
         p = p->leftChild;
      else
         p = p->rightChild;
   }
   if (p == NULL) {
   		cout << 0 << '\n';
   		return; // no pair with key theKey
   }
   cout << Xor << '\n';   
	
	// path1
	auto pre = path1.top();
	path.pop();   
	while (!path1.empty()) {		// last one : root
		auto now = path1.top();
		path.pop();
		
		if (*now.leftChild == pre) {
			now.element.leftSize ++;
		}
//		else break;
		
		pre = now;
	} 
	

   // restructure tree
   // handle case when p has two children
   if (p->leftChild != NULL && p->rightChild != NULL)
   {// two children
      // convert to zero or one child case
      // find smallest element in right subtree of p
      binaryTreeNode<node> *s = p->rightChild, *ps = p;  // parent of s
      
      while (s->leftChild != NULL)
      {// move to smaller element
         path2.push(*s);
         ps = s;
         s = s->leftChild;
      }

      // move smallest from s to p, can't do a simple move
      // p->element = s->element as key is const
      binaryTreeNode<node> *q = new binaryTreeNode<node> (s->element, p->leftChild, p->rightChild);
      if (pp == NULL)
         root = q;
      else if (p == pp->leftChild)
              pp->leftChild = q;
           else
              pp->rightChild = q;
      if (ps == p) pp = q;
      else pp = ps;
      delete p;
      p = s;
      
      // path2
      path2.pop();
	  while (!path2.empty()) {
	  	auto now = path2.top();
	  	path2.pop();
	  	// pre node must be leftchild of now node.
	  	now.element.leftSize --;
	  } 
   };

   // p has at most one child
   // save child pointer in c
   binaryTreeNode<node> *c;
   if (p->leftChild != NULL)
      c = p->leftChild;
   else
      c = p->rightChild;

   // delete p
   if (p == root)
      root = c;
   else
   {// is p left or right child of pp?
      if (p == pp->leftChild)
         pp->leftChild = c;
      else pp->rightChild = c;
   }
   treeSize--;
   delete p;
}

// overload << for pair
template <class K, class E>
ostream& operator<<(ostream& out, node& x)
   {out << "key: " << x.key << " value: " << x.value << " leftSize: " << x.leftSize; 
   	return out;}
   

int main(){
	return 0;
}

