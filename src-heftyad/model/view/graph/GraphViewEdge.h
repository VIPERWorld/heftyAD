#ifndef GRAPHVIEWEDGE_H
#define GRAPHVIEWEDGE_H

#include "ViewCurve.h"

class GraphViewEdge : public ViewCurve
{
    Q_OBJECT

public:
    explicit GraphViewEdge(QGraphicsItem *parent = 0);

    bool acceptModelItem(ModelItem *modelItem) override;
};

#endif // GRAPHVIEWEDGE_H
