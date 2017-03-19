#ifndef GRAPHVIEWVERTEX_H
#define GRAPHVIEWVERTEX_H

#include "ViewNode.h"

class GraphViewVertex : public ViewNode
{
    Q_OBJECT

public:
    explicit GraphViewVertex(QGraphicsItem *parent = 0);

    bool acceptModelItem(ModelItem *modelItem) override;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
};

#endif // GRAPHVIEWVERTEX_H
