#include<bits/stdc++.h>
using namespace std;

// huffman树本质上是二叉树，其构造是特殊的二叉树建树过程 
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


template<class E>
class linkedBinaryTree // : public binaryTree<binaryTreeNode<E> >
{
   public:
      linkedBinaryTree() {root = NULL; treeSize = 0;}
      ~linkedBinaryTree(){erase();}; 
      bool empty() const {return treeSize == 0;}
      int size() const {return treeSize;}
      E* rootElement() const;
      void makeTree(const E& element,
           linkedBinaryTree<E>&, linkedBinaryTree<E>&);
      linkedBinaryTree<E>& removeLeftSubtree();
      linkedBinaryTree<E>& removeRightSubtree();
      void preOrder(void(*theVisit)(binaryTreeNode<E>*))
           {visit = theVisit; preOrder(root);}
      void inOrder(void(*theVisit)(binaryTreeNode<E>*))
           {visit = theVisit; inOrder(root);}
      void postOrder(void(*theVisit)(binaryTreeNode<E>*))
           {visit = theVisit; postOrder(root);}
      void levelOrder(void(*)(binaryTreeNode<E> *));
      void preOrderOutput() {preOrder(output); cout << endl;}
      void inOrderOutput() {inOrder(output); cout << endl;}
      void postOrderOutput() {postOrder(output); cout << endl;}
      void levelOrderOutput() {levelOrder(output); cout << endl;}
      void erase()
           {
              postOrder(dispose);
              root = NULL;
              treeSize = 0;
           }
      int height() const {return height(root);}
   protected:
      binaryTreeNode<E> *root;                // pointer to root
      int treeSize;                           // number of nodes in tree
      static void (*visit)(binaryTreeNode<E>*);      // visit function
      static int count;         // used to count nodes in a subtree
      static void preOrder(binaryTreeNode<E> *t);
      static void inOrder(binaryTreeNode<E> *t);
      static void postOrder(binaryTreeNode<E> *t);
      static void countNodes(binaryTreeNode<E> *t)
                  {
                     visit = addToCount; 
                     count = 0;
                     preOrder(t);
                  }
      static void dispose(binaryTreeNode<E> *t) {delete t;}
      static void output(binaryTreeNode<E> *t)
                  {cout << t->element << ' ';}
      static void addToCount(binaryTreeNode<E> *t)
                  {count++;}
      static int height(binaryTreeNode<E> *t);
};
// the following should work but gives an internal compiler error
 template <class E> void (*linkedBinaryTree<E>::visit)(binaryTreeNode<E>*);
// so the explicit declarations that follow are used for our purpose instead
//void (*linkedBinaryTree<int>::visit)(binaryTreeNode<int>*);
//void (*linkedBinaryTree<booster>::visit)(binaryTreeNode<booster>*);
//void (*linkedBinaryTree<pair<int,int> >::visit)(binaryTreeNode<pair<int,int> >*);
//void (*linkedBinaryTree<pair<const int,char> >::visit)(binaryTreeNode<pair<const int,char> >*);
//void (*linkedBinaryTree<pair<const int,int> >::visit)(binaryTreeNode<pair<const int,int> >*);

template<class E>
E* linkedBinaryTree<E>::rootElement() const
{// Return NULL if no root. Otherwise, return pointer to root element.
   if (treeSize == 0)
      return NULL;  // no root
   else
      return &root->element;
}

template<class E>
void linkedBinaryTree<E>::makeTree(const E& element,
           linkedBinaryTree<E>& left, linkedBinaryTree<E>& right)
{// Combine left, right, and element to make new tree.
 // left, right, and this must be different trees.
   // create combined tree
   root = new binaryTreeNode<E> (element, left.root, right.root);
   treeSize = left.treeSize + right.treeSize + 1;

   // deny access from trees left and right
   left.root = right.root = NULL;
   left.treeSize = right.treeSize = 0;
}

template<class E>
linkedBinaryTree<E>& linkedBinaryTree<E>::removeLeftSubtree()
{// Remove and return the left subtree.
   // check if empty
//   if (treeSize == 0)
//      throw emptyTree();

   // detach left subtree and save in leftSubtree
   linkedBinaryTree<E> leftSubtree;
   leftSubtree.root = root->leftChild;
   count = 0;
   leftSubtree.treeSize = countNodes(leftSubtree.root);
   root->leftChild = NULL;
   treeSize -= leftSubtree.treeSize;
  
   return leftSubtree;
}

