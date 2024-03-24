#include "widget.h"
#include <QApplication>
#include "graphwidget.h"

// Qt对标准C++进行了拓展
// 元对象编译器（Mera-Object Compiler,Moc）是一个预处理器
// 先将Qt的特性程序转换为标准C++程序，再由标准C++编译器进行编译

// QObject类是所有使用元对象系统的基类
// private部分声明Q_OBJECT宏
// MOC为每个QObject子类提供必要代码

/*
QObject *obj = new QMyWidget;
QObject *widget = qobject_cast<QWidget *>(obj);
*/

/*
1. 时间模块
2. 文件操作（初始化、日志）
3. 算法部署
4. 代码裁剪
5. 渲染设计
6. 可视化
*/


int main(int argc, char *argv[])
{
    QApplication a(argc, argv); // 定义并创建应用程序
    Widget w;                   // 定义并创建窗口

    w.show();                   // 显示窗口

    return a.exec();            // 应用程序运行，开始消息循环和事件处理
}
