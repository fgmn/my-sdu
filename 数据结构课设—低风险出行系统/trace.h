#ifndef TRACE_H
#define TRACE_H


#include<item.h>
#include<list>
using namespace std;


struct trace
{
    trace():E(false), totRisk(1e10), totTime(0) {}
    bool E;
    double totRisk;
    int totTime;
    list<item> routine;
};

#endif // TRACE_H
