#ifndef ITEM_H
#define ITEM_H

struct item
{
    /*item(int type, int sc, int dc, int st, int t):type(type), sCity(sc), dCity(dc), sTime(st), time(t) {}
    item(const item &it) {
        type = it.type;
        sCity = it.sCity;
        dCity = it.dCity;
        sTime = it.sTime;
        time = it.time;
    }*/
    int type;               // 交通工具（0：汽车 1：火车 2：飞机）
    int sCity, dCity;       // 出发地，目的地
    int sTime, time;        // 出发时间，耗时
};

#endif // ITEM_H
