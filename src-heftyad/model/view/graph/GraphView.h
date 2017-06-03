#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include "View.h"

class GraphModel;
class GraphModelEdge;
class GraphModelVertex;
class GraphViewVertex;

class GraphView : public View
{
    Q_OBJECT

protected:
    GraphModel *m_modelAsGraph;

    bool m_onMousePressedAddNode;
    bool m_onMousePressedAddEdgeTwoByTwo;
        GraphViewVertex* m_twoVerticesLastlyPressed[2];
        int m_lastlyPressedVertexIndex;
    bool m_onMousePressedAddEdgeSuccessively;
    bool m_onMousePressedAddEdgeFromSource;

    bool m_visibleEdgePoints;

public:
    explicit GraphView(QWidget *parent = 0);

    bool acceptModel(Model *model) override;

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void selectVertices();
    void selectEdges();

    void resetLastlyPressedVertices(GraphViewVertex *vertex0 = nullptr, GraphViewVertex *vertex1 = nullptr);

signals:

public slots:
    void removeSelection() override;
    void setEnabledItemMove(bool enabled) override;

    void setVisibleEdgePoints(bool visible);

    void setOnMousePressedAddNode(bool value);
    void setOnMousePressedAddEdgeTwoByTwo(bool value);
    void setOnMousePressedAddEdgeSuccessively(bool value);
    void setOnMousePressedAddEdgeFromSource(bool value);

private slots:
    void onModelChanged() override;

    void onVertexAddedToModel(GraphModelVertex *vertex);
    void onEdgeAddedToModel(GraphModelEdge *edge);
    void onVertexRemovedFromModel();
    void onEdgeRemovedFromModel();
    void onModelCleared();
};

#endif // GRAPHVIEW_H
