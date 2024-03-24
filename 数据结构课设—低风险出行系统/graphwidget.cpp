#include "graphwidget.h"
#include "edge.h"
#include "node.h"

#include <math.h>
#include<QDebug>
#include <QKeyEvent>
//#include <QRandomGenerator>


GraphWidget::GraphWidget(QWidget *parent, int cnt, QSize size)
    : QGraphicsView(parent)
{
    myscene = new QGraphicsScene(this);
    myscene->setItemIndexMethod(QGraphicsScene::NoIndex);
    myscene->setSceneRect(-200, -200, 400, 400);

    // 设置窗口属性
    setScene(myscene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(1), qreal(1));
    setMinimumSize(760, 600);

    // 将窗口设置为透明
    setStyleSheet("background: transparent;border:0px");
    setWindowFlags(Qt::FramelessWindowHint);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    // to be fixed
//    Cnt = cnt;
    Cnt = 10;

    m_size = size;
    isWait = false;
}

void GraphWidget::Set(city *nodes) {
    reset();

    for (int i = 1; i <= Cnt; i++) {
        int seq = nodes[i].seq;
        qDebug() << nodes[seq].x << ' ' << nodes[seq].y;

        // 手动设置显示参数
        node[seq] = new Node(this, seq, m_size, nodes[seq].x / 25 - 150, nodes[seq].y / 25 - 180, nodes[seq].r);
        myscene->addItem(node[seq]);
    }
}

void GraphWidget::reset()
{
    myscene->clear();

    for (int i = 0; i <= Cnt; i++)
    {
        for (int j = 0; j <= Cnt; j++)
        {
            edge[i][j] = NULL;
        }
    }
}


void GraphWidget::setRoutine(list<item> rt) {
    for (auto i : rt) {
        int u = i.sCity, v = i.dCity;
        edge[u][v] = new Edge(node[u], node[v]);
        myscene->addItem(edge[u][v]);
    }
}

void GraphWidget::clrRoutine() {
    for (int i = 1; i <= Cnt; i++) {
        for (int j = 1; j <= Cnt; j++) {
            if (edge[i][j] != NULL) {
                myscene->removeItem(edge[i][j]);
            }
        }
    }
}

void GraphWidget::itemMoved()
{
    if (!timerId)
        timerId = startTimer(1000 / 25);
}


void GraphWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        crNode->moveBy(0, -20);
        break;
    case Qt::Key_Down:
        crNode->moveBy(0, 20);
        break;
    case Qt::Key_Left:
        crNode->moveBy(-20, 0);
        break;
    case Qt::Key_Right:
        crNode->moveBy(20, 0);
        break;
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    case Qt::Key_Space:
    case Qt::Key_Enter:
        shuffle();
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

void GraphWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    QVector<Node *> nodes;
    const QList<QGraphicsItem *> items = scene()->items();
    for (QGraphicsItem *item : items) {
        if (Node *node = qgraphicsitem_cast<Node *>(item))
            nodes << node;
    }

    for (Node *node : qAsConst(nodes))
        node->calculateForces();

    bool itemsMoved = false;
    for (Node *node : qAsConst(nodes)) {
        if (node->advancePosition())
            itemsMoved = true;
    }

    if (!itemsMoved) {
        killTimer(timerId);     // 实现一次定时器
        timerId = 0;
    }
}

void GraphWidget::chooseNode(int x)
{
    if(isWait == false)  // 记录第一个点
    {
        fn = x;
        node[fn]->setchoosed(true);
        node[fn]->update();
        isWait = true;
    }
    else // 判断和第一个点的关系
    {
        node[fn]->setchoosed(false);
        node[fn]->update();
        if(x != fn)
        {
            // int maxid = qMax(node_id, firstnode);
            // int minid = qMin(node_id, firstnode);
            // int edge_id = (18 - minid) * (minid + 1) / 2 + maxid - 10;

            qDebug() << fn << ' ' << x;
            qDebug() << edge[fn][x];

            if(edge[fn][x] == NULL)  // 添加
            {
                edge[fn][x] = new Edge(node[fn], node[x]);
                myscene->addItem(edge[fn][x]);
            }
            else
            {   // 移除
                myscene->removeItem(edge[fn][x]);
                edge[fn][x] = NULL;
                node[fn]->setchoosed(false);
            }
        }
        isWait = false;
    }
}

void GraphWidget::addEdge(int a,int b)
{
    edge[a][b] = new Edge(node[a], node[b]);
    myscene->addItem(edge[a][b]);
}


void GraphWidget::wheelEvent(QWheelEvent *event)
{
    scaleView(pow(2., -event->angleDelta().y() / 240.0));
}

void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);
    /*
    // Shadow
    QRectF sceneRect = this->sceneRect();
    QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
    QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
    if (rightShadow.intersects(rect) || rightShadow.contains(rect))
        painter->fillRect(rightShadow, Qt::darkGray);
    if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
        painter->fillRect(bottomShadow, Qt::darkGray);

    // Fill
    QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::lightGray);
    painter->fillRect(rect.intersected(sceneRect), gradient);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(sceneRect);

    // Text
    QRectF textRect(sceneRect.left() + 4, sceneRect.top() + 4,
                    sceneRect.width() - 4, sceneRect.height() - 4);
    QString message(tr("Please select two nodes to connect"));

    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(14);
    painter->setFont(font);
    painter->setPen(Qt::lightGray);
    painter->drawText(textRect.translated(2, 2), message);
    painter->setPen(Qt::black);
    painter->drawText(textRect, message);
    */
}


void GraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}


void GraphWidget::shuffle()
{
    /*
    const QList<QGraphicsItem *> items = scene()->items();
    for (QGraphicsItem *item : items) {
        if (qgraphicsitem_cast<Node *>(item))
            item->setPos(-150 + QRandomGenerator::global()->bounded(300), -150 + QRandomGenerator::global()->bounded(300));
    }
    */
}

void GraphWidget::zoomIn()
{
    scaleView(qreal(1.2));
}

void GraphWidget::zoomOut()
{
    scaleView(1 / qreal(1.2));
}
