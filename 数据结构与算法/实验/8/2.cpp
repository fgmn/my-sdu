#include<bits/stdc++.h>
using namespace std;

template <class K> class Hash;

// "Hash.h"
template<>
class Hash<string>
{
   public:
      size_t operator()(const string theKey) const
      {// Convert theKey to a nonnegative integer.
         unsigned long HashValue = 0; 
         int length = (int) theKey.length();
         for (int i = 0; i < length; i++)
            HashValue = 5 * HashValue + theKey.at(i);
    
         return size_t(HashValue);
      }
};

template<>
class Hash<int>
{
   public:
      size_t operator()(const int theKey) const
      {return size_t(theKey);}
};

template<>
class Hash<long>
{
   public:
      size_t operator()(const long theKey) const
      {return size_t(theKey);}
};


// "dictionary.h"
template<class K, class E>
class dictionary 
{
   public:
      virtual ~dictionary() {}
      virtual bool empty() const = 0;
                  // return true iff dictionary is empty
      virtual int size() const = 0;
                  // return number of pairs in dictionary
      virtual pair<const K, E>* find(const K&) const = 0;
                  // return pointer to matching pair
      virtual void erase(const K&) = 0;
                  // remove matching pair
      virtual void insert(const pair<const K, E>&) = 0;
                  // insert a (key, value) pair into the dictionary
};


template <class K, class E>
struct pairNode 
{
   typedef pair<const K, E> pairType;
   pairType element;
   pairNode<K,E> *next;

   pairNode(const pairType& thePair):element(thePair){}
   pairNode(const pairType& thePair, pairNode<K,E>* theNext)
            :element(thePair){next = theNext;}
};


//"sortedChain.h"   
template<class K, class E>
class sortedChain : public dictionary<K,E> 
{
   public:
      sortedChain() {firstNode = NULL; dSize = 0;}
      ~sortedChain();

      bool empty() const {return dSize == 0;}
      int size() const {return dSize;}
      pair<const K, E>* find(const K&) const;
      void erase(const K&);
      void insert(const pair<const K, E>&);
      void output(ostream& out) const;

   protected:
      pairNode<K,E>* firstNode;  // pointer to first node in chain
      int dSize;                 // number of elements in dictionary
};

template<class K, class E>
sortedChain<K,E>::~sortedChain()
{// Destructor.  Delete all nodes.
   while (firstNode != NULL)
   {// delete firstNode
      pairNode<K,E>* nextNode = firstNode->next;
      delete firstNode;
      firstNode = nextNode;
   }
}

template<class K, class E>
pair<const K,E>* sortedChain<K,E>::find(const K& theKey) const
{// Return pointer to matching pair.
 // Return NULL if no matching pair.
   pairNode<K,E>* currentNode = firstNode;

   // search for match with theKey
   while (currentNode != NULL && 
          currentNode->element.first != theKey)
      currentNode = currentNode->next;

   // verify match
   if (currentNode != NULL && currentNode->element.first == theKey) {
      // yes, found match
      cout << this -> dSize << '\n';
      return &currentNode->element;   	
   }


   // no match
   cout << "Not Found\n";
   return NULL;
}

template<class K, class E>
void sortedChain<K,E>::insert(const pair<const K, E>& thePair)
{// Insert thePair into the dictionary. Overwrite existing
 // pair, if any, with same key.
   pairNode<K,E> *p = firstNode,
                *tp = NULL; // tp trails p

   // move tp so that thePair can be inserted after tp
   while (p != NULL && p->element.first < thePair.first)
   {
      tp = p;
      p = p->next;
   }

   // check if there is a matching pair
   if (p != NULL && p->element.first == thePair.first)
   {// replace old value
   		cout << "Existed\n";
//      p->element.second = thePair.second;
      return;
   }

   // no match, set up node for thePair
   pairNode<K,E> *newNode = new pairNode<K,E>(thePair, p);

   // insert newNode just after tp
   if (tp == NULL) firstNode = newNode;
   else tp->next = newNode;

   dSize++;
   return;
}

template<class K, class E>
void sortedChain<K,E>::erase(const K& theKey)
{// Delete the pair, if any, whose key equals theKey.
   pairNode<K,E> *p = firstNode,
                *tp = NULL; // tp trails p
   
   // search for match with theKey
   while (p != NULL && p->element.first < theKey)
   {
      tp = p;
      p = p->next;
   }

   // verify match
   if (p != NULL && p->element.first == theKey)
   {// found a match
      // remove p from the chain
      if (tp == NULL) firstNode = p->next;  // p is first node
      else tp->next = p->next;

      delete p;
      dSize--;
      cout << dSize << '\n';
   }
   else cout << "Delete Failed\n";
}

template<class K, class E>
void sortedChain<K,E>::output(ostream& out) const
{// Insert the chain elements into the stream out.
   for (pairNode<K,E>* currentNode = firstNode;
                       currentNode != NULL;
                       currentNode = currentNode->next)
      out << currentNode->element.first << " "
          << currentNode->element.second << "  ";
}

// overload <<
template <class K, class E>
ostream& operator<<(ostream& out, const sortedChain<K,E>& x)
   {x.output(out); return out;}


template<class K, class E>
class HashChains : public dictionary<K,E>
{
   public:
      HashChains(int theDivisor = 11)
      {
         divisor = theDivisor;
         dSize = 0;
      
         // allocate and initialize Hash table array
         table = new sortedChain<K,E> [divisor];
      }

      ~HashChains(){delete [] table;}

      bool empty() const {return dSize == 0;}
      int size() const {return dSize;}

      pair<const K, E>* find(const K& theKey) const
         {return table[Hash_(theKey) % divisor].find(theKey);}	// find in sortedChain

      void insert(const pair<const K, E>& thePair)
      {
         int homeBucket = (int) Hash_(thePair.first) % divisor; // distributed bucket
         int homeSize = table[homeBucket].size();
         table[homeBucket].insert(thePair);		// insert into sortedChain
         if (table[homeBucket].size() > homeSize)
            dSize++;
      }

      void erase(const K& theKey)
         {table[Hash_(theKey) % divisor].erase(theKey);}

      void output(ostream& out) const	// out ?!!
      {
         for (int i = 0; i < divisor; i++)
            if (table[i].size() == 0)
               cout << "NULL" << endl;
            else
               cout << table[i] << endl;
      }


   protected:
      sortedChain<K, E>* table;  // Hash table
      Hash<K> Hash_;              // maps type K to nonnegative integer
      int dSize;                 // number of elements in list
      int divisor;               // Hash function divisor
};


// overload <<
template <class K, class E>
ostream& operator<<(ostream& out, const HashChains<K,E>& x)
   {x.output(out); return out;}
   


int main(){

	
	int D, m;
	cin >> D >> m;
	
	HashChains<int, int> obj(D);
	pair<int, int> p;
	p.second = 0;
		
	for (int i = 1, opt, x; i <= m; i++) {
		cin >> opt >> x;
		
		if (opt == 0) {
			p.first = x;
			obj.insert(p);
		}
		else if (opt == 1) {
			obj.find(x);
		}
		else if (opt == 2) {
			obj.erase(x);
		}
	}
	return 0;
}


/*

1 Mary
2 Ken
3 Kitty

*/


