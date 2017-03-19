#include "SectionedWidgetEltBar.h"

#include <QPainter>

SectionedWidgetEltBar::SectionedWidgetEltBar(QWidget *parent)
    : QPushButton(parent),
      m_widgetIsUnrolled(false),
      m_indicatorBrush(Qt::gray),
      m_indicatorPen(Qt::NoPen) // No stroke
{
    setCursor(Qt::PointingHandCursor);

    /*
     * Some examples of style sheet that can be set for this widget.
     * But of course you're not limited to those.
     */
    //setStyleSheet("background-color: rgb(105,124,92); font-weight: bold;");
    //setStyleSheet("background-color: rgb(83,103,93); font-weight: bold;");
}

void SectionedWidgetEltBar::setIndicatorBrush(const QBrush &brush) {m_indicatorBrush = brush;}
void SectionedWidgetEltBar::setIndicatorPen(const QPen &pen) {m_indicatorPen = pen;}

void SectionedWidgetEltBar::drawTriangle(QPainter *painter, const QPointF &A, const QPointF &B, const QPointF &C)
{
    const QPointF points[3] = {A, B, C};
    painter->drawConvexPolygon(points, 3);
}

void SectionedWidgetEltBar::drawTriangle(QPainter *painter, const QList<QPointF> &ABC)
{
    drawTriangle(painter, ABC[0], ABC[1], ABC[2]);
}

QList<QPointF> SectionedWidgetEltBar::getTriangle(const QRectF &rect, TriangleDirection dir)
{
    const QPointF &tl = rect.topLeft();
    const QPointF &tr = rect.topRight();
    const QPointF &br = rect.bottomRight();
    const QPointF &bl = rect.bottomLeft();

    switch(dir) {
    case UP:     return QList<QPointF>() << bl << br << (tl+tr)/2;
    case BOTTOM: return QList<QPointF>() << tl << tr << (bl+br)/2;

    case RIGHT:  return QList<QPointF>() << tl << bl << (tr+br)/2;
    case LEFT:   return QList<QPointF>() << tr << br << (tl+bl)/2;
    }

    return QList<QPointF>() << tl << tl << tl; // reached only when a wrong direction is given
}

void SectionedWidgetEltBar::paintEvent(QPaintEvent *event)
{
    QPushButton::paintEvent(event);

    const qreal space = 3.0;
    const qreal len = height()-2*space;
    const QRectF rectL = QRectF(space, space, len, len);
    const QRectF rectR = QRectF(width()-space-len, rectL.y(), len, len);

    QPainter painter(this);
    painter.setPen(m_indicatorPen);
    painter.setBrush(m_indicatorBrush);

    if(m_widgetIsUnrolled) {
        drawTriangle(&painter, getTriangle(rectL, BOTTOM));
        drawTriangle(&painter, getTriangle(rectR, BOTTOM));
    }
    else {
        drawTriangle(&painter, getTriangle(rectL, RIGHT));
        drawTriangle(&painter, getTriangle(rectR, LEFT));
    }
}
