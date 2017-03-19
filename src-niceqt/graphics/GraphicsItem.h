#ifndef GRAPHICSITEM_H
#define GRAPHICSITEM_H

#include <QGraphicsObject>

class GraphicsItem : public QGraphicsObject
{
    Q_OBJECT

public:
    GraphicsItem(QGraphicsItem * parent = 0);

    /**
     * Stack this item above sibling items.
     * A sibling item is an item such that item->parent() == this->parent() is true.
     * This function doesn't change this item z-value: see QGraphicsItem::stackBefore.
     *
     * This function can be called in mousePressEvent for instance,
     * to make sure an item is drawn on top of any other item within the scene.
     */
    void stackAboveSiblings();

    /**
     * Stack this item above scene items: brings this item to foreground.
     * This function simply sets the z value of this item to the highest one:
     *     (max of all items' z-value) + 1.
     *
     * This function can be called in mousePressEvent for instance,
     * to make sure an item is drawn on top of any other item within the scene.
     */
    void stackAboveItems();

signals:
};

#endif // GRAPHICSITEM_H
