#include "GraphModelVertex.h"

GraphModelVertex::GraphModelVertex(QObject *parent)
    : ModelNode(parent)
{
}

QVector<GraphModelVertex*> GraphModelVertex::succs() const {return m_succs;}
