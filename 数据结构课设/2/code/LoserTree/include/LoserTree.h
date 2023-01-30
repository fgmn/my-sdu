
#ifndef LoserTree_
#define LoserTree_
using namespace std;

template<class T>
class LoserTree {
    public:
        LoserTree(T *thePlayer, int theNumberOfPlayers) {
            tree = NULL;
            Winners = NULL;
            initialize(thePlayer, theNumberOfPlayers);
        }
        ~LoserTree() { delete [] tree; }
        void initialize(T*, int);
        int winner() const { return tree[0]; }
        int loser(int i) const {
            return (i < numberOfPlayers && i > 0) ? tree[i] : 0;
        }
        void replay(int, T);
        void output() const;
    
    private:
        int Winner;     // 比赛赢者
        int *Winners;   // 初始化时暂存赢者
        int lowExt;     // 最底层外部节点个数
        int offset;
        int *tree;      // 内部节点[1, n - 1]
        int numberOfPlayers;
        T *player;
        void play(int, int, int);
};

#endif