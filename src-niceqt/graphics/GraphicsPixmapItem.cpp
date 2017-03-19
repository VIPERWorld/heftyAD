#include "GraphicsPixmapItem.h"

#include <QIcon>
#include <QPainter>

GraphicsPixmapItem::GraphicsPixmapItem(QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
}

QRectF GraphicsPixmapItem::boundingRect(void) const
{
    return QRectF(0, 0, 55, 55);
}

void GraphicsPixmapItem::setPixmapImage(const QString &img)
{
    QRectF r = boundingRect();
    int w = r.width();
    int h = r.height();
    m_pixmap = QIcon(img).pixmap(w, h);
    //update(boundingRect());
}

void GraphicsPixmapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    QRectF r = boundingRect();
    //painter->drawRect(r);
    painter->drawPixmap(r.toRect(), m_pixmap);
}
