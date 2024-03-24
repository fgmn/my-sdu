# covid-19
基于QT的DSA课程设计低风险出行系统，记忆化搜索算法为用户制定最低风险或者是限时最低风险策略的出行方案，通过QGraphicsScene、QGraphicsView进行路线可视化渲染


# 文件说明
Qt 中打开通过 covid-19.pro 项目文件可以打开项目，截取其中项目结构的部分：
```
SOURCES += \
    edge.cpp \                  // 图形组件（线）
    graphwidget.cpp \           // 图形化窗口实现
        main.cpp \              // 主程序
    node.cpp \                  // 图形组件（点）
    travelSystem.cpp \          // 旅行系统的实现（搜索算法等）
        widget.cpp              // 前端显示窗口

HEADERS += \
    city.h \                    // 城市定义
    edge.h \                    // 图形组件（线）
    graphwidget.h \             // 图形化窗口定义，实现动态渲染
    item.h \                    // 车程、航班定义
    node.h \                    // 图形组件（点）
    trace.h \                   // 记忆化轨迹
    travel.h \                  // 出行路线定义（基于item）
    travelSystem.h \            // 旅行系统定义
        widget.h                // 窗口定义

FORMS += \
        widget.ui               // UI界面设计，点击跳转至图形设计界面
```