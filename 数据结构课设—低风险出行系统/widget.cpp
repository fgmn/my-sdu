#include "widget.h"
#include "ui_widget.h"
#include<travelSystem.h>
#include<fstream>
//#include <QWidget>
#define RD 0    // 读入检查

travelSystem mtravelSystem;

static int curTime = 0;
bool en = true;

Widget::Widget(QWidget *parent):
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);  // 组件设置、信号与槽的关联

    initUI();
    initConnect();

    int n = mtravelSystem.getSize();
    // 待渲染信息： 位置、风险等级、城市序号
    city* nodes = mtravelSystem.getNodes();

    // 渲染窗口初始化
    mGW = new GraphWidget(this, n, QSize(1080, 600));
    mGW->Set(nodes);
}

Widget::~Widget()
{
    delete ui;
}

// 初始化窗口界面
void Widget::initUI() {
    this->setFixedSize(1065, 600);
    this->setWindowTitle("低风险出行系统");
    QIcon exeIcon(":/icon/iconsrc/cute.png");
    this->setWindowIcon(exeIcon);

    ui->id_edit->setEnabled(false);
    ui->strategy_box->setEnabled(false);
    ui->departure_edit->setEnabled(false);
    ui->destination_edit->setEnabled(false);
    ui->time_edit->setEnabled(false);
}

// 初始化信号与槽（将前端交互设计与响应函数绑定）
void Widget::initConnect() {
    connect(ui->book_btn, SIGNAL(clicked()), this, SLOT(book()));
    connect(ui->sumbit_btn, SIGNAL(clicked()), this, SLOT(sumbit()));
    connect(ui->query_btn, SIGNAL(clicked()), this, SLOT(query()));

    // 系统时间模块
    QTimer *ctimer = new QTimer(this);
    ctimer->start(200);   // 系统时间模拟0.2s过去1h
    connect(ctimer, &QTimer::timeout, [=](){
        curTime++;

        mtravelSystem.updatePlan(curTime);

        QString time = QString("第 %1 天 %2 点").arg(curTime / 24 + 1).arg(curTime % 24);
        ui->current_time_display->setText(time);
        update();
    });


    // 有鼠标键盘输入时系统不计时
    connect(ui->book_btn, &QPushButton::clicked, [=](){
        ctimer->stop();
        en = false;
    });
    connect(ui->query_btn, &QPushButton::clicked, [=](){
        ctimer->stop();
        en = false;
    });
    connect(ui->enable_btn, &QPushButton::clicked, [=](){
        if (en == false) {
            en = true;
            ctimer->start();
        }
        else {
            en = false;
            ctimer->stop();
        }
    });
}

// 绘图
void Widget::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    QPixmap pix;
    pix.load(":/icon/iconsrc/background.png");

    qreal width = pix.width();
    qreal height = pix.height();

    pix = pix.scaled(width * 1.8, height * 1.4, Qt::KeepAspectRatio);
    painter.drawPixmap(0, 0, pix);
}

void Widget::book() {
    ui->id_edit->setEnabled(true);
    ui->strategy_box->setEnabled(true);
    ui->departure_edit->setEnabled(true);
    ui->destination_edit->setEnabled(true);
    ui->time_edit->setEnabled(true);
}

void Widget::sumbit() {
    ui->id_edit->setEnabled(false);
    ui->strategy_box->setEnabled(false);
    ui->departure_edit->setEnabled(false);
    ui->destination_edit->setEnabled(false);
    ui->time_edit->setEnabled(false);

    int id = ui->id_edit->document()->toPlainText().toInt();
    int str = ui->strategy_box->currentIndex();
    int depart = ui->departure_edit->document()->toPlainText().toInt();
    int dest = ui->destination_edit->document()->toPlainText().toInt();
    int time = ui->time_edit->document()->toPlainText().toInt();

    int c1 = mtravelSystem.ID(id);
    qDebug() << "c1 = " << c1;
    int c2;
    item it;
    if (c1 != -1) {
        // 旅客状态查询(通过内部序列号)
        c2 = mtravelSystem.query(c1, it);
        qDebug() << "c2 = " << c2;
        if (c2 != -1) {
            QMessageBox::information(this, "FAILED", "this traveller has an unfinished journey!");
            return;
        }
    }

    mtravelSystem.setLimit(time);
    mtravelSystem.setTraveller(id);
    qDebug() << id << ' ' << str << ' ' << depart << ' ' << dest << ' ' << time;

    if (str) {
        mtravelSystem.init();
        mtravelSystem.dfs1(id, depart, dest, curTime, 0, 0, (1 << (depart - 1)));

        // 可能没有符合限时条件的路线
        auto rt = mtravelSystem.getRoutine();
        if (rt.empty()) {
            QMessageBox::information(this, "SORRY", "there is no eligible route.");
            return;
        }

        mtravelSystem.savePlan(curTime);
    }
    else {
        mtravelSystem.init();
        mtravelSystem.dfs(id, depart, dest, curTime, 0, 0, (1 << (depart - 1)));
        qDebug() << "dfs over!";
        mtravelSystem.savePlan(curTime);
    }

    QString plan = mtravelSystem.outputPlan();
    qDebug() << plan.toUtf8().data();

    // 为该旅客设计一条符合旅行策略的旅行线路并输出
    ui->log_edit->setText(plan);

    // 路线渲染
    auto rt = mtravelSystem.getRoutine();
    mGW->clrRoutine();
    mGW->setRoutine(rt);
}

void Widget::query() {
    int id = ui->id_edit_2->document()->toPlainText().toInt();
    QString ret = mtravelSystem.qState(id, curTime);

    // 系统能查询当前时刻旅客所处的地点和状态（停留城市/所在交通工具）
    ui->log_edit->setText(ret);

    // 路线渲染
    auto tl = mtravelSystem.getTravel(id);
    mGW->clrRoutine();
    mGW->setRoutine(tl);
}


