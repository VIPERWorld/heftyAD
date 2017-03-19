#ifndef GRAPHICSTEXTITEM_H
#define GRAPHICSTEXTITEM_H

#include <QGraphicsTextItem>

/**
 * /!\ The GraphicsTextItem class doesn't inherits QGraphicsSimpleTextItem
 *     since it provides no way to edit its text (through keyboard).
 */
class GraphicsTextItem : public QGraphicsTextItem
{
    Q_OBJECT

public:
    explicit GraphicsTextItem(QGraphicsItem * parent = 0);

signals:

public slots:
};

#endif // GRAPHICSTEXTITEM_H
