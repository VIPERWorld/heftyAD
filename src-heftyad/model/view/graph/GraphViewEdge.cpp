#include "GraphModelEdge.h"
#include "GraphViewEdge.h"

GraphViewEdge::GraphViewEdge(QGraphicsItem *parent)
    : ViewCurve(parent)
{
}

bool GraphViewEdge::acceptModelItem(ModelItem *modelItem)
{
    return modelItem==nullptr || dynamic_cast<GraphModelEdge*>(modelItem);
}
