#ifndef GRAPHMODELEDGE_H
#define GRAPHMODELEDGE_H

#include "ModelCurve.h"

class GraphModelVertex;

class GraphModelEdge : public ModelCurve
{
    Q_OBJECT

public:
    explicit GraphModelEdge(const GraphModelVertex &node1, const GraphModelVertex &node2,
                            bool bidir = false,
                            QObject *parent = 0);

    GraphModelVertex& vertex1() const;
    GraphModelVertex& vertex2() const;

    bool startsAt(const GraphModelVertex &source) const;
    bool endsAt(const GraphModelVertex &target) const;
};

#endif // GRAPHMODELEDGE_H
