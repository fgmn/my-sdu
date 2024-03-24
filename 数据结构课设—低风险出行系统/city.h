#ifndef CITY_H
#define CITY_H

#include<list>
#include<item.h>
using namespace std;

struct city
{
    int seq;                // 城市序号
    int r;                  // 风险等级
    int x, y;               // 位置
    list<item> routeTable;  // 时刻表（路由表）
};

#endif // CITY_H
