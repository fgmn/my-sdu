
#include<iostream>
#include<fstream>
#include<QDebug>
#include<travelSystem.h>
#define RD 0

travelSystem::travelSystem() {
    readCity();
    readItem();
    floyd();
    if (RD) printReach();
}

void travelSystem::readCity() {
    int i, x, y, r;

    if (RD) cout << "\nReading city.in ...\n";
    qDebug() << "\nReading city.in ...\n";

    ifstream f("city.in", ios::in);
    while (f >> i >> r >> x >> y) {
        if (RD) cout << i << ' ' << r << ' ' << x << ' ' << y << '\n';
        qDebug() << i << ' ' << r << ' ' << x << ' ' << y;
        gCity[i].seq = i;
        gCity[i].r = r;
        gCity[i].x = x;
        gCity[i].y = y;
    }

    f.close();
}

void travelSystem::readItem() {
    int i, dCity, type, sTime, time;

    if (RD) cout << "\nReading items.in ...\n";

    ifstream f("items.in", ios::in);
    while (f >> i >> dCity >> type >> sTime >> time) {
        if (RD) cout << i << ' ' << dCity << ' ' << type << ' ' << sTime << ' ' << time << '\n';
        qDebug() << i << ' ' << dCity << ' ' << type << ' ' << sTime << ' ' << time;
        item newItem;
        newItem.sCity = i;
        newItem.dCity = dCity;
        newItem.type = type;
        newItem.sTime = sTime;
        newItem.time = time;
        gCity[i].routeTable.push_front(newItem);

        reach[i][dCity] = true;
    }
    f.close();
}

void travelSystem::floyd() {
    for (int k = 1; k <= n; k++) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                reach[i][j] |= (reach[i][k] & reach[k][j]);
            }
        }
    }
}

void travelSystem::printReach() {
    cout << '\n';
    cout << "Accessibility between cities is as follows:\n";
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            cout << reach[i][j] << ' ';
        }
        cout << '\n';
    }
}

void travelSystem::setLimit(int t) {
    timeLimit = t;
}

void travelSystem::setTraveller(int id) {
    traveller = id;
}

void travelSystem::init() {
    totRisk = 1e10;
    totTime = 0;
    routine.clear();
    rt.clear();
}

int travelSystem::ID(int id) {
    if (mp[id]) return mp[id];
    if (cnt == 10) cnt = 0;
    mp[id] = ++cnt;
    return -1;  // 返回-1代表首次创建
}

int travelSystem::query(int rid, item &it) {
    if (travel[rid].empty()) return -1;
    it = *travel[rid].begin();
    return 0;
}

QString travelSystem::qState(int id, int t) {
    QString ret;
    if (mp[id] == 0) {
        ret = "to forgive!";
        return ret;
    }
    int rid = mp[id];
    if (travel[rid].empty()) {
        ret = "He has completed the journey.";
        return ret;
    }

    auto it = travel[rid].begin();
    if (wait[rid] > t) {
        ret += "He is waiting for " + type[it->type] + " from city " + QString::number(it->sCity) + " to city " + QString::number(it->dCity);
        ret += " at clock " + QString::number(it->sTime) + ".";
    }
    else {
        ret += "He is on " + type[it->type] + " from city " + QString::number(it->sCity) + " to city " + QString::number(it->dCity) + ".";
    }
    return ret;
}

void travelSystem::savePlan(int t) {
    int rid = mp[traveller];
    travel[rid] = routine;
    auto it = travel[rid].begin();
    wait[rid] = t + Dur(t % 24, it->sTime);
    clk[rid] = wait[rid] + it->time;
}

void travelSystem::updatePlan(int t) {
//    qDebug() << t;  // t为绝对时间
    for (int i = 1; i <= cnt; i++) {
        if (travel[i].empty()) continue;
        auto it = travel[i].begin();

        // 初始化以及更新时设置沙漏值
//        qDebug() << i << ": " << it->type << ' ' << it->sCity << ' ' << it->dCity << ' ' << it->sTime << ' ' << it->time;

        if (clk[i] <= t) {
            travel[i].pop_front();
            if (travel[i].empty()) continue;
            it = travel[i].begin();

            wait[i] = t + Dur(t % 24, it->sTime);
            clk[i] = wait[i] + it->time;
        }
    }
}

QString travelSystem::outputPlan() {
    QString plan;
    plan += "traveller: " + QString::number(traveller) + "\n";
    plan = "total risk: " + QString::number(totRisk) + "\n";
    plan += "total time: " + QString::number(totTime) + "\n";

    list<item>::iterator it, end;
    it = routine.begin();
    end = routine.end();
    plan += "routine: \n";
    while (it != end) {
        plan += "-> " + type[it->type] + " from city " + QString::number(it->sCity) + " to city ";
        plan += QString::number(it->dCity) + " at clock " + QString::number(it->sTime) + " cost " + QString::number(it->time) + " hours \n";
        it ++;
    }

    return plan;
}

int travelSystem::getSize() { return n; }

city* travelSystem::getNodes() { return gCity; }

list<item> travelSystem::getRoutine() { return routine; }

list<item> travelSystem::getTravel(int id) {
    int rid = mp[id];
    return travel[rid];
}

int travelSystem::Dur(int a, int b) {
   if (a < b) return b - a;
   return b + 24 - a;
}

void travelSystem::dfs(int i, int s, int t, int stime, double r, int cost, int vis) {
//    qDebug() << rt.size();
//    qDebug() << i << ' ' << s << ' ' << t << ' ' << stime << ' ' << r << ' ' << cost << ' ' << vis;
    if (s == t) {
        if (totRisk > r) {
            totRisk = r;
            totTime = cost;
            routine = rt;
        }
        return;
    }

    for (auto it : gCity[s].routeTable) {
        // 判断是否经过该城市
        if (vis & (1 << (it.dCity - 1))) {
            continue;
        }

        int duration = Dur(stime % 24, it.sTime);
        double dr = duration * rLevel[gCity[s].r];
        int dcost = duration + it.time;

        if (r + dr > totRisk) continue;

        vis ^= (1 << (it.dCity - 1));
        rt.push_back(it);
        dfs(i, it.dCity, t, stime + dcost, r + dr, cost + dcost, vis);
        vis ^= (1 << (it.dCity - 1));
        rt.pop_back();
    }
}

void travelSystem::dfs1(int i, int s, int t, int stime, double r, int cost, int vis) {
    if (cost < timeLimit && s == t) {
        if (totRisk > r) {
            totRisk = r;
            totTime = cost;
            routine = rt;
        }
        return;
    }

    list<item>::iterator it, end;
    it = gCity[s].routeTable.begin();
    end = gCity[s].routeTable.end();

    for (; it != end; it++) {
        if (vis & (1 << (it->dCity - 1))) {
            continue;
        }

        int duration = Dur(stime % 24, it->sTime);
        double dr = duration * rLevel[gCity[s].r];
        int dcost = duration + it->time;

        if (r + dr > totRisk) continue;
        // 增加时间限制
        if (cost + dcost > timeLimit) continue;

        vis ^= (1 << (it->dCity - 1));
        rt.push_front(*it);
        dfs1(i, it->dCity, t, stime + dcost, r + dr, cost + dcost, vis);
        vis ^= (1 << (it->dCity - 1));
        rt.pop_front();
    }
}
