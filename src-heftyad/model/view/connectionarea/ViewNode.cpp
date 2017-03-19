#include "ViewNode.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>

ViewNode::ViewNode(QGraphicsItem *parent)
    : ViewMultiShapeItem(parent)
{
    setCacheMode(DeviceCoordinateCache); // Speed up rendering performance
}

//#include <QGraphicsScene>
void ViewNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    /*bool isSunken = (option->state & QStyle::State_Sunken);
    if(isSunken && scene()->selectedItems().count()==1) {
        qreal k = 10000;

        QLineF verLineAtLeft(boundingRect().topLeft()-QPointF(0, k), boundingRect().bottomLeft()+QPointF(0, k));
        QLineF verLineAtRight(verLineAtLeft);
        verLineAtRight.translate(boundingRect().width(), 0);

        QLineF horLineAtTop(boundingRect().topLeft()-QPointF(k, 0), boundingRect().topRight()+QPointF(k, 0));
        QLineF horLineAtBott(horLineAtTop);
        horLineAtBott.translate(0, boundingRect().height());

        QPen pen(painter->pen());
            pen.setStyle(Qt::DashLine);
            painter->setPen(pen);
        // It doesn't work, why don't the lines have the indicated length?
        painter->drawLine(verLineAtLeft);
        painter->drawLine(verLineAtRight);
        painter->drawLine(horLineAtTop);
        painter->drawLine(horLineAtBott);
    }*/

    /*bool isSunken = (option->state & QStyle::State_Sunken);
    if(isSunken && scene()->selectedItems().count()==1) {
//        QDesktopWidget screen;
//        QRect screenGeom = screen.screenGeometry((QWidget*)MainData::get()->win());
//        int screenWidth = screenGeom.width();
        int screenWidth = 10000;

        int line = screenWidth;
        qreal w = rect().width(), h = rect().height();

        // draw 2 horizontal lines
        painter->drawLine(-line, 0, line, 0);
        painter->drawLine(-line, h, line, h);

        // draw 2 vertical lines
        painter->drawLine(0, -line, 0, line);
        painter->drawLine(w, -line, w, line);
    }*/

    ViewMultiShapeItem::paint(painter, option, widget);
}
