#include<bits/stdc++.h>
#include "LoserTree.h"
using namespace std;

#define DEBUG 0

template<class T>
void LoserTree<T>::initialize(T *thePlayer, int theNumberOfPlayers) {
    int n = theNumberOfPlayers;
    if (n < 2) {
        return;
    }
    player = thePlayer;
    numberOfPlayers = n;
    delete [] tree;
    delete [] Winners;
    tree = new int [n];
    Winners = new int [n];

    int i, s;
    // s = 2 ^ (log(n - 1)) && s % 2 == 0
    for (s = 1; 2 * s <= n - 1; s += s);

    lowExt = 2 * (n - s);
    offset = 2 * s - 1;

    for (i = 2; i <= lowExt; i += 2)
        play((offset + i) / 2, i - 1, i);

    if (n % 2 == 1) {
        if (DEBUG) cout << "play:(" << n / 2 << ", " << tree[n - 1] << ", " << lowExt + 1 << ")\n"; 
        play(n / 2, Winners[n - 1], lowExt + 1);    // FIX
        i = lowExt + 3;
    } 
    else i = lowExt + 2;

    for (; i <= n; i += 2)
        play((i - lowExt + n - 1) / 2, i - 1, i);

    tree[0] = Winners[1];
    delete [] Winners;
}

template<class T>
void LoserTree<T>::play(int p, int leftChild, int rightChild) {
    if (player[leftChild] >=  player[rightChild]) {
        tree[p] = leftChild;
        Winners[p] = rightChild;
    }
    else {
        tree[p] = rightChild;
        Winners[p] = leftChild;
    }

    while (p % 2 == 1 && p > 1) {    // *
        if (player[Winners[p - 1]] >= player[Winners[p]]) {
            tree[p / 2] = Winners[p - 1];
            Winners[p / 2] = Winners[p];
        }
        else {
            tree[p / 2] = Winners[p];
            Winners[p / 2] = Winners[p - 1];
        }
        p /= 2;
    }
}

template<class T>
void LoserTree<T>::replay(int thePlayer, T newValue) {
    // 当前只考虑了replay当前赢者
    int n = numberOfPlayers;
    if (thePlayer <= 0 || thePlayer > n) {
        return;
    }

    player[thePlayer] = newValue;

    int matchNode; // matchNode： 下一个进行比赛的节点
    Winner = thePlayer;

    // thePlayer的父节点为第1个比赛点
    if (thePlayer <= lowExt) matchNode = (offset + thePlayer) / 2;
    else matchNode = (thePlayer - lowExt + n - 1) / 2;

    for (; matchNode >= 1; matchNode /= 2) {
        if (player[tree[matchNode]] >= player[Winner]) {
        }
        else {
            swap(tree[matchNode], Winner);
        }
        if (matchNode == 1) tree[0] = Winner;
    }

}

template<class T>
void LoserTree<T>::output() const {
    for (int i = 1; i < numberOfPlayers; i++)
        cout << tree[i] << ' ';
    cout << '\n';
}
