#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QVector>
#include <qmath.h>
class Edge;
class GraphWidget;

//! [0]
class Node : public QGraphicsItem
{
public:
    Node(GraphWidget *graphWidget, int node_id, QSize size, qreal xt, qreal yt, int c);

    void addEdge(Edge *edge);
    QVector<Edge *> edges() const;

    enum { Type = UserType + 1 };
    int type() const override { return Type; }

    void calculateForces();         // 拖拽节点可以动态返回原位置
    bool advancePosition();         // 判断节点位置是否更改

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    // 作为QGraphicsItem的子类，加入QGraphicsScene后会自行调用
    void setchoosed(bool choosed);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;


private:
    QVector<Edge *> edgeList;   // 邻接边
    QPointF newPos;             // 描述点在平面的位置关系
    GraphWidget *graph;         // 描述母窗口
    int mId;                    // 节点编号
    QSize mSize;                // 描述二维对象的大小
    bool isChoosed;             // 是否被选中
    int color;
    int W;
    int H;
};
//! [0]

#endif // NODE_H

