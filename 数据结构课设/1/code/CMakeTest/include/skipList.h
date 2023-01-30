#ifndef skipList_
#define skipList_

#include<bits/stdc++.h>
#include "skipNode.h"
#define null NULL
using namespace std;

template <class K, class E>
class skipList
{
    public:
        skipList(K, int, float);
        pair<K, E>* find(const K& theKey) const;                // O(logn)
        int level() const;
        skipNode<K, E>* search(const K& theKey) const;          // O(logn)
        void insert(const pair<K, E>& thePair);                 // O(logn)
        void erase(const K& theKey);                            // O(logn)
        // 链表成员类iterator
        class iterator
        {
            public:
                // 构造函数
                iterator(skipNode<K, E>* theNode = null) { node = theNode; }

                // 解引用操作符
                pair<K, E>& operator*() const { return node->ele; }
                pair<K, E>* operator->() const { return &node->ele; }

                // 迭代器加法操作
                iterator& operator++() {
                    node = node->nxt[0];
                    return *this;
                }
                iterator operator++(int) {
                    iterator old = *this;
                    node = node->nxt[0];
                    return old;
                }

                // 相等检验
                bool operator!=(const iterator right) const
                    { return node != right.node; }
                bool operator==(const iterator right) const
                    { return node == right.node; }            

            protected:
                skipNode<K, E>* node;

        };

        iterator begin() { return iterator(head); }
        iterator end() { return iterator(tail); }
    private:
        float cutOff;
        int levels, dSize, maxLevel;
        K tailKey;                  // 最大关键字
        skipNode<K, E>* head;
        skipNode<K, E>* tail;
        skipNode<K, E>** last;      // last[i]表示第i层的最后节点
};

#endif