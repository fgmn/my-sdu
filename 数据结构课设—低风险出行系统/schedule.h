#ifndef SCHEDULE_H
#define SCHEDULE_H


#include <iostream>
#include <QString>
#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#define citynum 10  // 城市数
#define INF INT_MAX

using namespace std;

typedef struct table table;
typedef struct City City;
typedef struct traveller travel;

struct table {
    int traffic_type;           // 交通工具类型：2->Car，5->Train，9->Plain
    int start_city, end_city;   // 起始城市和目的城市编号
    int start_time, end_time;   // 起始时刻和到达时刻
    int day;                    // 起始时处于哪一天
    table *next;
};

struct City {
    double danger;              // 该城市的风险值
    int seq;                    // 该城市的编号
    int x,y;                    // 城市当前位置
    table* arrange;         	// 城市路由表，记录各个交通工具的时刻表
};

struct traveller {
    int seq;                    // 旅客的编号
    int total_time;             // 当前旅行的总用时
    double total_danger;        // 当前旅行的总风险记录
    travel *next;               // 用在链接各个人
    table* real;                // 当前旅行的实际规划路线记录
};

class schedule
{
public:

    schedule();
    static int City_Seq(QString);
    static double Seq_Danger(int);
    static int Seq_x(int);
    static int Seq_y(int);
    static QString Seq_City(int);
    static QString Type_Vehcle(int);
    static int total(int, int, int);
    static int total(int ,int);
    static City allcity[citynum];
};

#endif // SCHEDULE_H
