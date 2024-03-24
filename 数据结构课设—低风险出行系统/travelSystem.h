#ifndef TRAVELSYSTEM_H
#define TRAVELSYSTEM_H

#include<QString>
#include<city.h>
#include<map>
using namespace std;

class travelSystem {
public:
    travelSystem();

    void setLimit(int t);
    void setTraveller(int id);
    void init();

    int ID(int id);                     // 创建新内部序号，或者查询内部序号
    int query(int rid, item &it);       // 通过内部序号查询旅客状态
    QString qState(int id, int t);      // 查询旅客精确状态

    void savePlan(int r);                    // 持久化旅客出行计划
    void updatePlan(int t);                  // 更新旅客状态
    QString outputPlan();                    // 将推荐路线输出至QString

    int getSize();
    city* getNodes();
    list<item> getRoutine();
    list<item> getTravel(int id);

    void dfs(int i, int s, int t, int stime, double r, int cost, int vis);         // 最小风险策略
    void dfs1(int i, int s, int t, int stime, double r, int cost, int vis);        // 限时最小风险策略
//    trace dfs2(int i, int s, int t, int stime, double r, int cost, int vis);     // 最小风险策略记忆化搜索

    int Dur(int a, int b);

private:
    // 静态变量
    const double rLevel[3] = {0.2, 0.5, 0.9};
    const static int N = 32;
    const QString type[3] = {"car", "train", "plane"};

    // 图
    int n;
    city gCity[N];

    bool reach[N][N];

    // 旅行计划
    int traveller;
    double totRisk;
    int totTime, timeLimit;
    list<item> routine;

    list<item> rt;

    // 系统中是否允许有多个旅客? 允许
    // 持久化routine
    // 旅客状态记录
    map<int, int> mp;   // 映射id
    list<item> travel[15];
    int cnt;
    int clk[15];        //  行程沙漏值
    int wait[15];       //  等待沙漏值

    // 初始化子操作
    void readCity();
    void readItem();
    void floyd();       // floyd求传递闭包：判断城市之间是否可达
    void printReach();
};

#endif // TRAVELSYSTEM_H
