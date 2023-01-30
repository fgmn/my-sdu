#include<bits/stdc++.h>
#include "skipNode.h"
#include "skipList.h"
#define null NULL
using namespace std;
using ll = long long;

const int Val = 0;

template <class K, class E>
skipList<K, E>::skipList(K largeKey, int maxPairs, float prob) 
{
    cutOff = prob * RAND_MAX;
    maxLevel = (int)ceil(logf((float)maxPairs) / logf(1 / prob)) - 1;   // 最大级数
    levels = 0;
    dSize = 0;
    tailKey = largeKey;

    pair<K, E> tailPair;
    tailPair.first = tailKey;
    head = new skipNode<K, E> (tailPair, maxLevel + 1);
    tail = new skipNode<K, E> (tailPair, 0);
    last = new skipNode<K, E> *[maxLevel + 1];

    // 链表为空时，任意级链表中的头结点均指向尾节点
    for (int i = 0; i <= maxLevel; i++)
        head->nxt[i] = tail;
}

template <class K, class E>
pair<K, E>* skipList<K, E>::find(const K& theKey) const
{
    if (theKey >= tailKey)
        return null;

    skipNode<K, E>* pre = head;

    for (int i = levels; i >= 0; i--) {
        while (pre->nxt[i]->ele.first < theKey)
            pre = pre->nxt[i];
    }

    if (pre->nxt[0]->ele.first == theKey)
        return &pre->nxt[0]->ele;

    return null;
}

template <class K, class E>
int skipList<K, E>::level() const
{
    int lev = 0;
    while (rand() <= cutOff)
        lev++;
    return (lev <= maxLevel) ? lev : maxLevel;
}

template <class K, class E>
skipNode<K, E>* skipList<K, E>::search(const K& theKey) const
{
    // 搜索theKey同时将每一级要查看的最后一个节点存至last
    skipNode<K, E>* pre = head;
    for (int i = levels; i >= 0; i--) {
        while (pre->nxt[i]->ele.first < theKey)
            pre = pre->nxt[i];
        last[i] = pre;
    }

    return pre->nxt[0];
}

template <class K, class E>
void skipList<K, E>::insert(const pair<K, E>& thePair)
{
    if (thePair.first >= tailKey) {
        // ostringstream s;
        // s << "Key = " << thePair.first << " Must be < " << tailKey;
        return;
    }

    skipNode<K, E>* theNode = search(thePair.first);
    if (theNode->ele.first == thePair.first) {
        theNode->ele.second = thePair.second;
        return;
    }

    // 不存在
    int theLevel = level();
    if (theLevel > levels) {
        theLevel = ++levels;        // 增长1级
        last[theLevel] = head;      // 最高级中最后访问的是头结点
    }

    // 在theNode之后插入新节点
    skipNode<K, E>* newNode = new skipNode<K, E>(thePair, theLevel + 1);
    for (int i = 0; i <= theLevel; i++) {
        newNode->nxt[i] = last[i]->nxt[i];
        last[i]->nxt[i] = newNode;
    }

    dSize++;
    return;
}

template <class K, class E>
void skipList<K, E>::erase(const K& theKey) {
    if (theKey >= tailKey)
        return;
    
    skipNode<K, E>* theNode = search(theKey);
    if (theNode->ele.first != theKey) 
        return;
    
    // 删除节点
    for (int i = 0; i <= levels && last[i]->nxt[i] == theNode; i++) {
        last[i]->nxt[i] = theNode->nxt[i];
    }

    while (levels > 0 && head->nxt[levels] == tail)
        levels--;
    
    delete theNode;
    dSize--;
}
