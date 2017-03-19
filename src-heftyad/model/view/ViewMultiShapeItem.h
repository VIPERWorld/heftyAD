#ifndef VIEWMULTISHAPEITEM_H
#define VIEWMULTISHAPEITEM_H

#include "ViewItem.h"

class ViewMultiShapeItem : public ViewItem
{
    Q_OBJECT

public:
    explicit ViewMultiShapeItem(QGraphicsItem *parent = 0);

    bool acceptModelItem(ModelItem *modelItem) override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

signals:

public slots:

protected slots:
    void onModelItemChanged() override;

    void onShapeKindChanged();
    void onRoundedChanged();
    void onDimensionChanged();
};

#endif // VIEWMULTISHAPEITEM_H
