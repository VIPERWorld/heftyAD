#include "GraphModelEdge.h"

GraphModelEdge::GraphModelEdge(const GraphModelVertex &source, const GraphModelVertex &target,
                               bool bidir,
                               QObject *parent)
    : ModelCurve((const ModelNode&)source, (const ModelNode&)target, bidir, parent)
{
}

GraphModelVertex& GraphModelEdge::vertex1() const {return (GraphModelVertex&) m_node1;}
GraphModelVertex& GraphModelEdge::vertex2() const {return (GraphModelVertex&) m_node2;}

bool GraphModelEdge::startsAt(const GraphModelVertex &source) const
{
    return &m_node1==(ModelNode*)&source || (&m_node2==(ModelNode*)&source && isBidir());
}

bool GraphModelEdge::endsAt(const GraphModelVertex &target) const
{
    return &m_node2==(ModelNode*)&target || (&m_node1==(ModelNode*)&target && isBidir());
}
