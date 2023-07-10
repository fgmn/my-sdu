#include<bits/stdc++.h>
using namespace std;

/* 3221225477
3221226356
*/


template <class T>
struct binaryTreeNode
{
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
		void postOrder(void(*theVisit)(binaryTreeNode<E>*)) { visit = theVisit; postOrder(root); }
		void levelOrder(void(*theVisit)(binaryTreeNode<E>*)) { visit = theVisit; levelOrder(root); }
		
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
	
	private:
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
	// recurve through stack
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

void solve1() {
	linkedBinaryTree<int> tree;
	int n; cin >> n;
	tree.make(n);
	tree.levelOut();
	
	tree.Count();
	
	for (int i = 1; i <= n; i++) {
		cout << tree.getheight(tree.getnode(i)) << " ";
	}
	cout << endl;
}

void solve2() {
	linkedBinaryTree<int> tree;
	int n; cin >> n;	
	int *s1 = new int[n + 1];
	int *s2 = new int[n + 1];
	for (int i = 1; i <= n; i++) cin >> s1[i];
	for (int i = 1; i <= n; i++) cin >> s2[i];
	
	tree.rebuild(s1, s2, n);
	tree.postOut();	
}

int main(){	
	
//	solve1();
	solve2();
	
	return 0;
}