template<class E>
linkedBinaryTree<E>& linkedBinaryTree<E>::removeRightSubtree()
{// Remove and return the right subtree.
   // check if empty
//   if (treeSize == 0)
//      throw emptyTree();

   // detach right subtree and save in rightSubtree
   linkedBinaryTree<E> rightSubtree;
   rightSubtree.root = root->rightChild;
   count = 0;
   rightSubtree.treeSize = countNodes(rightSubtree.root);
   root->rightChild = NULL;
   treeSize -= rightSubtree.treeSize;
  
   return rightSubtree;
}

template<class E>
void linkedBinaryTree<E>::preOrder(binaryTreeNode<E> *t)
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
void linkedBinaryTree<E>::levelOrder(void(*theVisit)(binaryTreeNode<E> *))
{// Level-order traversal.
   queue<binaryTreeNode<E>*> q;
   binaryTreeNode<E> *t = root;
   while (t != NULL)
   {
      theVisit(t);  // visit t

      // put t's children on queue
      if (t->leftChild != NULL)
         q.push(t->leftChild);
      if (t->rightChild != NULL)
         q.push(t->rightChild);

      // get next node to visit
//      try {t = q.front();}
//      catch (queueEmpty) {return;}
      
      if (!q.empty()) {
      	t = q.front();
	  }
	  else return;
	  
      q.pop();
   }
}

template <class E>
int linkedBinaryTree<E>::height(binaryTreeNode<E> *t)
{// Return height of tree rooted at *t.
   if (t == NULL)
      return 0;                    // empty tree
   int hl = height(t->leftChild);  // height of left
   int hr = height(t->rightChild); // height of right
   if (hl > hr)
      return ++hl;
   else
      return ++hr;
}


// 最小堆 
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
class minHeap // : public minPriorityQueue<T>
{
   public:
      minHeap(int initialCapacity = 10);
      ~minHeap() {delete [] heap;}
      bool empty() const {return heapSize == 0;}
      int size() const
          {return heapSize;}
      const T& top()
         {// return min element
//            if (heapSize == 0)
//               throw queueEmpty();
            return heap[1];
         }
      void pop();
      void push(const T&);
      void initialize(T *, int);
      void deactivateArray()
         {heap = NULL; arrayLength = heapSize = 0;}
      void output(ostream& out) const;
   private:
      int heapSize;       // number of elements in queue
      int arrayLength;    // queue capacity + 1
      T *heap;            // element array
};

template<class T>
minHeap<T>::minHeap(int initialCapacity)
{// Constructor.
//   if (initialCapacity < 1)
//   {ostringstream s;
//    s << "Initial capacity = " << initialCapacity << " Must be > 0";
//    throw illegalParameterValue(s.str());
//   }
   arrayLength = initialCapacity + 1;
   heap = new T[arrayLength];
   heapSize = 0;
}

template<class T>
void minHeap<T>::push(const T& theElement)
{// Add theElement to heap.

   // increase array length if necessary
   if (heapSize == arrayLength - 1)
   {// double array length
      changeLength1D(heap, arrayLength, 2 * arrayLength);
      arrayLength *= 2;
   }

   // find place for theElement
   // currentNode starts at new leaf and moves up tree
   int currentNode = ++heapSize;
   while (currentNode != 1 && heap[currentNode / 2] > theElement)
   {
      // cannot put theElement in heap[currentNode]
      heap[currentNode] = heap[currentNode / 2]; // move element down
      currentNode /= 2;                          // move to parent
   }

   heap[currentNode] = theElement;
}

template<class T>
void minHeap<T>::pop()
{// Remove max element.
   // if heap is empty return null
//   if (heapSize == 0)   // heap empty
//      throw queueEmpty();

   // Delete min element
   heap[1].~T();

   // Remove last element and reheapify
   T lastElement = heap[heapSize--];

   // find place for lastElement starting at root
   int currentNode = 1,
       child = 2;     // child of currentNode
   while (child <= heapSize)
   {
      // heap[child] should be smaller child of currentNode
      if (child < heapSize && heap[child] > heap[child + 1])
         child++;

      // can we put lastElement in heap[currentNode]?
      if (lastElement <= heap[child])
         break;   // yes

      // no
      heap[currentNode] = heap[child]; // move child up
      currentNode = child;             // move down a level
      child *= 2;
   }
   heap[currentNode] = lastElement;
}

