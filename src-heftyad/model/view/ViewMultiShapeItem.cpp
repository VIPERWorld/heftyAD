#include "ModelMultiShapeItem.h"
#include "ViewMultiShapeItem.h"

#include <QPainter>

ViewMultiShapeItem::ViewMultiShapeItem(QGraphicsItem *parent)
    : ViewItem(parent)
{
}

bool ViewMultiShapeItem::acceptModelItem(ModelItem *modelItem)
{
    return modelItem==nullptr || dynamic_cast<ModelMultiShapeItem*>(modelItem);
}

void ViewMultiShapeItem::onModelItemChanged()
{
    auto *item = static_cast<ModelMultiShapeItem*>(m_modelItem);
    if(item) {
        connect(item, &ModelMultiShapeItem::shapeKindChanged, this, &ViewMultiShapeItem::onShapeKindChanged);
        connect(item, &ModelMultiShapeItem::roundedChanged,   this, &ViewMultiShapeItem::onRoundedChanged);
        connect(item, &ModelMultiShapeItem::dimensionChanged, this, &ViewMultiShapeItem::onDimensionChanged);
    }

    ViewItem::onModelItemChanged();
}

void ViewMultiShapeItem::onShapeKindChanged() {update();}
void ViewMultiShapeItem::onRoundedChanged()   {update();}
void ViewMultiShapeItem::onDimensionChanged() {update();}

void ViewMultiShapeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    preparePainting(painter);

    const QRectF &r = rect();
    const auto *item = static_cast<ModelMultiShapeItem*>(m_modelItem);
    if(item) {
        switch(item->shapeKind()) {
        case ModelMultiShapeItem::Circle:
            painter->drawEllipse(r);
            break;

        case ModelMultiShapeItem::Rectangle:
            item->isRounded() ? painter->drawRoundedRect(r, 7., 7.) : painter->drawRect(r);
            break;
        }
    }
    else {
        painter->drawRect(r);
    }

    /*
     * We don't draw the pixmap in ViewItem::paint since we don't want the pixmap to show for curves (for instance).
     * Nevertheless curves draw pixmap, due to the call to the drawPath function.
     */
    const QPixmap &pixmap(painter->brush().texture());
    if(!pixmap.isNull()) {
        painter->drawPixmap(boundingRect().toRect(), pixmap);
    }

    ViewItem::paint(painter, option, widget);
}

void ViewMultiShapeItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    stackAboveItems();
    ViewItem::mousePressEvent(event);
}
