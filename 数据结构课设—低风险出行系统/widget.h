#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QString>
#include <QMessageBox>

#include <QDate>
#include <QTime>
#include <QDateTimeEdit>
#include <QTimer>

#include <QPixmap>
#include <QPainter>
#include "graphwidget.h"

namespace Ui {
    class Widget;   // ui_widget.h文件中定义的类，外部声明
}

class Widget : public QWidget
{
    Q_OBJECT    // 宏，使用信号与槽机制必须添加

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    void paintEvent(QPaintEvent *);     // 渲染&&可视化

    GraphWidget *mGW;
private:
    Ui::Widget *ui;             // 指向界面
    void initUI();              // 初始化界面
    void initConnect();         // 初始化信号槽

private slots:

    void book();    // 订票
    void sumbit();  // 提交
    void query();   // 查询

signals:

};

#endif // WIDGET_H
