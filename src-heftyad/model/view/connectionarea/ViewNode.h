#ifndef VIEWNODE_H
#define VIEWNODE_H

#include "ViewMultiShapeItem.h"

class ViewNode : public ViewMultiShapeItem
{
    Q_OBJECT

protected:
    ViewNode(QGraphicsItem *parent = 0);

public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

signals:

public slots:
};

#endif // VIEWNODE_H