template<class T>
void minHeap<T>::initialize(T *theHeap, int theSize)
{// Initialize max heap to element array theHeap[1:theSize].
   delete [] heap;
   heap = theHeap;
   heapSize = theSize;

   // heapify
   for (int root = heapSize / 2; root >= 1; root--)
   {
      T rootElement = heap[root];

      // find place to put rootElement
      int child = 2 * root; // parent of child is target
                            // location for rootElement
      while (child <= heapSize)
      {
         // heap[child] should be smaller sibling
         if (child < heapSize && heap[child] > heap[child + 1])
            child++;

         // can we put rootElement in heap[child/2]?
         if (rootElement <= heap[child])
            break;  // yes

         // no
         heap[child / 2] = heap[child]; // move child up
         child *= 2;                    // move down a level
      }
      heap[child / 2] = rootElement;
   }
}

template<class T>
void minHeap<T>::output(ostream& out) const
{// Put the array into the stream out.
   copy(heap + 1, heap + heapSize + 1, ostream_iterator<T>(cout, "  "));
}

// overload <<
template <class T>
ostream& operator<<(ostream& out, const minHeap<T>& x)
   {x.output(out); return out;}
   

template<class T>
struct huffmanNode {
	linkedBinaryTree<int> *tree;
	T weight;
	
	operator T () const { return weight; }
}; 

//int Ans = 0;

template <class T>
linkedBinaryTree<int>* huffmanTree(T weight[], int n, bool op)
{
	static int Ans = 0; 
	if (op) {
		cout << Ans << '\n';
		return NULL;
	}
	
// Generate Huffman tree with weights weight[1:n], n >= 1.
   // create an array of single node trees
   huffmanNode<T> *hNode = new huffmanNode<T> [n + 1];
   linkedBinaryTree<int> emptyTree;
   for (int i = 1; i <= n; i++)
   {
      hNode[i].weight = weight[i];
      hNode[i].tree = new linkedBinaryTree<int>;
      hNode[i].tree->makeTree(i, emptyTree, emptyTree);
   }

   // make node array into a min heap
   minHeap<huffmanNode<T> > heap(1);
   heap.initialize(hNode, n);

   // repeatedly combine trees from min heap
   // until only one tree remains
   huffmanNode<T> w, x, y;
   linkedBinaryTree<int> *z;
   for (int i = 1; i < n; i++)
   {
      // remove two lightest trees from the min heap
      x = heap.top(); heap.pop();
      y = heap.top(); heap.pop();

      // combine into a single tree
      z = new linkedBinaryTree<int>;
      z->makeTree(0, *x.tree, *y.tree);
      w.weight = x.weight + y.weight;
      Ans += w.weight;
//      cout << Ans << '\n';
      w.tree = z;
      heap.push(w);
      delete x.tree;
      delete y.tree;
   }

   // destructor for min heap deletes hNode
   return heap.top().tree;
}


void solve() {
	string s; cin >> s;
	int len = s.length();
	
//	if (len == 1) {
//		cout << "1\n";
//		return;
//	}
	
	int *cnt = new int[27];
	int *w = new int[27];
	
//	memset(cnt, 0, cnt + 27);	// 1 - 26
	for (int i = 1; i < 27; i++) cnt[i] = 0;
	
	for (int i = 0; i < len; i++) {
		cnt[s[i] - 'a' + 1] ++;
	}
	
	// 离散化
	int one = 1;
	for (int i = 1; i < 27; i++) {
		if (cnt[i] != 0) {
			w[one] = cnt[i];
			one ++;
		}
	}	 
	
	// 不发生合并 
	if (one == 2) {
		cout << len << '\n';
		return;
	}
	
	// 建立含 26 个小写字母节点的霍夫曼树 kill
	
	huffmanTree(w, one - 1, 0);
	
	huffmanTree(w, one - 1, 1);
}


int main(){
	solve(); 
	return 0;
}

