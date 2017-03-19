#include "GraphicsItem.h"

#include <QGraphicsScene>

GraphicsItem::GraphicsItem(QGraphicsItem *parent)
    : QGraphicsObject(parent)
{
    setFlags(ItemIsMovable|ItemIsSelectable);
}

void GraphicsItem::stackAboveSiblings()
{
    /*
     * This works but I'm not sure I understand how Qt deals with item stacking
     * when selection goes from an item to another one.
     *
     * In the for-loop below there is no need to check whether item != this.
     * If we did, that condition will be tested at each iteration.
     */

    for(QGraphicsItem *item : scene()->items()) {
        if(item->parentItem() == parentItem()) {
            item->stackBefore(this);
        }
    }
}

void GraphicsItem::stackAboveItems()
{
    /*
     * In the for-loop below there is no need to check whether item != this.
     * If we did, that condition will be tested at each iteration.
     */

    auto maxZValue = -1.;
    for(QGraphicsItem *item : scene()->items()) {
        if(item->zValue() > maxZValue) {
            maxZValue = item->zValue();
        }
    }

    setZValue(maxZValue+1);
}
