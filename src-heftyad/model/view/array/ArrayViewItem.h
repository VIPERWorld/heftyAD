#ifndef ARRAYVIEWITEM_H
#define ARRAYVIEWITEM_H

#include "ViewMultiShapeItem.h"

class ArrayModel;
class ArrayModelItem;

class ArrayViewItem : public ViewMultiShapeItem
{
protected:
    ArrayModel *m_arrayContainer;
    bool m_drawItemIndexInArray;

public:
    explicit ArrayViewItem(QGraphicsItem *parent = nullptr);

    bool acceptModelItem(ModelItem *modelItem) override;

    void setArrayContainer(ArrayModel *container);
    void setDrawItemIndexInArray(bool value);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void paintModelIndex(QPainter *painter);
};

#endif // ARRAYVIEWITEM_H
