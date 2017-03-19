#include "GraphModelEdge.h"
#include "GraphModelVertex.h"
#include "GraphViewEdge.h"
#include "GraphViewVertex.h"

#include <QGraphicsScene>

GraphViewVertex::GraphViewVertex(QGraphicsItem *parent)
    : ViewNode(parent)
{
}

bool GraphViewVertex::acceptModelItem(ModelItem *modelItem)
{
    return modelItem==nullptr || dynamic_cast<GraphModelVertex*>(modelItem);
}

QVariant GraphViewVertex::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    auto *vertex = static_cast<GraphModelVertex*>(m_modelItem);

    switch(change) {
        case ItemPositionHasChanged:
            if(vertex) {
                for(QGraphicsItem *item : scene()->items()) {
                    auto *edge = dynamic_cast<GraphViewEdge*>(item);
                    if(edge) {
                        auto *e = static_cast<GraphModelEdge*>(edge->modelItem());
                        if(e) {
                            if(&e->vertex1()==vertex || &e->vertex2()==vertex) {
                                emit e->needAdjustment();
                            }
                        }
                    }
                }

                /*foreach(GraphModelEdge *edge, vertex->graph()->edges(true, true, *vertex)) {
                    emit edge->needAdjustment();
                }*/
            }
            break;

        default:
            break;
    }

    return ViewNode::itemChange(change, value);
}
