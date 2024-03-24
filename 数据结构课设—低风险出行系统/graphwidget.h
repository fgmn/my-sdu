#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>
#include<city.h>

class Node;
class Edge;

//! [0]
class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:
    GraphWidget(QWidget *parent = nullptr, int cnt = 10, QSize size = QSize(1080, 600));  // 600,480
    void itemMoved(); // 定时检测节点是否移动
    int getCnt() { return Cnt; }
    void chooseNode(int); // 增删边

    void Set(city*);
    void reset();  // 刷新

    void setRoutine(list<item> rt);     // 当前旅客的计划路线或剩余路线渲染
    void clrRoutine();

    void addEdge(int, int); // 加边
    Edge *edge[15][15];

public slots:
    void shuffle();
    void zoomIn();
    void zoomOut();

protected:
    void keyPressEvent(QKeyEvent *event) override;      // 键盘输入事件
    void timerEvent(QTimerEvent *event) override;       // 设定一次定时器

    void wheelEvent(QWheelEvent *event) override;       // 鼠标滚轮事件

    void drawBackground(QPainter *painter, const QRectF &rect) override;

    void scaleView(qreal scaleFactor);    // 视图大小调整


private:
    QGraphicsScene *myscene;        // 平面，用于承载二维对象
    int timerId = 0;
    Node *node[15];     // 节点对象指针

    Node *crNode;
    int Cnt;            // 节点数
    QSize m_size;

    bool isWait;        // 是否已有选中点等待边操作
    int fn;             // 记录选中的第1个点
};
//! [0]

#endif // GRAPHWIDGET_H
