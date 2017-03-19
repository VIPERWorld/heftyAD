#ifndef GRAPHMODELVERTEX_H
#define GRAPHMODELVERTEX_H

#include "ModelNode.h"

class GraphModelVertex : public ModelNode
{
    Q_OBJECT

    friend class GraphModel; // to be able to access:
                             //     m_succs
                             //     m_indexInContainer

private:
    QVector<GraphModelVertex*> m_succs;

public:
    explicit GraphModelVertex(QObject *parent = 0);

    QVector<GraphModelVertex*> succs() const;

signals:

public slots:
};

#endif // GRAPHMODELVERTEX_H
