#include<bits/stdc++.h>
using namespace std;

/* 1.hash -> Hash
*/
 
template <class K> class Hash;

template<>
class Hash<string>
{
   public:
      size_t operator()(const string theKey) const
      {// Convert theKey to a nonnegative integer.
         unsigned long hashValue = 0; 
         int length = (int) theKey.length();
         for (int i = 0; i < length; i++)
            hashValue = 5 * hashValue + theKey.at(i);
    
         return size_t(hashValue);
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

template<class K, class E>
class hashTable
{
   public:
      hashTable(int theDivisor = 11);
      ~hashTable(){delete [] table;}

      bool empty() const {return dSize == 0;}
      int size() const {return dSize;}
      pair<const K, E>* find(const K&) const;
      void insert(const pair<const K, E>&);
      void output(ostream& out) const;
      
		void del(const K&);
		
   protected:
      int search(const K&) const;
      pair<const K, E>** table;  // hash table
      hash<K> Hash;              // maps type K to nonnegative integer
      int dSize;                 // number of pairs in dictionary
      int divisor;               // hash function divisor
};

template<class K, class E>
hashTable<K,E>::hashTable(int theDivisor)
{
   divisor = theDivisor;
   dSize = 0;

   // allocate and initialize hash table array
   table = new pair<const K, E>* [divisor];
   for (int i = 0; i < divisor; i++)
      table[i] = NULL;
}

template<class K, class E>
int hashTable<K,E>::search(const K& theKey) const
{// Search an open addressed hash table for a pair with key theKey.
 // Return location of matching pair if found, otherwise return
 // location where a pair with key theKey may be inserted
 // provided the hash table is not full.

   int i = (int) Hash(theKey) % divisor;  // home bucket
   int j = i;    // start at home bucket
   do
   {
      if (table[j] == NULL || table[j]->first == theKey)
         return j;
      j = (j + 1) % divisor;  // next bucket
   } while (j != i);          // returned to home bucket?

   return j;  // table full
}

template<class K, class E>
pair<const K,E>* hashTable<K,E>::find(const K& theKey) const
{// Return pointer to matching pair.
 // Return NULL if no matching pair.
   // search the table
   int b = search(theKey);

   // see if a match was found at table[b]
   if (table[b] == NULL || table[b]->first != theKey) {
   	cout << "-1\n";
   	return NULL;           // no match
   }
      
	cout << b << '\n';
   return table[b];  // matching pair
}

template<class K, class E>
void hashTable<K,E>::insert(const pair<const K, E>& thePair)
{// Insert thePair into the dictionary. Overwrite existing
 // pair, if any, with same key.
 // Throw hashTableFull exception in case table is full.
   // search the table for a matching pair
   int b = search(thePair.first);

   // check if matching pair found
   if (table[b] == NULL)
   {
      // no matching pair and table not full
      table[b] = new pair<const K,E> (thePair);
      dSize++;
      cout << b << '\n';
   }
   else
   {// check if duplicate or table full
      if (table[b]->first == thePair.first)
      {// duplicate, change table[b]->second
//         table[b]->second = thePair.second;
			cout << "Existed\n";
      }
//      else // table is full
//         throw hashTableFull();
   }
}

template<class K, class E>
void hashTable<K,E>::output(ostream& out) const
{// Insert the hash table into the stream out.
   for (int i = 0; i < divisor; i++)
      if (table[i] == NULL)
         cout << "NULL ";// << endl;
      else
         cout << table[i]->first << " ";
//              << table[i]->second << endl;
}

// overload <<
template <class K, class E>
ostream& operator<<(ostream& out, const hashTable<K,E>& x)
   {x.output(out); return out;}


template<class K, class E>
void hashTable<K,E>::del(const K& tar) {
	int b = search(tar);
	if (table[b] == NULL || table[b]->first != tar){
		cout << "Not Found\n";
		return;
	}
	delete table[b];
	dSize--;
	int pre = b, cnt = 0;
	
	for (int i = 1; i < dSize; i++) {
		b = (b + 1) % divisor;
		
		if (table[b] == NULL) break;
		else {
			int ini = Hash(table[b]->first) % divisor;
			if (ini == b) continue;
			else if ((ini < b && b < pre) || (b < pre && pre < ini) || (pre < ini && ini < b)) {
				continue;
			}
			else {
				table[pre] = table[b];
				pre = b;
				cnt++;
			}
		}
	}
	table[pre] = NULL;
	
	cout << cnt << '\n';
}

	
int main(){
	
//	freopen("out.txt", "w", stdout);
	
	int D, m; cin >> D >> m;
	hashTable<int, int> z(D); 
	pair<int, int> p;
	p.second = 0;
	
	for (int i = 1, opt, d; i <= m; i++) {
		cin >> opt >> d;
		p.first = d;
		if (opt == 0) {	
			z.insert(p);
		}
		else if (opt == 1) {
			z.find(p.first);
		}
		else if (opt == 2) {
			z.del(p.first);
		}
//		cout << z << '\n';
	}
	
	return 0;
}

