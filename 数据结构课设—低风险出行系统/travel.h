#ifndef TRAVEL_H
#define TRAVEL_H

#include<item.h>
#include<list>
using namespace std;

struct travel
{
    int seq;            // 旅行序号
    int str;            // 策略
    int sCity, dCity;   // 出发地，目的地
    int sTime;          // 申请时间
    double totRisk;     // 总风险
    int totTime;        // 总时间
    list<item> routine; // 规划路线
};

#endif // TRAVEL_H
