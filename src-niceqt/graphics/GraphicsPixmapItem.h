#ifndef GRAPHICSPIXMAPITEM_H
#define GRAPHICSPIXMAPITEM_H

#include <QGraphicsItem>

class GraphicsPixmapItem : public QGraphicsItem
{
protected:
    QPixmap m_pixmap;

public:
    explicit GraphicsPixmapItem(QGraphicsItem *parent = 0);

    QRectF boundingRect(void) const override;

    void setPixmapImage(const QString &img);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};

#endif // GRAPHICSPIXMAPITEM_H
