#include "schedule.h"


City schedule::allcity[citynum];

// 从文件中读取系统初始值

schedule::schedule()
{
    table *currentPtr = nullptr, *lastPtr = nullptr;
    // 带头结点的时刻链表
    for(int i=0;i<citynum;i++){
        allcity[i].seq = i;
        allcity[i].danger = Seq_Danger(i);
        allcity[i].x = Seq_x(i);
        allcity[i].y = Seq_y(i);
        lastPtr = new table;
        lastPtr->next = nullptr;
        allcity[i].arrange=lastPtr;
        lastPtr=lastPtr->next;
    }

    // 时刻表文件
    QFile file(":/icon/iconsrc/database.txt");
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"Can't open the file database.txt";
        return;
    }

    // 初始化
    QTextStream in(&file);
    QString from, to, begin, end, vehcle;
    QString begin_2,end_2;
    while(!in.atEnd()){
        in >> from >> to >> begin >> end >> vehcle;
        // Shanghai Beijing 06:00 12:00 5
        begin_2 = begin.mid(0,2);
        end_2 = end.mid(0,2);
        if(from!=""){
            //qDebug()<<from<<to<<begin<<end;
            currentPtr=new table;
            currentPtr->traffic_type = vehcle.toInt();
            currentPtr->start_city = City_Seq(from);
            currentPtr->end_city = City_Seq(to);
            currentPtr->start_time = begin_2.toInt();
            currentPtr->end_time = end_2.toInt();
            currentPtr->next = nullptr;
            lastPtr=allcity[City_Seq(from)].arrange;
            //qDebug() << currentPtr->start_city << currentPtr->end_city << currentPtr->start_time << currentPtr->end_time << currentPtr->traffic_type;
            while(lastPtr->next!=nullptr)
                lastPtr=lastPtr->next;
            lastPtr->next = currentPtr;
            while(lastPtr!=nullptr)
                lastPtr=lastPtr->next;
        }
    }
    file.close();
}

// 将城市字符串对应编号
int schedule::City_Seq(QString city){
    int num = -1;
    if(city == "Shanghai")
        num = 0;
    else if(city == "Beijing")
        num = 1;
    else if(city == "Cangzhou")
        num = 2;
    else if(city == "Tianjin")
        num = 3;
    else if(city == "Lanzhou")
        num = 4;
    else if(city == "Wulumuqi")
        num = 5;
    else if(city == "Lasa")
        num = 6;
    else if(city == "Nanjing")
        num = 7;
    else if(city == "Changchun")
        num = 8;
    else if(city == "Chengdu")
        num = 9;
    return num;
}

// 将编号对应风险值
double schedule::Seq_Danger(int seq){
    double danger = 0;
    if(seq == 0 || seq == 1 || seq == 3)
        danger = 0.9;
    else if(seq == 7 || seq == 8 || seq == 9)
        danger = 0.5;
    else
        danger = 0.2;
    return danger;
}

// 将编号对应x坐标
int schedule::Seq_x(int seq){
    int x=0;
    switch(seq){
    case 0:x=648;break;
    case 1:x=561;break;
    case 2:x=548;break;
    case 3:x=577;break;
    case 4:x=388;break;
    case 5:x=204;break;
    case 6:x=228;break;
    case 7:x=608;break;
    case 8:x=672;break;
    case 9:x=411;break;
    default:break;
    }
    return x;
}
// 将编号对应y坐标
int schedule::Seq_y(int seq){
    int y=0;
    switch(seq){
    case 0:y=372;break;
    case 1:y=226;break;
    case 2:y=263;break;
    case 3:y=247;break;
    case 4:y=274;break;
    case 5:y=141;break;
    case 6:y=378;break;
    case 7:y=368;break;
    case 8:y=154;break;
    case 9:y=393;break;
    default:break;
    }
    return y;
}

// 将编号对应城市字符串
QString schedule::Seq_City(int num){
    QString city;
    switch(num){
    case 0:city = "Shanghai";break;
    case 1:city = "Beijing";break;
    case 2:city = "Cangzhou";break;
    case 3:city = "Tianjin";break;
    case 4:city = "Lanzhou";break;
    case 5:city = "Wulumuqi";break;
    case 6:city = "Lasa";break;
    case 7:city = "Nanjing";break;
    case 8:city = "Changchun";break;
    case 9:city = "Chengdu";break;
    default:break;
    }
    return city;
}

// 将交通工具数值对应交通工具字符串
QString schedule::Type_Vehcle(int a){
    QString veh;
    switch(a){
    case 2:veh = "Car";break;
    case 5:veh = "Train";break;
    case 9:veh = "Plain";break;
    default:break;
    }
    return veh;
}

// 计算当前已过的总分钟
int schedule::total(int day, int hour){
    return (day * 24 + hour) * 60;
}

int schedule::total(int day, int hour, int min){
    return (day * 24 + hour) * 60 + min;
}
