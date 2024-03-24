#include "edge.h"
#include "node.h"
#include "graphwidget.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QDebug>

Node::Node(GraphWidget *graphWidget, int node_id, QSize size, qreal xt, qreal yt, int c)
    : graph(graphWidget)    // 设置母窗口
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
    mId = node_id;
    mSize = size;
    isChoosed = false;

//    W = size.width() * 2;
//    H = size.height() * 2;
//    qDebug() << W << H;


//    double num = graph->nodeNumber();
//    double interval = 2 * 3.1416 / num;
//    qreal xt = qSin(interval * m_node_id) * W;
//    qreal yt = qCos(interval * m_node_id) * H;

//    qreal xt = 10 * m_node_id;
//    qreal yt = 10 * m_node_id;
//    qDebug() << xt << yt;

    color = c;

    newPos.setX(xt);
    newPos.setY(yt);
    setPos(xt, yt);     // 设置节点位置,也可以设定newPos通过advancePosition()进行调整
}

void Node::addEdge(Edge *edge)
{
    edgeList << edge;
    edge->adjust();
}

QVector<Edge *> Node::edges() const
{
    return edgeList;
}

// to be fixed
void Node::calculateForces()
{
//    if (!scene() || scene()->mouseGrabberItem() == this) {
//        newPos = pos();
//        return;
//    }

    // 将所有节点固定在一个圆上

    /*
    qreal xvel = 0;
    qreal yvel = 0;

    double interval = 2 * 3.1416 / graph->nodeNumber();

    qreal xt = qSin(interval * m_node_id) * W;
    qreal yt = qCos(interval * m_node_id) * H;
    xvel = (xt - pos().rx()) / 10;
    yvel = (yt - pos().ry()) / 10;

    if (qAbs(xvel) < 0.1 && qAbs(yvel) < 0.1)
        return;

    QRectF sceneRect = scene()->sceneRect();
    newPos = pos() + QPointF(xvel, yvel);
    newPos.setX(qMin(qMax(newPos.x(), sceneRect.left() + 10), sceneRect.right() - 10));
    newPos.setY(qMin(qMax(newPos.y(), sceneRect.top() + 10), sceneRect.bottom() - 10));
    */
}


bool Node::advancePosition()
{
    if (newPos == pos())
        return false;

    setPos(newPos);
    return true;
}

QRectF Node::boundingRect() const
{
    qreal adjust = 2;
    return QRectF( -10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);
}


QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 20, 20);
    return path;
}


void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    // 绘制阴影
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::darkGray);
    painter->drawEllipse(-7, -7, 20, 20);


    // 构造一个简单的径向梯度，中心在(cx, cy)和指定的半径
    QRadialGradient gradient(-3, -3, 10);


    /*
    if (option->state & QStyle::State_Sunken) {
        gradient.setCenter(3, 3);
        gradient.setFocalPoint(3, 3);
        if(!is_choosed)
        {
            gradient.setColorAt(1, QColor(Qt::yellow).lighter(120));
            gradient.setColorAt(0, QColor(Qt::yellow).lighter(120));
        }
        else
        {
            gradient.setColorAt(1, QColor(Qt::green).lighter(120));
            gradient.setColorAt(0, QColor(Qt::green).lighter(120));
        }
    }
    else {
        // 节点颜色设置
        if(!is_choosed)
        {
            gradient.setColorAt(0, Qt::yellow);
            gradient.setColorAt(1, Qt::yellow);
        }
        else
        {
            gradient.setColorAt(0, Qt::green);
            gradient.setColorAt(1, Qt::green);
        }
    }
    */

    if (isChoosed) {
        gradient.setColorAt(0, Qt::green);
        gradient.setColorAt(1, Qt::green);
    }
    else if (color == 0) {
        gradient.setColorAt(0, Qt::yellow);
        gradient.setColorAt(1, Qt::yellow);
    }
    else if (color == 1) {
        gradient.setColorAt(0, Qt::blue);
        gradient.setColorAt(1, Qt::blue);
    }
    else if (color == 2) {
        gradient.setColorAt(0, Qt::red);
        gradient.setColorAt(1, Qt::red);
    }



    painter->setBrush(gradient);

    painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse(-10, -10, 20, 20);
    painter->drawText(QPointF(-4,4),QString::number(mId));
}


QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:    // 节点位置改变
        for (Edge *edge : qAsConst(edgeList))
            edge->adjust();
        graph->itemMoved();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}


void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    graph->chooseNode(mId);
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

void Node::setchoosed(bool choosed)
{
    isChoosed = choosed;
}
