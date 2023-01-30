#ifndef skipNode_
#define skipNode_

#include<bits/stdc++.h>
using namespace std;

template <class K, class E>
struct skipNode
{
    typedef pair<K, E> pairType;

    pairType ele;
    skipNode<K, E> **nxt;
    skipNode(const pairType &thePair, int sz)
        :ele(thePair) { nxt = new skipNode<K, E>* [sz]; }
};

#endif